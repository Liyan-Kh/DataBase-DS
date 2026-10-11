#ifndef COLLECTION_H
#define COLLECTION_H
#include <iostream>
#include <vector>

#include "../models/Student.h"


using namespace std;


class Collection {
public:
    virtual void insertOne(const Student& student) = 0;
    virtual void deleteOne(int id) = 0;
    virtual Student* findByID(int id) = 0;
    virtual vector<Student> findAll() = 0;

    virtual vector<Student> filter(const string& field, const string& value)=0;
    virtual int count() = 0;
    virtual double sum(const string& field)=0;
    virtual double average(const string& field)=0;

    virtual bool importCSV(const string& filename, bool hasHeader = true) = 0;
    virtual bool exportCSV(const string& filename, bool writeHeader = true) = 0;

    virtual ~Collection() = 0;
};
#endif