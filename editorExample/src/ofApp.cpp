#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	// handle ESC internally since we use it to exit selection
	ofSetEscapeQuitsApp(false);

	ofxEditor::loadFont("fonts/PrintChar21.ttf", 30);
	
	colorScheme.setWordColor("function", ofColor::fuchsia);
	colorScheme.setWordColor("end", ofColor::fuchsia);
	colorScheme.setStringColor(ofColor::yellow);
	colorScheme.setNumberColor(ofColor::orangeRed);
	colorScheme.setCommentColor(ofColor::gray);
	colorScheme.setSingleLineComment("--"); // lua style
	colorScheme.setMultiLineComment("--[[", "]]"); // lua style
	
	editor.setText("-- a single line comment\123456789012345678901234567890n\n--[[\n\ta multi line comment\n]]\n\nfunction setup\n\tprint(\"hello world\")\n\tprint(\"123string456\")\nend\n\nfunction draw\n\tof.setColor(200.1)\n\tof.line(10, 10, 100, 100)\nend\n\n--mix of numbers & text\n5hj55hj44\n \n\n\n\n\n\n\n\n\n\n\n\n\n");
	ofLogNotice() << "num chars: " << editor.getNumCharacters() << " num lines: " << editor.getNumLines();
	
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	editor.draw();
	//editor.drawGrid();
	
	ofSetColor(255);
	ofDrawBitmapString("fps: "+ofToString((int)ofGetFrameRate()), ofGetWidth()-70, ofGetHeight()-10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if(ofGetKeyPressed(OF_KEY_SUPER)) {
		switch(key) {
			case 'g':
				if(editor.getColorScheme()) {
					editor.clearColorScheme();
				}
				else {
					editor.setColorScheme(&colorScheme);
				}
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
void ofApp::windowResized(int w, int h){
	editor.resize(w, h);
}
