#ifndef BALLISTIC_FACTORY_H
#define BALLISTIC_FACTORY_H

#include "movable.h"
#include "ballistic.h"

class Ballistic_factory
{
public:
	static Ballistic_missile *create_ballistic_missile();
};

#endif

