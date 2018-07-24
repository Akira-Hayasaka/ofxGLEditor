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
 *
 * A rewrite of the Fluxus GLEditor http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#pragma once

#include "ofMain.h"
#include "ofxEditorSettings.h"
#include "ofxEditorColorScheme.h"

// custom fontstash wrapper
class ofxEditorFont;
class ofxGLEditor;

/// full screen text editor with optional syntax highlighting,
/// based on the Fluxus GLEditor
///
/// uses wide chars internally so string conversion functions are provided
class ofxEditor {

	public:
	
		ofxEditor();
		ofxEditor(ofxEditorSettings &sharedSettings); //< share settings object
		virtual ~ofxEditor();
		
	/// \section Static Settings
		
		/// load font to be used by all editors, *must* be a fixed width font
		///
		/// call this before drawing any editor
		///
		static bool loadFont(const std::string &font, int size);
	
		/// is a font currently loaded?
		static bool isFontLoaded();
	
		/// get the fixed width of the space char using editor font
		static int getCharWidth();
	
		/// get the fixed height of a char using editor font
		static int getCharHeight();
	
		/// enable/disable text offset shadow, default: true
		static void setTextShadow(bool shadow=true);
	
		/// is text being drawn with an offset shadow?
		static bool getTextShadow();
	
		/// set useSuper = true if you want to use the Super (Windows key, Mac CMD)
		/// key as the modifier key, otherwise false uses CTRL key
		/// default: true on Mac & false on all other platforms
		static void setSuperAsModifier(bool useSuper=true);
	
		/// is the Super key the modifier key? if not, then CTRL
		static bool getSuperAsModifier();
	
		/// set the minimum auto focus zoom scale, default: 0.5
		static void setAutoFocusMinScale(float min);
	
		/// get the minimum auto focus zoom scale
		static float getAutoFocusMinScale();
	
		/// set the maximum auto focus zoom scale, default: 5.0
		static void setAutoFocusMaxScale(float max);
	
		/// get the maximum auto focus zoom scale
		static float getAutoFocusMaxScale();
	
		/// set the auto focus scaling speed, default: 1.0
		/// clamped to 0.1 - 2.0, anything > ~1.2 may be glitchy depending on framerate
		static void setAutoFocusSpeed(float speed);
	
		/// get the auto focus scaling speed
		static float getAutoFocusSpeed();
	
		/// enable/disable undo action saving (default: true)
		static void setUndo(bool saveUndo=true);
	
		/// is undo enabled?
		static bool getUndo();
	
		/// set the max number of undo actions to save (default: 10)
		/// pops oldest actions if new depth is smaller than old depth
		static void setUndoDepth(unsigned int depth);
	
		/// get the current max number of undo actions
		static unsigned int getUndoDepth();
	
	/// \section Main
		
		/// draw the editor, pushes view and applies viewport
		virtual void draw();

		/// handles editor key events
		///
		/// MOD -> CTRL or Super (Windows Key, Mac CMD), set with
		///        ofxEditor::setSuperAsModifier()
		///
		/// MOD + b: blow up the cursor
		/// MOD + a: select all text
		/// MOD + a + SHIFT: clear all text
		/// MOD + c: copy from system clipboard (GLFW only) or copy buffer
		/// MOD + v: paste to system clipboard (GLFW only) or copy buffer
		///
		/// ARROWS + SHIFT: select text
		///
		/// note: ESC clears the current selection, but you'll need to set
		///       ofSetEscapeQuitsApp(false) to use it
		///
		virtual void keyPressed(int key);
		
		/// set the view port size,
		/// size of text field calculated based on font size
		virtual void resize();
		virtual void resize(int width, int height);
	
		/// open & load a file, clears existing text
		/// returns true on success
		virtual bool openFile(std::string filename);
		
		/// save the text to a file
		/// returns true on success
		virtual bool saveFile(std::string filename);
	
		/// get wide char text buffer contents or current selection
		virtual std::u32string getWideText();
	
		/// get text buffer contents or current selection
		virtual std::string getText();
	
		/// set text buffer contents
		virtual void setText(const std::u32string& text);
	
		/// set text buffer contents
		virtual void setText(const std::string& text);
	
		/// insert text at the current buffer position
		virtual void insertText(const std::u32string& text);
	
		/// insert text at the current buffer position
		virtual void insertText(const std::string& text);
	
		/// delete char(s) or selection at the current buffer position
		/// set optional number of chars and direction (default: current char)
		virtual void deleteText(unsigned int numChars=1, bool forward=true);
	
		/// clear text buffer contents
		virtual void clearText();
	
