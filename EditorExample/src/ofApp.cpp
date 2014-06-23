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
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(30);
	
	// let's see what's going on inside
	ofSetLogLevel("ofxGLEditor", OF_LOG_VERBOSE);
	
	// set a custom Repl banner & prompt (do this before setup())
	ofxGLEditor::setReplBanner("Hello, welcome to ofxGLEditor!");
	ofxGLEditor::setReplPrompt("prompt> ");
	
	// setup the editor with a font
	editor.setup("fonts/DroidSansMono.ttf");
	
	// open & load a file into the current editor (1)
	editor.openFile("hello.txt");
	ofLogNotice() << "number of lines: " << editor.getNumLines();

	// add editor event listening
	ofAddListener(editor.openFileEvent, this, &ofApp::openFileEvent);
	ofAddListener(editor.saveFileEvent, this, &ofApp::saveFileEvent);
	ofAddListener(editor.executeScriptEvent, this, &ofApp::executeScriptEvent);
	ofAddListener(editor.evalReplEvent, this, &ofApp::evalReplEvent);
	
	//editor.setTextColor(ofColor::red); // dosen't work right yet ... sorry
	editor.setCursorColor(ofColor::blue); // block cursor color
	//editor.setAlpha(0.5); // main text, cursor, & highlight alpha
	
	// move the cursor to a specific line
	//editor.setCurrentLine(4);
	ofLogNotice() << "current line: " << editor.getCurrentLine();
}

//--------------------------------------------------------------
void ofApp::update(){}

//--------------------------------------------------------------
void ofApp::draw(){
	editor.draw();
	
	ofSetColor(255, 0, 0);
	
	// draw using the same font as the editor
	if(!editor.isHidden() && editor.getCurrentEditor() > 0) {
	
		// draw the current editor num
		editor.drawString("Current editor: "+ofToString(editor.getCurrentEditor()), 10, 15);
		
		// draw the current & total line nums
		editor.drawString("Current line: "+ofToString(editor.getCurrentLine())
			+"/"+ofToString(editor.getNumLines()), 740, 15);
	
		// draw the current pos & line length
		editor.drawString("         pos: "+ofToString(editor.getCurrentLinePos()), 740, 35);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	// see ofxGLEditor.h for key commands
	editor.keyPressed(key);
	
	// can also check modifiers from editor
	//
	// note: CTRL + key might be an ascii control char so check for both
	// the char 'f' and 6, the ascii value for CTRL + f
	// see also: http://ascii-table.com/control-chars.php
	switch(key) {
		case 'f': case 6:
			if(editor.isModifierPressed()) {
				ofToggleFullscreen();
			}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	editor.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	editor.reShape(); // set new size using window or screen size
	//editor.reShape(w, h); // set new size manually
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}

//--------------------------------------------------------------
void ofApp::openFileEvent(int &whichEditor){
	// receievd an editor open via CTRL/Super + o
	
	ofLogNotice() << "received open event for editor " << whichEditor
		<< " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::saveFileEvent(int &whichEditor){
	// receievd an editor save via CTRL/Super + s or CTRL/Super + d
	
	ofLogNotice() << "received save event for editor " << whichEditor
		<< " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::executeScriptEvent(int &whichEditor){
	// received on editor CTRL/Super + e
	
	// get the text buffer with:
	// string txt = editor.getText(whichEditor);
	// note: returns only the seleceted area when holding Shift + arrow keys
	
	// if you have some scripting language (e.g. ofxLua)
	ofLogNotice() << "received execute script event for editor " << whichEditor;
}

//--------------------------------------------------------------
void ofApp::evalReplEvent(string &text){
	ofLogNotice() << "received eval repl event: " << text;
	
	// make sure there is a response since this triggers printing the next conosle prompt
	editor.evalReplReturn("did something"); // print this, then prompt
	//editor.evalReplReturn(); // empty response, just prints prompt
}
