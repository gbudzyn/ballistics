#include "engine.h"
#include "settings.h"

Engine::Engine()
{
	bool any_active = true; 
	
	double V_0_x = 400.0;
    double V_0_y = 0.0;
    double V_0_z = 300.0;
    
    Variables_vectors init_vec( Vector3D(),
								Vector3D(V_0_x, V_0_y, V_0_z),
								Vector3D() );
	
    try
    {
    	IMovable *ball1 = new Solid_ball( 200.0, 
										  "aluminium", 
										  "alu_ball", 
										  init_vec );
    	movables.push_back(ball1);
    	
    	IMovable *ball2 = new Solid_ball( 200.0, 
										  "steel", 
										  "steel_ball", 
										  init_vec );
    	movables.push_back(ball2);
    	
    	std::vector<Rocket_stage_var> rsv;
    	rsv.push_back( Rocket_stage_var( 10.0, 22.0, 0.5, Vector3D(4000.0, 0.0, 6000.0) ) );
    	rsv.push_back( Rocket_stage_var( 10.0, 21.0, 0.5, Vector3D(400.0, 0.0, 5000.0) ) );
    	rsv.push_back( Rocket_stage_var( 10.0, 20.0, 0.5, Vector3D(1000.0, 0.0, 4000.0) ) );
    	IMovable *rocket1 = new Rocket( 200.0, 
										"steel", 
										"steel_rocket", 
										Variables_vectors( Vector3D(), Vector3D(), Vector3D() ), 
										rsv);
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

