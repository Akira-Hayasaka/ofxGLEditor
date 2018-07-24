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
#include "ofxEditorSettings.h"

#include "ofLog.h"
#include "ofMath.h"
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
	openChars = U"([<{";
	closeChars = U")]>}";
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
	for(std::map<std::string,ofxEditorSyntax*>::const_iterator iter = from.langs.begin();
	    iter == from.langs.end(); ++iter) {
		langs[(*iter).first] = (*iter).second;
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
void ofxEditorSettings::setMatchingChars(std::u32string openChars, std::u32string closeChars) {
	if(openChars.length() == 0 || closeChars.length() == 0) {
		ofLogWarning("ofxEditorSettings") << "empty open or close char string";
		return;
	}
	openChars = openChars;
	closeChars = closeChars;
}

//--------------------------------------------------------------
void ofxEditorSettings::setMatchingChars(std::string openChars, std::string closeChars) {
	setMatchingChars(string_to_wstring(openChars), string_to_wstring(closeChars));
}

//--------------------------------------------------------------
std::u32string& ofxEditorSettings::getWideOpenChars() {
	return openChars;
}

//--------------------------------------------------------------
std::string ofxEditorSettings::getOpenChars() {
	return wstring_to_string(openChars);
}

//--------------------------------------------------------------
std::u32string& ofxEditorSettings::getWideCloseChars() {
	return closeChars;
}

//--------------------------------------------------------------
std::string ofxEditorSettings::getCloseChars() {
	return wstring_to_string(closeChars);
}

//--------------------------------------------------------------
void ofxEditorSettings::addSyntax(ofxEditorSyntax *syntax) {
	if(!syntax) return;
	if(syntax->getLang() == "") {
		ofLogWarning("ofxEditorSettings") << "cannot add syntax with empty lang string";
		return;
	}
	langs[syntax->getLang()] = syntax;
}

//--------------------------------------------------------------
void ofxEditorSettings::addSyntax(const std::string &lang, ofxEditorSyntax *syntax) {
	if(!syntax) return;
	if(lang == "") {
		ofLogWarning("ofxEditorSettings") << "cannot add syntax with empty lang string";
		return;
	}
	langs[lang] = syntax;
}

//--------------------------------------------------------------
ofxEditorSyntax* ofxEditorSettings::getSyntax(const std::string &lang) {
	std::map<std::string,ofxEditorSyntax*>::iterator iter = langs.find(lang);
	if(iter != langs.end()) {
		return (*iter).second;
	}
	return NULL;
}

//--------------------------------------------------------------
void ofxEditorSettings::clearSyntax(const std::string &lang) {
	std::map<std::string,ofxEditorSyntax*>::iterator iter = langs.find(lang);
	if(iter != langs.end()) {
		langs.erase(iter);
	}
}

//--------------------------------------------------------------
void ofxEditorSettings::clearAllSyntaxes() {
	langs.clear();
}

//--------------------------------------------------------------
ofxEditorSyntax* ofxEditorSettings::getSyntaxForFileExt(const std::string &ext) {
	std::map<std::string,ofxEditorSyntax*>::iterator iter;
	for(iter = langs.begin(); iter != langs.end(); ++iter) {
		if((*iter).second->hasFileExt(ext)) {
			return (*iter).second;
		}
	}
	return NULL;
}

//--------------------------------------------------------------
void ofxEditorSettings::printSyntaxes() {
	ofLogNotice("ofxEditorSettings") << "syntaxes: " << (langs.empty() ? "none" : "");
	std::map<std::string,ofxEditorSyntax*>::iterator iter;
	for(iter = langs.begin(); iter != langs.end(); ++iter) {
		std::string line = "  " + (*iter).second->getLang();
		const std::set<std::string> &fileExts = (*iter).second->getFileExts();
		if(!fileExts.empty()) {
			line += " (";
			int count = 0;
			for(std::set<std::string>::const_iterator i = fileExts.begin(); i != fileExts.end(); ++i) {
				line += (*i);
				count++;
				if(count < fileExts.size()) {
					line += ", ";
				}
			}
			line += ")";
		}
		ofLogNotice("ofxEditorSettings") << line;
	}
}
