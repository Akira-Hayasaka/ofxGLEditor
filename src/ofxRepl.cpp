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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/danomatika/ofxEditor for more info.
 *
 * Based on a rewrite of the Fluxus Repl in ofxGLEditor,
 * http://www.pawfal.org/fluxus Copyright (C) Dave Griffiths
 */
#include "ofxRepl.h"

// utils
bool isBalanced(string s);
bool isEmpty(string s);

string ofxRepl::s_banner = string("");
string ofxRepl::s_prompt = string("> ");

//--------------------------------------------------------------
ofxRepl::ofxRepl() : ofxEditor() {
	
	m_listener = NULL;
	m_promptPos = 0;
	m_insertPos = 0;
	m_historyNavStarted = false;
	m_linePos = 0;
	
	// setup our custom logger
	m_logger = ofPtr<Logger>(new Logger);
	m_logger->m_parent = this;
	ofSetLoggerChannel(m_logger);
	
	if(s_banner != "") {
		resize();
		print(s_banner);
	}
	printPrompt();
}

//--------------------------------------------------------------
ofxRepl::ofxRepl(ofxEditorSettings &sharedSettings) : ofxEditor(sharedSettings) {
	
	m_listener = NULL;
	m_promptPos = 0;
	m_insertPos = 0;
	m_historyNavStarted = false;
	m_linePos = 0;
	
	// setup our custom logger
	m_logger = ofPtr<Logger>(new Logger);
	m_logger->m_parent = this;
	ofSetLoggerChannel(m_logger);
	
	if(s_banner != "") {
		resize();
		print(s_banner);
	}
	printPrompt();
}

//--------------------------------------------------------------
ofxRepl::~ofxRepl() {
	ofLogToConsole();
}

//--------------------------------------------------------------
void ofxRepl::setListener(ofxReplListener *listener) {
	m_listener = listener;
}

//--------------------------------------------------------------
void ofxRepl::keyPressed(int key) {
	
	// check modifier keys
	bool modifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	if(modifierPressed) {
		switch(key) {
			case 'c': case 3:
				historyClear();
				return;
		}
	}
	
	// swallow text buffer removal
	if((m_position <= m_promptPos && key == OF_KEY_BACKSPACE) ||
		(m_position < m_promptPos && key == OF_KEY_DEL) ||
		((m_position < m_promptPos || m_highlightStart < m_promptPos || m_highlightEnd < m_promptPos) &&
		(modifierPressed && (key == 'x' || key == 24)))) {
		return;
	}

	if(m_position < m_promptPos) {
		m_position = m_text.length();
	}
	else {
		switch(key) {
			case OF_KEY_UP:
				historyPrev();
				return;
			case OF_KEY_DOWN:
				historyNext();
				return;
			case OF_KEY_END:
				m_position = m_text.length();
				return;
			case OF_KEY_HOME:
				m_position = m_promptPos;
				return;
		}
	}
		
	keepCursorVisible();

	if(key == OF_KEY_RETURN) {
		m_position = m_text.length();
        eval();
		return;
    }

	ofxEditor::keyPressed(key);
}

//--------------------------------------------------------------
void ofxRepl::print(const string &what) {

	string to_print;
	for(string::const_iterator i = what.begin(); i != what.end(); ++i) {
		m_linePos++;
		if(*i == '\n') {
			m_linePos = 0;
		}
		to_print += *i;
	}
	m_text.insert(m_insertPos, to_print);
		
	m_position += to_print.length();
	m_promptPos += to_print.length();
	m_insertPos += to_print.length();
	
	keepCursorVisible();
}

//--------------------------------------------------------------
void ofxRepl::printEvalReturn(const string &what) {
	if(what.size() > 0) {
		print(what);
		print("\n");
	}
	printPrompt();
}

//--------------------------------------------------------------
void ofxRepl::clear() {
	clearText();
	m_promptPos = 0;
	m_insertPos = 0;
	printPrompt();
}

//--------------------------------------------------------------
void ofxRepl::clearHistory() {
	historyClear();
}

// PROTECTED

//--------------------------------------------------------------
void ofxRepl::eval() {
	if(m_promptPos < m_text.size()) {
		string defun = m_text.substr(m_promptPos);
		if(!isEmpty(defun)) {
			m_insertPos = m_text.length();
			print("\n");
			
			m_evalText = defun;
			if(m_listener) {
				m_listener->evalReplEvent(m_evalText);
			}
			else {
				ofLogWarning("ofxRepl") << "listener not set";
			}

			if(defun[defun.length()-1] == '\n') {
				defun.resize(defun.length()-1, 0);
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
	if(m_text[m_insertPos-1] != '\n') {
		m_text += '\n';
		m_insertPos++;
	}
	m_text += s_prompt;
	m_position = m_promptPos = m_text.length();
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
void ofxRepl::historyShow(string what) {
	m_text.resize(m_promptPos, 0);
	m_text += what;
	m_position = m_text.length();
}

//--------------------------------------------------------------
void ofxRepl::keepCursorVisible() {
	unsigned int curVisLine = 0;
	for(int i = m_position; i > m_topTextPosition; i--) {
		if (m_text[i] == '\n') {
			curVisLine++;
		}
	}
	while(curVisLine >= m_visibleLines) {
		if(m_text[m_topTextPosition++] == '\n') {
			curVisLine--;
		}
	}
}

// UTIL

//--------------------------------------------------------------
void ofxRepl::setReplBanner(const string &text) {
	s_banner = text;
}

//--------------------------------------------------------------
string ofxRepl::getReplBanner() {
	return s_banner;
}
	
//--------------------------------------------------------------
void ofxRepl::setReplPrompt(const string &text) {
	s_prompt = text;
}

//--------------------------------------------------------------
string ofxRepl::getReplPrompt() {
	return s_prompt;
}

// PRIVATE

//--------------------------------------------------------------
void ofxRepl::Logger::log(ofLogLevel level, const string & module, const string & message){
	ofConsoleLoggerChannel::log(level, module, message);
	m_parent->print(message);
	m_parent->print("\n");
}

//--------------------------------------------------------------
void ofxRepl::Logger::log(ofLogLevel level, const string & module, const char* format, va_list args){
	ofConsoleLoggerChannel::log(level, module, format, args);
	m_parent->print(ofVAArgsToString(format, args));
	m_parent->print("\n");
}

// OTHER UTIL

//--------------------------------------------------------------
bool isBalanced(string s) {
	int balance = 0;
	for(string::iterator i = s.begin(); i != s.end(); i++) {
		switch(*i) {
			case '(':
				balance++;
				break;
			case ')':
				balance--;
				break;
		}
		if(balance < 0) {
			return false;
		}
	}
	return balance == 0;
}

//--------------------------------------------------------------
bool isEmpty(string s) {
    const string ws = " \t\n\r";
	for(string::iterator i = s.begin(); i != s.end(); i++) {
		if(ws.find(*i) == string::npos) {
			return false;
		}
	}
	return true;
}
