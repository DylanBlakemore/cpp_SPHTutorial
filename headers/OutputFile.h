/*
 * OutputFile.h
 *
 *  Created on: 30 Nov 2016
 *      Author: dylan
 */

#ifndef OUTPUTFILE_H_
#define OUTPUTFILE_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "ParticleSystem.h"
#include "Particle.h"
#include "Params.h"
#include "FileStructs.h"

class OutputFile {
public:
	OutputFile(Paramset* params_in, ParticleSystem* sys_in) {
		params = params_in;
		sys = sys_in;
		fname = params->f_name;
		outfile.open(fname.c_str(), std::ios::binary | std::ios::out);
	}
	void writeHeader();
	void writeFrame();
	void close(){outfile.close();}

	~OutputFile(){outfile.close();}
private:
	std::string fname;
	Paramset* params;
	ParticleSystem* sys;
	std::ofstream outfile;
};



#endif /* OUTPUTFILE_H_ */
