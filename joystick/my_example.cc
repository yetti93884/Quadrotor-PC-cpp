#include <SDL/SDL.h>
#include <sstream>
#include <fstream>

#include "joystick.h"
#include "glhelper.h"
#include <unistd.h>
#include "serial_talk.h"

#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>

using namespace std;

typedef unsigned char byte;
string temp ("");

string serialFloatPrint(float f) {
  string s ("");
  long num = f*100;
  int val;

  if(num>=0)
  	s += '+';
  else {
  	s += '-';
  	num = -1*num;
  }

  val = num/100;
  num = num%100;
  s += val + '0';

  val = num/10;
  num = num%10;

  s += val + '0';
  s += num + '0';

  //cout << s << endl;
  return s;
}

int main(int argc, char *argv[]) {
	cJoystick js; joystick_position jp[2];
	char default_portname[20] = "/dev/ttyUSB0";
	char default_baud[20] = "57600";
	char portname[20];
	char baud[20];
	char buf[20];

	bool FLAG_STREAM_DATA = false;
	bool FLAG_STOP = false;
	bool FLAG_CONTROL_RESTART = false;

	float zhi_ref = 1;

	string stop = "!";
	string control_restart = "?";
	string reset_setpoint = "%reset 1";

	cout << "ENTER PORT WHERE XBEE SITS (enter 'd' for default: /dev/ttyUSB0): ";
	cin >> buf;
	if (strcmp("d", buf) == 0) {
		strcpy(portname, default_portname);
	}
	else {
		strcpy(portname, buf);
	}

	cout << "ENTER BAUD RATE FOR XBEE COMMUNICATION (enter 'd' for default: 57600): ";
	cin >> buf;
	if (strcmp("d", buf) == 0) {
		strcpy(baud, default_baud);
	}
	else {
		strcpy(baud, buf);
	}

	port_initialize(portname, baud);

	while (true) {

		if(js.buttonPressed(1) == true || js.buttonPressed(2) == true || js.buttonPressed(3) == true || js.buttonPressed(4) == true) {
			FLAG_STOP = true;
			FLAG_STREAM_DATA = false;
			FLAG_CONTROL_RESTART = false;
			
			send_via_port(&stop, "string", 0);
			cout<<"JOYSTICK : EMERGENCY STOP FOR MOTORS\n";
			usleep(40000);
		}

		if(js.buttonPressed(9)) {
			FLAG_STOP = false;
			FLAG_STREAM_DATA = false;
			FLAG_CONTROL_RESTART = true;
			send_via_port(&control_restart, "string", 0);
			cout<<"JOYSTICK : Control restart\n";
			sleep(1);
		}
		
		if(js.buttonPressed(5)) {
			FLAG_STOP = false;
			FLAG_STREAM_DATA = true;
			FLAG_CONTROL_RESTART = false;
		}
		if(js.buttonPressed(6)) {
			FLAG_STREAM_DATA = false;
		}

		if(js.buttonPressed(7)) {
			zhi_ref += 0.02;
			if(zhi_ref > 3.1)
				zhi_ref = 3.1;
		}

		if(js.buttonPressed(8)) {
			zhi_ref -= 0.02;
			if(zhi_ref < -3.1)
				zhi_ref = -3.1;
		}

		if(js.buttonPressed(10)) {
			FLAG_STOP = false;
			FLAG_STREAM_DATA = false;
			FLAG_CONTROL_RESTART = true;
			send_via_port(&reset_setpoint, "string", 0);
			cout<<"JOYSTICK : reset values\n";
			sleep(1);
		}

		if(FLAG_STREAM_DATA == true && FLAG_STOP == false && FLAG_CONTROL_RESTART == false) {
		
			jp[0] = js.joystickPosition(0);
			jp[1] = js.joystickPosition(1);
			
			
			
			//send_via_port((char *)"$", "char", 1);  

			string Yaw;
			Yaw = serialFloatPrint(zhi_ref) ;

			string Roll;
			Roll = serialFloatPrint(jp[0].x) ;
			// cout << "yolo: " << Roll << endl;
			
			// #send_via_port((char *)"$", "char", 1);

			//send_via_port((char *)"Roll: ", "char", 6);  
		// #	send_via_port(&Roll, "string", 0);    
			
			string Pitch;
			Pitch = serialFloatPrint(jp[0].y);
			//strcpy(temp, serialFloatPrint(jp[0].y));
			// cout << "yolo: " << Pitch << endl;
			
			//send_via_port((char *)"Pitch: ", "char", 7);  
			//send_via_port(&(jp[0].y), "float", 0);  
// #			send_via_port(&Pitch, "string", 0);
// #			send_via_port((char *)",", "char", 1);
			
			string Thrust;
			float m = (-jp[1].x+1)/2*1.1;
			Thrust = serialFloatPrint(m);
 	
			string message = "$" + Yaw + "," + Pitch + "," + Roll + "," + Thrust + ":";

			send_via_port(&message,"string",0);
			std::cout<<message<<"  | ";
			std::cout<<zhi_ref<<" "<<jp[0].y<<" "<<jp[0].x<<" "<<m<<std::endl;

			// #std::cout << "Roll: " << jp[0].x << " " << "Pitch: " << jp[0].y << " ";
			// #std::cout << "Thrust: " << m<< std::endl;
			// cout << "yolo: " << Thrust << endl;
			
			//send_via_port((char *)"Thrust: ", "char", 8);  
			//send_via_port(&(jp[1].x), "float", 0);  

// #			send_via_port(&Thrust, "string", 0);
// #			send_via_port((char *)":", "char", 1);

			//send_via_port((char *)"\n\n", "char", 2); 
			
			usleep(40000);
		}
	}
	return 0;
}
