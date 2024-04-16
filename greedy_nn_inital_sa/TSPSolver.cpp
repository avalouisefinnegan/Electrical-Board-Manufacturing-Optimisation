
/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 *
 */

#include "TSPSolver.h"
#include <iostream>
using namespace std;
#include <cmath>

bool TSPSolver::solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol )
{
  try
  {
    TSPSolution currSol(initSol);
   
    TSPMove move;
    srand(time(NULL));
    currSol = findBestNeighborDecrement(tsp,currSol,move, bestSol);      
    bestSol = currSol;;
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

TSPSolution& TSPSolver::apply2optMoveCurr ( TSPSolution& tspSol , const TSPMove& move )
{
  TSPSolution tmpSol(tspSol);
  for ( int i = move.substring_init_temp ; i <= move.substring_end_temp ; ++i ) {
    tspSol.sequence[i] = tmpSol.sequence[move.substring_end_temp-(i-move.substring_init_temp)];
  }
  return tspSol;
}


double temperature = 1e+06;
//double TSPSolver::findBestNeighborDecrement ( const TSP& tsp ,TSPSolution& currSol , TSPMove& move , TSPSolution& bestSol )
//bool TSPSolver::solve ( const TSP& tsp ,TSPSolution& currSol , TSPMove& move , TSPSolution& bestSol )
TSPSolution& TSPSolver::findBestNeighborDecrement ( const TSP& tsp ,TSPSolution& currSol , TSPMove& move , TSPSolution& bestSol )
{  
  double bestDecrement = tsp.infinite;
  double currentDecrement = tsp.infinite;
  double temperature_cooling = 0.99;
  TSPSolution currbestSol(currSol);

   for (int n = 0; n < 1000  ; n ++) {

     if(n == 0) {
	   for ( uint i_subs_init = 1 ; i_subs_init < currSol.sequence.size() - 2 ; i_subs_init++ ) {
          	int h = currSol.sequence[i_subs_init-1];
          	int i = currSol.sequence[i_subs_init];
          	for ( uint i_subs_end = i_subs_init + 1 ; i_subs_end < currSol.sequence.size() - 1 ; ++i_subs_end ) {
                	int j = currSol.sequence[i_subs_end];
                	int l = currSol.sequence[i_subs_end+1];
    			double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l] + tsp.cost[h][j] + tsp.cost[i][l] ;
                	if ( neighCostVariation < currentDecrement ) {
                		currentDecrement = neighCostVariation;
                		move.substring_init_temp = i_subs_init;
                		move.substring_end_temp  = i_subs_end;}
                	if ( neighCostVariation < bestDecrement ) {
                		bestDecrement = neighCostVariation;
                		move.substring_init = i_subs_init;
                		move.substring_end  = i_subs_end;}
           }}
           currSol = apply2optMoveCurr(currSol,move);
           currbestSol = apply2optMove(currbestSol,move);}	    
    //printf("temperature is %f \n", temperature); 
    int rand_num_start = rand() % (currSol.sequence.size()-2) + 1;
    int rand_num_end = rand_num_start + (rand() % (currSol.sequence.size() - rand_num_start - 1));
    // create a random pertubation or 'kick'
    int h_rand = currSol.sequence[rand_num_start - 1];
    int i_rand = currSol.sequence[rand_num_start];
    int j_rand = currSol.sequence[rand_num_end];
    int l_rand = currSol.sequence[rand_num_end + 1];
    //printf("kick = %d \n",n);
    double perturbneighCostVariation = - tsp.cost[h_rand][i_rand] - tsp.cost[j_rand][l_rand]
                                       + tsp.cost[h_rand][j_rand]  + tsp.cost[i_rand][l_rand] ;
    //printf("costvariation = %f \n", perturbneighCostVariation);
    //printf("bestdecrement = %f \n", bestDecrement);
    //printf("rand_num_start and end %d, %d \n", rand_num_start, rand_num_end);
    if ( (perturbneighCostVariation < currentDecrement) & (i_rand != j_rand)) {
	        printf("kick was better \n");
	    	currentDecrement = perturbneighCostVariation;
                move.substring_init_temp = rand_num_start;
                move.substring_end_temp  = rand_num_end;
		currSol = apply2optMoveCurr(currSol,move);
		for ( uint i_subs_init = 1 ; i_subs_init < currSol.sequence.size() - 2 ; i_subs_init++ ) {
   		  int h = currSol.sequence[i_subs_init-1];
   		  int i = currSol.sequence[i_subs_init];
    		  for ( uint i_subs_end = i_subs_init + 1 ; i_subs_end < currSol.sequence.size() - 1 ; ++i_subs_end ) {
      			int j = currSol.sequence[i_subs_end];
      			int l = currSol.sequence[i_subs_end+1];
      			double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                  + tsp.cost[h][j] + tsp.cost[i][l] ;
      		        
      			if ( neighCostVariation < currentDecrement ) {
			currentDecrement = neighCostVariation;
        		move.substring_init_temp = i_subs_init;
       			move.substring_end_temp  = i_subs_end;
			if ( neighCostVariation < bestDecrement ) {
                        bestDecrement = neighCostVariation;
			move.substring_init = i_subs_init;
                        move.substring_end  = i_subs_end;}
			}} 
   		  currSol = apply2optMoveCurr(currSol,move);
                  currbestSol = apply2optMove(currbestSol,move);
              }}        		
    else { double accceptance_probability =  exp(-1 * fabs(bestDecrement -  perturbneighCostVariation)/temperature);
	  accceptance_probability = accceptance_probability * 100;
          int rand_num_100 = 1+ (rand() % 100);
	  //printf("random integer and acceptance probability %d, %f \n", rand_num_100, accceptance_probability);
          if ((accceptance_probability > rand_num_100) & (i_rand != j_rand) ){
                //printf("kick was worse \n");
		currentDecrement = perturbneighCostVariation;
                move.substring_init_temp = rand_num_start;
                move.substring_end_temp  = rand_num_end;
		//printf("worse kick accepted \n");
		currSol = apply2optMoveCurr(currSol,move);
		  for ( uint i_subs_init = 1 ; i_subs_init < currSol.sequence.size() - 2 ; i_subs_init++ ) {
   			 int h = currSol.sequence[i_subs_init-1];
    			 int i = currSol.sequence[i_subs_init];
    			  for ( uint i_subs_end = i_subs_init + 1 ; i_subs_end < currSol.sequence.size() - 1 ; ++i_subs_end ) {
     				 int j = currSol.sequence[i_subs_end];
     				 int l = currSol.sequence[i_subs_end+1];
      				 double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                 	 + tsp.cost[h][j] + tsp.cost[i][l] ;
      				
      				 if ( neighCostVariation < currentDecrement ) {
       				 currentDecrement = neighCostVariation;
        			 move.substring_init_temp = i_subs_init;
        			 move.substring_end_temp  = i_subs_end;
				 //printf("newbestdecrement after local search is %f \n",currentDecrement);
    		  		}
				 if (neighCostVariation < bestDecrement ) {
				 bestDecrement = neighCostVariation;
                                 move.substring_init = i_subs_init;
                                 move.substring_end  = i_subs_end;
                                }
  	 		 }
  			}currSol = apply2optMoveCurr(currSol,move);
		  	currbestSol = apply2optMove(currbestSol,move);
  	            
                 }
          	temperature =  temperature * temperature_cooling;
   	 	}}	

   return currSol;

}
