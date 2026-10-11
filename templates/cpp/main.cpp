    #include <iostream>
    #include <string>

    #include "storage/ArrayCollection.h"
    #include "engine/ExecutionEngine.h"
    #include "parser/QueryParser.h"


    using namespace std;


    int main() {
        ArrayCollection collection;
        ExecutionEngine engine(&collection);
        QueryParser parser;

        string input;
        cout << "UniDB Shell (C++)\nType 'exit' to quit.\n";

        while (true) {
            cout << "unidb> ";
            getline(cin, input);
            if (input == "exit")
                break;

            Command cmd;
            if (!parser.parse(input, cmd)){
                cout << "Invalid command.\n";
                continue;
            }

            EngineResult result = engine.execute(cmd);
            if (!result.ok){
                cout << "Error: " << result.message << "\n";
                continue;
            }
            if (cmd.type == CommandType::FindByID){
                if (result.hasStudent){
                    cout << result.student.id << " " << result.student.name << " " << result.student.gpa << "\n";
                } else{
                    cout << "Student not found.\n";
                }
            }
            else if (cmd.type == CommandType::FindAll){
                for (int i = 0; i < (int)result.students.size(); ++i) {
                    cout << result.students[i].id << " " << result.students[i].name << " " << result.students[i].gpa << "\n";
                }
            }
            else if (cmd.type == CommandType::Count || cmd.type == CommandType::Sum || cmd.type == CommandType::Average) {
                cout << result.number << "\n";
            }
            else{
                cout << result.message << "\n";
            }
        }

        return 0;
    }

