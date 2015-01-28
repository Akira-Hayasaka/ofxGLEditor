#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetVerticalSync(true);
	ofSetLogLevel("ofxLua", OF_LOG_VERBOSE);
		
	// scripts to run
	script = "scripts/liveCodingExample.lua";
	
	// init the lua state
	lua.init(true); // true because we want to stop on an error
	
	// listen to error events
	lua.addListener(this);
    
    // load editor font
    ofxEditor::loadFont("fonts/PrintChar21.ttf", 24);
	hideEditor = false;
    
    // sample lua syntax
	// lau 5.1 keywords from http://www.lua.org/manual/5.1/manual.html
    colorScheme.setWordColor("and", ofColor::fuchsia);
    colorScheme.setWordColor("end", ofColor::fuchsia);
	colorScheme.setWordColor("in", ofColor::fuchsia);
	colorScheme.setWordColor("repeat", ofColor::fuchsia);
	colorScheme.setWordColor("break", ofColor::fuchsia);
	colorScheme.setWordColor("false", ofColor::fuchsia);
	colorScheme.setWordColor("local", ofColor::fuchsia);
	colorScheme.setWordColor("return", ofColor::fuchsia);
	colorScheme.setWordColor("do", ofColor::fuchsia);
	colorScheme.setWordColor("for", ofColor::fuchsia);
	colorScheme.setWordColor("nil", ofColor::fuchsia);
	colorScheme.setWordColor("then", ofColor::fuchsia);
	colorScheme.setWordColor("else", ofColor::fuchsia);
	colorScheme.setWordColor("function", ofColor::fuchsia);
	colorScheme.setWordColor("not", ofColor::fuchsia);
	colorScheme.setWordColor("true", ofColor::fuchsia);
	colorScheme.setWordColor("elseif", ofColor::fuchsia);
	colorScheme.setWordColor("if", ofColor::fuchsia);
	colorScheme.setWordColor("or", ofColor::fuchsia);
	colorScheme.setWordColor("until", ofColor::fuchsia);
	colorScheme.setWordColor("while", ofColor::fuchsia);
    colorScheme.setSingleLineComment("--");
    colorScheme.setMultiLineComment("--[[", "]]");
    
    // syntax highlighter colors
    colorScheme.setStringColor(ofColor::yellow);
    colorScheme.setNumberColor(ofColor::orangeRed);
    colorScheme.setCommentColor(ofColor::gray);
    editor.setColorScheme(&colorScheme);
    
    // open script file into editor
    ofFile testFile;
    testFile.open(script, ofFile::ReadOnly);
    editor.setText(testFile.readToBuffer().getText());

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
			case 'z':
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

