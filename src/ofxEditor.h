#pragma once

#include "ofxEditorColorScheme.h"

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
		static void setTextColor(ofColor &color);
		static ofColor& getTextColor();
		
		/// cursor color, default: yellow w/ alpha
		static void setCursorColor(ofColor &color);
		static ofColor& getCursorColor();
		
		///selection color, default: green w/ alpha
		static void setSelectionColor(ofColor& color);
		static ofColor& getSelectionColor();
		
		/// set useSuper = true if you want to use the Super (Windows key, Mac CMD)
		/// key as the modifier key, default: false for CTRL key
		static void setSuperAsModifier(bool useSuper);
		static bool getSuperAsModifier();
		
	/// \section Main
		
		/// draw the editor, pushes view and applies viewport
		void draw();
		
		/// required for interactive editing, etc
		void keyPressed(int key);
		
		/// set the view port size,
		/// size of text field calculated based on font size
		void setSize(int width, int height);
		
		string getText();
		void setText(const string& text);
		void clearAllText();
		
		void setColorScheme(ofxEditorColorScheme &colorScheme);
		void clearColorScheme();
		ofxEditorColorScheme* getColorScheme();
		
		void setLineWrapping(bool wrap=true);
		bool getLineWrapping();
		
		void blowupCursor();
		
		void setCurrentLine(int line);
		int getCurrentLine();
		
//		int getNumLines();
//		int getNumCharacters();
//		
//		int getCursorPos();
//		int getCursorLine();
//		int getCursorLinePos();
//		
//		void setCursorPos(int pos);
//		void setCursorLinePos(int line, int character);
//		
		void reset();
	
	protected:
	
		/// custom font class to get access to protected functions
		class Font : public ofTrueTypeFont {
			public:
				void drawCharacter(int c, float x, float y) {
					drawCharAsShape(c, x, y);
				}
		};
	
		static ofPtr<Font> s_font;      //< global editor font
		static int s_charWidth;         //< char block pixel width
		static int s_charHeight;        //< char block pixel height
		static int s_cursorWidth;		///< cursor width, 1/3 char width
		
		static unsigned int s_tabWidth; //< tab width in spaces
		static bool s_convertTabs;      //< convert tabs to spaces?
		
		static float s_alpha;            //< overall text alpha
		static ofColor s_textColor;		 //< general text color, overridden by color scheme
		static ofColor s_cursorColor;	 //< text pos cursor color
		static ofColor s_selectionColor; //< char selection background color
		
		static unsigned int s_visibleLines;  //< visible text field height in lines
		static unsigned int s_visibleChars;  //< visible text field width in chars
		
		static bool s_superAsModifier; //< use the super key as modifier?
		
		static string s_copyBuffer; //< shared copy/paste buffer
	
		string m_text; //< string buffer
		
		ofRectangle m_viewport;     //< viewport when drawing editor
		unsigned int m_position;    //< 1D text pos within buffer
		unsigned int m_desiredXPos; //< used to calculate pos based on desired line
		
		int m_numCharsWidth;  //< computed text field char width
		int m_numLinesHeight; //< computed tex field num lines
		
		bool m_selection;
		unsigned int m_highlightStart;
		unsigned int m_highlightEnd;
		
		string m_openChars;
		string m_closeChars;
		unsigned int m_leftTextPosition;
		unsigned int m_topTextPosition;
		unsigned int m_bottomTextPosition;
		unsigned int m_lineCount;
		
		bool m_shiftState;
		
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
			ENDLINE
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
		
		void drawCharBlock(int x, int y);
		void drawCursor(int x, int y);
		
		void processTabs();
		int offsetToCurrentLineStart();
		int nextLineLength(int pos);
		int previousLineLength(int pos);
		int lineLength(int pos);
		unsigned int lineStart(int pos);
		unsigned int lineEnd(int pos);
		
	private:
	
		/// parses text into text blocks
		void parseTextBlocks();
		
		/// clears current text block list
		void clearAllTextBlocks();
};
