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
	
	// handle ESC internally since we use it to exit selection
	ofSetEscapeQuitsApp(false);

	// make sure to load editor font before anything else!
    ofxEditor::loadFont("fonts/DecimaMonoPro.ttf", 24);
	
    syntaxHighlight.setup(&colorScheme);
	editor.setColorScheme(&colorScheme);
    editor.setLineNumbers(!editor.getLineNumbers());
    
	// open test file
    testFile.open("EmptyShader.frag", ofFile::ReadOnly);
	editor.setText(testFile.readToBuffer().getText());
	ofLogNotice() << "num chars: " << editor.getNumCharacters() << " num lines: " << editor.getNumLines();
	
	// default: white text on black background
	ofBackground(0);

	debug = false;
    
    /// Shader stuff
    width = ofGetWidth();//640;
    height = ofGetHeight();//480;
    
    shaderFileName = ofSplitString(testFile.getFileName(), "/"); //Find on the last name in the path
    shaderFileName = ofSplitString(shaderFileName.at(shaderFileName.size()-1), "."); //Remove the .frag extension from the fileName
    
    shader.load(shaderFileName[0]);

    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    shader.load(shaderFileName[0]);

    fbo.begin();
    ofClear(0,0,0,0);
        shader.begin();
        shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
        shader.setUniform3f("iResolution", width, height,1);
    
        ofSetColor(255, 255, 255);
        ofRect(0, 0, width, height);
        shader.end();
    fbo.end();
    fbo.draw(0,0,ofGetWidth(),ofGetHeight());

	editor.draw();
	
	if(debug) {
		//editor.drawGrid();
	
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
            case 'r':
                editor.saveFile(testFile.getFileName());
                break;
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
			case 'z':
				editor.setAutoFocus(!editor.getAutoFocus());
				return;
			case 'w':
				ofxEditor::setTextShadow(!ofxEditor::getTextShadow());
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
