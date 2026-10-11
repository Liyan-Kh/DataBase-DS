#ifndef LINKEDLISTCOLLECTION_H
#define LINKEDLISTCOLLECTION_H

#include <vector>
#include "Collection.h"

using namespace std;

class LinkedListCollection : public Collection {
    struct Node {
        Student data;
        Node* next;
        Node* prev;
    };
    Node* head;
    Node* tail;

public:
    LinkedListCollection();
    ~LinkedListCollection();

    void insertOne(const Student& student) override;
    void deleteOne(int id) override;
    Student* findByID(int id) override;
    vector<Student> findAll() override;

    vector<Student> filter(const std::string& field, const std::string& value) override;
    double sum(const string& field) override;
    double average(const string& field) override;
    int count() override;

    bool importCSV(const string& filename, bool hasHeader = true) override;
    bool exportCSV(const string& filename, bool writeHeader = true) override;

};

#endif