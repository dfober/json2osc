/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include <stdexcept>
#include <sstream>
#include <iostream>
#include <fstream>

#include "json_osc.h"
#include "json_parser.h"
#include "json_object.h"

#include "osc_listener.h"
#include "osc_stream.h"

using namespace std;

namespace json
{

// --------------------------------------------------------------
int start_osc2json	(osc2json* client, int port, osc_listener** listener)
{
	try {
		*listener = new osc_listener (client, port);
		(*listener)->run();
	}
	catch (std::runtime_error e) {
		return kCantBindSocket;
	}
	return kNoError;
}

// --------------------------------------------------------------
void stop_osc2json	(osc_listener* listener)
{
	delete listener;
}

// --------------------------------------------------------------
osc_stream* start_json2osc	(const char* defaultDest, int defaultPort)
{
	return new	osc_stream (defaultDest, defaultPort);
}

// --------------------------------------------------------------
static int sendstream (istream* stream, osc_stream* osc)
{
	json_parser p (stream);
	json_object* obj = p.parse();
	if (obj) obj->print(*osc);
	else return kSyntaxError;
	return kNoError;
}

// --------------------------------------------------------------
int sendfile (const char * file, osc_stream* osc)
{
	if (!file || !osc) return kIncorrectParameter;
	fstream stream (file);
	if (stream.is_open())
		return sendstream (&stream, osc);
	else return kNoSuchFile;
}

// --------------------------------------------------------------
int send (const char * json, osc_stream* osc)
{
	if (!json || !osc) return kIncorrectParameter;
	stringstream stream (json);
	return sendstream (&stream, osc);
}

// --------------------------------------------------------------
void stop_json2osc	(osc_stream* stream)
{
	delete stream;
}

// --------------------------------------------------------------
const char*	error_string	(int error)
{
	switch (error) {
		case kNoError:				return "no error";
			break;
		case kSyntaxError:			return "syntax error";
			break;
		case kIncorrectParameter:	return "incorrect parameter";
			break;
		case kNoSuchFile:			return "can't open file";
			break;
		case kCantBindSocket:		return "can't bind udp socket";
			break;
		default:					return "unknown error";
	}
}

} // end namespoace
