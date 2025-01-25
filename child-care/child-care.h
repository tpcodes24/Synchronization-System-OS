#pragma once

#include "serializer/serializer-api.h"

// Child-care problem data structures
// //serializer_t *ser;

serializer_t *ser;            

crowd_t *caregivers_depart_c;            
crowd_t *child_departs_c;        
crowd_t *child_arrives_c;  
crowd_t *caregivers_arrive_c;  
//crowd_t *child_care_c;

queue_t *caregivers_depart_q;            
queue_t *child_departs_q;        
queue_t *child_arrives_q;
queue_t *caregivers_arrive_q;

