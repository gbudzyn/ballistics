#include "settings.h"

#include <fstream>
#include <sstream>

Settings::Settings()
{
	params.dt = 0.01;
	params.current_time = 0.0;
	
	constants.g_acc = 9.80665;
	constants.pi = 3.14159265359;
	constants.sphere_C_d = 0.47;
	constants.earth_radius = 6371000.0;
	constants.universal_gas_constant = 8.3144598;
	constants.air_molar_mass = 0.0289644;
	
	// ADD TREADING FROM FILE FOR THIS
	Material_coeffs steel;
	steel.density = 8000;
	steel.thermal_coeff = 13E-06;
	constants.material_coeffs.insert( std::make_pair<std::string, Material_coeffs>("steel", steel) );
	Material_coeffs aluminium;
	aluminium.density = 2700;
	aluminium.thermal_coeff = 24E-06;
	constants.material_coeffs.insert( std::make_pair<std::string, Material_coeffs>("aluminium", aluminium) );
	
	load_air_table("data/air_table.txt");
}

void Settings::load_air_table(std::string file_name)
{
	std::fstream file;
	file.open( file_name.c_str(), std::ios::in );
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
		int index;
		double x_0, x_1, x_2, x_3, x_4;
		if( !(iss >> index >> x_0 >> x_1 >> x_2 >> x_3 >> x_4) ) 
			break;
		
		weather.air_table.insert( std::make_pair<double, Air_table>(x_0,  Air_table( x_0, x_1, x_2, x_3, x_4 ) ) );
	}
}

