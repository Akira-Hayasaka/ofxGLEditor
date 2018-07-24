/*
 * Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 */
#include "ofxGLEditor.h"

//--------------------------------------------------------------
ofxGLEditor::ofxGLEditor() {
	m_listener = NULL;
	m_fileDialog = NULL;
	m_saveFiles.resize(s_numEditors);
	m_currentEditor = 0;
	bModifierPressed = false;
	bHideEditor = false;
	bFlashEvalSelection = false;
}

//--------------------------------------------------------------
ofxGLEditor::~ofxGLEditor() {
	clear();
}

//--------------------------------------------------------------
void ofxGLEditor::setup(ofxGLEditorListener *listener, bool enableRepl) {
	
	clear();
	
	// handle ESC internally since we use it to exit the file browser
	ofSetEscapeQuitsApp(false);
	
	m_listener = listener;
	
	// create editors
	if(enableRepl) {
		ofxRepl *repl = new ofxRepl(m_settings);
		repl->setup(listener);
		m_editors.push_back(repl);
	}
	else {
		m_editors.push_back(NULL); // no Repl
	}
	for(int i = 1; i < s_numEditors; i++) {
		ofxEditor* editor = new ofxEditor(m_settings);
		m_editors.push_back(editor);
	}
	
	m_fileDialog = new ofxFileDialog(m_settings);
	m_fileDialog->setPath(ofToDataPath(""));
	
	resize();
	setAutoFocus(true);
	
	m_currentEditor = 1;
}

//--------------------------------------------------------------
void ofxGLEditor::clear() {
	m_listener = NULL;
	for(int i = 0; i < (int) m_editors.size(); i++) {
		if(m_editors[i] != NULL)
			delete m_editors[i];
	}
	m_editors.clear();
	if(m_fileDialog != NULL) {
		delete m_fileDialog;
	}
}

