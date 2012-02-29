/*
 *  ofxGLEditor.cpp
 *  LiveCoding
 *
 *  Created by Makira on 11/07/05.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 *	Updated by Dan Wilcox <danomatika@gmail.com> 2012
 */

#include "ofxGLEditor.h"

// for mapping special keys
#ifndef __APPLE__
	#include <GL/glut.h>
#else
	#include <GLUT/glut.h>
#endif

using namespace fluxus;

//--------------------------------------------------------------
ofxGLEditor::ofxGLEditor(){
	bAltPressed = false;
	bShiftPressed = false;
	bControlPressed = false;
	currentEditor = 0;
}

//--------------------------------------------------------------
ofxGLEditor::~ofxGLEditor(){
	clear();
}

//--------------------------------------------------------------
bool ofxGLEditor::setup(string fontFile){
	
	string path = ofToDataPath(fontFile);
	if(!ofFile::doesFileExist(path)){
		ofLogFatalError("ofxGLEditor") << "couldn't find font \""
			<< path << "\"";
			return false;
	}
	
	ofLogVerbose("ofxGLEditor") << "setting up with font \""
		<< ofFilePath::getFileName(path) << "\"";
	
	GLEditor::InitFont(string_to_wstring(path));
	GLEditor::m_DoEffects = true;
	for(int i = 1; i < s_numEditors; i++) {
		GLEditor* editor = new GLEditor();
		glEditor.push_back(editor);
	}
	reShape();	
	
	currentEditor = 0;
}

//--------------------------------------------------------------
void ofxGLEditor::clear(){
	for(int i = 1; i < glEditor.size(); i++){
		if(glEditor[i] != NULL)
			delete glEditor[i];
	}
	glEditor.clear();
}

