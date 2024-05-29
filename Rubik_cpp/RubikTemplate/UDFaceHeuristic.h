/*  Heuristic Base Class definition
	Designed to be as generic as possible.  Combined with the appropriate search algorithm classes,
    this represents a heuristic estimate of the current node's distance from the Goal.
    By default, it returns 0.0 so that it is ready to use as a non-heuristic.
    For other heuristics, simply redefine ComputeHeuristic in the derived class.
*/

#ifndef UDFACEHEURISTIC_2012OCT09_RKD
#define UDFACEHEURISTIC_2012OCT09_RKD

#include <map>
#include <sstream>
#include <string>
#include <fstream>

#include "Heuristic.h"

class UDFaceHeuristic : public Heuristic
{
    private:
        std::map<unsigned int, int> UpDownFaceHeuristic;

    public:

        float ComputeHeuristic(ISearchableNode *node, ISearchableNode *Goal)
        {
            int ID;
            std::istringstream( node->GetUID() ) >> ID;
            return UpDownFaceHeuristic[ID];
        };

        UDFaceHeuristic()
        {
            std::fstream HFile;
            HFile.open("..\\UpDownFace.heu", std::ios::in );
            unsigned int ID;
            int moves, pos;
            std::string incomingLine;

            while( !HFile.eof() )
            {
                getline(HFile, incomingLine);
                pos = incomingLine.find(",");
                std::istringstream( (incomingLine.substr(0,pos)).c_str() ) >> ID;
                std::istringstream( (incomingLine.substr(pos+1)).c_str() ) >> moves;
                UpDownFaceHeuristic[ID] = moves;
            }

            HFile.close();
        };

        ~UDFaceHeuristic() {};
};


#endif

