#pragma once

#include "ofxEditorColorScheme.h"

/// basic full screen text editor with optional syntax highlighting,
/// based on the Fluxus GLEditor
///
/// TODO: support UTF-8 characters
///
class ofxEditor {

	public:
	
		ofxEditor();
		virtual ~ofxEditor();
		
	/// \section Static Settings
		
		/// load font to be used by all editors, *must* be a fixed width font
		///
		/// call this before drawing any editor
		///
		static bool loadFont(const string & font, int size);
		
		/// set the displayed tab width, default: 4
		static void setTabWidth(unsigned int numSpaces);
		static unsigned int getTabWidth();
		
		/// convert tabs to spaces? performed when setting or editing text and
		/// *cannot* be undone
		static void setConvertTabsToSpaces(bool convert=true);
		static bool getConvertTabsToSpaces();
		
		/// set overall text alpha
		static void setAlpha(float alpha);
		static float getAlpha();
		
		/// text color color, default: white
		/// overridden by color scheme if a scheme is set
		static void setTextColor(ofColor color);
		static ofColor& getTextColor();
		
		/// cursor color, default: yellow w/ alpha
		static void setCursorColor(ofColor color);
		static ofColor& getCursorColor();
		
		/// selection color, default: green w/ alpha
		static void setSelectionColor(ofColor color);
		static ofColor& getSelectionColor();
	
		/// matching chars highlight color, default: blue w/ alpha
		static void setMatchingCharsColor(ofColor color);
		static ofColor& getMatchingCharsColor();
		
		/// set useSuper = true if you want to use the Super (Windows key, Mac CMD)
		/// key as the modifier key, default: false for CTRL key
		static void setSuperAsModifier(bool useSuper);
		static bool getSuperAsModifier();
	
		/// highlight matching open/close chars?, default: true
		static void setHighlightMatchingChars(bool highlight);
		static bool getHighlightMatchingChars();
	
		/// set the matching open/close chars to highlight,
		/// default: "([<{" & ")]>}", strings should not be empty
		/// note: matching chars are not highlighted inside comments
		static void setMatchingChars(string openChars, string closeChars);
		static string getOpenChars();
		static string getCloseChars();
	
	/// \section Main
		
		/// draw the editor, pushes view and applies viewport
		void draw();
	
		/// draw the text field character grid
		void drawGrid();
		
		/// required for interactive editing, etc
		void keyPressed(int key);
		
		/// set the view port size,
		/// size of text field calculated based on font size
		void setSize(int width, int height);
	
		/// get text buffer contents or selection
		string getText();
	
		/// set text buffer contents
		void setText(const string& text);
	
		/// clear text buffer contents
		void clearAllText();
	
	/// \section Color Scheme
	
		/// set color scheme for this editor and highlight syntax
		/// note: pointer is never deleted
		void setColorScheme(ofxEditorColorScheme *colorScheme);
	
		/// clear the current color scheme
		void clearColorScheme();
	
		/// get the current color scheme, returns NULL if not set
		ofxEditorColorScheme* getColorScheme();
	
	/// \section Settings
	
		/// enable/disable line wrapping
		void setLineWrapping(bool wrap=true);
	
		/// get line wrapping value
		bool getLineWrapping();
	
	/// \section Current Position & Info
	
		/// animate the cursor so it's easy to find
		void blowupCursor();
	
		/// get the total number of lines in the text buffer
		unsigned int getNumLines();
	
		/// get the total number of characters in the text buffer
		unsigned int getNumCharacters();
	
		/// get the 1D cursor position in the text buffer
		unsigned int getCurrentPos();
	
		/// set the 1D cursor position in the text buffer
		void setCurrentPos(unsigned int pos);
	
		/// get the line the cursor is on
		unsigned int getCurrentLine();
	
		/// set the current line for the cursor
		void setCurrentLine(unsigned int line);
	
		/// get the current line character position of the cursor
		unsigned int getCurrentLinePos();
	
		/// get the cursor position by line and line character
		void setCurrentLinePos(unsigned int line, unsigned int character);
	
		/// reset position & selection
		void reset();
	
	protected:
	
