#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Files
{
private:
    vector<string*> members;
    vector<string> files;
public:
    Files(){};
    template <typename... Types>
    Files(string name, Types*... dataMembers)
    {
       files.push_back(name+".txt");
        (members.push_back(dataMembers), ...);

    }

    void display()
    {
        for (const auto& member: members)
        {
            cout << *member << endl;
        }
    }
    ~Files(){};
};
