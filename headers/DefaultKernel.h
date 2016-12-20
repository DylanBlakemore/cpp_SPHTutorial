/*
 * DefaultKernel.h
 *
 *  Created on: 04 Dec 2016
 *      Author: dylan
 */

#ifndef DEFAULTKERNEL_H_
#define DEFAULTKERNEL_H_

#include "Kernel.h"
#include <math.h>

class DefaultKernel:public Kernel {
public:
	void computeWeight(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;
		if(sqrt(r2) <= h)
			weight[0] = pow((pow(h,2) - r2),3) * 315/(64 * PI * pow(h,9));
		else
			weight[0] = 0;
	}
	void computeGradient(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;

		if(sqrt(r2) <= 0) {
			weight[0] = 0;
			weight[1] = 0;
			return;
		}

		float w_scalar = pow(pow(h,2) - r2,2) * (-945/(32 * PI * pow(h,9)));
		if(sqrt(r2) <= h) {
			weight[0] = w_scalar * dx;
			weight[1] = w_scalar * dy;
		}
		else {
			weight[0] = 0;
			weight[1] = 0;
		}
	}
	void computeLaplacian(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;

		if(sqrt(r2) <= 0) {
			weight[0] = 0;
			return;
		}

		float t1 = -945/(32 * PI * pow(h,9));
		float t2 = pow(h,2) - r2;
		float t3 = 3*pow(h,2) - 7*r2;
		if(sqrt(r2) <= h)
			weight[0] = t1 * t2 * t3;
		else
			weight[0] = 0;
	}
};



#endif /* DEFAULTKERNEL_H_ */
