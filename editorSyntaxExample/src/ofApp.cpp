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

	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	// handle ESC internally since we use it to exit selection
	ofSetEscapeQuitsApp(false);

	// make sure to load editor font before anything else!
	ofxEditor::loadFont("fonts/PrintChar21.ttf", 30);
	
	// sample lua syntax
	colorScheme.setWordColor("function", ofColor::fuchsia);
	colorScheme.setWordColor("end", ofColor::fuchsia);
	colorScheme.setSingleLineComment("--");
	colorScheme.setMultiLineComment("--[[", "]]");
	
	// syntax highlighter colors
	colorScheme.setStringColor(ofColor::yellow);
	colorScheme.setNumberColor(ofColor::orangeRed);
	colorScheme.setCommentColor(ofColor::gray);
	
	// open test file
	ofFile testFile;
	testFile.open("test.txt", ofFile::ReadOnly);
	editor.setText(testFile.readToBuffer().getText());
	ofLogNotice() << "num chars: " << editor.getNumCharacters() << " num lines: " << editor.getNumLines();
	
	ofBackground(0);
	debug = false;
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	editor.draw();
	
	if(debug) {
		editor.drawGrid();
	
		ofSetColor(255);
		ofDrawBitmapString("fps: "+ofToString((int)ofGetFrameRate()), ofGetWidth()-70, ofGetHeight()-10);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	bool modifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	if(modifierPressed) {
		switch(key) {
			case 's':
				if(editor.getColorScheme()) {
					editor.clearColorScheme();
				}
				else {
					editor.setColorScheme(&colorScheme);
				}
				return;
			case 'd':
				debug = !debug;
				return;
			case 'f':
				ofToggleFullscreen();
				return;
			case 'l':
				editor.setLineWrapping(!editor.getLineWrapping());
				return;
			case 'n':
				editor.setLineNumbers(!editor.getLineNumbers());
				return;
			case '1':
				ofLogNotice() << "current line: " << editor.getCurrentLine() <<	" pos: " << editor.getCurrentLinePos();
				editor.setCurrentLinePos(1, 5);
				ofLogNotice() << "current line: " << editor.getCurrentLine() <<	" pos: " << editor.getCurrentLinePos();
				break;
			case '2':
				ofLogNotice() << "current line: " << editor.getCurrentLine() <<	" pos: " << editor.getCurrentLinePos();
				editor.setCurrentLinePos(5, 2);
				ofLogNotice() << "current line: " << editor.getCurrentLine() <<	" pos: " << editor.getCurrentLinePos();
				break;
		}
	}
	editor.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	editor.resize(w, h);
}