	/// \section Settings

		/// access to the internal settings object
		///
		/// example usage:
		///
		///   editor.getSettings().getAlpha(0.5);
		///   editor.getSettings().getTabWidth(8);
		///   editor.getSettings().setLangSyntax("Lua", &luaSyntax);
		///   editor.getSettings().setFileExtLang("Lua", "lua");
		///   ... etc
		///
		ofxEditorSettings& getSettings();
	
	/// \section Color Scheme
	
		/// set color scheme for this editor and highlight syntax
		/// note: pointer is never deleted
		void setColorScheme(ofxEditorColorScheme *colorScheme);
	
		/// clear the current color scheme
		void clearColorScheme();
	
		/// get the current color scheme, returns NULL if not set
		ofxEditorColorScheme* getColorScheme();
	
	/// \section Language Syntax
	
		/// set language syntax for this editor
		/// note: pointer is never deleted
		void setSyntax(ofxEditorSyntax *syntax);
	
		/// set language syntax for this editor based on language string
		/// aka "C++", "Lua", "GLSL", etc from the available syntaxes in the
		/// current settings
		void setLangSyntax(const std::string& lang);
	
		/// set language syntax for this editor based on file extension string
		/// from the available syntaxes in the current settings
		///
		/// note: file extensions do not include the period, ex: "lua" not ".lua"
		void setFileExtSyntax(const std::string& ext);
	
		/// clear the current language syntax
		void clearSyntax();
	
		/// get the current language syntax, returns NULL if not set
		ofxEditorSyntax* getSyntax();
	
	/// \section Display Settings
	
		/// enable/disable line wrapping
		void setLineWrapping(bool wrap=true);
	
		/// get line wrapping value
		bool getLineWrapping();
	
		/// enable/disable line numbers
		void setLineNumbers(bool numbers=true);
	
		/// get line numbers value
		bool getLineNumbers();
	
		/// enable/disable auto focus zooming
		void setAutoFocus(bool focus=true);
	
		/// get auto focus value
		bool getAutoFocus();
	
	/// \section Current Position & Info
	
		/// animate the cursor so it's easy to find
		void blowupCursor();

		/// flash current text selection
		void flashSelection();

		/// flash text from start position to end position
		void flashText(unsigned int start, unsigned int end);
	
		/// returns true if text is currently selected
		bool isSelection();
	
		/// get the total number of lines in the text buffer
		unsigned int getNumLines();
	
		/// get the total number of characters in the text buffer
		unsigned int getNumCharacters();
	
		/// get the 1D cursor position in the text buffer
		unsigned int getCurrentPos();
	
		/// set the 1D cursor position in the text buffer
		void setCurrentPos(unsigned int pos);
	
		/// get the line the cursor is on
		/// note: starts at 0
		unsigned int getCurrentLine();
	
		/// set the current line for the cursor
		/// note: starts at 0
		void setCurrentLine(unsigned int line);
	
		/// get the current line character position of the cursor
		unsigned int getCurrentLinePos();
	
		/// get length of the current line cursor is on
		unsigned int getCurrentLineLen();
	
		/// get the cursor position by line and line character
		void setCurrentLinePos(unsigned int line, unsigned int character);
	
		/// reset position & selection
		void reset();
	
	/// \section Undo
	
		/// undo one action, clears newer redo actions
		virtual void undo();
	
		/// redo one action
		virtual void redo();
	
		/// clear undo actions
		void clearUndo();
	
	/// \section Utils
	
		/// draw a wide char string using the current editor font
		/// returns the width of drawn text
		float drawString(std::u32string s, float x, float y);
		float drawString(std::u32string s, ofPoint& p);
	
		/// draw a string using the current editor font
		/// returns the width of drawn text
		float drawString(std::string s, float x, float y);
		float drawString(std::string s, ofPoint& p);
	
		/// print the current syntax parser text blocks, useful for debugging
		void printSyntax();
	
		/// print the current undo state, useful for debugging
		void printUndo();
	
	protected:
	
		friend ofxGLEditor;
	
	/// \section Static Variables
	
		static std::shared_ptr<ofxEditorFont> s_font; //< global editor font
		static int s_charWidth;          //< space char pixel width
		static int s_zeroWidth;          //< zero char pixel width for line nums
		static int s_charHeight;         //< char block pixel height
		static int s_cursorWidth;        //< cursor width, 1/3 char width
		static bool s_textShadow;        //< draw text with a 2px offset shadow?
	
		static bool s_superAsModifier;   //< use the super key as modifier?
	
