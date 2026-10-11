#ifndef EXECUTIONENGINE_H
#define EXECUTIONENGINE_H

#include <vector>
#include <string>
#include <queue>

#include "Command.h"
#include "TransactionStack.h"
#include "../storage/Collection.h"

using namespace std;

struct EngineResult {
    bool ok = true;
    string message;

    bool hasStudent = false;
    Student student;
    vector<Student> students;

    bool hasNumber = false;
    double number = 0.0;
};

class ExecutionEngine{
private:
    Collection* currentCollection;

    TransactionStack txStack;
    bool batchMode;
    queue<Command> batchQueue;

    void restoreFromSnapshot(const vector<Student>& snapshot);
public:
    ExecutionEngine(Collection* c);

    EngineResult execute(const Command& cmd);

    EngineResult beginTransaction();
    EngineResult rollBack();
    EngineResult commit();

    EngineResult startBatch();
    EngineResult executeBatch();
};

#endif