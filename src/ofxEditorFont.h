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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
#include "ofColor.h"
#include "fontstash.h"

/// fontstash library wrapper for efficient text rendering since ofTrueTypeFont
/// is too slow for lots of chars, this may change in the future as the new
/// ofFont & unicode support are integrated into OpenFrameworks
///
/// supports UTF8 but is dependent on what glyphs the loaded font supports,
/// unknown glyphs are simply rendered as spaces
///
/// note: don't use this directly, requires alpha blending to avoid per-char
///       style & color pushes & pops
class ofxEditorFont {

	public:
	
		ofxEditorFont();
		virtual ~ofxEditorFont();
	
	/// \section Main
	
		/// create a fonstash context and load a given font
		/// returns false if the font could not be loaded
		bool load(std::string filename, int fontsize, int textureDimension = 512);
	
		/// returns true if the fonstash context exists (aka font is loaded)
		bool isLoaded();
	
		/// clear the font & fonstash context
		void clear();
	
	/// \section Font Info
	
		/// get the currently loaded font size
		int getFontSize();
	
		/// get the calculated font line height (vertical distance to next line)
		float getLineHeight();
	
		/// get the width for a given char
		float characterWidth(int c);
	
		/// get bounding box width for a given string
		float stringWidth(const std::string& s);
		float stringWidth(const std::u32string& s);
	
		/// get bounding box height for a given string (single line only)
        float stringHeight(const std::string& s);
		float stringHeight(const std::u32string& s);
	
	/// \section Drawing
	
		/// draw a single char / unicode codepoint using the current state color
		/// set shadowed=true to draw an offset shadow using the shadow color
		/// returns new x position
		float drawCharacter(int c, float x, float y, bool shadowed=false);
	
		/// draw a UTF8 string using the current state color
		/// set shadowed=true to draw an offset shadow using the shadow color
		/// returns new x position
		float drawString(const std::string& s, float x, float y, bool shadowed=false);
	
		/// draw a wide char string using the current state color
		/// set shadowed=true to draw an offset shadow using the shadow color
		/// returns new x position
		float drawString(const std::u32string& s, float x, float y, bool shadowed=false);
	
	/// \section Color & State
	
		/// set current state color, default: white
		void setColor(ofColor &c, float alpha=1.0);
	
		/// set cached shadow color (not affected by state push/pop), default: black
		void setShadowColor(ofColor &c, float alpha=1.0);
	
		// push current font state (color)
		void pushState();
	
		// pop current font state (color)
		void popState();
	
	private:
	
		struct FONScontext *context; //< fontstash context
		int font;         //< loaded font id
		int size;         //< requested font size
		float lineHeight; //< computed line height
		
		unsigned int textShadowColor; //< cached text shadow color
	
		/// static C error handler
		static void stashError(void* uptr, int error, int val);
};
