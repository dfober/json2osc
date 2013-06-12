
#include <iostream>
#include <fstream>

#include "json_object.h"
#include "json_array.h"
#include "json_parser.h"

#include "osc_listener.h"
#include "osc2json.h"
#include "osc_stream.h"

using namespace std;
using namespace json;

//-----------------------------------------------------------------------------
static void testbuild ()
{
	json_object obj;
	obj.add (new json_element("toto", new json_string_value("value")));
	obj.add (new json_element("titi", new json_int_value(888)));
	obj.add (new json_element("tata", new json_true_value()));

	json_array* a = new json_array;
	a->add(new json_int_value(1));
	a->add(new json_int_value(2));
	a->add(new json_float_value(3.2));
	a->add(new json_string_value("string"));
	a->add(new json_null_value());

	json_object* o2 = new json_object;
	o2->add (new json_element("text", new json_string_value("some text")));
	o2->add (new json_element("num", new json_int_value(999)));
	o2->add (new json_element("null", new json_null_value()));
	a->add (new json_object_value(o2));

	obj.add (new json_element ("tutu", new json_array_value(a)));

	cout << obj << endl;
}

//-----------------------------------------------------------------------------
class osc2jsonprinter : public osc2json
{
	public:
		virtual void received (const char* json) { cout << json << endl; }
};

static void testosc ()
{
	cout << "testing osc " << endl;
	osc2jsonprinter p;
	osc_listener listener (&p, 7000);
	listener.run();	
	cout << "osc test closed" << endl;
}

//-----------------------------------------------------------------------------
static void testparse (int argc, char * argv[])
{
	for (int i=1; i < argc; i++) {
		fstream stream (argv[i]);
		if (stream.is_open()) {
			json_parser p (&stream);
			json_object* obj = p.parse();
			if (obj) {
				cout << obj << endl;
			}
		}
		else cerr << "can't open file " << argv[i];
	}
}

//-----------------------------------------------------------------------------
static void testjson2osc (int argc, char * argv[])
{
	osc_stream out("localhost", 7000);
	for (int i=1; i < argc; i++) {
		fstream stream (argv[i]);
		if (stream.is_open()) {
			json_parser p (&stream);
			json_object* obj = p.parse();
			if (obj) obj->print(out);
		}
		else cerr << "can't open file " << argv[i];
	}
}

//-----------------------------------------------------------------------------
int main (int argc, char * argv[])
{
//	testbuild();
//	testparse(argc, argv);
//	testosc();
	testjson2osc(argc, argv);
	return 0;
}
