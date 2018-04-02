#include "ballistic.h"

double Ballistic_stages::get_mass() const
{
	double mass = 0.0;
	for(std::vector<Ballistic_stage*>::const_iterator it = stages.begin() ; it != stages.end() ; ++it )
	{
		mass += (*it)->get_mass();
	}
	
	return mass;
}

