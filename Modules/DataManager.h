#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

template<typename... Args>
static void Write(const string& filename, const Args&... vars) {
    ofstream file(filename, ios::app);
    ((file << vars << " "), ...);
    file << "\n";
    file.close();
}

template<typename... Args>
static void Read(const string& filename, Args&... vars) {
    ifstream file(filename);
    string line;
    if (getline(file, line)) {
        istringstream iss(line);
        ((iss >> vars), ...);
    }
    file.close();
}

template<typename T>
static void Fetch(const string& filename, vector<T>& Vector)
{
    ifstream file(filename);
    Vector.clear();
    T temp;
    while (file >> temp) {
        Vector.push_back(temp);
    }
    file.close();
}

static void Log(const string& filename)
{
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
}