		/// custom font class to get access to protected functions
		class Font : public ofTrueTypeFont {
			public:
				void drawCharacter(int c, float x, float y) {
					drawCharAsShape(c, x, y);
				}
		};
	
	/// \section Static Variables
	
		static ofPtr<Font> s_font;       //< global editor font
		static int s_charWidth;          //< char block pixel width
		static int s_charHeight;         //< char block pixel height
		static int s_cursorWidth;		 //< cursor width, 1/3 char width
		
		static unsigned int s_tabWidth;  //< tab width in spaces
		static bool s_convertTabs;       //< convert tabs to spaces?
		
		static float s_alpha;            //< overall text alpha
		static ofColor s_textColor;		 //< general text color, overridden by color scheme
		static ofColor s_cursorColor;	 //< text pos cursor color
		static ofColor s_selectionColor; //< char selection background color
		static ofColor s_matchingCharsColor; //< matching chars background color
	
		static bool s_superAsModifier;   //< use the super key as modifier?
		
		static string s_copyBuffer;      //< shared copy/paste buffer
	
		static bool s_highlightMatchingChars; //< highlight matching open/close chars?
		static string s_openChars;       //< open chars (parens, brackets, etc) for matching highlight
		static string s_closeChars;      //< close chars (parens, bracket, etc) for matching highlight
	
	/// \section Member Variables
	
		string m_text; //< text buffer
		unsigned int m_numLines; //< number of lines in the text buffer
		
		ofRectangle m_viewport;     //< viewport when drawing editor
		unsigned int m_position;    //< 1D text pos within buffer
		unsigned int m_desiredXPos; //< desired char pos on current line
		
		int m_visibleChars; //< computed text field char width
		int m_visibleLines; //< computed text field num lines
	
		int m_matchingCharsHighlight[2]; //< start & end pos for matching chars highlight
	
		bool m_selection; //< is text being selected (shift+arrows)
		unsigned int m_highlightStart; //< highlight start pos in buffer
		unsigned int m_highlightEnd;   //< highlight end pos in buffer
	
		unsigned int m_leftTextPosition;   //< left start char pos for horz scrolling
		unsigned int m_topTextPosition;    //< top start pos in buffer for vert scrolling
		unsigned int m_bottomTextPosition; //< bottom end pos in buffer for vert scrolling
		unsigned int m_displayedLineCount; //< current number of displayed lines (may be diff from m_visibleLines)
	
		bool m_shiftState; //< is shift pressed?
		
		ofxEditorColorScheme *m_colorScheme; //< optional syntax color scheme
		bool m_lineWrapping; //< enable line wrapping in this editor?
		
		float m_time;         //< timestamp for calculating animations
		float m_delta;        //< difference from last timestamp
		float m_flash;        //< cursor flash animation time
		bool m_blowupCursor;  //< blow up the cursor?
		float m_blowup;       //< how much the cursor is being blown up
		
	/// \section Syntax Parser Types
		
		/// syntax parser TextBlock types
		enum TextBlockType {
			UNKNOWN,
			WORD,
			STRING,
			NUMBER,
			SPACE,
			TAB,
			ENDLINE,
			COMMENT_BEGIN, // tag only, no text
			COMMENT_END    // tag only, no text
		};
		
		/// syntax parser custom class to represent a contextual block of text
		class TextBlock {
			public:
				
				TextBlockType type; ///< block type
				string text; ///< block text string
				
				TextBlock() {
					clear();
				}
				
				TextBlock(const TextBlock &from) {
					type = from.type;
					text = from.text;
				}
				
				void clear() {
					type = UNKNOWN;
					text = "";
				}
		};
		list<TextBlock> m_textBlocks; //< syntax parser text block linked list
		
	/// \section Helper Functions
	
		// draw a matching char hilight char block rectanlge at pos
		void drawMatchingCharBlock(int x, int y);
	
		/// draw a selection char block rectangle at pos
		void drawSelectionCharBlock(int x, int y);
	
		/// draw the cursor at pos
		void drawCursor(int x, int y);
	
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
		
	private:
	
		/// text buffer changed, so update syntax text blocks and/or other info
		void textBufferUpdated();
	
		/// parses text into text blocks
		void parseTextBlocks();
		
		/// clears current text block list
		void clearAllTextBlocks();
};
