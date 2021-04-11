#include "AIForce.h"

void AIForce::Begin(int& i,int& j)
{
    if(movecount==0)
    {
        i=sz/2,j=sz/2;
        move(sz/2+4,sz/2+4,my);
        movecount++;
    }
    else
    {
        getMove(i,j);
    }


}

void AIForce::turn(int i,int j,oxtype type)
{
    if(type==100)
        type=OPPO(my);
    if(movecount==0)
        firsti=i,firstj=j;
    move(i+4,j+4,type);
    movecount++;
#ifdef PERFORMDISPLAY
    if(WIN)perform_display();
#endif // PERFORMDISPLAY
    if(type==my)
        step_info.push_back({0.0,0,j,i,0});
}

bool AIForce::move(int i,int j,oxtype type)
{

    transtable.move(i-4,j-4,type);

    board[i][j].type=type;

    updateShape(i,j,type);
    shape_vec.push_back(bShape);

    board[i-2][j-2].adj++;board[i-2][j-1].adj++;board[i-2][j].adj++;board[i-2][j+1].adj++;
    board[i-2][j+2].adj++;board[i-1][j-2].adj++;board[i-1][j-1].adj++;board[i-1][j].adj++;
    board[i-1][j+1].adj++;board[i-1][j+2].adj++;board[i][j-2].adj++;board[i][j-1].adj++;
    board[i][j+1].adj++;board[i][j+2].adj++;board[i+1][j-2].adj++;board[i+1][j-1].adj++;
    board[i+1][j].adj++;board[i+1][j+1].adj++;board[i+1][j+2].adj++;board[i+2][j-2].adj++;
    board[i+2][j-1].adj++;board[i+2][j].adj++;board[i+2][j+1].adj++;board[i+2][j+2].adj++;


    if((i-4)-2<upleft.i)upleft.i=max(i-6,0);
    if((j-4)-2<upleft.j)upleft.j=max(j-6,0);
    if((i-4)+2>downright.i)downright.i=min(i-2,sz-1);
    if((j-4)+2>downright.j)downright.j=min(j-2,sz-1);
    upleft_vec.push_back(upleft);
    downright_vec.push_back(downright);

}



void AIForce::unmove(int i,int j)
{
    UC x,y;

    for(UC k=0;k<4;k++)
    {
        board[i][j].chesstype[k]=NOTYPE;
        x=i,y=j;
        for(UC l=0;l<4;l++)
        {
            x-=di[k],y-=dj[k];

            board[ x ][ y ].pattern[k][board[i][j].type ] ^= 1<<(3-l);
            if(board[x][y].type!=EMPTY&&board[x][y].type!=WRONG)
                board[x][y].chesstype[k]=eva_point(x,y,k,board[x][y].type);
        }
        x=i,y=j;
        for(UC r=0;r<4;r++)
        {
            x+=di[k],y+=dj[k];
            board[ x ][ y ].pattern[k][board[i][j].type ] ^= 1<<(4+r);
            if(board[x][y].type!=EMPTY&&board[x][y].type!=WRONG)
                board[x][y].chesstype[k]=eva_point(x,y,k,board[x][y].type);
        }
    }

    transtable.unmove(i-4,j-4,board[i][j].type);

    board[i][j].type=EMPTY;

    shape_vec.pop_back();
    bShape=shape_vec.back();
    upleft_vec.pop_back();
    upleft=upleft_vec.back();
    downright_vec.pop_back();
    downright=downright_vec.back();

    board[i-2][j-2].adj--;board[i-2][j-1].adj--;board[i-2][j].adj--;board[i-2][j+1].adj--;
    board[i-2][j+2].adj--;board[i-1][j-2].adj--;board[i-1][j-1].adj--;board[i-1][j].adj--;
    board[i-1][j+1].adj--;board[i-1][j+2].adj--;board[i][j-2].adj--;board[i][j-1].adj--;
    board[i][j+1].adj--;board[i][j+2].adj--;board[i+1][j-2].adj--;board[i+1][j-1].adj--;
    board[i+1][j].adj--;board[i+1][j+1].adj--;board[i+1][j+2].adj--;board[i+2][j-2].adj--;
    board[i+2][j-1].adj--;board[i+2][j].adj--;board[i+2][j+1].adj--;board[i+2][j+2].adj--;
}

