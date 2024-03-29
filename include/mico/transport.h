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

#ifndef __mico_transport_h__
#define __mico_transport_h__

namespace CORBA {

/************************* ForwardDecls ******************************/


class Address;
class Dispatcher;
struct DispatcherCallback;
class Transport;
struct TransportCallback;
class TransportServer;
struct TransportServerCallback;
class Buffer;


/************************** Interfaces *******************************/


class Dispatcher {
public:
    enum Event { Timer, Read, Write, Except, All, Remove, Moved };
    virtual void rd_event (DispatcherCallback *, Long fd) = 0;
    virtual void wr_event (DispatcherCallback *, Long fd) = 0;
    virtual void ex_event (DispatcherCallback *, Long fd) = 0;
    virtual void tm_event (DispatcherCallback *, ULong tmout) = 0;
    virtual void remove (DispatcherCallback *, Event) = 0;
    virtual void run (Boolean infinite = TRUE, CORBA::Boolean _runShm = FALSE) = 0;
    virtual void move (Dispatcher *) = 0;
    virtual Boolean idle () const = 0;

    virtual ~Dispatcher ();

    virtual void block (CORBA::Boolean b)
    { /* we can forget value here, since we'll always return FALSE */ }
    virtual Boolean isblocking ()
    { return FALSE; }
};

class DispatcherFactory
{
public:
    virtual
    ~DispatcherFactory()
    {}

    virtual Dispatcher*
    create() = 0;
};

struct DispatcherCallback {
    typedef Dispatcher::Event Event;
    virtual void callback (Dispatcher *, Event) = 0;
    virtual ~DispatcherCallback ();
};

class Timeout : public DispatcherCallback {
    Boolean _ready;
    Dispatcher *_disp;
    Boolean _have_tmout;
public:
    Timeout (Dispatcher *d, Long tm);
    ~Timeout ();
    void callback (Dispatcher *, Event);
    Boolean done () const
    {
	return _ready;
    }
};


class Transport {
public:
    enum State { Closed, Open };

    virtual void rselect (Dispatcher *, TransportCallback *) = 0;
    virtual void wselect (Dispatcher *, TransportCallback *) = 0;

    virtual Boolean bind (const Address *) = 0;
    virtual Boolean connect (const Address *, ULong timeout, Boolean& timedout) = 0;
    virtual void open (CORBA::Long fd = -1) = 0;
    virtual void open_sem (std::string semName) = 0;
    virtual void post () = 0;
    virtual void wait () = 0;
    virtual int get_sem_value () = 0;
    virtual void close () = 0;
    virtual void block (Boolean doblock = TRUE) = 0;
    virtual CORBA::Boolean isblocking () = 0;
    virtual void buffering (Boolean dobuffering = TRUE);
    virtual CORBA::Boolean isbuffering ();
    virtual CORBA::Boolean isreadable () = 0;

    Long read (Buffer &, Long len, CORBA::Boolean _msgRecv);
    virtual Long read (void *, Long len, CORBA::Boolean _msgRecv) = 0;
    Long write (Buffer &, Long len, Boolean eat = TRUE);
    virtual Long write (const void *, Long len) = 0;

    virtual const Address *addr () = 0;
    virtual const Address *peer () = 0;

    virtual Boolean eof () const = 0;
    virtual Boolean bad () const = 0;
    virtual std::string errormsg () const = 0;

    virtual CORBA::Principal_ptr get_principal ();

    virtual ~Transport ();
};

struct TransportCallback {
    enum Event { Read, Write, Remove };
    virtual void callback (Transport *, Event) = 0;
    virtual ~TransportCallback ();
};


class TransportServer {
public:
    virtual int get_sem_value () = 0;
    virtual CORBA::Boolean open_shm () = 0;

    virtual void aselect (Dispatcher *, TransportServerCallback *) = 0;

    virtual Boolean bind (const Address *) = 0;
    virtual void close () = 0;
    virtual void block (Boolean doblock = TRUE) = 0;
    virtual CORBA::Boolean isblocking () const = 0;

    virtual Transport *accept () = 0;
    virtual const Address *addr () = 0;

    virtual Boolean bad () const = 0;
    virtual std::string errormsg () const = 0;

#ifdef HAVE_THREADS
    virtual void create_thread() = 0;
    virtual void start() = 0;
#endif

    virtual ~TransportServer ();
};

struct TransportServerCallback {
    enum Event { Accept, Remove };
    virtual void callback (TransportServer *, Event) = 0;
    virtual ~TransportServerCallback ();
};

}

#endif // __mico_transport_h__