//--------------------------------------------------------------
void ofxGLEditor::draw() {
	ofPushView();
	ofPushMatrix();
	ofPushStyle();
	
		ofEnableAlphaBlending();
	
		if(!bHideEditor) {
			if(m_fileDialog->isActive()) {
				m_fileDialog->draw();
			}
			else {
				m_editors[m_currentEditor]->draw();
			}
		}
		else { // make sure to update animation timing even if not drawn
			m_editors[m_currentEditor]->updateTimestamps();
		}
		
		ofDisableAlphaBlending();
	
	ofPopStyle();
	ofPopMatrix();
	ofPopView();

//	glDisable(GL_LIGHTING);
//    glDisable(GL_DEPTH_TEST);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//--------------------------------------------------------------
void ofxGLEditor::keyPressed(int key) {

	// check modifier keys
	bModifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	
	// also check for ascii control chars: http://ascii-table.com/control-chars.php
	if(bModifierPressed) {
		switch(key) {
			
			case 'e': case 5:
				if(m_currentEditor != 0) {
					if(bFlashEvalSelection && m_editors[m_currentEditor]->isSelection()) {
						m_editors[m_currentEditor]->flashSelection();
					}
					string script = getText();
					if(m_listener) {
						m_listener->executeScriptEvent(m_currentEditor);
					}
				}
				return;
			
			case 't': case 20:
				bHideEditor = !bHideEditor;
				return;
			
			case 's': case 19:
				if(m_currentEditor != 0) {
					// show save as dialog on empty name
					if(m_saveFiles[m_currentEditor] == "") {
						m_fileDialog->setMode(ofxFileDialog::SAVEAS);
					}
					else {
						if(saveFile(m_saveFiles[m_currentEditor])) {
							if(m_listener) {
								m_listener->saveFileEvent(m_currentEditor);
							}
						}
					}
				}
				return;
			
			case 'd': case 4:
				if(m_currentEditor != 0) {
					m_fileDialog->setMode(ofxFileDialog::SAVEAS);
				}
				return;
				
			case 'o': case 15:
				if(m_currentEditor != 0) {
					m_fileDialog->setMode(ofxFileDialog::OPEN);
					m_fileDialog->refresh();
				}
				return;
				
			case '-':
				m_settings.setAlpha(m_settings.getAlpha()-0.05);
				return;
				
			case '=':
				m_settings.setAlpha(m_settings.getAlpha()+0.05);
				return;
				
			case 'r': case '0': case 18: // Repl
				if(m_editors[0]) {
					m_currentEditor = 0;
				}
				return;
				
			case '1': m_currentEditor = 1; return;
			case '2': m_currentEditor = 2; return;
			case '3': m_currentEditor = 3; return;
			case '4': m_currentEditor = 4; return;
			case '5': m_currentEditor = 5; return;
			case '6': m_currentEditor = 6; return;
			case '7': m_currentEditor = 7; return;
			case '8': m_currentEditor = 8; return;
			case '9': m_currentEditor = 9; return;
			
			default:
				break;
		}
	}
	
	if(m_fileDialog->isActive()) {
		m_fileDialog->keyPressed(key);
		if(m_fileDialog->getSelectedPath() != "") {
			if(m_fileDialog->getMode() == ofxFileDialog::SAVEAS) {
				m_saveFiles[m_currentEditor] = m_fileDialog->getSelectedPath();
				if(saveFile(m_saveFiles[m_currentEditor])) {
					if(m_listener) {
						m_listener->saveFileEvent(m_currentEditor);
					}
				}
			}
			else {
				if(openFile(m_fileDialog->getSelectedPath())) {
					m_saveFiles[m_currentEditor] = m_fileDialog->getSelectedPath();
					if(m_listener) {
						m_listener->openFileEvent(m_currentEditor);
					}
				}
			}
			m_fileDialog->clearSelectedPath();
			m_fileDialog->setActive(false);
		}
	}
	else if(!bHideEditor) {
		m_editors[m_currentEditor]->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxGLEditor::resize() {
	int w = (ofGetWindowMode() == OF_WINDOW) ? ofGetViewportWidth() : ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW) ? ofGetViewportHeight() : ofGetScreenHeight();
	resize(w, h);
}

//--------------------------------------------------------------
void ofxGLEditor::resize(int width, int height) {
	for(int i = 0; i < (int) m_editors.size(); i++) {
		if(m_editors[i]) m_editors[i]->resize(width, height);
	}
	m_fileDialog->resize(width, height);
}

//--------------------------------------------------------------
bool ofxGLEditor::openFile(std::string filename, int editor) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxGLEditor") << "cannot load into unknown editor " << editor;
		return false;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	if(editor == 0) {
		ofLogWarning("ofxGLEditor") << "ignoring load into repl";
		return false;
	}
	
	ofLogVerbose("ofxGLEditor") << "loading \"" << ofFilePath::getFileName(filename)
		<< "\" into editor " << editor;
	bool ret = m_editors[editor]->openFile(filename);
	if(ret) {
		m_saveFiles[editor] = ofToDataPath(filename);
	}
	return ret;
}

//--------------------------------------------------------------
bool ofxGLEditor::saveFile(std::string filename, int editor) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxGLEditor") << "cannot save from unknown editor " << editor;
		return "";
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	if(editor == 0) {
		ofLogWarning("ofxGLEditor") << "ignore save from repl";
		return false;
	}
	
	ofLogVerbose("ofxGLEditor") << "saving editor " << editor
		<< " to \"" << ofFilePath::getFileName(filename) << "\"";
	bool ret = m_editors[editor]->saveFile(filename);
	if(ret) {
		m_saveFiles[editor] = ofToDataPath(filename);
	}
	return ret;
}

//--------------------------------------------------------------
bool ofxGLEditor::saveFile(int editor) {
	if(m_saveFiles[editor] == "") {
		ofLogWarning("ofxGLEditor") << "ignore save, current filename not set";
		return false;
	}
	return saveFile(m_saveFiles[editor], editor);
}

//--------------------------------------------------------------
string ofxGLEditor::getText(int editor) {
    
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get text from unknown editor " << editor;
		return "";
	}
	
	// add an endline if there isn't one already
	string text = m_editors[editor]->getText();
	if(text[text.size()-1] != '\n') 
		text += "\n";
	return text;
}

