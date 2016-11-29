/*
 * Particle.h
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

class Particle {
public:
	Particle(float x_in, float y_in) {
		x[0] = x_in;
		x[1] = y_in;
		v[0] = 0;
		v[1] = 0;
	}
	float computeSquaredDistanceTo(const Particle* in_part);
	float density;	// Density
	float x[2];		// Position
	float v[2];		// Velocity (full step)
	float vh[2];	// Velocity (half step)
	float a[2];		// Acceleration
};



#endif /* PARTICLE_H_ */
