#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "child-care-api.h"
#include "child-care.h"

int caregivers_count = 0;
int children_count = 0;

void init_child_care() {
    ser = create_serializer();

    caregivers_arrive_c = create_crowd(ser);
    child_arrives_c = create_crowd(ser);
    child_departs_c = create_crowd(ser);
    caregivers_depart_c = create_crowd(ser);

    caregivers_arrive_q = create_queue(ser);
    child_arrives_q = create_queue(ser);
    child_departs_q = create_queue(ser);
    caregivers_depart_q = create_queue(ser);
}

void finish_child_care() {
    destroy_queue(ser, caregivers_depart_q);
    destroy_queue(ser, child_departs_q);
    destroy_queue(ser, child_arrives_q);
    destroy_queue(ser, caregivers_arrive_q);


    destroy_crowd(ser, caregivers_depart_c);
    destroy_crowd(ser, child_departs_c);
    destroy_crowd(ser, child_arrives_c);
    destroy_crowd(ser, caregivers_arrive_c);

    destroy_serializer(ser);
}



bool caregiver_arrival_cond(void *arg) {
    return true;
}

bool caregiver_departure_cond(void *arg) {
    return (caregivers_count > 1) || (caregivers_count >= 1 && children_count == 0);
    //(children_count==0) || (caregivers_count > 1) || (caregivers_count >= 1 && children_count == 0);
     //return (caregivers_count > 1 && children_count > 0) || (caregivers_count == 1 && children_count == 0);
}

bool child_arrival_cond(void *arg) {
    return caregivers_count > 0;
}

bool child_departure_cond(void *arg) {
    return children_count > 0;
}



void *caregiver_arrive_thread(void *arg) {
    tdata_t *td = (tdata_t *)arg;
    enter_serializer(ser);
    caregivers_count++;
    enqueue(ser, caregivers_arrive_q, caregiver_arrival_cond);
    join_crowd(ser, caregivers_arrive_c, td->body, td->body_arg);
    exit_serializer(ser);
    pthread_exit(NULL);
}

void *caregiver_depart_thread(void *arg) {
    tdata_t *td = (tdata_t *)arg;
    enter_serializer(ser);
    enqueue(ser, caregivers_depart_q, caregiver_departure_cond);
    caregivers_count--;
    join_crowd(ser, caregivers_depart_c, td->body, td->body_arg);
    exit_serializer(ser);
    pthread_exit(NULL);
}

void *child_arrive_thread(void *arg) {
    tdata_t *td = (tdata_t *)arg;
    enter_serializer(ser);
    enqueue(ser, child_arrives_q, child_arrival_cond);
    children_count++; 
    join_crowd(ser, child_arrives_c, td->body, td->body_arg);
    exit_serializer(ser);
    pthread_exit(NULL);
}

void *child_depart_thread(void *arg) {
    tdata_t *td = (tdata_t *)arg;
    enter_serializer(ser);
    enqueue(ser, child_departs_q, child_departure_cond);
    join_crowd(ser, child_departs_c, td->body, td->body_arg);
    children_count--;
    exit_serializer(ser);
    pthread_exit(NULL);
}
