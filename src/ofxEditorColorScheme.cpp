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
 */
#include "ofxEditorColorScheme.h"

ofxEditorColorScheme::ofxEditorColorScheme() {
}

ofxEditorColorScheme::~ofxEditorColorScheme() {
	wordColors.clear();
}

void ofxEditorColorScheme::clear() {
	textColor.set(255);
	clearAllWordColors();
	stringColor.set(255);
	numberColor.set(255);
	commentColor.set(255);
}

void ofxEditorColorScheme::setTextColor(ofColor color) {
	textColor = color;
}

ofColor& ofxEditorColorScheme::getTextColor() {
	return textColor;
}

void ofxEditorColorScheme::setStringColor(ofColor color) {
	stringColor = color;
}

ofColor& ofxEditorColorScheme::getStringColor() {
	return stringColor;
}

void ofxEditorColorScheme::setNumberColor(ofColor color) {
	numberColor = color;
}

ofColor& ofxEditorColorScheme::getNumberColor() {
	return numberColor;
}

void ofxEditorColorScheme::setCommentColor(ofColor color) {
	commentColor = color;
}

ofColor& ofxEditorColorScheme::getCommentColor() {
	return commentColor;
}

void ofxEditorColorScheme::setSingleLineComment(string begin) {
	singleLineComment = begin;
}

string ofxEditorColorScheme::getSingleLineComment() {
	return singleLineComment;
}

/// set the beginning and ending strings to match for a multi line comment,
/// blank by default
void ofxEditorColorScheme::setMultiLineComment(string begin, string end) {
	multiLineCommentBegin = begin;
	multiLineCommentEnd = end;
}

string ofxEditorColorScheme::getMultiLineCommentBegin() {
	return multiLineCommentBegin;
}

string ofxEditorColorScheme::getMultiLineCommentEnd() {
	return multiLineCommentEnd;
}

void ofxEditorColorScheme::setWordColor(const string& word, ofColor color) {
	map<string,ofColor>::iterator iter = wordColors.find(word);
	if(iter != wordColors.end()) { // already exists
		ofColor &c = (*iter).second;
		c.set(color);
	}
	else { // doesn't exist
		wordColors.insert(pair<string,ofColor>(word, color));
	}
}

void ofxEditorColorScheme::clearWordColor(const string &word) {
	map<string,ofColor>::iterator iter = wordColors.find(word);
	if(iter != wordColors.end()) {
		wordColors.erase(iter);
	}
}

void ofxEditorColorScheme::clearAllWordColors() {
	wordColors.clear();
}

ofColor& ofxEditorColorScheme::getWordColor(const string &word) {
	map<string,ofColor>::iterator iter = wordColors.find(word);
	if(iter != wordColors.end()) {
		return (*iter).second;
	}
	return textColor;
}
