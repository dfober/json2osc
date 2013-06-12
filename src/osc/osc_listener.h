/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef osc_listener__
#define osc_listener__

#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"

namespace json
{

class osc2json;

//--------------------------------------------------------------------------
/*!
	\brief an OSC listener that converts OSC input to json strings
*/
class osc_listener : public osc::OscPacketListener
{
	UdpListeningReceiveSocket	fSocket;	///< the udp socket listener
	osc2json*					fClient;	///< the listener client: converts OSC packets to json
	int							fPort;		///< the udp port number

	public:
				 osc_listener(osc2json* client, int port);
		virtual ~osc_listener();

		/*!
			\brief process OSC messages

			\param m the OSC message (pre-processed by the base class)
			\param remoteEndpoint the sender IP address
		*/
		virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );
		virtual void run();
		virtual void stop(); 
};


} // end namespoace

#endif
