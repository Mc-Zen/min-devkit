/// @file	
///	@ingroup 	minexamples
///	@copyright	Copyright (c) 2016, Cycling '74
/// @author		Andrew Benson
///	@license	Usage of this file and its contents is governed by the MIT License

#include "c74_min.h"

using namespace c74::min;


class jit_verlet : public object<jit_verlet>, matrix_operator {
public:
	
	inlet	input	= { this, "(matrix) Input", "matrix" };
	outlet	output	= { this, "(matrix) Output", "matrix" };
	
	
	jit_verlet(const atoms& args = {}) {}

	
	// TODO: mode attr for how to handle the edges
	

	attribute<double> distance = { this, "distance", 0.1, MIN_FUNCTION {
		double value = args[0];
		
		if (value < 0.0)
			value = 0.0;
		return {value};
	}};
	
	
	attribute<double> strength = { this, "strength", 0.2, MIN_FUNCTION {
		double value = args[0];
		
		if (value < 0.0)
			value = 0.0;
		return {value};
	}};

	
	template<class matrix_type, size_t planecount>
	cell<matrix_type,planecount> calc_cell(cell<matrix_type,planecount> input, const matrix_info& source, matrix_coord& position) {
		cell<matrix_type,planecount> output;
        long x1 = MIN_CLAMP( position.x() + 1, 0, source.width()-1);
        auto p1 = source.in_cell<matrix_type,planecount>(x1, position.y());
        
        long x2 = MIN_CLAMP( position.x() - 1, 0, source.width()-1);
        auto y1 = MIN_CLAMP( 0, position.y() + 1, source.height()-1);
        auto y2 = MIN_CLAMP( 0, position.y() - 1, source.height()-1);

        //constrain to the right
        double dx1 = input[0]-p1[0];
        double dy1 = input[1]-p1[1];
        double dz1 = input[2]-p1[2];
        
        double len1 = std::fmax(std::sqrt(dx1*dx1+dy1*dy1+dz1*dz1),0.0001);
        double amt1 = ((len1-distance)/len1)*strength;
        double ox1 = input[0]-amt1*dx1;
        double oy1 = input[1]-amt1*dy1;
        double oz1 = input[2]-amt1*dz1;
        
        
        //constrain to the left
        p1 = source.in_cell<matrix_type,planecount>(x2,position.y());
        dx1 = ox1-p1[0];
        dy1 = oy1-p1[1];
        dz1 = oz1-p1[2];
        
        len1 = std::fmax(std::sqrt(dx1*dx1+dy1*dy1+dz1*dz1),0.0001);
        amt1 = ((len1-distance)/len1)*strength;
        
        ox1 = ox1-amt1*dx1;
        oy1 = oy1-amt1*dy1;
        oz1 = oz1-amt1*dz1;
        
        //constrain downward
        p1 = source.in_cell<matrix_type,planecount>(position.x(),y1);
        dx1 = ox1-p1[0];
        dy1 = oy1-p1[1];
        dz1 = oz1-p1[2];
        
        len1 = std::fmax(std::sqrt(dx1*dx1+dy1*dy1+dz1*dz1),0.0001);
        amt1 = ((len1-distance)/len1)*strength;
        
        
        ox1 = ox1-amt1*dx1;
        oy1 = oy1-amt1*dy1;
        oz1 = oz1-amt1*dz1;

        //constrain upward
        p1 = source.in_cell<matrix_type,planecount>(position.x(),y2);
        dx1 = ox1-p1[0];
        dy1 = oy1-p1[1];
        dz1 = oz1-p1[2];
        
        len1 = std::fmax(std::sqrt(dx1*dx1+dy1*dy1+dz1*dz1),0.00001);
        amt1 = ((len1-distance)/len1)*strength;
        
        output[0] = ox1-amt1*dx1;
        output[1] = oy1-amt1*dy1;
        output[2] = oz1-amt1*dz1;

		return output;
	}

};


MIN_EXTERNAL(jit_verlet);
