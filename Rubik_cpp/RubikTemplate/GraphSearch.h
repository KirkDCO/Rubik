/**
    @author Robert Kirk DeLisle rkdelisle@gmail.com
    @version 0.1

    @section LICENSE

    @section DESCRIPTION

    GraphSearch Template Class
    Provides different graph search/pathfinding algorithms.
    Currently:
        Dijkstra
        A*
        IDA*

    Usage:  GraphSearch<Searchable, Heuristic>
    Searchable class:
        Any class intended to serve as a node within a graph for the search.
        Must implement:
            std::string GetUID() - retrieve a globally unique ID for the node
            vector< Searchable * > GetNeighbors() - get all valid neighbors of this node from the graph
            float G() - retrieve the G value (cost thus far) for this node
            void G(float value) - set the G value for this node
            float GetCostToExpand() - how much did it cost to expand this node from the parent
            Searchable * GetParentNode() - retrieve a pointer to the parent node.  If null, assumed to be the start location of the search
            std::string GetGeneratingTransform() - what is the parsable transform that led to generation of this node from the parent
                                                    a vector of these will be stored as the solution/path from Start to Goal
            void ReplaceNode( Searchable * ) - used to replace one node (the caller) with another (the parameter) in the search
                                                this will maintain the order of the nodes in the list and apply the correct costs
                                                if we find a shorter path to a node after first evaluating it

        These functions need to be defined to operate on the Searchable class:
            bool NodeSort( Searchable &left, Searchable &right ) - sorting algorithm used by GraphSearch to maintain search priority returns (left.F() > right.F())
            bool IsGoal( Searchable * ) - is this node the goal?

    Heuristic Template Class:
        A class providing a function to calculate the appropriate heuristic for the search.
        Implements:
            float ComputeH( Searchable & ) - compute and store the heuristic for the passed node

        Note:  for Dijkstra, ComputeH() returns 0.0

*/


#ifndef GRAPHSEARCH_12Oct12_RKD
#define GRAPHSEARCH_12Oct12_RKD

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "GraphSearchUtilityFxns.h"

template<class Searchable, class H>
class GraphSearch
{
    private:
        /** Vector to hold the solution.
            Each entry correponds to the results of Searchable::GetGeneratingTransform() moving from
            node to node in the search space.
            Each transformation is a string that comes from Searchable::GetGeneratingTransform() and correcponds
            to a transormation applied to the object represented by the node.
        */
        std::vector<std::string> m_Solution;
        bool m_SolutionFound;

        /** Store the transformations necessary to move from the Start node to the Goal node.
            Each transformation is a string that comes from Searchable::GetGeneratingTransform() and correcponds
            to a transormation applied to the object represented by the node.
        */
        std::vector<std::string> StorePathToGoal(Searchable *);

        /**  Depth-limited search used iterative by IDA* */
        bool DLS( Searchable *node, H &heu, float start_cost, float *cost_limit, float maximum_cost_limit, std::vector<std::string> &sol );


    public:

        /** Default constructor.  Initialize necessary variables. */
        GraphSearch() { Init(); };

        /** Initialize object variables. */
        void Init()
        {
            m_SolutionFound = false;
            m_Solution.clear();
//            vpq_open_list.clear();
//            map_open_list.clear();
//            map_closed_list.clear();
        };

        /** The Search Algorithms
            \param *Start   -   Pointer to the start node as a Searchable object.
            \param *heu     -   Heuristic object to be used by the algorithm.
            \return true if a solution is found
                    false if the search space is fully evaluated and no solution is found
        */
        bool AStar(Searchable *Start, H heu);
        bool IDAStar(Searchable *Start, H heu, float maximum_cost_limit);

        /** Retrieve the solution/path from Start to Goal */
        std::vector<std::string> GetSolution() const
        {
            return m_Solution;
        };

        /** Default destructor */
        ~GraphSearch() {};
};

template<class Searchable, class H>
bool GraphSearch<Searchable, H>::IDAStar(Searchable *Start, H heu, float maximum_cost_limit)
{
    float cost_limit = heu.ComputeH(Start);
    std::vector<std::string> sol;
    bool res;

    while(true)
    {
        res = DLS( Start, heu, 0, &cost_limit, maximum_cost_limit, sol );
        if ( res )
        {
            std::reverse(sol.begin(), sol.end());
            m_Solution = sol;
            return true;
        }
        if ( cost_limit >= maximum_cost_limit )
            return false;
    }
}

