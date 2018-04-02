#include "ballistic_factory.h"

Ballistic_missile *Ballistic_factory::create_ballistic_missile()
{
	// hardcoded for now
	std::vector<Ballistic_stage*> stages;
	
	Wing wing(0.4, 1.0, 20.0);
	double offset = 0.0;
	Ballistic_stage *stage_0 = new Cylinder_with_wings( 0.5, 100.0, offset,  2.0, 10000.0, 10.0, 1000.0, wing );
	std::cout << stage_0->to_str() << "\n";
	stages.push_back( stage_0 );
	
	// UGLY dynamic cast
	offset += dynamic_cast<Cylinder*>(stage_0)->get_H();
	Ballistic_stage *stage_1 = new Cylinder( 0.5, 100.0, offset, 2.0, 10000.0, 10.0, 1000.0);
	std::cout << stage_1->to_str() << "\n";
	stages.push_back( stage_1 );
	
	// UGLY dynamic cast
	offset += dynamic_cast<Cylinder*>(stage_1)->get_H();
	Ballistic_stage *stage_2 = new Cylinder( 0.5, 100.0, offset, 2.0, 10000.0, 10.0, 1000.0);
	std::cout << stage_2->to_str() << "\n";
	stages.push_back( stage_2 );
	
	// UGLY dynamic cast
	offset += dynamic_cast<Cylinder*>(stage_2)->get_H();
	Ballistic_stage *stage_3 = new Cone( 0.5, 100.0, offset, 4.0);
	std::cout << stage_3->to_str() << "\n";
	stages.push_back( stage_3 );
	
	////////////////
	IMovable_inits inits( 0.0, "steel", "steel_balistic", enums::Rocket_fuel, Variables_vectors( Vector3D(0.0, 0.0, 0.0),
										 														 Vector3D(0.0, 0.0, 0.0), 
																								 Vector3D(0.0, 0.0, 0.0) ) );
	Ballistic_missile *missile = new Ballistic_missile( inits, stages );
	
	return missile;
}
