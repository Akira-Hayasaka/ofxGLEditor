#pragma once

#include "ofColor.h"
#include "ofConstants.h"

/// internal editor settings which can be shared between instances
struct ofxEditorSettings {
	
	/// tab width in spaces, must be at least 1, default: 4
	unsigned int tabWidth;

	/// convert tabs to spaces? performed when setting or editing text and *cannot* be undone
	bool convertTabs;
	
	float alpha;                //< overall text alpha, must be in the range 0.0 - 1.0
	ofColor textColor;		    //< general text color, default: white, overridden by color scheme
	ofColor cursorColor;	    //< text pos cursor color, default; yellow w/ alpha
	ofColor selectionColor;     //< char selection highlight color, default: green w/ alpha
	ofColor matchingCharsColor; //< matching chars highlight color, default: blue w/ alpha
	ofColor lineNumberColor;    //< line number color, default: gray

	bool highlightMatchingChars; //< highlight matching open/close chars?, default: true

	/// set the matching open/close chars to highlight, strings should not be empty
	/// note: matching chars are not highlighted inside comments
	string openChars;  //< open chars (parens, brackets, etc) for matching highlight, default: "([<{"
	string closeChars; //< close chars (parens, bracket, etc) for matching highlight, default: ")]>}"

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
