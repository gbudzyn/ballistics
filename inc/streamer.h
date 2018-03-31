#ifndef STREAMER_H
#define STREAMER_H

#include <fstream>
#include <map>

#include "movable.h"

class Streamer
{
public:
	Streamer(std::vector<IMovable*> const &movables);
	void stream(const IMovable * const movable);
	~Streamer();
private:
	std::map<const IMovable* const, std::fstream*> files;
};


#endif

