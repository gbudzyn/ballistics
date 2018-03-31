#include "variables.h"

namespace enums
{
	
std::string to_string(Stages stage)
{	
	std::string ret;
	switch(stage)
	{
		case enums::Rocket_fuel: 
			ret += "Rocket_fuel"; break;
		case enums::Free_flight: 
			ret += "Free flight"; break;
		case enums::Descend: 
			ret += "Descend"; break;
		default: 
			ret += "UNKNOWN"; break;
	}
	
	return ret;
}

}

