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

#ifndef FLUXUS_RECORDER
#define FLUXUS_RECORDER

#include <sys/time.h>
#include <list>

using namespace std;

class RecorderMessage
{
public:
	RecorderMessage() : Time(0) {}
	RecorderMessage(const string &n, int d, int m) :
		Name(n), Time(0), Data(d), Mod(m), Button(0), State(0) {}
	RecorderMessage(const string &n, int d, int m, int b, int s) :
		Name(n), Time(0), Data(d), Mod(m), Button(b), State(s) {}

	string Name;
	double Time;
	int Data;
	int Mod;
	int Button;
	int State;
};

class EventRecorder
{
public:
	EventRecorder();
	~EventRecorder();

	enum Mode{OFF,RECORD,PLAYBACK,PAUSE};

	void SetMode(Mode mode) { m_Mode=mode; }
	Mode GetMode() { return m_Mode; }

	bool Get(list<RecorderMessage> &events);
	void Record(RecorderMessage event);

	void Reset();
	void ResetClock();
	void UpdateClock();
	void PauseToggle();
	void SetDelta(float s) { m_Delta=s; }

	void SetFilename(const string &filename) { m_Filename=filename; }

	void Save();
	void Load();

private:

	Mode m_Mode;
	timeval m_LastTime;
	double m_LastTimeSeconds;
	double m_TimeSeconds;
	double m_NextSave;
	string m_Filename;
	float m_Delta;

	list<RecorderMessage> m_EventList;
};

#endif

