#include "AIForce.h"
int di[4]={-1,-1,-1,0};
int dj[4]={-1,0,1,1};
int typescore[2][13]={
//     five  four   sfour   sfour2  three   three2    sthree    two   two2   two3  stwo   one
    {0,100000,8000, 1050,   1005,   1000,   900,      250,      200,  180,   160,  100,   0},
    {0,50000, 4000, 525,    502,    500,    450,      125,      100,  90,    80,   50,    0}
};
int typechessnum[14]={1,5,4,4,4,3,3,3,2,2,2,2,1,1};
int scoreconfig[14]={
            0,
            0,//VICTORY,
            110,
            105,
            100,
            100,
            100,
            25,
            20,
            20,
            20,
            2,
            1,
            0};
AIForce::AIForce(){stepTime=STEP_TIME;}
AIForce::~AIForce(){}
void AIForce::init(int sz,bool AIfirst)
{
    srand((unsigned int)(time(NULL)));
    this->sz=sz;
    board=new stone*[sz+8];
    for(int i=0;i<sz+8;i++)
        board[i]=new stone[sz+8];
    setFirst(AIfirst);
    clear();
}
void AIForce::setFirst(bool AIfirst)
{
    my=AIfirst?BLACK:WHITE;
}
#define five(who) (bShape.sp[who].patterns[FIVE])
#define four(who) (bShape.sp[who].patterns[FOUR])
#define sfour(who) (bShape.sp[who].patterns[SFOUR]+bShape.sp[who].patterns[SFOUR2])
#define three(who) (bShape.sp[who].patterns[THREE]+bShape.sp[who].patterns[THREE2])
#define sthree(who) (bShape.sp[who].patterns[STHREE])
#define two(who) (bShape.sp[who].patterns[TWO]+bShape.sp[who].patterns[TWO2]+bShape.sp[who].patterns[TWO3])
#define stwo(who) (bShape.sp[who].patterns[STWO])

int AIForce::evaluate(oxtype type)
{

    oxtype op=OPPO(type);

    if(five(type))
        return VICTORY;
    if(five(op))
        return -VICTORY;



    if(four(type)||sfour(type))
        return VICTORY-1000;

    if(four(op)||sfour(op)>=2)
        return -VICTORY+1000;



    //四三不一定必胜，如果对方在防守眠四的时候形成了反四三，反而是必败点，必须延伸搜索
    if(sfour(type)&&three(type))
    {
        return VICTORY-6000;

    }
    if(sfour(op)&&three(op))
    {
        return -VICTORY+6000;
    }


    if(three(type)&&sfour(op)==0)
        return VICTORY-2000;

    if(three(op)>=2&&three(type)==0&&sthree(type)==0)
        return -VICTORY+3000;


    /*ordinaryevaluate_2*///int res=2*bShape.sp[type].getscore()-bShape.sp[op].getscore();
    int res=BIAS_FACTOR*bShape.sp[type].getscore()-bShape.sp[op].getscore();

    if(three(type)>=2)
        res+=1800;
    if(three(op)>=2)
        res-=400;
    return res;
}

int AIForce::eva_point(int i,int j,int k,oxtype type)
{
    int pat[2];
    pat[WHITE]=board[i][j].pattern[k][WHITE];
    pat[BLACK]=board[i][j].pattern[k][BLACK];
    int mask=pat[WHITE]&pat[BLACK];
    pat[type]^=mask;
    return config[type][pat[BLACK]][pat[WHITE]];
}



#define FOR_EVERY_AVAILABLE for(int i=upleft.i;i<=downright.i;i++)               \
        for(int j=upleft.j;j<=downright.j;j++)                                   \
            if(board[i+4][j+4].type==EMPTY&&board[i+4][j+4].adj>0)

int AIForce::generateMoves(int depth,oxtype who)
{

    candMove[depth].clear();
    int rank=0;
    if(four(OPPO(who))>0 )
        rank=FOUR;
    else if(sfour(OPPO(who))>0)
        rank=FOUR;
    else if(three(OPPO(who))>0)
        rank=THREE;
    if(rank!=0)
    {
        int mytype,optype;
        FOR_EVERY_AVAILABLE
        {
            bool push=false;
            int val=0;
            for(int k=0;k<4;k++)
            {
                mytype=eva_point(i+4,j+4,k,who);
                optype=eva_point(i+4,j+4,k,OPPO(who));
                if(mytype>0&&mytype<rank)
                    push=true;
                if(optype>0&&optype<rank)
                    push=true;
                val+=typescore[0][eva_point(i+4,j+4,k,who)]+typescore[1][eva_point(i+4,j+4,k,OPPO(who))];
            }
            if(push)
                candMove[depth].push_back(Point(i,j,val) );
        }
        sort(candMove[depth].begin(),candMove[depth].end());
        return candMove[depth].size();
    }



    FOR_EVERY_AVAILABLE
    {
        int val=0;
        for(int k=0;k<4;k++)
            val+=typescore[0][eva_point(i+4,j+4,k,who)]+typescore[1][eva_point(i+4,j+4,k,OPPO(who))];
        candMove[depth].push_back(Point(i,j,val) );
    }

    sort(candMove[depth].begin(),candMove[depth].end());
    return candMove[depth].size();
}