//--------------------------------------------------------------
void ofxGLEditor::setText(std::string text, int editor) {

	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot set text into unknown editor " << editor;
		return;
	}

    ofxEditor *e = m_editors[editor];
	e->setText(text);
}

//--------------------------------------------------------------
void ofxGLEditor::insertText(std::string text, int editor) {

	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot insert text into unknown editor " << editor;
		return;
	}

    ofxEditor *e = m_editors[editor];
	e->insertText(text);
}

//--------------------------------------------------------------
void ofxGLEditor::clearText(int editor) {

	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot clear text in unknown editor " << editor;
		return;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	if(editor == 0) {
		ofLogWarning("ofxGLEditor") << "ignoring repl clearText(), use clearRepl() instead";
		return;
	}
	
	 // reset filename
	ofLogVerbose("ofxGLEditor") << "cleared text in editor" << m_currentEditor;
	m_editors[editor]->clearText();
	m_saveFiles[editor] = "";
}

//--------------------------------------------------------------
void ofxGLEditor::clearAllText() {
	for(int i = 1; i < (int) m_editors.size(); i++) {
		m_editors[i]->clearText();
		m_saveFiles[i] = "";
	}
	ofLogVerbose("ofxGLEditor") << "cleared text in all editors";
}

//--------------------------------------------------------------
void ofxGLEditor::setCurrentEditor(int editor) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxGLEditor") << "cannot set unknown editor " << editor;
		return;
	}
	
	m_currentEditor = editor;
	ofLogVerbose("ofxGLEditor") << "setting the current editor to " << m_currentEditor;
}

//--------------------------------------------------------------
int ofxGLEditor::getCurrentEditor() {
	return m_currentEditor;
}

//--------------------------------------------------------------
void ofxGLEditor::setEditorFilename(int editor, std::string filename) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxGLEditor") << "cannot set filename for unknown editor " << editor;
		return;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	ofLogVerbose("ofxGLEditor") << "setting filename for editor " << editor
		<< " to \"" << ofFilePath::getFileName(filename) << "\"";

	m_saveFiles[editor] = ofToDataPath(filename);
}
	
//--------------------------------------------------------------
std::string ofxGLEditor::getEditorFilename(int editor) {
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxGLEditor") << "cannot get filename for unknown editor " << editor;
		return "";
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	return m_saveFiles[editor];
}

