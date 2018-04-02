#ifndef BALLISTIC_STAGE_H
#define BALLISTIC_STAGE_H

#include <vector>
#include <cmath>
#include <sstream>

class Ballistic_stage
{
public:
	virtual double get_mass() const
	{
		return container_mass;
	}
	virtual double get_center_of_mass_Z() const = 0;
	virtual double get_center_of_pressure_Z() const = 0;
	virtual double get_pressure_area() const = 0;
	double get_C_d() const
	{
		return C_d;
	}
	double get_offset_H() const
	{
		return offset_H;
	}
	void update_offset(double new_offset)
	{
		offset_H = new_offset;
	}
	virtual std::string to_str() const;
protected:
	double R;
	double container_mass;
	double offset_H;
	double C_d;
	Ballistic_stage(double _R, double _container_mass, double _offset_H)
	: R(_R),
	  container_mass(_container_mass),
	  offset_H(_offset_H)
	  {}
};

struct Cylinder : public Ballistic_stage
{
public:
	virtual double get_mass() const
	{
		return container_mass + fuel_mass;
	}
	virtual double get_center_of_mass_Z() const
	{
		return (  H * container_mass + fuel_mass * fuel_height ) / ( 2.0 * this->get_mass() );
	}
	virtual double get_pressure_area() const
	{
		return 2.0 * R * H;
	}
	virtual double get_center_of_pressure_Z() const
	{
		return H / 2.0;
	}
	Cylinder(double _R, double _container_mass, double _offset_H, double _H, double _thrust, double _fuel_intake_per_second, double _fuel_density);
	virtual std::string to_str() const;
	double get_H() const
	{
		return H;
	}
protected:
	double H;
	double thrust;
	double fuel_intake_per_second; // kg/s
	double fuel_density; // kg/m^3 !!!
	// calculated from density and height
	double fuel_mass; 
	double fuel_height;
	
	double get_volume(double height) const
	{
		return M_PI * R * R * height; 
	}
};

struct Wing
{
	double length;
	double height;
	double mass;
	Wing(double _l, double _h, double _m)
	: length(_l),
	  height(_h),
	  mass(_m)
	  {}
};

// it can be only as a first stage!!!
// has 4 right angle triangle wings and is symmetric and not rotating yet
struct Cylinder_with_wings : public Cylinder
{
public:
	virtual double get_mass() const
	{
		return Cylinder::get_mass() + 4.0 * wing.mass;
	}
	virtual double get_center_of_mass_Z() const
	{
		return ( Cylinder::get_mass() * Cylinder::get_center_of_mass_Z() + 4.0 * wing.mass * wing.height / 3.0 ) / this->get_mass();
	}
	// 2.0 * wings area 'cause it's not rotating yet
	virtual double get_pressure_area() const
	{
		return Cylinder::get_pressure_area() + wing.length * wing.height;
	}
	virtual double get_center_of_pressure_Z() const
	{
		return ( 1.0/3.0 * wing.length * wing.height + R * H ) / this->get_pressure_area();
	}
	Cylinder_with_wings(double _R, 
						double _container_mass,
						double _offset_H, 
						double _H, 
						double _thrust, 
						double _fuel_intake_per_second, 
						double _fuel_density, 
						Wing _wing);
	virtual std::string to_str() const;
protected:
	Wing wing;
};

// cone does not have a fuel - only as a last stage!
struct Cone : public Ballistic_stage
{
public:
	virtual double get_center_of_mass_Z() const
	{
		return H / 4.0;
	}
	virtual double get_pressure_area() const
	{
		return R * H;
	}
	virtual double get_center_of_pressure_Z() const
	{
		return H/3.0;
	}
	Cone(double _R, double _container_mass, double _offset_H, double _H)
	: Ballistic_stage(_R, _container_mass, _offset_H),
	  H(_H)
	  {
	      C_d = 0.50;
	  }
	std::string to_str() const;
protected:
	double H;
};

// half_sphere does not have a fuel - only as a last stage!
struct Half_sphere : public Ballistic_stage
{
public:
	virtual double get_center_of_mass_Z() const
	{
		return 3.0/8.0 * R;
	}
	virtual double get_pressure_area() const
	{
		return M_PI * R * R / 2.0;
	}
	virtual double get_center_of_pressure_Z() const
	{
		return 4.0 / 3.0 * R / M_PI;
	}
	Half_sphere(double _R, double _container_mass, double _offset_H)
	: Ballistic_stage(_R, _container_mass, _offset_H)
	  {
	  	  C_d = 0.42;
	  }
protected:
};

#endif

