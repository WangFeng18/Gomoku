#include <iostream>
#include "AIForce.h"

#ifndef VSPLATFORM_H
#define VSPLATFORM_H
using namespace std;

class vsplatform
{
    public:
        vsplatform();
        virtual ~vsplatform();
        void start(int sz,int num);
    protected:
    private:
        //ai1”¿‘∂œ» ÷
        AIForce ai1,ai2;
        int win1,win2;

};

#endif // VSPLATFORM_H