//--------------------------------------------------------------
bool ofxGLEditor::isSelection(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot check selection from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->isSelection();
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getNumLines(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get num lines from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getNumLines();
}

//--------------------------------------------------------------
void ofxGLEditor::setCurrentLine(unsigned int line, int editor) {
	
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot set current line in unknown editor " << editor;
		return;
	}
		
	ofxEditor *e = m_editors[editor];
	if(line >= e->getNumLines()) {
		ofLogError("ofxGLEditor") << "cannot set current line, given line "
			<< line << " is >= num lines " << e->getNumLines();
		return;
	}
	e->setCurrentLine(line);
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getCurrentLine(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get current line from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentLine();
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getCurrentLinePos(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get current line pos from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentLinePos();
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getCurrentLineLen(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get current line len from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentLineLen();
}

//--------------------------------------------------------------
unsigned int ofxGLEditor::getCurrentPos(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get current pos from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentPos();
}

//--------------------------------------------------------------
void ofxGLEditor::evalReplReturn(const string &text) {
	if(m_editors[0]) {
		ofxRepl *repl = (ofxRepl*) m_editors[0];
		repl->printEvalReturn(text);
	}
}

//--------------------------------------------------------------
void ofxGLEditor::clearRepl() {
	if(m_editors[0]) {
		ofxRepl *repl = (ofxRepl*) m_editors[0];
		repl->clearText();
		ofLogVerbose("ofxGLEditor") << "cleared repl";
	}
}

//--------------------------------------------------------------
void ofxGLEditor::clearReplHistory() {
	if(m_editors[0]) {
		ofxRepl *repl = (ofxRepl*) m_editors[0];
		repl->clearHistory();
		ofLogVerbose("ofxGLEditor") << "cleared repl history";
	}
}

//--------------------------------------------------------------
void ofxGLEditor::setPath(std::string path) {
	// make sure there is a trailing /
	m_fileDialog->setPath(ofFilePath::addTrailingSlash(path));
	if(m_fileDialog->isActive()) {
		m_fileDialog->refresh();
	}
}

//--------------------------------------------------------------
void ofxGLEditor::setHidden(bool hidden) {
	bHideEditor = !bHideEditor;
}

//--------------------------------------------------------------
void ofxGLEditor::drawString(const std::string& s, float x, float y) {
	m_editors[m_currentEditor]->drawString(s, x, y);
}

//--------------------------------------------------------------
void ofxGLEditor::drawString(const std::string& s, ofPoint& p) {
	drawString(s, p.x, p.y);
}

// SETTINGS

//--------------------------------------------------------------
ofxEditorSettings& ofxGLEditor::getSettings() {
	return m_settings;
}

//--------------------------------------------------------------
void ofxGLEditor::setLineWrapping(bool wrap) {
	for(int i = 0; i < s_numEditors; ++i) { // include repl
		m_editors[i]->setLineWrapping(wrap);
	}
}

//--------------------------------------------------------------
bool ofxGLEditor::getLineWrapping() {
	return m_editors[1]->getLineWrapping();
}

//--------------------------------------------------------------
void ofxGLEditor::setLineNumbers(bool numbers) {
	for(int i = 1; i < s_numEditors; ++i) { // no repl
		m_editors[i]->setLineNumbers(numbers);
	}
}

//--------------------------------------------------------------
bool ofxGLEditor::getLineNumbers() {
	return m_editors[1]->getLineNumbers();
}

//--------------------------------------------------------------
void ofxGLEditor::setAutoFocus(bool focus) {
	for(int i = 0; i < s_numEditors; ++i) { // include repl
		m_editors[i]->setAutoFocus(focus);
	}
}

//--------------------------------------------------------------
bool ofxGLEditor::getAutoFocus() {
	return m_editors[1]->getAutoFocus();
}

//--------------------------------------------------------------
void ofxGLEditor::setFlashEvalSelection(bool flash) {
	bFlashEvalSelection = flash;
}
	
//--------------------------------------------------------------
bool ofxGLEditor::getFlashEvalSelection() {
	return bFlashEvalSelection;
}

// COLOR SCHEME

//--------------------------------------------------------------
void ofxGLEditor::setColorScheme(ofxEditorColorScheme *colorScheme) {
	for(int i = 1; i < s_numEditors; ++i) { // no repl
		m_editors[i]->setColorScheme(colorScheme);
	}
}

//--------------------------------------------------------------
void ofxGLEditor::clearColorScheme() {
	for(int i = 1; i < s_numEditors; ++i) { // no repl
		m_editors[i]->clearColorScheme();
	}
}

//--------------------------------------------------------------
ofxEditorColorScheme* ofxGLEditor::getColorScheme() {
	return m_editors[1]->getColorScheme();
}

// LANG SYNTAX

//--------------------------------------------------------------
void ofxGLEditor::setLangSyntax(const std::string& lang, int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot set syntax from unknown editor " << editor;
		return;
	}
	return m_editors[editor]->setLangSyntax(lang);
}

//--------------------------------------------------------------
void ofxGLEditor::clearSyntax(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot clear syntax from unknown editor " << editor;
		return;
	}
	return m_editors[editor]->clearSyntax();
}

//--------------------------------------------------------------
ofxEditorSyntax* ofxGLEditor::getSyntax(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxGLEditor") << "cannot get syntax from unknown editor " << editor;
		return NULL;
	}
	return m_editors[editor]->getSyntax();
}

// PRIVATE

//--------------------------------------------------------------
int ofxGLEditor::getEditorIndex(int editor) {
	if(editor < 0 || (editor - 1) >= (int) m_editors.size()) {
		return -1;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	return editor;
}
