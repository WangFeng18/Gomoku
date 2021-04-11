#ifndef EXSTRING_H
#define EXSTRING_H
#include <string>
#include <vector>
using namespace std;

class exString
{
    public:
        exString(string s);
        virtual ~exString();
        vector<string> split(string token);
        string substring(int b,int e);
    protected:
    private:
        string s;
};

#endif // EXSTRING_H
