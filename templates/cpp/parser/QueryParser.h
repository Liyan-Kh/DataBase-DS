#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <string>
#include "../engine/Command.h"

using namespace std;

class QueryParser{
private:
    static string extractValue(const string& text, int keyPos);
    static string trim(const string& s);
    static bool startsWith(const string& s, const string& prefix);
    static string insideParenthesis(const string& s);
    static string stripQuotes(const string& s);

    static bool parseInt(const string& s, int& out);
    static bool parseDouble(const string& s, double& out);

    static bool parseStudentObject(const string& objText, Student& outStudent);
    static bool splitTwoArgs(const string& args, string& left, string& right);

public:
    bool parse(const string& input, Command& outCommand) const;
};
#endif