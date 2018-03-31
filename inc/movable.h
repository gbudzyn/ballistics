#ifndef MOVABLE_H
#define MOVABLE_H

#include <vector>
#include <string>

#include "variables.h"

class IMovable
{	
public:
	IMovable(double _mass, std::string _material, std::string name, enums::Stages _stage, Variables_vectors initial_vec);
	virtual void get_next_step();
	std::string get_name() const
	{
		return name;
	}
	bool check_active()
	{
		return active;
	}
	const std::vector<Variables> &get_variables() const
	{
		return variables;
	}
	const Variables &get_last_variables() const
	{
		return variables[ variables.size() - 1 ];
	}
protected:
	std::vector<Variables> variables;
	std::string material;
	std::string name;
	bool active;
	double deployment_level;
	double mass;
	
	// CHANGE THIS FOR NICE GROUND CHECKING
	virtual bool check_for_deployment();
	virtual void add_forces(Variables &next, Variables const &last);
	virtual void add_special_forces(Variables &next, Variables const &last)
	{
		// no special forces as a default beh
	}
	void integrate(Variables &next, Variables const &last);
	
	virtual double get_mass_of_system() = 0;
	
private:
	double mass_of_system; // for rockets it's also with fuel!!!
};

// const mass, starts from non-zero velocity, no-thrust, starts from Free_flight stage
class Solid_ball : public IMovable
{
public:
	Solid_ball(double ms, std::string mat, std::string name, Variables_vectors initial_vec);
private:
	virtual double get_mass_of_system()
	{
		return mass;
	}
};

class Rocket : public IMovable
{
public:
	Rocket(double ms, std::string mat, std::string name, Variables_vectors initial_vec, Rocket_var rv);
private:
	std::vector<Rocket_var> rocket_var;
	virtual double get_mass_of_system()
	{
		return mass + rocket_var[ rocket_var.size() - 1 ].fuel_mass;
	}
	virtual void add_special_forces(Variables &next, Variables const &last);
};


#endif

