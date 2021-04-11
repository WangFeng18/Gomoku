#include "exString.h"

exString::exString(string s){this->s=s;}
exString::~exString(){}

vector<string> exString::split(string token)
{
    vector<string> ret;
    int begin=0;
    string s=this->s;
    s.append(1,token[0]);
    for(int i=0;i<s.length();i++)
    {
        for(int j=0;j<token.length();j++)
        {
            if(s[i]==token[j])
            {
                if(begin!=i)
                    ret.push_back(this->substring(begin,i));
                begin=i+1;
                break;
            }
        }
    }
    return ret;

}
string exString::substring(int b,int e)
{
    string ret(e-b,'a');
    for(int i=b;i<e;i++)
        ret[i-b]=s[i];
    return ret;
}
