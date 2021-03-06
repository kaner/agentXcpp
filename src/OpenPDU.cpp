/*
 * Copyright 2011 Tanjeff-Nicolai Moos <tanjeff@cccmz.de>
 *
 * This file is part of the agentXcpp library.
 *
 * AgentXcpp is free software: you can redistribute it and/or modify
 * it under the terms of the AgentXcpp library license, version 1, which 
 * consists of the GNU General Public License and some additional 
 * permissions.
 *
 * AgentXcpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See the AgentXcpp library license in the LICENSE file of this package 
 * for more details.
 */

#include "OpenPDU.h"


using namespace agentx;

OpenPDU::OpenPDU(data_t::const_iterator& pos, bool big_endian)
    : PDU(pos, big_endian)
{
    // header is parsed by base class constructor
    // so we continue with timeout field:
    timeout = *pos++;
    pos += 3;	// skip reserved fields

    id = oid(pos, big_endian);

    descr = Octet_String(pos, big_endian);
}
	    

OpenPDU::OpenPDU(Octet_String _descr,
		 byte_t _timeout,
		 oid _id)
{
    timeout = _timeout;
    id = _id;
    descr = _descr;
}


data_t OpenPDU::serialize()
{
    data_t serialized;

    // timeout and reserved fields
    serialized.push_back(timeout);
    serialized.push_back(0);
    serialized.push_back(0);
    serialized.push_back(0);

    // id
    serialized += id.serialize();

    // descr
    serialized += descr.serialize();

    // Add header (type for OpenPDU is 1)
    add_header(1, serialized);

    // return serialized form of PDU
    return serialized;
}
