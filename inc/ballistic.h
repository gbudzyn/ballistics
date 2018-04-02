#ifndef BALLISTIC_H
#define BALLISTIC_H

#include "movable.h"
#include "ballistic_stage.h"

#include <vector>

struct Ballistic_stages
{
	std::vector<Ballistic_stage*> stages;
};

class Ballistic_missile : public IMovable
{
public:
	
private:
	Ballistic_stages ballistic_stages;	
};


#endif

