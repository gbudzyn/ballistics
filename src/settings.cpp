#include "settings.h"

#include <fstream>
#include <sstream>

Settings::Settings()
{
	params.dt = 0.01;
	params.current_time = 0.0;
	
	constants.g_acc = -9.80665;
	constants.pi = 3.14159265359;
	constants.sphere_C_d = 0.47;
	constants.earth_radius = 6371000.0;
	
	// ADD TREADING FROM FILE FOR THIS
	Material_coeffs steel;
	steel.density = 8000;
	steel.thermal_coeff = 13E-06;
	constants.material_coeffs.insert( std::make_pair<std::string, Material_coeffs>("steel", steel) );
	Material_coeffs aluminium;
	aluminium.density = 2700;
	aluminium.thermal_coeff = 24E-06;
	constants.material_coeffs.insert( std::make_pair<std::string, Material_coeffs>("aluminium", aluminium) );
	
	//loading files
	load_file("data/air_temperature.txt", weather.air_temperature);
	load_file("data/air_density.txt", weather.air_density);
}

void Settings::load_file(std::string file_name, std::map<double, double> &container)
{
	std::fstream file;
	file.open( file_name.c_str(), std::ios::in  );
	if( false == file.good() )
	{
		std::string msg("ERROR: file read ");
		msg += file_name;
		throw msg;
	}
	
	std::string line;
	while( std::getline(file, line) )
	{
		std::istringstream iss(line);
		double x_0, x_1;
		if( !(iss >> x_0 >> x_1) ) 
			break;
		
		container.insert( std::make_pair<double, double>(x_0, x_1) );
	}
}

