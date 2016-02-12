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
#pragma once

#include "ofxEditor.h"

/// repl event listener
class ofxReplListener {

	public:
	
		/// this event is triggered when Enter is pressed in the Repl console
		/// returns the text to be evaluated
		virtual void evalReplEvent(const string &text) = 0;
};

/// Read-eval-print Loop
class ofxRepl : public ofxEditor {

	public:

		ofxRepl();
		ofxRepl(ofxEditorSettings &sharedSettings); //< share settings object
		virtual ~ofxRepl();
	
		/// prints greeting and first prompt
		void setup();
	
		/// prints greeting and first prompt
		/// set listener to receive eval events
		void setup(ofxReplListener *listener);
		
		/// handles key events
		///
		/// MOD -> CTRL or Super (Windows Key, Mac CMD), set with
		///        ofxEditor::setSuperAsModifier()
		///
		/// MOD + a + SHIFT: clear console
		/// MOD + c + SHIFT: clear console history
		///
		/// UP & DOWN: step through command history
		/// RETURN: eval current command
		///
		/// Undo works for the current command line
		///
		void keyPressed(int key);
		
		/// add a wide string to the console,
		/// set beforePrompt to true to print a line above the current prompt
		void print(const u32string &what, bool beforePrompt=false);
	
		/// add a string to the console with string conversion,
		/// set beforePrompt to true to print a line above the current prompt
		void print(const string &what, bool beforePrompt=false);
		
		/// add a wide string to the console and print a return after
		void printEvalReturn(const u32string &what);
	
		/// add a string to the console and print a return after with string conversion
		void printEvalReturn(const string &what);
		
		/// clear the console
		void clearText();
		
		/// clear the command history
		void clearHistory();
	
		bool openFile(string filename); //< dummy implementation
		bool saveFile(string filename); //< dummy implementation

	/// \section Static Utils
	
		/// set/get the Repl greeting banner, default: ""
		static void setReplBanner(const u32string &text); //< call this before setup()
		static void setReplBanner(const string &text); //< call this before setup()
		static u32string& getWideReplBanner();
		static string getReplBanner();
		
		/// set/get the Repl prompt, default: "> "
		static void setReplPrompt(const u32string &text); //< call this before setup()
		static void setReplPrompt(const string &text); //< call this before setup()
		static u32string& getWideReplPrompt();
		static string getReplPrompt();

	protected:
	
		void eval();
		void printPrompt();
		void historyPrev();
		void historyNext();
		void historyClear();
		void historyShow(u32string what);
		void keepCursorVisible();

		ofxReplListener *m_listener; //< eval event listener
		
		unsigned int m_promptPos; //< prompt position in the text buffer
		unsigned int m_insertPos; //< insert position in the text buffer

		u32string m_evalText; //< text to be evaluated when enter is pressed
		
		deque<u32string> m_history; //< line history
		deque<u32string>::iterator m_historyIter; //< current position in line history
		bool m_historyNavStarted; //< is the cursor within the line history?
		u32string m_historyPresent; //< current history line (aka live input)
		unsigned int m_linePos; //< current line the cursor is on
	
		static u32string s_banner; //< REPL header/greeting, default: ""
		static u32string s_prompt; //< prompt string, default: "> "
		
	private:

		/// custom logger to grab prints in the REPL
		class Logger : public ofConsoleLoggerChannel {
		public:
			ofxRepl *m_parent;
			void log(ofLogLevel level, const string & module, const string & message);
			void log(ofLogLevel level, const string & module, const char* format, va_list args);
		};
		ofPtr<Logger> m_logger;
};
