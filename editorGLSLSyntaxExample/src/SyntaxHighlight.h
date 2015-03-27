//
//  SyntaxHighlight.h
//  ofxGLEditor_example
//
//  Created by Joshua Batty on 26/03/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxEditor.h"

class SyntaxHighlight{
    
public:
    void setup(ofxEditorColorScheme *colorScheme);

    void setDataTypes();
    void setPreprocessor();
    void setFunctions();
    
private:
    vector<string> dataType;
    vector<string> preprocessor;
    vector<string> functions;
};
