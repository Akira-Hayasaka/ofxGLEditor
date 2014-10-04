#pragma once

#include "ofMain.h"

class ofxEditorHighlights {

	public:
	
		ofxEditorHighlights();
		virtual ~ofxEditorHighlights();
		
		// default text color
		void setTextColor(ofColor &color);
		ofColor& getTextColor();
		
		// word highlighting
		void setHighlight(const string& word, ofColor color);
		void clearHighlight(const string &word);
		void clearAllHighlights();
		ofColor& getHighlight(const string &word);
		
		// string highlighting
		void setStringHighlight(ofColor color);
		ofColor& getStringHighlight();
		
		// number highlighting
		void setNumberHighlight(ofColor color);
		ofColor& getNumberHighlight();
		
	protected:
	
		ofColor textColor;
		ofColor stringColor;
		ofColor numberColor;
		map<string,ofColor> highlights;
};
