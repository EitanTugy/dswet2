#pragma once

#ifndef WET2_SCHEDULE_H
#define WET2_SCHEDULE_H

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "Union.h"
#include "ListAndHash.h"
#include "Course.h"
#include "methodTreeIntoArray.h"

//#include <iostream>

enum TYPE_SCHEDULE_EXCEPTION {
    SCHEDULE_mem = 1,
    SCHEDULE_invalidInput,
    SCHEDULE_failure
};
class scheduleExceptions {
    TYPE_SCHEDULE_EXCEPTION type;
public:
    scheduleExceptions(TYPE_SCHEDULE_EXCEPTION type)
            :
            type(type)
    {}
    TYPE_SCHEDULE_EXCEPTION getType() {
        return type;
    }
};


class Schedule {
private:
    Course* courses;
    Union* coursesUnion;
    Hash<int, Room, HashFunc>* rooms;
    int numOfHours;

public:
    Schedule(int numOfCourses, int numOfHours)
            :
            courses(nullptr),
            coursesUnion(nullptr),
            rooms(nullptr),
            numOfHours(numOfHours)
    {
        try {
            courses = new Course[numOfCourses];
        }
        catch (std::bad_alloc a) {
            throw scheduleExceptions(SCHEDULE_mem);
        }
        try{
            coursesUnion = new Union(numOfCourses);
        }
        catch (unionExceptions a) {
            delete [] courses;
            if(a.getType() == UNION_mem)
                throw scheduleExceptions(SCHEDULE_mem);
        }
        //HashFunc hashFunction = HashFunc(2);
        try{
            rooms = new Hash<int, Room, HashFunc>(2, HashFunc(2));
        }
        catch (hashExceptions a) {
            delete coursesUnion;
            delete [] courses;
            if(a.getType() == HASH_mem)
                throw scheduleExceptions(SCHEDULE_mem);
        }

    }


    ~Schedule() {
        delete rooms;
        delete coursesUnion;
        delete [] courses;
    }

    void addRoom(int roomID){
        Room* room = new Room(roomID/*, numOfHours*/);
        try {
            rooms->insert(&roomID, room);
        }
        catch(hashExceptions a){
            if(a.getType() == HASH_mem)
                throw scheduleExceptions(SCHEDULE_mem);
            if(a.getType() == HASH_failure)
                throw scheduleExceptions(SCHEDULE_failure);
        }
    }

    void deleteRoom(int roomID){
        Room* room;
        try{
            room = rooms->find(&roomID);
        }
        catch(hashExceptions a){
            if(a.getType() == HASH_mem)
                throw scheduleExceptions(SCHEDULE_mem);
            if(a.getType() == HASH_failure)
                throw scheduleExceptions(SCHEDULE_failure);
        }
        if(!room)
            throw scheduleExceptions(SCHEDULE_failure);
        if(room->areThereLectures())
            throw scheduleExceptions(SCHEDULE_failure);
        try {
            rooms->pop(&roomID);
        }
        catch(hashExceptions a){
            if(a.getType() == HASH_mem)
                throw scheduleExceptions(SCHEDULE_mem);
            if(a.getType() == HASH_failure)
                throw scheduleExceptions(SCHEDULE_failure);
        }

    }

    void addLecture( int courseID, int groupID, int roomID, int hour, int numStudents){
        if(courseID > coursesUnion->getSize() || courseID < 1)
            throw scheduleExceptions(SCHEDULE_invalidInput);
        int unionCourseID = coursesUnion->find(courseID);
        Course* course = &courses[unionCourseID];
        Room* room;
        try {
            room = rooms->find(&roomID);
        }
        catch(hashExceptions a){
            if(a.getType() == HASH_mem)
                throw scheduleExceptions(SCHEDULE_mem);
            if(a.getType() == HASH_failure)
                throw scheduleExceptions(SCHEDULE_failure);
        }
        if(!room)
            throw scheduleExceptions(SCHEDULE_failure);
        if(!(room->checkIfHourAvailable(hour)) || course->isHourTaken(groupID, hour))
            throw scheduleExceptions(SCHEDULE_failure);
        course->numLecturesPlus1();
        course->addToNumStudents(numStudents);
        Lecture lecture(courseID, groupID, roomID, hour, numStudents);
        Tuple byGroupID(groupID, hour);
        Tuple byNumStudents(numStudents, groupID);
        try {
            course->addLecture(lecture, byGroupID, byNumStudents);
        }
        catch(courseExceptions a){
            if(a.getType() == COURSE_mem)
                throw scheduleExceptions(SCHEDULE_mem);
            if(a.getType() == COURSE_mem)
                throw scheduleExceptions(SCHEDULE_failure);
        }
        room->addLecture(lecture, hour);
    }

