#include "ballistic_factory.h"

Ballistic_missile *Ballistic_factory::create_ballistic_missile()
{
	// hardcoded for now
	Ballistic_stages bstages;
	
	Wing wing(0.4, 1.0, 20.0);
	Ballistic_stage *stage_0 = new Cylinder_with_wings( 0.5, 100.0, 2.0, 10000.0, 10.0, 1000.0, wing );
	std::cout << stage_0->to_str() << "\n";
	bstages.stages.push_back( stage_0 );
	
	Ballistic_stage *stage_1 = new Cylinder( 0.5, 100.0, 2.0, 10000.0, 10.0, 1000.0);
	std::cout << stage_1->to_str() << "\n";
	bstages.stages.push_back( stage_1 );
	
	Ballistic_stage *stage_2 = new Cylinder( 0.5, 100.0, 2.0, 10000.0, 10.0, 1000.0);
	std::cout << stage_2->to_str() << "\n";
	bstages.stages.push_back( stage_2 );
	
	Ballistic_stage *stage_3 = new Cone( 0.5, 100.0, 2.0);
	std::cout << stage_3->to_str() << "\n";
	bstages.stages.push_back( stage_3 );
	
	////////////////
	IMovable_inits inits( 0.0, "steel", "steel_balistic", enums::Rocket_fuel, Variables_vectors( Vector3D(0.0, 0.0, 0.0),
										 														 Vector3D(0.0, 0.0, 0.0), 
																								 Vector3D(0.0, 0.0, 0.0) ) );
	Ballistic_missile *missile = new Ballistic_missile( inits, bstages );
	
	return missile;
}
