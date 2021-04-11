#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <fstream>
#include "BoardHash.h"
#include "config.h"
#include "Statistic.h"
using namespace std;
#ifndef AIFORCE_H
#define AIFORCE_H
typedef int pat;
typedef char oxtype;
typedef unsigned char UC;
#define WHITE 1
#define BLACK 0
#define EMPTY -1
#define WRONG 2
#define OPPO(x) (1-x)

#define TYPENUM 62
#define NOTYPE 0
#define FIVE 1
#define FOUR 2
#define SFOUR 3
#define SFOUR2 4
#define THREE 5
#define THREE2 6
#define STHREE 7
#define TWO 8
#define TWO2 9
#define TWO3 10
#define STWO 11
#define ONE 12
#define SONE 13

#define VICTORY 2147483000
#define WIN bShape.sp[0].patterns[FIVE]||bShape.sp[1].patterns[FIVE]
extern char typeName[13][10];
extern char TypeTable[TYPENUM][15];
extern int config[2][256][256];
extern int di[4];
extern int dj[4];
extern int typescore[2][13];
extern int typechessnum[14];
extern int scoreconfig[14];

class AIForce
{
    public:
        AIForce();
        virtual ~AIForce();
        void init(int sz,bool AIfirst);
        void setFirst(bool AIfirst);
        void setStepTime(int milliseconds){stepTime=milliseconds;}
        int isWin();
        int evaluate(oxtype type);
        int get_pat(int i,int j,int k,oxtype);
        int eva_point(int i,int j,int k,oxtype type);
        void getMoveSimple(int &i,int &j);
        void getMove(int &i,int &j);


        void turn(int i,int j,oxtype type=100);
        bool move(int i,int j,oxtype type);
        void unmove(int i,int j);
        void updateShape(int i,int j,oxtype type);
        void printBoard();
        void Begin(int& i,int& j);
        void clear();

        oxtype getmy(){return my;}

        void generateTable();
        void printTable();
        struct stone{
            oxtype type;
            pat pattern[4][2];
            UC chesstype[4];
            char adj;
        };
        stone** board;


        /*************************************************************************/
        //DEBUG
        void printShape();
        void perform_display();
        void printBoardStone();
        /*************************************************************************/


        int movecount;
        oxtype my;
    protected:
    private:
        struct Point{
            int i,j;
            int val;
            Point(){}
            Point(int a,int b,int c):i(a),j(b),val(c){}
            bool operator<(const Point &p)const
            {
                return val>p.val;
            }
        };
        struct shape{
            int patterns[14];

            void clear()
            {
                memset(patterns,0,sizeof(patterns));

            }

            int getscore()
            {
                int score=0;
                for(int i=1;i<14;i++)
                    score+=patterns[i]*scoreconfig[i];
                return score;
            }

        };
        struct BWshape
        {
            shape sp[2];
            int getscore()
            {
                return sp[BLACK].getscore()-sp[WHITE].getscore();
            }
            void clear()
            {
                sp[BLACK].clear();
                sp[WHITE].clear();
            }
            void add(int color,int shape)
            {
                ++sp[color].patterns[shape];
            }
            void dec(int color,int shape)
            {
                --sp[color].patterns[shape];
            }

        };



        //棋形相关
        BWshape bShape;
        vector<BWshape> shape_vec;
        vector<Point> upleft_vec;
        vector<Point> downright_vec;
        Point upleft;
        Point downright;

        //着法生成
        vector<Point> candMove[100];
        //hash表
        BoardHash transtable;

        //统计
        Statistic timer;
        struct info{
            double time;
            int nodes;
            int x;
            int y;
            int val;
        };
        int nodes_every_step;
        vector<info> step_info;




        int generateMoves(int depth,oxtype who);
        int absearch(int d,int a,int b,bool who);

        vector<Point> killMove[100];
        int generateKillMove(int depth,oxtype who);
        bool vct(int d,bool attack);

        int besti,bestj;
        int vctbesti,vctbestj;
        Point bestpath[100];

        int sz;
        int win;    //-1为无判断胜负，0为黑胜，1为白胜




        int firsti,firstj;


        //search relevant
        int max_depth;
        int max_vctdepth;
        int stepTime;

};

#endif // AIFORCE_H
