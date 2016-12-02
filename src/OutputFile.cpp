/*
 * OutputFile.cpp
 *
 *  Created on: 30 Nov 2016
 *      Author: dylan
 */

#include "../headers/OutputFile.h"

void OutputFile::writeHeader() {
	system_parameters sp;
	sp.n_frames = params->n_frames;
	sp.np_frame = params->np_frame;
	sp.dt 		= params->dt;
	sp.h		= params->h;
	sp.rho_0 	= params->rho_0;
	sp.k		= params->k;
	sp.mu 		= params->mu;
	sp.g 		= params->g;
	sp.n_part 	= sys->n;
	sp.mass 	= sys->mass;

	domain_details dd;
	dd.scaling 	= sys->scale;
	dd.xmin 	= sys->XMIN;
	dd.xmax		= sys->XMAX;
	dd.ymin		= sys->YMIN;
	dd.ymax 	= sys->YMAX;

	outfile.write((char*)&sp, sizeof(sp));
	outfile.write((char*)&dd, sizeof(dd));
}

void OutputFile::writeFrame() {
	/* Particle entry indicates the frame end by
	 * setting id = -1; */
	particle_details pd0;
	pd0.x   = 0;
	pd0.y   = 0;
	pd0.vx  = 0;
	pd0.vy  = 0;
	pd0.ax  = 0;
	pd0.ay  = 0;
	pd0.rho = 0;
	pd0.id  = -1;

	/* Write particles one by one to file */
	int n = sys->n;
	for(int i = 0; i < n; i++) {
		Particle part = sys->particles[i];
		particle_details pd;
		pd.x   = part.x[0];
		pd.y   = part.x[1];
		pd.vx  = part.v[0];
		pd.vy  = part.v[1];
		pd.ax  = part.a[0];
		pd.ay  = part.a[1];
		pd.rho = part.density;
		pd.id  = i;
		outfile.write((char*)&pd, sizeof(pd));
	}
	outfile.write((char*)&pd0, sizeof(pd0));
}


