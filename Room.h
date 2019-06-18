//
// Created by eitan on 07-Jun-19.
//

#ifndef WET2_ROOM_H
#define WET2_ROOM_H

#include "Lecture.h"

enum TYPE_ROOM_EXCEPTION {
    ROOM_mem = 1,
    ROOM_invalidInput,
    ROOM_failure
};
class roomExceptions {
    TYPE_ROOM_EXCEPTION type;
public:
    roomExceptions(TYPE_ROOM_EXCEPTION type)
            :
            type(type)
    {}
    TYPE_ROOM_EXCEPTION getType() {
        return type;
    }
};

class Room {
    int roomID;
    Lecture lectures[10];
    bool freeHours[10];
    int numOfLectures;

public:
    explicit Room(int roomID/*, int numOfHours*/) : roomID(roomID), numOfLectures(0) {
        /*try {
            freeHours = new bool[numOfHours];
        }
        catch (std::bad_alloc a) {
            throw roomExceptions(ROOM_mem);
        }*/
        for (int i = 0; i < 10 /* numOfHours*/; i++) {
            freeHours[i] = true;
        }
        /*try{
            lectures = new Lecture[numOfHours];
        }
        catch (std::bad_alloc a) {
            delete [] freeHours;
            throw roomExceptions(ROOM_mem);
        }*/
    }

    ~Room(){
       //delete [] lectures;
       //delete [] freeHours;
    }

    bool checkIfHourAvailable(int hour){
        return freeHours[hour];
    }

    bool areThereLectures(){
        return numOfLectures;
    }

    void addLecture(Lecture lecture, int hour){
        lectures[hour] = lecture;
        freeHours[hour] = 0;
    }

    void deleteLecture(int hour){
        //Lecture lecture = Lecture();
        //lectures[hour] = lecture;
        freeHours[hour] = true;
    }

    int getGroupID(int hour){
        return lectures[hour].getGroupID();
    }
    int getHour(int hour){
        return lectures[hour].getHour();
    }
    int getRoomID(int hour){
        return lectures[hour].getRoomID();
    }
    int getNumOfStudents(int hour){
        return lectures[hour].getNumOfStudents();
    }
    int getCourseID(int hour){
        return lectures[hour].getCourseID();
    }
};

#endif //WET2_ROOM_H
