/**
 * @file TSP.h
 * @brief TSP data
 *
 */

#ifndef TSP_H
#define TSP_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * Class that describes a TSP instance (a cost matrix, nodes are identified by integer 0 ... n-1)
 */
class TSP
{
public:
  TSP() : N(0) , infinite(1e10) { }
  int N; //number of nodes
  std::vector<std::vector<double>> C;
  double infinite; // infinite value (an upper bound on the value of any feasible solution)

  void read(const char* filename)
  {
    std::ifstream in(filename);
    // read size
    in >> N;
    std::cout << "number of nodes n = " << N << std::endl;
    // read costs
    C.resize(N);
    for (int i = 0; i < N; i++) {
      C[i].reserve(N);
      for (int j = 0; j < N; j++) {
        double c;
        in >> c;
        C[i].push_back(c);
      }
    }
    in.close();
    
    // set infinite value
    infinite = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        infinite += C[i][j];
//	std::cout<< "The cost matrix = "<<C[i][j] << std::endl;

      }
    }
    infinite *= 2;
  }

};

#endif /* TSP_H */

