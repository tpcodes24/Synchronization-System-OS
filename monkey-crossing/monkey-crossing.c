#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "monkey-crossing-api.h"
#include "monkey-crossing.h"

void init_monkey_crossing(int rope_capacity) {
    ser = create_serializer();
    east_c = create_crowd(ser);
    west_c = create_crowd(ser);
    monkeys_q = create_queue(ser);    
}

void finish_monkey_crossing() {
    destroy_queue(ser, monkeys_q);
    destroy_crowd(ser, west_c);
    destroy_crowd(ser, east_c);
    destroy_serializer(ser);

}

bool eastbound_cond(void *arg) {

    return (crowd_count(ser, east_c) < rope_capacity) && (crowd_count(ser, west_c) == 0);
}

bool westbound_cond(void *arg) {

    return (crowd_count(ser, west_c) < rope_capacity) && (crowd_count(ser, east_c) == 0);
}

void *eastbound_thread(void *arg) {
    tdata_t *td = (tdata_t *)arg;
    enter_serializer(ser);
    enqueue(ser, monkeys_q, eastbound_cond);
    join_crowd(ser, east_c, td->body, td->body_arg);
    exit_serializer(ser);
    pthread_exit(NULL);
}


void *westbound_thread(void *arg) {
    tdata_t *td = (tdata_t *)arg;
    enter_serializer(ser);
    enqueue(ser, monkeys_q, westbound_cond);
    join_crowd(ser, west_c, td->body, td->body_arg);
    exit_serializer(ser);
    pthread_exit(NULL);
}