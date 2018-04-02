#ifndef BALLISTIC_H
#define BALLISTIC_H

#include "movable.h"
#include "ballistic_stage.h"

#include <vector>

struct Ballistic_stages
{
	std::vector<Ballistic_stage*> stages;
	double get_mass() const;
};

class Ballistic_missile : public IMovable
{
public:
	Ballistic_missile(IMovable_inits _inits, Ballistic_stages bstages)
	: IMovable( _inits ),
	  ballistic_stages(bstages)
	{}
	virtual double get_mass_of_system() const
	{
		return ballistic_stages.get_mass();
	}
private:
	Ballistic_stages ballistic_stages;	
};


#endif

