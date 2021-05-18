#include "p2_threads.h"
#include "utils.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <iostream>
extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
pthread_mutex_t mutex2;
using namespace std;
void *put_in_queue(void *args){
	
	pthread_mutex_lock(&mutex2);
	//Cast to thread_data type
	thread_data *temp_data = (thread_data*) args;
	
	//Look at person and put in appropriate queue
	int current_gender = ((*temp_data).temp_person).get_gender();	
	
	string this_gender;
	if(current_gender == 0){
		this_gender = "Man";
	}else{
		this_gender = "Woman";
	}

	Person current_person = (*temp_data).temp_person;
	if(current_gender == 0){
		(*(temp_data->m_queue)).push(current_person);

	}else if(current_gender == 1){
		(*(temp_data->f_queue)).push(current_person);
	}else{
		std::cerr << "My gender is " << current_gender << std::endl;
		std::cerr << "gEnDeR iS a SpEcTrUm\n";
	//((*temp_data).temp_person).complete();
	}
	pthread_mutex_unlock(&mutex2);	

	//std::cerr << "Exited put_in_queue\n";
}


void *threadfunc(void *parm)
{
	thread_data *input_data = (thread_data*) parm;	
	int status;
	//std::cerr << "[Thread] Start\n";

	//std::cerr << "[Thread] Locks\n";
	status = pthread_mutex_lock(&mutex);
	/////////////////////////////////////////////////////////////////////
	//Critical Section here
	//std::cerr << "Entered Critcal Section\n";
	long oyasumi;
		
	//Adds a person into the 0th fitting room
	//(*(input_data->room_vector))[0].add_person( (*input_data).temp_person ) ;	
	
	//Check Status of each fitting room before determining to put them in
	int i;
	int num_room;
	int current_status;
	int person_gender;	
	int room_gender_flag = *(input_data->gender_flag);	
	//std::cerr << "Room_gender_flag is = " << room_gender_flag << std::endl;
	int num_men= 0;
	int num_women = 0;
	int total_people_inside = num_men + num_women;
	int overall_total = 0;
	int this_timer = *(input_data->time_enter_fitting_room);
	//std::cerr << "Fitting room timer = " << this_timer << std::endl;
	num_room = (*input_data).total_rooms;

	Person first_in_queue;
	//Get person from queue -- use gender_flag
	if(room_gender_flag == 0) {	//Only males can enter the stalls
		//std::cerr << "Entered here 1 \n";
		if((*(input_data->m_queue)).empty() == false){
			//std::cerr << "Entered here 2 \n";
			first_in_queue = (*(input_data->m_queue)).front();
		}
	}else{	//Only females can enter the stalls
	//}else if(room_gender_flag == 1) {	//Only females can enter the stalls
		//std::cerr << "Entered here 3 \n";
		if((*(input_data->f_queue)).empty() == false){
			//std::cerr << "Entered here 4 \n";
			first_in_queue = (*(input_data->f_queue)).front();
		}	
	}
	person_gender = first_in_queue.get_gender();
	oyasumi = first_in_queue.get_sleep();
	
	string name_gender;
	if(person_gender == 0){
		name_gender = "Man";
	}else{
		name_gender = "Woman";
	}

	//Check if rooms are available for current gender
	//Woman gender flag
	if(room_gender_flag == 1){
		if(((*(input_data->f_queue)).empty() == false)){	
			for(i = 0; i < num_room; i++){
				//std::cerr << "Entered inner for loop\n";
				current_status = (*(input_data->room_vector))[i].get_status();	
				//Takes in people from specific gender queue based on flag
				if(current_status == 0){
					//Put them in the room!!!	
					//std::cerr << "Entered female add person region\n";
					
					//std::cerr << "Person gender is " << person_gender << std::endl;
					((*(input_data->room_vector))[i]).set_status(person_gender + 1);
					
					if(((*(input_data->f_queue)).empty() == false)){	
						(*(input_data->f_queue)).pop();
					}
					//std::cerr << "Oyasumi = " << oyasumi << std::endl;

					if(((*(input_data->m_queue)).empty() == false)){	
						(*(input_data->f_queue)).pop();
					}
					//Leave here
					usleep(MSEC(oyasumi));
					((*(input_data->room_vector))[i]).set_status(0);
					//(*input_data).current_state = i;
					//((*(input_data->room_vector))[(*input_data).current_state]).set_status(0);
					break;
				}else{
					//std::cerr << "Checking room status is: " << current_status << std::endl;
					if(((*(input_data->m_queue)).empty() == false)){
						*(input_data->gender_flag) = person_gender;
					}
				}
			}	
		}else{
			//std::cerr << "Female queues are empty, no need to enqueue\n";
		}
	}	//End if for women gender flag
	
	//Male gender flag
	if(room_gender_flag == 0){	
		if(((*(input_data->m_queue)).empty() == false)){	//Someone is in queue	
			for(i = 0; i < num_room; i++){
				//std::cerr << "Entered inner for loop\n";
				current_status = (*(input_data->room_vector))[i].get_status();	
				//Takes in people from specific gender queue based on flag
				if(current_status == 0){
					//Put them in the room!!!	
					//std::cerr << "Entered male add person region\n";
					
					//std::cerr << "Person gender is " << person_gender << std::endl;
					((*(input_data->room_vector))[i]).set_status(person_gender + 1);
					//Time to sleep for each person
					//oyasumi = first_in_queue.get_sleep();
					//std::cerr << "Oyasumi = " << oyasumi << std::endl;
					usleep(MSEC(oyasumi));
					if(((*(input_data->m_queue)).empty() == false)){	
						(*(input_data->m_queue)).pop();
					}	
					//Leave here	
					usleep(MSEC(oyasumi));
					//(*input_data).current_state = i;
					((*(input_data->room_vector))[i]).set_status(0);
					//((*(input_data->room_vector))[(*input_data).current_state]).set_status(0);
					
					break;
				}else{
					if(((*(input_data->m_queue)).empty() == false)){
						*(input_data->gender_flag) = person_gender;
					}
					//std::cerr << "Checking room status is: " << current_status << std::endl;
				}
			}	
		/*}else if(person_gender != room_gender_flag){
			//std::cerr << "Attempt to change gender flag\n";
			if(((*(input_data->f_queue)).empty() == false)){	
				*(input_data->gender_flag) = person_gender;
			}
		}//end	
		*/
		//std::cerr << "Leaving Critcal Section\n";
		}
	}	
	//Check how much ppl in each room after
	int overall_status;
	int j;
	for(j = 0; j < num_room; j++){
		overall_status = (*(input_data -> room_vector))[j].get_status();
		if(overall_status == 1){
			num_men++;
		}else if(overall_status == 2){
			num_women++;
		}
	}

	//This statement should always print
	std::cerr << "[" << this_timer << "][Queue] Send (" << name_gender << ") into the fitting room (Stay "<< oyasumi << " ms), Status: Total: "<< total_people_inside << "(Men:" << num_men << ", Women: " << num_women << ")\n" << std::endl;

	////////////////////////
	//std::cerr << "[Thread] Blocked\n";
	//Leaving here
    status = pthread_cond_wait(&cond, &mutex);
	/*
	usleep(MSEC(oyasumi));		
	std::cerr << "i is = " << i << std::endl;
	((*(input_data->room_vector))[i]).set_status(0);	//Leaves fitting room here
	*/
	//Person is changing for a short amount of time
		
	//std::cerr << "[Thread] Unlocks\n";
	//Next thread may enter after following statement
	status = pthread_mutex_unlock(&mutex);

	////////
	
	/*Checking
	for(i = 0; i < 10; i++){
		(*(input_data->room_vector))[i].print_status();
	}*/


	//((*input_data).temp_person).complete();;	//Prints completion time?
	//std::cerr << "[Thread] Complete\n";
	
	return NULL;	
}

