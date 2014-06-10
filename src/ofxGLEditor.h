/*
 * Copyright (C) 2012 Akira Hayasaka & Dan Wilcox <danomatika@gmail.com> 
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

#include "ofMain.h"

#include "GLEditor.h"
#include "GLFileDialog.h"
#include "Unicode.h"
#include "ClipBoard.h"

// TODO: add clipboard support on Windows:
// http://stackoverflow.com/questions/6436257/how-do-you-copy-paste-from-the-clipboard-in-c
//
// TODO: add clipboard support on Linux:
// http://michael.toren.net/mirrors/doc/X-copy+paste.txt

/// a wrapper around the fluxus opengl text editor for OpenFrameworks
/// http://www.pawfal.org/fluxus Copyright (C) Dave Griffiths
///
/// As of OF 0.8.0+, the default windowing system uses the GLFW library.
///
/// If you are still using the ofAppGlutWindow, you need to add -DOFX_GLEDITOR_GLUT
/// to your C++ flags in order to enable Glut modifier key support.
///
class ofxGLEditor {

public:
	
	ofxGLEditor();
	virtual ~ofxGLEditor();
	
	/// setup with the font to load
	/// returns false if font is not found
	///
	/// NOTE: variable width fonts are not handled well, use fixed-width fonts
	///
	/// WARNING: the editor will crash your app if a font is not loaded!
	bool setup(string fontFile);
	
	/// clear resources
	void clear();
	
	/// draw the editor over the current viewport
	void draw();
	
	/// handles editor key events
	///
	/// ALT + h: show/hide
	/// ALT + r & ALT + 0: switch to REPL (console)
	/// ALT + 1 to ALT + 9: switch to editor 1 - 9
	///
	/// ALT + e: trigger an executeScript event
	/// ALT + b: blow up the cursor
	/// ALT + a: clear all text in current editor
	/// ALT + c: copy from the system clipboard to current editor (Mac OS X only)
	/// ALT + v: paste from current editor to the system clipboard (Mac OS X only)
	///
	/// ALT + s: save file, shows save as dialog if no filename has been set
	/// ALT + d: save as dialog, saves in current path (default: data path)
	/// ALT + l: load a file via a file browser, starts in current path
	///
	/// ALT + -: decrease current editor alpha
	/// ALT + =: increase current editor alpha
	///
	void keyPressed(int key);
	void keyReleased(int key);
	
	/// reshape the drawing area
	/// call this if you change the window size (fullscreen, etc)
	void reShape();
	
	/// paste the latest text from the system clipboard
	/// note: currently only supported on Mac OS X
	void pasteFromClipBoard();
	
	/// copy the currently selected text to the system clipboard
	/// note: currently only supported on Mac OS X
	void copyToClipBoard();
	
	/// load a file into the an editor, clears existing text
	///
	/// set editor to 0 for the current editor
	/// or an editor index from 1- 9
	bool loadFile(string filename = "output.txt", int editor = 0);
	
	/// save the text in the current editor to a file
	///
	/// set editor to 0 for the current editor or an editor index from 1- 9
	bool saveFile(string filename = "output.txt", int editor = 0);
	
	/// set the contents of an editor
	///
	/// set editor to 0 for the current editor
	/// or an editor index from 1 - 9
	void setText(string text, int editor = 0);
    
	/// get the contents of an editor
	///
	/// set editor to 0 for the current editor
	/// or an editor index of 1 - 9
	string getText(int editor = 0);
	
	/// clear the contents of an editor
	void clearText(int editor = 0);
	
	/// clear the contents of *all* editors
	void clearAllText();
	
	/// set the current editor by index, from 1 - 9
	void setCurrentEditor(int editor);
	
	/// get the index of the current editor, from 1 - 9 (0 is REPL)
	int getCurrentEditor();
	
	/// set the filename of the editor by index, from 1 - 9
	void setEditorFilename(int editor, string filename);
	
	/// get the filename of the current editor (default: empty string "")
	string getEditorFilename(int editor);
    
	/// this event is triggered when ALT + e is pressed
	/// returns the index of the current editor
	ofEvent<int> executeScriptEvent;
	
	/// set the file browser path, default: data path when setup() is called
	void setPath(string path);
    
	/// keep track of pressed modifer keys
	///
	/// note: Super is not tracked when using ofAppGlutWindow
	inline bool isAltPressed()		{return bAltPressed;}
	inline bool isShiftPressed()	{return bShiftPressed;}
	inline bool isControlPressed()	{return bControlPressed;}
	inline bool isSuperPressed()	{return bSuperPressed;} //< Win or CMD key
	
	/// hide/unhide the editor
	void setHidden(bool hidden);
	
	/// is the editor hidden?
	inline bool isHidden() {return bHideEditor;}
	
	/// the number of editors
	static const int s_numEditors = 10;
	
	/// draw a string using the current editor font
	void drawString(const string& s, float x, float y);
	void drawString(const string& s, ofPoint& p);

// TODO: this works, but alpha blending/glyph size means it looks terrible
// real fix is to replace PolyGlyph with ofTrueTypeFont
//	/// text color (including alpha)
//	/// default: white
//	void setTextColor(ofColor c);
//	ofColor getTextColor();
//	
	/// cursor color (including alpha)
	/// default: yellow
	void setCursorColor(ofColor c);
	ofColor getCursorColor();
	
	/// overall alpha (0-1), note: in addition to individual editor alpha
	/// default: 1.0
	void setAlpha(float a);
	float getAlpha();
	
	/// current line pos of an editor (1-size)
	/// set editor to 0 for the current editor
	void setCurrentLine(unsigned int line, int editor = 0);
	unsigned int getCurrentLine(int editor = 0);
	
	/// number of lines of of text in an editor
	unsigned int getNumLines(int editor = 0);
	
private:

	/// checks given index and autodecrements
	/// editor = 0 sets current editor
	/// returns -1 if index out of bounds
	int getEditorIndex(int editor);

	class Editor;
	class FileDialog;
	
	vector<Editor*> glEditors;
	FileDialog *glFileDialog;
	ClipBoard clipBoard;
    
	unsigned int numEditors;
	int currentEditor; ///< note: 0-9
	vector<string> saveFiles; ///< one for each editor
    
	bool bAltPressed;
	bool bShiftPressed;
	bool bControlPressed;
	bool bSuperPressed;
	
	bool bHideEditor;
	bool bShowFileDialog;
	
	// wrappers for added functionality
	class Editor : public fluxus::GLEditor {
	public:

		// get the current num of lines in the buffer
		unsigned int GetLineCount() {return m_LineCount;}
		
		// wrappers to current line pos
		void SetCurLine(int line) {SetCurrentLine(line);}
		int GetCurLine() {return GetCurrentLine();}

		// insert text at the current position
		void InsertText(const string& s);
		
		// render a string using the built in PolyGlyph,
		// handles tabs and endlines
		void RenderString(const string& s, float x, float y, ofFloatColor color=ofFloatColor::white);
		
		// manually count the number of lines
		void CountLines();
	};
	class FileDialog : public fluxus::GLFileDialog {
	public:
	
		// set the current path in the file browser
		void SetPath(wstring path) {
			m_Path = path;
			ReadPath();
		}
	};
};