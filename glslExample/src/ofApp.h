//
//  Created by Joshua Batty <joshpbatty@gmail.com> on 26/03/2015.
//
#pragma once

#include "ofMain.h"
#include "ofxEditor.h"
#include "SyntaxHighlight.h"

// syntax example using a single text editor instance
class ofApp : public ofBaseApp {

	public:
		void setup();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		
		ofxEditor editor;
		SyntaxHighlight syntaxHighlight;
		bool debug; //< show fps?
    
        ofShader shader;
        ofFbo fbo;
    
    private:
        string shaderFileName;
        int width, height;
        bool bToggleVisable;
};
