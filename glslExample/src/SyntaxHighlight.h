//
//  Created by Joshua Batty <joshpbatty@gmail.com> on 26/03/2015.
//
#pragma once

#include "ofMain.h"
#include "ofxEditor.h"

class SyntaxHighlight {
    
	public:
	
		void setup();
	
		ofxEditorColorScheme colorScheme;
		ofxEditorSyntax syntax;
	
	private:
	
		void setDataTypes();
		void setFunctions();
};
