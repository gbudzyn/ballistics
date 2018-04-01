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
	const Variables get_current_variables() const
	{
		return variables[current_step];
	}
	virtual double get_mass_of_system() const = 0;
protected:
	std::string material;
	std::string name;
	bool active;
	double deployment_level;
	double mass;
	int current_step;
	std::vector<Variables> variables; // before and after integration
	
	// CHANGE THIS FOR NICE GROUND CHECKING
	virtual bool check_for_deployment();
	void add_forces(Variables &next, Variables const &last);
	void add_gravity(Variables &next, Variables const &last);
	void add_drag_force(Variables &next, Variables const &last);
	virtual void add_special_forces(Variables &next, Variables const &last)
	{
		// no special forces as a default behaviour
	}
	void integrate(Variables &next, Variables const &last);
	
private:
	double mass_of_system; // for rockets it's also with fuel!!!
};

// const mass, starts from non-zero velocity, no-thrust, starts from Free_flight stage
class Solid_ball : public IMovable
{
public:
	Solid_ball(double ms, std::string mat, std::string name, Variables_vectors initial_vec);
private:
	virtual double get_mass_of_system() const
	{
		return mass;
	}
};

#endif

