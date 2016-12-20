/*
 * Params.h
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include <string>

struct Paramset {
public:
	Paramset() {
		// Default parameters
		f_name 		= "sph.out";
		n_frames 	= 400;
		np_frame 	= 100;
		n_particles = 1000;
		dt 			= 1e-4;
		h			= 3e-2;
		rho_0		= 1000;
		k			= 1000;
		mu			= 0.001;
		g			= -9.8;
		c			= 10;
		sigma		= 1;
	}
	std::string f_name;	// File name
	int n_frames;		// Number of frames
	int np_frame;		// Steps per frame
	int n_particles;		// Number of particles
	float h;			// Particle neighbour size
	float dt;			// Time step size
	float rho_0;		// Reference density
	float k;			// Bulk modulus
	float mu;			// Viscosity
	float g;			// Gravity
	float c;			// Speed of sound
	float sigma;		// Surface tension coefficient

	void load(){} // possible code to read parameters from file
};



#endif /* PARAMS_H_ */
