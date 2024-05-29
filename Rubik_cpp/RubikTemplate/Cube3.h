/**
    @author Robert Kirk DeLisle rkdelisle@gmail.com
    @version 0.1

    @section LICENSE

    @section DESCRIPTION

    3x3x3 Rubik's Cube class definition

    Defines a 3x3x3 Rubik's cube ans associated methods to allow manipulation of the cube.

    The cube is defined by 8 corner cubies and 12 edge cubies, their locations, and their rotations.
    The standard orientation of the cube is defined using the center cubies which do not move
    with standard manipulations which consist of rotations of the left, right, top, bottom, front
    or back layers.

    constants.h defines various constants that can define locations as well as specific cubies in a solved cube.
*/

#ifndef CUBE3_2012Nov02_RKD
#define CUBE3_2012Nov02_RKD

#include <vector>
#include <map>
#include <sstream>

#include "constants.h"

class Cube3
{
    friend bool NodeSort(Cube3 *left, Cube3 *right);
    friend bool IsGoal(Cube3 *c);

    //From SearchableNode
    private:

        std::string m_GeneratingTransform; ///how did we get this node from the parent
        Cube3 *mp_ParentNode;
        float m_CostOfExpansion; ///what did it cost to get from the parent to here?

        float m_G; ///cost from start to here - expected to be set by the search fxn
        float m_H; ///heuristic from here to Goal

    public:

        //standard cost functions
        float G() { return m_G; }; ///known cost from start to here - set by the search function
        void G(float value) { m_G = value; };

        float F() { return m_G + this->H(); }; ///estimated total cost

        float H(); ///estimated cost to goal - should never overestimate actual cost
        void H(float value) { m_H = value; };

        std::string GetGeneratingTransform() { return m_GeneratingTransform; };
        void SetGeneratingTransform( std::string value ) { m_GeneratingTransform = value; };

        Cube3 *GetParentNode() { return mp_ParentNode; };
        void SetParentNode( Cube3 *p ) { mp_ParentNode = p; };

        float GetCostToExpand() { return m_CostOfExpansion; };
        void SetCostToExpand( float value ) { m_CostOfExpansion = value; };

        void ReplaceNode( Cube3 *c )
        {
            mp_ParentNode = c->mp_ParentNode;
            m_CostOfExpansion = c->m_CostOfExpansion;
        }

        std::vector<Cube3 *> GetNeighbors();
            //return all the immediate neighbors of this node
            //neighbors should store a pointer to this node so we have a linked list establishing the path
            //neighbors should store the cost of expansion from this node
            //neighbors should store the transform (as a string) that leads to their expansion

        bool operator==( Cube3 &sn );

    //unique to Cube3
    private:

        bool m_state;  //true = solved, false = unsolved

        unsigned int m_ID; ///a unique ID for each state of the cube.
        std::string m_bitID; ///a string containing the UID of the cube

        /** Maps of the cubie locations and rotational states.
            e.g., m_corner_positions[DLF) returns the cubie ID in the down-left-front position

                m_corner_states.at(DLF) returns the rotational state of the cubie in the DLF position
                                    0 = not rotated from standard/solved rotation
                                    1 = rotated clockwise 1/3 turn
                                    2 = rotated counter-clockwise 1/3 turn
                                    sum of all corner states must be divisible by 3.
                                    If not, the cube is not solvable by standard manipulations

                m_edge_position[DL] returns the cubie ID in the down-left edge location

                m_edge_states[DL] returns the flip-state of the cubie in the down-left position
                                    0 = not flipped from standard/solved state
                                    1 = flipped
                                    sum of all flips must be even to be solvable by standard minipulations
        **/
        std::map<int, int> m_corner_positions;
        std::map<int, int> m_corner_states;
        std::map<int, int> m_edge_positions;
        std::map<int, int> m_edge_states;

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

        bool Check();
            ///  function to assess the state of the cube, ie, is it solved?

        bool CheckCubie(int cubie);
            ///  assess the position and state of the passed cubie

        Cube3& operator=(Cube3 &c);

        void Randomize(int turns);
            ///  scramble the cube with turns random twists

        void Manipulate(std::vector<std::string> &sol);
            ///  given a sequence of transforms encoded as a vector of strings, apply them to the cube

        void GenerateUID();
            ///  generate a UID for this cube

        std::string GetUID()
        {
            std::stringstream ss;
            ss << m_ID;
            return std::string(ss.str());
        };

        Cube3();  //default constructor
        Cube3(Cube3 *); //copy constructor
        Cube3(std::map<int, int> cubie_pos, std::map<int, int> cubie_ori);  //construction from list of cubie positions and orientations
        ~Cube3(){};
};

#endif
