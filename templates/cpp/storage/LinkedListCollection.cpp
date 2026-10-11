#include "LinkedListCollection.h"
#include <fstream>
#include <sstream>
#include <cctype>

LinkedListCollection::LinkedListCollection(){
    head= nullptr;
    tail= nullptr;
}
LinkedListCollection::~LinkedListCollection() {
    Node* cur = head;
    while (cur!= nullptr){
        Node* nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }
    head= nullptr;
    tail= nullptr;
}


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
bool LinkedListCollection::importCSV(const std::string& filename, bool hasHeader) {
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

bool LinkedListCollection::exportCSV(const std::string& filename, bool writeHeader) {
    std::ofstream fout(filename);
    if (!fout.is_open()) return false;

    if (writeHeader) fout << "id,name,gpa\n";

    Node* cur = head;
    while (cur != nullptr) {
        const Student& s = cur->data;
        fout << s.id << "," << csvEscape(s.name) << "," << s.gpa << "\n";
        cur = cur->next;
    }
    return true;
}
vector<Student> LinkedListCollection::filter(const std::string &field, const std::string &value) {
    vector<Student> result;

    Node* cur = head;
    while (cur != nullptr){
        const Student& s = cur->data;

        if (field == "name"){
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

        cur = cur->next;
    }
    return result;
}

int LinkedListCollection::count() {
    int c=0;
    Node* cur = head;
    while(cur!= nullptr){
        c++;
        cur = cur->next;
    }
    return c;
}

double LinkedListCollection::sum(const std::string& field) {
    double total = 0.0;
    Node* cur = head;

    if (field == "gpa") {
        while (cur != nullptr) {
            total += cur->data.gpa;
            cur = cur->next;
        }
        return total;
    }

    if (field == "id") {
        while (cur != nullptr) {
            total += cur->data.id;
            cur = cur->next;
        }
        return total;
    }

    return 0.0;
}

double LinkedListCollection::average(const std::string& field) {
    int n = count();
    if (n == 0) return 0.0;
    return sum(field) / (double)n;
}

void LinkedListCollection::insertOne(const Student &student) {
    Node* newNode = new Node;
    newNode->data = student;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (head== nullptr){
        head= newNode;
        tail= newNode;
        return;
    }

    tail->next=newNode;
    newNode->prev= tail;
    tail = newNode;
}

void LinkedListCollection::deleteOne(int id) {
    Node* cur = head;

    while (cur!= nullptr){
        if (cur->data.id == id){
            //delete from the head
            if (cur==head){
                head = cur->next;
                if (head!= nullptr){
                    head->prev= nullptr;
                } else {
                    tail= nullptr;
                }
            }
            else if (cur==tail){
                tail = cur->prev;
                tail->next= nullptr;
            }
            else{
                cur->prev->next=cur->next;
                cur->next->prev=cur->prev;
            }
            delete cur;
            return;
        }
        cur = cur->next;
    }
}
Student* LinkedListCollection::findByID(int id) {
    Node* cur = head;
    while (cur != nullptr){
        if (cur->data.id == id){
            return &cur->data;
        }
        cur = cur->next;
    }
    return nullptr;
}

vector<Student> LinkedListCollection::findAll() {
    vector<Student> result;
    Node* cur = head;
    while (cur!= nullptr){
        result.push_back(cur->data);
        cur = cur->next;
    }
    return result;
}