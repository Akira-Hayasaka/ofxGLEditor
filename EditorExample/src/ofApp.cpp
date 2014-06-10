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
	
	// setup the editor with a font
	editor.setup("fonts/DroidSansMono.ttf");
	
	// load a file into the current editor (1)
	editor.loadFile("hello.txt");
	ofLogNotice() << "number of lines: " << editor.getNumLines();

	// add editor event listening
	ofAddListener(editor.executeScriptEvent, this, &ofApp::executeScriptEvent);
	
	//editor.setTextColor(ofColor::red); // dosen't work right yet ... sorry
	editor.setCursorColor(ofColor::blue); // block cursor color
	//editor.setAlpha(0.5); // main text, cursor, & highlight alpha
	
	// move the cursor
	//editor.setCurrentLine(0);
	ofLogNotice() << "current line pos: " << editor.getCurrentLine();
}

//--------------------------------------------------------------
void ofApp::update(){}

//--------------------------------------------------------------
void ofApp::draw(){
	editor.draw();
	
	ofSetColor(255, 0, 0);
	
	// draw using the same font as the editor
	
	// draw the current editor num
	editor.drawString("Current editor: "+ofToString(editor.getCurrentEditor()), 10, 15);
	
	// draw the current & total line nums
	editor.drawString("Current line: "+ofToString(editor.getCurrentLine())
		+"/"+ofToString(editor.getNumLines()), 740, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	// see ofxGLEditor.h for key commands
	editor.keyPressed(key);
	
	// can also check modifiers from editor
	if(editor.isAltPressed() && key == 'f'){
		ofToggleFullscreen();
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
	editor.reShape();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}

//--------------------------------------------------------------
void ofApp::executeScriptEvent(int &whichEditor){
	// received on editor ALT + e

	// if you have some scripting language (e.g. ofxLua)
	ofLogNotice() << "recieved execute script event for editor " << whichEditor << endl;
}
