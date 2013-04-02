/*
	==============================================================================
	This file is part of Tal-NoiseMaker by Patrick Kunz.

	Copyright(c) 2005-2010 Patrick Kunz, TAL
	Togu Audio Line, Inc.
	http://kunz.corrupt.ch

	This file may be licensed under the terms of of the
	GNU General Public License Version 2 (the ``GPL'').

	Software distributed under the License is distributed
	on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
	express or implied. See the GPL for the specific language
	governing rights and limitations.

	You should have received a copy of the GPL along with this
	program. If not, go to http://www.gnu.org/licenses/gpl.html
	or write to the Free Software Foundation, Inc.,  
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
	==============================================================================
 */

#ifndef OscPulse_H
#define OscPulse_H

#include "BlepData.h"

class OscPulse
{
public:
	const float *minBlep;
	const float sampleRate, sampleRateInv, oversampling;
	const int n;

	float *buffer;
	float x;
	float sign;
	int bufferPos;
	bool phaseReset;

	// Locals
	float pwOffset, value, frac;
	float tmp;

	float phaseFM;
	float pi;
	float pi2;

	float freq;

	OscPulse(float sampleRate, int oversampling):
		sampleRate(sampleRate),
		sampleRateInv(1.0f / sampleRate),
		oversampling(64.0f / oversampling),
		n(32 * (int)oversampling)
	{
		buffer= new float[n];
		BlepData *blepData= new BlepData();
		minBlep= blepData->getBlep();
        delete blepData;

		resetOsc(0.0f);

		pi= 3.1415926535897932384626433832795f;
		pi2= 2.0f * pi;
	
		this->freq = 0.0f;
	}

	~OscPulse() 
	{
		delete[] buffer;
	}

	void resetOsc(float phase) 
	{
		x = phase;
		phaseFM = 0.0f;
		bufferPos = 0;
		for (int i = 0; i < n; i++) 
        {
			buffer[i] = 0.0f;
		}

		sign= 1.0f;
	}

	inline float getNextSample(float freq, float pw, float fm, float fmFreq, bool reset, float resetFrac, float masterFreq) 
	{
		this->freq = freq;
		phaseReset = false;
		if (fm > 0.0f) 
		{
			phaseFM += fmFreq / sampleRate;
			freq +=  fm * 10.0f * fmFreq * (1.0f + sinf(phaseFM * pi2));
			if (phaseFM > 1.0f) phaseFM -= 1.0f;
		}
		if (freq > 22000.0f) freq = 22000.0f;

		float fs = freq * sampleRateInv;
		if (reset)
		{
			tmp =  masterFreq/sampleRate;
			if (sign == -1.0f)
			{
				sign = 1.0f;
				mixInBlep(resetFrac / tmp);
			}
			x = (fs * resetFrac) / tmp;
		}
		x += fs;
		if (x >= 1.0f && sign == -1.0f) 
		{
			x -= 1.0f;
			sign = 1.0f;
			mixInBlep(x/fs);
			phaseReset = true;
		}
		pw = ((pw - 0.5f) * (1.0f - fs * 2.5f)) + 0.5f;
		if (x >= pw && sign == 1.0f) 
		{
			sign = -1.0f;
			tmp = (x - pw)/fs;
			if (tmp > 1.0f) tmp = 1.0f;
			mixInBlep(tmp);
		}
		return getNextBlep() - sign * 0.5f;
	}

	inline float getNextBlep() 
	{
		buffer[bufferPos]= 0.0f;
		bufferPos++;

		// Wrap pos
		if (bufferPos>=n) 
		{
			bufferPos-= n;
		}
		value= buffer[bufferPos];
		return value;
	}

	#define LERP(A,B,F) ((B-A)*F+A)
	inline void mixInBlep(float offset) 
	{
		int lpIn = (int)(oversampling * offset);
		float frac = fmod(offset * oversampling, 1.0f);
		for (int i = 0; i < n; i++, lpIn += (int)oversampling) 
		{
			buffer[(bufferPos + i)&(n-1)] += (1.0f - LERP(minBlep[lpIn], minBlep[lpIn+1], frac)) * sign;
		}
	}
};
#endif