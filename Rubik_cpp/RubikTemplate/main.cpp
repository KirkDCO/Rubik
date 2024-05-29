#include <iostream>
#include <time.h>

#include "Cube2.h"
#include "GraphSearch.h"
#include "Cube2Heuristic.h"

using namespace std;

int main()
{
    Cube2 solved;
    Cube2 testcube;
    Cube2 tempcube;

    Cube2Heuristic<Cube2> heuristic;
    heuristic.SetHeuristic(Cube2Heuristic<Cube2>::UPDOWNFACE);

    GraphSearch<Cube2, Cube2Heuristic<Cube2> > GSAlgo;
    srand( time(NULL) );
    std::vector<std::string> sol;
    std::vector<std::string> sol2;

for (int i=0; i<100; i++) {


    testcube.Randomize(100);
    tempcube = testcube;
    GSAlgo.AStar(&testcube, heuristic);
    sol = GSAlgo.GetSolution();
    std::cout << std::endl << std::endl;
    std::cout << sol.size() << std::endl;
    for ( int i = 0; i<sol.size(); i++)
        std::cout << sol.at(i) << std::endl;
    std::cout << std::endl;

//    GSAlgo.IDAStar(&testcube, heuristic, 15);
//    sol2 = GSAlgo.GetSolution();
////    std::cout << std::endl << std::endl;
////    std::cout << sol.size() << std::endl;
////    for ( int i = 0; i<sol.size(); i++)
////        std::cout << sol.at(i) << std::endl;
//
//    if ( sol.size() != sol2.size() )
//        std::cout << "STOP!" << std::endl;
}

//    int minimum=20, maximum=0, average=0;

//
//    for (int c=0; c<100; c++)
//    {
//        testcube.Randomize(100);
//        GSAlgo.AStar( &testcube, heuristic );
//
//        sol = GSAlgo.GetSolution();
//
//        if (sol.size() < minimum)
//            minimum =sol.size();
//
//        if (sol.size() > maximum)
//            maximum = sol.size();
//
//        average += sol.size();
//    }
//    average /= 100;
//
//    std::cout << "Minimum: " << minimum << std::endl;
//    std::cout << "Maximum: " << maximum << std::endl;
//    std::cout << "Average: " << average << std::endl;



//    std::cout << std::endl << std::endl;
//    for (int s=0; s<sol.size(); s++)
//    {
//        std::cout << "Step " << s << " - " << sol.at(s) << std::endl;
//    }

//    heuristic.SetHeuristic(Cube2Heuristic<Cube2>::DIJKSTRA);
//    GSAlgo.AStar( &testclone, heuristic );
//
//    sol = GSAlgo.GetSolution();
//    std::cout << std::endl << std::endl;
//    for (int s=0; s<sol.size(); s++)
//    {
//        std::cout << "Step " << s << " - " << sol.at(s) << std::endl;
//    }

    return 0;
}
