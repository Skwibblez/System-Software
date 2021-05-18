#include "types_p2.h"
#include "utils.h"



void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }
int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}

long Person::get_sleep(void){return time_to_stay_ms;}


void Person::start(void) {
	gettimeofday(&t_start, NULL);
	printf("(%lu)th person enters the fittingroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}

void Person::complete(void) {
	gettimeofday(&t_end, NULL);
	printf("(%lu)th person comes out of the fittingroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
	printf(" - (%lu) milliseconds after using the fittingroom\n", get_elasped_time(t_start, t_end));
}

Person::Person() {
	gettimeofday(&t_create, NULL);
}



// You need to use this function to print the Fittingroom's status
void Fittingroom::print_status(void) {
	printf("Print fittingroom status %d\n", status);
}

int Fittingroom::get_status(void){
	return status;	
}

// Call by reference
// This is just an example. You can implement any function you need
void Fittingroom::add_person(Person p) {
	// Do nothing;
	//status = p.get_gender() + 1;
	status = 5;
}

void Fittingroom:: set_status(int new_status){
 	status = new_status;
 }

