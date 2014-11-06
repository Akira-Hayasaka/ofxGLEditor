#pragma once

#include "ofMain.h"

/// syntax highlighter color scheme
class ofxEditorColorScheme {

	public:
	
		ofxEditorColorScheme();
		virtual ~ofxEditorColorScheme();
		
		/// clear all colors to white
		void clear();
		
		/// default text color
		void setTextColor(ofColor color);
		ofColor& getTextColor();
		
		/// string coloring
		void setStringColor(ofColor color);
		ofColor& getStringColor();
		
		/// number coloring
		void setNumberColor(ofColor color);
		ofColor& getNumberColor();
		
		/// colors for specfic keywords
		void setWordColor(const string& word, ofColor color);
		void clearWordColor(const string &word);
		void clearAllWordColors();
		ofColor& getWordColor(const string &word);
		
	protected:
	
		ofColor textColor;
		ofColor stringColor;
		ofColor numberColor;
		map<string,ofColor> wordColors;
};
