#pragma once

#include "ofColor.h"
#include "ofConstants.h"
#include <map>

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
	
		/// single & multi line comment coloring
		void setCommentColor(ofColor color);
		ofColor& getCommentColor();
	
		/// set the beginning string to match for a single line comment,
		/// blank by default
		void setSingleLineComment(string begin);
		string getSingleLineComment();
	
		/// set the beginning and ending strings to match for a multi line comment,
		/// blank by default
		void setMultiLineComment(string begin, string end);
		string getMultiLineCommentBegin();
		string getMultiLineCommentEnd();
		
		/// colors for specfic keywords
		void setWordColor(const string& word, ofColor color);
		void clearWordColor(const string &word);
		void clearAllWordColors();
		ofColor& getWordColor(const string &word);
		
	protected:
	
		ofColor textColor;
		ofColor stringColor;
		ofColor numberColor;
		ofColor commentColor;
		map<string,ofColor> wordColors;
	
		string singleLineComment;
		string multiLineCommentBegin;
		string multiLineCommentEnd;
};
