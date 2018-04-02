#include <iostream>

#include "engine.h"
#include "ballistic_factory.h"

int main(int argc, char *argv[])
{   
	try
	{
		// THIS IS JUST FOR TESTING BALLISTIC MISSILE
		Ballistic_missile *V2 = Ballistic_factory::create_ballistic_missile();
		//END
		Engine engine;
		while( engine.run() );
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

