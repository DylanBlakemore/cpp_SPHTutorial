/*
 * ParticleSystem.cpp
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */
#include <math.h>
#include "../headers/ParticleSystem.h"

void ParticleSystem::computeDensities() {
	float C = 4 * mass / (PI_F * pow(params->h,8));

	for(int i = 0; i < n; i++) {
		particles[i]->density = 0.0; // Reset density to 0
		particles[i]->density += 4 * mass / (PI_F * pow(params->h,2)); // Contribution of particle's density to itself

		for(int j = i+1; j < n; j++) {
			float r2 = particles[i]->computeSquaredDistanceTo(particles[j]);
			float z = pow(params->h,2) - r2;

			if(z > 0) {
				float rho_ij = C * pow(z,3);
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
	float C_0 = mass / (PI_F * pow(h,4));
	float C_p = 15 * k;
	float C_v = -40 * mu;

	// Compute interactions
	for(int i = 0; i < n; i++) {
		const float rho_i = particles[i]->density;
		for(int j = i+1; j < n; j++) {
			float dx = particles[i]->x[0] - particles[j]->x[0];
			float dy = particles[i]->x[1] - particles[j]->x[1];
			float r2 = dx*dx + dy*dy;
			if(r2 < pow(h,2)) {
				const float rho_j = particles[j]->density;
				float q = sqrt(r2)/h;
				float u = 1 - q;
				float w_0 = C_0 * u/(rho_i * rho_j);
				float w_p = w_0 * C_p * (rho_i + rho_j - 2 * rho_0) * u/q;
				float w_v = w_0 * C_v;
				float dv_x = particles[i]->v[0] - particles[j]->v[0];
				float dv_y = particles[i]->v[1] - particles[j]->v[1];
				particles[i]->a[0] += (w_p*dx + w_v*dv_x);
				particles[i]->a[1] += (w_p*dy + w_v*dv_y);
				particles[j]->a[0] += (w_p*dx + w_v*dv_x);
				particles[j]->a[1] += (w_p*dy + w_v*dv_y);
			}
		}
	}
}

void ParticleSystem::leapfrogStep() {
	float dt = params->dt;
	// Update half-step velocity
	for(int i = 0; i < n; i++) {
		particles[i]->vh[0] += particles[i]->a[0] * dt;
		particles[i]->vh[1] += particles[i]->a[1] * dt;
	}
	// Update full-step velocity
	for(int i = 0; i < n; i++) {
		particles[i]->v[0] += particles[i]->a[0] * dt/2;
		particles[i]->v[1] += particles[i]->a[1] * dt/2;
	}
	// Update position
	for(int i = 0; i < n; i++) {
		particles[i]->x[0] += particles[i]->vh[0] * dt;
		particles[i]->x[1] += particles[i]->vh[1] * dt;
	}

	reflectBC();
}

void ParticleSystem::leapfrogStart() {
	float dt = params->dt;
	// Update half-step velocity
	for(int i = 0; i < n; i++) {
		particles[i]->vh[0] = particles[i]->v[0] + particles[i]->a[0] * dt/2;
		particles[i]->vh[1] = particles[i]->v[1] + particles[i]->a[1] * dt/2;
	}
	// Update full-step velocity
	for(int i = 0; i < n; i++) {
		particles[i]->v[0] += particles[i]->a[0] * dt;
		particles[i]->v[1] += particles[i]->a[1] * dt;
	}
	// Update position
	for(int i = 0; i < n; i++) {
		particles[i]->x[0] += particles[i]->vh[0] * dt;
		particles[i]->x[1] += particles[i]->vh[1] * dt;
	}

	reflectBC();
}

void ParticleSystem::reflectBC() {
	// Boundaries of domain
	const float XMIN = 0.0;
	const float XMAX = 1.0;
	const float YMIN = 0.0;
	const float YMAX = 1.0;

	for(int i = 0; i < n; i++) {
		if(particles[i]->x[0] < XMIN) dampReflect(0, XMIN, particles[i]);
		if(particles[i]->x[0] > XMAX) dampReflect(0, XMAX, particles[i]);;
		if(particles[i]->x[1] < YMIN) dampReflect(1, YMIN, particles[i]);;
		if(particles[i]->x[1] < YMAX) dampReflect(1, YMAX, particles[i]);;
	}
}

/* Arguments:
 *
 * which: 		0 (vertical barrier) or 1 (horizontal barrier)
 * barrier: 	position of barrier
 * particle:	particle under consideration
 */
void ParticleSystem::dampReflect(int which, float barrier, Particle* particle) {
	// Coefficient of restitution
	const float DAMP = 0.75;

	// Ignore degenerate cases
	if(particle->v[which] == 0)
		return;

	// Scale back distance traveled based on time from collision
	float t_bounce = (particle->x[which] - barrier)/particle->v[which];
	particle->x[0] -= particle->v[0] * (1 - DAMP) * t_bounce;
	particle->x[1] -= particle->v[1] * (1 - DAMP) * t_bounce;

	// Reflect position and velocity
	particle->x[which] = 2 * barrier - particle->x[which];
	particle->v[which] = -1 * particle->v[which];
	particle->vh[which] = -1 * particle->vh[which];

	// Damp the velocities
	particle->v[0] *= DAMP; particle->vh[0] *= DAMP;
	particle->v[1] *= DAMP; particle->vh[1] *= DAMP;
}

