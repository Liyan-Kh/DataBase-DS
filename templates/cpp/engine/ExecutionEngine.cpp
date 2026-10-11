#include "ExecutionEngine.h"

ExecutionEngine::ExecutionEngine(Collection *c) {
    currentCollection = c;
    batchMode = false;
}

void ExecutionEngine::restoreFromSnapshot(const vector<Student>& snapshot) {
    vector<Student> current = currentCollection->findAll();

    for (int i = 0; i < (int)current.size(); ++i)
        currentCollection->deleteOne(current[i].id);

    for (int i = 0; i < (int)snapshot.size(); ++i)
        currentCollection->insertOne(snapshot[i]);
}

EngineResult ExecutionEngine::beginTransaction() {
    EngineResult res;
    vector<Student> state = currentCollection->findAll();
    txStack.pushSnapshot(state);
    res.message = "Transaction started.";
    return res;
}

EngineResult ExecutionEngine::rollBack() {
    EngineResult res;

    if (txStack.isEmpty()){
        res.ok = false;
        res.message = "No active transaction to rollback.";
        return res;
    }

    vector<Student> snapshot = txStack.popSnapshot();
    restoreFromSnapshot(snapshot);

    res.message = "Rollback done.";
    return res;

}

EngineResult ExecutionEngine::commit() {
    EngineResult res;

    if (txStack.isEmpty()){
        res.ok = false;
        res.message = "No active transaction to commit.";
        return res;
    }

    txStack.discardTop();
    res.message = "Commit done.";
    return res;
}

EngineResult ExecutionEngine::startBatch() {
    EngineResult res;
    batchMode = true;
    while (!batchQueue.empty())
        batchQueue.pop();
    res.message = "Batch mode started.";
    return res;
}

EngineResult ExecutionEngine::executeBatch() {
    EngineResult res;

    batchMode = false;
    int executed = 0;
    while (!batchQueue.empty()){
        Command c = batchQueue.front();
        batchQueue.pop();

        EngineResult r = execute(c);
        if(!r.ok){
            res.ok = false;
            res.message = "Batch failed after" + to_string(executed) + "command" + r.message;
            return res;
        }

        executed++;
    }
    res.message = "Batch executed: " + to_string(executed) + "commands.";
    return res;
}
EngineResult ExecutionEngine::execute(const Command &cmd) {
    EngineResult res;

    if (batchMode && cmd.type!= CommandType::BatchExecute){
        batchQueue.push(cmd);
        res.message = "Command queued (batch mode)";
        return res;
    }

    switch (cmd.type) {
        case CommandType::BeginTransaction:
            return beginTransaction();
        case CommandType::Rollback:
            return rollBack();
        case CommandType::Commit:
            return commit();
        case CommandType::BatchStart:
            return startBatch();
        case CommandType::BatchExecute:
            return executeBatch();
        case CommandType::InsertOne:
            currentCollection->insertOne(cmd.student);
            res.message = "Inserted student.";
            return res;
        case CommandType::DeleteOne:
            currentCollection->deleteOne(cmd.id);
            res.message = "Deleted student (in case of existing).";
            return res;
        case CommandType::FindByID:{
            Student* s = currentCollection->findByID(cmd.id);
            if (s== nullptr){
                res.ok = false;
                res.message = "Student not found.";
                return res;
            }
            res.hasStudent = true;
            res.student = *s;
            res.message = "Student found.";
            return res;
        }
        case CommandType::FindAll:
            res.students = currentCollection->findAll();
            res.message = "Returned all students.";
            return res;
        case CommandType::Filter:
            res.students = currentCollection->filter(cmd.field, cmd.value);
            res.message = "Filter done.";
            return res;
        case CommandType::Count:
            res.hasNumber = true;
            res.number = (double)currentCollection->count();
            res.message = "Count done.";
            return res;
        case CommandType::Sum:
            res.hasNumber = true;
            res.number = currentCollection->sum(cmd.field);
            res.message = "Sum done.";
            return res;

        case CommandType::Average:
            res.hasNumber = true;
            res.number = currentCollection->average(cmd.field);
            res.message = "Average done.";
            return res;
        case CommandType::ImportCSV: {
            bool ok = currentCollection->importCSV(cmd.filename, true);
            if (!ok) {
                res.ok = false;
                res.message = "Could not open CSV file.";
                return res;
            }
            res.message = "CSV imported.";
            return res;
        }
        case CommandType::ExportCSV: {
            bool ok = currentCollection->exportCSV(cmd.filename, true);
            if (!ok) {
                res.ok = false;
                res.message = "Could not write CSV file.";
                return res;
            }
            res.message = "CSV exported.";
            return res;
        }

        default:
            res.ok = false;
            res.message = "Command not implemented.";
            return res;
    }
}