//--------------------------------------------------------------
void ofxGLEditor::keyPressed(int key){
	
	#ifdef TARGET_WIN32
		bAltPressed = (bool) ((GetKeyState(VK_MENU) & 0x80) > 0);
		bShiftPressed = (bool) ((GetKeyState(VK_SHIFT) & 0x80) > 0);
		bControlPressed = (bool) ((GetKeyState(VK_CONTROL) & 0x80) > 0);
    #else
		bAltPressed = (bool) (glutGetModifiers() & GLUT_ACTIVE_ALT);
		bShiftPressed = (bool) (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		bControlPressed = (bool) (glutGetModifiers() & GLUT_ACTIVE_CTRL);
    #endif
	
	int mod = 0;
	if(bShiftPressed){
		mod = 1;
	}
	else if (bControlPressed){
		mod = 2;
	}
	else if (bAltPressed){
		mod = 4;
	}
	
	int special = 0;
	if(key == OF_KEY_LEFT || key == OF_KEY_UP
		|| key == OF_KEY_RIGHT || key == OF_KEY_DOWN
		|| key == OF_KEY_PAGE_UP || key == OF_KEY_PAGE_DOWN
		|| key == OF_KEY_HOME || key == OF_KEY_END || key == OF_KEY_INSERT){
		
		special = key - 256;
		key = 0;
	}
	
	if(bAltPressed && key == 'r'){
		string script = getText();
		int ed = currentEditor + 1;
		ofNotifyEvent(runScriptEvent, ed, this);
	}else if(bAltPressed && key == 'b'){
		glEditor[currentEditor]->BlowupCursor();
	}else if(bAltPressed && key == 'a'){
		glEditor[currentEditor]->ClearAllText();
	}else if(bAltPressed && key == 'c'){
		copyToClipBoard();
	}else if(bAltPressed && key == 'v'){
		pasteFromClipBoard();
	}else if(bAltPressed && key == 's'){
		int ed = currentEditor + 1;
		ofNotifyEvent(saveFileEvent, ed, this);
	}else if(bAltPressed && key == 'd'){
		currentEditor = 0;
	}else if(bAltPressed && key == '1'){
		currentEditor = 0;
	}else if(bAltPressed && key == '2'){
		currentEditor = 1;
	}else if(bAltPressed && key == '3'){
		currentEditor = 2;
	}else if(bAltPressed && key == '4'){
		currentEditor = 3;
	}else if(bAltPressed && key == '5'){
		currentEditor = 4;
	}else if(bAltPressed && key == '6'){
		currentEditor = 5;
	}else if(bAltPressed && key == '7'){
		currentEditor = 6;
	}else if(bAltPressed && key == '8'){
		currentEditor = 7;
	}else if(bAltPressed && key == '9'){
		currentEditor = 8;
	}
	glEditor[currentEditor]->Handle(-1, key, special, 0, ofGetMouseX(), ofGetMouseY(), mod);
}

//--------------------------------------------------------------
void ofxGLEditor::setText(string text, int editor){

	if(editor < 0 || (editor - 1) >= glEditor.size()){
		ofLogError("ofxGLEditor") << "cannot set text into unknown editor " << editor;
		return;
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	else
        editor--; 
    
    glEditor[editor]->SetText(string_to_wstring(text));
};

//--------------------------------------------------------------
string ofxGLEditor::getText(int editor){
    
	if(editor < 0 || (editor - 1) >= glEditor.size()){
		ofLogError("ofxGLEditor") << "cannot get text from unknown editor " << editor;
		return "";
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	else
		editor--;
	
	// add an endline if there isn't one already
	string text = wstring_to_string(glEditor[editor]->GetAllText());
	if(text[text.size()-1] != '\n') 
		text += "\n";
	return text;
};

//--------------------------------------------------------------
void ofxGLEditor::setCurrentEditor(int editor){
	
	if(editor < 0 && (editor - 1) >= glEditor.size()){
		ofLogError("ofxGLEditor") << "cannot set unkown editor " << editor;
		return;
	}
	currentEditor = editor - 1;
	ofLogVerbose("ofxGLEditor") << "setting the current editor to " << currentEditor;
}

//--------------------------------------------------------------
int ofxGLEditor::getCurrentEditor(){
	return currentEditor + 1;
}

//--------------------------------------------------------------
void ofxGLEditor::draw(){
	ofPushView();
	ofPushMatrix();
	ofPushStyle();
	
	glEditor[currentEditor]->Render();
	
	ofPopStyle();
	ofPopMatrix();
	ofPopView();
}

//--------------------------------------------------------------
void ofxGLEditor::reShape(){

	int w = (ofGetWindowMode() == OF_WINDOW)?ofGetViewportWidth():ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW)?ofGetViewportHeight():ofGetScreenHeight();
	for(int i = 0; i < glEditor.size(); i++){
		glEditor[i]->Reshape(w, h);	
	}
}

//--------------------------------------------------------------
void ofxGLEditor::pasteFromClipBoard(){
	
	wstring m_CopyBuffer = string_to_wstring(clipBoard.getText());
	glEditor[currentEditor]->m_Text.insert(glEditor[currentEditor]->m_Position, m_CopyBuffer);
	glEditor[currentEditor]->m_Selection = false;
	glEditor[currentEditor]->m_Position += m_CopyBuffer.size();	
	glEditor[currentEditor]->ProcessTabs();
}

//--------------------------------------------------------------
void ofxGLEditor::copyToClipBoard(){
	
	string text = wstring_to_string(glEditor[currentEditor]->GetText());
	clipBoard.setText(text);
}

//--------------------------------------------------------------
bool ofxGLEditor::loadFile(string filename, int editor){
	
	if(editor < 0 && (editor - 1) >= glEditor.size()){
		ofLogError("ofxGLEditor") << "cannot load into unknown editor " << editor;
		return false;
	}
	else if(editor == 0){
		editor = currentEditor + 1;
	}
	
	string path = ofToDataPath(filename);
	ofLogVerbose("ofxGLEditor") << "loading \"" << ofFilePath::getFileName(path)
		<< "\" into editor " << editor;
	
	ofFile file;
	if(!file.open(ofToDataPath(path), ofFile::ReadOnly)){
		ofLogError() << "ofxGLEditor: couldn't load \""
			<< ofFilePath::getFileName(path) << "\"";
		return false;
	}
	
	setText(file.readToBuffer().getText(), editor);
	file.close();
	
	return true;
}

//--------------------------------------------------------------
bool ofxGLEditor::saveFile(string filename, int editor, bool addTimestamp){
	
	if(editor < 0 && (editor - 1) >= glEditor.size()){
		ofLogError("ofxGLEditor") << "cannot save from unknown editor " << editor;
		return "";
	}
	else if(editor == 0){
		editor = currentEditor + 1;
	}
	
	string path = ofToDataPath(filename);
	if(addTimestamp){
		path =  ofFilePath::getEnclosingDirectory(path)
				+ ofFilePath::getBaseName(path)
				+ "-" + ofGetTimestampString()
				+ "." + ofFilePath::getFileExt(path);
	}
	
	ofLogVerbose("ofxGLEditor") << "saving editor " << editor
		<< " to \"" << ofFilePath::getFileName(path) << "\"";
	
	ofFile file;
	if(!file.open(path, ofFile::WriteOnly)){
		ofLogError() << "ofxGLEditor: couldn't open \""
			<< ofFilePath::getFileName(path) << "\" for saving";
		return false;
	}
	
	file << getText(editor);
	file.close();
		
	return true;
}
