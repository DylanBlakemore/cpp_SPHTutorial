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
};



#endif /* KERNEL_H_ */
