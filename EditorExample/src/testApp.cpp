#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofSetFrameRate(30);
	
	// let's see what's going on inside
	ofSetLogLevel("ofxGLEditor", OF_LOG_VERBOSE);
	
	// setup the editor with a font
	// false to disable the current editor num in the upper left corner
	editor.setup("fonts/DroidSansMono.ttf", false); 
	
	// load a file into the current editor (1)
	editor.loadFile("hello.txt");
	ofLog() << "number of lines: " << editor.getNumLines();

	// add editor event listening
	ofAddListener(editor.runScriptEvent, this, &testApp::runScriptEvent);
	ofAddListener(editor.saveFileEvent, this, &testApp::saveFileEvent);
	
	//editor.setTextColor(ofColor::red); // dosen't work right yet ... sorry
	editor.setCursorColor(ofColor::blue); // block cursor color
	//editor.setAlpha(0.5); // main text, cursor, & highlight alpha
	
	// move the cursor
	//editor.setCurrentLine(0);
	ofLog() << "current line pos: " << editor.getCurrentLine();
}

//--------------------------------------------------------------
void testApp::update(){}

//--------------------------------------------------------------
void testApp::draw(){
	editor.draw();
	
	ofSetColor(255, 0, 0);
	
	// draw using the same font as the editor
	
	// if you disable the editor num, you can also draw it manually
	editor.drawString("Current editor: "+ofToString(editor.getCurrentEditor()), 10, 15);
	
	// draw the current & total line nums
	editor.drawString("Current line: "+ofToString(editor.getCurrentLine())
		+"/"+ofToString(editor.getNumLines()), ofGetWidth()-270, 15);
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
