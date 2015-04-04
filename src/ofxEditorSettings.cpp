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
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 */
#include "ofxEditorSettings.h"

#include "ofLog.h"
#include "Unicode.h"

//--------------------------------------------------------------
ofxEditorSettings::ofxEditorSettings() {

	tabWidth = 4;
	convertTabs = false;

	alpha = 1.0f;
	textColor = ofColor(255);
	textShadowColor = ofColor(0);
	cursorColor = ofColor(255, 255, 0, 200);
	selectionColor = ofColor(0, 255, 0, 127);
	flashColor = ofColor(0, 255, 0, 127);
	matchingCharsColor = ofColor(0, 127, 255, 127);
	lineNumberColor = ofColor(127);

	highlightMatchingChars = true;
	openChars = L"([<{+*/-"; // Added in + * / - for syntax highlighting
	closeChars = L")]>}";
}

//--------------------------------------------------------------
ofxEditorSettings::ofxEditorSettings(const ofxEditorSettings &from) {
	copy(from);
}

//--------------------------------------------------------------
ofxEditorSettings& ofxEditorSettings::operator=(const ofxEditorSettings &from) {
	copy(from);
	return *this;
}

//--------------------------------------------------------------
void ofxEditorSettings::copy(const ofxEditorSettings &from) {
	
	tabWidth = from.tabWidth;
	convertTabs = from.convertTabs;
	
	alpha = from.alpha;
	textColor = from.textColor;
	textShadowColor = from.textShadowColor;
	cursorColor = from.cursorColor;
	selectionColor = from.selectionColor;
	matchingCharsColor = from.matchingCharsColor;
	lineNumberColor = from.lineNumberColor;
	
	highlightMatchingChars = from.highlightMatchingChars;
	openChars = from.openChars;
	closeChars = from.closeChars;
	
	// make deep copies
	clearAllSyntaxes();
	clearAllFileExts();
	for(map<string,ofxEditorSyntax*>::const_iterator iter = from.langs.begin();
	    iter == from.langs.end(); ++iter) {
		langs[(*iter).first] = (*iter).second;
	}
	for(map<string,string>::const_iterator iter = from.fileExts.begin();
	    iter == from.fileExts.end(); ++iter) {
		fileExts[(*iter).first] = (*iter).second;
	}
}

//--------------------------------------------------------------
void ofxEditorSettings::setTabWidth(unsigned int numSpaces) {
	tabWidth = numSpaces < 1 ? 1 : numSpaces;
}

//--------------------------------------------------------------
unsigned int ofxEditorSettings::getTabWidth() {
	return tabWidth;
}

//--------------------------------------------------------------
void ofxEditorSettings::setConvertTabs(bool convert) {
	convertTabs = convert;
}

//--------------------------------------------------------------
bool ofxEditorSettings::getConvertTabs() {
	return convertTabs;
}

//--------------------------------------------------------------
void ofxEditorSettings::setAlpha(float alpha) {
	this->alpha = ofClamp(alpha, 0.1f, 1.0f);
}

//--------------------------------------------------------------
float ofxEditorSettings::getAlpha() {
	return alpha;
}

//--------------------------------------------------------------
void ofxEditorSettings::setTextColor(ofColor color) {
	textColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getTextColor() {
	return textColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setTextShadowColor(ofColor color) {
	textShadowColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getTextShadowColor() {
	return textShadowColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setCursorColor(ofColor color) {
	cursorColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getCursorColor() {
	return cursorColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setSelectionColor(ofColor color) {
	selectionColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getSelectionColor() {
	return selectionColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setFlashColor(ofColor color) {
	flashColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getFlashColor() {
	return flashColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setMatchingCharsColor(ofColor color) {
	matchingCharsColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getMatchingCharsColor() {
	return matchingCharsColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setLineNumberColor(ofColor color) {
	lineNumberColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorSettings::getLineNumberColor() {
	return lineNumberColor;
}

//--------------------------------------------------------------
void ofxEditorSettings::setHighlightMatchingChars(bool highlight) {
	highlightMatchingChars = highlight;
}

//--------------------------------------------------------------
bool ofxEditorSettings::getHighlightMatchingChars() {
	return highlightMatchingChars;
}

//--------------------------------------------------------------
void ofxEditorSettings::setMatchingChars(wstring openChars, wstring closeChars) {
	if(openChars.length() == 0 || closeChars.length() == 0) {
		ofLogWarning("ofxEditorSettings") << "empty open or close char string";
		return;
	}
	openChars = openChars;
	closeChars = closeChars;
}

//--------------------------------------------------------------
void ofxEditorSettings::setMatchingChars(string openChars, string closeChars) {
	setMatchingChars(string_to_wstring(openChars), string_to_wstring(closeChars));
}

//--------------------------------------------------------------
wstring& ofxEditorSettings::getWideOpenChars() {
	return openChars;
}

//--------------------------------------------------------------
string ofxEditorSettings::getOpenChars() {
	return wstring_to_string(openChars);
}

//--------------------------------------------------------------
wstring& ofxEditorSettings::getWideCloseChars() {
	return closeChars;
}

//--------------------------------------------------------------
string ofxEditorSettings::getCloseChars() {
	return wstring_to_string(closeChars);
}

//--------------------------------------------------------------
void ofxEditorSettings::setLangSyntax(const string &lang, ofxEditorSyntax *syntax) {
	if(!syntax) return;
	langs[lang] = syntax;
}

//--------------------------------------------------------------
ofxEditorSyntax* ofxEditorSettings::getLangSyntax(const string &lang) {
	map<string,ofxEditorSyntax*>::iterator iter = langs.find(lang);
	if(iter != langs.end()) {
		return (*iter).second;
	}
	return NULL;
}

//--------------------------------------------------------------
void ofxEditorSettings::clearLangSyntax(const string &lang) {
	map<string,ofxEditorSyntax*>::iterator iter = langs.find(lang);
	if(iter != langs.end()) {
		langs.erase(iter);
	}
}

//--------------------------------------------------------------
void ofxEditorSettings::clearAllSyntaxes() {
	langs.clear();
}

//--------------------------------------------------------------
void ofxEditorSettings::setFileExtLang(const string &ext, const string &lang) {
	fileExts[ext] = lang;
}

//--------------------------------------------------------------
string ofxEditorSettings::getFileExtLang(const string &ext) {
	map<string,string>::iterator iter = fileExts.find(ext);
	if(iter != fileExts.end()) {
		return (*iter).second;
	}
	return "";
}

//--------------------------------------------------------------
void ofxEditorSettings::clearFileExtLang(const string &lang) {
	map<string,string>::iterator iter = fileExts.begin();
	while(iter != fileExts.end()) {
		if((*iter).second == lang) {
			fileExts.erase(iter++);
		}
		else {
			iter++;
		}
	}
}

//--------------------------------------------------------------
void ofxEditorSettings::clearAllFileExts() {
	fileExts.clear();
}

//--------------------------------------------------------------
ofxEditorSyntax* ofxEditorSettings::getFileExtSyntax(const string &ext) {
	return getLangSyntax(getFileExtLang(ext));
}
