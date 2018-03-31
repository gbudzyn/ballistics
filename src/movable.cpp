#include "movable.h"
#include "settings.h"
#include "numerics.h"

#include <iostream>

void IMovable::add_forces(Variables &next, Variables const &last)
{
	const double * const velocities = &last.vecs.velocity.get_x();
	const double * const positions = &last.vecs.position.get_x();
	// FORCES
	//gravity force - dependent on the height! to be implemented
	double x2 = this->get_mass_of_system() * last.atm.actual_g_acc;
	next.vecs.force[2] -= x2; // minus 'cause g is positive in value
	//drag forces F_d = 1/2 * gas_density * square_length_of_velocity * C_d * cross_section_area
	double F_d_constants = 1.0/2.0 * last.atm.air_density * Settings::getInstance().get_constants().sphere_C_d * last.coeffs.effective_area;
	//std::cout << F_d_constants << "\n";
	for(unsigned i = 0 ; i < 3 ; i++)
	{
	    next.vecs.force[i] += -F_d_constants * velocities[i]; // minus ... opposite direction to velocity
	}
	
	this->add_special_forces(next, last);
}

void IMovable::get_next_step()
{	
	Variables &last = variables[ variables.size() - 1 ];
	
	// last coefficients as a base and change only those that are updated
	Variables_coeffs next_coeffs( last.coeffs );
	Atmospheric_variables next_atm( last.atm );
	// temp_difference for new effective area calculations
	double temp_diff = 0.0;
	if( variables.size() > 1 )
		temp_diff = last.atm.air_temperature - variables[ variables.size() - 2 ].atm.air_temperature;
	next_coeffs.effective_area = numerics::get_new_area_given_temperature_change( last.coeffs.effective_area, last.coeffs.temp_coeff, temp_diff );
	next_coeffs.time = Settings::getInstance().get_params().current_time;
	Variables next( Variables_vectors( Vector3D(), Vector3D(), Vector3D() ),
					next_coeffs,
					next_atm ); // atmospheric variables are updated after integration
	
	this->add_forces(next, last);
	
	// first order integration
	this->integrate(next, last);
	
	if( next.coeffs.stage == enums::Descend )
		active = ( false == check_for_deployment() );
	
	//BY YOUR POWERS COMBINED
	variables.push_back( next );
}

void IMovable::integrate(Variables &next, Variables const &last)
{
	const double * const forces = &last.vecs.force.get_x();
	const double * const velocities = &last.vecs.velocity.get_x();
	const double * const positions = &last.vecs.position.get_x();
	
	//VELOCITIES
	for(unsigned i = 0 ; i < 3 ; i++)
	{
		next.vecs.velocity[i] += forces[i] / this->get_mass_of_system() * Settings::getInstance().get_params().dt + velocities[i];
	}
	
	//POSITIONS
	for(unsigned i = 0 ; i < 3 ; i++)
	{
		next.vecs.position[i] += velocities[i] * Settings::getInstance().get_params().dt + positions[i];
	}
	
	// UPDATE NEXT measurements
	const double * const next_pos = &next.vecs.position.get_x();
	next.atm.air_temperature = numerics::get_air_temperature( next_pos[2] );
	next.atm.air_density = numerics::get_air_density( next_pos[2] );
	next.atm.air_pressure = numerics::get_air_pressure( next_pos[2] );
	next.atm.actual_g_acc = numerics::get_g_acc( positions[2] );
	
	//check for stage update
	if( last.coeffs.stage != enums::Descend && next_pos[2] - positions[2] < 0.0 )
	{
		next.coeffs.stage = enums::Descend;
	}
}

// will enter here only in Stage_3
bool IMovable::check_for_deployment()
{
	const double * const positions = &variables[ variables.size() - 1 ].vecs.position.get_x();
	if( positions[2] < deployment_level )
		return true;
		
	return false;
}

Solid_ball::Solid_ball(double ms, std::string mat, std::string name, Variables_vectors initial_vec)
 : IMovable(ms, mat, name, enums::Free_flight, initial_vec)
{
}

IMovable::IMovable(double _mass, std::string _material, std::string _name, enums::Stages _stage, Variables_vectors initial_vec) 
  : mass(_mass),
    material(_material),
    name(_name),
    active(true)
{
	std::map<std::string, Settings::Material_coeffs>::const_iterator it = Settings::getInstance().get_constants().material_coeffs.find(material);
	if( it == Settings::getInstance().get_constants().material_coeffs.end() )
	{
		std::string msg("wrong material ");
		msg += material;
		throw msg;
	}
	double mat_dens = it->second.density; 
	Variables_coeffs init_coeffs( it->second.density,
								  it->second.thermal_coeff,
								  numerics::get_cross_section_sphere( numerics::get_radius_from_mass_density(mass, mat_dens) ),
								  0.0,
								  _stage);
	
	const double &height = initial_vec.position.get_x(); 
	Atmospheric_variables initial_atm( numerics::get_air_temperature( height ),
									   numerics::get_air_pressure( height ),
									   numerics::get_air_density( height ),
									   numerics::get_g_acc( height ) );
	
	variables.push_back( Variables( initial_vec, init_coeffs, initial_atm ) );
	
	deployment_level = 0.0;
}

Rocket::Rocket(double ms, std::string mat, std::string name, Variables_vectors initial_vec, Rocket_var rv)
 : IMovable(ms, mat, name, enums::Rocket_fuel, initial_vec)
{
	rocket_var.push_back(rv);
	
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
	
	double &fuel = rocket_var[ rocket_var.size() -1 ].fuel_mass;
	if( fuel > 0.0  )
	{
		// add thrust
		double ratio = 1.0;
		if( fuel > Settings::getInstance().get_params().dt * rocket_var[ rocket_var.size() -1 ].second_fuel_intake )
		{
			fuel -= Settings::getInstance().get_params().dt * rocket_var[ rocket_var.size() -1 ].second_fuel_intake;
		}
		else
		{
			// cannot operate for the whole period... calc how many ...
			ratio = fuel / ( Settings::getInstance().get_params().dt * rocket_var[ rocket_var.size() -1 ].second_fuel_intake );
			fuel = 0.0;
		}
		
		const double * const thrust = &rocket_var[ rocket_var.size() -1 ].thrust.get_x();
		for(unsigned i = 0 ; i < 3 ; i++)
		{
	    	next.vecs.force[i] += ratio * thrust[i];
		}

	    if( fuel <= 0.0 )
	    	next.coeffs.stage = enums::Free_flight;
	}
}

