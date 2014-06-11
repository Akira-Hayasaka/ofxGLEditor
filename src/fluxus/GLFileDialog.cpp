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

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <iostream>

#ifdef WIN32
#include <dirent.h>
#else
#include <glob.h>
#endif

#ifdef __APPLE_APP__
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <sys/stat.h>
#include <sys/time.h>
#include "GLFileDialog.h"
#include "Unicode.h"

using namespace fluxus;

GLFileDialog::GLFileDialog() :
m_CurrentFile(0),
m_SaveAs(true)
{
	m_Path=L"";

#ifdef __APPLE_APP__
	// use ~/Documents/Fluxus in OSX app
	struct passwd *pwd = getpwuid(getuid());
	if (pwd)
	{
		string documents(pwd->pw_dir + string("/Documents/"));
		if (access(documents.c_str(), F_OK) == 0)
		{
			string fdocs = documents + "Fluxus/";
			if (access(fdocs.c_str(), F_OK) < 0)
			{
				if (mkdir(fdocs.c_str(), 0755) == 0)
				{
					documents = fdocs;
				}
			}
			else
			{
				documents = fdocs;
			}
		}
		m_Path = string_to_wstring(documents);
	}
#endif

	m_SaveAsInfoText=L"Save as (esc to exit)";
	m_Text=L"";
	ReadPath();
}

GLFileDialog::~GLFileDialog()
{
}


void GLFileDialog::ReadPath()
{
	m_Filenames.clear();
	m_Directories.clear();

#ifdef WIN32
    // this is posix code using mingw - so maybe it'll work for everything...
    string search=wstring_to_string(m_Path);
    if (search=="") search = ".";
    DIR *d = opendir(search.c_str());

    if (d)
    {
        dirent *dp;
        while ((dp=readdir(d))!=NULL)
        {
            if (opendir((search+string("/")+string(dp->d_name)).c_str()))
            {
                m_Directories.insert(m_Filenames.size());
            }                        
            m_Filenames.push_back(string_to_wstring(string(dp->d_name)));            
        }
    }
#else	
	glob_t g;
    

// NOTE: the following snippet is from dirscan.c in ldglite, 
//       Copyright (C) 1997-1998  Thomas Kern. 
//       it might even do something useful....
#ifndef GLOB_PERIOD
// Some BSD systems do NOT have the GNU implementation of glob,
// so we must do the GLOB_PERIOD work by hand to get the ".." dir.
#define GLOB_PERIOD GLOB_APPEND
    glob(".", 0, 0, &g);
    glob("..", GLOB_PERIOD, 0, &g);
#endif

    string temp = wstring_to_string(m_Path+L"*");
	glob(temp.c_str(),GLOB_PERIOD,NULL,&g);
	
	for (unsigned int n=0; n<g.gl_pathc; n++)
	{
		struct stat s; 
		stat(g.gl_pathv[n],&s);
		if (S_ISDIR(s.st_mode)) m_Directories.insert(m_Filenames.size());
		string path=g.gl_pathv[n];
		size_t lastslash=path.find_last_of("/");
		if (lastslash!=string::npos)
		{
			path=path.substr(path.find_last_of("/")+1,path.size());
		}
		
		m_Filenames.push_back(string_to_wstring(path));
	}
	
	globfree (&g);
#endif
}

void GLFileDialog::Render()
{
    glViewport(0,0,m_Width,m_Height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glFrustum(-1,1,-0.75,0.75,1,10);
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LINE_STIPPLE);
   	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glLineWidth(m_TextWidth);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glLoadIdentity();
	glTranslatef(-4,3,-5);
	glColor4f(0.7,0.7,0.7,1);
	glScalef(0.0001,0.0001,1);
	
	glTranslatef(m_PosX,m_PosY,0);
		
	glTranslatef(0,-25000,0);	
	
	if (m_SaveAs) RenderSaveAs();
	else RenderLoad();
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LINE_STIPPLE);	
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	#ifndef WIN32
	timeval ThisTime;
	// stop valgrind complaining
	ThisTime.tv_sec=0;
	ThisTime.tv_usec=0;
	
	gettimeofday(&ThisTime,NULL);
	m_Delta=(ThisTime.tv_sec-m_Time.tv_sec)+
			(ThisTime.tv_usec-m_Time.tv_usec)*0.000001f;
	m_Time=ThisTime;
	#else
	m_Delta=0.02;
	#endif

	if (m_Delta>100.0f) m_Delta=0.000001f;
}

