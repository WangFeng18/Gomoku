#ifndef STATISTIC_H
#define STATISTIC_H
#include <time.h>

class Statistic
{
    public:
        Statistic(){}
        virtual ~Statistic(){}
        void tstart(){start=clock();}
        void tend(){finish=clock();}
        double get_time(){return (finish-start)/CLOCKS_PER_SEC;}

    private:
        double start,finish;
};

#endif // STATISTIC_H
