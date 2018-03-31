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
	struct Weather
	{
		std::map<double, double> air_temperature;
		std::map<double, double> air_pressure;
		std::map<double, double> air_density;
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
		double sphere_C_d;
		double earth_radius;
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
};


#endif

