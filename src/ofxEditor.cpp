#include "ofxEditor.h"

ofPtr<ofxEditor::Font> ofxEditor::s_font;
int ofxEditor::s_charWidth = 1;
int ofxEditor::s_charHeight = 1;
unsigned int ofxEditor::s_tabWidth = 4;

ofxEditor::ofxEditor() {
	cursorPos = 2;
	viewport.position.set(0, 0, 0);
	setSize(ofGetWidth(), ofGetHeight());
	
	colorScheme = NULL;
	lineWrapping = false;
}

ofxEditor::~ofxEditor() {
}

bool ofxEditor::loadFont(const string &font, int size) {
	if(s_font == NULL) {
		s_font = ofPtr<ofxEditor::Font>(new ofxEditor::Font());
	}
	if(s_font->loadFont(font, size, true, true, true)) {
		s_charWidth = size;
		s_charHeight = s_font->getLineHeight();
	}
}

void ofxEditor::setTabWidth(unsigned int numSpaces) {
	s_tabWidth = numSpaces < 1 ? 1 : numSpaces;
}

unsigned int ofxEditor::getTabWidth() {
	return s_tabWidth;
}

void ofxEditor::draw() {
	ofPushStyle();
	ofPushView();
	
		ofViewport(viewport);

		// draw text
		if(colorScheme) { // with colorScheme
			ofFill();
			int x = 0, y = s_charHeight; // pixel pos
			int textPos = 0;//, textLine = 0; // char/line pos
			for(list<TextBlock>::iterator iter = textList.begin(); iter != textList.end(); iter++) {
			
				TextBlock &tb = (*iter);
				
				// line wrap
				//if(lineWrapping && textPos+tb.text.length() >= numCharsWidth) {
				if(lineWrapping && textPos >= numCharsWidth) {
					x = 0;
					y += s_charHeight;
				}
				
				switch(tb.type) {
				
					case UNKNOWN:
						ofLogWarning("ofxEditor") << "trying to draw UNKNOWN text block";
						return;
						
					case WORD:
						ofSetColor(colorScheme->getWordColor(tb.text));
						break;
						
					case STRING:
						ofSetColor(colorScheme->getStringColor());
						break;
						
					case NUMBER:
						ofSetColor(colorScheme->getNumberColor());
						break;
					
					default:
						break;
				}
				
				// draw block chars
				for(int i = 0; i < tb.text.length(); ++i) {
					
					// cursor
					if(textPos == cursorPos) {
						ofPushStyle();
							ofSetColor(255*0.7, 255*0.7, 0);
							ofRect(x, y-s_charHeight, s_charWidth, s_charHeight);
						ofPopStyle();
					}
					
					// draw chars
					switch(tb.type) {
						case ENDLINE:
							x = 0;
							y += s_charHeight;
							textPos++;
							break;
						case TAB:
							x += s_charWidth * s_tabWidth;
							textPos++;
							break;
						default:
							s_font->drawCharacter(tb.text[i], x, y);
							x += s_charWidth;
							textPos++;
							break;
					}
				}
			}
		}
		else { // without highlighter
			ofFill();
			int x = 0, y = s_charHeight; // pixel pos
			int textPos = 0, textLine = 0; // char/line pos
			for(int i = 0; i < text.length(); ++i) {
			
				// line wrap
				if(lineWrapping && textPos >= numCharsWidth) {
					x = 0;
					y += s_charHeight;
					textPos = 0;
					textLine++;
				}
				
				if(i == cursorPos) {
					ofSetColor(255*0.7, 255*0.7, 0);
					ofRect(x, y-s_charHeight, s_charWidth, s_charHeight);
				}
			
				// endline
				if(text[i] == '\n') {
					x = 0;
					y += s_charHeight;
					textPos = 0;
					textLine++;
				}
				// tab
				else if(text[i] == '\t') {
					x += s_charWidth * s_tabWidth;
					textPos += s_tabWidth;
				}
				// everything else
				else {
					ofSetColor(255);
					s_font->drawCharacter(text[i], x, y);
					x += s_charWidth;
					textPos++;
				}
			}
		}
			
//		// grid
//		ofSetColor(100, 100, 100);
//		for(int x = 0; x < ofGetWidth(); x = x+s_charWidth) {
//			ofLine(x, 0, x, ofGetHeight());
//		}
//		for(int y = s_charHeight; y < ofGetHeight(); y = y+s_charHeight) {
//			ofLine(0, y, ofGetWidth(), y);
//		}
		
	ofPopView();
	ofPopStyle();
}

