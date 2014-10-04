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
}

void ofxEditorColorScheme::setTextColor(ofColor &color) {
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
