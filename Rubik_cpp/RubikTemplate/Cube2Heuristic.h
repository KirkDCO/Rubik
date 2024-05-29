/**
    @author Robert Kirk DeLisle rkdelisle@gmail.com
    @version 0.1

    @section LICENSE

    @section DESCRIPTION

    Heuristic Template Class example
    For use with GraphSearch Template Class
    Provides a heuristic for the 2x2x2 Rubik's Cube

    Usage:  GraphSearch<Searchable>
    Searchable class:
        Any class intended to serve as a node within a graph for the search and for which a heuristic is calculated.
        Must implement:
            ComputeH( Searchable & ) - returns a heuristic estimate of distance to the goal from the passed Searchable node
*/


#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#ifndef CUBE2HEURISTIC_2012OCT09_RKD
#define CUBE2HEURISTIC_2012OCT09_RKD

template<class Searchable>
class Cube2Heuristic
{
    public:
        /** For this example, two possible heuristics are available:
            DIJKSTRA returns 0.0 as Dijkstra search uses no heuristic
            UPDOWNFACE returns the max of the number of turn necessary to properly position (NOT orient) the top corners or the bottom corners.
        */
        enum Heuristics
        {
            DIJKSTRA,
            UPDOWNFACE
        };

        /** Compute the heuristic value
            \param *node - Pointer to the node of interest.
            \return the computed value
        */
        float ComputeH(Searchable *node);

        /** For this example, I have set up the ability to pre-select which heuristic you want to use.
            \param  h - Value from the Cube2Heuristic::Heuristics enumeration
        */
        void SetHeuristic( Heuristics h ) { m_SelectedHeuristic = h; };

        /** Default constructor.  Initialize any heuristics and set a default one to be used. */
        Cube2Heuristic()
        {
            InitUpDownFace();
            m_SelectedHeuristic = UPDOWNFACE;
        };

        /** Default destructor. */
        ~Cube2Heuristic() {};

    private:

        /** Holds the pre-computed heuristic.
            \param .first - A UID for the 2x2x2 Cube
            \param .second - The maximum of the number of turns to properly position (NOT orient) the upper corners or the lower corners.
        */
        std::map<unsigned int, int> UpDownFaceHeuristic;

        /**  Initialize the heuristic */
        void InitUpDownFace();

        /** Retrieve the precomputed UPDONWFACEHEURISTIC - used by ComputeH()
            \param *node - pointer to the node/2x2x2 cube of interest
        */
        float GetUpDownFaceHeuristic(Searchable *node);

        Heuristics m_SelectedHeuristic;
};

template<class Searchable>
float Cube2Heuristic<Searchable>::GetUpDownFaceHeuristic(Searchable *node)
{
    return std::max( UpDownFaceHeuristic[node->GetTopID()], UpDownFaceHeuristic[node->GetBottomID()] );
};

template<class Searchable>
float Cube2Heuristic<Searchable>::ComputeH(Searchable *node)
{
    switch ( m_SelectedHeuristic )
    {
        case DIJKSTRA:
            return 0.0;

        case UPDOWNFACE:
            return GetUpDownFaceHeuristic(node);

        default:
            return 0.0;
    }
}

template<class Searchable>
void Cube2Heuristic<Searchable>::InitUpDownFace()
{
    std::fstream HFile;
    std::string incomingLine;
    int pos, moves;
    unsigned int ID;

    HFile.open("UpDownFace.heu", std::ios::in);

    while ( !HFile.eof() )
    {
        std::getline(HFile, incomingLine);
        pos = incomingLine.find(",");

        std::istringstream( incomingLine.substr(0,pos) ) >> ID;
        std::istringstream( incomingLine.substr(pos+1) ) >> moves;

        UpDownFaceHeuristic[ID] = moves;
    }

    HFile.close();
};

#endif
