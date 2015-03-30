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
 *
 * A rewrite of the Fluxus GLEditor http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#include "ofxEditor.h"

// string conversion, this will be replaced when OF has internal unicode support
#include "Unicode.h"
#include "ofxEditorFont.h"

// GLFW needed for clipboard support
#if !defined(TARGET_NODISPLAY) && !defined(TARGET_OF_IOS) && \
    !defined(TARGET_ANDROID) && !defined(TARGET_RASPBERRY_PI)
	#define HAS_GLFW
    #include "ofAppGLFWWindow.h"
#endif

#define FLASH_RATE 1
#define HALF_FLASH_RATE (FLASH_RATE*0.5)

// max time in secs for cursor to "blow up"
#define BLOWUP_FLASHES 1.8

// max cursor blow up size in chars
#define CURSOR_MAX_WIDTH 40
#define CURSOR_MAX_HEIGHT 40

// uncomment to see the viewport and auto focus bounding boxes
//#define DEBUG_AUTO_FOCUS

ofPtr<ofxEditorFont> ofxEditor::s_font;
int ofxEditor::s_charWidth = 1;
int ofxEditor::s_zeroWidth = 1;
int ofxEditor::s_charHeight = 1;
int ofxEditor::s_cursorWidth = 1;
bool ofxEditor::s_textShadow = true;

wstring ofxEditor::s_copyBuffer;

float ofxEditor::s_time = 0;

float ofxEditor::s_autoFocusError = 10;
float ofxEditor::s_autoFocusScaleDrift = 0.3;
float ofxEditor::s_autoFocusMinScale = 0.5;
float ofxEditor::s_autoFocusMaxScale = 5.0;

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
	
	m_numLines = 0;
	m_width = m_height = 0;
	m_position = 0;
	m_desiredXPos = 0;
	m_visibleWidth = 0;
	
	m_selection = NONE;
	m_highlightStart = 0;
	m_highlightEnd = 0;
	m_selectAllStartPos = 0;
	
	m_topTextPosition = 0;
	m_bottomTextPosition = 0;
	m_displayedLineCount = 0;
	
	m_shiftState = false;
	m_UTF8Bytes = 0;
	m_UTF8Char = "";
	
	m_colorScheme = NULL;
	m_lineWrapping = false;
	m_lineNumbers = false;
	m_lineNumWidth = 0;
	
	m_delta = 0;
	m_flash = 0;
	m_blowupCursor = false;
	m_blowup = 0;
	
	m_autoFocus = false;
	m_posX = m_posY = 0;
	m_scale = 1.0;
	m_BBMinX = 0; m_BBMaxX = 0;
	m_BBMinY = 0; m_BBMaxY = 0;
}

//--------------------------------------------------------------
ofxEditor::ofxEditor(ofxEditorSettings &sharedSettings) {
	
	// shared settings
	m_settings = &sharedSettings;
	m_sharedSettings = true;
	
	m_numLines = 0;
	m_width = m_height = 0;
	m_position = 0;
	m_desiredXPos = 0;
	m_visibleWidth = 0;
	
	m_selection = NONE;
	m_highlightStart = 0;
	m_highlightEnd = 0;
	m_selectAllStartPos = 0;
	
	m_topTextPosition = 0;
	m_bottomTextPosition = 0;
	m_displayedLineCount = 0;
	
	m_shiftState = false;
	m_UTF8Bytes = 0;
	m_UTF8Char = "";
	
	m_colorScheme = NULL;
	m_lineWrapping = false;
	m_lineNumbers = false;
	m_lineNumWidth = 0;
	
	m_delta = 0;
	m_flash = 0;
	m_blowupCursor = false;
	m_blowup = 0;
	
	m_autoFocus = false;
	m_posX = m_posY = 0;
	m_scale = 1.0;
	m_BBMinX = 0; m_BBMaxX = 0;
	m_BBMinY = 0; m_BBMaxY = 0;
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
	if(!ofFile::doesFileExist(path)) {
		ofLogFatalError("ofxEditor") << "couldn't find font \"" << font << "\"";
			return false;
	}
	ofLogVerbose("ofxEditor") << "loading font \"" << ofFilePath::getFileName(path) << "\"";
	
	if(s_font == NULL) {
		s_font = ofPtr<ofxEditorFont>(new ofxEditorFont());
	}
	if(s_font->loadFont(font, size)) {
		s_charWidth = s_font->characterWidth(' ');
		s_zeroWidth = s_font->characterWidth('0');
		s_charHeight = s_font->stringHeight("#Iqg"); // catch tall/chars which may hang down
		s_cursorWidth = MAX(floor(s_charWidth*0.3), 6);
		s_autoFocusError = MAX(floor(s_charHeight*0.5), 12); // make sure the error space is proportional to the glyph size
	}
}

//--------------------------------------------------------------
bool ofxEditor::isFontLoaded() {
	return s_font.get();
}

//--------------------------------------------------------------
int ofxEditor::getCharWidth() {
	return s_charWidth;
}

//--------------------------------------------------------------
int ofxEditor::getCharHeight() {
	return s_charHeight;
}

//--------------------------------------------------------------
void ofxEditor::setTextShadow(bool shadow) {
	s_textShadow = shadow;
}

//--------------------------------------------------------------
bool ofxEditor::getTextShadow() {
	return s_textShadow;
}

