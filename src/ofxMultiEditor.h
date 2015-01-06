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
 * Based on a rewrite of ofxGLEditor by Akira Hayasaka & Dan Wilcox.
 */
#pragma once

#include "ofxRepl.h"
#include "ofxFileDialog.h"

/// multi editor event listener
class ofxMultiEditorListener : public ofxReplListener {
	
	public:
		
		/// triggered when opening a file via CTRL/Super + o
		/// returns the index of the current editor
		virtual void openFileEvent(int &whichEditor) {}
	
		/// triggered when saving a file via CTRL/Super + s & CTRL/Super + d
		/// returns the index of the current editor
		virtual void saveFileEvent(int &whichEditor) {}
	
		/// triggered when CTRL/Super + e is pressed
		/// returns the index of the current editor
		virtual void executeScriptEvent(int &whichEditor) {}
	
		/// this event is triggered when Enter is pressed in the Repl console
		/// returns the text to be evaluated
		virtual void evalReplEvent(string &text) {}
};

/// 9 text editor buffers with a live optional Read-Eval-Print Loop
class ofxMultiEditor {

	public:

		ofxMultiEditor();
		virtual ~ofxMultiEditor();

	/// \section Main

		/// setup editors and set event listener
		///
		/// set enableRepl = false if you don't need the Read-eval-print loop console
		///
		/// Note: make sure to laod the global editor font with ofxEditor::loadFont()
		/// WARNING: the editor will crash your app if a font is not loaded!
		void setup(ofxMultiEditorListener *listener, bool enableRepl=true);
		
		/// clear resources
		void clear();
		
		/// draw the editor over the current viewport
		void draw();
		
		/// handles editor key events
		///
		/// MOD -> CTRL or Super (Windows Key, Mac CMD), set with
		///        ofxEditor::setSuperAsModifier()
		///
		/// MOD + t: toggle whether to show or hide the editor
		/// MOD + r & MOD + 0: switch to REPL (console), if enabled
		/// MOD + 1 to MOD + 9: switch to editor 1 - 9
		///
		/// MOD + e: trigger an executeScript event
		/// MOD + b: blow up the cursor
		/// MOD + a: clear all text in current editor
		/// MOD + c: copy from the system clipboard to current editor (GLFW only)
		/// MOD + v: paste from current editor to the system clipboard (GLFW only)
		///
		/// MOD + s: save file, shows save as dialog if no filename has been set
		/// MOD + d: save as dialog, saves in current path (default: data path)
		/// MOD + o: open a file via a file browser, starts in current path
		///
		/// MOD + -: decrease current editor alpha
		/// MOD + =: increase current editor alpha
		///
		/// Note: ESC to exit is disabled as ESC is used to exit the file dialog
		///
		void keyPressed(int key);
		
		/// reshape the drawing area
		/// call this if you change the window size (fullscreen, etc)
		void resize();
		void resize(int width, int height);
		
	/// \section Editor
	
		/// open & load a file into the an editor, clears existing text
		///
		/// set editor to 0 for the current editor
		/// or an editor index from 1- 9
		bool openFile(string filename, int editor=0);
		
		/// save the text in the current editor to a file
		///
		/// set editor to 0 for the current editor or an editor index from 1- 9
		bool saveFile(string filename, int editor=0);
	
		/// get the contents of an editor or contents of editor selection
		///
		/// set editor to 0 for the current editor
		/// or an editor index of 1 - 9
		string getText(int editor=0);
	
		/// set the contents of an editor
		///
		/// set editor to 0 for the current editor
		/// or an editor index from 1 - 9
		void setText(string text, int editor=0);
	
		/// insert text into an editor at the current position
		///
		/// set editor to 0 for the current editor
		/// or an editor index from 1 - 9
		void insertText(string text, int editor=0);
		
		/// clear the contents of an editor
		void clearText(int editor=0);
		
		/// clear the contents of *all* editors
		void clearAllText();
		
		/// set the current editor by index, from 1 - 9 (0 is Repl)
		void setCurrentEditor(int editor);
		
		/// get the index of the current editor, from 1 - 9 (0 is REPL)
		int getCurrentEditor();
		
		/// set the filename of the editor by index, from 1 - 9
		void setEditorFilename(int editor, string filename);
		
		/// get the filename of the current editor (default: empty string "")
		string getEditorFilename(int editor);
		
		/// number of lines of text in an editor
		unsigned int getNumLines(int editor=0);
		
		/// current line of an editor
		/// set editor to 0 for the current editor
		void setCurrentLine(unsigned int line, int editor=0);
		unsigned int getCurrentLine(int editor=0);
		
		/// current line pos of an editor
		/// set editor to 0 for the current editor
		unsigned int getCurrentLinePos(int editor=0);
		
		/// current line pos of an editor
		/// set editor to 0 for the current editor
		unsigned int getCurrentLineLen(int editor=0);
		
		/// current overall pos in the editor buffer
		/// set editor to 0 for the current editor
		unsigned int getCurrentPos(int editor=0);
		
	/// \section Repl (Read-Eval-Print Loop), switch to via setCurrentEditor(0)
		
		/// send a response to the last evalReplEvent to the Repl console
		///
		/// **important**: this must be called after an evalReplEvent in order to
		/// print the next prompt ...
		///
		/// note: this does nothing if the repl was not enabled in setup()
		void evalReplReturn(const string &text="");
		
		/// clears text in Repl buffer, does not clear history
		void clearRepl();
		
		/// clears Repl history, does not clear buffer text
		void clearReplHistory();
	
	/// \section Settings

		/// access to the internal settings object
		///
		/// example usage:
		///
		///   editor.getSettings().textColor = ofColor::gray;
		///   editor.getSettings().alpha = 0.5;
		///   editor.getSettings().tabWidth = 8;
		///   ... etc
		///
		ofxEditorSettings& getSettings();
	
		/// enable/disable line wrapping
		void setLineWrapping(bool wrap=true);
	
		/// get line wrapping value
		bool getLineWrapping();
	
		/// enable/disable editor line numbers
		void setLineNumbers(bool numbers=true);
	
		/// get editor line numbers value
		bool getLineNumbers();
		
	/// \section Util
		
		/// set the file browser path, default: data path when setup() is called
		void setPath(string path);
	
		/// the current modifier as set in setup(), either CTRL (default) or Super
		inline bool isModifierPressed() {return bModifierPressed;}
		
		/// hide/unhide the editor
		void setHidden(bool hidden);
		
		/// is the editor hidden?
		inline bool isHidden() {return bHideEditor;}
		
		/// the number of editors
		static const int s_numEditors = 10;
		
		/// draw a string using the current editor font
		void drawString(const string& s, float x, float y);
		void drawString(const string& s, ofPoint& p);
	
	private:
	
		/// checks given index and autodecrements
		/// editor = 0 sets current editor
		/// returns -1 if index out of bounds
		int getEditorIndex(int editor);
	
		ofxMultiEditorListener *m_listener; //< event listener
	
		ofxEditorSettings m_settings; //< shared editor settings
		vector<ofxEditor*> m_editors; //< editor instances, repl is at index 0
		ofxFileDialog *m_fileDialog; //< file dialog instance
	
		int m_currentEditor; //< current editor, repl is at index 0
		vector<string> m_saveFiles; //< one for each editor
		
		bool bModifierPressed; //< is the editor modifier key being pressed?
		
		bool bHideEditor;     //< hide the editor?
		bool bShowFileDialog; //< show the file dialog right now?
};
