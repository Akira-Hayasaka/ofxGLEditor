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

#ifndef _GL_DIALOG_H_
#define _GL_DIALOG_H_

#include <string>
#include <vector>
#include <set>
#include "GLEditor.h"

using namespace std;

namespace fluxus
{

class GLFileDialog : public GLEditor
{
public:
	GLFileDialog();
	virtual ~GLFileDialog();

	virtual void Render();
	virtual void Handle(int button, int key, int special, int state, int x, int y, int mod);

	wstring GetOutput() { return m_Output; }
	void Clear() { m_Output=L""; }
	void SetSaveAsMode(bool s) { m_SaveAs=s; }
	bool GetSaveAsMode() { return m_SaveAs; }

	void Refresh() { ReadPath(); }

protected:

	void RenderSaveAs();
	void RenderLoad();
	void HandleSaveAs(int button, int key, int special, int state, int x, int y, int mod);
	void HandleLoad(int button, int key, int special, int state, int x, int y, int mod);

	void ReadPath();

	unsigned int m_CurrentFile;
	vector<wstring> m_Filenames;
	set<int> m_Directories;
	wstring m_Path;
	wstring m_Output;
	wstring m_SaveAsInfoText;

	bool m_SaveAs;
};

} // namespace fluxus

#endif
