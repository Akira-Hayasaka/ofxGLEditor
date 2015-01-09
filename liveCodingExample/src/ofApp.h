#pragma once

#include "ofMain.h"

#include "ofxLua.h"
#include "ofxEditor.h"

// this is a simple live coding example using ofxLua with 
//
// key commands:
//    MOD -> CTRL or Super (Windows Key, Mac CMD)
//
// MOD + e: execute script or selected text
// MOD + l: toggle line wrapping
// MOD + n: toggle line numbers
// MOD + f: toggle fullscreen
// MOD + t: show/hide editor
class ofApp : public ofBaseApp, ofxLuaListener {

	public:

    // main
    void setup();
    void update();
    void draw();
    void exit();
    
    // input
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    // ofxLua error callback
    void errorReceived(string& msg);
    
    // script control
    void executeScript();
    
    ofxLua lua;
    string script;
    
    ofxEditor editor;
    ofxEditorColorScheme colorScheme;
	bool hideEditor;
};
