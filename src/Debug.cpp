#include "AIForce.h"

void AIForce::printShape()
{
    for(int i=0;i<13;i++)
    {
        cout<<"DEBUG BLACK\t"<<typeName[i]<<"\t"<<bShape.sp[0].patterns[i]<<endl;
        cout<<"DEBUG WHITE\t"<<typeName[i]<<"\t"<<bShape.sp[1].patterns[i]<<endl;
    }
    cout<<"DEBUG hitRate:"<<transtable.get_hit_rate()<<endl;
}
void AIForce::printBoardStone()
{
    for(int i=0;i<sz;i++)
    {
        for(int j=0;j<sz;j++)
        {
            cout<<"DEBUG "<<i<<"\t"<<j<<endl;
            for(int k=0;k<4;k++)
            {
                cout<<"DEBUG  \t"<<typeName[board[i+4][j+4].chesstype[k]]<<endl;
            }
            cout<<endl;
        }
    }
}



void AIForce::perform_display()
{
    ofstream fout("F:\\piskvork(1)\\performance.txt",ios::app);
    fout<<"SEARCH DEPTH  :\t"<<MAX_SEARCH_DEPTH<<endl;
    fout<<"TRANSTABLE HIT:\t"<<transtable.get_hit_rate()<<endl;
    fout<<"EACH STEP INFO:\n"<<endl;
    fout<<"POS\t\tTIME\tNODES\tVALUE"<<endl;
    for(vector<info>::iterator i=step_info.begin();i!=step_info.end();i++)
    {
        fout<<(*i).x<<( (*i).x>9?(","):(" ,") )<<(*i).y<<"\t";
        fout<<(*i).time<<"\t";
        fout<<(*i).nodes<<"\t";
        fout<<(*i).val<<endl;
    }
    for(vector<info>::iterator i=step_info.begin();i!=step_info.end();i++)
    {
        fout<<(*i).nodes/(*i).time<<endl;
    }
    fout<<"ENTRY COUNT:\t"<<transtable.get_entry_count()<<endl;
    fout<<"CRACK COUNT:\t"<<transtable.get_crash_count()<<endl;



}
