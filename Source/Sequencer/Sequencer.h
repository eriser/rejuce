/*
 * gbseq.h
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#ifndef GBSEQ_H_
#define GBSEQ_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#include "Phrase.h"
#include "Pattern.h"
#include "Song.h"
#include "SequencerCommand.h"

class Sequencer {
public:
	Sequencer(MidiMessageCollector* collector);
	virtual ~Sequencer();

	void start();
	void stop();

	bool command(SequencerCommand c);

private:
	class CommandQueue
	{
	public:
		CommandQueue()  : abstractFifo (1024){}
	    void addToFifo (SequencerCommand& c)
	    {
	        int start1, size1, start2, size2;
	        const char* data = c.getRaw();
	        abstractFifo.prepareToWrite (16*sizeof(int), start1, size1, start2, size2);
	        if (size1 > 0)
	            memcpy(myBuffer + start1, data, size1);
	        if (size2 > 0)
	            memcpy(myBuffer + start2, data + size1, size2);
	        abstractFifo.finishedWrite (size1 + size2);
	    }
	    SequencerCommand readFromFifo ()
	    {
	        int start1, size1, start2, size2;
	        char data[16];
	        abstractFifo.prepareToRead (16*sizeof(int), start1, size1, start2, size2);
	        if (size1 > 0)
	            memcpy (data, myBuffer + start1, size1);
	        if (size2 > 0)
	        	memcpy (data + size1, myBuffer + start2, size2);
	        abstractFifo.finishedRead (size1 + size2);

	        SequencerCommand c(data);
	        return c;
	    }
	private:
	    AbstractFifo abstractFifo;
	    int myBuffer [1024];
	};

private:
	int tick();

private:
	MidiMessageCollector* _pMessageCollector;
	float _bpm;

	Song _song;
};

Sequencer* g_sequencer = nullptr;

Sequencer* getSequencerInstance(MidiMessageCollector* collector)
{
	if (!g_sequencer)
	{
		g_sequencer = new Sequencer(collector);
		g_sequencer->start();
	}

	return g_sequencer;
}

void freeSequencer()
{
	g_sequencer->stop();
	delete g_sequencer;
	g_sequencer=nullptr;
}


#endif /* GBSEQ_H*_ */
