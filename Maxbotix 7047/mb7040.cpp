/*
 * mb7047.cpp
 */
/** TO COMPILE:
	g++ sensorHandler.cpp mb7047.cpp -o sonarTest
*/
// Standard C++ header files
#include <iostream>
#include "mb7040.h"

// Standard C header files
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stropts.h>
#include <errno.h>
#include <time.h>
using namespace std;

//I2C Device Definitions

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* NAME:
*
* DESCRIPTION:
*
* INPUT:
*
* OUTPUT:
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
mb7047::mb7047(int bus, int address)
{

	//Device bus and address
	I2CBus = bus;
	I2CAddress = address;


	numberBytes = MB_I2C_BUFFER;

	this->mb7047_Open();

//	readFullSensorState();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* NAME:
*
* DESCRIPTION:
*
* INPUT:
*
* OUTPUT:
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
mb7047::~mb7047( void )
{
	close(file);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* NAME:
*
* DESCRIPTION:
*
* INPUT:
*
* OUTPUT:
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mb7047::mb7047_Open( void )
{

	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);

	//Open a connection to the i2c bus.
	//Note this requires read/write access to the /dev/i2c-x file for a given user.
	if ((file = open(namebuf, O_RDWR)) < 0){
			cout << "Failed to open MB7047 Sonar on " << namebuf << " I2C Bus." << endl;
			//cout << "Ensure the user has the correct permissions for the I2C Bus." << endl;
		//printf("AIR: Open Error.");
	}
//	cout<<"File : "<<file<<endl;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* NAME:
*
* DESCRIPTION:
*
* INPUT:
*
* OUTPUT:
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float mb7047::readFullSensorState()
{

///////////////////////////////////////////////////////////////////
// This subsection of code establishes a connection with the device,
// reads the data into a buffer using the linux/i2c-dev functions.
///////////////////////////////////////////////////////////////////

	//Dim file pointer and assign the i2c bus number to the string file


	//Access the specific slave address for the desired device
	if (ioctl(file, I2C_SLAVE, I2CAddress) < 0){
			//cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
			return(2);
	}

	buf[0] = 0x51; // Range Command - 51

	if(write(file, buf, 1) !=1){
		return(3);
	}
 	usleep(80000); // 80 millisecond wait for proper acoustic dissipation

 	buf[0] = 0xE0; // write command - 224

 	if(write(file, buf, 1) !=1){
 		return(3);
 	}
 	usleep(50000); // 50 millisecond wait for range reading

	//Read in the data stream to the buffer. Upon success close the file; otherwise, stream error.
	bytesRead = read(file, dataBuffer, numberBytes);
	if (bytesRead == -1){
		//cout << "Failure to read Byte Stream in readFullSensorState()" << endl;
		return(4);
	}

	value = (float)((dataBuffer[0] << 8) + dataBuffer[1]);
//	cout<<"Value : "<<value<<endl;
	return value;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* NAME:
*
* DESCRIPTION:
*
* INPUT:
*
* OUTPUT:
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float mb7047 :: getDistance()
{

	distance = readFullSensorState();
	return distance;

}
