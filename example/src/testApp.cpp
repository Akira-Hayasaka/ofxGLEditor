#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofSetFrameRate(30);
	
	// let's see what's going on inside
	ofSetLogLevel("ofxGLEditor", OF_LOG_VERBOSE);
	
	// setup the editor with a font
	editor.setup("fonts/DroidSansMono.ttf");
	
	// load a file into the current editor (1)
	editor.loadFile("hello.txt");

	// add editor event listening
	ofAddListener(editor.runScriptEvent, this, &testApp::runScriptEvent);
	ofAddListener(editor.saveFileEvent, this, &testApp::saveFileEvent);
}

//--------------------------------------------------------------
void testApp::update(){}

//--------------------------------------------------------------
void testApp::draw(){
	editor.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	// see ofxGLEditor.h for key commands
	editor.keyPressed(key);
	
	// check for modifiers
	bool alt = editor.isAltPressed();
	if(alt && key == 'f'){
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

//--------------------------------------------------------------
void testApp::runScriptEvent(int & whichEditor){
	// received on editor ALT + r

	// if you have some scripting language (e.g. ofxLua)
	cout << "run script in editor " << whichEditor << endl;
}

//--------------------------------------------------------------
void testApp::saveFileEvent(int & whichEditor){
	// received on editor ALT + s
	
	cout << "save file from editor " << whichEditor << endl;
	editor.saveFile(ofToString(whichEditor)+".txt", whichEditor, false);
}
