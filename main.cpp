#include <iostream>
#include <fstream>
#include "oid.h"
#include "Integer.h"
#include "Octet_String.h"
#include "varbind.h"
#include "Counter32.h"
#include "Counter64.h"
#include "Gauge32.h"


using namespace agentx;
using namespace std;


void print_serialized(data_t stream)
{

    cout << "+----------+----------+----------+----------+" << endl;
    cout << "| ";//begin line
    for(int i = 0; i < stream.size(); i++)
    {
	cout.width(8);// 8 chars per field
	cout << (int)stream[i] << " | ";
	if( (i+1)%4 == 0 )
	{
	    cout << endl;   // end line
	    cout << "+----------+----------+----------+----------+";
	    if( i != stream.size() - 1 )
	    {
		// We have further data; begin a new line
		cout << endl << "| ";
	    }
	}
    }
    cout << endl;
}


class myoctetstring : public Octet_String
{
    public:
	myoctetstring(data_t v) { value = v; }
	myoctetstring() {}
	friend std::ostream& operator<<(std::ostream&, const myoctetstring&);

};
std::ostream& operator<<(std::ostream& out, const myoctetstring& o)
{
    data_t::const_iterator i = o.value.begin();
    while( i != o.value.end() )
    {
	out << *i;
	i++;
    }

    return out;
}
void test_Octet_String()
{
    cout << "--- Testing Octet_String ---" << endl;

    data_t tmp;
    tmp.push_back('H');
    tmp.push_back('e');
    tmp.push_back('l');
    tmp.push_back('l');
    tmp.push_back('o');

    myoctetstring os1(tmp);

    data_t data;

    ofstream ofile("test.data");
    data = os1.serialize();
    print_serialized(data);
    for( int i = 0; i < data.size(); i++)
    {
        ofile.put((char)data[i]);
    }
    cout << endl;
    ofile.close();


    data.clear();
    
    ifstream ifile("test.data");
    char ch;
    while ( ifile.get(ch) )
    {
        data.push_back(ch);
    }
    ifile.close();
    cout << "Read " << data.size() << " bytes." << endl;

    myoctetstring os2;
    data_t::const_iterator it = data.begin();
    os2.deserialize(it, true);
    cout << "os2 is \"" << os2 << "\"" << endl;

}



class myinteger : public Integer
{
    public:
	myinteger(int v) { value = v; }
	myinteger() {}
	friend std::ostream& operator<<(std::ostream&, const myinteger&);

};
std::ostream& operator<<(std::ostream& out, const myinteger& i)
{
    out << i.value;

    return out;
}
void test_integer()
{
    cout << "--- Testing Integer ---" << endl;

    myinteger i1(0xcafebabe);
    cout << hex;
    cout << "Integer value is " << i1 << endl;

    data_t data;

    ofstream i1_file("test.data");
    data = i1.serialize();
    print_serialized(data);
    for( int i = 0; i < data.size(); i++)
    {
        i1_file.put((char)data[i]);
    }
    cout << endl;
    i1_file.close();


    data.clear();
    
    ifstream file("test.data");
    char ch;
    while ( file.get(ch) )
    {
        data.push_back(ch);
    }
    file.close();
    cout << "Read " << data.size() << " bytes." << endl;

    myinteger i2;
    data_t::const_iterator it = data.begin();
    i2.deserialize(it, true);
    cout << "i2 is " << i2 << endl;

    cout << dec;

}




class mycounter32 : public Counter32
{
    public:
	mycounter32(int v) { value = v; }
	mycounter32() {}
	friend std::ostream& operator<<(std::ostream&, const mycounter32&);

};
std::ostream& operator<<(std::ostream& out, const mycounter32& i)
{
    out << i.value;

    return out;
}
void test_counter32()
{
    cout << "--- Testing Counter32 ---" << endl;

    mycounter32 c32_1(0xcafebabe);
    cout << hex;
    cout << "Counter32 value is " << c32_1 << endl;

    data_t data;

    ofstream ofile("test.data");
    data = c32_1.serialize();
    print_serialized(data);
    for( int i = 0; i < data.size(); i++)
    {
        ofile.put((char)data[i]);
    }
    cout << endl;
    ofile.close();


    data.clear();
    
    ifstream ifile("test.data");
    char ch;
    while ( ifile.get(ch) )
    {
        data.push_back(ch);
    }
    ifile.close();
    cout << "Read " << data.size() << " bytes." << endl;

    mycounter32 c32_2;
    data_t::const_iterator it = data.begin();
    c32_2.deserialize(it, true);
    cout << "c32_2 is " << c32_2 << endl;

    cout << dec;

}




