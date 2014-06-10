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
 *
 */
#include "ofxGLEditor.h"

using namespace fluxus;

#ifdef OFX_GLEDITOR_GLUT
	// for mapping special keys
	#ifndef __APPLE__
		#include <GL/glut.h>
	#else
		#include <GLUT/glut.h>
	#endif
#endif

#include "Repl.h"

//--------------------------------------------------------------
ofxGLEditor::ofxGLEditor(){
	bAltPressed = false;
	bShiftPressed = false;
	bControlPressed = false;
	currentEditor = 0;
	bHideEditor = false;
	bShowFileDialog = false;
	saveFiles.resize(s_numEditors);
}

//--------------------------------------------------------------
ofxGLEditor::~ofxGLEditor(){
	clear();
}

//--------------------------------------------------------------
bool ofxGLEditor::setup(string fontFile, bool enableRepl){
	
	// handle ESC internally since we use it to exit the file browser
	ofSetEscapeQuitsApp(false);
	
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
	
	if(enableRepl) {
		Repl *repl = new Repl(this);
		glEditors.push_back(repl);
	}
	else {
		glEditors.push_back(NULL); // no Repl
	}
	for(int i = 1; i < s_numEditors; i++){
		Editor* editor = new Editor();
		glEditors.push_back(editor);
	}
	
	GLFileDialog::InitFont(string_to_wstring(path));
	glFileDialog = new FileDialog();
	glFileDialog->SetPath(string_to_wstring(ofToDataPath("")));
	
	reShape();
	
	currentEditor = 1;
}

//--------------------------------------------------------------
void ofxGLEditor::clear(){
	for(int i = 0; i < (int) glEditors.size(); i++){
		if(glEditors[i] != NULL)
			delete glEditors[i];
	}
	glEditors.clear();
	if(glFileDialog != NULL) {
		delete glFileDialog;
	}
}

