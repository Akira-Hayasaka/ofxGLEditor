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

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include "Recorder.h"

EventRecorder::EventRecorder() :
m_Mode(OFF),
m_LastTimeSeconds(0),
m_TimeSeconds(0),
m_Delta(0.0f)
{
	m_LastTime.tv_sec=0;
	m_LastTime.tv_usec=0;
}

EventRecorder::~EventRecorder()
{
}

bool EventRecorder::Get(list<RecorderMessage> &events)
{
	bool found=false;
	if (m_Mode==PLAYBACK)
	{
		for (list<RecorderMessage>::iterator i=m_EventList.begin(); i!=m_EventList.end(); i++)
		{
			if (i->Time>m_LastTimeSeconds && i->Time<=m_TimeSeconds)
			{
				events.push_back(*i);
				found=true;
			}
		}
	}
	return found;
}

void EventRecorder::Record(RecorderMessage event)
{
	if (m_Mode==RECORD)
	{
		event.Time=m_TimeSeconds;
		m_EventList.push_back(event);
	}
}

void EventRecorder::Reset()
{
	m_EventList.clear();
}

void EventRecorder::ResetClock()
{
	m_TimeSeconds=0;
	m_LastTimeSeconds=0;
}

void EventRecorder::UpdateClock()
{
	if (m_Mode == PAUSE)
		return;

	double Delta=m_Delta;
	if (Delta==0)
	{
		timeval ThisTime;
		ThisTime.tv_sec=0;
		ThisTime.tv_usec=0;
		gettimeofday(&ThisTime,NULL);
		Delta=(ThisTime.tv_sec-m_LastTime.tv_sec)+
			  (ThisTime.tv_usec-m_LastTime.tv_usec)*0.000001f;
		m_LastTime=ThisTime;
	}

	m_LastTimeSeconds=m_TimeSeconds;
	if (Delta>0 && Delta<1000) m_TimeSeconds+=Delta;

	if (m_Mode==RECORD && m_TimeSeconds>m_NextSave)
	{
		Save();
		m_NextSave=m_TimeSeconds+10;
	}
}

void EventRecorder::PauseToggle()
{
	if (m_Mode == PLAYBACK)
	{
		m_Mode = PAUSE;
	}
	else
	if (m_Mode == PAUSE)
	{
		gettimeofday(&m_LastTime,NULL);
		m_Mode = PLAYBACK;
	}
}

void EventRecorder::Save()
{
	if (m_Mode==RECORD)
	{
		ofstream os(m_Filename.c_str());
		int version = 1;
		os<<version<<endl;
		os<<m_EventList.size()<<endl;
		for (list<RecorderMessage>::iterator i=m_EventList.begin(); i!=m_EventList.end(); i++)
		{
			os<<i->Name<<" "<<i->Time<<" "<<i->Data<<" "<<i->Mod<<" "<<i->Button<<" "<<i->State<<endl;
		}

		os.close();
	}
}

void EventRecorder::Load()
{
	Reset();
	ifstream is(m_Filename.c_str());
	if (is.is_open())
	{
		int version;
		is>>version;
		unsigned int size;
		is>>size;
		unsigned int count=0;
		int temp;

		while(count<size)
		{
			RecorderMessage event;
			is>>event.Name;
			is>>event.Time;
			is>>event.Data;
			is>>event.Mod;
			is>>event.Button;
			is>>event.State;
			m_EventList.push_back(event);
			count++;
		}

		is.close();
	}
}
