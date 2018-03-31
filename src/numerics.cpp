#include "numerics.h"

namespace numerics
{
	
double calculate_sq_distance(Vector3D const &r1, Vector3D const &r2)
{
	double const * const x1 = &r1.get_x();
	double const * const x2 = &r2.get_x();
	 
	return pow(x1[0] - x2[0], 2.0) + pow(x1[1] - x2[1], 2.0) + pow(x1[2] - x2[2], 2.0);
}

double calculate_distance(Vector3D const &r1, Vector3D const &r2)
{
	return sqrt( calculate_sq_distance(r1, r2) );
}

double get_vector_length_squared(Vector3D const &vec)
{
	double const * const x1 = &vec.get_x();
	return pow(x1[0], 2.0) + pow(x1[1], 2.0) + pow(x1[2], 2.0);
}

double linear_interpolation(std::map<double, double> const &data, double point)
{
	std::map<double, double>::const_iterator it = data.upper_bound(point);
	if( it == data.end() )
	{
		return (--it)->second; // TODO... maybe throw?
	}
	if( it == data.begin() )
	{
		return it->second; // TODO... maybe throw?
	}
	
	std::map<double, double>::const_iterator it_1 = it; --it_1;
	
	const double delta = ( point -it_1->first ) / (it->first - it_1->first);
	double res = delta * it->second + (1.0 - delta) * it_1->second;
	
	return res;
}

double get_radius_from_mass_density(double mass, double density)
{
	return pow( 3.0/4.0 * mass / (density * Settings::getInstance().get_constants().pi), 1.0/3.0);
}

double get_cross_section_sphere(double radius)
{
	return 4.0 * Settings::getInstance().get_constants().pi * radius * radius;
}

double get_new_area_given_temperature_change( double area, double thermal_coeff, double delta_T )
{
	return ( 1.0 + 2.0 * thermal_coeff * delta_T ) * area;
}

// F = m * g_0
// F = G * M * m / R^2
// m * g_0 = G * M * m / R^2
// g_0 = G * M / R^2
// g_h = G * M / (R + h)^2
// g_h / g_0 = R^2 / (R + h)^2
// g_h = g_0 * 1.0 / (1.0 + h/R)^2
// g_h = g_0 * (1.0 + h/R)^(-2)
// Taylor expansion to simplyfy (h << R)
// g_h = g_0 * (1.0 - 2.0 * h/R)
double get_g_acc(double height)
{
	return Settings::getInstance().get_constants().g_acc * (1.0 - 2.0 * height / Settings::getInstance().get_constants().earth_radius ) ;
}

double get_air_temperature(double height)
{
	return linear_interpolation( Settings::getInstance().get_weather().air_temperature, height );
}

double get_air_density(double height)
{
	return linear_interpolation( Settings::getInstance().get_weather().air_density, height );
}

double get_air_pressure(double height)
{
	return 10132250.0;
}

}

