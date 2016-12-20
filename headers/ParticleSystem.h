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
#include "BoxIndicator.h"
#include "CircleIndicator.h"
#include "DomainIndicator.h"

class ParticleSystem {
public:
	ParticleSystem(Paramset* params_in, std::vector<DomainIndicator*> domains_in) {
		params = params_in;
		domains = domains_in;
		placeParticles();
		normalizeMass();
	}
	const static float  PI_F = 3.14159265358979;
	// Boundaries of domain
	const static float XMIN = 0.0;
	const static float XMAX = 1.0;
	const static float YMIN = 0.0;
	const static float YMAX = 1.0;
	const static float scale = 1.0;

	int n;						// Number of particles
	float mass;					// Particle mass

	void leapfrogStep();
	void leapfrogStart();
	void computeAccelerations();
	std::vector<Particle> particles;	  // Vector containing the particle objects
private:
	Paramset* params;					  // Struct containing the system parameters
	std::vector<DomainIndicator*> domains; // Vector containing the domains to fill with particles

	void reflectBC();
	void dampReflect(int which, float barrier, int i);
	void computeDensities();
	void placeParticles();
	void normalizeMass();
};



#endif /* PARTICLESYSTEM_H_ */
