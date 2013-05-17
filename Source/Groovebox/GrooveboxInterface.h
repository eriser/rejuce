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

class GrooveboxInterface : public GrooveEventListener
{
public:

	virtual void start() =0;
	virtual void stop() =0;

	void setOutListener(GrooveEventListener* pOutListener);

protected:
	GrooveEventListener* _pOutListener;

};


#endif /* WEBINTERFACE_H_ */
