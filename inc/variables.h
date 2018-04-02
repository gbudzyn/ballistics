#ifndef VARIABLES_H
#define VARIABLES_H

#include "vector3d.h"

namespace enums
{

enum Flight_stages
{
	Rocket_fuel, 
	Free_flight,
	Descend 
};

std::string to_string(Flight_stages stage);
	
}

struct Variables_vectors
{
	Vector3D position;
	Vector3D velocity;
	Vector3D force;
	Variables_vectors(Vector3D pos, Vector3D vel, Vector3D fc)
	 : position(pos),
	   velocity(vel),
	   force(fc)
	   {}
	void zero()
	{
		position.zero();
		velocity.zero();
		force.zero();
	}
};

struct Atmospheric_variables
{
	double air_temperature;
	double air_pressure;
	double air_density;
	double actual_g_acc;
	Atmospheric_variables(double temp, double pressure, double density, double g_acc)
	 : air_temperature(temp),
	   air_pressure(pressure),
	   air_density(density),
	   actual_g_acc(g_acc)
	   {}
	std::string to_str() const;
};

struct Variables_coeffs
{
	double effective_area;
	double mat_density;
	double temp_coeff;
	double time;
	double drag_C_d;
	enums::Flight_stages stage;
	Variables_coeffs(double _mat_density, double _temp_coeff, double _effective_area, double _time, double _drag_C_d, enums::Flight_stages _stage)
	 : mat_density(_mat_density),
	   temp_coeff(_temp_coeff),
	   effective_area(_effective_area),
	   time(_time),
	   drag_C_d(_drag_C_d),
	   stage(_stage)
	{}
	std::string to_str() const;
};

struct Variables
{
	Variables_vectors vecs;
	Variables_coeffs coeffs;
	Atmospheric_variables atm;
	Variables(Variables_vectors _vecs, 
			  Variables_coeffs _coeffs,
			  Atmospheric_variables _atm)
	 : vecs(_vecs),
	   coeffs(_coeffs),
	   atm(_atm)
	   {}
	std::string to_str(bool brackets) const;
};

#endif

