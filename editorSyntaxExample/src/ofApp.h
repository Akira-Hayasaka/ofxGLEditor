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
#include "ofxEditor.h"

// syntax example using a single text editor instance
//
// app key commands:
//    MOD -> CTRL or Super (Windows Key, Mac CMD)
//
// MOD + s: toggle syntax highlighting
// MOD + d: toggle debug framerate display
// MOD + f: toggle fullscreen
// MOD + l: toggle line wrapping
// MOD + n: toggle line numbers
// MOD + k: toggle auto focus zooming
// MOD + w: toggle text shadow
// MOD + 1: set line position (debugging)
// MOD + 2: set line position again (debugging)
// MOD + p: print current syntax parser text blocks (debugging)
//
// see ofxGLEditor.h for editor key commands
//
class ofApp : public ofBaseApp {

	public:
		void setup();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		
		ofxEditor editor;
		ofxEditorColorScheme colorScheme;
		ofxEditorSyntax luaSyntax;
		bool debug; //< show fps?
};
