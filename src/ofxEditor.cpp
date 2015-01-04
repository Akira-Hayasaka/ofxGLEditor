#include "ofxEditor.h"

#if !defined(TARGET_NODISPLAY) && !defined(TARGET_OF_IOS) && \
		!defined(TARGET_ANDROID) && !defined(TARGET_RASPBERRY_PI)
	#include "ofAppGLFWWindow.h"
#endif

#define FLASH_RATE 1
#define HALF_FLASH_RATE (FLASH_RATE*0.5)

// max time in secs for cursor to "blow up"
#define BLOWUP_FLASHES 1.8

// max cursor blow up size in chars
#define CURSOR_MAX_WIDTH 40
#define CURSOR_MAX_HEIGHT 40

ofPtr<ofxEditor::Font> ofxEditor::s_font;
int ofxEditor::s_charWidth = 1;
int ofxEditor::s_charHeight = 1;
int ofxEditor::s_cursorWidth = 1;

string ofxEditor::s_copyBuffer;

// use CMD on OSX, CTRL for Windows & Linux by default
#ifdef __APPLE__
	bool ofxEditor::s_superAsModifier = true;
#else
	bool ofxEditor::s_superAsModifier = false;
#endif

//--------------------------------------------------------------
ofxEditor::ofxEditor() {
	
	// internal settings
	m_settings = new ofxEditorSettings;
	m_sharedSettings = false;
	
	m_position = 0;
	m_desiredXPos = 0;
	
	m_selection = false;
	
	m_leftTextPosition = 0;
	m_topTextPosition = 0;
	m_bottomTextPosition = 0;
	m_displayedLineCount = 0;
	
	m_shiftState = false;
	
	m_colorScheme = NULL;
	m_lineWrapping = false;
	m_lineNumbers = false;
	m_lineNumWidth = 0;
	
	m_time = 0;
	m_delta = 0;
	m_flash = 0;
	m_blowupCursor = false;
	m_blowup = 0;
}

//--------------------------------------------------------------
ofxEditor::ofxEditor(ofxEditorSettings &sharedSettings) {
	
	// shared settings
	m_settings = &sharedSettings;
	m_sharedSettings = true;
	
	m_position = 0;
	m_desiredXPos = 0;
	
	m_selection = false;
	
	m_leftTextPosition = 0;
	m_topTextPosition = 0;
	m_bottomTextPosition = 0;
	m_displayedLineCount = 0;
	
	m_shiftState = false;
	
	m_colorScheme = NULL;
	m_lineWrapping = false;
	m_lineNumbers = false;
	m_lineNumWidth = 0;
	
	m_time = 0;
	m_delta = 0;
	m_flash = 0;
	m_blowupCursor = false;
	m_blowup = 0;
}

//--------------------------------------------------------------
ofxEditor::~ofxEditor() {
	if(!m_sharedSettings) {
		delete m_settings;
	}
}

// STATIC SETTINGS

//--------------------------------------------------------------
bool ofxEditor::loadFont(const string &font, int size) {
	
	string path = ofToDataPath(font);
	if(!ofFile::doesFileExist(path)){
		ofLogFatalError("ofxEditor") << "couldn't find font \"" << font << "\"";
			return false;
	}
	ofLogVerbose("ofxEditor") << "loading font \"" << ofFilePath::getFileName(path) << "\"";
	
	if(s_font == NULL) {
		s_font = ofPtr<ofxEditor::Font>(new ofxEditor::Font());
	}
	if(s_font->loadFont(font, size, true, true, true)) {
		s_charWidth = size;
		s_charHeight = s_font->getLineHeight();
		s_cursorWidth = floor(s_charWidth*0.3);
	}
}

//--------------------------------------------------------------
void ofxEditor::setSuperAsModifier(bool useSuper) {
	s_superAsModifier = useSuper;
}

//--------------------------------------------------------------
bool ofxEditor::getSuperAsModifier() {
	return s_superAsModifier;
}

// MAIN

