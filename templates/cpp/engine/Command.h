#ifndef COMMAND_H
#define COMMAND_H


#include <string>
#include <vector>
#include "../models/Student.h"

using namespace std;

enum class CommandType {

    InsertOne,
    DeleteOne,
    FindByID,
    FindAll,

    BeginTransaction,
    Rollback,
    Commit,

    BatchStart,
    BatchExecute,

    Filter,
    Count,
    Sum,
    Average,
    ImportCSV,
    ExportCSV
};

struct Command{
    CommandType type;

    Student student;
    int id = -1;
    string field;
    string value;
    string filename;
};
#endif