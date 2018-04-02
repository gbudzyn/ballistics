#ifndef ROCKET_H
#define ROCKET_H

#include "movable.h"

struct Thrust_control
{
	double force; // in Newtons
	double max_angle_of_change; // in radians ... 7deg max? = PI/2 * 7/90
};

struct Rocket_stage_var
{
	double fuel_mass;
	double stage_mass;
	double fuel_intake_per_second;
	Vector3D thrust;
	std::string stage_name;
	Rocket_stage_var(double _fuel_mass, double _stage_mass, double _fis, Vector3D _thrust )
	: fuel_mass(_fuel_mass),
	  stage_mass(_stage_mass),
	  fuel_intake_per_second(_fis),
	  thrust(_thrust)
	  {}
	double get_stage_mass() const
	{
		return fuel_mass + stage_mass;
	}
};

// IMPORTANT... stages are from BOTTOM of vector... always current is rocket_var[0]
class Rocket : public IMovable
{
public:
	Rocket(IMovable_inits _inits, std::vector<Rocket_stage_var> rsv);
private:
	std::vector<Rocket_stage_var> rocket_var; // queue might be better
	virtual double get_mass_of_system() const
	{
		double rsv_mass = 0.0;
		for(std::vector<Rocket_stage_var>::const_iterator it = rocket_var.begin() ; it != rocket_var.end() ; ++it)
			rsv_mass += it->get_stage_mass();
		
		return mass + rsv_mass;
	}
	virtual void add_special_forces(Variables &next, Variables const &last);
};



#endif

