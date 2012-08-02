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
 */
#pragma once

#include "ofMain.h"

#include "GLEditor.h"
#include "Unicode.h"
#include "ClipBoard.h"

// TODO: add clipboard support on Windows:
// http://stackoverflow.com/questions/6436257/how-do-you-copy-paste-from-the-clipboard-in-c
//
// TODO: add clipboard support on Linux:
// http://michael.toren.net/mirrors/doc/X-copy+paste.txt

/// a wrapper around the fluxus opengl text editor
/// http://www.pawfal.org/fluxus/
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
	bool setup(string fontFile = "fonts/DroidSansMono.ttf");
	
	/// clear resources
	void clear();
	
	/// draw the editor over the current viewport
	void draw();
	
	/// handles editor key events
	///
	/// ALT + r: trigger a run script event
	/// ALT + b: blow up the cursor
	/// ALT + a: clear all text in current editor
	/// ALT + c: copy from the system clipboard to current editor (Mac OS X only)
	/// ALT + v: paste from current editor to the system clipboard (Mac OS X only)
	/// ALT + s: trigger a save file event
	///
	/// ALT + d: switch to the default editor (1)
	/// ALT + 1 to ALT + 9: switch to editor 1 - 9
	void keyPressed(int key);
	
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
	/// set addTimestamp = true if you want the filename prepended with a timestamp
	bool saveFile(string filename = "output.txt", int editor = 0, bool addTimestamp = false);
	
	/// set the contents of an editor
	///
	/// set editor to 0 for the current editor
	/// or an editor index from 1 - 9
	void setText(string text, int nEditor = 0);
    
	/// get the contents of an editor
	///
	/// set editor to 0 for the current editor
	/// or an editor index of 1 - 9
	string getText(int editor = 0);
	
	/// clear the contents of an editor
	void clearText(int editor = 0);
	
	/// clear the contents of *all* editors
	void clearAllText();
	
	/// set the current editor by index, form 1 - 9
	void setCurrentEditor(int editor);
	
	/// get the index of the current editor, from 1 - 9
	int getCurrentEditor();
    
	/// this event is triggered when ALT + r is pressed
	/// returns the index of the current editor
	ofEvent<int> runScriptEvent;
	
	/// this event is triggered when ALT + s is pressed
	/// returns the index of the current editor
	ofEvent<int> saveFileEvent;
    
	///
	/// keep track of pressed modifer keys
	///
	/// from Rick Companje's ofxKeyMap 2009.09.17
	/// https://github.com/companje/ofxKeyMap
	///
	inline bool isAltPressed()		{return bAltPressed;}
	inline bool isShiftPressed()	{return bShiftPressed;}
	inline bool isControlPressed()	{return bControlPressed;}
	
	/// the number of editors
	static const int s_numEditors = 10;
	
private:

	class Editor;
	
	vector<Editor*> glEditor;
	ClipBoard clipBoard;
    
	int currentEditor; ///< note: 0-9, while valid nums are 1 - 9
    
	bool bAltPressed;
	bool bShiftPressed;
	bool bControlPressed;
	
	// wrapper for added functionality
	class Editor : public fluxus::GLEditor {
	public:

		// insert text at the current position
		void InsertText(const string& s);
		
		// render a string using the built in PolyGlyph,
		// handles tabs and endlines
		void RenderString(const string& s, float x, float y, ofFloatColor color=ofFloatColor::white);
	};
};