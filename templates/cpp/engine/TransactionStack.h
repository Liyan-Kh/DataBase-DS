#include <vector>
#include "../models/Student.h"

#ifndef TRANSACTIONSTACK_H
#define TRANSACTIONSTACK_H

using namespace std;

class TransactionStack {
private:
    vector<vector<Student>> snapshots;
public:
    void pushSnapshot(const vector<Student>& state){
        snapshots.push_back(state);
    }
    bool isEmpty() const {
        return snapshots.empty();
    }
    vector<Student> popSnapshot(){
        vector<Student> top = snapshots.back();
        snapshots.pop_back();
        return top;
    }
    void discardTop(){
        if (!snapshots.empty()){
            snapshots.pop_back();
        }
    }
    int size() const {
        return (int)snapshots.size();
    }
};
#endif