		/// shared copy/paste buffer if system clipboard isn't available
		static std::u32string s_copyBuffer;
	
		/// timestamp for calculating animations, shared between editors for smooth
		/// animation when switching back and forth
		static float s_time;
	
		// auto focus
		static float s_autoFocusError; //< scale snapping amount
		static float s_autoFocusSpeed; //< scale speed (shrink/grow modifier)
		static float s_autoFocusMinScale;   //< minimum allowed scaling
		static float s_autoFocusMaxScale;   //< maximum allowed scaling
	
		// undo
		static bool s_undo; //< save undo actions?
		static unsigned int s_undoMaxDepth; //< maximum number of undo actions
	
	/// \section Member Variables

		ofxEditorSettings *m_settings; //< editor settings object
		bool m_sharedSettings; //< are the settings shared? if so, do not delete
	
		std::u32string m_text; //< text buffer
		unsigned int m_numLines; //< number of lines in the text buffer
		
		float m_width, m_height; //< editor viewport pixel size
		float m_posX, m_posY;    //< editor offset, calculated by line pos & auto focus
		unsigned int m_position;    //< 1D text pos within buffer
		unsigned int m_desiredXPos; //< desired char pos on current line
	
		int m_visibleWidth; //< computed text field pixel width minus line nums
		int m_visibleLines; //< computed text field num lines
	
		int m_matchingCharsHighlight[2]; //< start & end pos for matching chars highlight
	
		/// selection states
		enum SelectionState {
			NONE,
			FORWARD,
			BACKWARD,
			ALL
		};
		SelectionState m_selection; //< is text being selected and how
		unsigned int m_highlightStart; //< highlight start pos in buffer
		unsigned int m_highlightEnd;   //< highlight end pos in buffer
		unsigned int m_selectAllStartPos; //< return pos when pressed left after select all
	
		unsigned int m_topTextPosition;    //< top start pos in buffer for vert scrolling
		unsigned int m_bottomTextPosition; //< bottom end pos in buffer for vert scrolling
		unsigned int m_displayedLineCount; //< current number of displayed lines (may be diff from m_visibleLines)
	
		bool m_shiftState; //< is shift pressed?
		unsigned int m_UTF8Bytes; //< number of expected UTF8 input bytes
		std::string m_UTF8Char; //< used to build multibyte UTF8 input char
		
		ofxEditorColorScheme *m_colorScheme; //< optional syntax color scheme
		ofxEditorSyntax *m_syntax; //< optional lang syntax
		bool m_lineWrapping; //< enable line wrapping in this editor?
		bool m_lineNumbers;  //< enable line numbers?
		unsigned int m_lineNumWidth; //< line number block width in chars 
	
		float m_delta;        //< difference from last timestamp
		float m_flash;        //< cursor flash animation time
		bool m_blowupCursor;  //< blow up the cursor?
		float m_blowup;       //< how much the cursor is being blown up

		bool m_flashSelection;      //< is selection flash on?
		float m_flashSelTime;       //< flash animation time
		unsigned int m_flashStart;  //< flash start pos
		unsigned int m_flashEnd;    //< flash end pos
	
		// auto focus
		bool m_autoFocus;       //< enable auto focus scaling?
		float m_scale;          //< scale amount calculated by auto focus
		float m_BBMinX, m_BBMaxX, m_BBMinY, m_BBMaxY; //< current text bounding box
		
	/// \section Syntax Parser Types
		
		/// syntax parser TextBlock types
		enum TextBlockType {
			UNKNOWN,
			WORD,               //< basic text
			STRING_BEGIN,       //< tag only, no text
			STRING_END,         //< tag only, no text
			NUMBER,             //< number including .
			SPACE,              //< whitespace
			TAB,                //< whitespace
			ENDLINE,            //< whitespace
			MATCHING_CHAR,      //< open/close chars in settings aka []{}()<>
			OPERATOR_CHAR,      //< common operator chars aka =+-*/!|&~^
			PUNCTUATION_CHAR,   //< standard punctuation chars aka ;:,?
			COMMENT_BEGIN,      //< tag only, no text
			COMMENT_END,        //< tag only, no text
			LITERAL_BEGIN,      //< tag only, no text
			LITERAL_END,        //< tag only, no text
			PREPROCESSOR_BEGIN, //< tag only, no text
			PREPROCESSOR_END,   //< tag only, no text
		};
		
		/// syntax parser custom class to represent a contextual block of text
		class TextBlock {
			public:
				
				TextBlockType type; //< block type
				std::u32string text; //< block text string
				
