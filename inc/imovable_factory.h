#ifndef IMOVABLE_FACTORY_H
#define IMOVABLE_FACTORY_H

#include "movable.h"

class IMovable_factory
{
public:
	static IMovable *create(int control);
};

#endif

