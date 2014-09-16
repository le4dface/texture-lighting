/*
 * FileIO.h
 *
 *  Created on: 16/09/2014
 *      Author: reuben
 */

#ifndef FILEIO_H_
#define FILEIO_H_


#ifndef FILEIOH
#define FILEIOH

#include <stdio.h>
#include <GL/glut.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
// Using bitmasking to denote different degrees of freedom for joint motion
typedef int DOF;

#define DOF_NONE 0
#define DOF_RX 1
#define DOF_RY 2
#define DOF_RZ 4
#define DOF_ROOT 8

void trim(char**);

using namespace std;

class FileIO {

private:



public:

	FileIO();
	virtual ~FileIO();

};

#endif /* FILEIOH */













#endif /* FILEIO_H_ */