//--------------------------------------------------------------
void ofxEditor::draw() {

	// default size if not set
	if(m_viewport.width == 0 || m_viewport.height == 0) {
		resize();
	}

	ofPushStyle();
	ofPushView();
	
		ofViewport(m_viewport);
	
		m_matchingCharsHighlight[0] = -1;
		m_matchingCharsHighlight[1] = -1;
		if(m_settings->highlightMatchingChars) {
			parseMatchingChars();
		}
	
		m_displayedLineCount = 0;
		bool drawnCursor = false;
		int x = 0, y = s_charHeight; // pixel pos
		unsigned int textPos = 0;
		unsigned int xcount = 0;
	
		// for line numbers
		int currentLine = 0;

		// draw text
		if(m_colorScheme) { // with colorScheme
			ofFill();
			
			// start with line number
			if(m_lineNumbers) {
				currentLine = lineNumberForPos(m_topTextPosition);
				drawLineNumber(x, y, currentLine);
			}
			
			bool comment = false;
			for(list<TextBlock>::iterator iter = m_textBlocks.begin();
			    iter != m_textBlocks.end() && m_displayedLineCount < m_visibleLines; iter++) {
			
				TextBlock &tb = (*iter);
				
				// burn through text blocks until we get to the first visible line
				if(textPos < m_topTextPosition) {
					if(tb.type == ENDLINE) {
						textPos++;
					}
					else {
						textPos += tb.text.length();
					}
					
					continue;
				}
				
				switch(tb.type) {
				
					case UNKNOWN:
						ofLogWarning("ofxEditor") << "trying to draw UNKNOWN text block, contents: " << tb.text;
						break;
						
					case WORD: {
						if(comment) {
							ofColor &c = m_colorScheme->getCommentColor();
							ofSetColor(c.r, c.g, c.b, c.a * m_settings->alpha);
						}
						else {
							ofColor &c = m_colorScheme->getWordColor(tb.text);
							ofSetColor(c.r, c.g, c.b, c.a * m_settings->alpha);
						}
						break;
					}
						
					case STRING: {
						if(comment) {
							ofColor &c = m_colorScheme->getCommentColor();
							ofSetColor(c.r, c.g, c.b, c.a * m_settings->alpha);
						}
						else {
							ofColor &c = m_colorScheme->getStringColor();
							ofSetColor(c.r, c.g, c.b, c.a * m_settings->alpha);
						}
						break;
					}
						
					case NUMBER: {
						if(comment) {
							ofColor &c = m_colorScheme->getCommentColor();
							ofSetColor(c.r, c.g, c.b, c.a * m_settings->alpha);
						}
						else {
							ofColor &c = m_colorScheme->getNumberColor();
							ofSetColor(c.r, c.g, c.b, c.a * m_settings->alpha);
						}
						break;
					}
					
					case COMMENT_BEGIN:
						comment = true;
						continue; // nothing to draw
						
					case COMMENT_END:
						comment = false;
						continue; // nothing to draw
					
					default:
						break;
				}
				
				// draw block chars
				for(int i = 0; i < tb.text.length(); ++i) {
					
					// line wrap at the block level
					if(m_lineWrapping && xcount >= m_visibleChars) {
						x = 0;
						y += s_charHeight;
						xcount = 0;
						if(m_lineNumbers) { // pad for line numbers
							x += m_lineNumWidth * s_charWidth;
						}
					}
					
					// draw matching chars highlight
					if(!comment && !m_selection && textPos >= m_matchingCharsHighlight[0] && textPos <= m_matchingCharsHighlight[1]) {
						drawMatchingCharBlock(x, y);
					}
					
					// draw selection
					if(m_selection && textPos >= m_highlightStart && textPos < m_highlightEnd) {
						if(tb.type == TAB) {
							for(int i = 0; i < m_settings->tabWidth; ++i) {
								drawSelectionCharBlock(x+s_charWidth*i, y);
							}
						}
						else {
							drawSelectionCharBlock(x, y);
						}
					}
					
					// draw cursor
					if(textPos == m_position) {
						drawCursor(x, y);
						drawnCursor = true;
					}
					
					// draw chars
					switch(tb.type) {
						case ENDLINE:
							x = 0;
							y += s_charHeight;
							xcount = 0;
							textPos++;
							m_displayedLineCount++;
							if(m_lineNumbers) {
								drawLineNumber(x, y, currentLine);
							}
							break;
						case TAB:
							x += s_charWidth * m_settings->tabWidth;
							xcount += m_settings->tabWidth;
							textPos++;
							break;
						default:
							s_font->drawCharacter(tb.text[i], x, y);
							x += s_charWidth;
							xcount++;
							textPos++;
							break;
					}
				}
			}
		}
		else { // without syntax highlighting
			ofFill();
			
			// start with line number
			if(m_lineNumbers) {
				currentLine = lineNumberForPos(m_topTextPosition);
				drawLineNumber(x, y, currentLine);
			}
			
			textPos = m_topTextPosition;
			for(int i = m_topTextPosition; i < m_text.length() && m_displayedLineCount < m_visibleLines; ++i) {
			
				// line wrap
				if(m_lineWrapping) {
					if(xcount >= m_visibleChars) {
						x = 0;
						y += s_charHeight;
						xcount = 0;
						if(m_lineNumbers) { // pad for line numbers
							x += m_lineNumWidth * s_charWidth;
						}
					}
				}
				
				// draw matching chars highlight
				if(!m_selection && textPos >= m_matchingCharsHighlight[0] && textPos <= m_matchingCharsHighlight[1]) {
					drawMatchingCharBlock(x, y);
				}
				
				// draw selection
				if(m_selection && i >= m_highlightStart && i < m_highlightEnd) {
					if(xcount > m_visibleChars) {
						m_leftTextPosition = xcount-m_visibleChars;
					}
					else {
						m_leftTextPosition = 0;
					}
					drawSelectionCharBlock(x, y);
				}
				
				// draw cursor
				if(i == m_position) {
					drawCursor(x, y);
					drawnCursor = true;
				}
			
				// endline
				if(m_text[i] == '\n') {
					x = 0;
					y += s_charHeight;
					textPos++;
					xcount = 0;
					m_displayedLineCount++;
					if(m_lineNumbers) {
						drawLineNumber(x, y, currentLine);
					}
				}
				// tab
				else if(m_text[i] == '\t') {
					
					if(m_selection && i >= m_highlightStart && i < m_highlightEnd) {
						int tabX = x+s_charWidth;
						for(int i = 0; i < m_settings->tabWidth-1; ++i) {
							drawSelectionCharBlock(tabX, y);
							tabX += s_charWidth;
						}
					}
					
					x += s_charWidth * m_settings->tabWidth;
					xcount += m_settings->tabWidth;
					textPos++;
				}
				// everything else
				else {
					ofSetColor(m_settings->textColor.r, m_settings->textColor.g,
						       m_settings->textColor.b, m_settings->textColor.a * m_settings->alpha);
					s_font->drawCharacter(m_text[i], x, y);
					x += s_charWidth;
					xcount++;
					textPos++;
				}
			}
		}
		
		if(m_displayedLineCount >= m_visibleLines-1) {
			m_bottomTextPosition = textPos;
		}
		else {
			m_bottomTextPosition = m_text.size()+1;
		}
		
		// draw cursor if we have no text, or if we're at the end of the buffer
		if(!drawnCursor){
			if(xcount > m_visibleChars) {
				m_leftTextPosition = xcount-m_visibleChars;
			}
			else {
				m_leftTextPosition = 0;
			}
			drawCursor(x, y);
		}
	
	ofPopView();
	ofPopStyle();
	
	// update animation timestamps
	m_delta = ofGetElapsedTimef() - m_time;
	m_time = ofGetElapsedTimef();
	if (m_delta > 100.0f) {
		m_delta = 0.000001f;
	}
}

