#include "libs/RCSwitch.h"
#include <wiringPi.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <stdarg.h>  // For va_start, etc.


const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

const void log(const char* fmt, ...) {
	va_list vargs;
	
	printf("%s ", currentDateTime().c_str());
	va_start(vargs, fmt);
	vprintf(fmt, vargs);
	va_end(vargs);
	printf("\n");
}

int main(int argc, char *argv[]) {
	RCSwitch mySwitch;
    int RFRX = 2; // rf433 receiver pin, rpi pin13
	int motionDetectedCode = 666;

    if(wiringPiSetup() == -1)
       return 0;

    mySwitch = RCSwitch();
    mySwitch.enableReceive(RFRX);
	
	log("Detecting motion on pin %i\n", RFRX);
    
    while(1) {
		if (mySwitch.available()) {
			int value = mySwitch.getReceivedValue();
		
			if(value == motionDetectedCode)
			{
				log("Motion detected!");
			} else 
			{
				log("Noise %i", value );
			}
		
			mySwitch.resetAvailable();
       }
   }

  exit(0);
}

std::string string_format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}



