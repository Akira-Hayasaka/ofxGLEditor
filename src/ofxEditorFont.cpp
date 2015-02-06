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
#include "ofxEditorFont.h"

#include "ofMain.h"
#include "Unicode.h"

#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"

#define ATLAS_MAX_SIZE 2048

//--------------------------------------------------------------
ofxEditorFont::ofxEditorFont() {
	context = NULL;
	font = 0;
	size = 0;
	lineHeight = 0;
}

//--------------------------------------------------------------
ofxEditorFont::~ofxEditorFont() {
	clear();
}

//--------------------------------------------------------------
bool ofxEditorFont::loadFont(string filename, int fontsize, int textureDimension) {
	
	clear();
	
	textureDimension = ofNextPow2(textureDimension);
	context = glfonsCreate(textureDimension, textureDimension, FONS_ZERO_TOPLEFT);
	
	font = fonsAddFont(context, "normal", ofToDataPath(filename).c_str());
	if(font == FONS_INVALID) {
		ofLogError("ofxEditorFont") << "couldn't load font: " << filename;
		if(context) {
			glfonsDelete(context);
		}
		context = NULL;
		return false;
	}
	size = fontsize;
	fonsSetFont(context, font);
	fonsSetSize(context, size);
	fonsSetBlur(context, 0);
	fonsVertMetrics(context, NULL, NULL, &lineHeight); lineHeight *= 0.5;
	fonsSetErrorCallback(context, ofxEditorFont::stashError, context);
	
	return true;
}

//--------------------------------------------------------------
bool ofxEditorFont::isLoaded() {
	return context != NULL;
}

//--------------------------------------------------------------
void ofxEditorFont::clear() {
	if(context) {
		glfonsDelete(context);
	}
	context = NULL;
	font = 0;
	size = 0;
	lineHeight = 0;
}

//--------------------------------------------------------------
float ofxEditorFont::getLineHeight() {
	return lineHeight;
}

//--------------------------------------------------------------
float ofxEditorFont::stringWidth(const string& s) {
	float bounds[4] = {0, 0, 0, 0};
	fonsTextBounds(context, 0, 0, s.c_str(), NULL, bounds);
	return bounds[2] - bounds[0]; // maxx - minx
}

//--------------------------------------------------------------
float ofxEditorFont::stringHeight(const string& s) {
	float bounds[4] = {0, 0, 0, 0};
	fonsTextBounds(context, 0, 0, s.c_str(), NULL, bounds);
	return bounds[3] - bounds[1]; // maxy - miny
}

//--------------------------------------------------------------
void ofxEditorFont::drawCharacter(int c, float x, float y) {
	unsigned int color = glfonsRGBA(ofGetStyle().color.r, ofGetStyle().color.g, ofGetStyle().color.b, ofGetStyle().color.a);
	fonsSetColor(context, color);
	if(c < 0x80) { // ASCII
		string s = " ";
		s[0] = c;
		return fonsDrawText(context, x, y, s.c_str(), NULL)-x;
	}
	else { // UTF-8
		return fonsDrawText(context, x, y, wchar_to_string(c).c_str(), NULL)-x;
	}
}

//--------------------------------------------------------------
void ofxEditorFont::drawString(const string& s, float x, float y) {
	unsigned int color = glfonsRGBA(ofGetStyle().color.r, ofGetStyle().color.g, ofGetStyle().color.b, ofGetStyle().color.a);
	fonsSetColor(context, color);
	fonsDrawText(context, x, y, s.c_str(), NULL);
}

// PRIVATE

//--------------------------------------------------------------
void ofxEditorFont::stashError(void* uptr, int error, int val) {
	(void)uptr;
	FONScontext* context = (FONScontext*)uptr;
	switch(error) {
		case FONS_ATLAS_FULL: {
			ofLogError("ofxEditorFont") << "font atlas texture full, expanding";
			int width, height;
			fonsGetAtlasSize(context, &width, &height);
			if(width <= ATLAS_MAX_SIZE && height <= ATLAS_MAX_SIZE) {
				fonsExpandAtlas(context, width*2, height*2);
			}
			else {
				ofLogError("ofxEditorFont") << "couldn't expand altas more than " << ATLAS_MAX_SIZE;
			}
			break;
		}
		case FONS_SCRATCH_FULL:
			ofLogError("ofxEditorFont") << "scratch full, tried to allocate " << val << "has" << FONS_SCRATCH_BUF_SIZE;
			break;
		case FONS_STATES_OVERFLOW:
			ofLogError("ofxEditorFont") << "state overflow";
			break;
		case FONS_STATES_UNDERFLOW:
			ofLogError("ofxEditorFont") << "state underflow";
			break;
	}
}
