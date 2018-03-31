#include <iostream>

#include "engine.h"
#include "streamer.h"

int main(int argc, char *argv[])
{   
	try
	{
		Engine engine;
		while( engine.run() );
		
		streamer::stream_all( engine );
	}
	catch(std::string const &ex)
	{
		std::cout << ex << "\n";
	}
	catch(std::exception const &ex)
	{
		std::cout << ex.what() << "\n";
	}
	catch(...)
	{
		std::cout << "UNHANDLED EXCEPTION\n";
	}
	
    return 0;
}

