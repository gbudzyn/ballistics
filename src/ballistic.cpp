#include "ballistic.h"
#include "settings.h"

Ballistic_missile::Ballistic_missile(IMovable_inits _inits, std::vector<Ballistic_stage*> bstages)
 : IMovable( _inits ),
   stages(bstages)
{
	if( get_center_of_mass_Z() < get_center_of_pressure_Z() + Settings::getInstance().get_constants().epsilon_float )
	{
		std::string msg("Ballistic missile not stable! Center of mass Z < Center of pressure Z!");
		throw msg;
	}
}

double Ballistic_missile::get_mass_of_system() const
{
	double mass = 0.0;
	for(std::vector<Ballistic_stage*>::const_iterator it = stages.begin() ; it != stages.end() ; ++it )
	{
		mass += (*it)->get_mass();
	}
	
	return mass;
}

double Ballistic_missile::get_center_of_pressure_Z() const
{
    double center_Z = 0.0;
	for(std::vector<Ballistic_stage*>::const_iterator it = stages.begin() ; it != stages.end() ; ++it )
	{
		center_Z += (*it)->get_mass() * ( (*it)->get_offset_H() + (*it)->get_center_of_pressure_Z() ) ;
	}
	
	return center_Z / this->get_mass_of_system();
}

double Ballistic_missile::get_pressure_area() const
{
	double area = 0.0;
	for(std::vector<Ballistic_stage*>::const_iterator it = stages.begin() ; it != stages.end() ; ++it )
	{
		area += (*it)->get_pressure_area();
	}
	
	return area;
}

double Ballistic_missile::get_center_of_mass_Z() const
{
	// every Z is in local system of coordinates ... starting with 0
	// offset moves to the rocket global coordinate system
	// can be done clever... offset as a variable in cntainer and changes after throwing any
    double center_Z = 0.0;
	for(std::vector<Ballistic_stage*>::const_iterator it = stages.begin() ; it != stages.end() ; ++it )
	{
		center_Z += (*it)->get_mass() * ( (*it)->get_offset_H() + (*it)->get_center_of_mass_Z() ) ;
	}
	
	return center_Z / this->get_mass_of_system();
}