//移动时更新
void AIForce::updateShape(int i,int j,oxtype type)
{
    UC typenum[14];
    UC ntypenum[14];
    UC curtype;
    memset(typenum,0,sizeof(typenum));
    memset(ntypenum,0,sizeof(ntypenum));
    UC x,y;
    for(UC k=0;k<4;k++)
    {
            x=i,y=j;
            curtype=eva_point(x,y,k,board[x][y].type);
            ++ntypenum[curtype];
            if(ntypenum[curtype]%typechessnum[curtype]==0)
                bShape.add(board[x][y].type,curtype);

            board[x][y].chesstype[k]=curtype;
            for(UC l=0;l<4;l++)
            {
                x-=di[k],y-=dj[k];
                board[ x ][ y ].pattern[k][type] |= 1<<(3-l);
                if(board[x][y].type!=WRONG&&board[x][y].type!=EMPTY)
                {
                    curtype=eva_point(x,y,k,board[x][y].type);
                    if(curtype!=board[x][y].chesstype[k])
                    {
                        ++typenum[ board[x][y].chesstype[k] ];
                        ++ntypenum[curtype];
                        if(typenum[board[x][y].chesstype[k]]%typechessnum[board[x][y].chesstype[k]]==0)
                            bShape.dec(board[x][y].type,board[x][y].chesstype[k]);
                        if(ntypenum[curtype]%typechessnum[curtype]==0)
                            bShape.add(board[x][y].type,curtype);

                        board[x][y].chesstype[k]=curtype;
                    }
                }
            }
            x=i,y=j;
            for(UC r=0;r<4;r++)
            {
                x+=di[k],y+=dj[k];
                board[ x ][ y ].pattern[k][type] |= 1<<(4+r);
                if(board[x][y].type!=WRONG&&board[x][y].type!=EMPTY)
                {
                    curtype=eva_point(x,y,k,board[x][y].type);
                    if(curtype!=board[x][y].chesstype[k])
                    {
                        ++typenum[ board[x][y].chesstype[k] ];
                        ++ntypenum[curtype];
                        if(typenum[board[x][y].chesstype[k]]%typechessnum[board[x][y].chesstype[k]]==0)
                            bShape.dec(board[x][y].type,board[x][y].chesstype[k]);
                        if(ntypenum[curtype]%typechessnum[curtype]==0)
                            bShape.add(board[x][y].type,curtype);

                        board[x][y].chesstype[k]=curtype;
                    }
                }
            }
    }

}

