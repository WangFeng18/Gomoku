#include "vsplatform.h"

vsplatform::vsplatform(){}
vsplatform::~vsplatform(){}


void vsplatform::start(int sz,int num)
{
    int m,n;
    win1=0;win2=0;
    ai1.init(sz,false);
    ai2.init(sz,false);
    //ai1.generateTable();
    //ai2.generateTable();
    for(int i=0;i<num;i++)
    {
        ai1.clear();
        ai2.clear();
        ai1.setFirst(true);
        ai2.setFirst(false);

        ai1.Begin(m,n);
        ai2.turn(m,n);
        cout<<"ai1:"<<m<<"\t"<<n<<endl;

        ai2.getMove(m,n);
        ai1.turn(m,n);
        cout<<"ai2:"<<m<<"\t"<<n<<endl;
        cout<<endl;

        while(ai1.isWin()==0)
        {
            ai1.getMove(m,n);
            ai2.turn(m,n);
            cout<<"ai1:"<<m<<"\t"<<n<<endl;

            ai2.getMove(m,n);
            ai1.turn(m,n);
            cout<<"ai2:"<<m<<"\t"<<n<<endl;
            cout<<endl;
        }

        if(ai1.isWin()==1)win1++;else win2++;
    }
    cout<<"ai1:ai2"<<endl<<win1<<":"<<win2<<endl;
}
