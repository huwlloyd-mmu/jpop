#include "synthetic.h"
#include <sstream>
#include <iostream>


void SyntheticSimulator::StartRun(unsigned int startVal )
{
    curStep = 0;    
}

void SyntheticSimulator::GetChoices( std::vector<unsigned int>& choices )
{
    choices.clear();
    if ( curStep == n ) // finished
        return;
    for ( unsigned int i = 0; i < m; i++ )
    {
        choices.push_back( curStep*m + i);
    }
}

void SyntheticSimulator::SetChoice( unsigned int choice )
{
    choices[curStep] = choice%m;
    curStep++;
}

int SyntheticSimulator::GetCost( void )
{
    int cost = n;
    for ( unsigned int i = 0; i < n; i++ )
    {
        if ( correct[i] == choices[i] )
            --cost;
    }
    return cost;
}

Simulator* SyntheticSimulator::Copy()
{
    SyntheticSimulator *newSim = new SyntheticSimulator( n, m );
    // copy the solution
    for ( unsigned int i = 0; i < n; i++ )
    {
        newSim->correct[i] = correct[i];
    }
    return newSim;
}

std::string SyntheticSimulator::StateAsString()
{
    ostringstream answerStream;

    for ( unsigned int i = 0; i < n; i++ )
    {
        answerStream << choices[i];
        if ( choices[i] ==  correct[i])
            answerStream << "* ";
        else
            answerStream << " ";
    }
    return answerStream.str();
}

bool SyntheticSimulator::ComponentInSolution(unsigned int component)
{
    unsigned int i, v;
    i = component/m;
    v = component%m;
    return correct[i] == v;
}
