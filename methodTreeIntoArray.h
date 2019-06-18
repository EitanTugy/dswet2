#pragma once
#ifndef METHODTREEINTOARRAY_H
#define METHODTREEINTOARRAY_H
#include "AvlTree.h"
#include "Course.h"
#include "Tuple.h"
//template<class T>
class methodTreeIntoArrayOfLectures {
    AvlTree<Tuple,Lecture>::Node** lectures;
    int index;
public:
    methodTreeIntoArrayOfLectures(int numOfLectures) : lectures(nullptr) {
        lectures = new AvlTree<Tuple,Lecture>::Node*[numOfLectures];
        index = 0;
    }

    ~methodTreeIntoArrayOfLectures(){
        delete [] lectures;
    }

    void doSomething(Tuple tuple, Lecture lecture)
    {
        lectures[index] = new AvlTree<Tuple,Lecture>::Node(&tuple, &lecture, 1);
        index++;
    }

    AvlTree<Tuple,Lecture>::Node** getArray(){
        return lectures;
    }

};

class methodRankedTreeIntoArrayOfLectures {
    AvlRankedTree<Tuple,Lecture>::Node** lecturesByStudents;
    int index;
public:
    methodRankedTreeIntoArrayOfLectures(int numOfLectures): lecturesByStudents(nullptr){
        lecturesByStudents = new AvlRankedTree<Tuple,Lecture>::Node*[numOfLectures];
        index = 0;
    }

    ~methodRankedTreeIntoArrayOfLectures(){
        delete [] lecturesByStudents;
    }

    void doSomething(Tuple tuple, Lecture lecture)
    {
        int value = lecture.getNumOfStudents();
        lecturesByStudents[index] = new AvlRankedTree<Tuple,Lecture>::Node(&tuple, &lecture, value, 1);
        index++;
    }

    AvlRankedTree<Tuple,Lecture>::Node** getArray(){
        return lecturesByStudents;
    }
};
#endif