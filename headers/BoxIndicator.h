/*
 * BoxIndicator.h
 *
 *  Created on: 29 Nov 2016
 *      Author: dylan
 */

#ifndef BOXINDICATOR_H_
#define BOXINDICATOR_H_

#include "DomainIndicator.h"

class BoxIndicator:public DomainIndicator {
public:
	BoxIndicator(float xmax_in, float xmin_in,
				 float ymax_in, float ymin_in) {
		xmax = xmax_in;
		xmin = xmin_in;
		ymax = ymax_in;
		ymin=ymin_in;
	}

	int inside(const float x, const float y) {
		int in_x = (x < xmax) && (x > xmin);
		int in_y = (y < ymax) && (y > ymax);
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
