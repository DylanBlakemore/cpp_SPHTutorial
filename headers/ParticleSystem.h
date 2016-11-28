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

class ParticleSystem {
public:
	ParticleSystem();
private:
	int n;								// Number of particles
	std::vector<Particle*> particles;	// Vector containing the particle objects
};



#endif /* PARTICLESYSTEM_H_ */
