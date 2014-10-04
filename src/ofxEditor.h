#pragma once

#include "ofxEditorColorScheme.h"

class ofxEditor {

	public:
	
		ofxEditor();
		virtual ~ofxEditor();
		
		static bool loadFont(const string & font, int size);
		static void setTabWidth(unsigned int numSpaces);
		static unsigned int getTabWidth();
		
		void draw();
		void keyPressed(int key);
		
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
	
		// custom font class to get access to protected functions
		class Font : public ofTrueTypeFont {
			public:
				void drawCharacter(int c, float x, float y) {
					drawCharAsShape(c, x, y);
				}
		};
	
		static ofPtr<Font> s_font;
		static int s_charWidth;
		static int s_charHeight;
		static unsigned int s_tabWidth; // width in spaces
	
		string text;
		
		enum Type {
			UNKNOWN,
			WORD,
			STRING,
			NUMBER,
			
			SPACE,
			TAB,
			ENDLINE
		};
		class TextBlock {
			public:
				
				Type type;
				string text;
				
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
		list<TextBlock> textList; // linked list
		
		ofRectangle viewport;
		int cursorPos, cursorLine; // text pos
		
		int numCharsWidth;
		int numLinesHeight;
		
		ofxEditorColorScheme *colorScheme;
		bool lineWrapping;
		
	private:
	
		void parseTextToList();
		void clearList();
};
