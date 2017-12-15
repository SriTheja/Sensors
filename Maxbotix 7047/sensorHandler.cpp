#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <iterator>

#include "./sensorHandler.h"
#include "./mb7040.h"

using namespace std;

#define DEVICE_ADDR 0x70
#define DEVICE_BUS	1
int main(){
	ofstream file;
	float distance;
	float distance_prev;
	mb7047 *Sonar = new mb7047(DEVICE_BUS, DEVICE_ADDR);
	distance_prev = 0;
//	file.open("data.txt");
//	file<<"Distance_to_Object(cm)"<<endl;
	while(1)
	{
		distance = Sonar->getDistance();

//		if(distance <= 200)
//		{
//			cout<<"The distance to the Object is "<<distance<<"cm  STOP Immediately"<<endl;
//		}
//		else if(distance <= 350 && distance > 200)
//		{
//			cout<<"The distance to the Object is "<<distance<<"cm  Proceed with Caution"<<endl;
//		}
//		else if(distance <= 700 && distance > 350)
//		{
//			cout<<"The distance to the Object is "<<distance<<"cm"<<endl;
//		}
//		else
//		{
//			cout<<"No Object within 7 mts"<<endl;
//		}

		cout<<"Distance to Object(cm) = "<<distance<<endl;
//		file<<distance<<endl;
//		file.flush();
	}
//	file.close();
	return 0;

}

