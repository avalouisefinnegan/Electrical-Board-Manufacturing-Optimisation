/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 *
 */

#include "TSPSolver.h"
#include <iostream>

bool TSPSolver::solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol )
{
  try
  {
    bool stop = false;
    int  iter = 0;

    TSPSolution currSol(initSol);
    //double bestValue, currValue;
    //bestValue = currValue = evaluate(currSol,tsp);
    TSPMove move;
    while ( ! stop ) {
                if ( tsp.n < 20 ) currSol.print(); //log current solution (only small instances)
      double bestDecrement = findBestNeighborDecrement(tsp,currSol,move);
                std::cout << "(" << ++iter << "ls) move " << move.substring_init << " , " << move.substring_end << " improves by " << bestDecrement << std::endl;
      if ( bestDecrement < -1e-6 ) {
        currSol = apply2optMove(currSol,move);
        stop = false;
      } else {
        stop = true;
      } 
    }
    bestSol = currSol;
  }
  catch(std::exception& e)
  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    return false;
  }
  return true;
}

//TODO: "internal methods", if any
TSPSolution& TSPSolver::apply2optMove ( TSPSolution& tspSol , const TSPMove& move ) 
{
  TSPSolution tmpSol(tspSol);
  for ( int i = move.substring_init ; i <= move.substring_end ; ++i ) {
    tspSol.sequence[i] = tmpSol.sequence[move.substring_end-(i-move.substring_init)];
  }
  return tspSol;
}

double TSPSolver::findBestNeighborDecrement ( const TSP& tsp , const TSPSolution& currSol , TSPMove& move )
{
  double bestDecrement = tsp.infinite;
  // idx:            0  1  2             n-2  n-1  n
  // city:           0  6  3              9    7   0
  // move 2 -- n-2:     h  i              j    l
  //city: 0 3 5 6 ...  9   8  0
  for ( uint i_subs_init = 1 ; i_subs_init < currSol.sequence.size() - 2 ; i_subs_init++ ) {
    int h = currSol.sequence[i_subs_init-1];
    int i = currSol.sequence[i_subs_init];
    for ( uint i_subs_end = i_subs_init + 1 ; i_subs_end < currSol.sequence.size() - 1 ; ++i_subs_end ) {
      int j = currSol.sequence[i_subs_end];
      int l = currSol.sequence[i_subs_end+1];
      double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                  + tsp.cost[h][j] + tsp.cost[i][l] ;
      if ( neighCostVariation < bestDecrement ) {
        bestDecrement = neighCostVariation;
        move.substring_init = i_subs_init;
        move.substring_end  = i_subs_end;
      }
    }
  }

  /// Exercise: implement a First Improvement exploration strategy
  return bestDecrement;
}



