/**
 * @file TSPSolver.h
 * @brief TSP solver (neighborhood search)
 *
 */

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <vector>

#include "TSPSolution.h"

/**
 * Class representing substring reversal move
 */
typedef struct move {
  int			substring_init;
  int			substring_end;
  int                   substring_init_temp;
  int                   substring_end_temp;
} TSPMove;

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TSPSolver
{
public:
  /** Constructor */
  TSPSolver ( ) { }
  /**
   * evaluate a solution
   * @param sol: solution to be evaluated
   * @param TSP: TSP data
   * @return the value of the solution
   */
  double evaluate ( const TSPSolution& sol , const TSP& tsp ) const {
    double total = 0.0;
    for ( uint i = 0 ; i < sol.sequence.size() - 1 ; ++i ) {
      int from = sol.sequence[i]  ;
      int to   = sol.sequence[i+1];
      total += tsp.cost[from][to];
    }
    return total;
  }
  /**
   * initialize a solution as a random sequence by random swaps
   * @param sol solution to be initialized
   * @return true if everything OK, false otherwise
   */
  bool initRnd ( TSPSolution& sol , const TSP& tsp) {

	//initialise
	int size = sol.sequence.size() - 1;
	std::vector<int> checker(size);
	int idx_tmp;

	// populate indexes
	for (int i = 0; i < size; i++){
		if (i == 0){checker[i] = 1;}
		else {checker[i] = 0;}
	}

	// starts from row 0
	int i = 0;
	for (int k = 1; k < size; k++){
		int min_cost = 10000;
		// want to find the index with the smallest cost in row i
		for (int j = 0; j < size; j++){
			if (tsp.cost[i][j] > min_cost || i == j || checker[j] == 1 ){ continue; }
			min_cost = tsp.cost[i][j];
			idx_tmp = j;
		}
				
		checker[idx_tmp] = 1;

		// new loop to look in the i^th row
		i = idx_tmp;
		// overwrite the k^th 
		sol.sequence[k] = idx_tmp;
	}
	std::cout << "### "; sol.print(); std::cout << " ###" << std::endl;
 	return true;
  }
  /**
   * search for a good tour by neighbourhood search
   * @param TSP TSP data
   * @param initSol initial solution
   * @param bestSol best found solution (output)
   * @return true id everything OK, false otherwise
   */
    bool solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol );
 // bool TSPSolver::solve ( const TSP& tsp ,TSPSolution& currSol , TSPMove& move , TSPSolution& bestSol )



protected:
  //TODO: declare here any "internal" method
  TSPSolution& apply2optMove ( TSPSolution& tspSol , const TSPMove& move );
  TSPSolution& findBestNeighborDecrement ( const TSP& tsp , TSPSolution& currSol , TSPMove& move, TSPSolution& bestSol  );
  TSPSolution& apply2optMoveCurr ( TSPSolution& tspSol , const TSPMove& move );
};

#endif /* TSPSOLVER_H */

