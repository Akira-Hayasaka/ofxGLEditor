#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);

	ofxEditor::loadFont("fonts/PrintChar21.ttf", 30);
	editor.setSize(ofGetWidth(), ofGetHeight());
	editor.setText("\n\nfunction setup\n\tprint(\"hello\")\nend\n\nfunction draw\n\tof.setColor(200)\n\tof.line(10, 10, 100, 100)\nend\n");
	
	highlights.setHighlight("function", ofColor(255, 0, 255));
	highlights.setHighlight("end", ofColor(255, 0, 255));
	highlights.setStringHighlight(ofColor(255, 255, 0));
	highlights.setNumberHighlight(ofColor(255, 100, 100));
	editor.setHighlights(&highlights);
	
	ofBackground(0);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	editor.draw();
//	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
	editor.setSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
