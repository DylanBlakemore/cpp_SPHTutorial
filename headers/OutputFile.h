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
	void close(){
	/* Id = -2 indicates end of file */
		particle_details pdf;
		pdf.x   = 0;
		pdf.y   = 0;
		pdf.vx  = 0;
		pdf.vy  = 0;
		pdf.ax  = 0;
		pdf.ay  = 0;
		pdf.rho = 0;
		pdf.id  = -2;
		outfile.write((char*)&pdf, sizeof(pdf));
		outfile.close();
	}

	~OutputFile(){close();}
private:
	std::string fname;
	Paramset* params;
	ParticleSystem* sys;
	std::ofstream outfile;
};



#endif /* OUTPUTFILE_H_ */