//--------------------------------------------------------------
void ofxGLEditor::keyPressed(int key){

// check modifier keys
#ifdef OFX_GLEDITOR_GLUT // legacy Glut support
	// from Rick Companje's ofxKeyMap 2009.09.17
	// https://github.com/companje/ofxKeyMap
	// note: no super key support in Glut
	#ifdef TARGET_WIN32
		bAltPressed = (bool) ((GetKeyState(VK_MENU) & 0x80) > 0);
		bShiftPressed = (bool) ((GetKeyState(VK_SHIFT) & 0x80) > 0);
		bControlPressed = (bool) ((GetKeyState(VK_CONTROL) & 0x80) > 0);
	#else
		bAltPressed = (bool) (glutGetModifiers() & GLUT_ACTIVE_ALT);
		bShiftPressed = (bool) (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		bControlPressed = (bool) (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	#endif
#else // GLFW
	// we get 2 key events for every modifier, so try to catch them all
	switch(key){
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
			bAltPressed = true;
			return;
		case OF_KEY_SHIFT: case OF_KEY_LEFT_SHIFT: case OF_KEY_RIGHT_SHIFT:
			bShiftPressed = true;
			return;
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
			bControlPressed = true;
			return;
		case OF_KEY_SUPER: case OF_KEY_LEFT_COMMAND: case OF_KEY_RIGHT_COMMAND:
			bSuperPressed = true;
			return;
		default: break;
	}
#if (OF_VERSION_MAJOR == 0) && (OF_VERSION_MINOR == 8) && (OF_VERSION_PATCH == 1)
#pragma warning "using GLFW modifier key hack, may not work with non-US keyboards"
	// TODO: hack for non shifted chars in OF 0.8.1 until a fix comes with the
	//       updated GLFW in OF 0.8.2
	// from https://github.com/openframeworks/openFrameworks/issues/2554
	//
	// WARNING: This probably does not work with non US keyboards, it's just a
	//          hack to get it working in GLFW for now. In the meantime it's
	//          recommended to use the ofAppGlutWindow:
	//          see https://github.com/openframeworks/openFrameworks/issues/2562
	//
	// Another option is to simply edit the keymap below to match your keyboard
	// for a quick fix.
	if(bShiftPressed){
		switch (key){
			case '1':  key = '!'; break;
			case '2':  key = '@'; break;
			case '3':  key = '#'; break;
			case '4':  key = '$'; break;
			case '5':  key = '%'; break;
			case '6':  key = '^'; break;
			case '7':  key = '&'; break;
			case '8':  key = '*'; break;
			case '9':  key = '('; break;
			case '0':  key = ')'; break;
			case '-':  key = '_'; break;
			case '=':  key = '+'; break;
			case '[':  key = '{'; break;
			case ']':  key = '}'; break;
			case '\\': key = '|'; break;
			case ';':  key = ':'; break;
			case '\'': key = '"'; break;
			case ',':  key = '<'; break;
			case '.':  key = '>'; break;
			case '/':  key = '?'; break;
			default: break;
		}
	}
#endif
#endif
	
//	ofLog() << "alt: " << bAltPressed << " shft: " << bShiftPressed
//	        << " ctl: " << bControlPressed << " spr: " << bSuperPressed;
//	ofLog() << "key " << key << " " << (char) key;
	
	int mod = 0;
	if(bShiftPressed){
		mod = 1; // GLUT_ACTIVE_SHIFT
	}
	else if(bControlPressed){
		mod = mod | 2; // GLUT_ACTIVE_CTRL
	}
	else if(bAltPressed){
		mod = mod | 4; // GLUT_ACTIVE_ALT
	}
	
	int special = -1;
	if(key == OF_KEY_LEFT || key == OF_KEY_UP
		|| key == OF_KEY_RIGHT || key == OF_KEY_DOWN
		|| key == OF_KEY_PAGE_UP || key == OF_KEY_PAGE_DOWN
		|| key == OF_KEY_HOME || key == OF_KEY_END || key == OF_KEY_INSERT){
		
		special = key - 256;
		key = 0;
	}
	
	if(bAltPressed) {
		switch(key) {
			case 'e': {
				string script = getText();
				int ed = currentEditor;
				ofNotifyEvent(executeScriptEvent, ed, this);
				break;
			}
			case 'b':
				glEditors[currentEditor]->BlowupCursor();
				break;
			case 'h': bHideEditor = !bHideEditor; break;
			case 'a': clearText(); break;
			case 'c': copyToClipBoard(); break;
			case 'v': pasteFromClipBoard(); break;
			case 's': {
				// show save as dialog on empty name
				if(saveFiles[currentEditor] == "") {
					glFileDialog->SetSaveAsMode(true);
					bShowFileDialog = !bShowFileDialog;
				}
				else {
					saveFile(saveFiles[currentEditor]);
				}
				break;
			}
			case 'l':
				if(currentEditor != 0) {
					glFileDialog->SetSaveAsMode(false);
					glFileDialog->Refresh();
					bShowFileDialog = !bShowFileDialog;
				}
				break;
			case 'd':
				if(currentEditor != 0) {
					glFileDialog->SetSaveAsMode(true);
					bShowFileDialog = !bShowFileDialog;
				}
				break;
			case '-':
				if(currentEditor != 0) {
					glEditors[currentEditor]->m_Alpha -= 0.05;
					if(glEditors[currentEditor]->m_Alpha < 0) {
						glEditors[currentEditor]->m_Alpha = 0;
					}
				}
				break;
			case '=':
				if(currentEditor != 0) {
					glEditors[currentEditor]->m_Alpha += 0.05;
					if(glEditors[currentEditor]->m_Alpha > 1) {
						glEditors[currentEditor]->m_Alpha = 1;
					}
				}
				break;
			case 'r': case '0': // Repl
				if(glEditors[0]) {
					currentEditor = 0;
				}
				break;
			case '1': currentEditor = 1; break;
			case '2': currentEditor = 2; break;
			case '3': currentEditor = 3; break;
			case '4': currentEditor = 4; break;
			case '5': currentEditor = 5; break;
			case '6': currentEditor = 6; break;
			case '7': currentEditor = 7; break;
			case '8': currentEditor = 8; break;
			case '9': currentEditor = 9; break;
			default: break;
		}
		return; // catch modified keys
	}
	
	if(key != 0 || special != -1) {
		if(bShowFileDialog) {
			if(key == OF_KEY_ESC) {
				bShowFileDialog = false;
			}
			glFileDialog->Handle(-1, key, special, 0, ofGetMouseX(), ofGetMouseY(), mod);
			if(glFileDialog->GetOutput() != L"") {
				if(glFileDialog->GetSaveAsMode()) {
					saveFiles[currentEditor] = wstring_to_string(glFileDialog->GetOutput());
					saveFile(saveFiles[currentEditor]);
				}
				else {
					if(loadFile(wstring_to_string(glFileDialog->GetOutput()))) {
						saveFiles[currentEditor] = wstring_to_string(glFileDialog->GetOutput());
					}
				}
				glFileDialog->Clear();
				bShowFileDialog = false;
			}
		}
		else if(!bHideEditor) {
//			if(key == OF_KEY_ESC) {
//				ofExit();
//			}
			glEditors[currentEditor]->Handle(-1, key, special, 0, ofGetMouseX(), ofGetMouseY(), mod);
		}
	}
}

//--------------------------------------------------------------
void ofxGLEditor::keyReleased(int key){
#ifndef OFX_GLEDITOR_GLUT
	// GLFW
	switch(key){
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
			bAltPressed = false;
			break;
		case OF_KEY_SHIFT: case OF_KEY_LEFT_SHIFT: case OF_KEY_RIGHT_SHIFT:
			bShiftPressed = false;
			break;
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
			bControlPressed = false;
			break;
		case OF_KEY_SUPER: case OF_KEY_LEFT_COMMAND: case OF_KEY_RIGHT_COMMAND:
			bSuperPressed = false;
			break;
	}
#endif
}

//--------------------------------------------------------------
void ofxGLEditor::setText(string text, int editor){

	editor = getEditorIndex(editor);
	if(editor == -1){
		ofLogError("ofxGLEditor") << "cannot set text into unknown editor " << editor;
		return;
	}

    Editor *e = glEditors[editor];
	e->SetText(string_to_wstring(text));
	e->CountLines();
};

//--------------------------------------------------------------
string ofxGLEditor::getText(int editor){
    
	editor = getEditorIndex(editor);
	if(editor == -1){
		ofLogError("ofxGLEditor") << "cannot get text from unknown editor " << editor;
		return "";
	}
	
	// add an endline if there isn't one already
	string text = wstring_to_string(glEditors[editor]->GetAllText());
	if(text[text.size()-1] != '\n') 
		text += "\n";
	return text;
};

//--------------------------------------------------------------
void ofxGLEditor::clearText(int editor){

	editor = getEditorIndex(editor);
	if(editor == -1){
		ofLogError("ofxGLEditor") << "cannot clear text in unknown editor " << editor;
		return;
	}
	
	ofLogVerbose("ofxGLEditor") << "cleared text in editor" << currentEditor;
	glEditors[editor]->ClearAllText();
}

//--------------------------------------------------------------
void ofxGLEditor::clearAllText(){
	for(int i = 1; i < (int) glEditors.size(); i++){
		glEditors[i]->ClearAllText();
	}
	ofLogVerbose("ofxGLEditor") << "cleared text in all editors";
}

//--------------------------------------------------------------
void ofxGLEditor::setCurrentEditor(int editor){
	
	if(editor < 0 && (editor - 1) >= (int) glEditors.size()){
		ofLogError("ofxGLEditor") << "cannot set unknown editor " << editor;
		return;
	}
	
	currentEditor = editor;
	ofLogVerbose("ofxGLEditor") << "setting the current editor to " << currentEditor;
}

//--------------------------------------------------------------
int ofxGLEditor::getCurrentEditor(){
	return currentEditor;
}

//--------------------------------------------------------------
void ofxGLEditor::setEditorFilename(int editor, string filename) {
	
	if(editor < 0 && (editor - 1) >= (int) glEditors.size()){
		ofLogError("ofxGLEditor") << "cannot set filename for unknown editor " << editor;
		return;
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	
	ofLogVerbose("ofxGLEditor") << "setting filename for editor " << editor
		<< " to \"" << ofFilePath::getFileName(filename) << "\"";

	saveFiles[editor] = filename;
}
	
//--------------------------------------------------------------
string ofxGLEditor::getEditorFilename(int editor) {
	if(editor < 0 && (editor - 1) >= (int) glEditors.size()){
		ofLogError("ofxGLEditor") << "cannot get filename for unknown editor " << editor;
		return "";
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	return saveFiles[editor];
}

//--------------------------------------------------------------
void ofxGLEditor::evalReplReturn(const string &text) {
	if(glEditors[0]) {
		Repl *repl = (Repl*) glEditors[0];
		repl->printEvalReturn(string_to_wstring(text));
	}
}

//--------------------------------------------------------------
void ofxGLEditor::setReplBanner(const string &text) {
	Repl::s_banner = string_to_wstring(text);
}

string ofxGLEditor::getReplBanner() {
	return wstring_to_string(Repl::s_banner);
}
	
//--------------------------------------------------------------
void ofxGLEditor::setReplPrompt(const string &text) {
	Repl::s_prompt = string_to_wstring(text);
}

string ofxGLEditor::getReplPrompt() {
	return wstring_to_string(Repl::s_prompt);
}

//--------------------------------------------------------------
void ofxGLEditor::setPath(string path) {
	// make sure there is a trailing /
	glFileDialog->SetPath(string_to_wstring(ofFilePath::addTrailingSlash(path)));
	if(bShowFileDialog) {
		glFileDialog->Refresh();
	}
}

//--------------------------------------------------------------
void ofxGLEditor::setHidden(bool hidden) {
	bHideEditor = !bHideEditor;
}

//--------------------------------------------------------------
void ofxGLEditor::draw(){
	ofPushView();
	ofPushMatrix();
	ofPushStyle();
	
	ofEnableAlphaBlending();
	
	if(bShowFileDialog) {
		glFileDialog->Render();
	}
	else if(!bHideEditor) {
		glEditors[currentEditor]->Render();
	}
	
	ofDisableAlphaBlending();
	
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
	for(int i = 0; i < (int) glEditors.size(); i++){
		if(glEditors[i]) glEditors[i]->Reshape(w, h);
	}
	glFileDialog->Reshape(w, h);
}

//--------------------------------------------------------------
void ofxGLEditor::pasteFromClipBoard(){
	glEditors[currentEditor]->InsertText(clipBoard.getText());
}

//--------------------------------------------------------------
void ofxGLEditor::copyToClipBoard(){
	clipBoard.setText(wstring_to_string(glEditors[currentEditor]->GetText()));
}

//--------------------------------------------------------------
bool ofxGLEditor::loadFile(string filename, int editor){
	
	if(editor < 0 && (editor - 1) >= (int) glEditors.size()){
		ofLogError("ofxGLEditor") << "cannot load into unknown editor " << editor;
		return false;
	}
	else if(editor == 0){
		editor = currentEditor;
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
bool ofxGLEditor::saveFile(string filename, int editor){
	
	if(editor < 0 && (editor - 1) >= (int) glEditors.size()){
		ofLogError("ofxGLEditor") << "cannot save from unknown editor " << editor;
		return "";
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	
	string path = ofToDataPath(filename);
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

//--------------------------------------------------------------
void ofxGLEditor::drawString(const string& s, float x, float y){
	glEditors[currentEditor]->RenderString(s, x, y);//, ofFloatColor(ofGetStyle().color));
}

void ofxGLEditor::drawString(const string& s, ofPoint& p){
	drawString(s, p.x, p.y);
}

//--------------------------------------------------------------
// TODO: this works, but alpha blending/glyph size means it looks terrible
//void ofxGLEditor::setTextColor(ofColor c){
//	ofFloatColor fc(c);
//	Editor::m_TextColourRed = fc.r;
//	Editor::m_TextColourGreen = fc.g;
//	Editor::m_TextColourBlue = fc.b;
//	Editor::m_TextColourAlpha = fc.a;
//}

//--------------------------------------------------------------
//ofColor ofxGLEditor::getTextColor(){
//	return ofFloatColor(
//		Editor::m_TextColourRed,
//		Editor::m_TextColourGreen,
//		Editor::m_TextColourBlue,
//		Editor::m_TextColourAlpha);
//}
	
//--------------------------------------------------------------
void ofxGLEditor::setCursorColor(ofColor c){
	ofFloatColor fc(c);
	Editor::m_CursorColourRed = fc.r;
	Editor::m_CursorColourGreen = fc.g;
	Editor::m_CursorColourBlue = fc.b;
	Editor::m_CursorColourAlpha = fc.a;
}

//--------------------------------------------------------------
ofColor ofxGLEditor::getCursorColor(){
	return ofFloatColor(
		Editor::m_CursorColourRed,
		Editor::m_CursorColourGreen,
		Editor::m_CursorColourBlue,
		Editor::m_CursorColourAlpha);
}
	
//--------------------------------------------------------------
void ofxGLEditor::setAlpha(float a){
	Editor::m_Alpha = ofClamp(a, 0, 1.0);
}

//--------------------------------------------------------------
float ofxGLEditor::getAlpha(){
	return Editor::m_Alpha;
}

//--------------------------------------------------------------
void ofxGLEditor::setCurrentLine(unsigned int line, int editor){
	
	editor = getEditorIndex(editor);
	if(editor == -1){
		ofLogError("ofxGLEditor") << "cannot set current line in unknown editor " << editor;
		return;
	}
		
	Editor *e = glEditors[editor];
	if(line >= e->GetLineCount()){
		ofLogError("ofxGLEditor") << "cannot set current line, given line "
			<< line << " is >= num lines " << e->GetLineCount();
		return;
	}
	e->SetCurLine(line);
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getCurrentLine(int editor){
	editor = getEditorIndex(editor);
	if(editor == -1){
		ofLogError("ofxGLEditor") << "cannot get current line from unknown editor " << editor;
		return 0;
	}
	return glEditors[editor]->GetCurLine();
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getNumLines(int editor){
	editor = getEditorIndex(editor);
	if(editor == -1){
		ofLogError("ofxGLEditor") << "cannot get num lines from unknown editor " << editor;
		return 0;
	}
	return glEditors[editor]->GetLineCount();
}

// PRIVATE

//--------------------------------------------------------------
int ofxGLEditor::getEditorIndex(int editor){
	if(editor < 0 || (editor - 1) >= (int) glEditors.size()){
		return -1;
	}
	else if(editor == 0){
		editor = currentEditor;
	}
	return editor;
}

// EDITOR

//--------------------------------------------------------------
void ofxGLEditor::Editor::InsertText(const string& s){
	m_CopyBuffer = string_to_wstring(s);
	m_Text.insert(m_Position, m_CopyBuffer);
	m_Selection = false;
	m_Position += m_CopyBuffer.size();	
	ProcessTabs();
}

//--------------------------------------------------------------
void ofxGLEditor::Editor::RenderString(const string& s, float x, float y, ofFloatColor color){
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
	for(int i = 0; i < text.size(); ++i){
	
		// new line?
		if(text[i] == '\n'){
			glPopMatrix();
			glPushMatrix();
			xPos = x * 200;
			yPos -= m_PolyGlyph->CharacterHeight('N') * 0.5;
			glTranslatef(xPos, yPos, 0);
		}
		else if(text[i] == '\t'){
			xPos += m_PolyGlyph->CharacterWidth(text['N']) * 4;
		}
		else{
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

//--------------------------------------------------------------
void ofxGLEditor::Editor::CountLines(){
	m_LineCount = 0;
	for(unsigned int i = 0; i < m_Text.size(); i++){
		if(m_Text[i] == '\n')
			m_LineCount++;
	}
}
	