int AIForce::generateKillMove(int depth,oxtype who)
{
/*
    killMove[depth].clear();

    int rank=100;
    if(four(OPPO(who))>0 )
        rank=FOUR;
    else if(sfour(OPPO(who))>0)
        rank=FOUR;
    else if(three(OPPO(who))>0)
        rank=THREE;


    for(int i=0;i<sz;i++)
        for(int j=0;j<sz;j++)
            if(board[i+4][j+4].type==EMPTY&&VALID(i+4,j+4))
            {
                int maxval1=0,maxval2=0,push1=false,push2=false;
                for(int k=0;k<4;k++)
                {
                    int type=eva_point(i+4,j+4,k,who);
                    int val=typescore[0][type];
                    if(type<rank&&type>0)
                        switch(type)
                        {
                            case FIVE:
                            case FOUR:
                            case SFOUR:case SFOUR2:
                            case THREE:case THREE2:
                                push1=true;
                                maxval1=max(val,maxval1);
                                break;
                        }

                    type=eva_point(i+4,j+4,k,OPPO(who));
                    val=typescore[1][type];
                    switch(type)
                    {
                        case FIVE:
                        case FOUR:
                            push2=true;
                            maxval2=max(val,maxval2);
                            break;
                    }
                }
                if(push1&&push2)
                    killMove[depth].push_back(Point(i,j,maxval1+maxval2) );
                else if(push1)
                    killMove[depth].push_back( Point(i,j,maxval1) );
                else if(push2)
                    killMove[depth].push_back( Point(i,j,maxval2) );
            }
    sort(killMove[depth].begin(),killMove[depth].end());
    return killMove[depth].size();*/
}



int AIForce::absearch(int d,int a,int b,bool who)
{
    nodes_every_step++;

    if(bShape.sp[!who].patterns[FIVE])
        return -VICTORY;

    int score;
    if(d<=0)
    {
        return evaluate(oxtype(who));
    }

    int entry=0;
    int Count=generateMoves(d,oxtype(who));



    if(Count==1&&d==max_depth)
    {
        besti=candMove[d][0].i,bestj=candMove[d][0].j;
        return 0;
    }
#ifdef BRANCHLIMIT
    if(Count>BRANCH)
        Count=BRANCH;
#endif // BRANCHLIMIT

    if(Count!=0)
    {
        move(candMove[d][0].i+4,candMove[d][0].j+4,who);

#ifdef USE_TRANSPORT
        if(!transtable.search(score,d-1,a,b))
        {
            entry=1;
            score=-absearch(d-1,-b,-a,!who);
        }
#endif // USE_TRANSPORT


#ifndef USE_TRANSPORT
        score=-absearch(d-1,-b,-a,!who);
#endif // USE_TRANSPORT


        if(score>=b)
        {
            if(entry)
                transtable.entry(b,d-1,LOWER);
            unmove(candMove[d][0].i+4,candMove[d][0].j+4);
            return b;
        }
        if(score>a)
        {
            a=score;
            bestpath[d]=Point(candMove[d][0].i,candMove[d][0].j,score);
            if(d==max_depth)besti=candMove[d][0].i,bestj=candMove[d][0].j;
            if(entry)
                transtable.entry(a,d-1,EXACT);
        }
        else
            if(entry)
                transtable.entry(a,d-1,UPPER);


        unmove(candMove[d][0].i+4,candMove[d][0].j+4);
    }




    for(int i=1;i<Count;i++)
    {
        entry=0;
        move(candMove[d][i].i+4,candMove[d][i].j+4,who);


#ifdef USE_TRANSPORT
        if(!transtable.search(score,d-1,a,b))
        {
            entry=1;
            score=-absearch(d-1,-a-1,-a,!who);
            if(score>a&&score<b)
                score=-absearch(d-1,-b,-a-1,!who);

        }
#endif // USE_TRANSPORT

#ifndef USE_TRANSPORT
        score=-absearch(d-1,-a-1,-a,!who);
        if(score>a&&score<b)
            score=-absearch(d-1,-b,-a-1,!who);
#endif // USE_TRANSPORT


        if(score>=b)
        {
            if(entry)
                transtable.entry(b,d-1,LOWER);

            unmove(candMove[d][i].i+4,candMove[d][i].j+4);
            return b;
        }
        if(score>a)
        {
            a=score;
            bestpath[d]=Point(candMove[d][i].i,candMove[d][i].j,score);
            if(d==max_depth)besti=candMove[d][i].i,bestj=candMove[d][i].j;
            if(entry)
                transtable.entry(a,d-1,EXACT);
        }
        else
            if(entry)
                transtable.entry(a,d-1,UPPER);

        unmove(candMove[d][i].i+4,candMove[d][i].j+4);
    }

    return a;
}



bool AIForce::vct(int d,bool attack)
{
    if(!attack&&five(my))
        return false;
    if(attack&&five(OPPO(my)))
        return false;
    if(d==0)
        return false;
    int Count=generateKillMove(d,attack?my:OPPO(my));
    if(attack&&Count==0)
        return false;
    if(!attack&&Count==0)
        return true;
    for(int i=0;i<Count;i++)
    {
        move(killMove[d][i].i+4,killMove[d][i].j+4,attack?my:OPPO(my));

        if(!vct(d-1,!attack))
        {
            if(d==max_vctdepth)
                vctbesti=killMove[d][i].i,vctbestj=killMove[d][i].j;
            unmove(killMove[d][i].i+4,killMove[d][i].j+4);
            return true;
        }
        unmove(killMove[d][i].i+4,killMove[d][i].j+4);
    }
    return false;
}


