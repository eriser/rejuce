/*
 * WebInterface.h
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#ifndef GBINTERFACE_H_
#define GBINTERFACE_H_

#include "GrooveEvent.h"
#include "GrooveEventListener.h"
#include "ScreenManager.h"

class GrooveboxInterface : public GrooveEventListener
{
public:

	virtual void start() =0;
	virtual void stop() =0;

	virtual void setOutListener(GrooveEventListener* pOutListener);
	virtual void setScreenManager(ScreenManager* pScreenManager);

protected:
	GrooveEventListener* _pOutListener;
	ScreenManager* _screenManager;

};


#endif /* WEBINTERFACE_H_ */
