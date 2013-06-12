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
#include "osc_stream.h"

using namespace std;

namespace json
{

//--------------------------------------------------------------------------
void osc_stream::setAddress (const string& address)
{
	IpEndpointName dst (address.c_str());
	setAddress (dst.address);
}

//--------------------------------------------------------------------------
osc_stream& osc_stream::start(const char * address)
{ 
	stream().Clear();
	if (!stream().IsReady()) cerr << "osc_stream OutboundPacketStream not ready" << endl;
	stream() << osc::BeginMessage( address ) ; 
	fState = kInProgress;
	return *this;
}

//--------------------------------------------------------------------------
osc_stream& osc_stream::end()
{
	if (state() == kInProgress) {
		stream() << osc::EndMessage;
		fSocket.SendTo (IpEndpointName (fAddress, fPort), stream().Data(), stream().Size() );
		fState = kIdle;
	}
	return *this;
}


} // end namespace