void GLFileDialog::Handle(int button, int key, int special, int state, int x, int y, int mod)
{	
	unsigned int startpos=m_Position;
	
	if (m_SaveAs) HandleSaveAs(button, key, special, state, x, y, mod);
	else HandleLoad(button, key, special, state, x, y, mod);
}

void GLFileDialog::RenderSaveAs()
{
	glPushMatrix();
	glTranslatef(2000,0,0);
	
	glPushMatrix();
	for (unsigned int n=0; n<m_SaveAsInfoText.size(); n++)
	{
		float width=StrokeWidth(m_SaveAsInfoText[n]);
		glColor4f(m_TextColourRed,m_TextColourGreen,m_TextColourBlue,m_Alpha*m_TextColourAlpha);
		StrokeCharacter(m_SaveAsInfoText[n]);
	}
	glPopMatrix();
	
	glTranslatef(0,-m_CharHeight,0);

	bool drawncursor=false;
	for (unsigned int n=0; n<m_Text.size(); n++)
	{
		float width=StrokeWidth(m_Text[n]);

		if (width==0) // bad character
		{
			width=m_CursorWidth;
			glColor4f(1,1,0,0.5);
			DrawCharBlock();
			glColor4f(0.7,0.7,0.7,1);
			glTranslatef(width,0,0);
		}

		if (m_Position==n) // draw cursor
		{ 
			glColor4f(m_CursorColourRed,m_CursorColourGreen,m_CursorColourBlue,m_Alpha*m_CursorColourAlpha);
			DrawCursor();
			glColor4f(0.7,0.7,0.7,1);
			drawncursor=true;
		}	

		glColor4f(m_TextColourRed,m_TextColourGreen,m_TextColourBlue,m_Alpha*m_TextColourAlpha);
		StrokeCharacter(m_Text[n]);
	}

	// draw cursor if we have no text, or if we're at the end of the buffer
	if (!drawncursor)
	{
		glColor4f(m_CursorColourRed,m_CursorColourGreen,m_CursorColourBlue,m_Alpha*m_CursorColourAlpha);
		DrawCursor();
		glColor4f(0.7,0.7,0.7,1);
	}

	glPopMatrix();
}

void GLFileDialog::RenderLoad()
{
	glPushMatrix();

	float lineheight=m_CharHeight;
	const int displayrange = 10;
	float ypos=(m_CurrentFile/(float)m_Filenames.size())*
				lineheight*(float)m_Filenames.size();

	unsigned int count=0;
	for (vector<wstring>::iterator i=m_Filenames.begin(); i!=m_Filenames.end(); i++)
	{
		unsigned int n=0;
		m_LineCount=0;

		bool isdir=m_Directories.find(count)!=m_Directories.end();

		if (((int)count > ((int)m_CurrentFile - displayrange)) &&
		    ((int)count < ((int)m_CurrentFile + displayrange)))
		{
			glTranslatef(0, ypos, 0);
			while (n<i->size())
			{
				float width=StrokeWidth((*i)[n]);

				if (width==0) // bad character
				{
					width=m_CursorWidth;
					glColor4f(1,1,0,0.5);
					DrawCharBlock();
					glColor4f(0.7,0.7,0.7,1);
					glTranslatef(width,0,0);
				}

				if (count == m_CurrentFile)
				{
					glColor4f(m_CursorColourRed,m_CursorColourGreen,m_CursorColourBlue,m_Alpha*m_CursorColourAlpha);
					DrawCharBlock();
					glColor4f(0.7,0.7,0.7,1);
				}

				if (isdir) glColor4f(1-m_TextColourRed+0.5,
									 1-m_TextColourGreen+0.5,
									 1-m_TextColourBlue+0.5,
									 m_Alpha*(1-m_TextColourAlpha+0.5));
				else glColor4f(m_TextColourRed,m_TextColourGreen,m_TextColourBlue,m_Alpha*m_TextColourAlpha);
				StrokeCharacter((*i)[n]);

				n++;
			}
		}

		glPopMatrix();
		glPushMatrix();
		ypos-=lineheight;
		//glRotatef((m_CurrentFile-(float)count-1)*15.0f,0,0,1);
		// can't resist the foolishness...
		count++;
	}
	glPopMatrix();
}

