#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofeditor.setup();

	// if you have some script lang companion (e.g. ofxLua)	
	//ofAddListener(ofeditor.doCompileEvent, this, &yourClass::yourListener);
}

//--------------------------------------------------------------
void testApp::update(){
    
    if (ofGetKeyPressed(/*glutGetModifiers() & GLUT_ACTIVE_ALT*/OF_KEY_MODIFIER & OF_KEY_ALT)) {
        cout << "alt pressed!" << endl;
    }    
    
}

//--------------------------------------------------------------
void testApp::draw(){
	ofeditor.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	bool alt = ofeditor.isAltPressed();
	if (alt && key == 'f') {
		ofToggleFullscreen();
		ofeditor.reShape();
	}
	ofeditor.keyPressed(key);
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}