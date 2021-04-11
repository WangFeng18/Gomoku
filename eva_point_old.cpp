//老版本的空点评估，依据是字符串匹配
int AIForce::eva_point(int i,int j,int k,oxtype type)
{
    i=i+4;j=j+4;
    move(i,j,type);
    oxtype op=OPPO(type);
    int si=i-4*di[k],sj=j-4*dj[k];
    int ei=i+5*di[k],ej=j+5*dj[k];
    int num;
    for(num=0;num<TYPENUM;num++)
    {
        int x=si,y=sj;
        int p=0;
        char* patt;
        patt=TypeTable[num];
        while(!(x==ei&&y==ej))
        {

            if((patt[p]=='_'&&board[x][y].type==EMPTY)||(patt[p]=='X'&&(board[x][y].type==op||board[x][y].type==WRONG))
             ||(patt[p]=='O'&&board[x][y].type==type))
            {
                p++;
                x+=di[k];
                y+=dj[k];
            }
            else
            {
                x-=(p-1)*di[k];
                y-=(p-1)*dj[k];
                p=0;
            }
            if(patt[p]==0)
            {
                break;
            }
        }
        if(patt[p]==0)
            break;
    }

    unmove(i,j);
    if(num==TYPENUM)
    {
        return 0;
    }
    else
    {
        if(num==0)
            return FIVE;
        else if(num<3)
            return FOUR;
        else if(num<8)
            return SFOUR;
        else if(num<11)
            return THREE;
        else if(num<13)
            return THREE2;
        else if(num<23)
            return STHREE;
        else if(num<31)
            return TWO;
        else if(num<36)
            return TWO2;
        else if(num<37)
            return TWO3;
        else if(num<58)
            return STWO;
        else
            return ONE;
    }
}