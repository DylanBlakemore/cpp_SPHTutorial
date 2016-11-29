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
	ParticleSystem(Paramset* params_in, std::vector<DomainIndicator>& domains_in) {
		params = params_in;
		domains = domains_in;
		placeParticles();
		normalizeMass();
	}
	const static float  PI_F = 3.14159265358979f;
	int n;
	void leapfrogStep();
	void leapfrogStart();
	void writeHeader();
	void writeState();
	const std::vector<Particle>* getParticles(){return &particles;}
private:							  // Number of particles
	float mass;							  // Particle mass
	Paramset* params;					  // Struct containing the system parameters
	std::vector<Particle> particles;	  // Vector containing the particle objects
	std::vector<DomainIndicator> domains; // Vector containing the domains to fill with particles

	void reflectBC();
	void dampReflect(int which, float barrier, Particle* particle);
	void computeDensities();
	void computeAccelerations();
	void placeParticles();
	void normalizeMass();
};



#endif /* PARTICLESYSTEM_H_ */
