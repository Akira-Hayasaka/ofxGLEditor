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
	openChars = L"([<{";
	closeChars = L")]>}";
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
