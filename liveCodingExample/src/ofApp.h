/*
 * Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 */
#pragma once

#include "ofMain.h"

#include "ofxLua.h"
#include "ofxEditor.h"

// this is a simple live coding example using ofxLua with 
//
// app key commands:
//    MOD -> CTRL or Super (Windows Key, Mac CMD)
//
// MOD + e: execute script or selected text
// MOD + l: toggle line wrapping
// MOD + n: toggle line numbers
// MOD + f: toggle fullscreen
// MOD + k: toggle auto focus zooming
// MOD + t: show/hide editor
//
// see ofxGLEditor.h for editor key commands
//
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
	ofxEditorSyntax syntax;
    ofxEditorColorScheme colorScheme;
	bool hideEditor;
};
