#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "movable.h"
#include "streamer.h"

class Engine
{
public:
	Engine();
	bool run();
	const std::vector<IMovable*> &get_movables() const
	{
		return movables;
	}
private:
	std::vector<IMovable*> movables;
	Streamer *streamer;
	bool any_active;	
	void get_next_step();
};

#endif

