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
//		void reset();
	
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
		static unsigned int s_tabWidth; //< tab width in spaces
		static bool s_convertTabs;      //< convert tabs to spaces?
	
		string text; //< string buffer
		
		ofRectangle viewport;     //< viewport when drawing editor
		unsigned int cursorPos;   //< 1D text pos within buffer
		unsigned int desiredXPos; //< used to calculate pos based on desired line
		
		int numCharsWidth;  //< computed text field char width
		int numLinesHeight; //< computed tex field num lines
		
		ofxEditorColorScheme *colorScheme; //< optional syntax color scheme
		bool lineWrapping; //< enable line wrapping in this editor?
		
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
