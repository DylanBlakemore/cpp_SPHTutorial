/*
 * DomainIndicator.h
 *
 *  Created on: 29 Nov 2016
 *      Author: dylan
 */

#ifndef DOMAININDICATOR_H_
#define DOMAININDICATOR_H_

class DomainIndicator {
public:
	DomainIndicator();
	virtual int contains(const float x, const float y){return 0;}
	virtual ~DomainIndicator(){}
};



#endif /* DOMAININDICATOR_H_ */