/*
void AIForce::updateShape(int i,int j,oxtype type)
{
    UC typenum[14];
    UC ntypenum[14];
    UC curtype;
    memset(typenum,0,sizeof(typenum));
    memset(ntypenum,0,sizeof(ntypenum));
    UC x,y;
    for(UC k=0;k<4;k++)
    {
            x=i,y=j;
            for(UC l=0;l<4;l++)
            {
                x-=di[k],y-=dj[k];
                board[ x ][ y ].pattern[k][type] |= 1<<(3-l);
            }
            x=i,y=j;
            for(UC r=0;r<4;r++)
            {
                x+=di[k],y+=dj[k];
                board[ x ][ y ].pattern[k][type] |= 1<<(4+r);
            }
    }

    for(UC k=0;k<4;k++)
    {
        x=i-4*di[k],y=j-4*dj[k];
        for(UC m=0;m<9;m++)
        {
            if(board[x][y].type!=WRONG&&board[x][y].type!=EMPTY)
            {
                curtype=eva_point(x,y,k,board[x][y].type);

                if(curtype!=board[x][y].chesstype[k])
                {
                    ++typenum[ board[x][y].chesstype[k] ];
                    ++ntypenum[curtype];
                    if(typenum[board[x][y].chesstype[k]]%typechessnum[board[x][y].chesstype[k]]==0)
                        bShape.dec(board[x][y].type,board[x][y].chesstype[k]);
                    if(ntypenum[curtype]%typechessnum[curtype]==0)
                        bShape.add(board[x][y].type,curtype);

                    board[x][y].chesstype[k]=curtype;
                }
            }
            x+=di[k],y+=dj[k];
        }
    }

}*/
/*
void AIForce::updateShape(int i,int j,oxtype type)
{
    //添加
    int Oldij[4];
    int Old[4][8];
    int New[4][8];
    oxtype colortype[4][8];
    for(int k=0;k<4;k++)
    {
        //只能升级，不能降级
        Oldij[k]=eva_point(i,j,k,type);
        //bShape.add(type,eva_point(i,j,k,type));
        int x=i-4*di[k],y=j-4*dj[k];
        for(int m=0;m<8;m++)
        {
            colortype[k][m]=board[x][y].type;

            if(board[x][y].type!=EMPTY&&board[x][y].type!=WRONG)
            {
                Old[k][m]=eva_point(x,y,k,board[x][y].type);
            }
            else
            {
                Old[k][m]=0;
            }

            x+=di[k],y+=dj[k];
            if(m==3)
                    x+=di[k],y+=dj[k];
        }
    }




    int x,y;
    //更新图案
    for(int k=0;k<4;k++)
    {
            x=i,y=j;
            for(int l=0;l<4;l++)
            {
                x-=di[k],y-=dj[k];
                board[ x ][ y ].pattern[k][type] |= 1<<(3-l);
            }
            x=i,y=j;
            for(int r=0;r<4;r++)
            {
                x+=di[k],y+=dj[k];
                board[ x ][ y ].pattern[k][type] |= 1<<(4+r);
            }

            x=i-4*di[k],y=j-4*dj[k];
            for(int m=0;m<8;m++)
            {

                if(board[x][y].type!=EMPTY&&board[x][y].type!=WRONG)
                    New[k][m]=eva_point(x,y,k,board[x][y].type);
                else
                    New[k][m]=0;

                x+=di[k],y+=dj[k];
                if(m==3)
                    x+=di[k],y+=dj[k];
            }

    }




    //减少
    for(int k=0;k<4;k++)
    {
        int m;
        for(m=0;m<8;m++)
        {
            if(colortype[k][m]==type&&(Old[k][m]>New[k][m]||(Old[k][m]==NOTYPE&&New[k][m]!=NOTYPE) )  )
            {
                bShape.add(type,Oldij[k]);
                bShape.dec(type,Old[k][m]);
                break;
            }
        }
        if(m==8)
        {
            if(Oldij[k]==ONE||Oldij[k]==SONE)
                bShape.add(type,Oldij[k]);

        }
    }

    //对方棋形的增加减少
    int last_partial=0;
    for(int k=0;k<4;k++)
    {
        for(int m=0;m<4;m++)
        {
            if(colortype[k][m]==OPPO(type)&&Old[k][m]!=New[k][m])
            {
                last_partial=Old[k][m];
                bShape.dec(OPPO(type),Old[k][m]);
                bShape.add(OPPO(type),New[k][m]);
                break;
            }
        }
        for(int m=4;m<8;m++)
        {
            if(colortype[k][m]==OPPO(type)&&Old[k][m]!=New[k][m])
            {
                if(last_partial!=Old[k][m])
                    bShape.dec(OPPO(type),Old[k][m]);
                bShape.add(OPPO(type),New[k][m]);
                break;
            }
        }
    }


}
*/

int AIForce::isWin()
{
    if(bShape.sp[my].patterns[FIVE])
        return 1;
    if(bShape.sp[OPPO(my)].patterns[FIVE])
        return -1;
    return 0;
}


void AIForce::printBoard()
{
    for(int i=0;i<sz+8;i++)
    {
        for(int j=0;j<sz+8;j++)
            std::cout<<board[i][j].type<<"\t";
        std::cout<<std::endl;
    }
}



