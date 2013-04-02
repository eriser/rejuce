/*
	==============================================================================
	This file is part of Tal-Reverb by Patrick Kunz.

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

#ifndef ProgramChunk_H
#define ProgramChunk_H

#include "includes.h"

// XML in bytes, because of of char* size limit
// Dont forget to update size in "createStringFromData"
const unsigned char chunk[]= {
};

class ProgramChunk
{
public:
	ProgramChunk()  
	{
	}

	~ProgramChunk()  
	{
	}

	const String getXmlChunk() 
	{
	    const char* const data = static_cast <const char*> ((const void* const)chunk);

        //File *file = new File("c:/presets.txt");
        //file->replaceWithText(String::fromUTF8 (data, 171085 + 1));
        
        int sizeInBytes = 643983 + 1;
		const int stringLength = (int)ByteOrder::littleEndianInt (((const char*) data) + 4);

		// XmlDocument doc (String::fromUTF8 (((const char*) data) + 8, jmin ((sizeInBytes - 8), stringLength)));
        return String::fromUTF8(data, sizeInBytes);
        //return String::fromUTF8 (data, 171085 + 1);
	}
	private:
};
#endif