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

	int nControl = 0;
	int nPage =0;

	Array<Array<GrooveControlName> > screen;

	// loop over pages
	while (controls[nControl]!= GC_INVALID)
	{
		Array<GrooveControlName> page;

		// loop over controls
		while (controls[nControl]!= GC_INVALID)
		{
			page.add(controls[nControl]);
			nControl++;
		}

		screen.add(page);
		nPage++;
	}

	_screenMap.set(name,screen);
}

int ScreenManager::getScreenCount()
{
	int count = _screenNames.size();
	return count;
}

void ScreenManager::setCurrentScreen(int screen)
{
	if (screen >= 0 && screen <getScreenCount())
	{
		_currentPage = 0;
		_currentScreen = screen;
		_currentScreenName = _screenNames[screen];
	}
}

int ScreenManager::getCurrentScreen()
{
	return _currentScreen;
}

String ScreenManager::getCurrentScreenName()
{
	return _screenNames[_currentScreen];
}

void ScreenManager::setCurrentPage(int page)
{
	if (page>=0 && page<getPageCount())
	{
		_currentPage = page;
	}
}

int ScreenManager::getCurrentPage()
{
	return _currentPage;
}

int ScreenManager::getPageCount()
{
	int count = _screenMap[_currentScreenName].size();
	return count;
}

int ScreenManager::getControlCount()
{
	int count = _screenMap[_currentScreenName][_currentPage].size();
	return count;
}

GrooveControlName ScreenManager::getControl(int c)
{
	if (c>=0 && c<=getControlCount())
	{
		return _screenMap[_currentScreenName][_currentPage][c];
	}
	else return GC_INVALID;
}
