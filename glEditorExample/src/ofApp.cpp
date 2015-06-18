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
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 */
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(0);
	
	// let's see what's going on inside
	ofSetLogLevel("ofxGLEditor", OF_LOG_VERBOSE);
	
	// setup the global editor font before you do anything!
	ofxEditor::loadFont("fonts/PrintChar21.ttf", 24);
	
	// set a custom Repl banner & prompt (do this before setup())
	ofxRepl::setReplBanner("Hello, welcome to ofxGLEditor!");
	ofxRepl::setReplPrompt("prompt> ");

	// setup editor with event listener
	editor.setup(this);
	
	// make some room for the bottom editor info text
	editor.resize(ofGetWidth(), ofGetHeight()-ofxEditor::getCharHeight());
	
	// open & load a file into the current editor (1)
	editor.openFile("hello.txt", 1);
	ofLogNotice() << "number of lines: " << editor.getNumLines(1);
	editor.openFile("intl.txt", 2);
	ofLogNotice() << "number of lines: " << editor.getNumLines(2);
	
	// change multi editor settings, see ofxEditorSettings.h for details
	//editor.getSettings().setTextColor(ofColor::red); // main text color
	//editor.getSettings().setCursorColor(ofColor::blue); // current pos cursor
	//editor.getSettings().setAlpha(0.5); // main text, cursor, & highlight alpha
	
	// other settings
	//editor.setLineWrapping(true);
	//editor.setLineNumbers(true);
	//editor.setAutoFocus(true);
	
	// move the cursor to a specific line
	//editor.setCurrentLine(4);
	//ofLogNotice() << "current line: " << editor.getCurrentLine();
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	editor.draw();
	
	// draw current editor info using the same font as the editor
	if(!editor.isHidden() && editor.getCurrentEditor() > 0) {
	
		int bottom = ofGetHeight()-ofxEditor::getCharHeight();
		int right = ofGetWidth()-ofxEditor::getCharWidth()*7; // should be enough room
		ofSetColor(ofColor::gray);
	
		// draw the current editor num
		editor.drawString("Editor: "+ofToString(editor.getCurrentEditor()),
		                  0, bottom);
		
		// draw the current line & line pos
		editor.drawString(ofToString(editor.getCurrentLine()+1)+","+
		                  ofToString(editor.getCurrentLinePos()),
		                  right, bottom);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// note: when using CTRL as the modifier key, CTRL + key might be an ascii
	// control char so check for both the char 'f' and 6, the ascii value for
	// CTRL + f, see also: http://ascii-table.com/control-chars.php
	bool modifier = ofGetKeyPressed(ofxEditor::getSuperAsModifier() ? OF_KEY_SUPER : OF_KEY_CONTROL);
	if(modifier) {
		switch(key) {
			case 'f': case 6:
				ofToggleFullscreen();
				return;
			case 'l': case 12:
				editor.setLineWrapping(!editor.getLineWrapping());
				return;
			case 'n': case 14:
				editor.setLineNumbers(!editor.getLineNumbers());
				return;
			case 'k': case 26:
				editor.setAutoFocus(!editor.getAutoFocus());
				return;
		}
	}
	
	// see ofxGLEditor.h for key commands
	editor.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	// make some room for the bottom editor info text
	editor.resize(w, h-ofxEditor::getCharHeight());
}

//--------------------------------------------------------------
void ofApp::openFileEvent(int &whichEditor) {
	// received an editor open via CTRL/Super + o
	
	ofLogNotice() << "received open event for editor " << whichEditor
		<< " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::saveFileEvent(int &whichEditor) {
	// received an editor save via CTRL/Super + s or CTRL/Super + d
	
	ofLogNotice() << "received save event for editor " << whichEditor
		<< " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::executeScriptEvent(int &whichEditor) {
	// received on editor CTRL/Super + e
	
	// get the text buffer with:
	// string txt = editor.getText(whichEditor);
	// note: returns only the selected area when holding SHIFT + arrow keys
	
	// if you have some scripting language (e.g. ofxLua)
	ofLogNotice() << "received execute script event for editor " << whichEditor;
}

//--------------------------------------------------------------
void ofApp::evalReplEvent(const string &text) {
	ofLogNotice() << "received eval repl event: " << text;
	
	// make sure there is a response since this triggers printing the
	// next conosle prompt
	editor.evalReplReturn("did something"); // print this, then prompt
	//editor.evalReplReturn(); // empty response, just prints prompt
}
