#ifndef BALLISTIC_H
#define BALLISTIC_H

#include "movable.h"
#include "ballistic_stage.h"

#include <vector>

class Ballistic_missile : public IMovable
{
public:
	Ballistic_missile(IMovable_inits _inits, std::vector<Ballistic_stage*> bstages);
	virtual double get_mass_of_system() const;
	double get_pressure_area() const;
	double get_center_of_mass_Z() const;
	double get_center_of_pressure_Z() const;
private:
	std::vector<Ballistic_stage*> stages;
};


#endif