    void deleteLecture(int hour, int roomID){
        Room* room;
        try{
            room = rooms->find(&roomID);
        }
        catch(hashExceptions a){
            if(a.getType() == HASH_mem)
                throw scheduleExceptions(SCHEDULE_mem);
            if(a.getType() == HASH_failure)
                throw scheduleExceptions(SCHEDULE_failure);
        }
        if(!room)
            throw scheduleExceptions(SCHEDULE_failure);
        if(room->checkIfHourAvailable(hour))
            throw scheduleExceptions(SCHEDULE_failure);
        int courseID = room->getCourseID(hour);
        int groupID = room->getGroupID(hour);
        int numStudents = room->getNumOfStudents(hour);
        int unionCourseID = coursesUnion->find(courseID);
        Course course = courses[unionCourseID];
        Tuple byGroupID(groupID, hour);
        Tuple byNumStudents(numStudents, groupID);

        room->deleteLecture(hour);
        course.subFromNumStudents(numStudents);
        course.numLecturesMinus1();
        course.deleteLecture(byGroupID, byNumStudents);
    }

    void mergeArrays(AvlTree<Tuple,Lecture>::Node** lectures1,
                     AvlTree<Tuple,Lecture>::Node** lectures2,
                     AvlTree<Tuple,Lecture>::Node** combine,
                     int numOfLectures1,
                     int numOfLectures2){
        int i_1 = 0, i_2 = 0;
        for(int i=0; i<numOfLectures1 + numOfLectures2; i++)
        {
            if(i_1 == numOfLectures1){
                combine[i] = lectures2[i_2++];
                continue;
            }
            if(i_2 == numOfLectures2){
                combine[i] = lectures1[i_1];
                continue;
            }
            if((*lectures1[i_1]->getData()).getGroupID() == (*lectures2[i_2]->getData()).getGroupID())
                if((*lectures1[i_1]->getData()).getHour() == (*lectures2[i_2]->getData()).getHour())
                    throw scheduleExceptions(SCHEDULE_failure);
            if (*lectures1[i_1]->getKey() < *lectures2[i_2]->getKey()) {
                combine[i] = lectures1[i_1];
                continue;
            }
            if (*lectures2[i_2]->getKey() < *lectures1[i_1]->getKey()){ //is it really the key? or a pointer to key?
                combine[i] = lectures2[i_2];
            }
        }
    }

    void mergeStudentsArrays(AvlRankedTree<Tuple,Lecture>::Node** lectures1,
                             AvlRankedTree<Tuple,Lecture>::Node** lectures2,
                             AvlRankedTree<Tuple,Lecture>::Node** combine,
                             int numOfLectures1,
                             int numOfLectures2){
        int i_1 = 0, i_2 = 0;
        for(int i=0; i<numOfLectures1 + numOfLectures2; i++)
        {
            if(i_1 == numOfLectures1){
                combine[i] = lectures2[i_2++];
                continue;
            }
            if(i_2 == numOfLectures2){
                combine[i] = lectures1[i_1];
                continue;
            }
            if((*lectures1[i_1]->getData()).getGroupID() == (*lectures2[i_2]->getData()).getGroupID())
                if((*lectures1[i_1]->getData()).getHour() == (*lectures2[i_2]->getData()).getHour())
                    throw scheduleExceptions(SCHEDULE_failure);
            if (*lectures1[i_1]->getKey() < *lectures2[i_2]->getKey()) {
                combine[i] = lectures1[i_1];
                continue;
            }
            if (*lectures2[i_2]->getKey() < *lectures1[i_1]->getKey()){
                combine[i] = lectures2[i_2];
            }
        }
    }

