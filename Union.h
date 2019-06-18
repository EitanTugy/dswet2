#ifndef WET2_UNION_H
#define WET2_UNION_H
#pragma once
#include <exception>
#include <iostream>
enum TYPE_UNION_EXCEPTION {
    UNION_mem = 1,
    UNION_invalidInput,
    UNION_failure
};
class unionExceptions {
    TYPE_UNION_EXCEPTION type;
public:
    unionExceptions(TYPE_UNION_EXCEPTION type)
            :
            type(type)
    {}
    TYPE_UNION_EXCEPTION getType() {
        return type;
    }
};

class Union
{
    private:
        int size;
        int* sizes;
        int* parents;
    public:
    explicit Union(int size) : size(size), sizes(nullptr), parents(nullptr)
    {
        try {
            sizes = new int[size + 1];
            parents = new int[size + 1];
        }
        catch (std::bad_alloc a) {
            throw unionExceptions(UNION_mem);
        }
        for(int i=1; i<size + 1; i++){
            sizes[i] = 0;
            parents[i] = -1;
        }
    }
    ~Union()
    {
        delete[] parents;
        delete[] sizes;
    }

    int find(int n){
        int a = n;
        while(parents[a] != -1)
            a = parents[a];
        while(parents[n] != -1) {
            int tmp = parents[n];
            parents[n] = a;
            n = tmp;
        }
        return a-1;
    }

    int unite(int a, int b){
        a = find(a);
        b = find(b);
        if(a == b){
            return a-1;
        }
        if(sizes[a] <= sizes[b]){
            sizes[b] += sizes[a];
            parents[a] = b;
            return b-1;
        }
        else {
            sizes[a] += sizes[b];
            parents[b] = a;
            return a-1;
        }

    }

    int getSize(){
        return size;
    }

};
#endif //WET2_UNION_H
