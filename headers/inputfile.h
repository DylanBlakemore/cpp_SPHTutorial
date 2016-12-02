/*
 * inputfile.h
 *
 *  Created on: 30 Nov 2016
 *      Author: dylan
 */

#ifndef INPUTFILE_H_
#define INPUTFILE_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "FileStructs.h"

class InputFile {
public:
	InputFile(std::string fname) {
		readyToRead = 0;
		infile.open(fname.c_str(), std::ios::binary | std::ios::in);
	}
	system_parameters readSystemParameters();
	domain_details readDomainDetails();
	particle_details readParticle();
	void initParticleRead();
	void close(){infile.close();}

	~InputFile(){infile.close();}
private:
	int readyToRead;
	std::string fname;
	std::ifstream infile;
};




#endif /* INPUTFILE_H_ */
