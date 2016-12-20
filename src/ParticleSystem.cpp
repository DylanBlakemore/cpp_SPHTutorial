/*
 * ParticleSystem.cpp
 *
 *  Created on: 28 Nov 2016
 *      Author: dylan
 */
#include <math.h>
#include <iostream>
#include "../headers/ParticleSystem.h"
#include "../headers/PressureKernel.h"
#include "../headers/DefaultKernel.h"
#include "../headers/ViscosityKernel.h"

void ParticleSystem::computeDensities() {
	float h = params->h;
	DefaultKernel kernel;

	for(int i = 0; i < n; i++) {
		particles[i].density = 0.0; // Reset density to 0

		for(int j = 0; j < n; j++) {
			float dx = particles[i].x[0] - particles[j].x[0];
			float dy = particles[i].x[1] - particles[j].x[1];
			float r2 = dx*dx + dy*dy;
			float z = pow(h,2) - r2;
			if(z > 0 && i != j) {
				float weight;
				kernel.computeWeight(&particles[i], &particles[j], h, &weight);
				float rho_ij = mass * weight;
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
	const float mu		= params->mu;
	const float g		= params->g;
	const float c		= params->c;
	const float sigma	= params->sigma;

	computeDensities();

	PressureKernel p_kernel;
	ViscosityKernel v_kernel;
	DefaultKernel d_kernel;


	// Compute interactions
	for(int i = 0; i < n; i++) {
		/* Gravity */
		particles[i].a[0] = 0;
		particles[i].a[1] = g;

		float rho_i = particles[i].density;
		float p_i = pow(c,2) * (rho_i - rho_0);

		for(int j = 0; j < n; j++) {
			float dx = particles[i].x[0] - particles[j].x[0];
			float dy = particles[i].x[1] - particles[j].x[1];
			float r2 = dx*dx + dy*dy;

			if(r2 < pow(h,2) && r2 > 0) {
			/* Pressure term */
				float rho_j = particles[j].density;
				float p_j = pow(c,2) * (rho_j - rho_0);
				float grad_p[2];
				d_kernel.computeGradient(&particles[i], &particles[j], h, grad_p);
				float p_term = -1 * mass * (p_i/pow(rho_i,2) + p_j/pow(rho_j,2));
				particles[i].a[0] += (p_term * grad_p[0]);
				particles[i].a[1] += (p_term * grad_p[1]);
			/* Viscosity Term */
				float grad2_v;
				d_kernel.computeLaplacian(&particles[i], &particles[j], h, &grad2_v);
				float v_term = mu * mass * grad2_v / (rho_j * rho_i);
				particles[i].a[0] += v_term * (particles[i].v[0] - particles[j].v[0]);
				particles[i].a[1] += v_term * (particles[i].v[1] - particles[j].v[1]);
			/* Surface Tension */
				/*float n[2];
				float grad2_c;
				d_kernel.computeGradient(&particles[i], &particles[j], h, n);
				d_kernel.computeLaplacian(&particles[i], &particles[j], h, &grad2_c);
				float mod_n = sqrt(n[0]*n[0] + n[1]*n[1]);
				float st_const = -1 * sigma * grad2_c / mod_n;
				particles[i].a[0] += st_const * (n[0]) /rho_i;
				particles[i].a[1] += st_const * (n[1]) /rho_i;*/
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
	float hh = h/2;

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

