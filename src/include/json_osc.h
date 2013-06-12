/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/*! \mainpage JSON to OSC Gateway
 
\section intro_sec Introduction
 
The JSON to OSC gateway is a library intended to facilitate the communication between the HTTP and UDP worlds.
The gateway provides JSON conversion to OSC and the reverse.
 
\section json_sec Encoding OSC in JSON
OSC is encoded in JSON in a very simple and straighforward way: \n
- an OSC message is encoded as a JSON object
- the OSC address is stored in an element named \c "OSCAddress"
- the OSC parameters are stored as a JSON array in an element named \c "OSCParams"

Thus the \c "OSCAddress" and \c "OSCParams" fields are required. 
JSON objects that don't contain these fields are ignored by the gateway.

\b Example:
\code
{
	"OSCAddress": "/my/osc/target",
	"OSCParams": [ 0, 1, 2, 3, 4, 5 ]
}
\endcode

\section json_opt Optional JSON fields

The gateway supports the following optional fields: \n
- \c "OSCSender" : set by the OSC to JSON encoder. It contains the sender ip address.
- \c "OSCDest" : used to specify a destination host. The field supports host name or ip number encoded as strings
  or ip numbers as long values.
- \c "OSCPort" : used to specify a destination udp port number.

\note all the other fields are ignored by the gateway.

\b Example:
\code
{
	"OSCAddress": "/my/osc/target",
	"OSCParams": [ 0, 1, 2, 3, 4, 5 ]
	"OSCDest": "localhost",
	"OSCPort": 8000,
	"comment": "a comment that is ignored by the gateway"
}
\endcode


\section json_mult JSON bundles

Multiple messages can be included in a single JSON : they should be encoded using a field named \c "OSCBundle" which value must be an array of JSON objects. 

\note Array of JSON objects stored under a different name will be ignored by the gateway.

\b Example:
\code
"OSCBundle": [
	{
		"OSCAddress": "/my/osc/target1",
		"OSCParams": [ 0, 1, 2, 3, 4, 5 ]
	},
	{
		"OSCAddress": "/my/osc/target2",
		"OSCParams": "start"
	}
]	
\endcode


\section hist Change log
<b>Version 0.50</b>
- first version

*/


#ifndef json_osc__
#define json_osc__

#include "osc2json.h"

namespace json
{
class osc_listener;
class osc_stream;

enum { kNoError, kSyntaxError, kIncorrectParameter, kNoSuchFile, kCantBindSocket };

/**
	\brief starts osc to json conversion
	\param client an object that will be called for each osc message with the corresponding json convertion as argument
	\param port the udp port number to listen to
	\param listener a pointer that will be set to an opaque osc_listener object
	\return an error code in case of failure
	\note The function does not return since it listens to a socket. 
	Thus the caller is responsible of calling the function in a separate thread when appropriate.
*/
int			start_osc2json	(osc2json* client, int port, osc_listener** listener);

/**
	\brief stops osc to json conversion
	\param listener an osc listener that has been previously returned by start_osc2json
*/
void			stop_osc2json	(osc_listener* listener);

/**
	\brief starts json to osc conversion
	\param defaultDest the default destination (host name or ip number), used when no destination is specified by json
	\param defaultPort the default udp port number, used when no port is specified by json
	\return a pointer to an opaque osc_stream object
	\note sending a json string including a destination and/or a udp port changes 
	the default destination and/or default udp port number.
*/
osc_stream*		start_json2osc	(const char* defaultDest, int defaultPort);

/**
	\brief sends a json string as osc packets
	\param json the json code 
	\param stream an osc stream that has been previously returned by start_json2osc
	\return an error code or kNoError when no error
*/
int				send			(const char * json, osc_stream* stream);

/**
	\brief sends a json file as osc packets
	\param file the file path name 
	\param stream an osc stream that has been previously returned by start_json2osc
	\return an error code or kNoError when no error
*/
int				sendfile		(const char * file, osc_stream* stream);

/**
	\brief stops json to osc conversion
	\param stream an osc stream that has been previously returned by start_json2osc
*/
void			stop_json2osc	(osc_stream* stream);

/**
	\brief gives a textual description of an error
	\param error an error code 
	\return a string
*/
const char*		error_string	(int error);


} // end namespoace

#endif
