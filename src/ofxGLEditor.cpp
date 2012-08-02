/*
 * Copyright (C) 2012 Akira Hayasaka & Dan Wilcox <danomatika@gmail.com> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
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
	
	Editor::InitFont(string_to_wstring(path));
	Editor::m_DoEffects = true;
	for(int i = 1; i < s_numEditors; i++) {
		Editor* editor = new Editor();
		glEditor.push_back(editor);
	}
	reShape();	
	
	currentEditor = 0;
}

//--------------------------------------------------------------
void ofxGLEditor::clear(){
	for(int i = 1; i < (int) glEditor.size(); i++){
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
		clearText();
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

	if(editor < 0 || (editor - 1) >= (int) glEditor.size()){
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
    
	if(editor < 0 || (editor - 1) >= (int) glEditor.size()){
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
void ofxGLEditor::clearText(int editor){
	if(editor < 0 || (editor - 1) >= (int) glEditor.size()){
		ofLogError("ofxGLEditor") << "cannot clear text in unknown editor " << editor;
		return;
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	else
		editor--;
	
	ofLogVerbose("ofxGLEditor") << "cleared text in editor" << currentEditor;
	glEditor[editor]->ClearAllText();
}

//--------------------------------------------------------------
void ofxGLEditor::clearAllText(){
	for(int i = 1; i < (int) glEditor.size(); i++) {
		glEditor[i]->ClearAllText();
	}
	ofLogVerbose("ofxGLEditor") << "cleared text in all editors";
}

//--------------------------------------------------------------
void ofxGLEditor::setCurrentEditor(int editor){
	
	if(editor < 0 && (editor - 1) >= (int) glEditor.size()){
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
	
	glEditor[currentEditor]->RenderString(ofToString(currentEditor+1), 0, 0);
	glEditor[currentEditor]->Render();
	
	ofPopStyle();
	ofPopMatrix();
	ofPopView();
	
	glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//--------------------------------------------------------------
void ofxGLEditor::reShape(){
	int w = (ofGetWindowMode() == OF_WINDOW)?ofGetViewportWidth():ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW)?ofGetViewportHeight():ofGetScreenHeight();
	for(int i = 0; i < (int) glEditor.size(); i++){
		glEditor[i]->Reshape(w, h);	
	}
}

//--------------------------------------------------------------
void ofxGLEditor::pasteFromClipBoard(){
	glEditor[currentEditor]->InsertText(clipBoard.getText());
}

//--------------------------------------------------------------
void ofxGLEditor::copyToClipBoard(){
	clipBoard.setText(wstring_to_string(glEditor[currentEditor]->GetText()));
}

//--------------------------------------------------------------
bool ofxGLEditor::loadFile(string filename, int editor){
	
	if(editor < 0 && (editor - 1) >= (int) glEditor.size()){
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
	
	if(editor < 0 && (editor - 1) >= (int) glEditor.size()){
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

// EDITOR

//--------------------------------------------------------------
void ofxGLEditor::Editor::InsertText(const string& s) {
	m_CopyBuffer = string_to_wstring(s);
	m_Text.insert(m_Position, m_CopyBuffer);
	m_Selection = false;
	m_Position += m_CopyBuffer.size();	
	ProcessTabs();
}

//--------------------------------------------------------------
void ofxGLEditor::Editor::RenderString(const string& s, float x, float y, ofFloatColor color) {
	wstring text = string_to_wstring(s);
	
	// the gl stuff is from GLEditor::render()
	
	glViewport(0, 0, m_Width,m_Height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-50, 50, -37.5, 37.5, 0, 10);

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);


	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT, GL_FILL);

	glLoadIdentity();
	
	// magic numbers wee!
	// kind of wonky but works ...
	glTranslatef(-50, 35.65, 0);
	glScalef(0.0005f, 0.0005f, 1);

	glPushMatrix();
	
	// draw char by char
	float xPos = x * 200, yPos = -y * 200; //< more magic nums here
	for(int i = 0; i < text.size(); ++i) {
	
		// new line?
		if(text[i] == '\n') {
			glPopMatrix();
			glPushMatrix();
			xPos = x * 200;
			yPos -= m_PolyGlyph->CharacterHeight('N') * 0.5;
			glTranslatef(xPos, yPos, 0);
		}
		else if(text[i] == '\t') {
			xPos += m_PolyGlyph->CharacterWidth(text['N']) * 4;
		}
		else {
			m_PolyGlyph->Render(text[i], color.r, color.g, color.b, color.a, xPos, yPos);
			xPos += m_PolyGlyph->CharacterWidth(text[i]) * 0.0005f;
		}
	}
	
	glPopMatrix();
	
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
	
