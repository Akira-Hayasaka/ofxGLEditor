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
 * See https://github.com/danomatika/ofxEditor for more info.
 *
 * A rewrite of the Fluxus FileDialog http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#include "ofxFileDialog.h"

#include "ofxEditorFont.h"
#include "Unicode.h"

//const unsigned int ofxFileDialog::s_fileDisplayRange = 10;
u32string ofxFileDialog::s_saveAsText = U"Save as (esc to exit)";
u32string ofxFileDialog::s_newFolderText = U"New Folder (esc to exit)";
u32string ofxFileDialog::s_newFolderButtonText = U"New Folder";

//--------------------------------------------------------------
ofxFileDialog::ofxFileDialog() : ofxEditor() {
	m_currentFile = 0;
	m_path = string_to_wstring(ofFilePath::getUserHomeDir());
	m_mode = SAVEAS;
	m_active = false;
	m_saveAsState = FILENAME;
	refresh();
}

//--------------------------------------------------------------
ofxFileDialog::ofxFileDialog(ofxEditorSettings &sharedSettings) : ofxEditor(sharedSettings) {
	m_currentFile = 0;
	m_path = string_to_wstring(ofFilePath::getUserHomeDir());
	m_mode = SAVEAS;
	m_active = false
	;
	m_saveAsState = FILENAME;
	refresh();
}

//--------------------------------------------------------------
ofxFileDialog::~ofxFileDialog() {
}

