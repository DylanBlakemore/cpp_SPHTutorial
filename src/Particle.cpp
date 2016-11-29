/*
 * Particle.cpp
 *
 *  Created on: 29 Nov 2016
 *      Author: dylan
 */

#include "../headers/Particle.h"

float Particle::computeSquaredDistanceTo(const Particle* in_part) {
	float dx = x[0] - in_part->x[0];
	float dy = x[1] - in_part->x[1];
	return (dx*dx) + (dy*dy);
}


