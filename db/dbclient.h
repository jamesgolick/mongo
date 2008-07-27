// dbclient.h - connect to a Mongo database as a client, from C++

/**
*    Copyright (C) 2008 10gen Inc.
*  
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*  
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*  
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../grid/message.h"

class JSObj;

#pragma pack(push)
#pragma pack(1)
struct QueryResult : public MsgData {
	long long cursorId;
	int startingFrom;
	int nReturned;
	const char *data() { return (char *) (((int *)&nReturned)+1); }
};
#pragma pack(pop)

class DBClientCursor : boost::noncopyable { 
	friend class DBClientConnection;
	MessagingPort& p;
	long long cursorId;
	int nReturned;
	int pos;
	const char *data;
	auto_ptr<Message> m;
	string ns;
	int nToReturn;
	void dataReceived();
	void requestMore();
public:
	DBClientCursor(MessagingPort& _p, auto_ptr<Message> _m) : 
	  p(_p), m(_m) { dataReceived(); }
	
	bool more();
	JSObj next();
};

class DBClientConnection : boost::noncopyable { 
	MessagingPort p;
	auto_ptr<SockAddr> server;
public:
	DBClientConnection() { }
	bool connect(const char *serverHostname, string& errmsg);

	/* send a query to the database.
       ns: namespace to query, format is <dbname>.<collectname>[.<collectname>]*
       query: query to perform on the collection.  this is a JSObj (binary JSON)
	   nToReturn: n to return.  0 = unlimited
       nToSkip: start with the nth item
	   fieldsToReturn: optional template of which fields to select. if unspecified, returns all fields

	   returns: cursor.
                returns 0 if error
	*/
	auto_ptr<DBClientCursor> query(const char *ns, JSObj query, int nToReturn = 0, int nToSkip = 0, JSObj *fieldsToReturn = 0);
};


