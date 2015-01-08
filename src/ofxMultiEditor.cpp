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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/danomatika/ofxEditor for more info.
 *
 * Based on a rewrite of ofxGLEditor by Akira Hayasaka & Dan Wilcox.
 */
#include "ofxMultiEditor.h"

//--------------------------------------------------------------
ofxMultiEditor::ofxMultiEditor() {
	m_listener = NULL;
	m_saveFiles.resize(s_numEditors);
	m_currentEditor = 0;
	bModifierPressed = false;
	bHideEditor = false;
	bShowFileDialog = false;
}

//--------------------------------------------------------------
ofxMultiEditor::~ofxMultiEditor() {
	clear();
}

//--------------------------------------------------------------
void ofxMultiEditor::setup(ofxMultiEditorListener *listener, bool enableRepl) {
	
	clear();
	
	// handle ESC internally since we use it to exit the file browser
	ofSetEscapeQuitsApp(false);
	
	m_listener = listener;
	
	// create editors
	if(enableRepl) {
		ofxRepl *repl = new ofxRepl(m_settings);
		repl->setListener(listener);
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
	
	m_currentEditor = 1;
}

//--------------------------------------------------------------
void ofxMultiEditor::clear() {
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
void ofxMultiEditor::draw() {
	ofPushView();
	ofPushMatrix();
	ofPushStyle();
	
		ofEnableAlphaBlending();
		
		if(bShowFileDialog) {
			m_fileDialog->draw();
		}
		else if(!bHideEditor) {
			m_editors[m_currentEditor]->draw();
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
void ofxMultiEditor::keyPressed(int key) {

	// check modifier keys
	bModifierPressed = ofxEditor::getSuperAsModifier() ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	
	// also check for ascii control chars: http://ascii-table.com/control-chars.php
	if(bModifierPressed) {
		switch(key) {
			
			case 'e': case 5:
				if(m_currentEditor != 0) {
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
						m_fileDialog->setSaveAsMode(true);
						bShowFileDialog = !bShowFileDialog;
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
					m_fileDialog->setSaveAsMode(true);
					bShowFileDialog = !bShowFileDialog;
				}
				return;
				
			case 'o': case 15:
				if(m_currentEditor != 0) {
					m_fileDialog->setSaveAsMode(false);
					m_fileDialog->refresh();
					bShowFileDialog = !bShowFileDialog;
				}
				return;
				
			case '-':
				m_settings.alpha -= 0.05;
				if(m_settings.alpha < 0) {
					m_settings.alpha = 0;
				}
				return;
				
			case '=':
				m_settings.alpha += 0.05;
				if(m_settings.alpha > 1) {
					m_settings.alpha = 1;
				}
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
	
	if(bShowFileDialog) {
		if(key == OF_KEY_ESC) {
			bShowFileDialog = false;
		}
		m_fileDialog->keyPressed(key);
		if(m_fileDialog->getSelectedPath() != "") {
			if(m_fileDialog->getSaveAsMode()) {
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
			bShowFileDialog = false;
		}
	}
	else if(!bHideEditor) {
		m_editors[m_currentEditor]->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxMultiEditor::resize() {
	int w = (ofGetWindowMode() == OF_WINDOW) ? ofGetViewportWidth() : ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW) ? ofGetViewportHeight() : ofGetScreenHeight();
	resize(w, h);
}

//--------------------------------------------------------------
void ofxMultiEditor::resize(int width, int height) {
	for(int i = 0; i < (int) m_editors.size(); i++) {
		if(m_editors[i]) m_editors[i]->resize(width, height);
	}
	m_fileDialog->resize(width, height);
}

//--------------------------------------------------------------
bool ofxMultiEditor::openFile(string filename, int editor) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxMultiEditor") << "cannot load into unknown editor " << editor;
		return false;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	string path = ofToDataPath(filename);
	ofLogVerbose("ofxMultiEditor") << "loading \"" << ofFilePath::getFileName(path)
		<< "\" into editor " << editor;
	
	ofFile file;
	if(!file.open(ofToDataPath(path), ofFile::ReadOnly)) {
		ofLogError() << "ofxMultiEditor: couldn't load \""
			<< ofFilePath::getFileName(path) << "\"";
		return false;
	}
	
	setText(file.readToBuffer().getText(), editor);
	file.close();
	
	return true;
}

//--------------------------------------------------------------
bool ofxMultiEditor::saveFile(string filename, int editor) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxMultiEditor") << "cannot save from unknown editor " << editor;
		return "";
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	string path = ofToDataPath(filename);
	ofLogVerbose("ofxMultiEditor") << "saving editor " << editor
		<< " to \"" << ofFilePath::getFileName(path) << "\"";
	
	ofFile file;
	if(!file.open(path, ofFile::WriteOnly)) {
		ofLogError() << "ofxMultiEditor: couldn't open \""
			<< ofFilePath::getFileName(path) << "\" for saving";
		return false;
	}
	
	file << getText(editor);
	file.close();
		
	return true;
}

//--------------------------------------------------------------
string ofxMultiEditor::getText(int editor) {
    
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot get text from unknown editor " << editor;
		return "";
	}
	
	// add an endline if there isn't one already
	string text = m_editors[editor]->getText();
	if(text[text.size()-1] != '\n') 
		text += "\n";
	return text;
}

//--------------------------------------------------------------
void ofxMultiEditor::setText(string text, int editor) {

	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot set text into unknown editor " << editor;
		return;
	}

    ofxEditor *e = m_editors[editor];
	e->setText(text);
}

//--------------------------------------------------------------
void ofxMultiEditor::insertText(string text, int editor) {

	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot insert text into unknown editor " << editor;
		return;
	}

    ofxEditor *e = m_editors[editor];
	e->insertText(text);
}

//--------------------------------------------------------------
void ofxMultiEditor::clearText(int editor) {

	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot clear text in unknown editor " << editor;
		return;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	 // reset filename
	ofLogVerbose("ofxMultiEditor") << "cleared text in editor" << m_currentEditor;
	m_editors[editor]->clearText();
	m_saveFiles[editor] = "";
}

//--------------------------------------------------------------
void ofxMultiEditor::clearAllText() {
	for(int i = 1; i < (int) m_editors.size(); i++) {
		m_editors[i]->clearText();
		m_saveFiles[i] = "";
	}
	ofLogVerbose("ofxMultiEditor") << "cleared text in all editors";
}

//--------------------------------------------------------------
void ofxMultiEditor::setCurrentEditor(int editor) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxMultiEditor") << "cannot set unknown editor " << editor;
		return;
	}
	
	m_currentEditor = editor;
	ofLogVerbose("ofxMultiEditor") << "setting the current editor to " << m_currentEditor;
}

//--------------------------------------------------------------
int ofxMultiEditor::getCurrentEditor() {
	return m_currentEditor;
}

//--------------------------------------------------------------
void ofxMultiEditor::setEditorFilename(int editor, string filename) {
	
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxMultiEditor") << "cannot set filename for unknown editor " << editor;
		return;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	
	ofLogVerbose("ofxMultiEditor") << "setting filename for editor " << editor
		<< " to \"" << ofFilePath::getFileName(filename) << "\"";

	m_saveFiles[editor] = filename;
}
	
//--------------------------------------------------------------
string ofxMultiEditor::getEditorFilename(int editor) {
	if(editor < 0 && (editor - 1) >= (int) m_editors.size()) {
		ofLogError("ofxMultiEditor") << "cannot get filename for unknown editor " << editor;
		return "";
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	return m_saveFiles[editor];
}

//--------------------------------------------------------------
unsigned int ofxMultiEditor::getNumLines(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot get num lines from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getNumLines();
}

//--------------------------------------------------------------
void ofxMultiEditor::setCurrentLine(unsigned int line, int editor) {
	
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot set current line in unknown editor " << editor;
		return;
	}
		
	ofxEditor *e = m_editors[editor];
	if(line >= e->getNumLines()) {
		ofLogError("ofxMultiEditor") << "cannot set current line, given line "
			<< line << " is >= num lines " << e->getNumLines();
		return;
	}
	e->setCurrentLine(line);
}

//--------------------------------------------------------------
unsigned int ofxMultiEditor::getCurrentLine(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot get current line from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentLine();
}

//--------------------------------------------------------------
unsigned int ofxMultiEditor::getCurrentLinePos(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot get current line pos from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentLinePos();
}

//--------------------------------------------------------------
unsigned int ofxMultiEditor::getCurrentLineLen(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot get current line len from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentLineLen();
}

//--------------------------------------------------------------
unsigned int ofxMultiEditor::getCurrentPos(int editor) {
	editor = getEditorIndex(editor);
	if(editor == -1) {
		ofLogError("ofxMultiEditor") << "cannot get current pos from unknown editor " << editor;
		return 0;
	}
	return m_editors[editor]->getCurrentPos();
}

//--------------------------------------------------------------
void ofxMultiEditor::evalReplReturn(const string &text) {
	if(m_editors[0]) {
		ofxRepl *repl = (ofxRepl*) m_editors[0];
		repl->printEvalReturn(text);
	}
}

//--------------------------------------------------------------
void ofxMultiEditor::clearRepl() {
	if(m_editors[0]) {
		ofxRepl *repl = (ofxRepl*) m_editors[0];
		repl->clear();
		ofLogVerbose("ofxMultiEditor") << "cleared repl";
	}
}

//--------------------------------------------------------------
void ofxMultiEditor::clearReplHistory() {
	if(m_editors[0]) {
		ofxRepl *repl = (ofxRepl*) m_editors[0];
		repl->clearHistory();
		ofLogVerbose("ofxMultiEditor") << "cleared repl history";
	}
}

//--------------------------------------------------------------
void ofxMultiEditor::setPath(string path) {
	// make sure there is a trailing /
	m_fileDialog->setPath(ofFilePath::addTrailingSlash(path));
	if(bShowFileDialog) {
		m_fileDialog->refresh();
	}
}

//--------------------------------------------------------------
void ofxMultiEditor::setHidden(bool hidden) {
	bHideEditor = !bHideEditor;
}

//--------------------------------------------------------------
void ofxMultiEditor::drawString(const string& s, float x, float y) {
	m_editors[m_currentEditor]->drawString(s, x, y);
}

//--------------------------------------------------------------
void ofxMultiEditor::drawString(const string& s, ofPoint& p) {
	drawString(s, p.x, p.y);
}

// SETTINGS

//--------------------------------------------------------------
ofxEditorSettings& ofxMultiEditor::getSettings() {
	return m_settings;
}

//--------------------------------------------------------------
void ofxMultiEditor::setLineWrapping(bool wrap) {
	for(int i = 0; i < s_numEditors; ++i) { // include repl
		m_editors[i]->setLineWrapping(wrap);
	}
}

//--------------------------------------------------------------
bool ofxMultiEditor::getLineWrapping() {
	return m_editors[1]->getLineWrapping();
}

//--------------------------------------------------------------
void ofxMultiEditor::setLineNumbers(bool numbers) {
	for(int i = 1; i < s_numEditors; ++i) { // no repl
		m_editors[i]->setLineNumbers(numbers);
	}
}

//--------------------------------------------------------------
bool ofxMultiEditor::getLineNumbers() {
	return m_editors[1]->getLineNumbers();
}

// PRIVATE

//--------------------------------------------------------------
int ofxMultiEditor::getEditorIndex(int editor) {
	if(editor < 0 || (editor - 1) >= (int) m_editors.size()) {
		return -1;
	}
	else if(editor == 0) {
		editor = m_currentEditor;
	}
	return editor;
}
