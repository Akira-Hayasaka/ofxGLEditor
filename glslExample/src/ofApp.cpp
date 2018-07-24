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
	
	// setup color scheme via xml file
	colorScheme.loadFile("colorScheme.xml");
	editor.setColorScheme(&colorScheme);
	
	// setup GLSL syntax via xml file
	syntax.loadFile("glslSyntax.xml"); // lang: GLSL, file exts: frag & vert
	editor.getSettings().addSyntax(&syntax);
	editor.getSettings().printSyntaxes();
	
	// other settings
	editor.setLineNumbers(true);
	editor.setAutoFocus(true);
	
	// open file, sets syntax based on file extension
	shaderName = "shader";
	editor.openFile(shaderName+".frag");
	ofLogNotice() << "num chars: " << editor.getNumCharacters() << " num lines: " << editor.getNumLines();

    bToggleVisible = true;
	debug = false;
    
    /// shader stuff
    width = ofGetWidth();
    height = ofGetHeight();
    shader.load(shaderName);

    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
		ofClear(0, 0, 0, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    fbo.begin();
		ofClear(0, 0, 0, 0);
        shader.begin();
        shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
        shader.setUniform3f("iResolution", width, height, 1);
    
        ofSetColor(255, 255, 255);
        ofDrawRectangle(0, 0, width, height);
        shader.end();
    fbo.end();
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());

    if(bToggleVisible){
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
                bToggleVisible = !bToggleVisible;
            case 's': // save file
                editor.saveFile(shaderName+".frag");
                break;
			case 'e': // evaluate aka save & reload shader
                editor.saveFile(shaderName+".frag");
                shader.load(shaderName);
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
			case 'k':
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
