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

#ifndef __PDU_H__
#define __PDU_H__

#include "types.h"
#include "exceptions.h"
#include "Octet_String.h"
#include <boost/asio.hpp>

namespace agentx
{
    /**
     * \internal
     *
     * \brief The base class of all PDU's
     *
     * This class is never instantiated itself, but serves as the base class 
     * for all concrete %PDU classes (e.g.  OpenPDU). It contains data and 
     * functionality common to all %PDU types and can be said to represent the 
     * %PDU header. Note, however, that in terms of RFC2741 it does not exactly 
     * represent a %PDU header; for example it takes care of the context field 
     * of the %PDU's, which is not part of the header (but is common to all 
     * %PDU types).
     *
     * This class also provides an automatic packetID generator. Whenever a PDU 
     * object is created, the packetID is automatically incremented. The two 
     * exceptions are:
     * - The parse contructor fills the packetID from the received %PDU
     * - The ResponsePDU class overwrites the packetID
     * The mechanism uses a static class member packetID_cnt to store the last 
     * used packetID. The counter wraps at its limit.
     */
    class PDU
    {
	private:
	    // header flags
	    bool instance_registration;
	    bool new_index;
	    bool any_index;
	    
	    /**
	     * \brief h.sessionID field
	     *
	     * According to RFC 2741, 6.1. "AgentX PDU Header"
	     */
	    uint32_t sessionID;
	    
	    /**
	     * \brief h.transactionID field
	     *
	     * According to RFC 2741, 6.1. "AgentX PDU Header"
	     */
	    uint32_t transactionID;
	    
	    /**
	     * \brief The PDU context
	     *
	     * The PDU context, if any. If this field is NULL, the PDU has the 
	     * default context.
	     *
	     * When serializing a %PDU, the context (if present) is included.  
	     * When parsing a %PDU, this field is filled.
	     *
	     * TODO: How to handle memory?
	     */
	    Octet_String* context;

	    /**
	     * \brief Counter for automatic packetID generator
	     *
	     * The packetID member is set automatically by the constructors; 
	     * each new PDU gets a new packetID. This member contains the last 
	     * used packetID.
	     *
	     * The parse constructor does not use this member, because it reads 
	     * the packetID from a stream.
	     */
	    static uint32_t packetID_cnt;
	    

	protected:
	    /**
	     * \brief h.packetID field according to RFC 2741, 6.1. "AgentX PDU
	     *        Header".
	     * 
	     * Is automatically filled by constructors, is set to another value 
	     * by ResponsePDU. The PDU class has no setter for this member.
	     *
	     * The ResponsePDU is a special case in resepect of packetIDs, 
	     * therefore this member is protected to allow the ResponsePDU to 
	     * alter it.
	     */
	    uint32_t packetID;
	    
	    /**
	     * \brief Parse constructor
	     *
	     * Read the %PDU header from a buffer and initialize part of the 
	     * %PDU object. Is called by the parse constructors of derived 
	     * classes. See \ref parsing for details about %PDU parsing.
	     * 
	     * \param pos Iterator pointing to the current stream position. The
	     *		  iterator is advanced while reading the header.
	     *
	     * \param big_endian Whether the serialized form of the %PDU is in
	     *                   big_endian format.
	     *
	     * \exception parse_error If parsing fails, for example because
	     *			      reading the stream fails or the %PDU is 
	     *			      malformed.
	     */
	    PDU(data_t::const_iterator& pos, bool big_endian) throw(parse_error);

	    /**
	     * \brief Construct the PDU header and add it to the payload
	     *
	     * Add the PDU header to the payload. This also adds the context, 
	     * although it is not strictly part of the header. Called by 
	     * derived classes during serialization.
	     * 
	     * \warning The payload must not grow or shrink after a call to
	     *          this function as its size is encoded into the header.
	     *
	     * The header is encoded in big endian format.
	     *
	     * \param type The PDU type, according to RFC 2741, 6.1. "AgentX
	     *             PDU Header"
	     *
	     * \param payload The payload of the PDU, excluding the context.
	     *		      The header is added to the payload, i.e. the 
	     *		      payload is altered by this function.
	     */
	    void add_header(byte_t type, data_t& payload);

	    /**
	     * \brief Default constructor
	     *
	     * The packetID_cnt is incremented and the packetID is set to the 
	     * new packetID_cnt value. The sessionID and transactionID are both 
	     * set to 0. The context is set to the provided value or to 0.
	     *
	     * TODO: What to du with the flags? Currently, they are set to 
	     * false.
	     * 
	     * \param context The PDU context
	     */
	    PDU(Octet_String* context=0) throw();

	public:
	    /**
	     * \brief Get sessionID
	     */
	    uint32_t get_sessionID() { return sessionID; }
	    
	    /**
	     * \brief Set sessionID
	     */
	    void set_sessionID(uint32_t id) { sessionID = id; }

	    /**
	     * \brief Get transactionID
	     */
	    uint32_t get_transactionID() { return transactionID; }
	    
	    /**
	     * \brief Set transactionID
	     */
	    void set_transactionID(uint32_t id) { transactionID = id; }
	    
	    /**
	     * \brief Get packetID
	     */
	    uint32_t get_packetID() { return packetID; }
	    
	    /**
	     * \brief Get context
	     */
	    Octet_String* get_context() { return context; }
	    
	    /**
	     * \brief Set context
	     */
	    void set_context(Octet_String* c) { context = c; }

	    
	    /**
	     * \brief Parse a %PDU from an input stream
	     *
	     * Read the %PDU into a buffer, then create a %PDU of the according 
	     * type (e.g. OpenPDU) from that buffer. See \ref parsing for 
	     * details about %PDU parsing.
	     *
	     * \exception parse_error If parsing fails, for example because
	     *			      reading the stream fails or the PDU is 
	     *			      malformed. The stream position is 
	     *			      undefined after this error.
	     * 
	     * \exception version_mismatch If the AgentX version of the %PDU
	     *                             in the stream is not 1.  The stream 
	     *                             position is undefined after this 
	     *                             error.
	     *
	     * \param in The input stream to read from.
	     *
	     * \return Pointer to PDU object of according type; the user must
	     *	       delete the object if it is not longer needed.
	     */
	    /*
	    static PDU* get_pdu(input_stream& in) throw(parse_error, version_mismatch);
	    */
	    static PDU* get_pdu(boost::asio::local::stream_protocol::socket& in)
		throw(parse_error, version_mismatch);

    };
}


#endif


