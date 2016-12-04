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
#include <csv.h>

#include "../headers/BoxIndicator.h"
#include "../headers/CircleIndicator.h"
#include "../headers/DomainIndicator.h"
#include "../headers/Params.h"
#include "../headers/Particle.h"
#include "../headers/ParticleSystem.h"
#include "../headers/OutputFile.h"
#include "../headers/inputfile.h"

using namespace std;

/* Write the positions of each of the particles in the system to the terminal. */
void displayPositions(ParticleSystem* p_sys) {
	for(int i = 0; i < p_sys->n; i++) {
		std::cout << p_sys->particles[i].x[0] << "," << p_sys->particles[i].x[1] << std::endl;
	}
}

/* Check the state of the system, i.e. check that each particle is within the boundaries. */
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

/* Using the info in the binary file, create a csv file with only
 * the particle information.
 */
void createCSVFile(CSVFile* csvfile, InputFile* ifile) {
	/* Move the pointer to after the header */
	ifile->initParticleRead();
	std::vector<std::vector<float> > csv_data;

	int pID;
	do {
		particle_details pd = ifile->readParticle();
		pID = pd.id;
		if(pID == -2)
			break;
		std::vector<float> row;
		row.resize(8);
		row[0] = (float)pd.id;
		row[1] = pd.x;
		row[2] = pd.y;
		row[3] = pd.vx;
		row[4] = pd.vy;
		row[5] = pd.ax;
		row[6] = pd.ay;
		row[7] = pd.rho;
		csv_data.push_back(row);
	}while(pID != -2);
	csvfile->open('w');
	csvfile->write(&csv_data);
	csvfile->close();
}

int main() {
	Paramset params;
	params.load(); // Still need to complete

	/* Create domains in which to place particles and initialize the ParticleSystem object */
	vector<DomainIndicator*> domains;

	BoxIndicator box = BoxIndicator(0.0,0.3,0.0,0.6);
	domains.push_back(&box);
	/* ----------------------------------------------------------------------------------- */

	ParticleSystem p_sys(&params, domains);

	OutputFile outfile(&params, &p_sys);
	outfile.writeHeader();

	int n_frames = params.n_frames;
	int np_frame = params.np_frame;

	std::cout << "Processing initial state" << std::endl;

	checkState(&p_sys, &outfile, 0, 0);

	p_sys.computeAccelerations();
	/* Determine the average density. Should be
	 * roughly equal to the reference density
	 */
	float rho_av = 0;
	for(int i = 0; i < p_sys.n; i++) {
		rho_av += p_sys.particles[i].density;
	}
	std::cout << "Average initial density: " << rho_av/p_sys.n << std::endl;
	outfile.writeFrame();
	p_sys.leapfrogStart();
	outfile.writeFrame();

	float percent_inc = 10;;
	float percent_done = 0;
	float fraction_done = 0.0;

	for(int frame = 0; frame < n_frames; frame++) {

		for(int i = 0; i < np_frame; i++) {
			p_sys.computeAccelerations();
			p_sys.leapfrogStep();
			//checkState(&p_sys, &outfile, frame, i);
		}
		outfile.writeFrame();
		fraction_done = (float)frame/(float)params.n_frames;
		if(fraction_done >= percent_done/100) {
			percent_done += percent_inc;
			std::cout << percent_done << "% complete" << std::endl;
		}
	}
	outfile.close();
	std::cout << "All frames processed, writing CSV file..." << std::endl;

	/* Write more readable csv file. */
	InputFile infile(params.f_name);
	std::string csv_fname = "csv_" + params.f_name;
	CSVFile csvfile(csv_fname);
	createCSVFile(& csvfile, & infile);

	std::cout << "Done." << std::endl;
	return 0;
}
