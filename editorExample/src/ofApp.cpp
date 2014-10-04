#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);

	ofxEditor::loadFont("fonts/PrintChar21.ttf", 30);
	
	colorScheme.setWordColor("function", ofColor(255, 0, 255));
	colorScheme.setWordColor("end", ofColor(255, 0, 255));
	colorScheme.setStringColor(ofColor(255, 255, 0));
	colorScheme.setNumberColor(ofColor(255, 100, 100));
	editor.setColorScheme(colorScheme);
	
	editor.setSize(ofGetWidth(), ofGetHeight());
	editor.setText("function setup\n\tprint(\"123hello456\")\nend\n\nfunction draw\n\tof.setColor(200.1)\n\tof.line(10, 10, 100, 100)\nend\n5hj55hj44\n");
	
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

	switch(key) {
		case 'h':
			if(editor.getColorScheme()) {
				editor.clearColorScheme();
			}
			else {
				editor.setColorScheme(colorScheme);
			}
			break;
		case 'l':
			editor.setLineWrapping(!editor.getLineWrapping());
			break;
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
	editor.setSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
