
#include <iostream>
#include <fstream>

#include "json_osc.h"

using namespace std;
using namespace json;


//-----------------------------------------------------------------------------
class osc2jsonprinter : public osc2json
{
	public:
		virtual void received (const char* json) { cout << json << endl; }
};

//-----------------------------------------------------------------------------
static void testosc ()
{
	cout << "testing osc " << endl;
	osc2jsonprinter p;
	osc_listener * listener;
	int ret = start_osc2json (&p, 7000, &listener);
	if (ret) 
		cerr << error_string(ret) << endl;
}


//-----------------------------------------------------------------------------
static void testjson2osc (int argc, char * argv[])
{
	osc_stream*	osc = start_json2osc ("localhost", 7000);
	for (int i=1; i < argc; i++) {
		int res = sendfile (argv[i], osc);
		if (res)
			cerr << argv[i] << ": " << error_string(res) << endl;
	}
}

//-----------------------------------------------------------------------------
int main (int argc, char * argv[])
{
	if (0)
		testosc();
	else
		testjson2osc(argc, argv);
	return 0;
}
