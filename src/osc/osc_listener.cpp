/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <sstream>

#include "osc_listener.h"
#include "json_object.h"
#include "json_array.h"
#include "osc2json.h"
#include "json2osc.h"

using namespace std;
using namespace osc;

namespace json
{

//--------------------------------------------------------------------------
osc_listener::osc_listener(osc2json* client, int port)
		: fSocket(IpEndpointName( IpEndpointName::ANY_ADDRESS, port ), this), fClient(client), fPort(port) {}
osc_listener::~osc_listener()	{ stop(); }

//--------------------------------------------------------------------------
void osc_listener::run()
{ 
	try {
		fSocket.Run(); 
	}
	catch (osc::Exception e) {
		cerr << "osc error: " << e.what() << endl;
	}
}

//--------------------------------------------------------------------------
void osc_listener::stop()
{ 
	fSocket.AsynchronousBreak();
}

//--------------------------------------------------------------------------
void osc_listener::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& src )
{
	json_object* osc = new json_object();
	osc->add (new json_element (kSenderField, new json_int_value(src.address)));
	osc->add (new json_element (kPortField, new json_int_value(fPort)));
	osc->add (new json_element (kAddressField, new json_string_value(m.AddressPattern())));
	
	json_array* params = new json_array();
	ReceivedMessageArgumentIterator i = m.ArgumentsBegin();
	while (i != m.ArgumentsEnd()) {
		if (i->IsString()) {
			params->add(new json_string_value(i->AsStringUnchecked()));
		}
		else if (i->IsInt32()) {
			params->add(new json_int_value(i->AsInt32Unchecked()));
		}
		else if (i->IsFloat()) {
			params->add(new json_float_value(i->AsFloatUnchecked()));
		}
		i++;
	}
	osc->add (new json_element (kParamsField, new json_array_value(params)));

	json_array* oscs = new json_array();
	oscs->add(new json_object_value(osc));

	json_object json;
	json.add (new json_element (kOSCMarker, new json_array_value(oscs)));
	
	stringstream stream;
	stream << json;
	fClient->received (stream.str().c_str());
}

} // end namespoace
