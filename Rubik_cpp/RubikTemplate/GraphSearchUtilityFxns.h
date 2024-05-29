#ifndef GRAPHSEARCHUTILFXN_29Oct12_RKD
#define GRAPHSEARCHUTILFXN_29Oct12_RKD

#include "Cube2.h"
//#include "Cube3.h"

/** Provides comparison for sorting to maintain Priority Queue in GraphSearch */
bool NodeSort(Cube2 *left, Cube2 *right)
{
    return left->F() > right->F();
};
/*
bool NodeSort(Cube3 *left, Cube3 *right)
{
    return left->F() > right->F();
}
*/
/** Provides a goal test for GraphSearch */
bool IsGoal(Cube2 *c)
{
    if ( c->CheckFace(UP) == 0 and c->CheckFace(DOWN) == 0 )
        return true;
    else
        return false;
};

#endif