//--------------------------------------------------------------
void ofxEditor::drawGrid() {
	ofPushStyle();
	ofPushView();
	
		ofViewport(m_viewport);
	
		int x = 1, y = 1;
		ofSetColor(100, 100, 100);
		for(int i = 0; i < m_visibleChars+m_leftTextPosition+1; ++i) {
			ofLine(x, 1, x, m_visibleLines*s_charHeight);
			x += s_charWidth;
		}
		for(int i = 0; i < m_visibleLines+1; ++i) {
			ofLine(1, y, (m_visibleChars+m_leftTextPosition+1)*s_charWidth, y);
			y += s_charHeight;
		}
	
	ofPopView();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxEditor::keyPressed(int key) {

	// filter out modifier key events, except SHIFT
	switch(key) {
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
		case OF_KEY_LEFT_SHIFT: case OF_KEY_RIGHT_SHIFT:
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
		case OF_KEY_SUPER: case OF_KEY_LEFT_COMMAND: case OF_KEY_RIGHT_COMMAND:
			return;
	}
	
	bool modifierPressed = s_superAsModifier ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	if(modifierPressed) {
	
		// check control chars too if CTRL is modifier
		switch(key) {
				
			case 'a': case 10: // select all
				m_selection = true;
				m_highlightStart = 0;
				m_highlightEnd = m_text.size();
				break;
				
			case 'x': case 24: // cut
				if(m_selection) {
					copySelection();
					m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
					if(m_position >= m_highlightEnd) {
						m_position -= m_highlightEnd-m_highlightStart;
					}
				}
				break;
			
			case 'c': case 3: // copy
				copySelection();
				break;
			
			case 'v': case 22: // paste
				pasteSelection();
				break;
			
			case 'b' : // show cursor location
				blowupCursor();
				break;
		}
	}
	else {
		
		// also check for ascii control chars: http://ascii-table.com/control-chars.php
		switch(key) {
			
			case OF_KEY_RIGHT:
				if(!m_text.empty()) {
					m_position++;
				}
				if(!m_lineWrapping) {
					m_desiredXPos = (unsigned int)offsetToCurrentLineStart();
					if(m_desiredXPos >= m_visibleChars) {
						m_leftTextPosition++;
						m_viewport.x -= s_charWidth;
					}
					else {
						m_leftTextPosition = 0;
						m_viewport.x = 0;
					}
				}
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
			
			case OF_KEY_LEFT:
				if(m_position > 0) {
					m_position--;
				}
				if(!m_lineWrapping) {
					m_desiredXPos = (unsigned int)offsetToCurrentLineStart();
					if(m_desiredXPos >= m_visibleChars) {
						if(m_leftTextPosition > 0) {
							m_leftTextPosition--;
							m_viewport.x += s_charWidth;
						}
						else {
							m_leftTextPosition = m_desiredXPos - m_visibleChars+1;
							m_viewport.x = -(int)m_leftTextPosition * s_charWidth;
						}
					}
					else {
						m_leftTextPosition = 0;
						m_viewport.x = 0;
					}
				}
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
			
			case OF_KEY_END:
				m_position = lineEnd(m_position);
				m_desiredXPos = (unsigned int)offsetToCurrentLineStart()+1;
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
				
			case OF_KEY_HOME:
				m_position = lineStart(m_position);
				m_desiredXPos = offsetToCurrentLineStart();
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
					
			case OF_KEY_UP:
				if(lineStart(m_position) > 0) { // if we're not on the first line
					
					int prevLineLen = previousLineLength(m_position);
					if(prevLineLen < m_desiredXPos) {
						m_position = lineStart(m_position)-1; // end of previous
					}
					else {
						m_position = lineStart(lineStart(m_position)-1) + m_desiredXPos; // start of previous+offset
					}
					
					if(m_position < m_topTextPosition) {
						m_topTextPosition = lineStart(m_position);
					}
					m_flash = HALF_FLASH_RATE; // show cursor after moving
				}
				break;
				
			case OF_KEY_DOWN:
				if(lineEnd(m_position) < m_text.size()) { // if we're not on the last line
					
					int nextLineLen = nextLineLength(m_position);
					if(nextLineLen < m_desiredXPos) {
						m_position = lineEnd(lineEnd(m_position)+1); // end of next line
					}
					else {
						m_position = lineStart(lineEnd(m_position)+1) + m_desiredXPos; // start of next+offset
					}
					
					if(m_position >= m_bottomTextPosition) {
						m_topTextPosition = lineEnd(m_topTextPosition)+1;
					}
					m_flash = HALF_FLASH_RATE; // show cursor after moving
				}
				break;
				
			case OF_KEY_PAGE_UP:
				for(unsigned int i = 0; i < m_visibleLines+1; i++) {
					int start = lineStart(m_position);
					if(start > 0) {
						m_position = start-1;
					}
				}
				if(m_position < m_topTextPosition) {
					m_topTextPosition = lineStart(m_position);
				}
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
				
			case OF_KEY_PAGE_DOWN:
				for(unsigned int i = 0; i < m_visibleLines+1; i++) {
					m_position = lineEnd(m_position)+1;
				}
				if(m_position >= m_bottomTextPosition) {
					m_topTextPosition = lineStart(m_position);
				}
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
				
			case OF_KEY_SHIFT:
				if(!m_shiftState) {
					m_highlightStart = m_position;
					m_highlightEnd = m_position;
					m_shiftState = true;
					m_selection = true;
				}
				break;
				
			case OF_KEY_DEL:
				if(m_selection) {
					m_selection = false;
					m_highlightStart = 0;
					m_highlightEnd = 0;
					clearText();
				}
				else {
					m_text.erase(m_position, 1);
					textBufferUpdated();
				}
				break;
				
			case OF_KEY_BACKSPACE:
				if(!m_text.empty()) {
					if(m_selection) {
						m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
						if(m_position >= m_highlightEnd) {
							m_position -= m_highlightEnd-m_highlightStart;
						}						
						m_selection = false;
					}
					else if(m_position != 0) {
						m_text.erase(m_position-1, 1);
						m_position--;
					}
				}
				textBufferUpdated();
				break;
				
			case OF_KEY_TAB:
				if(m_settings->convertTabs) {
					m_text.insert(m_position, "\t");
					m_position++;
				}
				else {
					m_text.insert(m_position, string(m_settings->tabWidth, ' '));
					m_position += m_settings->tabWidth;
				}
				textBufferUpdated();
				break;
				
			case OF_KEY_RETURN:
				key = '\n'; // fallthrough (replacement of newline)
				m_numLines++;
				
			case OF_KEY_ESC:
				m_selection = false;
				m_position = m_highlightStart;
				break;
				
			default:
				
				if(key != '\n' && (key < ' ' || key > 0x80)) { // ignore UTF chars for now ...
					break;
				}
				
				if(m_selection) {
					m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
					if (m_position >= m_highlightEnd) {
						m_position -= m_highlightEnd-m_highlightStart;
					}
					m_selection = false;
				}
				
				m_text.insert(m_position, (const char*)&key);
				m_position++;
				
				if(key == '\n' && m_position >= m_bottomTextPosition && m_displayedLineCount+1 >= m_visibleLines) {
					m_topTextPosition = lineEnd(m_topTextPosition)+1;
				}
				
				textBufferUpdated();
				break;
		}
	}
	
	if(m_position >= m_text.size()) {
		m_position = m_text.size();
	}

	if(m_shiftState) {
	
		if(!ofGetKeyPressed(OF_KEY_SHIFT)) {
			m_shiftState = false;
			m_selection = false;
			return;
		}
		
		if(m_position < m_highlightStart) {
			m_highlightStart = m_position;
		}
		else {
			m_highlightEnd = m_position;
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::resize() {
	int w = (ofGetWindowMode() == OF_WINDOW)?ofGetViewportWidth():ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW)?ofGetViewportHeight():ofGetScreenHeight();
	resize(w, h);
}

//--------------------------------------------------------------
void ofxEditor::resize(int width, int height) {
	m_viewport.width = width;
	m_viewport.height = height;
	
	m_visibleChars = width/s_charWidth;
	m_visibleLines = height/s_charHeight;
	
	ofLogVerbose("ofxEditor") << "pixel size: " << width << " " << height;
	ofLogVerbose("ofxEditor") << "text size: " << m_visibleChars << " " << m_visibleLines;
}

//--------------------------------------------------------------
string ofxEditor::getText() {
	if(m_selection) {
		return m_text.substr(m_highlightStart, m_highlightEnd-m_highlightStart);
	}
	return m_text;
}

//--------------------------------------------------------------
void ofxEditor::setText(const string& text) {
	
	if(m_text != ""){
		m_position = lineStart(m_position);
		int line = getCurrentLine();
		m_text = text;
		setCurrentLine(line);
	}
	else {
		m_text = text;
	}
	
	if(m_settings->convertTabs) {
		processTabs();
	}
	textBufferUpdated();
}

//--------------------------------------------------------------
void ofxEditor::insertText(const string& text) {
	m_text.insert(m_position, text);
	m_selection = false;
	m_position += text.size();
	if(m_settings->convertTabs) {
		processTabs();
	}
	textBufferUpdated();
}

//--------------------------------------------------------------
void ofxEditor::clearText() {
	m_text = "";
	if(m_colorScheme) {
		clearTextBlocks();
	}
	m_position = 0;
	m_numLines = 0;
	setCurrentLine(0);
	m_shiftState = false;
	m_selection = false;
}

// SETTINGS

//--------------------------------------------------------------
ofxEditorSettings& ofxEditor::getSettings() {
	return *m_settings;
}

// COLOR SCHEME

//--------------------------------------------------------------
void ofxEditor::setColorScheme(ofxEditorColorScheme *colorScheme) {
	
	// clear to save memory
	if(colorScheme == NULL) {
		clearTextBlocks();
		m_colorScheme = NULL;
		return;
	}
	
	bool shouldReparse = (m_colorScheme == NULL);
	m_colorScheme = colorScheme;
	
	// reparse if enabling highlighting
	if(shouldReparse) {
		parseTextBlocks();
	}
}

//--------------------------------------------------------------
void ofxEditor::clearColorScheme() {
	m_colorScheme = NULL;
	clearTextBlocks();
}

//--------------------------------------------------------------
ofxEditorColorScheme* ofxEditor::getColorScheme() {
	return m_colorScheme;
}

// DISPLAY SETTINGS

//--------------------------------------------------------------
void ofxEditor::setLineWrapping(bool wrap) {
	m_lineWrapping = wrap;
	if(m_lineWrapping) { // reset x viewport offset
		m_leftTextPosition = 0;
		m_viewport.x = 0;
	}
	else { // recalculate x viewport offset if needed
		m_desiredXPos = (unsigned int)offsetToCurrentLineStart();
		if(m_desiredXPos >= m_visibleChars) {
			m_leftTextPosition = m_desiredXPos-m_visibleChars;
			m_viewport.x -= s_charWidth * (m_desiredXPos-m_visibleChars);
		}
		else {
			m_leftTextPosition = 0;
			m_viewport.x = 0;
		}
	}
}

//--------------------------------------------------------------
bool ofxEditor::getLineWrapping() {
	return m_lineWrapping;
}

//--------------------------------------------------------------
void ofxEditor::setLineNumbers(bool numbers) {
	m_lineNumbers = numbers;
	if(m_lineNumbers) {
		m_lineNumWidth = ofToString(m_numLines).length()+1; // 1 extra for the space
		m_visibleChars -= m_lineNumWidth;
	}
	else {
		m_visibleChars += m_lineNumWidth;
		m_lineNumWidth = 0;
	}
}

//--------------------------------------------------------------
bool ofxEditor::getLineNumbers() {
	return m_lineNumbers;
}

// CURSOR POSITION & INFO

//--------------------------------------------------------------
void ofxEditor::blowupCursor() {
	m_blowupCursor = true;
	m_blowup = 0;
}

//--------------------------------------------------------------
unsigned int ofxEditor::getNumLines() {
	return m_numLines;
}

//--------------------------------------------------------------
unsigned int ofxEditor::getNumCharacters() {
	return m_text.length();
}

//--------------------------------------------------------------
unsigned int ofxEditor::getCurrentPos() {
	return m_position;
}

//--------------------------------------------------------------
void ofxEditor::setCurrentPos(unsigned int pos) {
	if(pos >= m_text.length()) {
		ofLogWarning("ofxEditor") << "ignoring invalid position " << pos;
		return;
	}
	m_position = pos;
}

//--------------------------------------------------------------
unsigned int ofxEditor::getCurrentLine() {
	return lineNumberForPos(m_position);
}

//--------------------------------------------------------------
void ofxEditor::setCurrentLine(unsigned int line) {
	m_position = 0;
	int count = 0;
	for(unsigned int i = 0; i < m_text.size(); i++) {
		if(m_text[i] == '\n') {
			count++;
		}
		if(count <= line) {
			m_position++;
		}
	}
	if(m_position < m_topTextPosition) {
		m_topTextPosition = lineStart(m_position);
	}
	if(m_position >= m_bottomTextPosition) {
		m_topTextPosition = lineEnd(m_topTextPosition)+1;
	}
	m_position = lineStart(m_position);
}

//--------------------------------------------------------------
unsigned int ofxEditor::getCurrentLinePos() {
	return m_position - lineStart(m_position);
}

//--------------------------------------------------------------
unsigned int ofxEditor::getCurrentLineLen() {
	return offsetToCurrentLineStart();
}

//--------------------------------------------------------------
void ofxEditor::setCurrentLinePos(unsigned int line, unsigned int character) {
	setCurrentLine(line);
	m_position += character;
}

//--------------------------------------------------------------
void ofxEditor::reset() {
	m_blowupCursor = false;
	m_blowup = 0.0f;
	m_position = 0;
	setCurrentLine(0);
	m_shiftState = false;
	m_selection = false;
}

// DRAWING UTILS

//--------------------------------------------------------------
void ofxEditor::drawString(const string& s, float x, float y) {
	int xPos = x, yPos = y+s_charHeight;
	for(int i = 0; i < s.size(); ++i) {
		if(s[i] == '\n') {
			xPos = x;
			yPos += s_charHeight;
		}
		else if(s[i] == '\t') {
			xPos += s_charWidth*m_settings->tabWidth;
		}
		else {
			s_font->drawCharacter(s[i], xPos, yPos);
			xPos += s_charWidth;
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::drawString(const string& s, ofPoint& p) {
	return drawString(s, p.x, p.y);
}

//--------------------------------------------------------------
int ofxEditor::getCharWidth() {
	return s_charWidth;
}

//--------------------------------------------------------------
int ofxEditor::getCharHeight() {
	return s_charHeight;
}

// PROTECTED

//--------------------------------------------------------------
void ofxEditor::drawMatchingCharBlock(int x, int y) {
	ofPushStyle();
		ofSetColor(
			m_settings->matchingCharsColor.r, m_settings->matchingCharsColor.g,
			m_settings->matchingCharsColor.b, m_settings->matchingCharsColor.a * m_settings->alpha);
		ofRect(x, y-s_charHeight, s_charWidth, s_charHeight);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxEditor::drawSelectionCharBlock(int x, int y) {
	ofPushStyle();
		ofSetColor(
			m_settings->selectionColor.r, m_settings->selectionColor.g,
			m_settings->selectionColor.b, m_settings->selectionColor.a * m_settings->alpha);
		ofRect(x, y-s_charHeight, s_charWidth, s_charHeight);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxEditor::drawCursor(int x, int y) {
	
	if(m_blowupCursor) {
		
		// set this to zero when starting
		m_blowup += m_delta;
		if (m_blowup >= BLOWUP_FLASHES) {
			m_blowupCursor = false;
		}
		else {
			float maxCW = (BLOWUP_FLASHES - m_blowup) / BLOWUP_FLASHES * (CURSOR_MAX_WIDTH * s_cursorWidth * 0.5f) + s_cursorWidth * 0.5f;
			float maxCH = (BLOWUP_FLASHES - m_blowup) / BLOWUP_FLASHES * (CURSOR_MAX_HEIGHT * s_charHeight) + s_charHeight;
			ofPushStyle();
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofSetColor(m_settings->cursorColor.r, m_settings->cursorColor.g,
					       m_settings->cursorColor.b, m_settings->cursorColor.a * m_settings->alpha * m_blowup/BLOWUP_FLASHES);
				ofRect(MAX(x+maxCW/2, x), MAX(y-s_charHeight+maxCH/2, y-s_charHeight), MAX(maxCW, s_cursorWidth), MAX(maxCH, s_charHeight));
			ofPopStyle();
		}
	}
	else {
		m_flash += m_delta;
		if (m_flash > FLASH_RATE) {
			m_flash = 0;
		}
		if (m_flash > HALF_FLASH_RATE) {
			ofPushStyle();
				ofSetColor(m_settings->cursorColor.r, m_settings->cursorColor.g,
					       m_settings->cursorColor.b, m_settings->cursorColor.a * m_settings->alpha);
				ofRect(x, y-s_charHeight, s_cursorWidth, s_charHeight);
			ofPopStyle();
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::drawLineNumber(int &x, int &y, int &currentLine) {
	ofPushStyle();
		ofSetColor(m_settings->lineNumberColor.r, m_settings->lineNumberColor.g,
			       m_settings->lineNumberColor.b, m_settings->lineNumberColor.a * m_settings->alpha);
		currentLine++;
		string currentLineString = ofToString(currentLine);
		x += s_charWidth*(m_lineNumWidth-currentLineString.length()-1); // 1 for the space
		for(int i = 0; i < currentLineString.length(); ++i) {
			s_font->drawCharacter(currentLineString[i], x, y);
			x += s_charWidth;
		}
		x += s_charWidth; // the space
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxEditor::processTabs() {
	size_t pos = m_text.find("\t", 0);
	while(pos != string::npos) {
		m_text.erase(pos, 1);
		m_text.insert(pos, string(m_settings->tabWidth, ' '));
		pos = m_text.find("\t", pos);
	}
}

//--------------------------------------------------------------
int ofxEditor::offsetToCurrentLineStart() {
	return m_position - lineStart(m_position);
}

//--------------------------------------------------------------
int ofxEditor::nextLineLength(int pos) {
	size_t nextLineStart = m_text.find("\n", pos);
	if(nextLineStart != string::npos) {
		return lineLength(nextLineStart+1);
	}
	return 0;
}

//--------------------------------------------------------------
int ofxEditor::previousLineLength(int pos) {
	size_t prevLineEnd = string::npos;
	if(pos > 0) {
		prevLineEnd = m_text.rfind("\n", pos-1);
	}
	if(prevLineEnd != string::npos) {
		return lineLength(prevLineEnd);
	}
	return 0;
}

//--------------------------------------------------------------
int ofxEditor::lineLength(int pos) {
	int len = lineEnd(pos) - lineStart(pos);
	return len > 0 ? len : 0; // clip line length to zero
}

//--------------------------------------------------------------
unsigned int ofxEditor::lineStart(int pos) {
	size_t start = string::npos;
	if(pos > 0) {
		// take one off if we're over a newline
		if(m_text[pos] == '\n') {
			start = m_text.rfind("\n", pos-1);
		}
		else {
			start = m_text.rfind("\n", pos);
		}
	}
	if(start != string::npos) {
		start++; // move the start off the newline
	}
	else {
		start = 0; // if are on the first line, set the start to 0
	}
	return start;
}

//--------------------------------------------------------------
unsigned int ofxEditor::lineEnd(int pos) {
	if(m_text.empty()) {
		return 0;
	}
	size_t end = m_text.find("\n", pos);
	if(end == string::npos) {
		end = m_text.size()-1;
	}
	return end;
}

//--------------------------------------------------------------
void ofxEditor::parseMatchingChars() {

	// parse the parentheses
	int type = 0;
	for(string::iterator i = m_settings->openChars.begin(); i != m_settings->openChars.end(); ++i) {
		if(m_text[m_position] == *i) {
			parseOpenChars(m_position, type);
		}
		type++;
	}
		
	if(m_position > 0) {
		type = 0;
		for(string::iterator i = m_settings->closeChars.begin(); i != m_settings->closeChars.end(); ++i) {
			if(m_text[m_position-1] == *i) {
				parseCloseChars(m_position-1, type);
			}
			type++;
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::parseOpenChars(int pos, int type) {

	// looking for a close, so search forward
	int stack = 0, start_pos = pos;
	pos++;
	while(stack != -1 && pos < (int)m_text.size()) {
		if(m_text[pos] == m_settings->openChars[type]) {
			stack++;
		}
		if(m_text[pos] == m_settings->closeChars[type]) {
			stack--;
		}
		pos++;
	}
	if(stack == -1) {
		m_matchingCharsHighlight[0] = start_pos;
		m_matchingCharsHighlight[1] = pos-1;
	}
}

//--------------------------------------------------------------
void ofxEditor::parseCloseChars(int pos, int type) {

	// looking for a open, so search backward
	int stack = 0, start_pos = pos;
	pos--;
	while(stack != -1 && pos >= 0) {
		if(m_text[pos] == m_settings->closeChars[type]) {
			stack++;
		}
		if(m_text[pos] == m_settings->openChars[type]) {
			stack--;
		}
		pos--;
	}
	if(stack == -1) {
		m_matchingCharsHighlight[0] = pos+1;
		m_matchingCharsHighlight[1] = start_pos;
	}	
}

//--------------------------------------------------------------
unsigned int ofxEditor::lineNumberForPos(unsigned int pos) {
	int ret = 0;
	for(unsigned int i = 0; i < pos; i++) {
		if(m_text[i] == '\n') {
			ret++;
		}
	}
	return ret;
}

//--------------------------------------------------------------
void ofxEditor::copySelection() {
	
	if(!m_selection) {
		return;
	}

	// use clipboard if available, otherwise use internal copybuffer
	#if !defined(TARGET_NODISPLAY) && !defined(TARGET_OF_IOS) && \
		!defined(TARGET_ANDROID) && !defined(TARGET_RASPBERRY_PI)
		ofAppGLFWWindow *window = (ofAppGLFWWindow *) ofGetWindowPtr();
		glfwSetClipboardString(window->getGLFWWindow(), getText().c_str());
	#else
		s_copyBuffer = m_text.substr(m_highlightStart, m_highlightEnd-m_highlightStart);
	#endif
	
	m_selection = false;
}

//--------------------------------------------------------------
void ofxEditor::pasteSelection() {

	// use clipboard if available, otherwise use internal copybuffer
	#if !defined(TARGET_NODISPLAY) && !defined(TARGET_OF_IOS) && \
		!defined(TARGET_ANDROID) && !defined(TARGET_RASPBERRY_PI)
		ofAppGLFWWindow *window = (ofAppGLFWWindow *) ofGetWindowPtr();
		const char * text = glfwGetClipboardString(window->getGLFWWindow());
		if(!text) {
		 ofLogError("ofxEditor") << "pasting from clipboard failed";
		}
		insertText((string) text);
	#else
		insertText(s_copyBuffer);
	#endif
}

// PRIVATE

//--------------------------------------------------------------
void ofxEditor::textBufferUpdated() {
	
	if(m_colorScheme) {
		parseTextBlocks();
	}
	else {
		// compute number of lines
		m_numLines = count(m_text.begin(), m_text.end(), '\n');
	}
	
	// adjust max screen width for line numbers
	if(m_lineNumbers) {
		m_visibleChars += m_lineNumWidth; // back to normal
		m_lineNumWidth = ofToString(m_numLines).length()+1; // 1 extra for the space
		m_visibleChars -= m_lineNumWidth;
	}

	// scroll if we've added content at the far right
	if(!m_lineWrapping) {
		m_desiredXPos = (unsigned int)offsetToCurrentLineStart();
		if(m_desiredXPos >= m_visibleChars) {
			m_leftTextPosition = m_desiredXPos-m_visibleChars;
			m_viewport.x = -(int)m_leftTextPosition * s_charWidth;
		}
		else {
			m_leftTextPosition = 0;
			m_viewport.x = 0;
		}
	}
}

//--------------------------------------------------------------
// simple syntax parser
void ofxEditor::parseTextBlocks() {
	
	clearTextBlocks();
	m_numLines = 0;
	
	bool singleComment = false;
	bool multiComment = false;
	string singleLineComment = m_colorScheme->getSingleLineComment();
	string multiLineCommentBegin = m_colorScheme->getMultiLineCommentBegin();
	string multiLineCommentEnd = m_colorScheme->getMultiLineCommentEnd();
	
	TextBlock tb;
	for(int i = 0; i < m_text.length(); ++i) {
		
		switch(m_text[i]) {
		
			case ' ':
				if(tb.type != UNKNOWN) {
					// catch spaces inside STRINGS
					if(tb.type == STRING) {
						tb.text += m_text[i];
						break;
					}
					else {
						m_textBlocks.push_back(tb);
						tb.clear();
					}
				}
				tb.type = SPACE;
				tb.text = m_text[i];
				m_textBlocks.push_back(tb);
				tb.clear();
				break;
		
			case '\n':
				m_numLines++; // compute number of lines while parsing
				if(tb.type != UNKNOWN) {
					m_textBlocks.push_back(tb);
					tb.clear();
				}
				if(singleComment) {
					TextBlock commentBlock;
					commentBlock.type = COMMENT_END;
					m_textBlocks.push_back(commentBlock);
					singleComment = false;
				}
				tb.type = ENDLINE;
				tb.text = m_text[i];
				m_textBlocks.push_back(tb);
				tb.clear();
				break;
				
			case '\t':
				if(tb.type != UNKNOWN) {
					m_textBlocks.push_back(tb);
					tb.clear();
				}
				tb.type = TAB;
				tb.text = m_text[i];
				m_textBlocks.push_back(tb);
				tb.clear();
				break;
				
			case '"': case '\'':
				if(tb.type == STRING) {
					tb.text += m_text[i];
					m_textBlocks.push_back(tb);
					tb.clear();
					break;
				}
				else if(tb.type != UNKNOWN) {
					m_textBlocks.push_back(tb);
					tb.clear();
				}
				tb.type = STRING;
				tb.text += m_text[i];
				break;
				
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				if(tb.type != UNKNOWN) {
					if(tb.type == STRING) {
						tb.text += m_text[i];
						break;
					}
					else if(tb.type == WORD) {
						// detect words after punctuation aka (, [, etc
						if(i > 0 && ispunct(m_text[i-1]) ) {
							m_textBlocks.push_back(tb);
							tb.clear();
						}
					}
					else if(tb.type != NUMBER) {
						m_textBlocks.push_back(tb);
						tb.clear();
					}
				}
				if(tb.type != WORD) {
					tb.type = NUMBER;
				}
				tb.text += m_text[i];
				break;
		
			case '.': // could be number decimal point
				if(tb.type == UNKNOWN) {
					tb.type = WORD;
				}
				tb.text += m_text[i];
				break;
		
			default: // everything else
				switch(tb.type) {
					case NUMBER:
						m_textBlocks.push_back(tb);
						tb.clear();
					case UNKNOWN:
						tb.type = WORD;
					case WORD: case STRING:
						tb.text += m_text[i];
					default:
						if(tb.type == WORD) {
							
							// detect comments
							if(!multiComment) {
								
								// check beginning of word for multi line comment begin
								if(tb.text.length() >= multiLineCommentBegin.length() &&
								   tb.text.substr(0, multiLineCommentBegin.length()) == multiLineCommentBegin) {
									
									if(singleComment) {
										// already pushed comment begin
										singleComment = false;
									}
									else {
										// push comment begin
										TextBlock commentBlock;
										commentBlock.type = COMMENT_BEGIN;
										m_textBlocks.push_back(commentBlock);
									}
									
									multiComment = true;
									continue;
								}
								
								// check beginning of word for single line comment
								else if(!singleComment && tb.text.length() >= singleLineComment.length()) {
									if(tb.text.substr(0, singleLineComment.length()) == singleLineComment) {
										
										// push comment begin
										TextBlock commentBlock;
										commentBlock.type = COMMENT_BEGIN;
										m_textBlocks.push_back(commentBlock);
										
										singleComment = true;
										continue;
									}
								}
							}
							else { // check end of word for multi line comment end
								if(tb.text.length() >= multiLineCommentEnd.length() &&
								   tb.text.substr(tb.text.length()-multiLineCommentEnd.length(),
								                  multiLineCommentEnd.length()) == multiLineCommentEnd) {
									
									// push latest block
									m_textBlocks.push_back(tb);
									tb.clear();
									
									// push comment end
									TextBlock commentBlock;
									commentBlock.type = COMMENT_END;
									m_textBlocks.push_back(commentBlock);
									
									multiComment = false;
									continue;
								}
							}
						}
						break;
				}
				break;
		}
	}
	
	// catch any unfinished blocks at the end
	if(tb.type != UNKNOWN) {
		m_textBlocks.push_back(tb);
	}
	
	// catch any unfinished comments, unfinished multiline comments are a
	// syntax error so don't close them
	if(singleComment) {
		TextBlock commentBlock;
		commentBlock.type = COMMENT_END;
		m_textBlocks.push_back(commentBlock);
	}
}

//--------------------------------------------------------------
void ofxEditor::clearTextBlocks() {
	m_textBlocks.clear();
}
