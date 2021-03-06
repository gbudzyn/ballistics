#include "ballistic_stage.h"
#include "settings.h"

Cylinder::Cylinder(double _R, double _container_mass, double _offset_H, double _H, double _thrust, double _fuel_intake_per_second, double _fuel_density)
 : Ballistic_stage(_R, _container_mass, _offset_H),
   H(_H),
   thrust(_thrust),
   fuel_intake_per_second(_fuel_intake_per_second),
   fuel_density(_fuel_density)
{
	// drag_coefficient... 4.0 is selected ... can be calculated
	double cutoff_H_drag = 4.0;
	if( H >= cutoff_H_drag * R )
	    C_d = 0.82;
	else if(H <= R )
		C_d = 1.05;
	else
	{
		C_d = 0.82 + (1.05 - 0.82) * ( H - R ) / ( (cutoff_H_drag - 1.0) * R);
	}
		
	// assume that all H is filled with fuel!
	fuel_height = H;
	fuel_mass = get_volume( fuel_height ) * fuel_density;
}

Cylinder_with_wings::Cylinder_with_wings(double _R, double _container_mass, double _offset_H, double _H, double _thrust, double _fuel_intake_per_second, double _fuel_density, Wing _wing)
 : Cylinder(_R, _container_mass, _offset_H, _H, _thrust, _fuel_intake_per_second, _fuel_density),
   wing(_wing)
{
	// TODO ... epsilon
	// wing height cannot be greater that cylinder height
	if( wing.height > H + Settings::getInstance().get_constants().epsilon_float ) 
	{
		std::string msg("Wing height greater than cylinder!");
		throw msg;
	}
	// center of mass Z should be > center of pressure Z... in order to be stable
	if(  this->get_center_of_pressure_Z() + Settings::getInstance().get_constants().epsilon_float > this->get_center_of_mass_Z() )
	{
		std::string msg("Rocker stage not stable: center_of_pressure > center_of_mass");
		throw msg;
	}
	
	C_d = Cylinder::get_C_d() + 0.25; // can be calculated... have no idea but should be greater than cylinder
}

std::string Ballistic_stage::to_str() const
{
	std::stringstream ret;
	ret << R << " " 
	    << container_mass << " " 
		<< C_d << " "
		<< this->get_center_of_mass_Z() << " "
		<< this->get_center_of_pressure_Z() << " "
		<< this->get_pressure_area() << " ";
	
	return ret.str();
}

std::string Cylinder::to_str() const
{
	std::stringstream ret;
	ret << Ballistic_stage::to_str() << " "
	    << H << " "
	    << thrust << " "
	    << fuel_intake_per_second << " "
	    << fuel_density << " "
	    << fuel_mass << " "
	    << fuel_height << " "
	    << offset_H << " "
	    << this->get_mass() << " ";
	    
	return ret.str();
}

std::string Cylinder_with_wings::to_str() const
{
	std::stringstream ret;
	ret << Cylinder::to_str() << " "
	    << wing.length << " "
	    << wing.height << " "
	    << wing.mass << " "
	    << this->get_mass() << " ";
	    
	return ret.str();
}

std::string Cone::to_str() const
{
	std::stringstream ret;
	ret << Ballistic_stage::to_str() << " "
	    << H << " ";
	    
	return ret.str();
}
