#ifndef NUMERICS_H
#define NUMERICS_H

#include <cmath>
#include <map>

#include "vector3d.h"
#include "settings.h"

namespace numerics
{

double calculate_sq_distance(Vector3D const &r1, Vector3D const &r2);
double calculate_distance(Vector3D const &r1, Vector3D const &r2);
double linear_interpolation(std::map<double, double> const &data, double point);
double get_vector_length_squared(Vector3D const &vec);

// for sphere
double get_radius_from_mass_density(double mass, double density);
double get_cross_section_sphere(double radius);
double get_new_area_given_temperature_change( double area, double thermal_coeff, double delta_T );
double get_g_acc(double height);

// TO BE IMPLEMENTED WELL
double get_air_temperature(double height);
double get_air_density(double height);
double barometric_formula_1(double val_B, double TB, double LB, double hB, double height, double addition);
double barometric_formula_2(double val_B, double TB, double LB, double hB, double height);
double get_air_param_from_table(double value_0, double height, double addition, std::map<double, Settings::Air_table>::const_iterator const &it);
double get_air_pressure(double height);
}

#endif

