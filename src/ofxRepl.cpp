/*
 * Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 *
 * A rewrite of the Fluxus Repl http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#include "ofxRepl.h"

#include "Unicode.h"

#define MAX_TEXT_LINES	256
#define MAX_HISTORY_LEN	256

// utils
bool isEmpty(u32string s);

std::u32string ofxRepl::s_banner = U"";
std::u32string ofxRepl::s_prompt = U"> ";

//--------------------------------------------------------------
ofxRepl::ofxRepl() : ofxEditor() {
	m_listener = NULL;
	m_promptPos = 0;
	m_selectAllStartPos = 0;
	m_insertPos = 0;
	m_historyNavStarted = false;
	m_linePos = 0;
}

//--------------------------------------------------------------
ofxRepl::ofxRepl(ofxEditorSettings &sharedSettings) : ofxEditor(sharedSettings) {
	m_listener = NULL;
	m_promptPos = 0;
	m_selectAllStartPos = 0;
	m_insertPos = 0;
	m_historyNavStarted = false;
	m_linePos = 0;
}

//--------------------------------------------------------------
ofxRepl::~ofxRepl() {
	ofLogToConsole();
}

//--------------------------------------------------------------
void ofxRepl::setup() {

	// setup our custom logger
	m_logger = std::shared_ptr<Logger>(new Logger);
	m_logger->m_parent = this;
	ofSetLoggerChannel(m_logger);

	// print greeting and first prompt
	if(s_banner != U"") {
		resize();
		print(s_banner);
	}
	printPrompt();
}

//--------------------------------------------------------------
void ofxRepl::setup(ofxReplListener *listener) {
	setup();
	m_listener = listener;
}

//--------------------------------------------------------------
void ofxRepl::keyPressed(int key) {
	
	// filter out modifier key events, except SHIFT
	switch(key) {
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
		case OF_KEY_SUPER: case OF_KEY_LEFT_SUPER: case OF_KEY_RIGHT_SUPER:
			return;
	}
	bool copying = false;
	
	// check modifier keys
	bool modifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	if(modifierPressed) {
		switch(key) {
			case 'a': case 10: // select all / clear all text
				if(ofGetKeyPressed(OF_KEY_SHIFT)) {
					clearText();
				}
				else { // select all
					m_selection = ALL;
					m_highlightStart = m_promptPos;
					m_highlightEnd = m_text.size();
					m_position = m_text.size();
				}
				keepCursorVisible();
				return;
			case 'c': case 3: // clear
				if(ofGetKeyPressed(OF_KEY_SHIFT)) {
					clearHistory();
					return;
				}
				copying = true;
				break;
			case 'v': case 22: // only paste on the current prompt line
				if(m_position < m_promptPos) {
					m_position = m_text.length();
					m_selection = NONE;
					keepCursorVisible();
				}
				break;
			case 'x': case 24: // convert cut into copy
				if(m_position < m_promptPos || m_highlightStart < m_promptPos || m_highlightEnd < m_promptPos) {
					key = 'c';
				}
				copying = true;
				break;
		}
	}
	
	// swallow text buffer removal
	if((m_position <= m_promptPos && key == OF_KEY_BACKSPACE) ||
	   (m_position < m_promptPos && key == OF_KEY_DEL)) {
		return;
	}
	
	// any text input moves cursor to end of the prompt line
	if(isalnum(key) && m_position < m_promptPos && !copying) {
		m_position = m_text.length();
		keepCursorVisible();
	}
	
	// prompt line history etc
	if(m_position >= m_promptPos) {
		switch(key) {
			case OF_KEY_UP:
				historyPrev();
				m_selection = NONE;
				return;
			case OF_KEY_DOWN:
				historyNext();
				m_selection = NONE;
				return;
			case OF_KEY_END:
				m_position = m_text.length();
				m_selection = NONE;
				return;
			case OF_KEY_HOME:
				m_position = m_promptPos;
				m_selection = NONE;
				return;
		}
	}

	if(key == OF_KEY_RETURN) {
		m_position = m_text.length();
		m_selection = NONE;
		eval();
		return;
    }

	ofxEditor::keyPressed(key);
}

//--------------------------------------------------------------
void ofxRepl::print(const std::u32string &what, bool beforePrompt) {

	// trim half of text if we overflow the max num of lines
	if(m_numLines > MAX_TEXT_LINES) {
		int line = (int)m_numLines*0.25;
		int count = 0;
		int pos = 0;
		for(unsigned int i = 0; count <= line && i < m_text.size(); i++) {
			if(m_text[i] == '\n') {
				count++;
				pos = i+1;
			}
		}
		m_text.erase(0, pos);
		textBufferUpdated();
		m_position = m_promptPos = m_insertPos = m_text.size();
		m_topTextPosition -= pos;
		m_bottomTextPosition -= pos;
	}

	std::u32string to_print;
	for(std::u32string::const_iterator i = what.begin(); i != what.end(); ++i) {
		m_linePos++;
		if(*i == '\n') {
			m_linePos = 0;
		}
		to_print += *i;
	}
	
	if(beforePrompt) {
		m_text.insert(MAX(0, m_promptPos-s_prompt.length()), to_print);
		m_position += to_print.length();
		m_promptPos += to_print.length();
		m_insertPos += to_print.length();
		m_selectAllStartPos += to_print.length();
		if(m_selection != NONE) {
			m_highlightStart += to_print.length();
			m_highlightEnd += to_print.length();
		}
	}
	else {
		insertText(to_print);
		m_promptPos = m_insertPos;
		m_promptPos = m_position+s_prompt.length();
		m_selectAllStartPos = m_position;
		m_highlightStart = m_position;
		m_highlightEnd = m_position;
	}
	
	keepCursorVisible();
}

//--------------------------------------------------------------
void ofxRepl::print(const std::string &what, bool beforePrompt) {
	print(string_to_wstring(what), beforePrompt);
}

//--------------------------------------------------------------
void ofxRepl::printEvalReturn(const std::u32string &what) {
	if(what.size() > 0) {
		print(what+U"\n");
	}
	printPrompt();
}

//--------------------------------------------------------------
void ofxRepl::printEvalReturn(const std::string &what) {
	printEvalReturn(string_to_wstring(what));
}

//--------------------------------------------------------------
void ofxRepl::clearText() {
	ofxEditor::clearText();
	m_promptPos = 0;
	m_selectAllStartPos = 0;
	m_insertPos = 0;
	printPrompt();
}

//--------------------------------------------------------------
void ofxRepl::clearHistory() {
	historyClear();
}

//--------------------------------------------------------------
bool ofxRepl::openFile(std::string filename) {
	ofLogWarning("ofxRepl") << "ignoring openFile";
	return false;
}

//--------------------------------------------------------------
bool ofxRepl::saveFile(string filename) {
	ofLogWarning("ofxRepl") << "ignoring saveFile";
	return false;
}

// PROTECTED

//--------------------------------------------------------------
void ofxRepl::eval() {
	if(m_promptPos < m_text.size()) {
		u32string defun = m_text.substr(m_promptPos);
		if(!isEmpty(defun)) {
			m_insertPos = m_text.length();
			print(U"\n");
			
			m_evalText = defun;
			if(m_listener) {
				m_listener->evalReplEvent(wstring_to_string(m_evalText));
			}
			else {
				ofLogWarning("ofxRepl") << "listener not set";
			}

			if(defun[defun.length()-1] == '\n') {
				defun.resize(defun.length()-1, 0);
			}
			if(m_history.size() >= MAX_HISTORY_LEN) {
				m_history.pop_front();
				m_historyIter--;
			}
			m_history.push_back(defun);
			m_historyNavStarted = false;
		
			// go to next line in case the listener isn't set
			if(!m_listener) {
				printPrompt();
			}
		}
	}
}

//--------------------------------------------------------------
void ofxRepl::printPrompt() {
	m_insertPos = m_text.length();
	if(m_text.length() > 0 && m_text[m_insertPos-1] != '\n') {
		m_text += '\n';
		m_insertPos++;
		m_position++;
	}
	insertText(s_prompt);
	m_promptPos = m_selectAllStartPos = m_position;
	clearUndo(); // only allow undo actions on the current command line
}

//--------------------------------------------------------------
void ofxRepl::historyClear() {
	m_historyNavStarted = false;
	m_history.clear();
	m_insertPos = 0;
	m_historyIter = m_history.end();
	m_historyPresent = m_text.substr(m_promptPos);
}

//--------------------------------------------------------------
void ofxRepl::historyPrev() {
	
	if(!m_historyNavStarted) {
		m_historyIter = m_history.end();
		m_historyNavStarted = true;
	}

	if(m_historyIter == m_history.end()) {
		m_historyPresent = m_text.substr(m_promptPos);
	}

	if(m_historyIter == m_history.begin()) {
		return;
	}

	m_historyIter--;
	historyShow(*m_historyIter);
}

//--------------------------------------------------------------
void ofxRepl::historyNext() {
	if(!m_historyNavStarted || (m_historyIter == m_history.end())) {
		return;
	}
	m_historyIter++;
	historyShow((m_historyIter == m_history.end()) ? m_historyPresent : *m_historyIter);
}

//--------------------------------------------------------------
void ofxRepl::historyShow(std::u32string what) {
	m_text.resize(m_promptPos, 0);
	m_text += what;
	m_position = m_text.length();
}

//--------------------------------------------------------------
void ofxRepl::keepCursorVisible() {

	// count num lines from visible top to current pos
	int curVisLine = 0;
	size_t step = 0;
	for(unsigned int i = m_topTextPosition;
	    i < m_position && (step = m_text.find('\n', i)) != std::u32string::npos;
		i = step+1) {
		curVisLine++;
	}
	
	// move top down until pos is on a visible line
	while(curVisLine >= m_visibleLines) {
		if(m_text[m_topTextPosition++] == '\n') {
			curVisLine--;
		}
	}
}

// STATIC UTILS

//--------------------------------------------------------------
void ofxRepl::setReplBanner(const std::u32string &text) {
	s_banner = text;
}

//--------------------------------------------------------------
void ofxRepl::setReplBanner(const std::string &text) {
	s_banner = string_to_wstring(text);
}

//--------------------------------------------------------------
std::u32string& ofxRepl::getWideReplBanner() {
	return s_banner;
}

//--------------------------------------------------------------
std::string ofxRepl::getReplBanner() {
	return wstring_to_string(s_banner);
}
	
//--------------------------------------------------------------
void ofxRepl::setReplPrompt(const std::u32string &text) {
	s_prompt = text;
}

//--------------------------------------------------------------
void ofxRepl::setReplPrompt(const std::string &text) {
	s_prompt = string_to_wstring(text);
}

//--------------------------------------------------------------
std::u32string& ofxRepl::getWideReplPrompt() {
	return s_prompt;
}

//--------------------------------------------------------------
std::string ofxRepl::getReplPrompt() {
	return wstring_to_string(s_prompt);
}

// PRIVATE

//--------------------------------------------------------------
void ofxRepl::Logger::log(ofLogLevel level, const std::string & module, const std::string & message){
	ofConsoleLoggerChannel::log(level, module, message);
	if(level >= ofGetLogLevel()) {
		m_parent->print(string_to_wstring(message)+U"\n", true);
	}
}

// OTHER UTIL

//--------------------------------------------------------------
bool isEmpty(u32string s) {
    const std::u32string ws = U" \t\n\r";
	for(std::u32string::iterator i = s.begin(); i != s.end(); i++) {
		if(ws.find(*i) == std::string::npos) {
			return false;
		}
	}
	return true;
}
