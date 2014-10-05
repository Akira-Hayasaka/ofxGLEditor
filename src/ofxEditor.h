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
		static void setTextColor(ofColor &color);
		static ofColor& getTextColor();
		
		/// cursor color, default: yellow
		static void setCursorColor(ofColor &color);
		static ofColor& getCursorColor();
		
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
		void clearText();
		
		void setColorScheme(ofxEditorColorScheme &colorScheme);
		void clearColorScheme();
		ofxEditorColorScheme* getColorScheme();
		
		void setLineWrapping(bool wrap=true);
		bool getLineWrapping();
		
		void blowupCursor();
		
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
		
		static float s_alpha;           //< overall text alpha
		static ofColor s_textColor;		//< general text color, overridden by color scheme
		static ofColor s_cursorColor;	//< color of the text pos cursor
	
		string text; //< string buffer
		
		ofRectangle viewport;     //< viewport when drawing editor
		unsigned int cursorPos;   //< 1D text pos within buffer
		unsigned int desiredXPos; //< used to calculate pos based on desired line
		
		int numCharsWidth;  //< computed text field char width
		int numLinesHeight; //< computed tex field num lines
		
		ofxEditorColorScheme *colorScheme; //< optional syntax color scheme
		bool lineWrapping; //< enable line wrapping in this editor?
		
		float m_time; //< timestamp for calculating animations
		float m_delta; //< difference from last timestamp
		float m_cursorFlash; //< cursor flash animation time
		bool m_blowupCursor; //< blow up the cursor?
		float m_blowup; //< how much the cursor is being blown up
		
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
		list<TextBlock> textList; //< syntax parser text block linked list
		
	/// \section Helper Functions
		
		void drawCursor(int x, int y);
		
		void processTabs();
		int offsetToCurrentLineStart();
		int nextLineLength(int pos);
		int previousLineLength(int pos);
		int lineLength(int pos);
		unsigned int lineStart(int pos);
		unsigned int lineEnd(int pos);
		
		unsigned int countTabs(int startPos, int endPos);
		
	private:
	
		/// parses text into text blocks
		void parseTextToList();
		
		/// clears current text block list
		void clearList();
};
