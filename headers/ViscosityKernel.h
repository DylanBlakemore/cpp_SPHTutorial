/*
 * ViscosityKernel.h
 *
 *  Created on: 14 Dec 2016
 *      Author: dylan
 */

#ifndef VISCOSITYKERNEL_H_
#define VISCOSITYKERNEL_H_

class ViscosityKernel:public Kernel {
public:
	void computeWeight(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;
		float r = sqrt(r2);
		/* Degenerate case */
		if(r <= 0) {
			weight[0] = 0;
			return;
		}

		float c1 = 15 / (2 * PI * pow(h,3));
		float c2 = -1 * pow(r,3)/(2*pow(h,3));
		float c3 = r2/pow(h,2);
		float c4 = h/(2 * r);

		if(r <= h)
			weight[0] = c1 * (c2 + c3 + c4 - 1);
		else
			weight[0] = 0;

	}
	void computeGradient(Particle* p1, Particle* p2, float h, float* weight) {
		weight[0] = 0;
		weight[1] = 0;
	}
	void computeLaplacian(Particle* p1, Particle* p2, float h, float* weight) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r2 = dx*dx + dy*dy;
		float r = sqrt(r2);
		if(r <= h)
			weight[0] = (h - r) * 45 / (PI * pow(h,6));
		else
			weight[0] = 0;
	}
};



#endif /* VISCOSITYKERNEL_H_ */
