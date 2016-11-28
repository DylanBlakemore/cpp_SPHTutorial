/*
 * ParticleSystem.cpp
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */
#include "../headers/ParticleSystem.h"

void ParticleSystem::computeDensities() {
	float C = 4 * mass / (PI_F * (params->h)^8);

	for(int i = 0; i < n; i++) {
		particles[i]->density = 0.0; // Reset density to 0
		particles[i]->density += 4 * mass / (PI_F * (params->h)^2); // Contribution of particle's density to itself

		for(int j = i+1; j < n; j++) {
			float r2 = particles[i]->computeSquaredDistanceTo(particles[j]);
			float z = (params->h)^2 - r2;

			if(z > 0) {
				float rho_ij = C * z^3;
				particles[i]->density += rho_ij;
				particles[j]->density += rho_ij;
			}
		}
	}
}

void ParticleSystem::computeAccelerations() {
	// For simplicity, parameters are unpacked from Paramset struct
	const float h 		= params->h;
	const float rho_0 	= params->rho_0;
	const float k 		= params->k;
	const float mu		= params->mu;
	const float g		= params->g;

	computeDensities();

	// Add gravitational force
	for(int i = 0; i < n; i++) {
		particles[i]->a[0] = 0;
		particles[i]->a[1] = -g;
	}

	// Constants for interaction term
	float C_0 = mass / (PI_F * h^4);
	float C_p = 15 * k;
	float C_v = -40 * mu;

	// Compute interactions
	for(int i = 0; i < n; i++) {
		const float rho_i = particles[i]->density;
		for(int j = i+1; j < n; j++) {
			float r2 = particles[i]->computeSquaredDistanceTo(particles[j]);
			if(r2 < h^2) {
				const float rho_j = particles[j]->density;
				float q = sqrt(r2)/h;
				float u = 1 - q;
				float w_0 = C_0 * u/(rho_i * rho_j);
			}
		}
	}
}


