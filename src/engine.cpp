#include "engine.h"
#include "settings.h"

Engine::Engine()
{
	bool any_active = true; 
	
	double V_0_x = 400.0;
    double V_0_y = 0.0;
    double V_0_z = 300.0;
    
    Variables_vectors init_vec( Vector3D(0.0, 0.0, 0.0),
								Vector3D(V_0_x, V_0_y, V_0_z),
								Vector3D(0.0,0.0,0.0) );
	
    try
    {
    	IMovable_inits inits(200.0, "aluminium", "alu_ball", enums::Free_flight, init_vec);
    	IMovable *ball1 = new Solid_ball( inits );
    	movables.push_back(ball1);

    	inits.material = "steel";
    	inits.name = "steel_ball";
    	IMovable *ball2 = new Solid_ball( inits );
    	movables.push_back(ball2);
    	
    	std::vector<Rocket_stage_var> rsv;
    	rsv.push_back( Rocket_stage_var( 100.0, 99.0, 1.0, Vector3D(0.0, 0.0, 20000.0) ) );
    	rsv.push_back( Rocket_stage_var( 10.0, 21.0, 0.5, Vector3D(0.0, 0.0, 10000.0) ) );
    	rsv.push_back( Rocket_stage_var( 10.0, 20.0, 0.5, Vector3D(0.0, 0.0, 5000.0) ) );
    	inits.stage = enums::Rocket_fuel;
    	inits.name = "steel_rocket";
    	inits.initial_vec = Variables_vectors( Vector3D(0.0, 0.0, 0.0), 
											   Vector3D(0.0, 0.0, 0.0), 
											   Vector3D(0.0, 0.0, 0.0) );
    	IMovable *rocket1 = new Rocket( inits, rsv );
    	movables.push_back(rocket1);
    	
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

