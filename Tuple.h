#ifndef WET2_TUPLE_H
#define WET2_TUPLE_H
class Tuple {
    int key1;
    int key2;

public:
    Tuple(int key1=-1, int key2=-1): key1(key1), key2(key2){}
    ~Tuple(){}

    bool operator<(const Tuple& tuple2) {
        if (key1 > tuple2.key1)
            return false;
        if (key1 < tuple2.key1)
            return true;
        return key2 < tuple2.key2;
    }

    bool operator==(const Tuple& tuple2) {
        return key1 == tuple2.key1 && key2 == tuple2.key2;
    }

    int getKey1(){
        return key1;
    }
    int getKey2(){
        return key2;
    }
};
#endif //WET2_TUPLE_H