    void mergeCourses(int courseID1, int courseID2){
        if(courseID1 > coursesUnion->getSize() || courseID1 < 1 || courseID2 > coursesUnion->getSize() || courseID2 < 1)
            throw scheduleExceptions(SCHEDULE_invalidInput);
        int unionCourseID1 = coursesUnion->find(courseID1);
        int unionCourseID2 = coursesUnion->find(courseID2);
        if(unionCourseID1 == unionCourseID2)
            throw scheduleExceptions(SCHEDULE_failure);
        Course course1 = courses[unionCourseID1];
        Course course2 = courses[unionCourseID2];
        int unitedCourseID = coursesUnion->unite(courseID1, courseID2);
        AvlTree<Tuple,Lecture>::Node** lectures1 = course1.getArrayOfLecturesByGroup();
        AvlTree<Tuple,Lecture>::Node** lectures2 = course2.getArrayOfLecturesByGroup();
        AvlRankedTree<Tuple,Lecture>::Node** lecturesByStudents1 = course1.getArrayOfLecturesByStudents();
        AvlRankedTree<Tuple,Lecture>::Node** lecturesByStudents2 = course2.getArrayOfLecturesByStudents();
        int numOfLectures1 = course1.getNumOfLectures();
        int numOfLectures2 = course2.getNumOfLectures();

        AvlTree<Tuple,Lecture>::Node** lectures_combined;
        try{
            lectures_combined = new AvlTree<Tuple,Lecture>::Node*[numOfLectures1+numOfLectures2];
        }
        catch (std::bad_alloc a) {
            throw scheduleExceptions(SCHEDULE_mem);
        }
        AvlRankedTree<Tuple,Lecture>::Node** lecturesByStudents_combined;
        try{
            lecturesByStudents_combined = new AvlRankedTree<Tuple,Lecture>::Node*[numOfLectures1+numOfLectures2];
        }
        catch (std::bad_alloc a) {
            delete [] lectures_combined;
            throw scheduleExceptions(SCHEDULE_mem);
        }

        try{
            mergeArrays(lectures1, lectures2, lectures_combined, numOfLectures1, numOfLectures2);
            mergeStudentsArrays(lecturesByStudents1, lecturesByStudents2, lecturesByStudents_combined, numOfLectures1, numOfLectures2);
        }
        catch (std::bad_alloc a) {
            delete [] lecturesByStudents_combined;
            delete [] lectures_combined;
            throw scheduleExceptions(SCHEDULE_mem);
        }

        AvlTree<Tuple,Lecture>* combinedLectures;
        try{
            combinedLectures = new AvlTree<Tuple,Lecture>(lectures_combined, numOfLectures1+numOfLectures2);
        }
        catch (std::bad_alloc a) {
            delete [] lecturesByStudents_combined;
            delete [] lectures_combined;
            throw scheduleExceptions(SCHEDULE_mem);
        }
        AvlRankedTree<Tuple,Lecture>* combinedLecturesByStudents;
        try{
            combinedLecturesByStudents = new AvlRankedTree<Tuple,Lecture>(lecturesByStudents_combined, numOfLectures1+numOfLectures2, 1);
        }
        catch (std::bad_alloc a) {
            delete combinedLectures;
            delete [] lecturesByStudents_combined;
            delete [] lectures_combined;
            throw scheduleExceptions(SCHEDULE_mem);
        }
        int numOfStudents1 = course1.getNumOfStudents();
        int numOfStudents2 = course2.getNumOfStudents();

        courses[unitedCourseID].replaceCourse(combinedLectures, combinedLecturesByStudents, numOfLectures1+numOfLectures2, numOfStudents1+numOfStudents2);
        //*************************************************************************************************
        //should I delete the trees in courses[courseID1 + courseID2 + unitedCourseID] ?????????????????????
        //*************************************************************************************************
        delete [] lecturesByStudents_combined;
        delete [] lectures_combined;
    }

    void competition(int courseID1, int courseID2, int numGroups, int * winner){
        if(courseID1 > coursesUnion->getSize() || courseID1 < 1 || courseID2 > coursesUnion->getSize() || courseID2 < 1)
            throw scheduleExceptions(SCHEDULE_invalidInput);
        int unionCourseID1 = coursesUnion->find(courseID1);
        int unionCourseID2 = coursesUnion->find(courseID2);
        if(unionCourseID1 == unionCourseID2)
            throw scheduleExceptions(SCHEDULE_failure);
        Course course1 = courses[unionCourseID1];
        Course course2 = courses[unionCourseID2];

        int score1 = course1.getSumScore(numGroups);
        int score2 = course2.getSumScore(numGroups);
        if(score1 > score2)
            *winner = courseID1;
        else
            *winner = courseID2;
    }

    void getAverageStudentsInCourse(int hour, int roomID, float * average){
        int unionCourseID = coursesUnion->find(rooms->find(&roomID)->getCourseID(hour));
        int students = courses[unionCourseID].getNumOfStudents();
        int lectures = courses[unionCourseID].getNumOfLectures();
        *average = (float)(students/lectures);
    }
};


#endif //WET2_SCHEDULE_H
