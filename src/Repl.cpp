/*
 * Copyright (C) 2012 Akira Hayasaka & Dan Wilcox <danomatika@gmail.com> 
 *
 * Adapted from the fluxus Repl class,
 * Copyright (C) 2005 Dave Griffiths
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
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 *
 */
#include "Repl.h"

#ifndef __APPLE__
	#include <GL/glut.h>
#else
	#include <GLUT/glut.h>
#endif

#include "Unicode.h"
#include <wchar.h>

// utils
bool isBalanced(wstring s);
bool isEmpty(wstring s);

wstring Repl::s_banner = wstring(L"");
wstring Repl::s_prompt = wstring(L"> ");
unsigned int Repl::MAX_LINE_LENGTH = 80;

//--------------------------------------------------------------
Repl::Repl(ofxGLEditor *parent) : m_InsertPos(0), m_History(),
	m_HistoryNavStarted(false), m_LinePos(0), m_Parent(parent) {
	
	// setup our custom logger
	m_Logger = ofPtr<Logger>(new Logger);
	m_Logger->m_Parent = this;
	ofSetLoggerChannel(m_Logger);
	
	//if(s_banner != L"") {
		print(s_banner);
	//}
	printPrompt();
}

//--------------------------------------------------------------
Repl::~Repl() {
	ofLogToConsole();
}

//--------------------------------------------------------------
void Repl::Handle(int button, int key, int special, int state, int x, int y, int mod) {
	
	if(mod & GLUT_ACTIVE_CTRL && state) {
		switch(key) {
			case 3: historyClear(); break; // CTL+C
		}
	}
	
	if(key != 0) {
		if((m_Position <= m_PromptPos && key == GLEDITOR_BACKSPACE) ||
			(m_Position < m_PromptPos && key == GLEDITOR_DELETE) ||
			((m_Position < m_PromptPos ||
			m_HighlightStart < m_PromptPos ||
			m_HighlightEnd < m_PromptPos)
			&& key == GLEDITOR_CUT)) {
			return;
		}

		if(m_Position < m_PromptPos && key != GLEDITOR_COPY) {
			m_Position = m_Text.length();
		}
	}
	
	if(special != 0) {
		if(m_Position >= m_PromptPos) {
			switch(special) {
				case GLUT_KEY_UP:
					historyPrev();
					return;
				case GLUT_KEY_DOWN:
					historyNext();
					return;
				case GLUT_KEY_END:
					m_Position = m_Text.length();
					return;
				case GLUT_KEY_HOME:
					m_Position = m_PromptPos;
					return;
			}
		}
	}
		
	ensureCursorVisible();

	if(key == GLEDITOR_RETURN) {
		m_Position = m_Text.length();
        eval();
		return;
    }

    GLEditor::Handle(button, key, special, state, x, y, mod);
}

//--------------------------------------------------------------
/// TODO: wrap long lines?
void Repl::print(const wstring &what)
{
	int l = what.length();
	
	int slop = l;
	int end = slop;
	int start = 0;
	
	wstring to_print;
	
	for(wstring::const_iterator i = what.begin(); i != what.end(); ++i) {
		m_LinePos++;
		if(*i == L'\n') {
			m_LinePos = 0;
		}
		if(m_LinePos >= MAX_LINE_LENGTH) {
			to_print += L"\n";
			m_LinePos = 0;
		}
		to_print += *i;
	}
	
	m_Text.insert(m_InsertPos, to_print);
		
	m_Position += to_print.length();
	m_PromptPos += to_print.length();
	m_InsertPos += to_print.length();
	
// printing is done in custom logger clss, but keep this here for now
//	#ifdef WIN32
//		cout << wstring_to_string(to_print);
//	#else
//		wcout << to_print;
//	#endif
	ensureCursorVisible();
}

//--------------------------------------------------------------
void Repl::printEvalReturn(const wstring &what) {
	if(what.size() > 0) {
		print(what);
		print(L"\n");
	}
	printPrompt();
}

