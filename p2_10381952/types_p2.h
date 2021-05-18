#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <queue>
#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2


class Person
{
	
	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	long get_sleep(void);
	void set_time(long data);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
};


// Class for the fittingroom
// You may need to add more class member variables and functions
class Fittingroom {
	int status;

	//std::vector <int> status_vector;
	// You need to define the data structure to
    // save the information of people using the fittingroom
	// You can probebly use Standard Template Library (STL) vector


public:
	Fittingroom(){
		status = EMPTY;


	}

	// You need to use this function to print the Fittingroom's status
	void print_status(void);
	//Added
	void set_status(int new_status);

	int get_status(void);
	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person p);







};

struct thread_data{
	Person temp_person;
	bool choice;
	std::vector<Fittingroom>* room_vector;
	std::queue<Person>* f_queue;
	std::queue<Person>* m_queue;
	int* gender_flag;
	int total_rooms;
	int current_state;
	long timer_status;
	long *time_enter_fitting_room;
};







#endif
