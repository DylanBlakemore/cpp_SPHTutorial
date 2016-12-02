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
#include "../headers/OutputFile.h"
#include "../headers/inputfile.h"

using namespace std;

void displayPositions(ParticleSystem* p_sys) {
	for(int i = 0; i < p_sys->n; i++) {
		std::cout << p_sys->particles[i].x[0] << "," << p_sys->particles[i].x[1] << std::endl;
	}
}

void checkState(ParticleSystem* sys, OutputFile* outfile, int frame, int step) {
	/* If an error occurs, close the output file to avoid corruption of existing data */
	for(int i = 0; i < sys->n; i++) {
		if(!(sys->particles[i].x[0] >= 0 && sys->particles[i].x[0] <= 1) || !(sys->particles[i].x[1] >= 0 && sys->particles[i].x[1] <= 1)) {
			outfile->close();
			std::cout << "--------------------------------------" << std::endl;
			std::cout << "Error in frame " << frame + 1 <<", step " << step << std::endl;
			std::cout << "--------------------------------------" << std::endl;
		}
		/* Submit error */
		assert(sys->particles[i].x[0] >= 0 && sys->particles[i].x[0] <= 1);
		assert(sys->particles[i].x[1] >= 0 && sys->particles[i].x[1] <= 1);
	}
}

int main() {
	Paramset params;
	params.load(); // Still need to complete

	// Create domains in which to place particles and initialize the ParticleSystem object
	vector<DomainIndicator*> domains;

	BoxIndicator box = BoxIndicator(0.05,0.5,0.5,0.9);
	domains.push_back(&box);
	/////////////////////////////////////////////////////////////////////////////////////
	ParticleSystem p_sys(&params, domains);

	OutputFile outfile(&params, &p_sys);
	outfile.writeHeader();

	int n_frames = params.n_frames;
	int np_frame = params.np_frame;

	std::cout << "Processing initial state" << std::endl;

	checkState(&p_sys, &outfile, 0, 0);

	p_sys.computeAccelerations();
	p_sys.leapfrogStart();
	outfile.writeFrame();

	for(int frame = 0; frame < n_frames; frame++) {
		std::cout << "Processing frame " << frame + 1 << std::endl;
		//displayPositions(&p_sys);
		for(int i = 0; i < np_frame; i++) {
			p_sys.computeAccelerations();
			p_sys.leapfrogStep();
			//checkState(&p_sys, &outfile, frame, i);
		}
		outfile.writeFrame();
	}
	outfile.close();
	std::cout << "All frames processed" << std::endl;

	// Write more readable csv file.
	InputFile infile(params.f_name);
	return 0;
}
