#ifndef SETTINGS_H
#define SETTINGS_H

#include <map>
#include <string>

class Settings
{
public:
	static Settings& getInstance()
    {
		static Settings instance;
		
		return instance;
    }
    struct Params
	{
		double dt;
		double current_time;
	};
	
	struct Air_table
	{
		double height; // in meters
		double static_pressure; // in Pascals
		double mass_density; // kg/m^3
		double standard_temperature; // in Kelvins
		double temp_lapse_rate;
		Air_table(double _height, double _stat_press, double _mass_dens, double _std_temp, double _tlr)
		 : height(_height),
		   static_pressure(_stat_press),
		   mass_density(_mass_dens),
		   standard_temperature(_std_temp),
		   temp_lapse_rate(_tlr)
		 {}
	};
	
	struct Weather
	{
		std::map<double, Air_table> air_table;
	};
	
	struct Material_coeffs
	{
		double density;
		double thermal_coeff;
	};
	struct Constants
	{
		double g_acc;
		double pi;
		double earth_radius;
		double universal_gas_constant;
		double air_molar_mass;
		std::map<std::string, Material_coeffs > material_coeffs;
	};
	
	const Constants& get_constants() const
    {
    	return constants;
	}
    
    const Weather& get_weather() const
    {
    	return weather;
	}
    
    const Params& get_params() const
    {
    	return params;
	}
	
	void increment_time()
	{
		params.current_time += params.dt;
	}
	
private:
	Settings();
	Settings(Settings const&);
    void operator=(Settings const&);
    Params params;
    Constants constants;
    Weather weather;
    
	void load_file(std::string file_name, std::map<double, double> &container);
	void load_air_table(std::string file_name);
};


#endif

