#include "Game.h"
#include "exString.h"
Game::Game(){}
Game::~Game(){}
#define CASE(x,y) if(cmds[0]==x||cmds[0]==y)
void Game::command()
{
    string cmd;
    string token=" ,";
    vector<string> cmds;
    while(true)
    {
        getline(cin,cmd);
        exString excmd(cmd);
        cmds=excmd.split(token);
        CASE("start","START")
        {
            start( atoi(cmds[1].c_str() ));
            continue;
        }
        CASE("turn","TURN")
        {
            turn(atoi(cmds[1].c_str() ),atoi(cmds[2].c_str() ));
            continue;
        }
        CASE("begin","BEGIN")
        {
            Begin();
            continue;
        }
        CASE("restart","RESTART")
        {
            restart();
            continue;
        }
        CASE("board","BOARD")
        {
            while(true)
            {
                getline(cin,cmd);
                exString excmdchild(cmd);
                cmds=excmdchild.split(token);
                CASE("done","DONE")
                {

                    Begin();
                    break;
                }
                if(cmds[2]=="1")
                {
                    if(ai.movecount==0)
                        ai.setFirst(true);
                    ai.turn(atoi(cmds[1].c_str() ),atoi(cmds[0].c_str() ),ai.my);
                }
                else
                {
                    if(ai.movecount==0)
                        ai.setFirst(false);
                    ai.turn(atoi(cmds[1].c_str() ),atoi(cmds[0].c_str() ),1-ai.my);
                }

            }
            continue;
        }
        CASE("takeback","TAKEBACK")
        {
            ai.unmove(atoi(cmds[2].c_str() )+4,atoi(cmds[1].c_str() )+4);
            cout<<"OK"<<endl;
        }
        CASE("info","INFO")
        {
            if(cmds[1]=="timeout_turn")
            {

            }

        }

    }
}
void Game::start(int sz)
{

    if(sz<=0||sz>30)
    {
        cout<<"ERROR"<<endl;
        return;
    }
    ai.init(sz,false);
    cout<<"OK"<<endl;
}
void Game::restart()
{
    cout<<"OK"<<endl;
    ai.clear();
    ai.setFirst(false);

}
void Game::turn(int x,int y)
{
    int i,j;
    ai.turn(y,x);
    ai.getMove(i,j);
    cout<<j<<","<<i<<endl;


}
void Game::Begin()
{
    int i,j;
    if(ai.movecount==0)
        ai.setFirst(true);
    ai.Begin(i,j);
    cout<<j<<","<<i<<endl;

}
