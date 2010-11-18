#ifndef _OID_H_
#define _OID_H_

#include <vector>
#include <ostream>
#include <string>
#include "types.h"
#include "exceptions.h"

namespace agentx
{
    /**
     * \brief Represents an SNMP object identifier (OID)
     */
    class oid
    {
	std::vector<uint32_t> identifier;
	bool include;

	public:

	/**
	 * \brief Initialize an oid object with a sequence of up to 20 
	 * intergers.
	 *
	 * This constructor takes up to 20 integers which forms up the object 
	 * identifier. If you need more than 20 components, use 
	 * oid::operator,() .
	 */
	// TODO: How wide are the integers for OIDs?
	oid(int c1=0, int c2=0, int c3=0, int c4=0, int c5=0, int c6=0,
	    int c7=0, int c8=0, int c9=0, int c10=0, int c11=0, int c12=0,
	    int c13=0, int c14=0, int c15=0, int c16=0, int c17=0, int c18=0,
	    int c19=0, int c20=0 );
	
	/**
	 * \brief Append integers to an oid object.
	 *
	 * The ',' operator is provided to append integers to an oid object.  
	 * The following example will create the oid 1.3.6.1:
	 *
	 * \code
	 * oid led_state(1);
	 * led_state = led_state, 3;
	 * led_state = led_state, 6;
	 * led_state = led_state, 1;
	 * \endcode
	 * 
	 * A more useful example would be:
	 * 
	 * \code
	 * oid led_state;
	 * led_state = 1, 3, 6, 1;
	 * \endcode
	 * 
	 * In the last example, the assignment is evaluated before the ',' 
	 * operators (due to the C++ operator precedence rules). Therefore the 
	 * literal 1 is converted to an oid object which is then assigned to 
	 * led_state. After that the ',' is invoked on led_state several times.
	 *
	 * Note that the following will NOT work:
	 * 
	 * \code
	 * oid led_state = 1, 3, 6, 1;
	 * \endcode
	 *
	 * To understand why it does not work, consider this code:
	 *
	 * \code
	 * oid led_state = 1, fan_speed = 3, cpu_usage = 6, hdd_temp = 1;
	 * \endcode
	 */
	oid& operator,(int);

	/**
	 * \brief get the current include value
	 *
	 * The include value is present in the serialized form of an OID. If an 
	 * OID object is created by parsing a AgentX message, the 'include' 
	 * member is set accordingly.
	 *
	 * See RFC 2741, sections 5.1 and 5.2 for details.
	 */
	bool get_include() { return include; }

	/**
	 * \brief set the include value
	 *
	 * The include value is present in the serialized form of an OID. If an 
	 * OID object is serialized, the include field is encoded into the 
	 * stream.
	 *
	 * See RFC 2741, sections 5.1 and 5.2 for details.
	 */
	void set_include(bool i) { include = i; }

	/**
	 * \brief Encode an OID object as described in RFC 2741, section 5.1.
	 */
	data_t serialize();

	/**
	 * \brief Decode an OID object as described in RFC 2741, section 5.1.
	 *
	 * This function expects a data packet containing exactly one OID. If 
	 * the packet contains too much (or too less) data, a parse_error is 
	 * exception thrown.
	 *
	 * An exception is also thrown on other parse errors.
	 */
	void deserialize(data_t::const_iterator start,
		bool big_endian=false)
	    throw(parse_error);
    
	friend std::ostream& operator<<(std::ostream&, const agentx::oid&);

    };
    
    /**
     * \brief The output operator for the oid class.
     *
     * Object identifiers (oid objects) can be output as follows:
     * 
     * \code
     * oid led_state(1,3,6,1);
     * cout << "LED state OID: " << led_state << endl;
     * \endcode
     *
     * The last line will output "LED state OID: 1.3.6.1".
     */
    std::ostream& operator<<(std::ostream&, const agentx::oid&);

}


#endif