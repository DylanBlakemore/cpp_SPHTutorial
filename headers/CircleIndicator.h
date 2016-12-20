/*
 * CircleIndicator.h
 *
 *  Created on: 29 Nov 2016
 *      Author: dylan
 */

#ifndef CIRCLEINDICATOR_H_
#define CIRCLEINDICATOR_H_

#include "DomainIndicator.h"

class CircleIndicator:public DomainIndicator {
public:
	CircleIndicator(float xcentre_in, float ycentre_in, float radius_in) {
		xcentre = xcentre_in;
		ycentre = ycentre_in;
		radius = radius_in;
	}

	int contains(const float x, const float y) {
		float dx = x - xcentre;
		float dy = y - ycentre;
		float r2 = dx*dx + dy*dy;
		return (r2 <= radius*radius);
	}
	~CircleIndicator(){}
private:
	float xcentre;
	float ycentre;
	float radius;
};



#endif /* CIRCLEINDICATOR_H_ */
