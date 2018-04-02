#include "variables.h"

#include <sstream>

namespace enums
{
	
std::string to_string(Flight_stages stage)
{	
	std::string ret;
	switch(stage)
	{
		case enums::Rocket_fuel: 
			ret += "Rocket_fuel"; break;
		case enums::Free_flight: 
			ret += "Free_flight"; break;
		case enums::Descend: 
			ret += "Descend"; break;
		default: 
			ret += "UNKNOWN"; break;
	}
	
	return ret;
}

}

std::string Atmospheric_variables::to_str() const
{
	std::stringstream ret;
	ret << air_temperature << " " 
	    << air_pressure << " "
	    << air_density << " "
	    << actual_g_acc << " ";
	
	return ret.str();
}

std::string Variables_coeffs::to_str() const
{
	std::stringstream ret;
	ret << effective_area << " "
	    << mat_density << " "
	    << temp_coeff << " "
	    << time << " "
	    << drag_C_d << " "
	    << enums::to_string(stage) << " ";
	
	return ret.str();
}

std::string Variables::to_str(bool brackets) const
{
	std::stringstream ret;
	ret << vecs.position.to_str(brackets) <<
	       vecs.velocity.to_str(brackets) <<
	       vecs.force.to_str(brackets) <<
	       coeffs.to_str() <<
		   atm.to_str();
	       
	return ret.str();
}


