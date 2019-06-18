//#pragma once
#include <stdlib.h>
#include <cstddef>
//#include <iostream>

#include "library2.h"
#include "Schedule.h"

#define m 10

void* Init(int n){
    Schedule* sys = nullptr;
    try {
        sys = new Schedule(n, m);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return NULL;
    }
    return (void*)sys;
}

StatusType addRoom(void *DS, int roomID){
    if(DS == NULL || roomID <= 0)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->addRoom(roomID);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

StatusType deleteRoom(void *DS, int roomID){
    if(DS == NULL || roomID <= 0)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->deleteRoom(roomID);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

StatusType addLecture(void *DS, int courseID, int groupID, int roomID, int hour, int numStudents){
    if(DS == NULL || groupID < 0 || numStudents < 0 || roomID <= 0 || hour > m || hour < 1)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->addLecture(courseID, groupID, roomID, hour, numStudents);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

StatusType deleteLecture(void *DS, int hour, int roomID){
    if(DS == NULL || roomID <= 0 || hour < 1 || hour > m)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->deleteLecture(hour, roomID);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

StatusType mergeCourses(void *DS, int courseID1, int courseID2){
    if(DS == NULL)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->mergeCourses(courseID1, courseID2);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

StatusType competition(void *DS, int courseID1, int courseID2, int numGroups, int * winner){
    if(DS == NULL || numGroups <= 0 || winner == NULL)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->competition(courseID1, courseID2, numGroups, winner);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

StatusType getAverageStudentsInCourse(void *DS, int hour, int roomID, float * average){
    if(DS == NULL || hour < 1 || hour > m || roomID <= 0 || average == NULL)
        return INVALID_INPUT;
    try{
        ((Schedule*)DS)->getAverageStudentsInCourse(hour, roomID, average);
    }
    catch (scheduleExceptions a) {
        if (a.getType() == SCHEDULE_mem)
            return ALLOCATION_ERROR;
        if (a.getType() == SCHEDULE_invalidInput)
            return INVALID_INPUT;
        if (a.getType() == SCHEDULE_failure)
            return FAILURE;
    }
    return SUCCESS;
}

void Quit(void** DS){
    if (DS == NULL)
        return;
    delete ((Schedule*)(*DS));
    *DS = NULL;
}
