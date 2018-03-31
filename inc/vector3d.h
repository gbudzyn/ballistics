#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cassert>
#include <sstream>

struct Vector3D
{	
	Vector3D(double x0 = 0.0, double x1 = 0.0, double x2 = 0.0)
	{
		x[0] = x0; 
		x[1] = x1; 
		x[2] = x2;
	}
	
	std::string to_str(bool brackets = false) const
	{
		std::stringstream ret;
		ret << (brackets ? "(" : " ")<< x[0] << (brackets ? ", " :  " ") << x[1] << (brackets ? ", " : " ") << x[2] << (brackets ? ") " : " "); 
		return ret.str();
	}
	
	double const &get_x() const
	{
		return *x;
	}
	
	double &operator[] (int pos)
	{
		assert(pos < 3 && pos >= 0);
		return x[pos];
	}
	
	double operator[] (int pos) const
	{
		assert(pos < 3 && pos >= 0);
		return x[pos];
	}
private:
	double x[3];
};

#endif

