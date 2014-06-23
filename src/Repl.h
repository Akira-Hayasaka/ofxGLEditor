/*
 * Copyright (C) 2014 Dan Wilcox <danomatika@gmail.com> 
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
#pragma once

#include "ofxGLEditor.h"

/// Read-eval-print Loop
class Repl : public ofxGLEditor::Editor {

public:

	/// requires
	Repl(ofxGLEditor *parent);
	virtual ~Repl();
	
	void Handle(int button, int key, int special, int state,
			    int x, int y, int mod);
	
	/// add a string to the console
	void print(const wstring &what);
	
	/// add a string to the console and print a return after
	void printEvalReturn(const wstring &what);
	
	/// clear the console
	void clear();
	
	/// clear the command history
	void clearHistory();
	
	/// text to be evaluated, needed for ofEvent
	string evalText;

	static wstring s_banner; //< REPL header/greeting, default: ""
	static wstring s_prompt; //< prompt string, default: "> "
	static unsigned int MAX_LINE_LENGTH; //< default: 80

protected:
	
	unsigned int m_PromptPos;
	unsigned int m_InsertPos;

	void eval();
	void printPrompt();
	void historyPrev();
	void historyNext();
	void historyClear();
	void historyShow(wstring what);
	void ensureCursorVisible();

	// line history
	deque<wstring> m_History;
	deque<wstring>::iterator m_HistoryIter;
	bool m_HistoryNavStarted;
	wstring m_HistoryPresent;
	unsigned int m_LinePos;
	
private:

	ofxGLEditor *m_Parent; //< used to forward eval events

	/// custom logger to grab prints in the REPL
	class Logger : public ofConsoleLoggerChannel {
	public:
		Repl *m_Parent;
		void log(ofLogLevel level, const string & module, const string & message);
		void log(ofLogLevel level, const string & module, const char* format, va_list args);
	};
	ofPtr<Logger> m_Logger;
	
};
