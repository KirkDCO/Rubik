/*  2x2x2 Cube Class definition
	Defines a 2x2x2 Rubik's cube and associated methods
	to allow manipulation of the cube.

	The cube is defined by 8 cubies, their locations, and their rotations.  This requires
	a standard orientation to be defined, so the down-left-front (DLF) cubie is considered to define the locations
	of the others.  As such, this cubie is #1, and in the solved cube, the remainder are numbered as:
	2 - DRF, 3 - DLB, 4 - DRB, 5 - ULF, 6 - URF, 7 - ULB, 8 - URB
*/

#ifndef CUBE2_2007Jun06_RKD
#define CUBE2_2007Jun06_RKD

#include <vector>
#include <sstream>

#include "constants.h"

class Cube2
{
    friend bool NodeSort(Cube2 *left, Cube2 *right);
    friend bool IsGoal(Cube2 *c);

    //From SearchableNode
    private:

        std::string m_GeneratingTransform; //how did we get this node from the parent
        Cube2 *mp_ParentNode;
        float m_CostOfExpansion; //what did it cost to get from the parent to here?

        float m_G; //cost from start to here - expected to be set by the search fxn
        float m_H; //heuristic from here to Goal

    public:

        //standard cost functions
        float G() { return m_G; }; //known cost from start to here - set by the search function
        void G(float value) { m_G = value; }; //setter fxn

        float F() { return m_G + this->H(); }; //estimated total cost

        float H(); //estimated cost to goal - should never overestimate actual cost
        void H(float value) { m_H = value; };

        std::string GetGeneratingTransform() { return m_GeneratingTransform; };
        void SetGeneratingTransform( std::string value ) { m_GeneratingTransform = value; };

        Cube2 *GetParentNode() { return mp_ParentNode; };
        void SetParentNode( Cube2 *p ) { mp_ParentNode = p; };

        float GetCostToExpand() { return m_CostOfExpansion; };
        void SetCostToExpand( float value ) { m_CostOfExpansion = value; };

        void ReplaceNode( Cube2 *c )
        {
            mp_ParentNode = c->mp_ParentNode;
            m_CostOfExpansion = c->m_CostOfExpansion;
        }

    public:

        std::vector<Cube2 *> GetNeighbors();
            //return all the immediate neighbors of this node
            //neighbors should store a pointer to this node so we have a linked list establishing the path
            //neighbors should store the cost of expansion from this node
            //neighbors should store the transform (as a string) that leads to their expansion

        bool operator==( Cube2 &sn );

    //unique to Cube2
    private:

        bool m_state;  //true = solved, false = unsolved
        bool m_oriented; //true = the cube is in the standard orientation based on the DRF cubie
        bool m_CubieTurnsCurrent;
        int m_CubieTurns; //how many turns to get the heuristic

        unsigned int m_ID;
            //a unique ID for each state of the cube.

        std::string m_bitID;

        std::vector<long> m_cubie_positions;
            //a map of the cubie locations in the cube, e.g.,
            //vector.at(0) is the DLF _position_ of the cube
            //and the value here tells us which cubie is in this spot
            //indices and values are defined as CornerCubies in constants.h

        std::vector<long> m_cubie_states;
            //a map of the cubie rotational states, e.g.,
            //vector.at(0) represents the DLF _position_ of the cube
            //the value here tells us the rotational state of the cubie occupying this place
            //the value will be -1 for a counter-clockwise rotation of the cubie,
            //1 for a clockwise rotation, and 0 fo not rotation from normal

    public:

        void Display();
            /*  Draw the cube to the standard output  */

        bool GetState()
            { return m_state; };

        //face rotation operators
        //briefly, FaceDirection describe the rotation/transformation
        void FrontClock();
        void FrontCounter();
        void Front180();
        void BackClock();
        void BackCounter();
        void Back180();
        void LeftClock();
        void LeftCounter();
        void Left180();
        void RightClock();
        void RightCounter();
        void Right180();
        void UpClock();
        void UpCounter();
        void Up180();
        void DownClock();
        void DownCounter();
        void Down180();

        int Check();
            /*  function to assess the state of the cube, ie, is it solved?  */

        int CheckFace(int face);
            /*  assess the state of a particular face  */

        bool CheckCubie(int cubie);
            /*  assess the position and state of the passed cubie  */

        int TurnsToSolveCubie(int cubie);
            /* for the given face, check the cubies to see how displaced they are
               this is something of a manhattan distance  */

        unsigned int GetTopID();
            /*  return a similar UID for just the top cubies */

        unsigned int GetBottomID();
            /*  return a similar UID for just the bottom cubies */

        unsigned int GetPairID(int cubie1, int cubie2);
            /*  return a UID for any arbitrary pair of cubies  */

        Cube2& operator=(Cube2 &c);

        void Orient();
            /*  put the cube into the correct orientation  */

        void Randomize(int turns);
            /*  scramble the cube with turns random twists  */

        void Manipulate(std::vector<std::string> &sol);

        void GenerateUID();
            /*  generate a UID for this cube  */

        std::string GetUID()
        {
            std::stringstream ss;
            ss << m_ID;
            return std::string(ss.str());
        };

        Cube2();  //default constructor
        Cube2(Cube2 *); //copy constructor
        Cube2(std::vector<long> cubie_pos, std::vector<long> cubie_ori);  //construction from list of cubie positions and orientations
        ~Cube2(){};
};

#endif
