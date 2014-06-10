// Copyright (C) 2005 Dave Griffiths
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef _GL_EDITOR_H_
#define _GL_EDITOR_H_

#include <string>
#include "PolyGlyph.h"

#ifndef __APPLE__
#define GLEDITOR_DELETE 127
#define GLEDITOR_BACKSPACE 8
#else
#define GLEDITOR_DELETE 8
#define GLEDITOR_BACKSPACE 127
#endif

#define GLEDITOR_TAB 9
#define GLEDITOR_RETURN 13
#define GLEDITOR_CUT 24 
#define GLEDITOR_COPY 3 
#define GLEDITOR_PASTE 22 
#define GLEDITOR_PLUS 61 
#define GLEDITOR_MINUS 45 

using namespace std;

namespace fluxus 
{

class GLEditor
{
public:
	GLEditor();
	virtual ~GLEditor();

	virtual void Render();
	virtual void Handle(int button, int key, int special, int state, int x, int y, int mod);
	void Reshape(unsigned int w,unsigned int h);
	void BlowupCursor();

	wstring GetText();
	wstring GetAllText() { return m_Text; }
	wstring GetSExpr();
	void ClearAllText();

	void SetText(const wstring& s);
	void Reset();

	float m_PosX,m_PosY;
	float m_Scale;
	float m_CursorMaxWidth;
	float m_CursorMaxHeight;

	void StrokeCharacter(wchar_t c, float dx = 0, float dy = 0);
	float StrokeWidth(wchar_t c);

	static void InitFont(const wstring &ttf);
	static void GetEffectParameters();

	static float m_TextWidth;
	static float m_TextColourRed;
	static float m_TextColourGreen;
	static float m_TextColourBlue;
	static float m_TextColourAlpha;
	static float m_CursorColourRed;
	static float m_CursorColourGreen;
	static float m_CursorColourBlue;
	static float m_CursorColourAlpha;
	static float m_Alpha;
	static bool m_DoAutoFocus;
	static bool m_DebugAutoFocus;
	static float m_AutoFocusWidth;
	static float m_AutoFocusHeight;
	static float m_AutoFocusError;
	static float m_AutoFocusDrift;
	static float m_AutoFocusScaleDrift;
	static float m_AutoFocusMinScale;
	static float m_AutoFocusMaxScale;
	static unsigned int m_VisibleLines;
	static unsigned int m_VisibleColumns;
	static float m_XPos;
	static float m_YPos;

	static bool m_DoEffects;
	static float m_EffectJiggleSize;

	static float m_EffectWaveSize;
	static float m_EffectWaveWavelength;
	static float m_EffectWaveSpeed;
	static float m_EffectWaveTimer;

	static float m_EffectRippleSize;
	static float m_EffectRippleCenterX;
	static float m_EffectRippleCenterY;
	static float m_EffectRippleWavelength;
	static float m_EffectRippleSpeed;
	static float m_EffectRippleTimer;

	static float m_EffectSwirlSize;
	static float m_EffectSwirlCenterX;
	static float m_EffectSwirlCenterY;
	static float m_EffectSwirlRotation;

protected:

	void DrawCharBlock();
	void DrawCursor();
	void ProcessTabs();
	int OffsetToCurrentLineStart();
	int NextLineLength(int pos);
	int PreviousLineLength(int pos);
	int LineLength(int pos);
	unsigned int LineStart(int pos);
	unsigned int LineEnd(int pos);
	void ParseParentheses();
	void ParseOpenParentheses(int pos, int type);
	void ParseCloseParentheses(int pos, int type);

	int GetCurrentLine();
	void SetCurrentLine(int line);

	void BBExpand(float x, float y);
	void BBClear() { m_BBMinX=m_BBMinY=m_BBMaxX=m_BBMaxY=0; }

	wstring m_Text;
	static wstring m_CopyBuffer;
	unsigned int m_Position;
	unsigned int m_HighlightStart;
	unsigned int m_HighlightEnd;
	unsigned int m_DesiredXPos;
	bool m_Selection;
	bool m_ShiftState;
	bool m_CtrlState;
	float m_CursorWidth;
	float m_CharWidth;
	float m_CharHeight;
	int m_ParenthesesHighlight[2];
	wstring m_OpenChars;
	wstring m_CloseChars;
	unsigned int m_LeftTextPosition;
	unsigned int m_TopTextPosition;
	unsigned int m_BottomTextPosition;
	unsigned int m_LineCount;

	float m_BBMinX;
	float m_BBMinY;
	float m_BBMaxX;
	float m_BBMaxY;

	int m_Width;
	int m_Height;

	static PolyGlyph *m_PolyGlyph;
#ifndef WIN32
	static timeval m_Time;
#endif
	float m_Delta;
	float m_Flash;
	bool m_BlowupCursor;
	float m_Blowup;
    char m_FirstUTF8Byte;

};

} // namespace fluxus

#endif
