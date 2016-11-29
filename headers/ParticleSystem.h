/*
 * ParticleSystem.h
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>
#include "Particle.h"
#include "Params.h"

class ParticleSystem {
public:
	ParticleSystem();
	const static float  PI_F = 3.14159265358979f;
	void computeDensities();
	void computeAccelerations();
	void leapfrogStep();
	void leapfrogStart();
	void reflectBC();
	void dampReflect(int which, float barrier, Particle* particle);
private:
	int n;								// Number of particles
	float mass;							// Particle mass
	Paramset* params;					// Struct containing the system parameters
	std::vector<Particle*> particles;	// Vector containing the particle objects
};



#endif /* PARTICLESYSTEM_H_ */
