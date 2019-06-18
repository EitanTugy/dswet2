#ifndef WET2_LECTURE_H
#define WET2_LECTURE_H

#include <exception>

enum TYPE_LECTURE_EXCEPTION {
    LECTURE_mem = 1,
    LECTURE_invalidInput,
    LECTURE_failure
};
class lectureExceptions {
    TYPE_LECTURE_EXCEPTION type;
public:
    lectureExceptions(TYPE_LECTURE_EXCEPTION type)
            :
            type(type)
    {}
    TYPE_LECTURE_EXCEPTION getType() {
        return type;
    }
};

class Lecture {
    int groupID;
    int hour;
    int roomID;
    int numOfStudents;
    int courseID;

public:
    explicit Lecture(int courseID = -1, int groupID = -1, int roomID = -1, int hour = -1, int numOfStudents = -1)
    : groupID(groupID), hour(hour), roomID(roomID), numOfStudents(numOfStudents), courseID(courseID)
    {}

    ~Lecture(){}

    int getGroupID(){
        return groupID;
    }
    int getHour(){
        return hour;
    }
    int getRoomID(){
        return roomID;
    }
    int getNumOfStudents(){
        return numOfStudents;
    }
    int getCourseID(){
        return courseID;
    }

    bool operator==(const Lecture& lecture2) {
        return groupID == lecture2.groupID &&
               hour == lecture2.hour &&
               roomID == lecture2.roomID &&
               numOfStudents == lecture2.numOfStudents &&
               courseID == lecture2.courseID;
    }

};

#endif //WET2_LECTURE_H
