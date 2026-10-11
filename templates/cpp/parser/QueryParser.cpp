#include "QueryParser.h"
#include <string>

using namespace std;
string QueryParser::trim(const string &s) {
    int start = 0;
    int end = (int)s.size() -1;

    while (start<= end && s[start] == ' ')
        start++;
    while (end>= start && s[end] == ' ')
        end--;

    if (start > end)
        return "";

    return s.substr(start, end - start + 1);
}

bool QueryParser::startsWith(const string &s, const string &prefix) {
    if (s.size() < prefix.size())
        return false;
    else
        return s.compare(0, prefix.size(), prefix) == 0;
}

string QueryParser::insideParenthesis(const string &s) {
    int l= -1 , r= -1;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '(') {
            l = i;
            break;
        }
    }
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        if (s[i] == ')') {
            r = i;
            break;
        }
    }
    if (l == -1 || r == -1 || r <= l)
        return "";

    return trim(s.substr(l+1, r - l - 1));
}

string QueryParser::stripQuotes(const std::string &s) {
    string t = trim(s);
    if (t.size() >= 2 && t[0]=='"' && t[t.size()-1]=='"'){
        return t.substr(1,t.size()-2);
    }
    return t;
}

bool QueryParser::parseInt(const string& t, int& out) {
    string s = trim(t);
    if (s.empty()) return false;

    int sign = 1;
    int i = 0;
    if (s[0] == '-') { sign = -1; i = 1; }

    long long val = 0;
    for (; i < (int)s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
        val = val * 10 + (s[i] - '0');
    }

    out = (int)(sign * val);
    return true;
}

bool QueryParser::parseDouble(const std::string &s, double &out) {
    string t= trim(s);
    if (t.empty())
        return false;
    int i=0;
    bool neg = false;
    if (t[0]=='-'){
        neg = true;
        i= 1;
    }
    double val = 0.0;
    for (; i < (int)t.size() && t[i] != '.' ; i++) {
        if (t[i]<'0'|| t[i]>'9')
            return false;
        val = val * 10 + (t[i] - '0');
    }

    if (i < (int)t.size() && t[i]=='.'){
        i++;
        double factor = 0.1;
        for(; i< (int)t.size(); i++){
            if (t[i] < '0' || t[i]> '9')
                return false;
            val +=(t[i] - '0') * factor;
            factor *= 0.1;
        }
    }
    if (neg)
        val = -val;
    out = val;
    return true;

}

string QueryParser::extractValue(const string& text, int keyPos) {
    int colon = (int)text.find(':', keyPos);
    if (colon == -1) return "";

    int comma = (int)text.find(',', colon + 1);
    if (comma == -1) {
        return trim(text.substr(colon + 1));
    }

    return trim(text.substr(colon + 1, comma - (colon + 1)));
}

bool QueryParser::parseStudentObject(const string& text, Student& out) {
    string t = trim(text);


    if (t.size() < 2 || t.front() != '{' || t.back() != '}') return false;
    t = trim(t.substr(1, t.size() - 2));


    vector<string> parts;
    string cur;
    bool inQuotes = false;

    for (char c : t) {
        if (c == '"') inQuotes = !inQuotes;

        if (c == ',' && !inQuotes) {
            parts.push_back(trim(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) parts.push_back(trim(cur));

    bool hasId = false, hasName = false, hasGpa = false;

    for (const string& p : parts) {
        int colon = (int)p.find(':');
        if (colon == (int)string::npos) return false;

        string key = trim(p.substr(0, colon));
        string val = trim(p.substr(colon + 1));

        if (key == "id") {
            int id;
            if (!parseInt(val, id)) return false;
            out.id = id;
            hasId = true;
        } else if (key == "name") {
            out.name = stripQuotes(val);
            hasName = true;
        } else if (key == "gpa") {
            double g;
            if (!parseDouble(val, g)) return false;
            out.gpa = g;
            hasGpa = true;
        }
    }

    return hasId && hasName && hasGpa;
}

bool QueryParser::splitTwoArgs(const string& args, string& left, string& right) {
    int comma = (int)args.find(',');
    if (comma == -1)
        return false;

    left = stripQuotes(trim(args.substr(0, comma)));
    right = stripQuotes(trim(args.substr(comma + 1)));

    return true;
}

bool QueryParser::parse(const std::string &input, Command &outCommand) const {
    string line = trim(input);
    if (line.empty())
        return false;

    if (line == "db.beginTransaction()"){
        outCommand.type = CommandType::BeginTransaction;
        return true;
    }
    if (line == "db.rollback()"){
        outCommand.type = CommandType::Rollback;
        return true;
    }
    if (line == "db.commit()"){
        outCommand.type = CommandType::Commit;
        return true;
    }
    if (line == "db.batch.start()"){
        outCommand.type = CommandType::BatchStart;
        return true;
    }
    if (line == "db.batch.execute()"){
        outCommand.type = CommandType::BatchExecute;
        return true;
    }
    if (!startsWith(line, "db.students."))
        return false;
    if (startsWith(line, "db.students.insertOne")) {
        outCommand.type = CommandType::InsertOne;

        Student s;
        if (!parseStudentObject(insideParenthesis(line), s))
            return false;

        outCommand.student = s;
        return true;
    }
    if (startsWith(line, "db.students.deleteOne")){
        outCommand.type = CommandType::DeleteOne;
        Student s;
        if (!parseStudentObject(insideParenthesis(line),s))
            return false;
        outCommand.id = s.id;
        return true;
    }
    if (startsWith(line, "db.students.findByID")) {
        outCommand.type = CommandType::FindByID;
        string inside = trim(insideParenthesis(line));
        int id;
        if (!parseInt(inside, id)) return false;
        outCommand.id = id;
        return true;
    }
    if (startsWith(line,"db.students.findAll")){
        outCommand.type = CommandType::FindAll;
        return true;
    }
    if (startsWith(line, "db.students.import")){
        outCommand.type = CommandType::ImportCSV;
        outCommand.filename = stripQuotes(insideParenthesis(line));
        return true;
    }
    if (startsWith(line, "db.students.filter")){
        outCommand.type = CommandType::Filter;
        if (!splitTwoArgs(insideParenthesis(line), outCommand.field, outCommand.value))
            return false;
        return true;
    }
    if (line == "db.students.count()"){
        outCommand.type = CommandType::Count;
        return true;
    }
    if (startsWith(line, "db.students.sum")){
        outCommand.type = CommandType::Sum;
        outCommand.field = stripQuotes(trim(insideParenthesis(line)));
        return true;
    }
    if (startsWith(line, "db.students.average")){
        outCommand.type = CommandType::Average;
        outCommand.field = stripQuotes(trim(insideParenthesis(line)));
        return true;
    }
    return false;
}