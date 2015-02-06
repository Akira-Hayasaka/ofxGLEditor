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

#include "ofConstants.h"
#include "fontstash.h"

/// fontstash library wrapper for efficient text rendering since ofTrueTypeFont
/// is too slow for lots of chars, this may change in the future as the new
/// ofFont & unicode support are integrated into OpenFrameworks
///
/// note: don't use this directly
class ofxEditorFont {

	public:
	
		ofxEditorFont();
		virtual ~ofxEditorFont();
	
		bool loadFont(string filename, int fontsize, int textureDimension = 512);
        bool isLoaded();
		void clear();
	
		float getLineHeight();
		float stringWidth(const string& s);
        float stringHeight(const string& s);
	
		void drawCharacter(int c, float x, float y);
		void drawString(const string& s, float x, float y);
	
	private:
	
		struct FONScontext *context; //< fontstash context
		int font;         //< loaded font id
		int size;         //< requested font size
		float lineHeight; //< computed line height
	
		/// static C error handler
		static void stashError(void* uptr, int error, int val);
};
