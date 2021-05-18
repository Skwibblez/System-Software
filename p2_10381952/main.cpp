#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

#include <time.h>
#include <string>
////////////////
using namespace std;
/////////////////

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;

int main(int argc, char** argv)
{
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);


	pthread_t       tid = 0;
	int             status = 0;
	int             work = 0;


		
	// Example code for sleep and class usage
	/*
	Person p1;
	p1.set_order(1);

	usleep(MSEC(200));
	//p1.start();

	usleep(MSEC(150));
	p1.complete();
	*/	
	///////////////////////////////////////////

	int num_males = 0;
	int num_females = 0;
	
	int total_num_threads = 0;
	int each_gender_cnt = 0;
	int number_of_stalls = 0;
	int random_num;
	srand(time(NULL));
	int rand_num;
	//Take in inputs
	if(argc == 3 && (atoi(argv[1]) > 0 && atoi(argv[2]) >0 )){
		each_gender_cnt = atoi(argv[1]);
		number_of_stalls = atoi(argv[2]);
	}else{
		std::cerr << "Expecting values greater than 0, but got less\n";
	}
		
	int f_num = 0;
	int m_num = 0;

	/* Thread stuff */
	pthread_t threads[each_gender_cnt*4];
	int i;
	
	/* Generate time_to_stay in the fitting room */
	long sleep_number; 
	
	//Stuff to pass into threadfunction
	std::vector<thread_data> input_vector;
	thread_data inputs;
	
	

	//Initialize vector of Fittingroom s
	Fittingroom stalls;
	std::vector <Fittingroom>* Rooms = new std::vector<Fittingroom>(number_of_stalls);

	//Declare m/f queues here
	std::queue<Person> male_queue;
	std::queue<Person> female_queue;
		
	std::queue<Person>* male_queue_ptr = &male_queue;
	std::queue<Person>* female_queue_ptr = &female_queue;

	inputs.m_queue = male_queue_ptr;
	inputs.f_queue = female_queue_ptr;

	inputs.total_rooms = number_of_stalls;
	int k;
	int total_thread = 0;
	int num_created_threads = 0;

	//Create all the threads
	struct timeval created_instance;	
	struct timeval starting_instance;
	struct timeval entered_fitting_room;
	gettimeofday(&starting_instance, NULL);
	int rand_num2;
	int time_wait_next_queue;
	for(i = 0; i < each_gender_cnt*2; i++){
		//People
		Person p2;
		p2.set_order(i+1);
		
		// Generate time_to_stay in the fitting room 
		sleep_number= rand()%8 +3;	//Random number between 3-10	
		p2.set_time(sleep_number);	//Time how long to wait before exit fitting room
	
		//Balances number of males and females created	
		rand_num = rand()%2;	//Generate random gender
		if(rand_num == 0){
			num_males++;
		}else{
			num_females++;
		}
		//Check if too much of a single gender
		//std::cerr << "Num_males is " << num_males << std::endl;
		//std::cerr << "Num_females is " << num_females << std::endl;
		if(num_males > each_gender_cnt){
			rand_num = 1;	//Enough males, spawn female instead
			num_males--;
			num_females++;
		}
		if(num_females > each_gender_cnt){
			rand_num = 0;	//Enough males, spawn female instead
			num_females--;
			num_males++;
		}
		//std::cerr << "Current gender is: " << rand_num << std::endl;
		p2.set_gender(rand_num);	//Assign random gender to customer	
		
		inputs.temp_person = p2;	//Store customer
		inputs.room_vector = Rooms;	//Store vector of rooms	

		int* gender_ptr = &rand_num;	//Create gender_flag
		if(i == 0){
			inputs.gender_flag = gender_ptr;
		}
		int converted_gender_flag = *(inputs.gender_flag);
		input_vector.push_back(inputs);

		//Assign customers to waiting queue
		//Adjust sleep time appropriately
		time_wait_next_queue = (rand()%5 +1);	//Rand number between 1-6
		usleep(MSEC(time_wait_next_queue));	//Delay for creation
		//time_wait_next_queue = time_wait_next_queue/1000;

		//Define queue time and person type
		string person_type;
		if(rand_num == 0){
			person_type = "Man";
		}else{
			person_type = "Woman";
		}
		//Prints out when they are sent into the queue	
		//usleep(MSEC(100));
		long queue_time;
		gettimeofday(&created_instance, NULL);
		queue_time = get_elasped_time(starting_instance, created_instance);
		std::cerr << "[" << queue_time << " ms][Input] A person (" << person_type << ") goes into the queue" <<std::endl;
		pthread_create(&threads[2*i],NULL, put_in_queue, &input_vector[i]);	

		//Need to pass in the current time they entered the fitting room
		//usleep(MSEC(300));
		long entered_fitting_room_time;
		gettimeofday(&entered_fitting_room, NULL);
		entered_fitting_room_time = get_elasped_time(starting_instance, entered_fitting_room);		
		long *fitting_room_time_ptr = &entered_fitting_room_time;
		//(input_vector[i]).time_enter_fitting_room = fitting_room_time_ptr;

		//Store this value into the struct
		//inputs.time_enter_fitting_room = entered_fitting_room_time;
		inputs.time_enter_fitting_room = fitting_room_time_ptr;
		input_vector[i] = inputs;
		//Delay a bit
		usleep(MSEC(100));

		pthread_create(&threads[2*i+1],NULL, threadfunc, &input_vector[i]);
		total_thread++;
		input_vector[i] = inputs;
		usleep(MSEC(10));
		status = pthread_cond_signal(&cond);

	}
	//Wait for all the threads to finish
	//Doesn't work properly need to get proper count
	for(i = 0; i < each_gender_cnt*2; i++){
		if(pthread_join(threads[i], NULL)) {
			fprintf(stderr, "Error joining thread\n");	
		}
	}
	///////////////////////////////////////////
	/*for(i = 0; i < number_of_stalls; i++){
		//(*Rooms)[i].print_status();
		(*(inputs.room_vector))[i].print_status();
		int person_gender = (inputs.temp_person).get_gender();
		//std::cerr << "Gender is now!!! " << person_gender << std::endl;	
	}*/
	
	return 0;

}