//--------------------------------------------------------------
void ofxFileDialog::draw() {
	if(!m_active) {return;}
	
	// default size if not set
	if(m_width == 0 || m_height == 0) {
		resize(ofGetWidth(), ofGetHeight());
	}

	ofPushStyle();
	ofPushView();
		ofEnableAlphaBlending(); // for fontstash
		ofViewport(0, 0, m_width, m_height);
		ofFill();
	
		// font color
		s_font->setColor(m_settings->getTextColor(), m_settings->getAlpha());
		s_font->setShadowColor(m_settings->getTextShadowColor(), m_settings->getAlpha());
	
		// draw current path
		int pathWidth = s_font->stringWidth(m_path);
		if(pathWidth > m_visibleWidth) { // make sure right side is visible
			s_font->drawString(m_path, m_visibleWidth-pathWidth, s_charHeight);
		}
		else {
			s_font->drawString(m_path, 0, s_charHeight);
		}
	
		// indent and draw dialogs
		ofTranslate(s_charWidth*4, 0);
		switch(m_mode) {
			case SAVEAS:
				drawSaveAs();
				break;
			case OPEN:
				drawOpen();
				break;
			case NEWFOLDER:
				drawNewFolder();
				break;
		}
	
	ofPopView();
	ofPopStyle();

	// update animation timestamps
	updateTimestamps();
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressed(int key) {
	if(!m_active) {return;}
	
	// filter out modifier key events, except SHIFT
	switch(key) {
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
		case OF_KEY_LEFT_SHIFT: case OF_KEY_RIGHT_SHIFT:
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
		case OF_KEY_SUPER: case OF_KEY_LEFT_SUPER: case OF_KEY_RIGHT_SUPER:
			return;
	}
	
	switch(m_mode) {
		case SAVEAS:
			keyPressedSaveAs(key);
			break;
		case OPEN:
			keyPressedOpen(key);
			break;
		case NEWFOLDER:
			keyPressedNewFolder(key);
			break;
	}
}

//--------------------------------------------------------------
void ofxFileDialog::setPath(std::string path) {

	if(!ofDirectory::doesDirectoryExist(path)) {
		ofLogWarning("ofxFileDialog") << "can't set path, " << path << " doesn't exist";
		return;
	}
	
	if(!ofFilePath::isAbsolute(path)) {
		m_path = string_to_wstring(ofFilePath::getAbsolutePath(path));
	}
	else {
		m_path = string_to_wstring(path);
	}
	refresh();
}

//--------------------------------------------------------------
string ofxFileDialog::getSelectedPath() {
	return wstring_to_string(m_selectedPath);
}

//--------------------------------------------------------------
void ofxFileDialog::clearSelectedPath() {
	m_selectedPath = U"";
}

//--------------------------------------------------------------
void ofxFileDialog::setMode(ofxFileDialog::Mode mode) {
	m_mode = mode;
	m_active = true;
	if(mode == SAVEAS) {
		m_saveAsState = FILENAME;
	}
}

//--------------------------------------------------------------
ofxFileDialog::Mode ofxFileDialog::getMode() {
	return m_mode;
}

//--------------------------------------------------------------
bool ofxFileDialog::isActive() {
	return m_active;
}

//--------------------------------------------------------------
void ofxFileDialog::setActive(bool active) {
	m_active = active;
}

//--------------------------------------------------------------
void ofxFileDialog::refresh() {
	
	ofLogVerbose("ofxFileDialog") << "loading path: " << wstring_to_string(m_path);
	
	m_filenames.clear();
	m_directories.clear();
	
	ofDirectory dir(wstring_to_string(m_path));
	dir.listDir();
	vector<ofFile> files = dir.getFiles();
	ofLogVerbose("ofxFileDialog") << files.size() << " files: ";
	
	// one level up
	if(m_path != U"/") {
		m_directories.insert(0);
		m_filenames.push_back(U"..");
	}
	
	// first dirs
	for(int i = 0; i < files.size(); ++i) {
		if(files[i].isDirectory()) {
			m_directories.insert(m_filenames.size());
			m_filenames.push_back(string_to_wstring(files[i].getFileName()));
			ofLogVerbose("ofxFileDialog") << "\t" << wstring_to_string(m_filenames.back());
			// select previous dir
			if(m_prevBasename != U"" && m_filenames.back() == m_prevBasename) {
				m_currentFile = m_filenames.size()-1;
			}
		}
	}
	
	// then files
	for(int i = 0; i < files.size(); ++i) {
		if(!files[i].isDirectory()) {
			m_filenames.push_back(string_to_wstring(files[i].getFileName()));
			ofLogVerbose("ofxFileDialog") << "\t" << wstring_to_string(m_filenames.back());
		}
	}
	
	m_prevBasename = U"";
}

//--------------------------------------------------------------
bool ofxFileDialog::openFile(std::string filename) {
	ofLogWarning("ofxFileDialog") << "ignoring openFile";
	return false;
}

//--------------------------------------------------------------
bool ofxFileDialog::saveFile(std::string filename) {
	ofLogWarning("ofxFileDialog") << "ignoring saveFile";
	return false;
}

//--------------------------------------------------------------
void ofxFileDialog::undo() {
	ofLogWarning("ofxFileDialog") << "ignoring undo";
}

//--------------------------------------------------------------
void ofxFileDialog::redo() {
	ofLogWarning("ofxFileDialog") << "ignoring redo";
}

// STATIC UTILS

//--------------------------------------------------------------
void ofxFileDialog::setSaveAsText(const std::u32string &text) {
	s_saveAsText = text;
}

//--------------------------------------------------------------
void ofxFileDialog::setSaveAsText(const std::string &text) {
	s_saveAsText = string_to_wstring(text);
}

//--------------------------------------------------------------
std::u32string& ofxFileDialog::getWideSaveAsText() {
	return s_saveAsText;
}

//--------------------------------------------------------------
std::string ofxFileDialog::getSaveAsText() {
	return wstring_to_string(s_saveAsText);
}

//--------------------------------------------------------------
void ofxFileDialog::setNewFolderText(const std::u32string &text) {
	s_newFolderText = text;
}

//--------------------------------------------------------------
void ofxFileDialog::setNewFolderText(const std::string &text) {
	s_newFolderText = string_to_wstring(text);
}

//--------------------------------------------------------------
std::u32string& ofxFileDialog::getWideNewFolderText() {
	return s_newFolderText;
}

//--------------------------------------------------------------
std::string ofxFileDialog::getNewFolderText() {
	return wstring_to_string(s_newFolderText);
}

//--------------------------------------------------------------
void ofxFileDialog::setNewFolderButton(const std::u32string &text) {
	s_newFolderButtonText = text;
}

//--------------------------------------------------------------
void ofxFileDialog::setNewFolderButton(const std::string &text) {
	s_newFolderButtonText = string_to_wstring(text);
}

//--------------------------------------------------------------
std::u32string& ofxFileDialog::getWideNewFolderButton() {
	return s_newFolderButtonText;
}

//--------------------------------------------------------------
std::string ofxFileDialog::getNewFolderButton() {
	return wstring_to_string(s_newFolderButtonText);;
}

// PROTECTED

//--------------------------------------------------------------
void ofxFileDialog::drawSaveAs() {

	bool drawnCursor = false;
	int x = 0, y = s_charHeight;
	s_font->setColor(m_settings->getTextColor(), m_settings->getAlpha());
	s_font->setShadowColor(m_settings->getTextShadowColor(), m_settings->getAlpha());

	if(m_saveAsState == FOLDER_DIALOG) {
		drawNewFolder();
	}
	else {
		ofPushMatrix();
		ofTranslate(0, s_charHeight*2);

		// info text
		s_font->drawString(s_saveAsText, x, y, s_textShadow);

		// new file name with cursor
		y += s_charHeight*2;
		for(unsigned int i = 0; i < m_text.size(); ++i) {
		
			// draw cursor
			if(m_saveAsState == FILENAME && i == m_position) {
				drawCursor(x, y);
				drawnCursor = true;
			}
			
			// text
			x = s_font->drawCharacter(m_text[i], x, y, s_textShadow);
		}

		// draw cursor if we have no text, or if we're at the end of the buffer
		if(m_saveAsState == FILENAME && !drawnCursor) {
			drawCursor(x, y);
		}
		
		drawFilenames(5, 2, m_saveAsState == BROWSER);
		ofPopMatrix();
		
		y = m_height-s_charHeight;
		if(m_saveAsState == FOLDER) {
			int width = s_font->stringWidth(s_newFolderButtonText);
			ofSetColor(m_settings->getCursorColor().r, m_settings->getCursorColor().g,
							   m_settings->getCursorColor().b, m_settings->getCursorColor().a * m_settings->getAlpha());
			ofRectMode rectMode = ofGetRectMode();
			ofSetRectMode(OF_RECTMODE_CORNER);
			ofDrawRectangle(x, y-s_charWidth, width, s_charHeight);
			ofSetRectMode(rectMode);
		}
		s_font->drawString(s_newFolderButtonText, 0, y, s_textShadow);
	}
}

//--------------------------------------------------------------
void ofxFileDialog::drawOpen() {
	drawFilenames();
}

//--------------------------------------------------------------
void ofxFileDialog::drawNewFolder() {
	
	bool drawnCursor = false;
	int x = 0, y = s_charHeight;
	s_font->setColor(m_settings->getTextColor(), m_settings->getAlpha());
	s_font->setShadowColor(m_settings->getTextShadowColor(), m_settings->getAlpha());
	
	ofTranslate(0, m_visibleLines*0.5*s_charHeight);

	// info text
	s_font->drawString(s_newFolderText, x, y, s_textShadow);
	
	// new folder name with cursor
	y += s_charHeight*2;
	for(unsigned int i = 0; i < m_text.size(); ++i) {
	
		// draw cursor
		if(m_saveAsState == FILENAME && i == m_position) {
			drawCursor(x, y);
			drawnCursor = true;
		}
		
		// text
		x = s_font->drawCharacter(m_text[i], x, y, s_textShadow);
	}

	// draw cursor if we have no text, or if we're at the end of the buffer
	if(!drawnCursor) {
		drawCursor(x, y);
	}
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressedSaveAs(int key) {
	
	switch(m_saveAsState) {
		case FILENAME:
			switch(key) {
				case OF_KEY_DOWN:
					m_saveAsState = BROWSER;
					m_currentFile = 0;
					return;
					
				case OF_KEY_UP:
					m_saveAsState = FOLDER;
					return;

				case OF_KEY_RETURN:
					m_selectedPath = m_path + m_text;
					break;
					
				case OF_KEY_ESC:
					m_active = false;
					break;
					
				default:
					keyPressedText(key);
					break;
			}
			break;
			
		case BROWSER:
			keyPressedOpen(key, true);
			break;
			
		case FOLDER:
			switch(key) {
				case OF_KEY_UP:
					m_saveAsState = BROWSER;
					m_currentFile = m_filenames.size()-1;
					return;
				
				case OF_KEY_DOWN:
					m_saveAsState = FILENAME;
					return;
					
				case OF_KEY_RETURN:
					m_saveAsState = FOLDER_DIALOG;
					return;
					
				case OF_KEY_ESC:
					m_active = false;
					break;
			}
			break;
			
		case FOLDER_DIALOG:
			keyPressedNewFolder(key, true);
			break;
	}
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressedOpen(int key, bool saveAs) {
	
	switch(key) {
	
		case OF_KEY_END:
			m_currentFile = m_filenames.size();
			break;
		
		case OF_KEY_HOME:
			m_currentFile = 0;
			break;
		
		case OF_KEY_UP:
			if(m_currentFile == 0) {
				if(saveAs) {
					m_saveAsState = FILENAME;
				}
				else {
					m_currentFile = m_filenames.size();
				}
			}
			else {
				m_currentFile--;
			}
			break;
		
		case OF_KEY_DOWN:
			m_currentFile++;
			if(m_currentFile >= m_filenames.size()) {
				if(saveAs) {
					m_currentFile = m_filenames.size()-1;
					m_saveAsState = FOLDER;
				}
				else {
					m_currentFile = 0;
				}
			}
			break;
		
		case OF_KEY_PAGE_UP:
			m_currentFile -= 10;
			break;
			
		case OF_KEY_PAGE_DOWN:
			m_currentFile += 10;
			break;

		case OF_KEY_RETURN:
			if(m_directories.find(m_currentFile) != m_directories.end()) {
				if(m_filenames[m_currentFile] == U"..") { // go back?
					m_prevBasename = string_to_wstring(ofSplitString(ofFilePath::removeTrailingSlash(wstring_to_string(m_path)), "/").back());
				}
				m_path += m_filenames[m_currentFile];
				m_path += U"/";
				m_path = string_to_wstring(ofFilePath::getAbsolutePath(wstring_to_string(m_path)));
				m_currentFile = 0;
				refresh();
			}
			else if(!saveAs) { // file
				m_selectedPath = m_path + m_filenames[m_currentFile];
				m_active = false;
			}
			break;
			
		case OF_KEY_ESC:
			m_active = false;
			break;
	}	
	
	if(m_currentFile >= m_filenames.size()) {
		m_currentFile = m_filenames.size()-1;
	}
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressedNewFolder(int key, bool saveAs) {
	
	switch(key) {

		case OF_KEY_RETURN:
			if(!ofDirectory::createDirectory(wstring_to_string(m_path+m_text))) {
				ofLogError("ofxFileDialog") << "couldn't create new folder: \"" << wstring_to_string(m_text) << "\"";
				m_saveAsState = FILENAME;
			}
			else {
				ofLogVerbose("ofxFileDialog") << "created new folder: \"" << wstring_to_string(m_text) << "\"";
				refresh();
				for(int i = 0; i < m_filenames.size(); ++i) {
					if(m_filenames[i] == m_text) {
						m_currentFile = i;
						break;
					}
				}
				if(SAVEAS) {
					m_saveAsState = BROWSER;
				}
			}
			clearText();
			break;
			
		case OF_KEY_ESC:
			if(saveAs) {
				m_saveAsState = FILENAME;
			}
			else {
				m_active = false;
			}
			break;
			
		default:
			keyPressedText(key);
			break;
	}
}

//--------------------------------------------------------------
void ofxFileDialog::drawFilenames(int offset, int bottomOffset, bool highlight) {
	
	int x = 0;
	float center = (m_height-s_charHeight*offset)*0.5;
	int top = -center+(s_charHeight*2);
	int bottom = center-(s_charHeight*bottomOffset);
	int displayRange = m_visibleLines-offset;
	
	// center vertically
	ofPushMatrix();
	ofTranslate(0, m_visibleLines*0.5*s_charHeight);
	
	// start drawing based on current file location in file list so selection remains centered
	float y = (m_currentFile/(float)m_filenames.size()) * -s_charHeight * (float)m_filenames.size() + s_charHeight;
	unsigned int count = 0;
	for(vector<u32string>::iterator i = m_filenames.begin(); i != m_filenames.end(); i++) {
	
		m_numLines = 0;
		bool isDir = m_directories.find(count) != m_directories.end();
		
		// don't draw on top of path
		if(y < top) {
			y += s_charHeight;
			count++;
			continue;
		}
		
		// only show numbers in the display range
		if(((int)count > ((int)m_currentFile - (int)displayRange)) &&
		   ((int)count < ((int)m_currentFile + (int)displayRange))) {
			
			for(int c = 0; c < i->size(); ++c) {
			
				// current file background
				if(highlight && count == m_currentFile) {
					ofSetColor(m_settings->getCursorColor().r, m_settings->getCursorColor().g,
					       m_settings->getCursorColor().b, m_settings->getCursorColor().a * m_settings->getAlpha());
					ofRectMode rectMode = ofGetRectMode();
					ofSetRectMode(OF_RECTMODE_CORNER);
					ofDrawRectangle(x, y-s_charHeight, characterWidth((*i)[c]), s_charHeight);
					ofSetRectMode(rectMode);
				}
				
				// file or dir name
				x = s_font->drawCharacter((*i)[c], x, y, s_textShadow);
			}
			x = 0;
		}
		y += s_charHeight;
		count++;
		
		// don't draw beyond bottom
		if(y > bottom) {
			break;
		}
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressedText(int key) {
	
	switch(key) {
	
		case OF_KEY_UP: case OF_KEY_DOWN:
			return;
			
		case OF_KEY_RIGHT:
			if(!m_text.empty()) {
				m_position++;
			}
			if(m_position > m_text.size()) {
				m_position = m_text.size();
			}
			break;
			
		case OF_KEY_LEFT:
			if(m_position > 0) {
				m_position--;
			}
			break;

		case OF_KEY_DEL:
			m_text.erase(m_position, 1);
			break;
			
		case OF_KEY_BACKSPACE:
			if(!m_text.empty() && m_position != 0) {
				if(m_selection != NONE) {
					m_text.erase(m_highlightStart, m_highlightEnd - m_highlightStart);
					m_position -= m_highlightEnd - m_highlightStart;
					m_selection = NONE;
				}
				else {
					m_text.erase(m_position-1, 1);
					m_position--;
				}
			}
			break;
	
		default:
	
			// build multibyte UTF-8 character
			if(key > 0x80) {
				if(m_UTF8Bytes == 0) {
					m_UTF8Bytes = wchar_width(key);
				}
				m_UTF8Char.push_back(key);
				if(m_UTF8Char.length() < m_UTF8Bytes) {
					return;
				}
			}
			else if(m_UTF8Bytes > 0) {
				ofLogWarning("ofxFileDialog") << "dropping bad UTF8 bytes";
				m_UTF8Bytes = 0;
				m_UTF8Char = "";
			}

			// ignore control chars
			if(key != '\n' && key < ' ') {
				return;
			}
			
			// multibyte UTF8
			if(m_UTF8Bytes > 0) {
				m_UTF8Bytes = 0;
			}
			else { // single byte UTF8 & ASCII
				m_UTF8Char.push_back(key);
			}
			m_text.insert(m_position, string_to_wstring(m_UTF8Char));
			m_UTF8Char = "";
			m_position++;
			break;
	}
}
