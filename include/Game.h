#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#endif

#ifndef GAME_H
#define GAME_H
#include "AIForce.h"
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
using namespace std;
class Game
{
    public:
        Game();
        virtual ~Game();
        void start(int sz);
        void restart();
        void turn(int x,int y);
        void Begin();
        void info(char* key,char* value);
        void command();
    protected:
    private:
        AIForce ai;

};

#endif // GAME_H