void GLFileDialog::HandleSaveAs(int button, int key, int special, int state, int x, int y, int mod)
{
	if (special!=-1)
	{
		switch(special)
		{
			case GLUT_KEY_RIGHT: 
			{
				if (!m_Text.empty()) m_Position++;
				if (m_Position>m_Text.size()) m_Position=m_Text.size();
			}
			break;
			case GLUT_KEY_LEFT: 
			{
				if (m_Position>0) m_Position--;  
			}
			break;
		}
	}	
	else
	{
		switch (key)
		{
			case GLEDITOR_RETURN:
			{
				m_Output=m_Path+m_Text;
			}
			break;
			case GLEDITOR_DELETE: m_Text.erase(m_Position,1); break; // delete
			case GLEDITOR_BACKSPACE: // backspace
			{
				if (!m_Text.empty() && m_Position!=0)
				{
					if (m_Selection)
					{
						m_Text.erase(m_HighlightStart,m_HighlightEnd-m_HighlightStart); 
						m_Position-=m_HighlightEnd-m_HighlightStart;						
						m_Selection=false;
					}
					else
					{
						m_Text.erase(m_Position-1,1); 
						m_Position--; 
					}
				}
			}
			break;
			default:
			{
				if (key!=-1 && !(mod&GLUT_ACTIVE_CTRL))
				{
					string temp(" ");
					temp[0]=(char)key;
					m_Text.insert(m_Position,string_to_wstring(temp));
					m_Position++;
				}
			}
			break;
		}
	}	
}

void GLFileDialog::HandleLoad(int button, int key, int special, int state, int x, int y, int mod)
{
	if (special!=-1)
	{
		switch(special)
		{
			case GLUT_KEY_END: m_CurrentFile=m_Filenames.size(); break;
			case GLUT_KEY_HOME: m_CurrentFile=0; break;
			case GLUT_KEY_UP: 
			{
				if (m_CurrentFile==0) m_CurrentFile=m_Filenames.size();
				else m_CurrentFile--;			
			}
			break;
			case GLUT_KEY_DOWN: 
			{
				m_CurrentFile++;
				if (m_CurrentFile>=m_Filenames.size()) m_CurrentFile=0;
			}
			break;
			case GLUT_KEY_PAGE_UP: m_CurrentFile-=10; break;
			case GLUT_KEY_PAGE_DOWN: m_CurrentFile+=10; break;
		}
	}	
	else
	{
		switch (key)
		{
			case GLEDITOR_RETURN:
			{
				if(m_Directories.find(m_CurrentFile)!=m_Directories.end())
				{
					m_Path+=m_Filenames[m_CurrentFile];
					m_Path+=L"/";
					ReadPath();
				}
				else
				{
					m_Output=m_Path+m_Filenames[m_CurrentFile];
				}
			}
			break;
		}
		
	}	
	
	if (m_CurrentFile>=m_Filenames.size())
	{
		m_CurrentFile=m_Filenames.size()-1;
	}
}

