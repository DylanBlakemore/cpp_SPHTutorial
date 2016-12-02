/*
 * FileStructs.h
 *
 *  Created on: 02 Dec 2016
 *      Author: dylan
 */

#ifndef FILESTRUCTS_H_
#define FILESTRUCTS_H_

struct system_parameters {
	int n_frames;
	int np_frame;
	float dt;
	float h;
	float rho_0;
	float k;
	float mu;
	float g;
	int n_part;
	float mass;
};

struct domain_details {
	float scaling;
	float xmin;
	float xmax;
	float ymin;
	float ymax;
};

struct particle_details {
	int id;	// id == -1 indicates end of frame
	float x;
	float y;
	float vx;
	float vy;
	float ax;
	float ay;
	float rho;
};

#endif /* FILESTRUCTS_H_ */
