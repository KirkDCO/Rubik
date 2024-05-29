/*	Cube2
    A 2x2x2 Rubik's cube class
	21 January 2001 Robert Kirk DeLisle
*/
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <sstream>

#include "Cube2.h"

std::vector<Cube2 *> Cube2::GetNeighbors()
{
    //Expand this node to generate all neighbors in the graph
    //do it smart to avoid cycles or reversals of previous moves

    std::vector<Cube2 *> neighbors;
    Cube2 *tempCube;

    //first, generate the children
    //only one side is rotated in each plane in a 2x2x2 cube, rotation of one member of the plane
    //generates the same results as the opposite rotation of the opposite side
    //e.g. UP-COUNTER results in the same cube as DOWN-CLOCK
    //this reduces the branching factor from 18 to 9
    if ( GetGeneratingTransform().find("UP") == std::string::npos && GetGeneratingTransform().find("DOWN") == std::string::npos )
    {
        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->UpClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "UP_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->UpCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "UP_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->Up180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "UP_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("LEFT") == std::string::npos && GetGeneratingTransform().find("RIGHT") == std::string::npos )
    {
        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->RightClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "RIGHT_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->RightCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "RIGHT_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->Right180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "RIGHT_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    if ( GetGeneratingTransform().find("FRONT") == std::string::npos && GetGeneratingTransform().find("BACK") == std::string::npos )
    {
        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->BackClock();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "BACK_CLOCK" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->BackCounter();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "BACK_COUNTER" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );

        tempCube = new Cube2;
        *tempCube = *this;
        tempCube->Back180();
        tempCube->GenerateUID();
        tempCube->SetParentNode(this);
        tempCube->SetGeneratingTransform( "BACK_180" );
        tempCube->SetCostToExpand(1.0);
        neighbors.push_back( tempCube );
    }

    return neighbors;
}

Cube2::Cube2(Cube2 *c)
{
	//copy constructor
	m_state = c->GetState();
	m_oriented = c->m_oriented;

    m_cubie_positions = c->m_cubie_positions;
    m_cubie_states = c->m_cubie_states;
    m_CubieTurnsCurrent = c->m_CubieTurnsCurrent;

    Orient();
}

Cube2& Cube2::operator=( Cube2 &c )
{
    m_state = c.GetState();
    m_oriented = c.m_oriented;

    m_cubie_positions = c.m_cubie_positions;
    m_cubie_states = c.m_cubie_states;
    m_CubieTurnsCurrent = c.m_CubieTurnsCurrent;

    Orient();

	return *this;
}


Cube2::Cube2()
{
	//default constructor
	//creates a solved cube
	//the color scheme is specific to my own 2x2x2 cube
    int x;

    for (x=0; x<8; x++)
    {
        m_cubie_positions.push_back(x);  //set each cubie into its designated location
        m_cubie_states.push_back(0); //and properly oriented
    }

    GenerateUID();

	m_state = true;
	m_oriented = true;
	m_CubieTurnsCurrent = false;

	m_GeneratingTransform = "";
    mp_ParentNode = 0;
    m_CostOfExpansion = 0.0;
    m_G = 0.0;
    m_H = 0.0;
}

Cube2::Cube2(std::vector<long> cubie_pos, std::vector<long> cubie_ori)
{
    //construct a cube based on the passed cubie positions and orientations
    //a HUGE assumption exists that the passed vectors actually make sense.

    bool solved = true;  //optimistic outlook

    m_cubie_positions = cubie_pos;
    m_cubie_states = cubie_ori;

    Orient();

    //is it solved already?
    for (int x=0; x<8; x++)
    {
        if ( m_cubie_positions.at(x) != x || m_cubie_states.at(x) != 0 )
        {
            solved = false;
            break;  //no need to look further
        }
    }

    m_CubieTurnsCurrent = false;
}

int Cube2::Check()
{
	//Determine how many cubies are misplaced, ie, wrong position
	//at this point, I'm ignoring the twist
	int misplaced = 0;

	if ( !m_oriented )
        Orient(); //put in standard reference

    for ( long x=0; x<8; x++ )
    {
        if ( m_cubie_positions.at(x) != x )
            misplaced++;
    }

	return misplaced;
}

int Cube2::TurnsToSolveCubie(int cubie)
{
    //for the given cubie, determine how many turns it takes to get it in position and oriented
    int turns=0;
    int loc;
    int state;

    if ( m_CubieTurnsCurrent )
        return m_CubieTurns;

    //find the cubie's location
    for ( int p=0; p<8; p++)
    {
        if ( m_cubie_positions.at(p) == cubie )
        {
            loc = p;
            state = m_cubie_states.at(loc);
            break;
        }
    }

    //now add up the distances from our desired location
    switch (cubie)
    {
        case ULF:
            switch (loc)
            {
                case ULF:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case URF:
                    if ( state == 0 || state == 1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case ULB:
                    if ( state == 0 || state == -1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case URB:
                    if ( state == 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case DRF:
                case DLB:
                    if ( state == 0 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DRB:
                    turns += 2;
                    break;
            }
            break;

        case URF:
            switch (loc)
            {
                case URF:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case ULF:
                    if ( state == 0 || state == -1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case URB:
                    if ( state == 0 || state == 1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case ULB:
                    if ( state == 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case DRB:
                    if ( state == 0 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DRF:
                    if ( state == 0 )
                        turns += 2;
                    else
                        turns += 1;
                    break;
                case DLB:
                    turns += 2;
                    break;
            }
            break;

        case ULB:
            switch (loc)
            {
                case ULB:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case URB:
                    if ( state == 0 || state == -1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case ULF:
                    if ( state == 0 || state == 1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case URF:
                    if ( state == 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case DRB:
                    if ( state == 0 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DLB:
                    if ( state == 0 )
                        turns += 2;
                    else
                        turns += 1;
                    break;
                case DRF:
                    turns += 2;
                    break;
            }
            break;

        case URB:
            switch (loc)
            {
                case URB:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case ULB:
                    if ( state == 0 || state == 1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case URF:
                    if ( state == 0 || state == -1 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case ULF:
                    if ( state == 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
                case DRF:
                case DLB:
                    if ( state == 0 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DRB:
                    if ( state == 0 )
                        turns += 2;
                    else
                        turns += 1;
                    break;
            }
            break;
        case DRF:
            switch (loc)
            {
                case DRF:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case DLB:
                    if ( state == 0 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DRB:
                    if ( state == 0 || state == -1)
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case URB:
                    if ( state != 0 )
                        turns+=2;
                    else
                        turns+=1;
                    break;
                case ULB:
                    turns+=2;
                    break;
                case URF:
                    if ( state == 0 )
                        turns+=2;
                    else
                        turns+=1;
                    break;
                case ULF:
                    if ( state == 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
            }
            break;
        case DLB:
            switch (loc)
            {
                case DLB:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case DRF:
                    if ( state == 0 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DRB:
                    if ( state == 0 || state == 1)
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case URB:
                    if ( state != 0 )
                        turns+=2;
                    else
                        turns+=1;
                    break;
                case URF:
                    turns+=2;
                    break;
                case ULF:
                    if ( state != 0 )
                        turns+=2;
                    else
                        turns+=1;
                    break;
                case ULB:
                    if ( state != 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
            }
            break;
        case DRB:
            switch (loc)
            {
                case DRB:
                    if ( state != 0 )
                        turns+=7;
                    break;
                case DRF:
                    if ( state == 0 || state == -1 )
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case DLB:
                    if ( state == 0 || state == -1)
                        turns += 1;
                    else
                        turns += 2;
                    break;
                case URF:
                    if ( state != 0 )
                        turns+=2;
                    else
                        turns+=1;
                    break;
                case ULF:
                    turns+=2;
                    break;
                case ULB:
                    if ( state != 0 )
                        turns+=2;
                    else
                        turns+=1;
                    break;
                case URB:
                    if ( state != 0 )
                        turns+=1;
                    else
                        turns+=2;
                    break;
            }
            break;
    }

    m_CubieTurns = turns;
    m_CubieTurnsCurrent = true;
    return m_CubieTurns;
}

int Cube2::CheckFace(int face)
{
	//Check if the requested face is solved
	int misplaced = 0; //this is somewhat meaningless

    if ( !m_oriented )
        Orient();

	switch (face)
	{
	    case UP:
            if ( m_cubie_positions.at(ULF) != ULF || m_cubie_states.at(ULF) != 0 )
                misplaced++;
            if ( m_cubie_positions.at(URF) != URF || m_cubie_states.at(URF) != 0 )
                misplaced++;
            if ( m_cubie_positions.at(ULB) != ULB || m_cubie_states.at(ULB) != 0 )
                misplaced++;
            if ( m_cubie_positions.at(URB) != URB || m_cubie_states.at(URB) != 0 )
                misplaced++;
            break;

        case DOWN:
            if ( m_cubie_positions.at(DLF) != DLF || m_cubie_states.at(DLF) != 0)
                misplaced++;
            if ( m_cubie_positions.at(DRF) != DRF || m_cubie_states.at(DRF) != 0)
                misplaced++;
            if ( m_cubie_positions.at(DLB) != DLB || m_cubie_states.at(DLB) != 0)
                misplaced++;
            if ( m_cubie_positions.at(DRB) != DRB || m_cubie_states.at(DRB) != 0)
                misplaced++;
            break;
	}

	return misplaced;
}

void Cube2::BackClock()
{
	//Back face, clockwise rotation
	long temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLB);
	m_cubie_positions.at(DLB) = m_cubie_positions.at(ULB);
	m_cubie_positions.at(ULB) = m_cubie_positions.at(URB);
	m_cubie_positions.at(URB) = m_cubie_positions.at(DRB);
	m_cubie_positions.at(DRB) = temp;

    //I have to move the states, too
    temp = m_cubie_states.at(DLB);
	m_cubie_states.at(DLB) = m_cubie_states.at(ULB);
	m_cubie_states.at(ULB) = m_cubie_states.at(URB);
	m_cubie_states.at(URB) = m_cubie_states.at(DRB);
	m_cubie_states.at(DRB) = temp;

	//now rotate the cubies
    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLB:
            case URB:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = 1;
                break;

            case ULB:
            case DRB:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::BackCounter()
{
	//Back face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLB);
	m_cubie_positions.at(DLB) = m_cubie_positions.at(DRB);
	m_cubie_positions.at(DRB) = m_cubie_positions.at(URB);
	m_cubie_positions.at(URB) = m_cubie_positions.at(ULB);
	m_cubie_positions.at(ULB) = temp;

	//states have to move too
	temp = m_cubie_states.at(DLB);
	m_cubie_states.at(DLB) = m_cubie_states.at(DRB);
	m_cubie_states.at(DRB) = m_cubie_states.at(URB);
	m_cubie_states.at(URB) = m_cubie_states.at(ULB);
	m_cubie_states.at(ULB) = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLB:
            case URB:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = 1;
                break;

            case ULB:
            case DRB:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::Back180()
{
    //Back face, 180 degree rotation
    BackCounter();
    BackCounter();
}

void Cube2::DownClock()
{
	//  down face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLB);
	m_cubie_positions.at(DLB) = m_cubie_positions.at(DRB);
	m_cubie_positions.at(DRB) = m_cubie_positions.at(DRF);
	m_cubie_positions.at(DRF) = m_cubie_positions.at(DLF);
	m_cubie_positions.at(DLF) = temp;

	//states have to follow
	temp = m_cubie_states.at(DLB);
	m_cubie_states.at(DLB) = m_cubie_states.at(DRB);
	m_cubie_states.at(DRB) = m_cubie_states.at(DRF);
	m_cubie_states.at(DRF) = m_cubie_states.at(DLF);
	m_cubie_states.at(DLF) = temp;

	//no rotations of cubies occurs

	m_oriented = false;
	m_CubieTurnsCurrent = false;
}

void Cube2::DownCounter()
{
	//  down face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLB);
	m_cubie_positions.at(DLB) = m_cubie_positions.at(DLF);
	m_cubie_positions.at(DLF) = m_cubie_positions.at(DRF);
	m_cubie_positions.at(DRF) = m_cubie_positions.at(DRB);
	m_cubie_positions.at(DRB) = temp;

	//states follow
	temp = m_cubie_states.at(DLB);
	m_cubie_states.at(DLB) = m_cubie_states.at(DLF);
	m_cubie_states.at(DLF) = m_cubie_states.at(DRF);
	m_cubie_states.at(DRF) = m_cubie_states.at(DRB);
	m_cubie_states.at(DRB) = temp;

	//no rotations of cubies occurs

	m_oriented = false;
	m_CubieTurnsCurrent = false;
}

void Cube2::Down180()
{
    //Down face, 180 degree rotation
    DownCounter();
    DownCounter();
}

void Cube2::FrontClock()
{
	//  front face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLF);
	m_cubie_positions.at(DLF) = m_cubie_positions.at(DRF);
	m_cubie_positions.at(DRF) = m_cubie_positions.at(URF);
	m_cubie_positions.at(URF) = m_cubie_positions.at(ULF);
	m_cubie_positions.at(ULF) = temp;

	//states follow
	temp = m_cubie_states.at(DLF);
	m_cubie_states.at(DLF) = m_cubie_states.at(DRF);
	m_cubie_states.at(DRF) = m_cubie_states.at(URF);
	m_cubie_states.at(URF) = m_cubie_states.at(ULF);
	m_cubie_states.at(ULF) = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the front face
        switch(temp)
        {
            case DLF:
            case URF:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;

            case ULF:
            case DRF:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = 1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::FrontCounter()
{
	//  front face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLF);
	m_cubie_positions.at(DLF) = m_cubie_positions.at(ULF);
	m_cubie_positions.at(ULF) = m_cubie_positions.at(URF);
	m_cubie_positions.at(URF) = m_cubie_positions.at(DRF);
	m_cubie_positions.at(DRF) = temp;

	//states follow
	temp = m_cubie_states.at(DLF);
	m_cubie_states.at(DLF) = m_cubie_states.at(ULF);
	m_cubie_states.at(ULF) = m_cubie_states.at(URF);
	m_cubie_states.at(URF) = m_cubie_states.at(DRF);
	m_cubie_states.at(DRF) = temp;


    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLF:
            case URF:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;

            case ULF:
            case DRF:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = +1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::Front180()
{
    //Front face, 180 degree rotation
    FrontCounter();
    FrontCounter();
}

void Cube2::LeftClock()
{
	//  left face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLF);
	m_cubie_positions.at(DLF) = m_cubie_positions.at(ULF);
	m_cubie_positions.at(ULF) = m_cubie_positions.at(ULB);
	m_cubie_positions.at(ULB) = m_cubie_positions.at(DLB);
	m_cubie_positions.at(DLB) = temp;

	//states follow
	temp = m_cubie_states.at(DLF);
	m_cubie_states.at(DLF) = m_cubie_states.at(ULF);
	m_cubie_states.at(ULF) = m_cubie_states.at(ULB);
	m_cubie_states.at(ULB) = m_cubie_states.at(DLB);
	m_cubie_states.at(DLB) = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLF:
            case ULB:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = +1;
                break;

            case ULF:
            case DLB:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::LeftCounter()
{
	//  left face, counter clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DLF);
	m_cubie_positions.at(DLF) = m_cubie_positions.at(DLB);
	m_cubie_positions.at(DLB) = m_cubie_positions.at(ULB);
	m_cubie_positions.at(ULB) = m_cubie_positions.at(ULF);
	m_cubie_positions.at(ULF) = temp;

	//states follow
	temp = m_cubie_states.at(DLF);
	m_cubie_states.at(DLF) = m_cubie_states.at(DLB);
	m_cubie_states.at(DLB) = m_cubie_states.at(ULB);
	m_cubie_states.at(ULB) = m_cubie_states.at(ULF);
	m_cubie_states.at(ULF) = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DLF:
            case ULB:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = 1;
                break;

            case ULF:
            case DLB:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::Left180()
{
    //Left face, 180 degree rotation
    LeftCounter();
    LeftCounter();
}

void Cube2::RightClock()
{
	//  right face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DRF);
	m_cubie_positions.at(DRF) = m_cubie_positions.at(DRB);
	m_cubie_positions.at(DRB) = m_cubie_positions.at(URB);
	m_cubie_positions.at(URB) = m_cubie_positions.at(URF);
	m_cubie_positions.at(URF) = temp;

	//states follow
	temp = m_cubie_states.at(DRF);
	m_cubie_states.at(DRF) = m_cubie_states.at(DRB);
	m_cubie_states.at(DRB) = m_cubie_states.at(URB);
	m_cubie_states.at(URB) = m_cubie_states.at(URF);
	m_cubie_states.at(URF) = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DRF:
            case URB:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;

            case URF:
            case DRB:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = 1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::RightCounter()
{
	//  right face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(DRF);
	m_cubie_positions.at(DRF) = m_cubie_positions.at(URF);
	m_cubie_positions.at(URF) = m_cubie_positions.at(URB);
	m_cubie_positions.at(URB) = m_cubie_positions.at(DRB);
	m_cubie_positions.at(DRB) = temp;


    //states follow
    temp = m_cubie_states.at(DRF);
	m_cubie_states.at(DRF) = m_cubie_states.at(URF);
	m_cubie_states.at(URF) = m_cubie_states.at(URB);
	m_cubie_states.at(URB) = m_cubie_states.at(DRB);
	m_cubie_states.at(DRB) = temp;

    for (temp=0; temp<8; temp++) //step through all the cubies
    {
        //we're only interested in those on the back face
        switch(temp)
        {
            case DRF:
            case URB:
                m_cubie_states.at(temp) += 1;
                if ( m_cubie_states.at(temp) == 2 )
                    m_cubie_states.at(temp) = -1;
                break;

            case URF:
            case DRB:
                m_cubie_states.at(temp) -= 1;
                if ( m_cubie_states.at(temp) == -2 )
                    m_cubie_states.at(temp) = 1;
                break;
        }
    }

    m_oriented = false;
    m_CubieTurnsCurrent = false;
}

void Cube2::Right180()
{
    //Right face, 180 degree rotation
    RightCounter();
    RightCounter();
}

void Cube2::UpClock()
{
	//  up face, clockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(URF);
	m_cubie_positions.at(URF) = m_cubie_positions.at(URB);
	m_cubie_positions.at(URB) = m_cubie_positions.at(ULB);
	m_cubie_positions.at(ULB) = m_cubie_positions.at(ULF);
	m_cubie_positions.at(ULF) = temp;

	//states follow
	temp = m_cubie_states.at(URF);
	m_cubie_states.at(URF) = m_cubie_states.at(URB);
	m_cubie_states.at(URB) = m_cubie_states.at(ULB);
	m_cubie_states.at(ULB) = m_cubie_states.at(ULF);
	m_cubie_states.at(ULF) = temp;

	//no rotations of cubies occurs

	m_oriented = false;
	m_CubieTurnsCurrent = false;
}

void Cube2::UpCounter()
{
	//  up face, counterclockwise rotation
	int temp;

	//move the cubies affected by the rotation
	temp = m_cubie_positions.at(URF);
	m_cubie_positions.at(URF) = m_cubie_positions.at(ULF);
	m_cubie_positions.at(ULF) = m_cubie_positions.at(ULB);
	m_cubie_positions.at(ULB) = m_cubie_positions.at(URB);
	m_cubie_positions.at(URB) = temp;

	//states follow
	temp = m_cubie_states.at(URF);
	m_cubie_states.at(URF) = m_cubie_states.at(ULF);
	m_cubie_states.at(ULF) = m_cubie_states.at(ULB);
	m_cubie_states.at(ULB) = m_cubie_states.at(URB);
	m_cubie_states.at(URB) = temp;

	//no rotations of cubies occurs

	m_oriented = false;
	m_CubieTurnsCurrent = false;
}

void Cube2::Up180()
{
    //up face, 180 degree rotation
    UpCounter();
    UpCounter();
}

void Cube2::Display()
{
    for ( long x=0; x<8; x++)
    {
        switch (x)
        {
            case URF:
                std::cout << "URF: ";
                break;

            case ULF:
                std::cout << "ULF: ";
                break;

            case URB:
                std::cout << "URB: ";
                break;

            case ULB:
                std::cout << "ULB: ";
                break;

            case DRF:
                std::cout << "DRF: ";
                break;

            case DRB:
                std::cout << "DRB: ";
                break;

            case DLF:
                std::cout << "DLF: ";
                break;

            case DLB:
                std::cout << "DLB: ";
                break;
        }

        switch (m_cubie_positions.at(x))
        {
            case URF:
                std::cout << "URF (BRG) - ";
                break;

            case ULF:
                std::cout << "ULF (BRY) - ";
                break;

            case URB:
                std::cout << "URB (BOG) - ";
                break;

            case ULB:
                std::cout << "ULB (BOY) - ";
                break;

            case DRF:
                std::cout << "DRF (WRG) - ";
                break;

            case DRB:
                std::cout << "DRB (WOG) - ";
                break;

            case DLF:
                std::cout << "DLF (WRY) - ";
                break;

            case DLB:
                std::cout << "DLB (WOY) - ";
                break;

            default:
                std::cout << "ERROR ";
        }

        switch ( m_cubie_states.at(x) )
        {
            case -1:
                std::cout << "counter" << std::endl;
                break;

            case 0:
                std::cout << "neutral" << std::endl;
                break;

            case 1:
                std::cout <<"clock" << std::endl;
                break;

            default:
                std::cout << "ERROR " << m_cubie_states.at(x) << std::endl;
        }
    }
}

void Cube2::Orient()
{
    /*  Put the cube into the "proper" orientation
        I have arbitraririly defined this as having the down(white)-left(yellow)-front(red) (DLF/WYR) cubie
        in the proper location/orientation.  This simplifies solving the cube.
    */
    long horizontal, frontal, saggital;
    bool oriented=false;

    for (horizontal=0; horizontal<4; horizontal++)
    {
        for (frontal=0; frontal<4; frontal++)
        {
            for (saggital=0; saggital<4; saggital++)
            {
                //are we oriented correctly?
                if ( m_cubie_positions.at(DLF) == DLF && m_cubie_states.at(DLF) == 0 )
                {
                    oriented=true;
                    break;
                }
                //no match, so rotate the saggital plane
                LeftClock();
                RightCounter();
            }
            if ( oriented )
                break;
            else
            {
                //no match, so rotate the frontal plane
                FrontCounter();
                BackClock();
            }
        }
        if ( oriented )
            break;
        else
        {
            //no match, so rotate the horizontal plane
            UpCounter();
            DownClock();
        }
    }

    //now set the UID
    GenerateUID();

    m_oriented = true;
    m_CubieTurnsCurrent = false;
}


void Cube2::GenerateUID()
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
        if ( m_cubie_positions[cubie] ==  DRF )
            m_bitID += "001";
        else if ( m_cubie_positions[cubie] ==  DLB )
            m_bitID += "010";
        else if ( m_cubie_positions[cubie] ==  DRB )
            m_bitID += "011";
        else if ( m_cubie_positions[cubie] ==  ULF )
            m_bitID += "100";
        else if ( m_cubie_positions[cubie] ==  URF )
            m_bitID += "101";
        else if ( m_cubie_positions[cubie] ==  ULB )
            m_bitID += "110";
        else if ( m_cubie_positions[cubie] ==  URB )
            m_bitID += "111";

        if ( m_cubie_states[cubie] == 0 ) //neutral rotation
            m_bitID += "00";
        else if ( m_cubie_states[cubie] == 1 )
            m_bitID += "01";
        else if ( m_cubie_states[cubie] == -1 )
            m_bitID += "10";
    }

    //add on the last cubie's orientation
    if ( m_cubie_states[7] == 0 ) //neutral rotation
        m_bitID += "00";
    else if ( m_cubie_states[7] == 1 ) //clockwise
        m_bitID += "01";
    else if ( m_cubie_states[7] == -1 )  //counter-clockwise
        m_bitID += "10";

    //reset the ID
    m_ID = 0;

    //now we've got the binary, so convert it to an unsigned int
    for (unsigned int x = 0; x < m_bitID.length(); x++)
    {
        if ( (m_bitID.substr(x,1)).compare("1") == 0 )
            m_ID += static_cast<unsigned int>(pow(2.0,(31-x)));  //(31-x) included to make sure each bit is promoted to the right power of 2
    }
}

unsigned int Cube2::GetPairID(int cubie1, int cubie2)
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

    //properly orient the cube
    if ( !m_oriented )
        Orient();

    //find the first sought cubie
    pairID.at(0) = cubie1; //sought cubie #1
    for (unsigned int cubie=1; cubie<8; cubie++)
    {
        if ( m_cubie_positions.at(cubie) == cubie1 )
        {
            pairID.at(1) = cubie;
            if ( m_cubie_states.at(cubie) == -1 )
                pairID.at(2) = 2;
            else
                pairID.at(2) = m_cubie_states.at(cubie);

            break;
        }
    }

    //now the second sought cubie
    pairID.at(3) = cubie2; //sought cubie #2
    for (unsigned int cubie=1; cubie<8; cubie++)
    {
        if ( m_cubie_positions.at(cubie) == cubie2 )
        {
            pairID.at(4) = cubie;
            if ( m_cubie_states.at(cubie) == -1 )
                pairID.at(5) = 2;
            else
                pairID.at(5) = m_cubie_states.at(cubie);

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
unsigned int Cube2::GetTopID()
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

    //make sure the cube is properly oriented
    if ( !m_oriented )
        Orient();

    for (int cubie=1; cubie<8; cubie++)
    {
        found = false;
        switch ( m_cubie_positions.at(cubie) )
        {
            case ULF:
            case URF:
            case ULB:
            case URB:
                topID.at( m_cubie_positions.at(cubie)-4 ) = (cubie << 2);
                found = true;
        }

        if ( found )
        {
            if ( m_cubie_states.at(cubie) == 0 ) //neutral rotation
            {
                topID.at(m_cubie_positions.at(cubie)-4) = topID.at(m_cubie_positions.at(cubie)-4) | 0; //effectively leaves it with zeros at the tail
            }
            else if ( m_cubie_states[cubie] == 1 ) //clockwise
            {
                topID.at(m_cubie_positions.at(cubie)-4) = topID.at(m_cubie_positions.at(cubie)-4) | 1;
            }
            else if ( m_cubie_states[cubie] == -1 ) //counter-clockwise
             {
                topID.at(m_cubie_positions.at(cubie)-4) = topID.at(m_cubie_positions.at(cubie)-4) | 2;
             }
        }
    }

    for (int cubie=0; cubie<4; cubie++)
    {
        ID = ID + ( topID.at(cubie) << ( cubie*5 ));
    }

    return ID;
}

unsigned int Cube2::GetBottomID()
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

    //make sure the cube is properly oriented
    if ( !m_oriented )
        Orient();

    for (int cubie=1; cubie<8; cubie++)
    {
        found = false;
        switch ( m_cubie_positions.at(cubie) )
        {
            case DRF:
            case DLB:
            case DRB:
                botID.at( m_cubie_positions.at(cubie)-1 ) = (cubie << 2);
                found = true;
        }

        if ( found )
        {
            if ( m_cubie_states.at(cubie) == 0 ) //neutral rotation
            {
                botID.at(m_cubie_positions.at(cubie)-1) = botID.at(m_cubie_positions.at(cubie)-1) | 0; //effectively leaves it with zeros at the tail
            }
            else if ( m_cubie_states[cubie] == 1 ) //clockwise
            {
                botID.at(m_cubie_positions.at(cubie)-1) = botID.at(m_cubie_positions.at(cubie)-1) | 1;
            }
            else if ( m_cubie_states[cubie] == -1 ) //counter-clockwise
             {
                botID.at(m_cubie_positions.at(cubie)-1) = botID.at(m_cubie_positions.at(cubie)-1) | 2;
             }
        }
    }

    for (int cubie=0; cubie<3; cubie++)
    {
        ID = ID + ( botID.at(cubie) << ( cubie*5 ));
    }

    return ID;
}

bool Cube2::CheckCubie(int cubie)
{
    /*  assess the position and state of the passed cubie  */
    /*  return true if the cubie is in its solved position  */
    if ( !m_oriented )
        Orient();

    return ( m_cubie_positions.at(cubie) == cubie && m_cubie_states.at(cubie) == 0 );
}

void Cube2::Randomize(int turns)
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

bool Cube2::operator==( Cube2 &sn )
{
    GenerateUID();
    sn.GenerateUID();

    if ( GetUID() == sn.GetUID() )
        return true;
    else
        return false;
}

float Cube2::H()
{
    return m_H;
};

void Cube2::Manipulate(std::vector<std::string> &sol)
{
    for (unsigned int s=0; s<sol.size(); s++)
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
