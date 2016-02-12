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
#include "ofxEditorColorScheme.h"
#include "Unicode.h"
#include "ofLog.h"
#include "ofUtils.h"
#include "ofXml.h"

// COLOR SCHEME

//--------------------------------------------------------------
ofxEditorColorScheme::ofxEditorColorScheme() {
	clear();
}

//--------------------------------------------------------------
ofxEditorColorScheme::ofxEditorColorScheme(const string& xmlFile) {
	if(!loadFile(xmlFile)) {
		clear();
	}
}

//--------------------------------------------------------------
bool ofxEditorColorScheme::loadFile(const string& xmlFile) {
	string path = ofToDataPath(xmlFile);
	ofXml xml;
	if(!xml.load(path)) {
		ofLogError("ofxEditorColorScheme") << "couldn't load \""
			<< ofFilePath::getFileName(xmlFile) << "\"";
		return false;
	}
	xml.setToParent();
	if(!xml.exists("colorscheme")) {
		ofLogWarning("ofxEditorColorScheme") << "root xml tag not \"colorscheme\", ignoring";
		return false;
	}
	xml.setTo("colorscheme");
	int numTags = xml.getNumChildren();
	clear();
	for(int i = 0; i < numTags; ++i) {
		xml.setToChild(i);
		if(xml.getName() == "text")    {setColorFromXml(xml, textColor);}
		else if(xml.getName() == "string")  {setColorFromXml(xml, stringColor);}
		else if(xml.getName() == "number")  {setColorFromXml(xml, numberColor);}
		else if(xml.getName() == "comment") {setColorFromXml(xml, commentColor);}
		else if(xml.getName() == "preprocessor") {setColorFromXml(xml, preprocessorColor);}
		else if(xml.getName() == "keyword")  {setColorFromXml(xml, keywordColor);}
		else if(xml.getName() == "typename") {setColorFromXml(xml, typenameColor);}
		else if(xml.getName() == "function") {setColorFromXml(xml, functionColor);}
		else {
			ofLogWarning("ofxEditorColorScheme") << "ignoring unknown xml tag \"" << xml.getName() << "\"";
		}
		xml.setToParent();
	}
	xml.clear();
	
	return true;
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
	preprocessorColor = color;
	keywordColor = color;
	typenameColor = color;
	functionColor = color;
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

// PROTECTED

//--------------------------------------------------------------
void ofxEditorColorScheme::setColorFromXml(ofXml &xml, ofColor &color) {
	if(xml.exists("gray")) {color.set(xml.getIntValue("gray"));}
	if(xml.exists("hex")) {color.setHex(ofHexToInt(xml.getValue("hex")));}
	if(xml.exists("r")) {color.r = xml.getIntValue("r");}
	if(xml.exists("g")) {color.g = xml.getIntValue("g");}
	if(xml.exists("b")) {color.b = xml.getIntValue("b");}
	if(xml.exists("a")) {color.a = xml.getIntValue("a");}
}
