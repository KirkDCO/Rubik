/*	Cube3
    A 3x3x3 Rubik's cube class
	02 November 2012 Robert Kirk DeLisle
*/
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <sstream>

#include "Cube3.h"

std::vector<Cube3 *> Cube3::GetNeighbors()
{
    //Expand this node to generate all neighbors in the graph
    //do it smart to avoid cycles or reversals of previous moves

    std::vector<Cube3 *> neighbors;
    Cube3 *tempCube;

    //first, generate the children
    //only one side is rotated in each plane in a 2x2x2 cube, rotation of one member of the plane
    //generates the same results as the opposite rotation of the opposite side
    //e.g. UP-COUNTER results in the same cube as DOWN-CLOCK
    //this reduces the branching factor from 18 to 9
    if ( GetGeneratingTransform().find("UP") == std::string::npos )
    {
        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->UpClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "UP_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->UpCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "UP_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->Up180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "UP_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("DOWN") == std::string::npos )
    {
        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->DownClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "DOWN_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->DownCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "DOWN_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->Down180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "DOWN_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("RIGHT") == std::string::npos )
    {
        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->RightClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "RIGHT_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->RightCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "RIGHT_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->Right180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "RIGHT_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("LEFT") == std::string::npos )
    {
        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->LeftClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "LEFT_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->LeftCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "LEFT_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->Left180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "LEFT_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("BACK") == std::string::npos )
    {
        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->BackClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "BACK_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->BackCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "BACK_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->Back180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "BACK_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("FRONT") == std::string::npos )
    {
        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->FrontClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "FRONT_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->FrontCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "FRONT_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube3;
        *tempCube = *this;
        tempCube->Front180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "FRONT_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    return neighbors;
}

Cube3::Cube3(Cube3 *c)
{
	//copy constructor
	m_state = c->GetState();

    m_corner_positions = c->m_corner_positions;
    m_corner_states = c->m_corner_states;
    m_CubieTurnsCurrent = c->m_CubieTurnsCurrent;
}

Cube3& Cube3::operator=( Cube3 &c )
{
    m_state = c.GetState();

    m_corner_positions = c.m_corner_positions;
    m_corner_states = c.m_corner_states;
    m_CubieTurnsCurrent = c.m_CubieTurnsCurrent;

	return *this;
}

Cube3::Cube3()
{
	//default constructor
	//creates a solved cube

    m_corner_positions[DLF] = DLF;
    m_corner_positions[DRF] = DRF;
    m_corner_positions[DRB] = DRB;
    m_corner_positions[DLB] = DLB;
    m_corner_positions[ULF] = ULF;
    m_corner_positions[URF] = URF;
    m_corner_positions[ULB] = ULB;
    m_corner_positions[URB] = URB;

    m_corner_statess[DLF] = 0;
    m_corner_statess[DRF] = 0;
    m_corner_statess[DRB] = 0;
    m_corner_statess[DLB] = 0;
    m_corner_statess[ULF] = 0;
    m_corner_statess[URF] = 0;
    m_corner_statess[ULB] = 0;
    m_corner_statess[URB] = 0;

    m_edge_positions[DL] = DL;
    m_edge_positions[DF] = DF;
    m_edge_positions[DR] = DR;
    m_edge_positions[DB] = DB;
    m_edge_positions[LF] = LF;
    m_edge_positions[RF] = RF;
    m_edge_positions[RB] = RB;
    m_edge_positions[LB] = LB;
    m_edge_positions[UL] = DL;
    m_edge_positions[UF] = DL;
    m_edge_positions[UR] = DL;
    m_edge_positions[UB] = DL;

    m_edge_states[DL] = 0;
    m_edge_states[DF] = 0;
    m_edge_states[DR] = 0;
    m_edge_states[DB] = 0;
    m_edge_states[LF] = 0;
    m_edge_states[RF] = 0;
    m_edge_states[RB] = 0;
    m_edge_states[LB] = 0;
    m_edge_states[UL] = 0;
    m_edge_states[UF] = 0;
    m_edge_states[UR] = 0;
    m_edge_states[UB] = 0;

    GenerateUID();

	m_state = true;

	m_GeneratingTransform = "";
    mp_ParentNode = 0;
    m_CostOfExpansion = 0.0;
    m_G = 0.0;
    m_H = 0.0;
}

Cube3::Cube3(std::map<int, int> cubie_pos, std::map<int, int> cubie_ori)
{
    //construct a cube based on the passed cubie positions and orientations
    //a HUGE assumption exists that the passed vectors actually make sense.

    bool solved = false;

    m_corner_positions = cubie_pos;
    m_corner_states = cubie_ori;

    //is it solved already?
    for (int x=0; x<8; x++)
    {
        if ( m_corner_positions[x] != x || m_corner_states[x] != 0 )
        {
            solved = false;
            break;  //no need to look further
        }
    }

    if (
        m_corner_positions[DLF] == DLF &&
        m_corner_positions[DRF] == DRF &&
        m_corner_positions[DRB] == DRB &&
        m_corner_positions[DLB] == DLB &&
        m_corner_positions[ULF] == ULF &&
        m_corner_positions[URF] == URF &&
        m_corner_positions[ULB] == ULB &&
        m_corner_positions[URB] == URB &&

        m_corner_statess[DLF] == 0 &&
        m_corner_statess[DRF] == 0 &&
        m_corner_statess[DRB] == 0 &&
        m_corner_statess[DLB] == 0 &&
        m_corner_statess[ULF] == 0 &&
        m_corner_statess[URF] == 0 &&
        m_corner_statess[ULB] == 0 &&
        m_corner_statess[URB] == 0
        )
        {
            solved = true;
        }
}

bool Cube3::Check()
{
	return (m_corner_positions[DLF] == DLF &&
            m_corner_positions[DRF] == DRF &&
            m_corner_positions[DRB] == DRB &&
            m_corner_positions[DLB] == DLB &&
            m_corner_positions[ULF] == ULF &&
            m_corner_positions[URF] == URF &&
            m_corner_positions[ULB] == ULB &&
            m_corner_positions[URB] == URB &&

            m_corner_statess[DLF] == 0 &&
            m_corner_statess[DRF] == 0 &&
            m_corner_statess[DRB] == 0 &&
            m_corner_statess[DLB] == 0 &&
            m_corner_statess[ULF] == 0 &&
            m_corner_statess[URF] == 0 &&
            m_corner_statess[ULB] == 0 &&
            m_corner_statess[URB] == 0);
}

void Cube3::BackClock()
{
	//Back face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLB];
	m_corner_positions[DLB] = m_corner_positions[ULB];
	m_corner_positions[ULB] = m_corner_positions[URB];
	m_corner_positions[URB] = m_corner_positions[DRB];
	m_corner_positions[DRB] = temp;

    //I have to move the states, too
    temp = m_corner_states[DLB];
	m_corner_states[DLB] = m_corner_states[ULB];
	m_corner_states[ULB] = m_corner_states[URB];
	m_corner_states[URB] = m_corner_states[DRB];
	m_corner_states[DRB] = temp;

    //now rotate the cubies
    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLB:
            case URB:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = 1;
                break;

            case ULB:
            case DRB:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UB];
	m_edge_positions[UB] = m_edge_positions[RB];
	m_edge_positions[RB] = m_edge_positions[DB];
	m_edge_positions[DB] = m_edge_positions[LB];
	m_edge_positions[LB] = temp;

	temp = m_edge_states[UB] * ( (m_edge_states[UB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UB] = m_edge_states[RB] * ( (m_edge_states[RB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RB] = m_edge_states[DB] * ( (m_edge_states[DB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DB] = m_edge_states[LB] * ( (m_edge_states[LB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LB] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::BackCounter()
{
	//Back face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLB];
	m_corner_positions[DLB] = m_corner_positions[DRB];
	m_corner_positions[DRB] = m_corner_positions[URB];
	m_corner_positions[URB] = m_corner_positions[ULB];
	m_corner_positions[ULB] = temp;

	//states have to move too
	temp = m_corner_states[DLB];
	m_corner_states[DLB] = m_corner_states[DRB];
	m_corner_states[DRB] = m_corner_states[URB];
	m_corner_states[URB] = m_corner_states[ULB];
	m_corner_states[ULB] = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLB:
            case URB:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = 1;
                break;

            case ULB:
            case DRB:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UB];
	m_edge_positions[UB] = m_edge_positions[LB];
	m_edge_positions[LB] = m_edge_positions[DB];
	m_edge_positions[DB] = m_edge_positions[RB];
	m_edge_positions[RB] = temp;

	temp = m_edge_states[UB] * ( (m_edge_states[UB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UB] = m_edge_states[LB] * ( (m_edge_states[LB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LB] = m_edge_states[DB] * ( (m_edge_states[DB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DB] = m_edge_states[RB] * ( (m_edge_states[RB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RB] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::Back180()
{
    //Back face, 180 degree rotation
    BackCounter();
    BackCounter();
}

void Cube3::DownClock()
{
	//  down face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLB];
	m_corner_positions[DLB] = m_corner_positions[DRB];
	m_corner_positions[DRB] = m_corner_positions[DRF];
	m_corner_positions[DRF] = m_corner_positions[DLF];
	m_corner_positions[DLF] = temp;

	//states have to follow
	temp = m_corner_states[DLB];
	m_corner_states[DLB] = m_corner_states[DRB];
	m_corner_states[DRB] = m_corner_states[DRF];
	m_corner_states[DRF] = m_corner_states[DLF];
	m_corner_states[DLF] = temp;

	//no rotations of corner cubies occurs

    //now for the edges
    temp = m_edge_positions[DF];
	m_edge_positions[DF] = m_edge_positions[DL];
	m_edge_positions[DL] = m_edge_positions[DB];
	m_edge_positions[DB] = m_edge_positions[DR];
	m_edge_positions[DR] = temp;

	temp = m_edge_states[DF] * ( (m_edge_states[DF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DF] = m_edge_states[DL] * ( (m_edge_states[DL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DL] = m_edge_states[DB] * ( (m_edge_states[DB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DB] = m_edge_states[DR] * ( (m_edge_states[DR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DR] = temp;

	m_CubieTurnsCurrent = false;
}

void Cube3::DownCounter()
{
	//  down face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLB];
	m_corner_positions[DLB] = m_corner_positions[DLF];
	m_corner_positions[DLF] = m_corner_positions[DRF];
	m_corner_positions[DRF] = m_corner_positions[DRB];
	m_corner_positions[DRB] = temp;

	//states follow
	temp = m_corner_states[DLB];
	m_corner_states[DLB] = m_corner_states[DLF];
	m_corner_states[DLF] = m_corner_states[DRF];
	m_corner_states[DRF] = m_corner_states[DRB];
	m_corner_states[DRB] = temp;

	//no rotations of corner cubies occurs

    //now for the edges
    temp = m_edge_positions[DF];
	m_edge_positions[DF] = m_edge_positions[DR];
	m_edge_positions[DR] = m_edge_positions[DB];
	m_edge_positions[DB] = m_edge_positions[DL];
	m_edge_positions[DL] = temp;

	temp = m_edge_states[DF] * ( (m_edge_states[DF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DF] = m_edge_states[DR] * ( (m_edge_states[DR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DR] = m_edge_states[DB] * ( (m_edge_states[DB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DB] = m_edge_states[DL] * ( (m_edge_states[DL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DL] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::Down180()
{
    //Down face, 180 degree rotation
    DownCounter();
    DownCounter();
}

void Cube3::FrontClock()
{
	//  front face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLF];
	m_corner_positions[DLF] = m_corner_positions[DRF];
	m_corner_positions[DRF] = m_corner_positions[URF];
	m_corner_positions[URF] = m_corner_positions[ULF];
	m_corner_positions[ULF] = temp;

	//states follow
	temp = m_corner_states[DLF];
	m_corner_states[DLF] = m_corner_states[DRF];
	m_corner_states[DRF] = m_corner_states[URF];
	m_corner_states[URF] = m_corner_states[ULF];
	m_corner_states[ULF] = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the front face
        switch(temp)
        {
            case DLF:
            case URF:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;

            case ULF:
            case DRF:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = 1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UF];
	m_edge_positions[UF] = m_edge_positions[LF];
	m_edge_positions[LF] = m_edge_positions[DF];
	m_edge_positions[DF] = m_edge_positions[RF];
	m_edge_positions[RF] = temp;

	temp = m_edge_states[UF] * ( (m_edge_states[UF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UF] = m_edge_states[LF] * ( (m_edge_states[LF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LF] = m_edge_states[DF] * ( (m_edge_states[DF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DF] = m_edge_states[RF] * ( (m_edge_states[RF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RF] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::FrontCounter()
{
	//  front face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLF];
	m_corner_positions[DLF] = m_corner_positions[ULF];
	m_corner_positions[ULF] = m_corner_positions[URF];
	m_corner_positions[URF] = m_corner_positions[DRF];
	m_corner_positions[DRF] = temp;

	//states follow
	temp = m_corner_states[DLF];
	m_corner_states[DLF] = m_corner_states[ULF];
	m_corner_states[ULF] = m_corner_states[URF];
	m_corner_states[URF] = m_corner_states[DRF];
	m_corner_states[DRF] = temp;


    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLF:
            case URF:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;

            case ULF:
            case DRF:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = +1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UF];
	m_edge_positions[UF] = m_edge_positions[RF];
	m_edge_positions[RF] = m_edge_positions[DF];
	m_edge_positions[DF] = m_edge_positions[LF];
	m_edge_positions[LF] = temp;

	temp = m_edge_states[UF] * ( (m_edge_states[UF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UF] = m_edge_states[RF] * ( (m_edge_states[RF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RF] = m_edge_states[DF] * ( (m_edge_states[DF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DF] = m_edge_states[LF] * ( (m_edge_states[LF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LF] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::Front180()
{
    //Front face, 180 degree rotation
    FrontCounter();
    FrontCounter();
}

void Cube3::LeftClock()
{
	//  left face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLF];
	m_corner_positions[DLF] = m_corner_positions[ULF];
	m_corner_positions[ULF] = m_corner_positions[ULB];
	m_corner_positions[ULB] = m_corner_positions[DLB];
	m_corner_positions[DLB] = temp;

	//states follow
	temp = m_corner_states[DLF];
	m_corner_states[DLF] = m_corner_states[ULF];
	m_corner_states[ULF] = m_corner_states[ULB];
	m_corner_states[ULB] = m_corner_states[DLB];
	m_corner_states[DLB] = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLF:
            case ULB:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = +1;
                break;

            case ULF:
            case DLB:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UL];
	m_edge_positions[UL] = m_edge_positions[LB];
	m_edge_positions[LB] = m_edge_positions[DL];
	m_edge_positions[DL] = m_edge_positions[LF];
	m_edge_positions[LF] = temp;

	temp = m_edge_states[UL] * ( (m_edge_states[UL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UL] = m_edge_states[LB] * ( (m_edge_states[LB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LB] = m_edge_states[DL] * ( (m_edge_states[DL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DL] = m_edge_states[LF] * ( (m_edge_states[LF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LF] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::LeftCounter()
{
	//  left face, counter clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DLF];
	m_corner_positions[DLF] = m_corner_positions[DLB];
	m_corner_positions[DLB] = m_corner_positions[ULB];
	m_corner_positions[ULB] = m_corner_positions[ULF];
	m_corner_positions[ULF] = temp;

	//states follow
	temp = m_corner_states[DLF];
	m_corner_states[DLF] = m_corner_states[DLB];
	m_corner_states[DLB] = m_corner_states[ULB];
	m_corner_states[ULB] = m_corner_states[ULF];
	m_corner_states[ULF] = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLF:
            case ULB:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = 1;
                break;

            case ULF:
            case DLB:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UL];
	m_edge_positions[UL] = m_edge_positions[LF];
	m_edge_positions[LF] = m_edge_positions[DL];
	m_edge_positions[DL] = m_edge_positions[LB];
	m_edge_positions[LB] = temp;

	temp = m_edge_states[UL] * ( (m_edge_states[UL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UL] = m_edge_states[LF] * ( (m_edge_states[LF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LF] = m_edge_states[DL] * ( (m_edge_states[DL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DL] = m_edge_states[LB] * ( (m_edge_states[LB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[LB] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::Left180()
{
    //Left face, 180 degree rotation
    LeftCounter();
    LeftCounter();
}

void Cube3::RightClock()
{
	//  right face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DRF];
	m_corner_positions[DRF] = m_corner_positions[DRB];
	m_corner_positions[DRB] = m_corner_positions[URB];
	m_corner_positions[URB] = m_corner_positions[URF];
	m_corner_positions[URF] = temp;

	//states follow
	temp = m_corner_states[DRF];
	m_corner_states[DRF] = m_corner_states[DRB];
	m_corner_states[DRB] = m_corner_states[URB];
	m_corner_states[URB] = m_corner_states[URF];
	m_corner_states[URF] = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DRF:
            case URB:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;

            case URF:
            case DRB:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = 1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UR];
	m_edge_positions[UR] = m_edge_positions[RF];
	m_edge_positions[RF] = m_edge_positions[DR];
	m_edge_positions[DR] = m_edge_positions[RB];
	m_edge_positions[RB] = temp;

	temp = m_edge_states[UR] * ( (m_edge_states[UR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UR] = m_edge_states[RF] * ( (m_edge_states[RF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RF] = m_edge_states[DR] * ( (m_edge_states[DR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DR] = m_edge_states[RB] * ( (m_edge_states[RB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RB] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::RightCounter()
{
	//  right face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[DRF];
	m_corner_positions[DRF] = m_corner_positions[URF];
	m_corner_positions[URF] = m_corner_positions[URB];
	m_corner_positions[URB] = m_corner_positions[DRB];
	m_corner_positions[DRB] = temp;

    //states follow
    temp = m_corner_states[DRF];
	m_corner_states[DRF] = m_corner_states[URF];
	m_corner_states[URF] = m_corner_states[URB];
	m_corner_states[URB] = m_corner_states[DRB];
	m_corner_states[DRB] = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DRF:
            case URB:
                m_corner_states[temp] += 1;
                if ( m_corner_states[temp] == 2 )
                    m_corner_states[temp] = -1;
                break;

            case URF:
            case DRB:
                m_corner_states[temp] -= 1;
                if ( m_corner_states[temp] == -2 )
                    m_corner_states[temp] = 1;
                break;
        }
    }

    //now for the edges
    temp = m_edge_positions[UR];
	m_edge_positions[UR] = m_edge_positions[RB];
	m_edge_positions[RB] = m_edge_positions[DR];
	m_edge_positions[DR] = m_edge_positions[RF];
	m_edge_positions[RF] = temp;

	temp = m_edge_states[UR] * ( (m_edge_states[UR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UR] = m_edge_states[RB] * ( (m_edge_states[RB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RB] = m_edge_states[DR] * ( (m_edge_states[DR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[DR] = m_edge_states[RF] * ( (m_edge_states[RF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[RF] = temp;

    m_CubieTurnsCurrent = false;
}

void Cube3::Right180()
{
    //Right face, 180 degree rotation
    RightCounter();
    RightCounter();
}

void Cube3::UpClock()
{
	//  up face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[URF];
	m_corner_positions[URF] = m_corner_positions[URB];
	m_corner_positions[URB] = m_corner_positions[ULB];
	m_corner_positions[ULB] = m_corner_positions[ULF];
	m_corner_positions[ULF] = temp;

	//states follow
	temp = m_corner_states[URF];
	m_corner_states[URF] = m_corner_states[URB];
	m_corner_states[URB] = m_corner_states[ULB];
	m_corner_states[ULB] = m_corner_states[ULF];
	m_corner_states[ULF] = temp;

	//no rotations of corner cubies occurs

    //now for the edges
    temp = m_edge_positions[UF];
	m_edge_positions[UF] = m_edge_positions[UR];
	m_edge_positions[UR] = m_edge_positions[UB];
	m_edge_positions[UB] = m_edge_positions[UL];
	m_edge_positions[UL] = temp;

	temp = m_edge_states[UF] * ( (m_edge_states[UF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UF] = m_edge_states[UR] * ( (m_edge_states[UR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UR] = m_edge_states[UB] * ( (m_edge_states[UB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UB] = m_edge_states[UL] * ( (m_edge_states[UL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UL] = temp;

	m_CubieTurnsCurrent = false;
}

void Cube3::UpCounter()
{
	//  up face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_corner_positions[URF];
	m_corner_positions[URF] = m_corner_positions[ULF];
	m_corner_positions[ULF] = m_corner_positions[ULB];
	m_corner_positions[ULB] = m_corner_positions[URB];
	m_corner_positions[URB] = temp;

	//states follow
	temp = m_corner_states[URF];
	m_corner_states[URF] = m_corner_states[ULF];
	m_corner_states[ULF] = m_corner_states[ULB];
	m_corner_states[ULB] = m_corner_states[URB];
	m_corner_states[URB] = temp;

	//no rotations of corner cubies occurs

    //now for the edges
    temp = m_edge_positions[UF];
	m_edge_positions[UF] = m_edge_positions[UL];
	m_edge_positions[UL] = m_edge_positions[UB];
	m_edge_positions[UB] = m_edge_positions[UR];
	m_edge_positions[UR] = temp;

	temp = m_edge_states[UF] * ( (m_edge_states[UF] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UF] = m_edge_states[UL] * ( (m_edge_states[UL] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UL] = m_edge_states[UB] * ( (m_edge_states[UB] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UB] = m_edge_states[UR] * ( (m_edge_states[UR] & BACK == BACK) ? 1 : -1 );
	m_edge_states[UR] = temp;

	m_CubieTurnsCurrent = false;
}

void Cube3::Up180()
{
    //up face, 180 degree rotation
    UpCounter();
    UpCounter();
}

void Cube3::Display()
{

}

void Cube3::GenerateUID()
{
    /*  A unique ID is generated for each cube.  This can be done by considering the corners
        individually with regard to which cubie is present and what orientation it is in.
        The down(white)-left(yellow)-front(red) (DLF/WYR) corner is considered to be fixed
        and the other are considered in turn.  Each of the other 7 positions could have one
        of 7 cubies in one of 3 possible orientations.  The result is 7 positions * (3 bits
        to describe which cubie is present + 2 bits) to describe the orientation = 35 bits
    */

    //get rid of the old bitID
    m_bitID.clear();

    //generate a binary representation of the cube:
    //with DLF as standard reference, and the information of 6 other cubie locations
    //all we need is the orientation of the 8th cubie
    //this allows me to have a 32-bit representation for the cube
    for (int cubie=1; cubie<7; cubie++)
    {
        if ( m_corner_positions[cubie] ==  DRF )
            m_bitID += "001";
        else if ( m_corner_positions[cubie] ==  DLB )
            m_bitID += "010";
        else if ( m_corner_positions[cubie] ==  DRB )
            m_bitID += "011";
        else if ( m_corner_positions[cubie] ==  ULF )
            m_bitID += "100";
        else if ( m_corner_positions[cubie] ==  URF )
            m_bitID += "101";
        else if ( m_corner_positions[cubie] ==  ULB )
            m_bitID += "110";
        else if ( m_corner_positions[cubie] ==  URB )
            m_bitID += "111";

        if ( m_corner_states[cubie] == 0 ) //neutral rotation
            m_bitID += "00";
        else if ( m_corner_states[cubie] == 1 )
            m_bitID += "01";
        else if ( m_corner_states[cubie] == -1 )
            m_bitID += "10";
    }

    //add on the last cubie's orientation
    if ( m_corner_states[7] == 0 ) //neutral rotation
        m_bitID += "00";
    else if ( m_corner_states[7] == 1 ) //clockwise
        m_bitID += "01";
    else if ( m_corner_states[7] == -1 )  //counter-clockwise
        m_bitID += "10";

    //reset the ID
    m_ID = 0;

    //now we've got the binary, so convert it to an unsigned int
    for (int x = 0; x < m_bitID.length(); x++)
    {
        if ( (m_bitID.substr(x,1)).compare("1") == 0 )
            m_ID += static_cast<unsigned int>(pow(2.0,(31-x)));  //(31-x) included to make sure each bit is promoted to the right power of 2
    }
}

unsigned int Cube3::GetPairID(int cubie1, int cubie2)
{
    /*  Generate a UID for the positions and orientations of the requested cubies.
        The representation consists of 16 bits:
        3 bits - sought cubie ID
        3 bits - sought cubie location
        2 bits - sought cubie orientation
        times 2
        In order to prevent duplicates, the cubie with the lowest numerical value
        (order determined by the CornerCubies enumeration in constants.h)
        is forced to be cubie1
    */

    std::vector<unsigned int> pairID(6,0);
    unsigned int ID=0;
    int swap;

    //force proper ordering
    if ( cubie2 < cubie1 )
    {
        swap = cubie1;
        cubie1 = cubie2;
        cubie2 = swap;
    }

    //find the first sought cubie
    pairID.at(0) = cubie1; //sought cubie #1
    for (unsigned int cubie=1; cubie<8; cubie++)
    {
        if ( m_corner_positions[cubie] == cubie1 )
        {
            pairID.at(1) = cubie;
            if ( m_corner_states[cubie] == -1 )
                pairID.at(2) = 2;
            else
                pairID.at(2) = m_corner_states[cubie];

            break;
        }
    }

    //now the second sought cubie
    pairID.at(3) = cubie2; //sought cubie #2
    for (unsigned int cubie=1; cubie<8; cubie++)
    {
        if ( m_corner_positions[cubie] == cubie2 )
        {
            pairID.at(4) = cubie;
            if ( m_corner_states[cubie] == -1 )
                pairID.at(5) = 2;
            else
                pairID.at(5) = m_corner_states[cubie];

            break;
        }
    }

    //convert the IDs into a single UID
    ID = ID + ( pairID.at(0) << 13);    //std::cout << pairID.at(0) << " " << ID << std::endl;
    ID = ID + ( pairID.at(1) << 10);    //std::cout << pairID.at(1) << " " << ID << std::endl;
    ID = ID + ( pairID.at(2) << 8);     //std::cout << pairID.at(2) << " " << ID << std::endl;
    ID = ID + ( pairID.at(3) << 5);     //std::cout << pairID.at(3) << " " << ID << std::endl;
    ID = ID + ( pairID.at(4) << 2);     //std::cout << pairID.at(4) << " " << ID << std::endl;
    ID = ID + ( pairID.at(5) );         //std::cout << pairID.at(5) << " " << ID << std::endl;

    return ID;
}
unsigned int Cube3::GetTopID()
{
    /*  Generate a UID for the positions and orientations of the top cubies only.
        The representation for this one is different from the global UID.  That version
        evaluated each position for which cubie was present.  In this case, I can save
        space by evaluating each cubie of interest and where it is located.  This
        gives 4 cubies of interest * ( 7 possible locations (3 bits) + 3 possible
        orientations (2 bits)) = 20 bits.  It still fits within an unsigned integer.
    */

    bool found = false; //tracks whether the target cubie has been found
    std::vector<unsigned int> topID(4, 0);
    unsigned int ID=0;

    for (int cubie=1; cubie<8; cubie++)
    {
        found = false;
        switch ( m_corner_positions[cubie] )
        {
            case ULF:
            case URF:
            case ULB:
            case URB:
                topID.at( m_corner_positions[cubie]-4 ) = (cubie << 2);
                found = true;
        }

        if ( found )
        {
            if ( m_corner_states[cubie] == 0 ) //neutral rotation
            {
                topID.at(m_corner_positions[cubie]-4) = topID.at(m_corner_positions[cubie]-4) | 0; //effectively leaves it with zeros at the tail
            }
            else if ( m_corner_states[cubie] == 1 ) //clockwise
            {
                topID.at(m_corner_positions[cubie]-4) = topID.at(m_corner_positions[cubie]-4) | 1;
            }
            else if ( m_corner_states[cubie] == -1 ) //counter-clockwise
             {
                topID.at(m_corner_positions[cubie]-4) = topID.at(m_corner_positions[cubie]-4) | 2;
             }
        }
    }

    for (int cubie=0; cubie<4; cubie++)
    {
        ID = ID + ( topID.at(cubie) << ( cubie*5 ));
    }

    return ID;
}

unsigned int Cube3::GetBottomID()
{
    /*  Generate a UID for the positions and orientations of the bottom cubies only.
        The representation for this one is different from the global UID.  That version
        evaluated each position for which cubie was present.  In this case, I can save
        space by evaluating each cubie of interest and where it is located.  This
        gives 3 cubies of interest * ( 7 possible locations (3 bits) + 3 possible
        orientations (2 bits)) = 15 bits.  It still fits within an unsigned integer.
        The DLF cubie is ignored as it is the standard reference for all others.
    */

    bool found = false; //tracks whether the target cubie has been found
    std::vector<unsigned int> botID(3, 0);
    unsigned int ID=0;

    for (int cubie=1; cubie<8; cubie++)
    {
        found = false;
        switch ( m_corner_positions[cubie] )
        {
            case DRF:
            case DLB:
            case DRB:
                botID.at( m_corner_positions[cubie]-1 ) = (cubie << 2);
                found = true;
        }

        if ( found )
        {
            if ( m_corner_states[cubie] == 0 ) //neutral rotation
            {
                botID.at(m_corner_positions[cubie]-1) = botID.at(m_corner_positions[cubie]-1) | 0; //effectively leaves it with zeros at the tail
            }
            else if ( m_corner_states[cubie] == 1 ) //clockwise
            {
                botID.at(m_corner_positions[cubie]-1) = botID.at(m_corner_positions[cubie]-1) | 1;
            }
            else if ( m_corner_states[cubie] == -1 ) //counter-clockwise
             {
                botID.at(m_corner_positions[cubie]-1) = botID.at(m_corner_positions[cubie]-1) | 2;
             }
        }
    }

    for (int cubie=0; cubie<3; cubie++)
    {
        ID = ID + ( botID.at(cubie) << ( cubie*5 ));
    }

    return ID;
}

bool Cube3::CheckCubie(int cubie)
{
    /*  assess the position and state of the passed cubie  */
    /*  return true if the cubie is in its solved position  */
    return ( m_corner_positions[cubie] == cubie && m_corner_states[cubie] == 0 );
}

void Cube3::Randomize(int turns)
{
    /*  randomize the cube by imposing "turns" random twists  */
    //srand( time(NULL) );
    for ( int x=0; x<turns; x++)
    {
        switch ( rand() % 9 )
        {
            case 0:
                UpCounter();
                //std::cout << "UpCounter" << std::endl;
                break;

            case 1:
                UpClock();
                //std::cout << "UpClock" << std::endl;
                break;

            case 2:
                Up180();
                //std::cout << "Up180" << std::endl;
                break;

            case 3:
                RightCounter();
                //std::cout << "RightCounter" << std::endl;
                break;

            case 4:
                RightClock();
                //std::cout << "RightClock" << std::endl;
                break;

            case 5:
                Right180();
                //std::cout << "Right180" << std::endl;
                break;

            case 6:
                BackCounter();
                //std::cout << "BackCounter" << std::endl;
                break;

            case 7:
                BackClock();
                //std::cout << "BackClock" << std::endl;
                break;

            case 8:
                Back180();
                //std::cout << "Back180" << std::endl;
                break;
        }
    }
    GenerateUID();
}

bool Cube3::operator==( Cube3 &sn )
{
    GenerateUID();
    sn.GenerateUID();

    if ( GetUID() == sn.GetUID() )
        return true;
    else
        return false;
}

float Cube3::H()
{
    return m_H;
};

void Cube3::Manipulate(std::vector<std::string> &sol)
{
    for (int s=0; s<sol.size(); s++)
    {
            if( sol.at(s) == "UP_CLOCK" )
                UpClock();

            if( sol.at(s) == "UP_COUNTER" )
                UpCounter();

            if( sol.at(s) == "UP_180" )
                Up180();

            if( sol.at(s) == "RIGHT_CLOCK" )
                RightClock();

            if( sol.at(s) == "RIGHT_COUNTER" )
                RightCounter();

            if( sol.at(s) == "RIGHT_180" )
                Right180();

            if( sol.at(s) == "BACK_CLOCK" )
                BackClock();

            if( sol.at(s) == "BACK_COUNTER" )
                BackCounter();

            if( sol.at(s) == "BACK_180" )
                Back180();
        }
}