void Repl::clear() {
	ClearAllText();
	m_PromptPos = 0;
	m_InsertPos = 0;
	printPrompt();
}

void Repl::clearHistory() {
	historyClear();
}

// PRIVATE

//--------------------------------------------------------------
void Repl::eval() {
	if(m_PromptPos < m_Text.size()) {
		wstring defun = m_Text.substr(m_PromptPos);
		if(!isEmpty(defun)) {
			m_InsertPos = m_Text.length();
			print(L"\n");
			
			evalText = wstring_to_string(defun);
			ofNotifyEvent(m_Parent->evalReplEvent, evalText, this);

			if(defun[defun.length()-1] == L'\n') {
					defun.resize(defun.length()-1,0);
			}
			m_History.push_back(defun);
			m_HistoryNavStarted = false;
		}
	}
}

//--------------------------------------------------------------
void Repl::printPrompt() {
	m_InsertPos = m_Text.length();
	if(m_Text[m_InsertPos-1] != L'\n') {
		m_Text += L'\n';
		m_InsertPos++;
	}
	m_Text += s_prompt;
	m_Position = m_PromptPos = m_Text.length();
}

//--------------------------------------------------------------
void Repl::historyClear() {
	m_HistoryNavStarted = false;
	m_History.clear();
	m_InsertPos = 0;
	m_HistoryIter = m_History.end();
	m_HistoryPresent = m_Text.substr(m_PromptPos);
}

//--------------------------------------------------------------
void Repl::historyPrev() {
	if(!m_HistoryNavStarted) {
		m_HistoryIter = m_History.end();
		m_HistoryNavStarted = true;
	}

	if(m_HistoryIter == m_History.end()) {
		m_HistoryPresent = m_Text.substr(m_PromptPos);
	}

	if(m_HistoryIter == m_History.begin()) {
		return;
	}

	m_HistoryIter--;
	historyShow(*m_HistoryIter);
}

//--------------------------------------------------------------
void Repl::historyNext() {
	if(!m_HistoryNavStarted || (m_HistoryIter == m_History.end())) {
		return;
	}
	
	m_HistoryIter++;
	historyShow((m_HistoryIter == m_History.end()) ?
		m_HistoryPresent : *m_HistoryIter);
}

//--------------------------------------------------------------
void Repl::historyShow(wstring what) {
	m_Text.resize(m_PromptPos, 0);
	m_Text += what;
	m_Position = m_Text.length();
}

//--------------------------------------------------------------
void Repl::ensureCursorVisible() {
	unsigned int i;
	unsigned int curVisLine = 0;
	// use m_VisibleLines, m_TopTextPosition;
	for(i = m_Position; i > m_TopTextPosition; i--) {
		if (m_Text[i] == L'\n') {
			curVisLine++;
		}
	}
	while(curVisLine >= m_VisibleLines) {
		if(m_Text[m_TopTextPosition++] == L'\n') {
			curVisLine--;
		}
	}
}

// UTIL

//--------------------------------------------------------------
// FIXME: skip parens in wstrings and comments
bool isBalanced(wstring s) {
	int balance = 0;
	for(wstring::iterator i = s.begin(); i != s.end(); i++) {
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
bool isEmpty(wstring s) {
    const wstring ws = L" \t\n\r";
	for(wstring::iterator i = s.begin(); i != s.end(); i++) {
		if(ws.find(*i) == wstring::npos) {
			return false;
		}
	}
	return true;
}

// LOGGER

//--------------------------------------------------------------
void Repl::Logger::log(ofLogLevel level, const string & module, const string & message){
	ofConsoleLoggerChannel::log(level, module, message);
	m_Parent->print(string_to_wstring(message));
	m_Parent->print(L"\n");
}

void Repl::Logger::log(ofLogLevel level, const string & module, const char* format, va_list args){
	ofConsoleLoggerChannel::log(level, module, format, args);
	m_Parent->print(string_to_wstring(ofVAArgsToString(format, args)));
	m_Parent->print(L"\n");
}