				TextBlock() {clear();}
				TextBlock(TextBlockType type) : type(type) {}
				TextBlock(TextBlockType type, u32string text) : type(type), text(text) {}
				
				TextBlock(const TextBlock &from) {
					type = from.type;
					text = from.text;
				}
				
				void clear() {
					type = UNKNOWN;
					text = U"";
				}
		};
		list<TextBlock> m_textBlocks; //< syntax parser text block linked list
	
	/// \section Undo Types
	
		/// undo action types
		enum UndoActionType {
			ACTION_INSERT,    //< text inserted
			ACTION_REPLACE,   //< selection replaced via paste action
			ACTION_OVERWRITE, //< selection replaced via key input
			ACTION_BACKSPACE, //< text deleted to the left
			ACTION_DELETE     //< text deleted to the right
		};
	
		/// undo action state
		class UndoAction {
			public:
			
				UndoActionType type; //< action type
				std::u32string insertText, deleteText; //< any added/removed text
				unsigned int pos; //< text position
				long timestamp; //< timestamp of last update
			
				UndoAction() {clear();}
				
				UndoAction(const UndoAction &from) {
					type = from.type;
					insertText = from.insertText;
					deleteText = from.deleteText;
					pos = from.pos;
					timestamp = from.timestamp;
				}
			
				void clear() {
					type = ACTION_INSERT;
					insertText = U"";
					deleteText = U"";
					pos = 0;
					timestamp = ofGetElapsedTimeMillis();
				}
			
		};
		std::vector<UndoAction> m_undoActions; //< current undo actions
		int m_undoPos; //< current undo position, -1 denotes no undos left
	
	/// \section Helper Functions
	
		/// get the width of a given character,
		/// endlines are 1 space and tabs are depending on the tab width setting
		float characterWidth(int c);
	
		/// draw a matching char highlight char block rectangle at pos
		void drawMatchingCharBlock(int c, int x, int y);
	
		/// draw a selection char block rectangle at pos
		void drawSelectionCharBlock(int c, int x, int y);

		/// flash over char block rectangle at pos
		void drawFlashCharBlock(int c, int x, int y);
	
		/// draw the cursor at pos
		void drawCursor(int x, int y);
	
		/// draw current line number starting at a given pos, padded by digit width of last line number
		void drawLineNumber(int &x, int &y, int &currentLine);
	
		/// replace tabs in buffer with spaces
		void processTabs();
	
		/// get offset in buffer to the current line
		int offsetToCurrentLineStart();
	
		/// get the length of the next line from the current buffer pos
		int nextLineLength(int pos);
	
		/// get the length of the previous line from the current buffer pos
		int previousLineLength(int pos);
	
		/// get the length of the curent line from the current buffer pos
		int lineLength(int pos);
	
		/// get the start of the current line from the current buffer pos
		unsigned int lineStart(int pos);
	
		/// get the end of the current line from the current buffer pos
		unsigned int lineEnd(int pos);
	
		/// find matching open/close char highlight positions base on current buffer pos
		void parseMatchingChars();
	
		/// look forward for a close char
		void parseOpenChars(int pos, int type);
	
		/// look backward for an open char
		void parseCloseChars(int pos, int type);
	
		/// get the number of lines at a buffer pos
		unsigned int lineNumberForPos(unsigned int pos);
	
		/// copy selected text to the system clipboard or copy buffer
		/// note: clipboard only supported when using a GLFW Window
		void copySelection();
	
		/// paste text from the system clipboard or copy buffer
		/// note: clipboard only supported when using a GLFW Window
		void pasteSelection();
	
		/// erase the current highlight selection, updates undo
		void eraseSelection(UndoActionType type=ACTION_DELETE);
	
		/// update the animation timestamps
		/// make sure to call this if you implement your own draw() function
		void updateTimestamps();
	
		/// expand the auto focus bounding box
		void expandBoundingBox(float x, float y);
	
		// clear the autofocus bounding box
		void clearBoundingBox();
	
		/// text buffer changed, so update syntax text blocks and/or other info
		///
		/// TODO: replace this brute force method with a link list pointer based
		/// approach that updates only those text blocks around the current
		/// editor area
		void textBufferUpdated();
	
		/// update visible char size based on pixel size, char size, & auto focus
		void updateVisibleSize();
	
		/// update undo state, creates of modifies actions using timeout
		/// on new input, clears actions newer than current undo pos
		void updateUndo(UndoActionType type, unsigned int pos, const u32string &insertText, const u32string &deleteText);
	
	private:
	
		/// parses text into text blocks
		void parseTextBlocks();
		
		/// clears current text block list
		void clearTextBlocks();
};
