/*
 * Song.h
 *
 *  Created on: 12 Apr 2013
 *      Author: gboxdev
 */

#ifndef SONG_H_
#define SONG_H_

#include "Section.h"
#include "../Host/HostEventListener.h"
#include "../Host/HostEvent.h"

enum SongState
{
	SONG_STOPPED =0,
	SONG_PAUSED,
	SONG_PLAYING,
	SONG_RECORDING,
};

enum MetronomeState
{
	METRONOME_OFF =0,
	METRONOME_RECORD,
	METRONOME_PLAY,
};

class Song {
public:

	Song();
	void init(HostEventListener* pHostEventListener);
	virtual ~Song();

	// live operations
	void play();
	void record();
	void stop();
	void pause();

	void setNextSection(int i);
	int getCurrentSection();
	int getNextSection();
	Section* getSection(int i);
	Section* getCurrentSectionPointer();

	void togglePhraseMute(int i);
	bool getPhraseMuteState(int i);
	void setMetronomeState(MetronomeState s);
	void setMetronomeBars(int bars);

	void setBpm(float bpm);
	float getBpm();

	int tick(MidiMessageCollector* pCollector);

	void addEvent(MidiMessage m); // new events merged on next Play or Stop
	void clear();

	int getCurrentPatternLengthClocks();

	void setQuantiseNumerator(int num);
	void setQuantiseDivisor(int div);
	void setAutoQuant(bool quant);
	void quantisePhrase();
	void setCurrentPhrase(int phrase);

private:
	Section* _sections[16];
	int _currentSection;
	Section* _pCurrentSection;
	int _nextSection;
	int _clock;
	float _bpm;
	int _currentPhrase;

	SongState _state;
	MetronomeState _metronomeState;
	int _metronomeBars;

	HostEventListener* _pHostEventListener;

	int _currentPatternLengthClocks;
	int _countInClockPos;
	int _countInClockMax;

	int _quantiseNumerator;
	int _quantiseDenominator;
	bool _autoQuant;
};

#endif /* SONG_H_ */
