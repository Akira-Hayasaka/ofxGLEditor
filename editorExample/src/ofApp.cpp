#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(30);

	ofxEditor::loadFont("fonts/PrintChar21.ttf", 30);
	
	colorScheme.setWordColor("function", ofColor::fuchsia);
	colorScheme.setWordColor("end", ofColor::fuchsia);
	colorScheme.setStringColor(ofColor::yellow);
	colorScheme.setNumberColor(ofColor::orangeRed);
	colorScheme.setCommentColor(ofColor::gray);
	colorScheme.setSingleLineComment("--"); // lua style
	colorScheme.setMultiLineComment("--[[", "]]"); // lua style
	
	editor.setText("-- a single line comment\n--[[\n\ta multi line comment\n]]\n\nfunction setup\n\tprint(\"hello world\")\n\tprint(\"123string456\")\nend\n\nfunction draw\n\tof.setColor(200.1)\n\tof.line(10, 10, 100, 100)\nend\n\n--mix of numbers & text\n5hj55hj44\n \n\n\n\n\n\n\n\n\n\n\n\n\n");
	ofLogNotice() << "num chars: " << editor.getNumCharacters() << " num lines: " << editor.getNumLines();
	
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){

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
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

	// update size
	editor.setSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
