#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define STEP_TIME 2
#define MAX_SEARCH_DEPTH 29
#define BRANCH 20
#define STEP 1

#define BIAS_FACTOR 2

#define DEPTH_BEGIN 1//(2-MAX_SEARCH_DEPTH%2)


#define FIRST_STEP_RANDOM
#define USE_TRANSPORT
#define PERFORMDISPLAY
#define BRANCHLIMIT

#define HASH_TYPE1
#endif // CONFIG_H_INCLUDED
/*
    改进方案
    1.估值改进
    2.非固定分支
*/
