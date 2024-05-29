/*  SearchableNode Base Class definition
	Designed to be as generic as possible.  Combined with the appropriate search algorithm classes,
    this represents a node in the graph being searched.  Any classes implementing this interface (correctly),
    will  work with the search algorithms.
*/

#ifndef ISEARCHABLENODE_12Oct04_RKD
#define ISEARCHABLENODE_12Oct04_RKD

#include <vector>
#include <string>
#include <iostream>

class ISearchableNode
{
    friend bool NodeSort(ISearchableNode *left, ISearchableNode *right);

    private:

        std::string m_GeneratingTransform; //how did we get this node from the parent
        ISearchableNode *mp_ParentNode;
        float m_CostOfExpansion; //what did it cost to get from the parent to here?

        std::string m_UID; //unique ID for this node

        float m_G; //cost from start to here - expected to be set by the search fxn
        float m_H; //heuristic from here to Goal

    public:

        //standard cost functions
        float G() { return m_G; }; //known cost from start to here - set by the search function
        void G(float value) { m_G = value; }; //setter fxn

        float F() { return m_G + this->H(); }; //estimated total cost

        float H() { return m_H; }; //estimated cost to goal - should never overestimate actual cost
        void H(float value) { m_H = value; };

        virtual std::vector<ISearchableNode *> GetNeighbors() =0;
            //return all the immediate neighbors of this node
            //neighbors should store a pointer to this node so we have a linked list establishing the path
            //neighbors should store the cost of expansion from this node
            //neighbors should store the transform (as a string) that leads to their expansion

        std::string GetGeneratingTransform() { return m_GeneratingTransform; };
        void SetGeneratingTransform( std::string value ) { m_GeneratingTransform = value; };

        ISearchableNode *GetParentNode() { return mp_ParentNode; };
        void SetParentNode( ISearchableNode *p ) { mp_ParentNode = p; };

        float GetCostToExpand() { return m_CostOfExpansion; };
        void SetCostToExpand( float value ) { m_CostOfExpansion = value; };

        std::string GetUID() { return m_UID; };
        virtual void SetUID( std::string s ) { m_UID = s; };
            //nodes should set their own GUID

//        bool operator<( ISearchableNode &sn )
//        {
//            if ( this->F() < sn.F() )
//            {
//                std::cout << "true" << std::endl;
//                return true;
//            }
//            else
//            {
//                std::cout << "false" << std::endl;
//                return false;
//            }
//        };

        virtual bool operator==( ISearchableNode &sn ) =0;

        ISearchableNode()
        {
            m_GeneratingTransform = "";
            mp_ParentNode = 0;
            m_CostOfExpansion = 0.0;
            m_UID = "";
            m_G = 0.0;
            m_H = 0.0;
        };

        virtual void GenerateUID() =0;
        ~ISearchableNode() {};
};

#endif
