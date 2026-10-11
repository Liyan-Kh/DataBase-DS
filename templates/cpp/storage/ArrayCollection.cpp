#include "ArrayCollection.h"

using namespace std;
#include <fstream>
#include <sstream>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t a = 0;
    while (a < s.size() && std::isspace((unsigned char)s[a])) a++;
    size_t b = s.size();
    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

static std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> out;
    std::string cell;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                cell.push_back('"');
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            out.push_back(trim(cell));
            cell.clear();
        } else {
            cell.push_back(c);
        }
    }
    out.push_back(trim(cell));
    return out;
}

static std::string csvEscape(const std::string& s) {
    bool needQuotes = (s.find(',') != std::string::npos) || (s.find('"') != std::string::npos);
    if (!needQuotes) return s;

    std::string escaped;
    for (char c : s) {
        if (c == '"') escaped += "\"\"";
        else escaped.push_back(c);
    }
    return "\"" + escaped + "\"";
}
bool ArrayCollection::importCSV(const std::string& filename, bool hasHeader) {
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    std::string line;
    if (hasHeader) std::getline(fin, line);

    while (std::getline(fin, line)) {
        if (trim(line).empty()) continue;
        auto cols = parseCSVLine(line);
        if (cols.size() < 3) continue;

        try {
            Student s;
            s.id = std::stoi(cols[0]);
            s.name = cols[1];
            s.gpa = std::stod(cols[2]);
            insertOne(s);
        } catch (...) {

        }
    }
    return true;
}

bool ArrayCollection::exportCSV(const std::string& filename, bool writeHeader) {
    std::ofstream fout(filename);
    if (!fout.is_open()) return false;

    if (writeHeader) fout << "id,name,gpa\n";
    for (const Student& s : data) {
        fout << s.id << "," << csvEscape(s.name) << "," << s.gpa << "\n";
    }
    return true;
}
vector<Student> ArrayCollection::filter(const std::string &field, const std::string &value) {
    vector<Student> result;

    for (int i = 0; i < (int)data.size(); ++i) {
        const Student& s = data[i];

        if (field == "name") {
            if (s.name == value)
                result.push_back(s);
        }

        else if (field == "id"){
            int v = stoi(value);
            if (s.id == v){
                result.push_back(s);
            }
        }

        else if (field == "gpa"){
            double v = stod(value);
            if (s.gpa == v){
                result.push_back(s);
            }
        }

    }
    return result;
}

int ArrayCollection::count() {
    return (int)data.size();
}

double ArrayCollection::sum(const std::string& field) {
    double total = 0.0;

    if (field == "gpa") {
        for (int i = 0; i < (int)data.size(); ++i) total += data[i].gpa;
        return total;
    }
    if (field == "id") {
        for (int i = 0; i < (int)data.size(); ++i) total += data[i].id;
        return total;
    }

    return 0.0;
}

double ArrayCollection::average(const std::string& field) {
    if (data.empty())
        return 0.0;
    return sum(field) / (double)data.size();
}

void ArrayCollection::insertOne(const Student &student) {
    data.push_back(student);
}
void ArrayCollection::deleteOne(int id) {
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].id == id){
            data.erase(data.begin()+i);
            return;
        }
    }
}
Student* ArrayCollection::findByID(int id) {
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].id == id){
            return &data[i];
        }
    }
    return nullptr;
}
vector<Student> ArrayCollection::findAll()  {
    return data;
}