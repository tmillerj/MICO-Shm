// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2007 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __mico_transport_unix_h__
#define __mico_transport_unix_h__

namespace MICO {

#ifdef HAVE_SYS_UN_H
class UnixTransport : public SocketTransport {
    UnixAddress local_addr, peer_addr;
public:
    CORBA::Boolean bind (const CORBA::Address *);
    CORBA::Boolean connect (const CORBA::Address *, CORBA::ULong, CORBA::Boolean&);

    void open (CORBA::Long fd = -1);
    void close ();

    CORBA::Long read (void *, CORBA::Long len, CORBA::Boolean _msgRecv);
    CORBA::Long write (const void *, CORBA::Long len);

    const CORBA::Address *addr ();
    const CORBA::Address *peer ();
};

class UnixTransportServer : public SocketTransportServer {

    UnixAddress local_addr;

public:
    UnixTransportServer ();

    CORBA::Boolean bind (const CORBA::Address *);
    void close ();

    CORBA::Transport *accept ();
    const CORBA::Address *addr ();
    void listen ();
};
#endif // HAVE_SYS_UN_H

}

#endif // __mico_transport_unix_h__