//--------------------------------------------------------------
void ofxEditor::setSuperAsModifier(bool useSuper) {
	s_superAsModifier = useSuper;
}

//--------------------------------------------------------------
bool ofxEditor::getSuperAsModifier() {
	return s_superAsModifier;
}

//--------------------------------------------------------------
void ofxEditor::setAutoFocusMinScale(float min) {
	s_autoFocusMinScale = min;
}
	
//--------------------------------------------------------------
float ofxEditor::getAutoFocusMinScale() {
	return s_autoFocusMinScale;
}

//--------------------------------------------------------------
void ofxEditor::setAutoFocusMaxScale(float max) {
	s_autoFocusMaxScale = max;
}

//--------------------------------------------------------------
float ofxEditor::getAutoFocusMaxScale() {
	return s_autoFocusMaxScale;
}

// MAIN

//--------------------------------------------------------------
// TODO: check for some easy performance improvements here
void ofxEditor::draw() {

	// default size if not set
	if(m_width == 0 || m_height == 0) {
		resize();
	}
	
	m_visibleWidth = (m_width - s_charWidth) * (m_autoFocus ? 1/m_scale : 1.0);
	m_visibleLines = m_height/(s_charHeight * (m_autoFocus ? s_autoFocusMinScale : 1.0));
	
	// update scrolling
	m_posX = 0;
	if(!m_lineWrapping) {
		int currentLineWidth =
			m_lineNumWidth +
			s_font->stringWidth(m_text.substr(lineStart(m_position), m_desiredXPos)) +
			(s_charWidth == s_zeroWidth ? 0 : s_charWidth); // fixed width fonts don't need the extra padding
		if(currentLineWidth > m_visibleWidth) {
			m_posX = -(currentLineWidth-m_visibleWidth);
		}
		m_desiredXPos = offsetToCurrentLineStart();
	}

	ofPushStyle();
	ofPushView();
		ofEnableAlphaBlending(); // for fontstash
		ofViewport(0, 0, m_width, m_height);
		ofPushMatrix();
		
		// scale when using auto focus
		if(m_autoFocus) {
			#ifdef DEBUG_AUTO_FOCUS
				ofNoFill();
				ofSetColor(ofColor::green);
				ofRect(0, 0, m_width, m_height);
				ofFill();
			#endif
			ofTranslate(0, m_height/2);
			ofScale(m_scale, m_scale);
		}
		ofTranslate(m_posX, m_posY);
	
		m_matchingCharsHighlight[0] = -1;
		m_matchingCharsHighlight[1] = -1;
		if(m_settings->getHighlightMatchingChars()) {
			parseMatchingChars();
		}
	
		m_displayedLineCount = 0;
		bool drawnCursor = false;
		int x = 0, y = s_charHeight; // pixel pos
		unsigned int textPos = 0;
		clearBoundingBox();
	
		// for line numbers
		int currentLine = 0;

		// draw text
		if(m_colorScheme) { // with colorScheme
			ofFill();
			
			s_font->setColor(m_settings->getTextColor(), m_settings->getAlpha());
			s_font->setShadowColor(m_settings->getTextShadowColor(), m_settings->getAlpha());
			
			// start with line number
			if(m_lineNumbers) {
				currentLine = lineNumberForPos(m_topTextPosition);
				drawLineNumber(x, y, currentLine);
			}
			
			bool string = false;
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
				
				// set font color based on block type
				switch(tb.type) {
				
					case UNKNOWN:
						ofLogWarning("ofxEditor") << "trying to draw UNKNOWN text block, contents: " << wstring_to_string(tb.text);
						continue; // skip
						
					case WORD:
						if(!string && !comment) s_font->setColor(m_colorScheme->getWordColor(tb.text), m_settings->getAlpha());
						break;
						
					case STRING_BEGIN:
						string = true;
						s_font->setColor(m_colorScheme->getStringColor(), m_settings->getAlpha());
						continue; // nothing to draw
						
					case STRING_END:
						string = false;
						s_font->setColor(m_colorScheme->getTextColor(), m_settings->getAlpha());
						continue; // nothing to draw
						
					case NUMBER:
						if(!string && !comment) s_font->setColor(m_colorScheme->getNumberColor(), m_settings->getAlpha());
						break;
					
					case MATCHING_CHAR: case MATH_CHAR: case PUNCTUATION_CHAR:
						if(!comment) s_font->setColor(m_colorScheme->getTextColor(), m_settings->getAlpha());
						break;
					
					case COMMENT_BEGIN:
						comment = true;
						s_font->setColor(m_colorScheme->getCommentColor(), m_settings->getAlpha());
						continue; // nothing to draw
						
					case COMMENT_END:
						comment = false;
						s_font->setColor(m_colorScheme->getTextColor(), m_settings->getAlpha());
						continue; // nothing to draw
					
					case SPACE: case TAB: case ENDLINE: // whitespace
						if(!string && !comment) s_font->setColor(m_colorScheme->getTextColor(), m_settings->getAlpha()); // for fonts with whitespace glyphs
						break;
				}
				
				// draw block chars
				for(int i = 0; i < tb.text.length(); ++i) {
					
					// line wrap at the block level
					if(m_lineWrapping && x >= m_visibleWidth) {
						y += s_charHeight;
						expandBoundingBox(x, y);
						x = 0;
						if(m_lineNumbers) { // pad for line numbers
							x += m_lineNumWidth;
						}
						m_displayedLineCount++;
					}
					
					// draw matching chars highlight
					if(!comment && m_selection == NONE && textPos >= m_matchingCharsHighlight[0] && textPos <= m_matchingCharsHighlight[1]) {
						drawMatchingCharBlock(tb.text[i], x, y);
					}
					
					// draw selection
					if(m_selection != NONE && textPos >= m_highlightStart && textPos < m_highlightEnd) {
						drawSelectionCharBlock(tb.text[i], x, y);
					}
					
					// draw cursor
					if(textPos == m_position) {
						drawCursor(x, y);
						expandBoundingBox(x+s_zeroWidth, y); // extra space for the cursor
						drawnCursor = true;
					}
					
					// draw chars
					switch(tb.type) {
						case ENDLINE:
							x = 0;
							y += s_charHeight;
							textPos++;
							m_displayedLineCount++;
							if(m_lineNumbers) {
								drawLineNumber(x, y, currentLine);
							}
							break;
						case TAB:
							x += s_charWidth * m_settings->getTabWidth();
							textPos++;
							break;
						default:
							x = s_font->drawCharacter(tb.text[i], x, y, s_textShadow);
							textPos++;
							break;
					}
					expandBoundingBox(x,y);
				}
			}
		}
		else { // without syntax highlighting
			ofFill();
			
			s_font->setColor(m_settings->getTextColor(), m_settings->getAlpha());
			s_font->setShadowColor(m_settings->getTextShadowColor(), m_settings->getAlpha());
			
			// start with line number
			if(m_lineNumbers) {
				currentLine = lineNumberForPos(m_topTextPosition);
				drawLineNumber(x, y, currentLine);
			}
			
			textPos = m_topTextPosition;
			for(int i = m_topTextPosition; i < m_text.length() && m_displayedLineCount < m_visibleLines; ++i) {
				
				// line wrap
				if(m_lineWrapping && x >= m_visibleWidth) {
					y += s_charHeight;
					expandBoundingBox(x, y);
					x = 0;
					if(m_lineNumbers) { // pad for line numbers
						x += m_lineNumWidth;
					}
					m_displayedLineCount++;
				}
				
				// draw matching chars highlight
				if(m_selection == NONE && textPos >= m_matchingCharsHighlight[0] && textPos <= m_matchingCharsHighlight[1]) {
					drawMatchingCharBlock(m_text[i], x, y);
				}
				
				// draw selection
				if(m_selection != NONE && i >= m_highlightStart && i < m_highlightEnd) {
					drawSelectionCharBlock(m_text[i], x, y);
				}
				
				// draw cursor
				if(i == m_position) {
					drawCursor(x, y);
					expandBoundingBox(x+s_zeroWidth, y); // extra space for the cursor
					drawnCursor = true;
				}
			
				// endline
				if(m_text[i] == '\n') {
					expandBoundingBox(x, y+s_charHeight);
					x = 0;
					y += s_charHeight;
					textPos++;
					m_displayedLineCount++;
					if(m_lineNumbers) {
						drawLineNumber(x, y, currentLine);
					}
				}
				// tab
				else if(m_text[i] == '\t') {
					x += s_charWidth * m_settings->getTabWidth();
					textPos++;
					expandBoundingBox(x, y);
				}
				// everything else
				else {
					x = s_font->drawCharacter(m_text[i], x, y, s_textShadow);
					textPos++;
					expandBoundingBox(x, y);
				}
			}
		}
	
		// update vertical scrolling
		if(m_displayedLineCount >= m_visibleLines) {
			m_bottomTextPosition = textPos;
		}
		else {
			m_bottomTextPosition = m_text.size()+1;
		}
		
		// draw cursor if we have no text, or if we're at the end of the buffer
		if(!drawnCursor) {
			drawCursor(x, y);
			expandBoundingBox(x+s_zeroWidth, y); // extra space for the cursor
		}
	
		// calculate auto focus bounding box and scaling
		if(m_autoFocus) {
			m_posY = m_posY*(1-m_delta) - (m_BBMinY+(m_BBMaxY-m_BBMinY)/2)*m_delta;
			
			float boxwidth = (m_BBMaxX-m_BBMinX) * m_scale;
			float boxheight = (m_BBMaxY-m_BBMinY) * m_scale;
			
			if(boxwidth > m_width + s_autoFocusError) {
				m_scale *= 1-s_autoFocusScaleDrift * m_delta; // shrink
			}
			else if(boxwidth < m_width - s_autoFocusError &&
			        boxheight < m_height - s_autoFocusError) {
				m_scale *= 1+s_autoFocusScaleDrift * m_delta; // grow
			}
			else if(boxheight > m_height + s_autoFocusError) {
				m_scale *= 1-s_autoFocusScaleDrift * m_delta; // shrink
			}
			else if(boxwidth < m_width - s_autoFocusError &&
					boxheight < m_height - s_autoFocusError) {
				m_scale *= 1+s_autoFocusScaleDrift * m_delta; // grow
			}
			m_scale = ofClamp(m_scale, s_autoFocusMinScale, s_autoFocusMaxScale);
			
			#ifdef DEBUG_AUTO_FOCUS
				ofNoFill();
				ofSetColor(ofColor::red);
				ofBeginShape();
					ofVertex(m_BBMinX, m_BBMinY, 0);
					ofVertex(m_BBMaxX, m_BBMinY, 0);
					ofVertex(m_BBMaxX, m_BBMaxY, 0);
					ofVertex(m_BBMinX, m_BBMaxY, 0);
				ofEndShape();
			#endif
		}
		else {
			m_posY = 0;
			m_scale = 1.0;
		}
	
		ofPopMatrix();
	ofPopView();
	ofPopStyle();
	
	updateTimestamps();
}

