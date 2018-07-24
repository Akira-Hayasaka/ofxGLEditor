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
 */
#pragma once

#include "ofxRepl.h"
#include "ofxFileDialog.h"

/// multi editor event listener
class ofxGLEditorListener : public ofxReplListener {
	
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
		virtual void evalReplEvent(const std::string &text) {}
};

/// 9 text editor buffers with a live optional Read-Eval-Print Loop,
/// equivalent to the Fluxus editor
class ofxGLEditor {

	public:

		ofxGLEditor();
		virtual ~ofxGLEditor();

	/// \section Main

		/// setup editors and set event listener
		///
		/// set enableRepl = false if you don't need the Read-eval-print loop console
		///
		/// Note: make sure to load the global editor font with ofxEditor::loadFont()
		/// WARNING: the editor will crash your app if a font is not loaded!
		void setup(ofxGLEditorListener *listener, bool enableRepl=true);
		
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
		/// ARROWS + SHIFT: select text
		///
		/// MOD + e: trigger an executeScript event
		/// MOD + b: blow up the cursor
		/// MOD + a: select all text in the current editor
		/// MOD + a + SHIFT: clear all text in the current editor
		/// MOD + c: copy from system clipboard (GLFW only) or copy buffer
		/// MOD + v: paste to system clipboard (GLFW only) or copy buffer
		///
		/// MOD + s: save file, shows save as dialog if no filename has been set
		/// MOD + d: save as dialog, saves in current path (default: data path)
		/// MOD + o: open a file via a file browser, starts in current path
		///
		/// MOD + z: undo last key input action
		/// MOD + y: redo last key input action
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
	
		/// open & load a file into an editor, clears existing text
		///
		/// set editor to 0 for the current editor
		/// or an editor index from 1- 9
		///
		/// returns true on success
		bool openFile(std::string filename, int editor=0);
		
		/// save the text in an editor to a file
		///
		/// set editor to 0 for the current editor or an editor index from 1- 9
		///
		/// returns true on success
		bool saveFile(std::string filename, int editor=0);
	
		/// save the text in an editor to the current filename
		///
		/// set editor to 0 for the current editor or an editor index from 1- 9
		///
		/// returns true on success
		bool saveFile(int editor=0);
	
		/// get the contents of an editor or contents of editor selection
		///
		/// set editor to 0 for the current editor
		/// or an editor index of 1 - 9
		string getText(int editor=0);
	
		/// set the contents of an editor
		///
		/// set editor to 0 for the current editor
		/// or an editor index from 1 - 9
		void setText(std::string text, int editor=0);
	
		/// insert text into an editor at the current position
		///
		/// set editor to 0 for the current editor
		/// or an editor index from 1 - 9
		void insertText(std::string text, int editor=0);
		
		/// clear the contents of an editor
		void clearText(int editor=0);
		
		/// clear the contents of *all* editors
		void clearAllText();
		
		/// set the current editor by index, from 1 - 9 (0 is Repl)
		void setCurrentEditor(int editor);
		
		/// get the index of the current editor, from 1 - 9 (0 is REPL)
		int getCurrentEditor();
		
		/// set the filename of the editor by index, from 1 - 9
		void setEditorFilename(int editor, std::string filename);
		
		/// get the filename of the current editor,
		/// eturns empty string "" on error
		string getEditorFilename(int editor);
	
		/// returns true if text is currently selected in an editor
		/// set editor to 0 for the current editor
		bool isSelection(int editor=0);
		
		/// number of lines of text in an editor
		/// set editor to 0 for the current editor
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
		void evalReplReturn(const std::string &text="");
		
		/// clears text in Repl buffer, does not clear history
		void clearRepl();
		
		/// clears Repl history, does not clear buffer text
		void clearReplHistory();
	
	/// \section Display Settings

		/// access to the internal settings object
		///
		/// example usage:
		///
		///   editor.getSettings().getTextColor(ofColor::gray);
		///   editor.getSettings().getAlpha(0.5);
		///   editor.getSettings().getTabWidth(8);
		///   editor.getSettings().setLangSyntax("Lua", &luaSyntax);
		///   editor.getSettings().setFileExtLang("Lua", "lua");
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
	
		/// enable/disable auto focus zooming
		void setAutoFocus(bool focus=true);
	
		/// get auto focus value
		bool getAutoFocus();
	
		/// enable/disable flashing selection on eval
		void setFlashEvalSelection(bool flash=true);
	
		/// get flashing selection on eval value
		bool getFlashEvalSelection();
	
	/// \section Color Scheme
	
		/// set color scheme and highlight syntax
		/// note: pointer is never deleted
		void setColorScheme(ofxEditorColorScheme *colorScheme);
	
		/// clear the current color scheme
		/// set editor to 0 for the current editor
		void clearColorScheme();
	
		/// get the current color scheme, returns NULL if not set
		ofxEditorColorScheme* getColorScheme();
	
	/// \section Language Syntax
	
		/// set language syntax manually for an editor based on language string
		/// aka "C++", "Lua", "GLSL", etc from the available syntaxes in the
		/// current settings
		///
		/// create syntax(es), add to settings, & set; example:
		///
		/// ofxEditorSyntax *luaSyntax = new ofxEditorSyntax();
		/// luaSyntax->setSingleLineComment("--");
		/// luaSyntax->setMultiLineComment("--[[", "]]");
		/// editor.getSettings().setLangSyntax("Lua", luaSyntax);
		/// editor.setLangSyntax("Lua");
		/// // don't forget to delete the luaSyntax object when you're done
		///
		/// optionally set file extensions so syntax is chosen automatically based
		/// on filename when opening or saving, example:
		///
		/// editor.getSettings().setFileExtLang("lua", "Lua"); // "lua not ".lua"
		///
		/// note: syntax does not apply to REPL or FileDialog
		///
		/// set editor to 0 for the current editor
		void setLangSyntax(const std::string& lang, int editor=0);
	
		/// clear the current language syntax for an editor
		/// set editor to 0 for the current editor
		void clearSyntax(int editor=0);
	
		/// get the current language syntax for an editor, returns NULL if not set
		/// set editor to 0 for the current editor
		ofxEditorSyntax* getSyntax(int editor=0);
		
	/// \section Util
		
		/// set the file browser path, default: data path when setup() is called
		void setPath(std::string path);
	
		/// the current modifier as set in setup(), either CTRL (default) or Super
		inline bool isModifierPressed() {return bModifierPressed;}
		
		/// hide/unhide the editor
		void setHidden(bool hidden);
		
		/// is the editor hidden?
		inline bool isHidden() {return bHideEditor;}
		
		/// the number of editors
		static const int s_numEditors = 10;
		
		/// draw a string using the current editor font
		void drawString(const std::string& s, float x, float y);
		void drawString(const std::string& s, ofPoint& p);
	
	private:
	
		/// checks given index and autodecrements
		/// editor = 0 sets current editor
		/// returns -1 if index out of bounds
		int getEditorIndex(int editor);
	
		ofxGLEditorListener *m_listener; //< event listener
	
		ofxEditorSettings m_settings; //< shared editor settings
		vector<ofxEditor*> m_editors; //< editor instances, repl is at index 0
		ofxFileDialog *m_fileDialog; //< file dialog instance
	
		int m_currentEditor; //< current editor, repl is at index 0
		std::vector<std::string> m_saveFiles; //< one for each editor
		
		bool bModifierPressed; //< is the editor modifier key being pressed?
		
		bool bHideEditor;     //< hide the editor?
	
		bool bFlashEvalSelection; //< flash selection on eval?
};