template<class Searchable, class H>
bool GraphSearch<Searchable, H>::DLS( Searchable *node, H &heu, float start_cost, float *cost_limit, float maximum_cost_limit, std::vector<std::string> &sol )
{
    std::vector<Searchable *> neighbor_list;
    float next_cost_limit, new_start_cost, min_cost, new_cost_limit;
    bool res;

    //dead end elimination step
    min_cost = start_cost + heu.ComputeH(node);
    if ( min_cost > *cost_limit )
    {
        *cost_limit = min_cost;
        return false;
    }
    if ( IsGoal(node) )
        return true;

    neighbor_list = node->GetNeighbors();
    next_cost_limit = maximum_cost_limit;
    for( typename std::vector<Searchable *>::iterator it=neighbor_list.begin(); it != neighbor_list.end(); it++ )
    {
        new_cost_limit = *cost_limit;
        new_start_cost = start_cost + (*it)->GetCostToExpand();
        res = DLS( (*it), heu, new_start_cost, &new_cost_limit, maximum_cost_limit, sol );
        if (res)
        {
            sol.push_back( (*it)->GetGeneratingTransform() );
            break;
        }
        next_cost_limit = std::min( next_cost_limit, new_cost_limit );
    }

    for( typename std::vector<Searchable *>::iterator it=neighbor_list.begin(); it != neighbor_list.end(); it++ )
            delete (*it);

    if (res)
        return true;
    else
    {
        *cost_limit = next_cost_limit;
        return false;
    }
}

template<class Searchable, class H>
bool GraphSearch<Searchable, H>::AStar(Searchable *Start, H heu)
{
    /* The priority queue will be a vector managed by std::algorithm::make_heap(), ::push_heap() and ::pop_heap(). */
    std::vector<Searchable *> vpq_open_list;

    /* To facilitate searching the priority queue, a std::map will be maintained in parallel. */
    std::map<std::string, Searchable *> map_open_list; //

    /* Closed list will be a std::map to facilitate easy searching */
    std::map<std::string, Searchable *> map_closed_list;

    Searchable *current;
    std::vector<Searchable *> neighbor_list;

    Init();

    vpq_open_list.push_back(Start);
    std::make_heap(vpq_open_list.begin(), vpq_open_list.end(), NodeSort);
    map_open_list[ Start->GetUID() ] = Start;

    Start->G(0.0);

    while (!vpq_open_list.empty())
    {
        std::pop_heap(vpq_open_list.begin(), vpq_open_list.end(), NodeSort);
        current = vpq_open_list.back();

        if ( IsGoal(current) )
        {
            m_SolutionFound = true;
            StorePathToGoal( current );

            //take the Start out of the lists so that it doesn't get deleted
            map_open_list.erase(Start->GetUID());
            map_closed_list.erase(Start->GetUID());

            //free resources
            for ( typename std::map<std::string, Searchable *>::iterator it=map_open_list.begin(); it!=map_open_list.end(); it++)
                delete (*it).second;
            map_open_list.clear();

            for ( typename std::map<std::string, Searchable *>::iterator it=map_closed_list.begin(); it!=map_closed_list.end(); it++)
                delete (*it).second;
            map_closed_list.clear();

            return true;
        }

        vpq_open_list.pop_back();
        map_open_list.erase( current->GetUID() );
        map_closed_list[ current->GetUID() ] = current;

        neighbor_list = current->GetNeighbors();
        for( typename std::vector<Searchable *>::iterator it=neighbor_list.begin(); it != neighbor_list.end(); it++ )
        {
            if ( map_closed_list.find( (*it)->GetUID() ) != map_closed_list.end() )
            {
                delete *it;
                continue;
            }

            if ( map_open_list.find( (*it)->GetUID() ) != map_open_list.end() )
            {
                typename std::map<std::string, Searchable *>::iterator ol_it = map_open_list.find( (*it)->GetUID() );
                if ( (*ol_it).second->G() > current->G() + (*it)->GetCostToExpand() )
                {
                    (*ol_it).second->ReplaceNode( (*it) );
                    (*ol_it).second->G( current->G() + (*it)->GetCostToExpand() );
                    (*ol_it).second->H( heu.ComputeH( (*ol_it).second ));
                    std::make_heap(vpq_open_list.begin(), vpq_open_list.end(), NodeSort);
                }
                delete *it;
            }
            else
            {
                (*it)->G( current->G() + (*it)->GetCostToExpand() );
                (*it)->H( heu.ComputeH( (*it) ));
                map_open_list[ (*it)->GetUID() ] = (*it);
                vpq_open_list.push_back( (*it) );
                std::push_heap(vpq_open_list.begin(), vpq_open_list.end(), NodeSort);
            }
        }
    }

    m_SolutionFound = false;
    return false;  //if we get here, we've exhaused the entire search space
}

template<class Searchable, class H>
std::vector<std::string> GraphSearch<Searchable, H>::StorePathToGoal(Searchable *current)
{
    //capture the path to the goal once it is found - used by A*
    //return the length of that path

    m_Solution.clear();

    if ( !m_SolutionFound )
        return m_Solution;

    while( current->GetParentNode() != 0 )
    {
        m_Solution.push_back( current->GetGeneratingTransform() );
        current = current->GetParentNode();
    };

    std::reverse(m_Solution.begin(), m_Solution.end());
    return m_Solution;
}

#endif
