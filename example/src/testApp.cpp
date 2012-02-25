#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	editor.setup();

	// if you have some script lang companion (e.g. ofxLua)	
	//ofAddListener(ofeditor.doCompileEvent, this, &yourClass::yourListener);
}

//--------------------------------------------------------------
void testApp::update(){
    
    if (editor.isAltPressed()) {
        cout << "alt pressed!" << endl;
    }    
    
}

//--------------------------------------------------------------
void testApp::draw(){
	editor.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	editor.keyPressed(key);
	bool alt = editor.isAltPressed();
	if (alt && key == 'f') {
		ofToggleFullscreen();
		editor.reShape();
	}
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