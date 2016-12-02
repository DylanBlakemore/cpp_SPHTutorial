/*
 * inputfile.cpp
 *
 *  Created on: 30 Nov 2016
 *      Author: dylan
 */

#include <exception>
#include "../headers/inputfile.h"

system_parameters InputFile::readSystemParameters() {
	system_parameters sp;
	try {
		infile.seekg(0,std::ios::beg);
		infile.read((char*)&sp, sizeof(sp));
	}
	catch(std::exception& exc) {
		std::cout << "Standard exception: " << exc.what() << std::endl;
		sp.dt = 0;
		sp.g = 0;
		sp.h = 0;
		sp.k = 0;
		sp.mass = 0;
		sp.mu = 0;
		sp.n_frames = 0;
		sp.n_part = 0;
		sp.np_frame = 0;
		sp.rho_0 = 0;
	}
	return sp;
}

domain_details InputFile::readDomainDetails() {
	domain_details dd;
	try {
		infile.seekg(sizeof(system_parameters),std::ios::beg);
		infile.read((char*)&dd, sizeof(dd));
	}
	catch(std::exception& exc) {
		std::cout << "Standard exception: " << exc.what() << std::endl;
		dd.scaling = 0;
		dd.xmax = 0;
		dd.xmin = 0;
		dd.ymax = 0;
		dd.ymin = 0;
	}
	return dd;
}

void InputFile::initParticleRead() {
	try {
		infile.seekg(sizeof(system_parameters) + sizeof(domain_details), std::ios::beg);
	}
	catch(std::exception& exc) {
		std::cout << "Standard exception: " << exc.what() << std::endl;
	}
	readyToRead = 1;
}

particle_details InputFile::readParticle() {
	particle_details pd;
	if(!readyToRead)
		throw std::runtime_error("Pointer not ready to read particles");
	try {
		infile.read((char*)&pd, sizeof(pd));
	}
	catch(std::exception& exc) {
		std::cout << "Standard exception: " << exc.what() << std::endl;
	}
	return pd;
}
