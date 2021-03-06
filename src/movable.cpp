#include "movable.h"
#include "settings.h"
#include "numerics.h"

void IMovable::add_forces(Variables &next, Variables const &last)
{
	// FORCES
	add_gravity( next, last );
	add_drag_force( next, last );
	this->add_special_forces(next, last);
}

void IMovable::add_drag_force(Variables &next, Variables const &last)
{
	//drag forces F_d = 1/2 * gas_density * square_length_of_velocity * C_d * cross_section_area
	double F_d_constants = 1.0/2.0 * last.atm.air_density * last.coeffs.drag_C_d * last.coeffs.effective_area;
	
	const double * const velocities = &last.vecs.velocity.get_x();
	double multiplyer = 1.0;
	for(unsigned i = 0 ; i < 3 ; i++)
	{	
		// TODO - REAL PHYSICS after reaching sonic speeds 
		multiplyer = exp( 1.0 + pow( fabs( velocities[i] ) / 1000.0, last.atm.air_density / 0.5 ) );
	    next.vecs.force[i] += -F_d_constants * velocities[i] * multiplyer; // minus ... opposite direction to velocity
	}
}

void IMovable::add_gravity(Variables &next, Variables const &last)
{
	//gravity force - dependent on the height! ... but also on angle (implement coriolis effect)
	double x2 = this->get_mass_of_system() * last.atm.actual_g_acc;
	next.vecs.force[2] -= x2; // minus 'cause g is positive in value
}

void IMovable::get_next_step()
{	
	// wizardry begins here
	Variables &last = variables[ current_step++ ];
	current_step %= 2; // only 0 or 1 is possible!
	Variables &next = variables[ current_step ];
	// temp_difference for new effective area calculations
	double temp_diff = last.atm.air_temperature - next.atm.air_temperature;
	
	// last coefficients as a base and change only those that are updated	
	next.vecs.zero();
	next.coeffs = last.coeffs;
	next.coeffs.effective_area = numerics::get_new_area_given_temperature_change( last.coeffs.effective_area, 
																				  last.coeffs.temp_coeff, 
																				  temp_diff );
	next.coeffs.time = Settings::getInstance().get_params().current_time;
	// atmospheric variables are updated after integration
	
	this->add_forces(next, last);
	
	// first order integration
	this->integrate(next, last);
	
	if( next.coeffs.stage == enums::Descend )
		active = ( false == check_for_deployment() );
		
	// CHECK for escape velocity ... change to escape velocity as a function of height!
	if( next.vecs.velocity[2] > 10000.0 )
	{
		std::string msg("ERROR escape velocity reached");
		throw msg;
	}
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
	const double * const positions = &variables[ current_step ].vecs.position.get_x();
	if( positions[2] < deployment_level )
		return true;
		
	return false;
}

Solid_ball::Solid_ball(IMovable_inits _inits)
 : IMovable(_inits)
{
}

IMovable::IMovable(IMovable_inits _inits) 
  : mass(_inits.mass),
    material(_inits.material),
    name(_inits.name),
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
								  0.47, /// TODO hardcoded drag C_d for sphere
								  _inits.stage);
	
	const double &height = _inits.initial_vec.position.get_x(); 
	Atmospheric_variables initial_atm( numerics::get_air_temperature( height ),
									   numerics::get_air_pressure( height ),
									   numerics::get_air_density( height ),
									   numerics::get_g_acc( height ) );
	
	current_step = 0; // starting from 0
	// first steps are the same - easier to implement integration etc.
	variables.reserve(2);
	variables[0] = Variables( _inits.initial_vec, init_coeffs, initial_atm );
	variables[1] = Variables( _inits.initial_vec, init_coeffs, initial_atm );
	
	deployment_level = 0.0;
}

