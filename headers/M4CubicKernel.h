/*
 * M4CubicKernel.h
 *
 *  Created on: 05 Dec 2016
 *      Author: dylan
 */

#ifndef M4CUBICKERNEL_H_
#define M4CUBICKERNEL_H_

class M4CubicKernel:Kernel {
public:
	void computeWeight(Particle* p1, Particle* p2, float h, float* weight){}
	void computeGradient(Particle* p1, Particle* p2, float h, float* weight){}
	void computeLaplacian(Particle* p1, Particle* p2, float h, float* weight){}
};



#endif /* M4CUBICKERNEL_H_ */
