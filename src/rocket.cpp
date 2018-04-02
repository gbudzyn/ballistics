#include "rocket.h"
#include "settings.h"
#include "numerics.h"

Rocket::Rocket(IMovable_inits _inits, std::vector<Rocket_stage_var> rsv)
 : IMovable(_inits),
   rocket_var(rsv)	
{	
	if( this->get_mass_of_system() * Settings::getInstance().get_constants().g_acc > sqrt( numerics::get_vector_length_squared( rocket_var[0].thrust ) ) )
	{
		std::string msg("Rocket thrust too low for this data!");
		throw msg;
	}
}

void Rocket::add_special_forces(Variables &next, Variables const &last)
{
	// check if we have enough fuel!!!
	if( last.coeffs.stage != enums::Rocket_fuel )
	{
		return;
	}
	
	if( rocket_var.empty() == true )
	{
		next.coeffs.stage = enums::Free_flight;
		return;
	}
	
	Rocket_stage_var &current_stage = rocket_var[0];
	double &fuel = current_stage.fuel_mass;
	if( fuel > 0.0  )
	{
		// add thrust
		double ratio = 1.0;
		if( fuel > Settings::getInstance().get_params().dt * current_stage.fuel_intake_per_second )
		{
			fuel -= Settings::getInstance().get_params().dt * current_stage.fuel_intake_per_second;
		}
		else
		{
			// cannot operate for the whole period... calc how many ...
			ratio = fuel / ( Settings::getInstance().get_params().dt * current_stage.fuel_intake_per_second );
			fuel = 0.0;
		}
		
		const double * const thrust = &current_stage.thrust.get_x();
		for(unsigned i = 0 ; i < 3 ; i++)
		{
	    	next.vecs.force[i] += ratio * thrust[i];
		}

		// drop the stage
	    if( fuel <= 0.0 )
	    {
	    	rocket_var.erase( rocket_var.begin() );
		}
	}
}

