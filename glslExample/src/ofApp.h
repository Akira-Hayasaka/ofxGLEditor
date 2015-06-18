//
//  Created by Joshua Batty <joshpbatty@gmail.com> on 26/03/2015.
//
#pragma once

#include "ofMain.h"
#include "ofxEditor.h"

// a live glsl pixel shader editor example using a single ofxEditor
//
// app key commands:
//    MOD -> CTRL or Super (Windows Key, Mac CMD)
//
// MOD + s: save pixel shader
// MOD + e: save & evaluate pixel shader
// MOD + l: toggle line wrapping
// MOD + n: toggle line numbers
// MOD + f: toggle fullscreen
// MOD + t: show/hide editor
// MOD + k: toggle auto focus
// MOD + w: toggle text shadow
// MOD + d: toggle debug
//
// see ofxEditor.h for editor key commands
//
class ofApp : public ofBaseApp {

	public:
		void setup();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		
		ofxEditor editor;
		ofxEditorColorScheme colorScheme;
		ofxEditorSyntax syntax;
		bool debug; //< show fps?
    
        ofShader shader;
        ofFbo fbo;
    
    private:
        string shaderName;
        int width, height;
        bool bToggleVisible;
};
