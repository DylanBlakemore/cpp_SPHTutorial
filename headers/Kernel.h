/*
 * Kernel.h
 *
 *  Created on: 04 Dec 2016
 *      Author: dylan
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "Particle.h"

class Kernel {
public:
	virtual void computeWeight(Particle* p1, Particle* p2, float h, float* weight);
	/* gradient = float[2] */
	void computeGradient(Particle* p1, Particle* p2, float h, float* gradient);
	void computeLaplacian(Particle* p1, Particle* p2, float h, float* laplacian);
	virtual ~Kernel(){};
protected:
	const static float  PI = 3.14159265358979;
	float getQ(Particle* p1, Particle* p2, float h) {
		float q;
		float r = getR(p1, p2);
		q = r/h;
		return q;
	}
	float getR(Particle* p1, Particle* p2) {
		float dx = p1->x[0] - p2->x[0];
		float dy = p1->x[1] - p2->x[1];
		float r = sqrt(dx*dx + dy*dy);
		return r;
	}
};



#endif /* KERNEL_H_ */
