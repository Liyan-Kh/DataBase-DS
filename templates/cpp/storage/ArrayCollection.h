#ifndef ARRAYCOLLECTION_H
#define ARRAYCOLLECTION_H

#include <vector>
#include <string>
#include "Collection.h"

class ArrayCollection : public Collection {
private:
    std::vector<Student> data;

public:
    void insertOne(const Student& student) override;
    void deleteOne(int id) override;
    Student* findByID(int id) override;
    std::vector<Student> findAll() override;

    std::vector<Student> filter(const std::string& field, const std::string& value) override;
    int count() override;
    double sum(const string& field) override;
    double average(const string& field) override;

    bool importCSV(const string& filename, bool hasHeader = true) override;
    bool exportCSV(const string& filename, bool writeHeader = true) override;

    ~ArrayCollection() override = default;
};

#endif
