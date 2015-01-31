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
 * A rewrite of the Fluxus FileDialog http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#include "ofxFileDialog.h"

#include "Unicode.h"

const unsigned int ofxFileDialog::s_fileDisplayRange = 10;
wstring ofxFileDialog::s_saveAsText = L"Save as (esc to exit)";

//--------------------------------------------------------------
ofxFileDialog::ofxFileDialog() : ofxEditor() {
	m_currentFile = 0;
	m_path = string_to_wstring(ofFilePath::getUserHomeDir());
	m_saveAs = true;
	refresh();
}

//--------------------------------------------------------------
ofxFileDialog::ofxFileDialog(ofxEditorSettings &sharedSettings) : ofxEditor(sharedSettings) {
	m_currentFile = 0;
	m_path = string_to_wstring(ofFilePath::getUserHomeDir());
	m_saveAs = true;
	refresh();
}

//--------------------------------------------------------------
ofxFileDialog::~ofxFileDialog() {
}

//--------------------------------------------------------------
void ofxFileDialog::draw() {
	
	// default size if not set
	if(m_width == 0 || m_height == 0) {
		resize(ofGetWidth(), ofGetHeight());
	}

	ofPushStyle();
	ofPushView();
	
		ofViewport(0, 0, m_width, m_height);
		ofTranslate(s_charWidth*4, m_height/2);
	
		ofFill();
		if(m_saveAs) {
			drawSaveAs();
		}
		else {
			drawOpen();
		}
	
	ofPopView();
	ofPopStyle();

	// update animation timestamps
	updateTimestamps();
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressed(int key) {
	
	// filter out modifier key events, except SHIFT
	switch(key) {
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
		case OF_KEY_LEFT_SHIFT: case OF_KEY_RIGHT_SHIFT:
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
		case OF_KEY_SUPER: case OF_KEY_LEFT_COMMAND: case OF_KEY_RIGHT_COMMAND:
			return;
	}
	
	unsigned int startpos = m_position;
	if(m_saveAs) {
		keyPressedSaveAs(key);
	}
	else {
		keyPressedOpen(key);
	}
}

//--------------------------------------------------------------
void ofxFileDialog::setPath(string path) {

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
	m_selectedPath = L"";
}

//--------------------------------------------------------------
void ofxFileDialog::setSaveAsMode(bool s) {
	m_saveAs = s;
}

//--------------------------------------------------------------
bool ofxFileDialog::getSaveAsMode() {
	return m_saveAs;
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
	m_directories.insert(0);
	m_filenames.push_back(L"..");
	
	// everything else
	for(int i = 0; i < files.size(); ++i) {
		if(files[i].isDirectory()) {
			m_directories.insert(m_filenames.size());
		}
		m_filenames.push_back(string_to_wstring(files[i].getFileName()));
		ofLogVerbose("ofxFileDialog") << "\t" << wstring_to_string(m_filenames.back());
	}
}

// STATIC UTILS

//--------------------------------------------------------------
void ofxFileDialog::setSaveAsText(const wstring &text) {
	s_saveAsText = text;
}

//--------------------------------------------------------------
void ofxFileDialog::setSaveAsText(const string &text) {
	s_saveAsText = string_to_wstring(text);
}

//--------------------------------------------------------------
wstring& ofxFileDialog::getWideSaveAsText() {
	return s_saveAsText;
}

//--------------------------------------------------------------
string ofxFileDialog::getSaveAsText() {
	return wstring_to_string(s_saveAsText);
}

// PROTECTED

//--------------------------------------------------------------
void ofxFileDialog::drawSaveAs() {

	bool drawnCursor = false;
	int x = 0, y = -s_charHeight;

	// info text
	ofSetColor(m_settings->getTextColor().r, m_settings->getTextColor().g,
			   m_settings->getTextColor().b, m_settings->getTextColor().a * m_settings->getAlpha());
	drawString(s_saveAsText, x, y);

	// new file name with cursor
	y += s_charHeight*2;
	for(unsigned int i = 0; i < m_text.size(); ++i) {
	
		// draw cursor
		if(i == m_position) {
			drawCursor(x, y);
			drawnCursor = true;
		}
		
		// text
		ofSetColor(m_settings->getTextColor().r, m_settings->getTextColor().g,
				   m_settings->getTextColor().b, m_settings->getTextColor().a * m_settings->getAlpha());
		s_font->drawCharacter(m_text[i], x, y);
		x += s_charWidth;
	}

	// draw cursor if we have no text, or if we're at the end of the buffer
	if(!drawnCursor){
		drawCursor(x, y);
	}
}

//--------------------------------------------------------------
void ofxFileDialog::drawOpen() {

	int x = 0;
	ofColor dirColor = m_settings->getTextColor(); // TODO: separate dir color?
	
	// start drawing based on current file location in file list so selection is centered vertically
	float y = (m_currentFile/(float)m_filenames.size()) * -s_charHeight * (float)m_filenames.size();

	unsigned int count = 0;
	for(vector<wstring>::iterator i = m_filenames.begin(); i != m_filenames.end(); i++) {
	
		m_numLines = 0;
		bool isDir = m_directories.find(count) != m_directories.end();
	
		// only show numbers in the display range
		if(((int)count > ((int)m_currentFile - (int)s_fileDisplayRange)) &&
		   ((int)count < ((int)m_currentFile + (int)s_fileDisplayRange))) {
			
			for(int c = 0; c < i->size(); ++c) {
			
				// current file background
				if(count == m_currentFile) {
					ofSetColor(m_settings->getCursorColor().r, m_settings->getCursorColor().g,
					       m_settings->getCursorColor().b, m_settings->getCursorColor().a * m_settings->getAlpha());
					ofRect(x, y-s_charHeight, s_charWidth, s_charHeight);
				}
				
				// file or dir name
				if(isDir) {
					ofSetColor(dirColor.r, dirColor.g, dirColor.b, dirColor.a * m_settings->getAlpha());
				}
				else {
					ofSetColor(m_settings->getTextColor().r, m_settings->getTextColor().g,
					           m_settings->getTextColor().b, m_settings->getTextColor().a * m_settings->getAlpha());
				}
				s_font->drawCharacter((*i)[c], x, y);
				x += s_charWidth;
			}
			x = 0;
		}
		y += s_charHeight;
		count++;
	}
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressedSaveAs(int key) {
	
	switch(key) {
		
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

		case OF_KEY_RETURN:
			m_selectedPath = m_path + m_text;
			break;
			
		case OF_KEY_DEL:
			m_text.erase(m_position, 1);
			break;
			
		case OF_KEY_BACKSPACE:
			if(!m_text.empty() && m_position != 0) {
				if(m_selection) {
					m_text.erase(m_highlightStart, m_highlightEnd - m_highlightStart);
					m_position -= m_highlightEnd - m_highlightStart;
					m_selection = false;
				}
				else {
					m_text.erase(m_position-1, 1);
					m_position--;
				}
			}
			break;
			
		default:
		
			// build a two byte UTF-8 character
			// TODO: what happens if it's 3 or 4 bytes?
			if(key > 0x80 && m_firstUTF8Byte == 0) {
				m_firstUTF8Byte = key;
				return;
			}
		
			// ignore control chars
			if(key != '\n' && key < ' ') {
				break;
			}
			
			// build wide char and insert
			if(m_firstUTF8Byte > 0) {
				string temp("  ");
				temp[0] = m_firstUTF8Byte;
				temp[1] = key;
				m_text.insert(m_position, string_to_wstring(temp));
				m_firstUTF8Byte = 0;
			}
			else {
				if(key < 0x80) {
					string temp(" ");
					temp[0] = key;
					m_text.insert(m_position, string_to_wstring(temp));
				}
				else {
					wchar_t k[2];
					memset(&k, 0, sizeof(wchar_t)*2);
					k[0] = key;
					m_text.insert(m_position, wstring(k));
				}
			}
			m_position++;
			
			break;
	}	
}

//--------------------------------------------------------------
void ofxFileDialog::keyPressedOpen(int key) {

	switch(key) {
		
		case OF_KEY_END:
			m_currentFile = m_filenames.size();
			break;
		
		case OF_KEY_HOME:
			m_currentFile = 0;
			break;
		
		case OF_KEY_UP:
			if(m_currentFile == 0) {
				m_currentFile = m_filenames.size();
			}
			else {
				m_currentFile--;
			}
			break;
		
		case OF_KEY_DOWN:
			m_currentFile++;
			if(m_currentFile >= m_filenames.size()) {
				m_currentFile = 0;
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
				m_path += m_filenames[m_currentFile];
				m_path += L"/";
				refresh();
			}
			else {
				m_selectedPath = m_path + m_filenames[m_currentFile];
			}
			break;
	}	
	
	if(m_currentFile >= m_filenames.size()) {
		m_currentFile = m_filenames.size()-1;
	}
}
