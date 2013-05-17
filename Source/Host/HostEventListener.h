/*
 * GrooveEventListener.h
 *
 *  Created on: 17 May 2013
 *      Author: gnuvebox
 */

#ifndef HOSTEVENTLISTENER_H_
#define HOSTEVENTLISTENER_H_

#include "HostEvent.h"

class HostEventListener {
public:

	virtual void onHostEvent(HostEvent& event) =0;
};

#endif /* GROOVEEVENTLISTENER_H_ */
