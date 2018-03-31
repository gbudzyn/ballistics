#include "streamer.h"

void streamer::stream_all(Engine const &engine)
{
	// save to file
	bool brackets = false;
	const std::vector<IMovable*> &movables = engine.get_movables();
	for(std::vector<IMovable*>::const_iterator it = movables.begin() ; it != movables.end() ; ++it)
	{
		std::string file_name("output/test_");
		file_name += (*it)->get_name();
		file_name += ".txt";
		
		std::fstream file_RK(file_name.c_str(), std::ios::out );
		const std::vector<Variables> &variables = (*it)->get_variables();
		for(std::vector<Variables>::const_iterator it_var = variables.begin() ; it_var != variables.end()  ; ++it_var)
		{
			const double &t = it_var->coeffs.time;
			const Vector3D &pos = it_var->vecs.position;
			const Vector3D &vel = it_var->vecs.velocity;
			const Vector3D &force = it_var->vecs.force;
			const double &eff_area = it_var->coeffs.effective_area;
			const double &air_temp = it_var->atm.air_temperature;
			const double &air_dens = it_var->atm.air_density;
			const double &actual_g_acc = it_var->atm.actual_g_acc;
			
			file_RK << t << " " << pos.to_str(brackets) << 
								   vel.to_str(brackets) << 
								   force.to_str(brackets) << 
								   eff_area << " " << 
								   air_temp << " " << 
								   air_dens << " " <<
								   actual_g_acc << " " <<
								   enums::to_string(it_var->coeffs.stage) << "\n";
		}
	}
}

