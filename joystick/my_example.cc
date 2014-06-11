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
  byte * b = (byte *) &f;
  //printf("%u %u %u %u\n", b[0], b[1], b[2], b[3]);
  int i;
  for(i=0; i<4; i++) {
    
    byte b1 = (b[i] >> 4) & 0x0f;
    byte b2 = (b[i] & 0x0f);
    
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
    
    //printf("%c", c1);
    //printf("%c", c2);
    //printf("\n");
    s += c1;
    s += c2;
  }
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
	
		jp[0] = js.joystickPosition(0);
		jp[1] = js.joystickPosition(1);
		
		std::cout << "Roll: " << jp[0].x << " " << "Pitch: " << jp[0].y << std::endl;
		std::cout << "Thrust: " << -1.0*jp[1].x << std::endl;
		
		//send_via_port((char *)"$", "char", 1);  
		
		string Roll;
		Roll = serialFloatPrint(jp[0].x);
		cout << "yolo: " << Roll << endl;
		
		send_via_port((char *)"$", "char", 1);

		//send_via_port((char *)"Roll: ", "char", 6);  
		send_via_port(&Roll, "string", 0);  
		send_via_port((char *)",", "char", 1);  
		
		string Pitch;
		Pitch = serialFloatPrint(jp[0].y);
		//strcpy(temp, serialFloatPrint(jp[0].y));
		cout << "yolo: " << Pitch << endl;
		
		//send_via_port((char *)"Pitch: ", "char", 7);  
		//send_via_port(&(jp[0].y), "float", 0);  
		send_via_port(&Pitch, "string", 0);
		send_via_port((char *)",", "char", 1);
		
		string Thrust;
		Thrust = serialFloatPrint(-1.0*jp[1].x);
		cout << "yolo: " << Thrust << endl;
		
		//send_via_port((char *)"Thrust: ", "char", 8);  
		//send_via_port(&(jp[1].x), "float", 0);  

		send_via_port(&Thrust, "string", 0);
		send_via_port((char *)":", "char", 1);
		//send_via_port((char *)"\n\n", "char", 2); 
		
		usleep(30000);
	}
	return 0;
}
