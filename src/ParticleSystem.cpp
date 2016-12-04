/*
 * ParticleSystem.cpp
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */
#include <math.h>
#include <iostream>
#include "../headers/ParticleSystem.h"

float ParticleSystem::weightFunction(float r, float h) {
	float func;
	float normal = 10/(7 * PI_F);
	float q = r/h;

	if (q >= 2)
		func = 0;
	else if (q >= 0 && q < 1)
		func = 0.25 * pow(2-q,3) - pow(1-q,3);
	else if (q >= 1 && q < 2)
		func = 0.25 * pow(2-q,3);

	return func * normal;
}

float ParticleSystem::weightGradient(Particle* p1, Particle* p2, int dim, float h) {
	float gradient = 0;

	float dx = p1->x[0] - p2->x[0];
	float dy = p1->x[1] - p2->x[1];
	float r	 = sqrt(dx*dx + dy*dy);
	float q = r/h;

	if(q >= 2)
		gradient = 0;
	else if(q >= 1 && q < 2)
		gradient = ((-3*pow(2 - q,2)) / (4*h)) * (p1->x[dim] - p2->x[dim])/r;
	else if(q >= 0 && q < 1) {
		gradient = ( (-3*pow(2 - q,2) / (4*h)) +
					(-3*pow(1 - q,2)/h) ) * (p1->x[dim] - p2->x[dim])/r;
	}

	return gradient;
}

