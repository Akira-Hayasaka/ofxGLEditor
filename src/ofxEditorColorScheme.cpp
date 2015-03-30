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
#include "ofxEditorColorScheme.h"
#include "Unicode.h"
#include "ofLog.h"

//--------------------------------------------------------------
ofxEditorColorScheme::ofxEditorColorScheme() {
	clear();
}

//--------------------------------------------------------------
ofxEditorColorScheme::~ofxEditorColorScheme() {
	clearAllWordColors();
}

//--------------------------------------------------------------
void ofxEditorColorScheme::clear() {
	textColor.set(255);
	stringColor.set(255);
	numberColor.set(255);
	commentColor.set(255);
	clearAllWordColors();
	
	mathChars = L"+-*/!|&~";
	punctuationChars = L";:,?";
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setTextColor(ofColor color) {
	textColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getTextColor() {
	return textColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setStringColor(ofColor color) {
	stringColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getStringColor() {
	return stringColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setNumberColor(ofColor color) {
	numberColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getNumberColor() {
	return numberColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setCommentColor(ofColor color) {
	commentColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getCommentColor() {
	return commentColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setSingleLineComment(const wstring &begin) {
	singleLineComment = begin;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setSingleLineComment(const string &begin) {
	singleLineComment = string_to_wstring(begin);
}

//--------------------------------------------------------------
wstring& ofxEditorColorScheme::getWideSingleLineComment() {
	return singleLineComment;
}

//--------------------------------------------------------------
string ofxEditorColorScheme::getSingleLineComment() {
	return wstring_to_string(singleLineComment);
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setMultiLineComment(const wstring &begin, const wstring &end) {
	multiLineCommentBegin = begin;
	multiLineCommentEnd = end;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setMultiLineComment(const string &begin, const string &end) {
	multiLineCommentBegin = string_to_wstring(begin);
	multiLineCommentEnd = string_to_wstring(end);
}

//--------------------------------------------------------------
wstring& ofxEditorColorScheme::getWideMultiLineCommentBegin() {
	return multiLineCommentBegin;
}

//--------------------------------------------------------------
string ofxEditorColorScheme::getMultiLineCommentBegin() {
	return wstring_to_string(multiLineCommentBegin);
}

//--------------------------------------------------------------
wstring& ofxEditorColorScheme::getWideMultiLineCommentEnd() {
	return multiLineCommentEnd;
}

//--------------------------------------------------------------
string ofxEditorColorScheme::getMultiLineCommentEnd() {
	return wstring_to_string(multiLineCommentEnd);
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setWordColor(const wstring &word, ofColor color) {
	map<wstring,ofColor>::iterator iter = wordColors.find(word);
	if(iter != wordColors.end()) { // already exists
		ofColor &c = (*iter).second;
		c.set(color);
	}
	else { // doesn't exist
		wordColors.insert(pair<wstring,ofColor>(word, color));
	}
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setWordColor(const string &word, ofColor color) {
	setWordColor(string_to_wstring(word), color);
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getWordColor(const wstring &word) {
	map<wstring,ofColor>::iterator iter = wordColors.find(word);
	if(iter != wordColors.end()) {
		return (*iter).second;
	}
	return textColor;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getWordColor(const string &word) {
	return getWordColor(string_to_wstring(word));
}

//--------------------------------------------------------------
void ofxEditorColorScheme::clearWordColor(const wstring &word) {
	map<wstring,ofColor>::iterator iter = wordColors.find(word);
	if(iter != wordColors.end()) {
		wordColors.erase(iter);
	}
}

//--------------------------------------------------------------
void ofxEditorColorScheme::clearWordColor(const string &word) {
	clearWordColor(string_to_wstring(word));
}

//--------------------------------------------------------------
void ofxEditorColorScheme::clearAllWordColors() {
	wordColors.clear();
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setMathChars(wstring chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSettings") << "empty math string";
		return;
	}
	mathChars = chars;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setMathChars(string chars) {
	setMathChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
wstring& ofxEditorColorScheme::getWideMathChars() {
	return mathChars;
}

//--------------------------------------------------------------
string ofxEditorColorScheme::getMathChars() {
	return wstring_to_string(mathChars);
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setPunctuationChars(wstring chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSettings") << "empty punctuation string";
		return;
	}
	punctuationChars = chars;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setPunctuationChars(string chars) {
	setPunctuationChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
wstring& ofxEditorColorScheme::getWidePunctuationChars() {
	return punctuationChars;
}

//--------------------------------------------------------------
string ofxEditorColorScheme::getPunctuationChars() {
	return wstring_to_string(punctuationChars);
}
