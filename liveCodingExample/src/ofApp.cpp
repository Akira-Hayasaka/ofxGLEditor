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
	ofSetLogLevel("ofxLua", OF_LOG_VERBOSE);
		
	// script to run
	script = "scripts/liveCodingExample.lua";
	
	// init the lua state
	lua.init(true); // true because we want to stop on an error
	
	// listen to error events
	lua.addListener(this);
    
    // load editor font
    ofxEditor::loadFont("fonts/PrintChar21.ttf", 24);
	hideEditor = false;
    
    // sample lua syntax
	syntax.loadFile("luaSyntax.xml");
	editor.getSettings().addSyntax(&syntax);
    
    // syntax highlighter colors
    colorScheme.loadFile("colorScheme.xml");
    editor.setColorScheme(&colorScheme);
    
    // open script file into editor, sets Lua syntax based on .lua file extension
    editor.openFile(script);

	// execute script from editor
	lua.doString(editor.getText());
	
	// call the script's setup() function
	lua.scriptSetup();
}

//--------------------------------------------------------------
void ofApp::update() {
	// call the script's update() function
	lua.scriptUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// call the script's draw() function
	lua.scriptDraw();
	
	if(!hideEditor) {
		editor.draw();
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	// call the script's exit() function
	lua.scriptExit();
	
	// clear the lua state
	lua.clear();
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
            case 'e':
                executeScript();
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
			case 't':
				hideEditor = !hideEditor;
				return;
			case 'k':
				editor.setAutoFocus(!editor.getAutoFocus());
				return;
        }
    }
	
	// sedn regular key pressed to script if the editor is hidden
	if(hideEditor) {
		lua.scriptKeyPressed(key);
	}
	else {
		editor.keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	editor.resize(w, h);
}

//--------------------------------------------------------------
void ofApp::errorReceived(string& msg) {
	ofLogNotice() << "got a script error: " << msg;
}

//--------------------------------------------------------------
void ofApp::executeScript() {
	// only reload lua state if running the entire script, otherwise
	// run text selection
	bool selection = editor.isSelection();
	if(!selection) {
		// exit, reinit the lua state, and reload the current script
		lua.scriptExit();
		lua.init(true);
	}
    lua.doString(editor.getText());
	if(!selection) {
		lua.scriptSetup();
	}
}

