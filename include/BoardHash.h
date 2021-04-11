#ifndef BOARDHASH_H
#define BOARDHASH_H
#include <iostream>
#include <unordered_map>
#include "config.h"
#define UPPER 0
#define EXACT 1
#define LOWER 2
using namespace std;


struct hashItem{
    __int64 checksum;
    int depth;
    int score;
    char type;
};
extern __int64 zobrist[2][900];
extern __int64 check_zobrist[2][900];

extern hashItem table[1024*1024*16];
extern bool tableset[1024*1024*16];

class BoardHash
{
    public:
        BoardHash();
        virtual ~BoardHash();

        void clear(int _sz){key=0;check_key=0;sz=_sz;hit_search=0;total_search=0;entry_count=0;crash_count=0;cleartable();}//no need to clear the transport table
        void cleartable();
        void move(int i,int j,int who){key^=zobrist[who][i*sz+j];check_key^=check_zobrist[who][i*sz+j];}
        void unmove(int i,int j,int who){key^=zobrist[who][i*sz+j];check_key^=check_zobrist[who][i*sz+j];}

        bool search(int& score,int d,int alpha,int beta);
        void entry(int score,int d,char type);

        double get_hit_rate(){return double(hit_search)/total_search;}
        int get_entry_count(){return entry_count;}
        int get_crash_count(){return crash_count;}
        void check();
    private:
        __int64 key;
        __int64 check_key;


        unordered_map<__int64,hashItem> table2;


        int total_search;
        int hit_search;
        int entry_count;
        int crash_count;

        int sz;

};

#endif // BOARDHASH_H
