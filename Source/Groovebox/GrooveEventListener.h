/*
 * GrooveEventListener.h
 *
 *  Created on: 17 May 2013
 *      Author: gnuvebox
 */

#ifndef GROOVEEVENTLISTENER_H_
#define GROOVEEVENTLISTENER_H_

#include "GrooveEvent.h"

class GrooveEventListener {
public:

	virtual void onGrooveEvent(GrooveEvent& event) =0;
};

#endif /* GROOVEEVENTLISTENER_H_ */
