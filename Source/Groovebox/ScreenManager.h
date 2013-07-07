/*
 * ScreenManager.h
 *
 *  Created on: 5 Jul 2013
 *      Author: slippy
 */

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include "../../JuceLibraryCode/JuceHeader.h"
#include "GrooveEvent.h"

/**
 * The screenmanager holds a number of screens and manages the current screen
 * A screen is an array of pages
 * A page is an array of controls
 */
class ScreenManager {
public:
	ScreenManager();
	virtual ~ScreenManager();

	void setCurrentScreen(int screen);
	int getCurrentScreen();
	String getCurrentScreenName();

	void setCurrentPage(int page);
	int getCurrentPage();


private:
	void addScreen(String name,GrooveControlName controls[]);

	int _currentScreen;
	int _currentScreenName;
	int _currentPage;

	// a map of arrays of parameters
	HashMap <String,Array<Array<GrooveControlName> > > _screenMap;
	Array <String> _screenNames;
};

#endif /* SCREENMANAGER_H_ */
