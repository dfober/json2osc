/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef osc2json__
#define osc2json__


namespace json
{

//--------------------------------------------------------------------------
/*!
	\brief an OSC listener client that receive json translations
*/
class osc2json
{
	public:
		virtual ~osc2json() {}
		
		/*!
			\brief a method called by OSCListener
			\param json a JSON string
		*/
		virtual void received (const char* json) = 0;
};

} // end namespoace

#endif