void ofxEditor::keyPressed(int key) {
	switch(key) {
		case OF_KEY_UP:
			//viewport.y -= s_charHeight;
			//cursorPos -= 1;
			break;
		case OF_KEY_DOWN:
			//viewport.y += s_charHeight;
			//cursorLine += 1;
			break;
		case OF_KEY_LEFT:
			//viewport.x -= s_charWidth;
			cursorPos -= 1;
			if(cursorPos < 0) cursorPos = 0;
			break;
		case OF_KEY_RIGHT:
			//viewport.x += s_charWidth;
			cursorPos += 1;
			if(cursorPos >= text.length()) cursorPos = text.length()-1;
			break;
	}
}

void ofxEditor::setSize(int width, int height) {
	viewport.width = width;
	viewport.height = height;
	
	numCharsWidth = width/s_charWidth;
	numLinesHeight = height/s_charHeight;
	
	ofLogVerbose("ofxEditor") << "pixel size: " << width << " " << height;
	ofLogVerbose("ofxEditor") << "text size: " << numCharsWidth << " " << numLinesHeight;
}

string ofxEditor::getText() {
	return text;
}

void ofxEditor::setText(const string& text) {
	this->text = text;
	if(colorScheme) {
		parseTextToList();
	}
}

void ofxEditor::clearText() {
	text = "";
	if(colorScheme) {
		clearList();
	}
}

void ofxEditor::setColorScheme(ofxEditorColorScheme &colorScheme) {
	
	// clear to save memory
	if(&colorScheme == NULL) {
		clearList();
		return;
	}
	
	bool shouldReparse = (this->colorScheme == NULL);
	this->colorScheme = &colorScheme;
	
	// reparse if enabling highlighting
	if(shouldReparse) {
		parseTextToList();
	}
}

void ofxEditor::clearColorScheme() {
	this->colorScheme = NULL;
	clearList();
}

ofxEditorColorScheme* ofxEditor::getColorScheme() {
	return colorScheme;
}

void ofxEditor::setLineWrapping(bool wrap) {
	lineWrapping = wrap;
}

bool ofxEditor::getLineWrapping() {
	return lineWrapping;
}

// PRIVATE

void ofxEditor::parseTextToList() {
	
	clearList();
	
	TextBlock tb;
	for(int i = 0; i < text.length(); ++i) {
		
		// space
		switch(text[i]) {
		
			case ' ':
				if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = SPACE;
				tb.text = text[i];
				textList.push_back(tb);
				tb.clear();
				break;
		
			case '\n':
				if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = ENDLINE;
				tb.text = text[i];
				textList.push_back(tb);
				tb.clear();
				break;
				
			case '\t':
				if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = TAB;
				tb.text = text[i];
				textList.push_back(tb);
				tb.clear();
				break;
				
			case '"': case '\'':
				if(tb.type == STRING) {
					tb.text += text[i];
					textList.push_back(tb);
					tb.clear();
					break;
				}
				else if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = STRING;
				tb.text += text[i];
				break;
				
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				if(tb.type != UNKNOWN) {
					if(tb.type == STRING) {
						tb.text += text[i];
						break;
					}
					else if(tb.type == WORD) {
						// detect words after punctuation aka (, [, etc
						if(i > 0 && ispunct(text[i-1]) ) {
							textList.push_back(tb);
							tb.clear();
						}
					}
					else if(tb.type != NUMBER) {
						textList.push_back(tb);
						tb.clear();
					}
				}
				if(tb.type != WORD) {
					tb.type = NUMBER;
				}
				tb.text += text[i];
				break;
		
			case '.': // could be number decimal point
				tb.text += text[i];
				break;
		
			default: // everything else
				switch(tb.type) {
					case NUMBER:
						textList.push_back(tb);
						tb.clear();
					case UNKNOWN:
						tb.type = WORD;
					case WORD: case STRING:
						tb.text += text[i];
						break;
					default:
						break;
				}
				break;
		}
	}
}

void ofxEditor::clearList() {
	textList.clear();
}
