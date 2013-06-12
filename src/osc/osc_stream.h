/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef osc_stream__
#define osc_stream__

#include <string>
#include <vector>

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

namespace json
{

#define kLocalhost	0x7f000001

//--------------------------------------------------------------------------
/*!
	\brief	OSC output streams

	An osc output stream embed a destination address and udp port that can be dynamically changed.
	
	
*/
class osc_stream
{
	enum		{ kOutBufferSize = 16384, kDefaultPort=1024 };
	
	int			fState;
	int			fPort;				// the destination UDP port
	unsigned long fAddress;			// the destination IP address
	char		fBuffer[kOutBufferSize];

	osc::OutboundPacketStream	fOutStream;
	UdpSocket					fSocket;
	
	public:
		enum state	{ kIdle, kInProgress };

				 osc_stream()
					: fState(kIdle), fPort(kDefaultPort), fAddress(kLocalhost), fOutStream(fBuffer, kOutBufferSize) {}
				 osc_stream(const char* address, int port=kDefaultPort)
					: fState(kIdle), fPort(port), fAddress(kLocalhost), fOutStream(fBuffer, kOutBufferSize) {}
		virtual ~osc_stream() {}
		
		/// \brief gives the underlying stream
		osc::OutboundPacketStream&	stream()					{ return fOutStream; }
		/// \brief gives the stream current state (idle or message in progress)
		int							state()	const				{ return fState; }
		
		/// \brief starts a new osc message which osc address is \c address
		osc_stream&			start(const char * address);
		/// \brief closes the current osc message and send it to the current address on the current udp port
		osc_stream&			end();

		// address and udp port management
		int				getPort () const						{ return fPort; }
		unsigned long	getAddress () const						{ return fAddress; }
		void			setPort (int port)						{ fPort = port; }
		void			setAddress (unsigned long address)		{ fAddress = address; }
		void			setAddress (const std::string& address);
};

/// \private
inline	osc_stream& operator <<(osc_stream& s, const char* val)			{ s.stream() << val; return s; }
/// \private
inline	osc_stream& operator <<(osc_stream& s, const std::string& val)	{ s.stream() << val.c_str(); return s; }
/// \private
inline	osc_stream& operator <<(osc_stream& s, int val)					{ s.stream() << val; return s; }
/// \private
inline	osc_stream& operator <<(osc_stream& s, long val)				{ s.stream() << (int)val; return s; }
/// \private
inline	osc_stream& operator <<(osc_stream& s, float val)				{ s.stream() << val; return s; }
/// \private
inline	osc_stream& operator <<(osc_stream& s, double val)				{ s.stream() << float(val); return s; }

} // end namespace

#endif
