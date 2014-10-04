#pragma once

#include "ofMain.h"

class ofxEditorColorScheme {

	public:
	
		ofxEditorColorScheme();
		virtual ~ofxEditorColorScheme();
		
		// default text color
		void setTextColor(ofColor &color);
		ofColor& getTextColor();
		
		// specific word colors
		void setWordColor(const string& word, ofColor color);
		void clearWordColor(const string &word);
		void clearAllWordColors();
		ofColor& getWordColor(const string &word);
		
		// string coloring
		void setStringColor(ofColor color);
		ofColor& getStringColor();
		
		// number coloring
		void setNumberColor(ofColor color);
		ofColor& getNumberColor();
		
	protected:
	
		ofColor textColor;
		ofColor stringColor;
		ofColor numberColor;
		map<string,ofColor> wordColors;
};
