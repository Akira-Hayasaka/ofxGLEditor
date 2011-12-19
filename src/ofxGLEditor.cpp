/*
 *  ofxGLEditor.cpp
 *  LiveCoding
 *
 *  Created by Makira on 11/07/05.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 */

#include "ofxGLEditor.h"

void ofxGLEditor::setup(string fontFile) {
	
	string font = fontFile;	
	string path = ofToDataPath(font);
	for (int i = 1; i < 10; i++) {
		GLEditor::InitFont(string_to_wstring(path));		
		GLEditor* editor = new GLEditor();
		glEditor.push_back(editor);
		
	}
	reShape();	
	
	currentEditor = 1;
	
}

void ofxGLEditor::keyPressed(int key) {
	   
	bool alt = kmap.isAltDown();
	bool shift = kmap.isShiftDown();
	bool ctrl = kmap.isControlDown();
	
	int mod = 0;
	if (shift) {
		mod = 1;
	}else if (ctrl) {
		mod = 2;
	}else if (alt) {
		mod = 4;
	}
	
	int special = 0;
	if (
		key == OF_KEY_LEFT || key == OF_KEY_UP || 
		key == OF_KEY_RIGHT || key == OF_KEY_DOWN ||
		key == OF_KEY_PAGE_UP || key == OF_KEY_PAGE_DOWN ||
		key == OF_KEY_HOME || key == OF_KEY_END || key == OF_KEY_INSERT
		) 
	{
		special = key - 256;
		key = 0;
	}
	
	if (alt && key == 'r') {
		int i = 0;
		string script;
		for (int i = 0; i < glEditor.size(); i++) {
			script += wstring_to_string(glEditor[i]->GetAllText());
			script += "\n";
		}
		ofNotifyEvent(doCompileEvent, script);
	}else if (alt && key == 'b') {
		glEditor[currentEditor]->BlowupCursor();
	}else if (alt && key == 'a') {
		glEditor[currentEditor]->ClearAllText();
	}else if (alt && key == 'c') {
		copyToClipBoard();
	}else if (alt && key == 'v') {
		pasteFromClipBoard();
	}else if (alt && key == 's') {
		saveToFile();
	}else if (alt && key == 'd') {
		currentEditor = 1;
	}else if (alt && key == '1') {
		currentEditor = 1;
	}else if (alt && key == '2') {
		currentEditor = 2;
	}else if (alt && key == '3') {
		currentEditor = 3;
	}else if (alt && key == '4') {
		currentEditor = 4;
	}else if (alt && key == '5') {
		currentEditor = 5;
	}else if (alt && key == '6') {
		currentEditor = 6;
	}else if (alt && key == '7') {
		currentEditor = 7;
	}else if (alt && key == '8') {
		currentEditor = 8;
	}else if (alt && key == '9') {
		currentEditor = 9;
	}
	glEditor[currentEditor]->Handle(-1, key, special, 0, ofGetMouseX(), ofGetMouseY(), mod);

}

void ofxGLEditor::draw() {
	
	ofPushView();
	ofPushMatrix();
	ofPushStyle();
	glEditor[currentEditor]->Render();
	ofPopStyle();
	ofPopMatrix();
	ofPopView();
	glDisable(GL_LIGHTING);
	
}

bool ofxGLEditor::isAltPressed() {
	
	bool alt = kmap.isAltDown();
	if (alt) return true;
	return false;
	
}

void ofxGLEditor::reShape() {
	
	int w = (ofGetWindowMode() == OF_WINDOW)?ofGetWidth():ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW)?ofGetHeight():ofGetScreenHeight();
	for (int i = 0; i < glEditor.size(); i++) {
		glEditor[i]->Reshape(w*1.2, h);	
	}
	
}

void ofxGLEditor::pasteFromClipBoard() {
	
	wstring m_CopyBuffer = string_to_wstring(clipBoard.getTextFromPasteboard());
	glEditor[currentEditor]->m_Text.insert(glEditor[currentEditor]->m_Position,m_CopyBuffer);
	glEditor[currentEditor]->m_Selection=false;
	glEditor[currentEditor]->m_Position+=m_CopyBuffer.size();	
	glEditor[currentEditor]->ProcessTabs();
	
}

void ofxGLEditor::copyToClipBoard() {
	
	string script = wstring_to_string(glEditor[currentEditor]->GetText());
	clipBoard.setTextToPasteboard(script.c_str());
	
}

void ofxGLEditor::saveToFile() {
	
	string curPath = ofToDataPath("", false);
	ofSetDataPathRoot("../../../data/");
	
	string zero = "0";
	string y = ofToString(ofGetYear()); 
	string m = ofToString(ofGetMonth()); 
	if (m.size() == 1) m = zero+m;
	string d = ofToString(ofGetDay());
	if (d.size() == 1) d = zero+d;
	string hour = ofToString(ofGetHours());
	if (hour.size() == 1) hour = zero+hour;
	string min = ofToString(ofGetMinutes());
	if (min.size() == 1) min = zero+min;
	string sec = ofToString(ofGetSeconds());
	if (sec.size() == 1) sec = zero+sec;
	string ymdhms = y+m+d+hour+min+sec;
	
	string scriptPath = ymdhms;
	string file = ofToDataPath(scriptPath);
	ofstream myfile;
	myfile.open(file.c_str(), _S_out);
	string script;
	for (int i = 0; i < glEditor.size(); i++) {
		script += wstring_to_string(glEditor[i]->GetAllText());
		script += "\n";
	}
	myfile << script;
	myfile.close();		
	ofSetDataPathRoot(curPath);
	cout << "saved" << endl;
}