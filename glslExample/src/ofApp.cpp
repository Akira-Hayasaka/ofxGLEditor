//
//  Created by Joshua Batty <joshpbatty@gmail.com> on 26/03/2015.
//
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetVerticalSync(true);
	ofBackground(0);
	
	// handle ESC internally since we use it to exit selection
	ofSetEscapeQuitsApp(false);

	// make sure to load editor font before anything else!
    ofxEditor::loadFont("fonts/PrintChar21.ttf", 24);
	
	// setup color scheme and glsl syntax
	syntaxHighlight.setup();
	editor.setColorScheme(&syntaxHighlight.colorScheme);
	editor.getSettings().setLangSyntax("GLSL", &syntaxHighlight.syntax);
	editor.getSettings().setFileExtLang("frag", "GLSL");
	editor.getSettings().setFileExtLang("vert", "GLSL");
	
	editor.setLineNumbers(true);
	editor.setAutoFocus(true);
	
	// open test file, sets syntax based on filename
	shaderFileName = "EmptyShader.frag";
	editor.openFile(shaderFileName);
	ofLogNotice() << "num chars: " << editor.getNumCharacters() << " num lines: " << editor.getNumLines();

    bToggleVisable = true;
	debug = false;
    
    /// shader stuff
    width = ofGetWidth();
    height = ofGetHeight();
    shader.load("", shaderFileName);

    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
		ofClear(0, 0, 0, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    fbo.begin();
		ofClear(0,0,0,0);
        shader.begin();
        shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
        shader.setUniform3f("iResolution", width, height, 1);
    
        ofSetColor(255, 255, 255);
        ofRect(0, 0, width, height);
        shader.end();
    fbo.end();
    fbo.draw(0,0,ofGetWidth(),ofGetHeight());

    if(bToggleVisable){
        editor.draw();
    }
	
	if(debug) {
		ofSetColor(255);
		ofDrawBitmapString("fps: "+ofToString((int)ofGetFrameRate()), ofGetWidth()-70, ofGetHeight()-10);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	bool modifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	if(modifierPressed) {
		switch(key) {
            case 't':
                bToggleVisable = !bToggleVisable;
            case 's':
                editor.saveFile(shaderFileName);
                shader.load("", shaderFileName);
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
		}
	}
	editor.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	editor.resize(w, h);
}
