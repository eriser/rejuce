/*
 * MidiSequence.h
 *
 *  Created on: 10 Apr 2013
 *      Author: gboxdev
 */

#ifndef MIDISEQUENCE_H_
#define MIDISEQUENCE_H_

#include "../../JuceLibraryCode/JuceHeader.h"

class MidiSequence {
public:
	MidiSequence();
	virtual ~MidiSequence();

	void reset(int length);

	void addEvent(int pos,int rawMidi);
	int getEventCountAt(int pos);
	int getEventAt(int pos,int which);


private:
	Array<int>* _events;
	int _length;
};

#endif /* MIDISEQUENCE_H_ */
