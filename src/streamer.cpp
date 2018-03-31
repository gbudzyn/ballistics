#include "streamer.h"

#include "settings.h"

Streamer::Streamer(std::vector<IMovable*> const &movables)
{
	// open all files
	for(std::vector<IMovable*>::const_iterator it = movables.begin() ; it != movables.end() ; ++it )
	{
		std::string file_name("output/test_");
		file_name += (*it)->get_name();
		file_name += ".txt";
		files[ *it ] = new std::fstream(file_name.c_str(), std::ios::out);
	}
}

void Streamer::stream(const IMovable * const movable)
{
	std::map<const IMovable * const, std::fstream *>::iterator it = files.find(movable);
	std::fstream *file = it->second;
	if( it == files.end() )
	{
		std::string msg("This should not happen");
		throw msg;
	}
	
	*file << Settings::getInstance().get_params().current_time << " " << 
	         movable->get_current_variables().to_str(false) << "\n";
}

Streamer::~Streamer()
{
	for( std::map<const IMovable * const, std::fstream *>::iterator it = files.begin() ; it != files.begin() ; ++it )
	{
		it->second->close();
		delete it->second;
	}
}

