/*
 * PressureKernel.h
 *
 *  Created on: 14 Dec 2016
 *      Author: dylan
 */

#ifndef PRESSUREKERNEL_H_
#define PRESSUREKERNEL_H_

#include "Kernel.h"

class PressureKernel:public Kernel {
public:
	void computeWeight(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;
		if(sqrt(r2) <= 0) {
			weight[0] = 0;
			return;
		}

		if(sqrt(r2) <= h) {
			float c1 = 15/(PI * pow(h,6));
			float c2 = pow(h - sqrt(r2),3);
			weight[0] = c1 * c2;
		}
		else {
			weight[0] = 0;
		}
	}
	void computeGradient(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;
		/* Degenerate case to prevent division by zero */
		if(r2 <= 0) {
			weight[0] = 0;
			weight[1] = 0;
			return;
		}

		float r = sqrt(r2);
		float c1 = -45/(PI * pow(h,6));
		float c2 = pow(h - r,2);

		if(r <= h) {
			weight[0] = c1 * c2 * dx / r;
			weight[1] = c1 * c2 * dy / r;
		}
		else {
			weight[0] = 0;
			weight[1] = 0;
		}
	}
	void computeLaplacian(Particle* p1, Particle* p2, float h, float* weight) {
		weight[0] = 0;
	}
};



#endif /* PRESSUREKERNEL_H_ */
