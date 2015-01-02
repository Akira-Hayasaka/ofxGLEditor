#pragma once

#include "ofColor.h"
#include "ofConstants.h"

/// internal editor settings which can be shared between instances
struct ofxEditorSettings {
		
		unsigned int tabWidth; //< tab width in spaces
		bool convertTabs;      //< convert tabs to spaces?
		
		float alpha;                //< overall text alpha
		ofColor textColor;		    //< general text color, overridden by color scheme
		ofColor cursorColor;	    //< text pos cursor color
		ofColor selectionColor;     //< char selection background color
		ofColor matchingCharsColor; //< matching chars background color
		ofColor lineNumberColor;    //< line number color
	
		bool highlightMatchingChars; //< highlight matching open/close chars?
		string openChars;  //< open chars (parens, brackets, etc) for matching highlight
		string closeChars; //< close chars (parens, bracket, etc) for matching highlight
	
		// defaults
		ofxEditorSettings() {

			tabWidth = 4;
			convertTabs = false;

			alpha = 1.0f;
			textColor = ofColor(255);
			cursorColor = ofColor(255, 255, 0, 200);
			selectionColor = ofColor(0, 255, 0, 127);
			matchingCharsColor = ofColor(0, 127, 255, 127);
			lineNumberColor = ofColor(127);

			highlightMatchingChars = true;
			openChars = "([<{";
			closeChars = ")]>}";
		}
};
