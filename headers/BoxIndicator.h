/*
 * BoxIndicator.h
 *
 *  Created on: 29 Nov 2016
 *      Author: dylan
 */

#ifndef BOXINDICATOR_H_
#define BOXINDICATOR_H_

#include <iostream>
#include "DomainIndicator.h"

class BoxIndicator:public DomainIndicator {
public:
	BoxIndicator(float xmin_in, float xmax_in,
				 float ymin_in, float ymax_in) {
		xmax = xmax_in;
		xmin = xmin_in;
		ymax = ymax_in;
		ymin = ymin_in;
	}

	int contains(const float x, const float y) {
		int in_x = (x < xmax) && (x > xmin);
		int in_y = (y < ymax) && (y > ymin);
		return in_x && in_y;
	}
	~BoxIndicator(){}
private:
	float xmax;
	float xmin;
	float ymax;
	float ymin;
};



#endif /* BOXINDICATOR_H_ */
