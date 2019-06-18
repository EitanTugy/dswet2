#ifndef WET2_COURSE_H
#define WET2_COURSE_H

#include "Lecture.h"
#include "Room.h"
#include "AvlTree.h"
#include "AvlRankedTree.h"
#include "methodTreeIntoArray.h"
#include "Tuple.h"

enum TYPE_COURSE_EXCEPTION {
    COURSE_mem = 1,
    COURSE_invalidInput,
    COURSE_failure
};
class courseExceptions {
    TYPE_COURSE_EXCEPTION type;
public:
    courseExceptions(TYPE_COURSE_EXCEPTION type)
    :
    type(type)
            {}
    TYPE_COURSE_EXCEPTION getType() {
        return type;
    }
};

class Course {
    AvlTree<Tuple,Lecture>* lectures;
    AvlRankedTree<Tuple,Lecture>* lecturesByStudents;
    int numOfLectures;
    int numOfStudents;
public:
    Course() : lectures(nullptr), lecturesByStudents(nullptr), numOfLectures(0), numOfStudents(0){
        try {
            lectures = new AvlTree<Tuple,Lecture>(1);
        }
        catch (std::bad_alloc a) {
            throw courseExceptions(COURSE_mem);
        }
        try {
            lecturesByStudents = new AvlRankedTree<Tuple,Lecture>(1);
        }
        catch (std::bad_alloc a) {
            delete lectures;
            throw courseExceptions(COURSE_mem);
        }
    }

    //Course(AvlTree<Tuple,Lecture>* lectures, AvlRankedTree<Tuple,Lecture>* lecturesByStudents, int numOfLectures, int numOfStudents)
    //    : lectures(nullptr), lecturesByStudents(nullptr), numOfLectures(numOfLectures), numOfStudents(numOfStudents){
    //    lectures = lectures;
    //    lecturesByStudents = lecturesByStudents;
    //}

    ~Course(){
        if(lecturesByStudents)
            delete lecturesByStudents;
        if(lectures)
            delete lectures;
    }

    AvlTree<Tuple,Lecture>::Node** getArrayOfLecturesByGroup(){
        methodTreeIntoArrayOfLectures mt = methodTreeIntoArrayOfLectures(numOfLectures);
        lectures->inOrder(mt);
        return mt.getArray();
    }
    AvlRankedTree<Tuple,Lecture>::Node** getArrayOfLecturesByStudents(){
        methodRankedTreeIntoArrayOfLectures mt = methodRankedTreeIntoArrayOfLectures(numOfLectures);
        lecturesByStudents->inOrder(mt);
        return mt.getArray();
    }

    bool isHourTaken(int groupID, int hour){
        if(numOfLectures == 0)
            return false;
        Tuple t(groupID, hour);
        try{
            lectures->find(&t);
        }
        catch (avlExceptions &a) {
            return false;
        }
        return true;

    }
    void numLecturesPlus1(){
        numOfLectures++;
    }
    void numLecturesMinus1(){
        numOfLectures--;
    }
    void addToNumStudents(int num){
        numOfStudents = numOfStudents + num;
    }
    void subFromNumStudents(int num){
        numOfStudents = numOfStudents - num;
    }
    int getNumOfLectures(){
        return numOfLectures;
    }
    int getNumOfStudents(){
        return numOfStudents;
    }

    void replaceCourse(AvlTree<Tuple,Lecture>* lectures_replacement,
                       AvlRankedTree<Tuple,Lecture>* lecturesByStudents_replacement,
                       int numOfLectures_replacement,
                       int numOfStudents_replacement){
        AvlTree<Tuple,Lecture>* lectures_tmp = lectures;
        AvlRankedTree<Tuple,Lecture>* lecturesByStudents_tmp = lecturesByStudents;

        lectures = lectures_replacement;
        lecturesByStudents = lecturesByStudents_replacement;
        numOfLectures = numOfLectures_replacement;
        numOfStudents = numOfStudents_replacement;

        delete lectures_tmp;
        delete lecturesByStudents_tmp;


    }

    void addLecture(Lecture lecture, Tuple byGroupID, Tuple byNumStudents){
        try{
            lectures->insert(&byGroupID, &lecture);
        }
        catch(avlrExceptions a){
            if(a.getType() == AVLR_mem)
                throw courseExceptions(COURSE_mem);
            if(a.getType() == AVLR_failure)
                throw courseExceptions(COURSE_failure);
        }
        try{
            lecturesByStudents->insert(&byNumStudents, &lecture, byNumStudents.getKey1());
        }
        catch(avlrExceptions a){
            lectures->pop(&byGroupID);
            if(a.getType() == AVLR_mem)
                throw courseExceptions(COURSE_mem);
            if(a.getType() == AVLR_failure)
                throw courseExceptions(COURSE_failure);
        }
    }

    void deleteLecture(Tuple byGroupID, Tuple byNumStudents){
        try{
            lectures->pop(&byGroupID);
            lecturesByStudents->pop(&byNumStudents);
        }
        catch(avlrExceptions a){
            if(a.getType() == AVLR_mem)
                throw courseExceptions(COURSE_mem);
            if(a.getType() == AVLR_failure)
                throw courseExceptions(COURSE_failure);
        }
    }

    int getSumScore(int numGroups){
        return lecturesByStudents->getSumScoresOfCourse(numGroups);
    }

};

#endif //WET2_COURSE_H
