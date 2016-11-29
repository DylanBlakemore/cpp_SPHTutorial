//============================================================================
// Name        : cpp_SPHTutorial.cpp
// Author      : Dylan Blakemore
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <assert.h>

#include "../headers/BoxIndicator.h"
#include "../headers/CircleIndicator.h"
#include "../headers/DomainIndicator.h"
#include "../headers/Params.h"
#include "../headers/Particle.h"
#include "../headers/ParticleSystem.h"

using namespace std;

void checkState(ParticleSystem* sys) {
	const std::vector<Particle>* particles = sys->getParticles();
	for(int i = 0; i < sys->n; i++) {
		Particle part = (&particles)[i];
		float xi = part.x[0];
		float yi = part.x[1];
		assert(xi >= 0 && xi <= 1);
		assert(yi >= 0 && yi <= 1);
	}
}

ParticleSystem initParticles(Paramset* params) {
	return 0;
}

int main() {
	Paramset params;
	params.load(); // Still need to complete

	ParticleSystem p_sys = initParticles(&params);

	int n_frames = params.n_frames;
	int np_frame = params.np_frame;
	float dt = params.dt;
	int n = p_sys.n;

	p_sys.writeState(); // Still need to complete
	p_sys.computeAccelerations();
	p_sys.leapfrogStart();
	checkState(&p_sys);

	for(int frame = 1; frame < n_frames; frame++) {
		for(int i = 0; i < np_frame; i++) {
			p_sys.computeAccelerations();
			p_sys.leapfrogStep();
			checkState(&p_sys);
		}
		p_sys.writeState();
	}

	return 0;
}