class mycounter64 : public Counter64
{
    public:
	mycounter64(unsigned long long v) { value = v; }
	mycounter64() {}
	friend std::ostream& operator<<(std::ostream&, const mycounter64&);

};
std::ostream& operator<<(std::ostream& out, const mycounter64& i)
{
    out << i.value;

    return out;
}
void test_counter64()
{
    cout << "--- Testing Counter64 ---" << endl;

    mycounter64 c64_1(0xcafebabeaffe0505);
    cout << hex;
    cout << "Counter64 value is " << c64_1 << endl;

    data_t data;

    ofstream ofile("test.data");
    data = c64_1.serialize();
    print_serialized(data);
    for( int i = 0; i < data.size(); i++)
    {
        ofile.put((char)data[i]);
    }
    cout << endl;
    ofile.close();


    data.clear();
    
    ifstream ifile("test.data");
    char ch;
    while ( ifile.get(ch) )
    {
        data.push_back(ch);
    }
    ifile.close();
    cout << "Read " << data.size() << " bytes." << endl;

    mycounter64 c64_2;
    data_t::const_iterator it = data.begin();
    c64_2.deserialize(it, true);
    cout << "c64_2 is " << c64_2 << endl;

    cout << dec;

}



class myGauge32 : public Gauge32
{
    public:
	myGauge32(int v) { value = v; }
	myGauge32() {}
	friend std::ostream& operator<<(std::ostream&, const myGauge32&);

};
std::ostream& operator<<(std::ostream& out, const myGauge32& i)
{
    out << i.value;

    return out;
}
void test_Gauge32()
{
    cout << "--- Testing Gauge32---" << endl;

    myGauge32 g1(0xcafebabe);
    cout << hex;
    cout << "Gauge32 value is " << g1 << endl;

    data_t data;

    ofstream ofile("test.data");
    data = g1.serialize();
    print_serialized(data);
    for( int i = 0; i < data.size(); i++)
    {
        ofile.put((char)data[i]);
    }
    cout << endl;
    ofile.close();


    data.clear();
    
    ifstream ifile("test.data");
    char ch;
    while ( ifile.get(ch) )
    {
        data.push_back(ch);
    }
    ifile.close();
    cout << "Read " << data.size() << " bytes." << endl;

    myGauge32 g2;
    data_t::const_iterator it = data.begin();
    g2.deserialize(it, true);
    cout << "g2 is " << g2 << endl;

    cout << dec;

}

void test_varbind()
{
    cout << "--- Testing varbind ---" << endl;

    data_t data;
    
    oid o(1,3,6,1,23,42);
    
    myinteger i(0xa5);
    varbind vb_integer(&o, &i);
    data = vb_integer.serialize();
    print_serialized(data);
    
    data_t tmp;
    tmp.push_back('H');
    tmp.push_back('e');
    tmp.push_back('l');
    tmp.push_back('l');
    tmp.push_back('o');
    Octet_String os(tmp);
    varbind vb_Octet_String(&o, &os);
    data = vb_Octet_String.serialize();
    print_serialized(data);
    
    mycounter32 c32(0x5a);
    varbind vb_counter32(&o, &c32);
    data = vb_counter32.serialize();
    print_serialized(data);
    
    myGauge32 g32(0x5a);
    varbind vb_Gauge32(&o, &g32);
    data = vb_Gauge32.serialize();
    print_serialized(data);
    
    cout << endl;
}

int main()
{
    test_integer();
    test_Octet_String();
    test_counter32();
    test_counter64();
    test_Gauge32();


    test_varbind();

    return 0;
}