//--------------------------------------------------------------
void ofxEditor::keyPressed(int key) {

	// filter out modifier key events, except SHIFT & current modifier
	switch(key) {
		case OF_KEY_ALT: case OF_KEY_LEFT_ALT: case OF_KEY_RIGHT_ALT:
		case OF_KEY_LEFT_SHIFT: case OF_KEY_RIGHT_SHIFT:
		case OF_KEY_CONTROL: case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
		case OF_KEY_SUPER: case OF_KEY_LEFT_SUPER: case OF_KEY_RIGHT_SUPER:
		return;
	}
	
	bool modifierPressed = s_superAsModifier ? ofGetKeyPressed(OF_KEY_SUPER) : ofGetKeyPressed(OF_KEY_CONTROL);
	if(modifierPressed) {
	
		// check control chars too if CTRL is modifier
		switch(key) {
				
			case 'a': case 10: // clear all text
				if(ofGetKeyPressed(OF_KEY_SHIFT)) {
					clearText();
				}
				else { // select all
					m_selection = ALL;
					m_highlightStart = 0;
					m_highlightEnd = m_text.size();
					m_position = m_text.size();
				}
				break;
				
			case 'x': case 24: // cut
				if(m_selection != NONE) {
					copySelection();
					m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
					if(m_position >= m_highlightEnd) {
						m_position -= m_highlightEnd-m_highlightStart;
					}
					m_selection = NONE;
				}
				break;
			
			case 'c': case 3: // copy
				copySelection();
				break;
			
			case 'v': case 22: // paste
				pasteSelection();
				m_selection = NONE;
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
					m_desiredXPos = offsetToCurrentLineStart();
				}
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
			
			case OF_KEY_LEFT:
				if(m_position > 0) {
					m_position--;
				}
				if(!m_lineWrapping) {
					m_desiredXPos = offsetToCurrentLineStart();
				}
				m_flash = HALF_FLASH_RATE; // show cursor after moving
				break;
			
			case OF_KEY_END:
				m_position = lineEnd(m_position);
				m_desiredXPos = offsetToCurrentLineStart()+1;
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
					m_shiftState = true;
					if(m_selection == NONE) {
						m_highlightStart = m_position;
						m_highlightEnd = m_position;
						m_selection = FORWARD;
					}
				}
				break;
				
			case OF_KEY_DEL:
				if(m_selection) {
					m_selection = NONE;
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
					if(m_selection != NONE) {
						m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
						if(m_position >= m_highlightEnd) {
							m_position -= m_highlightEnd-m_highlightStart;
						}						
						m_selection = NONE;
					}
					else if(m_position != 0) {
						m_text.erase(m_position-1, 1);
						m_position--;
					}
				}
				textBufferUpdated();
				break;
				
			case OF_KEY_TAB:
				if(m_settings->getConvertTabs()) {
					m_text.insert(m_position, wstring(m_settings->getTabWidth(), ' '));
					m_position += m_settings->getTabWidth();
				}
				else {
					m_text.insert(m_position, L"\t");
					m_position++;
				}
				textBufferUpdated();
				break;
				
			case OF_KEY_ESC:
				if(m_selection == FORWARD) {
					m_position = m_highlightEnd;
				}
				else {
					m_position = m_highlightStart;
				}
				m_selection = NONE;
				break;
				
			case OF_KEY_RETURN:
				key = '\n'; // fallthrough (replacement of newline)
				m_numLines++;
				
			default:
				
				// build multibyte UTF-8 character
				if(key > 0x80) {
					if(m_UTF8Bytes == 0) {
						m_UTF8Bytes = wchar_width(key);
					}
					m_UTF8Char.push_back(key);
					if(m_UTF8Char.length() < m_UTF8Bytes) {
						return;
					}
				}
				else if(m_UTF8Bytes > 0) {
					ofLogWarning("ofxEditor") << "dropping bad UTF8 bytes";
					m_UTF8Bytes = 0;
					m_UTF8Char = "";
				}
				
				if(m_selection != NONE) {
					m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
					if(m_position >= m_highlightEnd) {
						m_position -= m_highlightEnd-m_highlightStart;
					}
					m_selection = NONE;
				}
				
				// ignore control chars
				if(key != '\n' && key < ' ') {
					break;
				}
				
				// multibyte UTF8
				if(m_UTF8Bytes > 0) {
					m_UTF8Bytes = 0;
				}
				else { // single byte UTF8 & ASCII
					m_UTF8Char.push_back(key);
				}
				m_text.insert(m_position, string_to_wstring(m_UTF8Char));
				m_UTF8Char = "";
				m_position++;
				
				if(key == '\n' && m_position >= m_bottomTextPosition && m_displayedLineCount+1 >= m_visibleLines) {
					m_topTextPosition = lineEnd(m_topTextPosition)+1;
				}
				
				textBufferUpdated();
				break;
		}
	}
	
	bool over = false;
	if(m_position > m_text.size()) {
		m_position = m_text.size();
		over = true;
	}
	
	// update selection
	if(m_shiftState) {

		if(!ofGetKeyPressed(OF_KEY_SHIFT) && !modifierPressed) {
			m_shiftState = false;
			m_selection = NONE;
			return;
		}
		
		switch(m_selection) {
			
			case ALL:
				if(m_position < m_highlightEnd) {
					m_highlightEnd = m_position;
					m_selection = FORWARD;
				}
				break;
			
			case BACKWARD:
				if(m_position < m_highlightStart) {
					m_highlightStart = m_position;
				}
				else if(m_position > m_highlightStart) {
					if(m_position > m_highlightEnd) {
						m_highlightStart = m_highlightEnd;
						m_highlightEnd = m_position;
						m_selection = FORWARD;
					}
					else {
						m_highlightStart = m_position;
					}
				}
				break;
			
			case FORWARD:
				if(m_position < m_highlightStart) {
					m_highlightStart = m_position;
					m_selection = BACKWARD;
				}
				else {
					m_highlightEnd = m_position;
				}
				break;
				
			case NONE:
				break;
		}
	}
	else if(m_selection == ALL) {
		if(over) {
			m_selection = NONE; // stay on end
		}
		else if(m_position != m_highlightEnd) {
			m_selection = NONE;
			m_position = m_selectAllStartPos; // jump to start
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::resize() {
	int w = (ofGetWindowMode() == OF_WINDOW) ? ofGetViewportWidth() : ofGetScreenWidth();
	int h = (ofGetWindowMode() == OF_WINDOW) ? ofGetViewportHeight() : ofGetScreenHeight();
	resize(w, h);
}


void ofxEditor::resize(int width, int height) {
	m_width = width;
	m_height = height;
	
	m_visibleLines = height/s_charHeight;
	
	ofLogVerbose("ofxEditor") << "pixel size: " << width << " " << height;
	ofLogVerbose("ofxEditor") << "num lines: " << m_visibleLines;
}

//--------------------------------------------------------------
bool ofxEditor::openFile(string filename) {
	ofFile file;
	if(!file.open(ofToDataPath(filename), ofFile::ReadOnly)) {
		ofLogError() << "ofxEditor: couldn't load \""
			<< ofFilePath::getFileName(filename) << "\"";
		return false;
	}
	setText(file.readToBuffer().getText());
	file.close();
	return true;
}
		
//--------------------------------------------------------------
bool ofxEditor::saveFile(string filename) {
	ofFile file;
	if(!file.open(ofToDataPath(filename), ofFile::WriteOnly)) {
		ofLogError() << "ofxGLEditor: couldn't open \""
			<< ofFilePath::getFileName(filename) << "\" for saving";
		return false;
	}
	file << getText();
	file.close();
	return true;
}

//--------------------------------------------------------------
wstring ofxEditor::getWideText() {
	if(m_selection != NONE) {
		return m_text.substr(m_highlightStart, m_highlightEnd-m_highlightStart);
	}
	return m_text;
}

//--------------------------------------------------------------
string ofxEditor::getText() {
	if(m_selection != NONE) {
		return wstring_to_string(m_text.substr(m_highlightStart, m_highlightEnd-m_highlightStart));
	}
	return wstring_to_string(m_text);
}

//--------------------------------------------------------------
void ofxEditor::setText(const wstring& text) {
	
	if(m_text != L"") {
		m_position = lineStart(m_position);
		int line = getCurrentLine();
		m_text = text;
		setCurrentLine(line);
	}
	else {
		m_text = text;
	}
		
	if(m_settings->getConvertTabs()) {
		processTabs();
	}
	textBufferUpdated();
}

//--------------------------------------------------------------
void ofxEditor::setText(const string& text) {
	setText(string_to_wstring(text));
}

//--------------------------------------------------------------
void ofxEditor::insertText(const wstring& text) {
	if(m_selection != NONE) {
		m_text.erase(m_highlightStart, m_highlightEnd-m_highlightStart);
		if(m_position >= m_highlightEnd) {
			m_position -= m_highlightEnd-m_highlightStart;
		}
	}
	m_text.insert(m_position, text);
	m_selection = NONE;
	m_position += text.size();
	if(m_settings->getConvertTabs()) {
		processTabs();
	}
	textBufferUpdated();
}

//--------------------------------------------------------------
void ofxEditor::insertText(const string& text) {
	insertText(string_to_wstring(text));
}

//--------------------------------------------------------------
void ofxEditor::clearText() {
	m_text = L"";
	if(m_colorScheme) {
		clearTextBlocks();
	}
	m_position = 0;
	m_numLines = 0;
	setCurrentLine(0);
	m_shiftState = false;
	m_selection = NONE;
	m_posX = m_posY = 0;
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
	if(!m_lineWrapping) { // recalculate text offset if needed
		m_desiredXPos = offsetToCurrentLineStart();
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
		m_lineNumWidth = ofToString(m_numLines+1).length()*s_zeroWidth + s_charWidth; // include space
	}
	else {
		m_lineNumWidth = 0;
	}
}

//--------------------------------------------------------------
bool ofxEditor::getLineNumbers() {
	return m_lineNumbers;
}

//--------------------------------------------------------------
void ofxEditor::setAutoFocus(bool focus) {
	m_autoFocus = focus;
}
	
//--------------------------------------------------------------
bool ofxEditor::getAutoFocus() {
	return m_autoFocus;
}

// CURSOR POSITION & INFO

//--------------------------------------------------------------
void ofxEditor::blowupCursor() {
	m_blowupCursor = true;
	m_blowup = 0;
}

//--------------------------------------------------------------
bool ofxEditor::isSelection() {
	return m_selection != NONE;
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
	for(unsigned int i = 0; count <= line && i < m_text.size(); i++) {
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
	m_selection = NONE;
	m_scale = 1.0;
	m_posX = m_posY = 0;
}

// UTILS

//--------------------------------------------------------------
float ofxEditor::drawString(string s, float x, float y) {
	return drawString(string_to_wstring(s), x, y);
}

//--------------------------------------------------------------
float ofxEditor::drawString(string s, ofPoint& p) {
	return drawString(string_to_wstring(s), p.x, p.y);
}

//--------------------------------------------------------------
float ofxEditor::drawString(wstring s, float x, float y) {
	s_font->pushState();
	ofColor c = ofGetStyle().color;
	s_font->setColor(c);
	
	int xPos = x, yPos = y+s_charHeight;
	for(int i = 0; i < s.size(); ++i) {
		if(s[i] == '\n') {
			xPos = x;
			yPos += s_charHeight;
		}
		else if(s[i] == '\t') {
			xPos += s_charWidth*m_settings->getTabWidth();
		}
		else {
			xPos = s_font->drawCharacter(s[i], xPos, yPos, s_textShadow);
		}
	}
	
	s_font->popState();
	return xPos;
}

//--------------------------------------------------------------
float ofxEditor::drawString(wstring s, ofPoint& p) {
	return drawString(s, p.x, p.y);
}

//--------------------------------------------------------------
void ofxEditor::printSyntax() {
	if(!m_colorScheme) return;
	for(list<TextBlock>::iterator iter = m_textBlocks.begin();
		iter != m_textBlocks.end() && m_displayedLineCount < m_visibleLines; iter++) {
		TextBlock &tb = (*iter);
		string type;
		switch(tb.type) {
			case UNKNOWN:          type += "UNKNOWN"; break;
			case WORD:             type += "WORD"; break;
			case STRING_BEGIN:     ofLogNotice() << "STRING_BEGIN"; continue;
			case STRING_END:       ofLogNotice() << "STRING_END"; continue;
			case NUMBER:           type += "NUMBER"; break;
			case SPACE:            type += "SPACE"; break;
			case TAB:              type += "TAB"; break;
			case ENDLINE:          ofLogNotice() << "ENDLINE"; continue;
			case MATCHING_CHAR:    type += "MATCHING_CHAR"; break;
			case MATH_CHAR:        type += "MATH_CHAR"; break;
			case PUNCTUATION_CHAR: type += "PUNCTUATION_CHAR"; break;
			case COMMENT_BEGIN:    ofLogNotice() << "COMMENT_BEGIN"; continue;
			case COMMENT_END:      ofLogNotice() << "COMMENT_END"; continue;
		}
		ofLogNotice() << type << ": \"" << wstring_to_string(tb.text) << "\"";
	}
}

// PROTECTED

//--------------------------------------------------------------
float ofxEditor::characterWidth(int c) {
	switch(c) {
		case '\n':
			return s_charWidth;
		case '\t':
			return s_charWidth * m_settings->getTabWidth();
		default:
			return s_font->characterWidth(c);
	}
}

//--------------------------------------------------------------
void ofxEditor::drawMatchingCharBlock(int c, int x, int y) {
	ofSetColor(
		m_settings->getMatchingCharsColor().r, m_settings->getMatchingCharsColor().g,
		m_settings->getMatchingCharsColor().b, m_settings->getMatchingCharsColor().a * m_settings->getAlpha());
	ofRect(x, y-s_charHeight, characterWidth(c), s_charHeight);
}

//--------------------------------------------------------------
void ofxEditor::drawSelectionCharBlock(int c, int x, int y) {
	ofSetColor(
		m_settings->getSelectionColor().r, m_settings->getSelectionColor().g,
		m_settings->getSelectionColor().b, m_settings->getSelectionColor().a * m_settings->getAlpha());
	ofRect(x, y-s_charHeight, characterWidth(c), s_charHeight);
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
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofSetColor(m_settings->getCursorColor().r, m_settings->getCursorColor().g,
					   m_settings->getCursorColor().b, m_settings->getCursorColor().a * m_settings->getAlpha() * m_blowup/BLOWUP_FLASHES);
			ofRect(MAX(x+maxCW/2, x), MAX(y-s_charHeight+maxCH/2, y-s_charHeight), MAX(maxCW, s_cursorWidth), MAX(maxCH, s_charHeight));
			ofSetRectMode(OF_RECTMODE_CORNER);
		}
	}
	else {
		m_flash += m_delta;
		if (m_flash > FLASH_RATE) {
			m_flash = 0;
		}
		if (m_flash > HALF_FLASH_RATE) {
			ofSetColor(m_settings->getCursorColor().r, m_settings->getCursorColor().g,
					   m_settings->getCursorColor().b, m_settings->getCursorColor().a * m_settings->getAlpha());
			ofRect(x, y-s_charHeight, s_cursorWidth, s_charHeight);
			ofSetRectMode(OF_RECTMODE_CORNER);
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::drawLineNumber(int &x, int &y, int &currentLine) {
	s_font->pushState();
	s_font->setColor(m_settings->getLineNumberColor(), m_settings->getAlpha());
	
	currentLine++;
	string currentLineString = ofToString(currentLine);
	x += s_zeroWidth*(ofToString(m_numLines).length()-currentLineString.length()); // leading space padding
	for(int i = 0; i < currentLineString.length(); ++i) {
		x = s_font->drawCharacter(currentLineString[i], x, y, s_textShadow);
	}
	x += s_charWidth; // the trailing space
	
	s_font->popState();
}

//--------------------------------------------------------------
void ofxEditor::processTabs() {
	size_t pos = m_text.find(L"\t", 0);
	while(pos != string::npos) {
		m_text.erase(pos, 1);
		m_text.insert(pos, wstring(m_settings->getTabWidth(), ' '));
		pos = m_text.find(L"\t", pos);
	}
}

//--------------------------------------------------------------
int ofxEditor::offsetToCurrentLineStart() {
	return m_position - lineStart(m_position);
}

//--------------------------------------------------------------
int ofxEditor::nextLineLength(int pos) {
	size_t nextLineStart = m_text.find(L"\n", pos);
	if(nextLineStart != string::npos) {
		return lineLength(nextLineStart+1);
	}
	return 0;
}

//--------------------------------------------------------------
int ofxEditor::previousLineLength(int pos) {
	size_t prevLineEnd = string::npos;
	if(pos > 0) {
		prevLineEnd = m_text.rfind(L"\n", pos-1);
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
			start = m_text.rfind(L"\n", pos-1);
		}
		else {
			start = m_text.rfind(L"\n", pos);
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
	size_t end = m_text.find(L"\n", pos);
	if(end == string::npos) {
		end = m_text.size()-1;
	}
	return end;
}

//--------------------------------------------------------------
void ofxEditor::parseMatchingChars() {

	wstring &openChars = m_settings->getWideOpenChars();
	wstring &closeChars = m_settings->getWideCloseChars();

	// parse the parentheses
	int type = 0;
	for(wstring::iterator i = openChars.begin(); i != openChars.end(); ++i) {
		if(m_text[m_position] == *i) {
			parseOpenChars(m_position, type);
		}
		type++;
	}
		
	if(m_position > 0) {
		type = 0;
		for(wstring::iterator i = closeChars.begin(); i != closeChars.end(); ++i) {
			if(m_text[m_position-1] == *i) {
				parseCloseChars(m_position-1, type);
			}
			type++;
		}
	}
}

//--------------------------------------------------------------
void ofxEditor::parseOpenChars(int pos, int type) {

	wstring &openChars = m_settings->getWideOpenChars();
	wstring &closeChars = m_settings->getWideCloseChars();

	// looking for a close, so search forward
	int stack = 0, start_pos = pos;
	pos++;
	while(stack != -1 && pos < (int)m_text.size()) {
		if(m_text[pos] == openChars[type]) {
			stack++;
		}
		if(m_text[pos] == closeChars[type]) {
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

	wstring &openChars = m_settings->getWideOpenChars();
	wstring &closeChars = m_settings->getWideCloseChars();

	// looking for a open, so search backward
	int stack = 0, start_pos = pos;
	pos--;
	while(stack != -1 && pos >= 0) {
		if(m_text[pos] == closeChars[type]) {
			stack++;
		}
		if(m_text[pos] == openChars[type]) {
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
	
	if(m_selection == NONE) {
		return;
	}

	// use clipboard if available, otherwise use internal copybuffer
	#ifdef HAS_GLFW
		ofAppGLFWWindow *window = (ofAppGLFWWindow *) ofGetWindowPtr();
		glfwSetClipboardString(window->getGLFWWindow(), getText().c_str());
	#else
		s_copyBuffer = m_text.substr(m_highlightStart, m_highlightEnd-m_highlightStart);
	#endif
}

//--------------------------------------------------------------
void ofxEditor::pasteSelection() {

	// use clipboard if available, otherwise use internal copybuffer
	#ifdef HAS_GLFW
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

//--------------------------------------------------------------
void ofxEditor::updateTimestamps() {
	m_delta = ofGetElapsedTimef() - s_time;
	s_time = ofGetElapsedTimef();
	if (m_delta > 10.0f) {
		m_delta = 0.00001f;
	}
}

//--------------------------------------------------------------
void ofxEditor::expandBoundingBox(float x, float y) {
	if(x < m_BBMinX) m_BBMinX = x;
	if(x > m_BBMaxX) m_BBMaxX = x;
	if(y < m_BBMinY) m_BBMinY = y;
	if(y > m_BBMaxY) m_BBMaxY = y;
}

//--------------------------------------------------------------
void ofxEditor::clearBoundingBox() {
	m_BBMinX = m_BBMinY = m_BBMaxX = m_BBMaxY = 0;
}

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
		m_lineNumWidth = ofToString(m_numLines+1).length()*s_zeroWidth + s_charWidth; // +1 for 10 & 1 extra for the space
	}

	// scroll if we've added content at the far right
	if(!m_lineWrapping) {
		m_desiredXPos = offsetToCurrentLineStart();
	}
}

// PRIVATE

//--------------------------------------------------------------
// simple syntax parser
void ofxEditor::parseTextBlocks() {
	
	clearTextBlocks();
	m_numLines = 0;
	
	int string = false;
	bool singleComment = false;
	bool multiComment = false;
	wstring &singleLineComment = m_colorScheme->getWideSingleLineComment();
	wstring &multiLineCommentBegin = m_colorScheme->getWideMultiLineCommentBegin();
	wstring &multiLineCommentEnd = m_colorScheme->getWideMultiLineCommentEnd();
	
	TextBlock tb;
	for(int i = 0; i < m_text.length(); ++i) {
		
		switch(m_text[i]) {
		
			case ' ':
				if(tb.type != UNKNOWN) {
					m_textBlocks.push_back(tb);
					tb.clear();
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
				if(singleComment || multiComment) { // ignore strings in comments
					tb.text += m_text[i];
					break;
				}
				if(string == m_text[i]) { // same as the opening string char?
				
					// don't terminate an escaping slash
					if(tb.text.length() > 1 && tb.text[tb.text.size()-1] == '\\') {
						tb.text += m_text[i];
						break;
					}

					if(tb.type == UNKNOWN) {
						tb.type = WORD;
					}
					tb.text += m_text[i];
					m_textBlocks.push_back(tb);
					tb.clear();
					
					// push string end
					TextBlock stringBlock;
					stringBlock.type = STRING_END;
					m_textBlocks.push_back(stringBlock);
					
					string = false;
				}
				else if(string) { // wrong char, keep going
					tb.text += m_text[i];
				}
				else { // opening string char
					
					if(tb.type != UNKNOWN) {
						m_textBlocks.push_back(tb);
						tb.clear();
					}
					
					if(tb.type == UNKNOWN) {
						tb.type = WORD;
					}
					tb.text += m_text[i];
					
					// push string begin
					TextBlock stringBlock;
					stringBlock.type = STRING_BEGIN;
					m_textBlocks.push_back(stringBlock);
					
					string = m_text[i];
				}
				break;
				
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				if(tb.type != UNKNOWN) {
					if(string) {
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
				if(tb.type == NUMBER) {
					tb.text += m_text[i];
					break;
				}
		
			default: // everything else
				switch(tb.type) {
					case NUMBER:
						m_textBlocks.push_back(tb);
						tb.clear();
					case UNKNOWN:
						tb.type = WORD;
					case WORD:
						tb.text += m_text[i];
						
						// in a string, so everything is a word, number, or whitespace
						if(string) {
							break;
						}
						
						// detect comments
						if(!multiComment) {
							
							// check ahead for multi line comment begin
							if(i <= m_text.size()-multiLineCommentBegin.length() &&
							   m_text.substr(i, multiLineCommentBegin.length()) == multiLineCommentBegin) {
								
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
							else if(!singleComment) {
							
								// check ahead for single line comment
								if(i <= m_text.size()-singleLineComment.length() &&
								   m_text.substr(i, singleLineComment.length()) == singleLineComment) {
									
									// push comment begin
									TextBlock commentBlock;
									commentBlock.type = COMMENT_BEGIN;
									m_textBlocks.push_back(commentBlock);
									
									singleComment = true;
									continue;
								}
								
								// check for open/close characters
								if(m_settings->getWideOpenChars().find(m_text[i], 0) != wstring::npos ||
								   m_settings->getWideCloseChars().find(m_text[i], 0) != wstring::npos) {
									if(tb.type != UNKNOWN && tb.text.length() > 1) {
										tb.text = tb.text.substr(0, tb.text.length()-1);
										m_textBlocks.push_back(tb);
										tb.clear();
									}
									tb.type = MATCHING_CHAR;
									tb.text = m_text[i];
									m_textBlocks.push_back(tb);
									tb.clear();
									break;
								}
								
								// check for single mathematical characters
								if(m_colorScheme->getWideMathChars().find(m_text[i], 0) != wstring::npos) {
									if(tb.type != UNKNOWN && tb.text.length() > 1) {
										tb.text = tb.text.substr(0, tb.text.length()-1);
										m_textBlocks.push_back(tb);
										tb.clear();
									}
									tb.type = MATH_CHAR;
									tb.text = m_text[i];
									m_textBlocks.push_back(tb);
									tb.clear();
									break;
								}
								
								// check for single punctuation characters
								if(m_colorScheme->getWidePunctuationChars().find(m_text[i], 0) != wstring::npos) {
									if(tb.type != UNKNOWN && tb.text.length() > 1) {
										tb.text = tb.text.substr(0, tb.text.length()-1);
										m_textBlocks.push_back(tb);
										tb.clear();
									}
									tb.type = PUNCTUATION_CHAR;
									tb.text = m_text[i];
									m_textBlocks.push_back(tb);
									tb.clear();
									break;
								}
							}
						}
						else { // check for multi line comment end
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
						break;
						
					default: // already handled
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
