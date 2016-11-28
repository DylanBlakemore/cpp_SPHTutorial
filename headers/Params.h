/*
 * Params.h
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */

#ifndef PARAMS_H_
#define PARAMS_H_

struct Params {
public:
	std::string f_name;	// File name
	int n_frames;		// Number of frames
	int np_frame;		// Steps per frame
	float h;			// Particle size
	float dt;			// Time step size
	float rho_0;		// Reference density
	float k;			// Bulk modulus
	float mu;			// Viscosity
	float g;			// Gravity
};



#endif /* PARAMS_H_ */
