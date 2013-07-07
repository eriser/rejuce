/*
 * ScreenManager.cpp
 *
 *  Created on: 5 Jul 2013
 *      Author: slippy
 */

#include "ScreenManager.h"

struct Screen
{
	String name;
	GrooveControlName controls[];
};

ScreenManager::ScreenManager()
{
	GrooveControlName screenPlaybackControls[] = {
		GCP_REC_METSTATE,
		GCP_REC_METRONOME,
		GCP_REC_AUTOQUANT,
		GC_INVALID,GC_INVALID
	};
	addScreen("playback",screenPlaybackControls);

	GrooveControlName screenPhraseControls[] = {
		GCP_PHRASE_LENGTH,
		GCP_PHASE_QUANTISENUM,
		GCP_PHASE_QUANTISEDIV,
		GCB_PHASE_QUANTISE,
		GC_INVALID,GC_INVALID
	};
	addScreen("phrase",screenPlaybackControls);


	_currentScreen = 0;
	_currentPage = 0;
	_currentScreenName = _screenNames[_currentScreen];
}

ScreenManager::~ScreenManager()
{

}

void ScreenManager::addScreen(String name,GrooveControlName controls[])
{
	_screenNames.add(name);
	Array<Array<GrooveControlName> > screen;
	_screenMap.set(name,screen);

	int nControl=0;

	while (controls[nControl]!= GC_INVALID)
	{
		Array <GrooveControlName> page;
		page.add((GrooveControlName)0);	// <-- holds the current selected control

		while (controls[nControl]!= GC_INVALID)
		{
			page.add(controls[nControl]);

			nControl++;
		}

		_screenMap[name].add(page);
	}
}

void ScreenManager::setCurrentScreen(int screen)
{

}

int ScreenManager::getCurrentScreen()
{

}

String ScreenManager::getCurrentScreenName()
{

}

void ScreenManager::setCurrentPage(int page)
{
	if (page>=0 && page<_screenMap)

	_currentPage = page;
}

int ScreenManager::getCurrentPage()
{
	return _currentPage;
}