void ParticleSystem::computeDensities() {
	float h = params->h;
	float C = 4 * mass/(PI_F * pow(h,8));

	for(int i = 0; i < n; i++) {
		particles[i].density = 0.0; // Reset density to 0
		particles[i].density += 4 * mass / (PI_F * pow(h,2)); // Contribution of particle's density to itself

		for(int j = 0; j < n; j++) {
			float dx = particles[i].x[0] - particles[j].x[0];
			float dy = particles[i].x[1] - particles[j].x[1];
			float r2 = dx*dx + dy*dy;
			float z = pow(h,2) - r2;
			if(z > 0) {
				float rho_ij = C*z*z;
				particles[i].density += rho_ij;
				//particles[j].density += rho_ij;
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
		particles[i].a[0] = 0;
		particles[i].a[1] = -g;
	}

	// Constants for interaction term
	float C_0 = mass / (PI_F * pow(h,4));
	float C_p = 15 * k;
	float C_v = -40 * mu;

	// Compute interactions
	for(int i = 0; i < n; i++) {
		const float rho_i = particles[i].density;
		for(int j = i+1; j < n; j++) {
			float dx = particles[i].x[0] - particles[j].x[0];
			float dy = particles[i].x[1] - particles[j].x[1];
			float r2 = dx*dx + dy*dy;
			if(r2 < pow(h,2)) {
				const float rho_j = particles[j].density;
				float q = sqrt(r2)/h;
				float u = 1 - q;
				float w_0 = C_0 * u/(rho_j);
				float w_p = w_0 * C_p * (rho_i + rho_j - 2 * rho_0) * u/q;
				float w_v = w_0 * C_v;
				float dv_x = particles[i].v[0] - particles[j].v[0];
				float dv_y = particles[i].v[1] - particles[j].v[1];
				particles[i].a[0] += (w_p*dx + w_v*dv_x);
				particles[i].a[1] += (w_p*dy + w_v*dv_y);
				particles[j].a[0] -= (w_p*dx + w_v*dv_x);
				particles[j].a[1] -= (w_p*dy + w_v*dv_y);
			}
		}
	}
}

void ParticleSystem::leapfrogStep() {
	float dt = params->dt;
	// Update half-step velocity
	for(int i = 0; i < n; i++) {
		particles[i].vh[0] += particles[i].a[0] * dt;
		particles[i].vh[1] += particles[i].a[1] * dt;
	}
	// Update full-step velocity
	for(int i = 0; i < n; i++) {
		particles[i].v[0] += particles[i].vh[0] + particles[i].a[0] * dt/2;
		particles[i].v[1] += particles[i].vh[1] + particles[i].a[1] * dt/2;
	}
	// Update position
	for(int i = 0; i < n; i++) {
		particles[i].x[0] += particles[i].vh[0] * dt;
		particles[i].x[1] += particles[i].vh[1] * dt;
	}

	reflectBC();
}

void ParticleSystem::leapfrogStart() {
	float dt = params->dt;
	// Update half-step velocity
	for(int i = 0; i < n; i++) {
		particles[i].vh[0] = particles[i].v[0] + particles[i].a[0] * dt/2;
		particles[i].vh[1] = particles[i].v[1] + particles[i].a[1] * dt/2;
	}
	// Update full-step velocity
	for(int i = 0; i < n; i++) {
		particles[i].v[0] += particles[i].a[0] * dt;
		particles[i].v[1] += particles[i].a[1] * dt;
	}
	// Update position
	for(int i = 0; i < n; i++) {
		particles[i].x[0] += particles[i].vh[0] * dt;
		particles[i].x[1] += particles[i].vh[1] * dt;
	}

	reflectBC();
}

void ParticleSystem::reflectBC() {
	for(int i = 0; i < n; i++) {
		if(particles[i].x[0] < XMIN) dampReflect(0, XMIN, i);
		if(particles[i].x[0] > XMAX) dampReflect(0, XMAX, i);;
		if(particles[i].x[1] < YMIN) dampReflect(1, YMIN, i);;
		if(particles[i].x[1] > YMAX) dampReflect(1, YMAX, i);;
	}
}

/* Arguments:
 *
 * which: 		0 (vertical barrier) or 1 (horizontal barrier)
 * barrier: 	position of barrier
 * particle:	particle under consideration
 */
void ParticleSystem::dampReflect(int which, float barrier, int i) {
	// Coefficient of restitution
	const float DAMP = 0.75;

	// Ignore degenerate cases
	if(particles[i].v[which] == 0) {
		return;
	}
	//std::cout << "D" << which << " before: " << particles[i].x[which] << std::endl;
	// Scale back distance traveled based on time from collision
	float t_bounce = (particles[i].x[which] - barrier)/particles[i].v[which];
	particles[i].x[0] -= particles[i].v[0] * (1 - DAMP) * t_bounce;
	particles[i].x[1] -= particles[i].v[1] * (1 - DAMP) * t_bounce;

	// Reflect position and velocity
	particles[i].x[which] = 2 * barrier - particles[i].x[which];
	particles[i].v[which] = -1 * particles[i].v[which];
	particles[i].vh[which] = -1 * particles[i].vh[which];

	// Damp the velocities
	particles[i].v[0] *= DAMP; particles[i].vh[0] *= DAMP;
	particles[i].v[1] *= DAMP; particles[i].vh[1] *= DAMP;

	//std::cout << "D" << which << " after: " << particles[i].x[which] << std::endl;
	//std::cout << "==================================" << std::endl;
}

void ParticleSystem::placeParticles() {
	float h = params->h;
	float hh = h/1.3;

	int count = 0;

	for(float x = 0; x < 1.0; x += hh) {
		for(float y = 0; y < 1.0; y += hh) {
			/* Determine if the point (x,y) is inside one of  the domains specified
			 * by looping over the domains and testing for each.
			 * An OR statement is used to determine the end result (i.e. if it is
			 * inside at least one domain then is_inside = 1).
			 */
			int num_domains = domains.size();
			int is_inside = 0;
			for(int i = 0; i < num_domains; i++)
				is_inside = is_inside || domains[i]->contains(x,y);

			/* If the point falls inside one of the domains, create a particle and
			 * add it to the vector of particles.
			 */
			if(is_inside) {
				Particle new_part = Particle(x, y);
				particles.push_back(new_part);
			}
			count++;
		}
	}
	n = particles.size();
	std::cout << "Number of domains: " << domains.size() << std::endl;
	std::cout << "Number of cells: " << count << std::endl;
	std::cout << "Number of particles created: " << n << std::endl;
}

void ParticleSystem::normalizeMass() {
	mass = 1;
	computeDensities();
	float rho_0 = params->rho_0;
	float rho_av = 0;

	for(int i = 0; i < n; i++) {
		rho_av += particles[i].density;
	}
	rho_av = rho_av / n;
	mass = mass * rho_0/rho_av;
}