void AIForce::clear()
{
    movecount=0;


    win=EMPTY;
    max_depth=4;
    max_vctdepth=24;
    upleft.i=sz-1;upleft.j=sz-1;
    downright.i=0;downright.j=0;


    bShape.clear();
    step_info.clear();
    shape_vec.clear();
    upleft_vec.clear();
    downright_vec.clear();
    shape_vec.push_back(bShape);
    upleft_vec.push_back(upleft);
    downright_vec.push_back(downright);

    transtable.clear(sz);
    for(int i=0;i<sz+8;i++)
        for(int j=0;j<sz+8;j++)
        {
            for(int k=0;k<4;k++)
            {
                board[i][j].chesstype[k]=NOTYPE;
                for(int l=0;l<2;l++)
                    board[i][j].pattern[k][l]=0;
            }
            board[i][j].adj=0;
        }
    int x,y;
    for(int i=0;i<sz+8;i++)
    {
        for(int j=0;j<sz+8;j++)
        {
            board[i][j].type=(i<4||j<4||i>sz+3||j>sz+3)?WRONG:EMPTY;
            if(board[i][j].type==WRONG)
            {
                for(int k=0;k<4;k++)
                {
                    x=i,y=j;
                    for(int l=0;l<4;l++)
                    {
                        x-=di[k],y-=dj[k];
                        if(x>=0&&x<sz+8&&y>=0&&y<sz+8)
                        {
                            board[ x ][ y ].pattern[k][WHITE] |= 1<<(3-l);
                            board[ x ][ y ].pattern[k][BLACK] |= 1<<(3-l);
                        }
                    }
                    x=i,y=j;
                    for(int r=0;r<4;r++)
                    {
                        x+=di[k],y+=dj[k];
                        if(x>=0&&x<sz+8&&y>=0&&y<sz+8)
                        {
                            board[ x ][ y ].pattern[k][WHITE] |= 1<<(4+r);
                            board[ x ][ y ].pattern[k][BLACK] |= 1<<(4+r);
                        }
                    }
                }
            }

        }
    }
}




void AIForce::getMoveSimple(int& i,int& j)
{
    if(movecount==1)
    {
        int a=rand()%4;
        switch(a)
        {
        case 0:i=firsti,j=firstj-1;if(j>=0)break;
        case 1:i=firsti-1,j=firstj;if(i>=0)break;
        case 2:i=firsti,j=firstj+1;if(j<sz)break;
        case 3:i=firsti+1,j=firstj;if(i<sz)break;
        }
        move(i+4,j+4,my);
        movecount++;
        return;
    }

    int maxscore=0;
    i=0,j=0;
    for(int x=0;x<sz;x++)
        for(int y=0;y<sz;y++)
            if(board[x+4][y+4].type==EMPTY)
            {
                int score=0;
                int sfm=0,tm=0,sfo=0,to=0;
                for(int k=0;k<4;k++)
                {
                    int smy=eva_point(x+4,y+4,k,my);
                    int soppo=eva_point(x+4,y+4,k,OPPO(my));
                    if(smy==SFOUR)sfm++;
                    if(smy==THREE||smy==THREE2)tm++;
                    if(soppo==SFOUR)sfo++;
                    if(soppo==THREE||soppo==THREE2)to++;

                    score+=1.1*typescore[0][smy];
                    score+=typescore[0][soppo];

                }
                if((sfm&&tm))
                    score+=6000;
                if((sfo&&to))
                    score+=4000;
                if(tm>1)
                    score+=2000;
                if(to>1)
                    score+=1500;
                if(score>maxscore)
                {
                    maxscore=score;
                    i=x,j=y;
                }
            }
    move(i+4,j+4,my);
    movecount++;
}







void AIForce::getMove(int& i,int& j)
{

    if(movecount==1)
    {
        int a=0;

#ifdef FIRST_STEP_RANDOM
        a=rand()%4;
#endif

        switch(a)
        {
        case 0:i=firsti,j=firstj-1;if(j>=0)break;
        case 1:i=firsti-1,j=firstj;if(i>=0)break;
        case 2:i=firsti,j=firstj+1;if(j<sz)break;
        case 3:i=firsti+1,j=firstj;if(i<sz)break;
        }
        move(i+4,j+4,my);
        movecount++;

        step_info.push_back({0.0,0,j,i,0});

        return;
    }


    int score,a=-VICTORY-5,b=VICTORY+5;

    timer.tstart();
    nodes_every_step=0;
    for(int di=DEPTH_BEGIN;di<=MAX_SEARCH_DEPTH;di+=STEP)
    {
        cout<<"DEBUG :"<<di<<endl;
        max_depth=di;
        score=absearch(max_depth,a,b,my);
        if(score>VICTORY-5000||score<-VICTORY+5000)
        {
            max_depth=di+STEP;
            break;
        }
        timer.tend();
        if(stepTime/2<timer.get_time())
            break;
    }
    timer.tend();
    double t=timer.get_time();
    step_info.push_back({t,nodes_every_step,bestj,besti,score});
    cout<<"DEBUG "<<nodes_every_step/t<<endl;

    i=besti,j=bestj;

    move(i+4,j+4,my);
    movecount++;
    //printShape();
    //printBoardStone();
#ifdef PERFORMDISPLAY
    if(WIN)perform_display();
#endif



}
