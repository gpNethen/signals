#include <stdio.h>      // printf
#include <time.h>       // time_t, struct tm, difftime, time, mktime
#include <iostream>		// using cout instead of printf
#include <iomanip>		// setting precision of output etc...
#include <random>		// generation of pseuso-random numbers
#include <conio.h>		// kbhit functionality for sigusr1 simulation
#include "signals.h"	// header file for #define statements

using namespace std;

int main ()
{
	random_device rd; // Obtains random number from the hardware
	mt19937 eng(rd()); // seeds pseudo-random generator
	uniform_int_distribution<> distr(0,MAX_TIME); // Defines the range
	
	int maxStartingAlarms=10; // A basic max limit to the number of initial alarms
	int numOfSigAlarms=0;
	int sigAlarms[MAX_SIGUSR];
	int numOfAlarms; // Variable to hold the number of alarms the user wishes to set
	cout << "Press 'f' key for SIGUSR1 after timer begins counting.\n";
	cout << "Please enter a number in the range [0, 10] of alarms you wish to set: ";
	cin >> numOfAlarms; // Sets number of alarms based on user input
	while(!(numOfAlarms>=0 && numOfAlarms<=10)) // While range of allowable number of alarms is violated
	{
		cout << "Number not in range, please try again: ";   // Keep requesting valid input
		cin >> numOfAlarms;
	}
	cout << "You have chosen to set " << numOfAlarms <<  " alarms.\n";
	cout << "Setting alarms at randomized times.\n";
	int startingTime=time(NULL); // Sets the start time
	int setAlarms[numOfAlarms]; // array to hold the random alarm times
	for(int i=0; i<numOfAlarms; i++) // for each of the number of alarms
	{
		setAlarms[i]=startingTime+distr(eng); // sets the alarm to a random time between 0 and 60 seconds from startingTime
		cout << "Alarm number " << i+1 << " set at time " << setAlarms[i] << endl;
	}
	
	int timePassed=0; // variable to measure how many seconds pass during program run
	int timeCheck=0; // should update every second to "keep track" of the time
	int sigusrCount=numOfAlarms+1; // keeps track of the alarm count after initial randomized alarms
	while(timePassed <= MAX_TIME) // while running within alotted maximum time
	{
		if(timeCheck!=time(NULL))  // if the checked time is not the same as current time
		{
			timeCheck=time(NULL); // updates the checked time
			cout << "Current time: " << timeCheck << endl; // displays the current time to the terminal
			timePassed++; // increment the number of seconds passed
			for(int i=0; i<numOfAlarms; i++) // scan through the array of alarms
			{
				if(setAlarms[i]==timeCheck)  // if there is an alarm scheduled at this time
					cout << "\tAlarm " << i+1 << " caught at time " << timeCheck << "!\n"; // say so
			}
			if(kbhit())
			{
				if(getch() == 'f' && numOfSigAlarms < MAX_SIGUSR)
				{
					uniform_int_distribution<> distr(timePassed, (MAX_TIME-timePassed));
					sigAlarms[numOfSigAlarms]=timeCheck+distr(eng);
					cout << "SIGUSR1 caught.\n" << "Alarm " << numOfSigAlarms+1+numOfAlarms << " assigned to time " << sigAlarms[numOfSigAlarms]  << "\n";
					numOfSigAlarms++;
				}
			}
			for(int j=0; j<numOfSigAlarms; j++)
			{
				if(sigAlarms[j]==timeCheck)
					cout << "\tAlarm " << j+1+numOfAlarms << " caught at time " << timeCheck << "!\n";
			}
		}
	}
	cout << "Current time is now " << time(NULL) << endl;
	cout << "Maximum time of " << MAX_TIME << " seconds: achieved.\n" << "System exiting...\n";

    return 0;
}
