#include "imovable_factory.h"
#include "ballistic_factory.h"
#include "movable.h"
#include "rocket.h"

IMovable *IMovable_factory::create(int control)
{
    double V_0_x = 400.0;
    double V_0_y = 0.0;
    double V_0_z = 300.0;

    Variables_vectors init_vec( Vector3D(0.0, 0.0, 0.0),
                                Vector3D(V_0_x, V_0_y, V_0_z),
                                Vector3D(0.0,0.0,0.0) );
    IMovable_inits inits(200.0, "aluminium", "alu_ball", enums::Free_flight, init_vec);
    std::vector<Rocket_stage_var> rsv;

    IMovable *created;
    
    // DUMMY VALUES JUST FOR TESTING
    switch(control)
    {
        case 0:
            created = new Solid_ball( inits );
        break;

        case 1:
            rsv.push_back( Rocket_stage_var( 100.0, 99.0, 1.0, Vector3D(5000.0, 0.0, 15000.0) ) );
            rsv.push_back( Rocket_stage_var( 10.0, 21.0, 0.5, Vector3D(4000.0, 0.0, 10000.0) ) );
            rsv.push_back( Rocket_stage_var( 10.0, 20.0, 0.5, Vector3D(1000.0, 0.0, 5000.0) ) );
            inits.stage = enums::Rocket_fuel;
            inits.name = "steel_rocket";
            inits.initial_vec = Variables_vectors( Vector3D(0.0, 0.0, 0.0),
                                                   Vector3D(0.0, 0.0, 0.0),
                                                   Vector3D(0.0, 0.0, 0.0) );
            created = new Rocket( inits, rsv );
        break;

        case 2:
            created = Ballistic_factory::create_ballistic_missile();
        break;

        default:
            std::string msg("Unknown IMovable type");
            throw msg;
    }

    return created;
}

