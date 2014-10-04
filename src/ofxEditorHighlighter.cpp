#include "ofxEditorHighlighter.h"

ofxEditorHighlights::ofxEditorHighlights() {
}

ofxEditorHighlights::~ofxEditorHighlights() {
	highlights.clear();
}

void ofxEditorHighlights::setTextColor(ofColor &color) {
	textColor = color;
}

ofColor& ofxEditorHighlights::getTextColor() {
	return textColor;
}

void ofxEditorHighlights::setHighlight(const string& word, ofColor color) {
	map<string,ofColor>::iterator iter = highlights.find(word);
	if(iter != highlights.end()) { // already exists
		ofColor &c = (*iter).second;
		c.set(color);
	}
	else { // doesn't exist
		highlights.insert(pair<string,ofColor>(word, color));
	}
}

void ofxEditorHighlights::clearHighlight(const string &word) {
	map<string,ofColor>::iterator iter = highlights.find(word);
	if(iter != highlights.end()) {
		highlights.erase(iter);
	}
}

void ofxEditorHighlights::clearAllHighlights() {
	highlights.clear();
}

ofColor& ofxEditorHighlights::getHighlight(const string &word) {
	map<string,ofColor>::iterator iter = highlights.find(word);
	if(iter != highlights.end()) {
		return (*iter).second;
	}
	return textColor;
}

void ofxEditorHighlights::setStringHighlight(ofColor color) {
	stringColor = color;
}

ofColor& ofxEditorHighlights::getStringHighlight() {
	return stringColor;
}

void ofxEditorHighlights::setNumberHighlight(ofColor color) {
	numberColor = color;
}

ofColor& ofxEditorHighlights::getNumberHighlight() {
	return numberColor;
}
