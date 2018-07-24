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
ofxEditorColorScheme::ofxEditorColorScheme(const std::string& xmlFile) {
	if(!loadFile(xmlFile)) {
		clear();
	}
}

//--------------------------------------------------------------
bool ofxEditorColorScheme::loadFile(const std::string& xmlFile) {
	std::string path = ofToDataPath(xmlFile);
	ofXml xml;
	if(!xml.load(path)) {
		ofLogError("ofxEditorColorScheme") << "couldn't load \""
			<< ofFilePath::getFileName(xmlFile) << "\"";
		return false;
	}
	auto root = xml.getChild("colorscheme");
	if(!root) {
		ofLogWarning("ofxEditorColorScheme") << "root xml tag not \"colorscheme\", ignoring";
		return false;
	}
	clear();
	for(auto &child : root.getChildren()) {
		if(child.getName() == "text")         {setColorFromXml(child, textColor);}
		else if(child.getName() == "string")  {setColorFromXml(child, stringColor);}
		else if(child.getName() == "number")  {setColorFromXml(child, numberColor);}
		else if(child.getName() == "comment") {setColorFromXml(child, commentColor);}
		else if(child.getName() == "preprocessor") {setColorFromXml(child, preprocessorColor);}
		else if(child.getName() == "keyword")  {setColorFromXml(child, keywordColor);}
		else if(child.getName() == "typename") {setColorFromXml(child, typenameColor);}
		else if(child.getName() == "function") {setColorFromXml(child, functionColor);}
		else {
			ofLogWarning("ofxEditorColorScheme") << "ignoring unknown xml tag \"" << child.getName() << "\"";
		}
	}
	
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
	auto gray = xml.getChild("gray");
	auto hex = xml.getChild("hex");
	auto r = xml.getChild("r");
	auto g = xml.getChild("g");
	auto b = xml.getChild("b");
	auto a = xml.getChild("a");
	if(gray) {color.set(gray.getIntValue());}
	if(hex) {color.setHex(ofHexToInt(hex.getValue()));}
	if(r) {color.r = r.getIntValue();}
	if(g) {color.g = g.getIntValue();}
	if(b) {color.b = b.getIntValue();}
	if(a) {color.a = a.getIntValue();}
}
