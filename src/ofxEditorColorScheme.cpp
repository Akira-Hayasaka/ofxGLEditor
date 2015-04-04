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

// COLOR SCHEME

//--------------------------------------------------------------
ofxEditorColorScheme::ofxEditorColorScheme() {
	clear();
}

//--------------------------------------------------------------
void ofxEditorColorScheme::clear() {
	clear(ofColor(255));
}

//--------------------------------------------------------------
void ofxEditorColorScheme::clear(ofColor color) {
	textColor = color;
	stringColor = color;
	numberColor = color;
	commentColor = color;
	keywordColor = color;
	typenameColor = color;
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
void ofxEditorColorScheme::setPreprocessorColor(ofColor color) {
	preprocessorColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getPreprocessorColor() {
	return preprocessorColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setKeywordColor(ofColor color) {
	keywordColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getKeywordColor() {
	return keywordColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setTypenameColor(ofColor color) {
	typenameColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getTypenameColor() {
	return typenameColor;
}

//--------------------------------------------------------------
void ofxEditorColorScheme::setFunctionColor(ofColor color) {
	functionColor = color;
}

//--------------------------------------------------------------
ofColor& ofxEditorColorScheme::getFunctionColor() {
	return functionColor;
}
