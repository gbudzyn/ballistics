#include "engine.h"
#include "settings.h"
#include "imovable_factory.h"

Engine::Engine()
{	
    try
    {
    	movables.push_back( IMovable_factory::create(0) );
    	movables.push_back( IMovable_factory::create(1) );
    	movables.push_back( IMovable_factory::create(2) );
    	
    	streamer = new Streamer( movables );
	}
	catch(...)
	{
		throw;
	}
}

void Engine::get_next_step()
{
	Settings::getInstance().increment_time(); //increment time before integration
	for(std::vector<IMovable*>::iterator it = movables.begin() ; it != movables.end() ; ++it )
	{	
		if( false == (*it)->check_active() )
			continue;
		
		(*it)->get_next_step();
		if( (*it)->check_active() )
		{
			any_active = true;
		}
		
		streamer->stream( *it );
	}
}

bool Engine::run()
{
	any_active = false;
	get_next_step();
	
	return any_active;
}

