/********** tell emacs we use -*- c++ -*- style comments *******************
 $Revision: 1.4 $  $Author: trey $  $Date: 2006-04-28 17:57:41 $
   
 @file    solverUtils.h
 @brief   No brief

 Copyright (c) 2006, Trey Smith. All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License"); you may
 not use this file except in compliance with the License.  You may
 obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 implied.  See the License for the specific language governing
 permissions and limitations under the License.

 ***************************************************************************/

#ifndef INCsolverUtils_h
#define INCsolverUtils_h

#include <iostream>

// search strategies
#include "RTDP.h"
#include "LRTDP.h"
#include "HDP.h"
#include "FRTDP.h"
#include "HSVI.h"

// problem types
#include "RaceTrack.h"
#include "Pomdp.h"
#include "MDPSim.h"
#include "PomdpSim.h"

// value function representations
#include "PointBounds.h"
#include "ConvexBounds.h"

// initialization code
#include "RelaxUBInitializer.h"

namespace zmdp {

struct EnumEntry {
  const char* key;
  int val;
};

enum StrategiesEnum {
  S_RTDP,
  S_LRTDP,
  S_HDP,
  S_HSVI,
  S_FRTDP
};

enum ProbTypesEnum {
  T_RACETRACK,
  T_POMDP
};

enum ValueReprsEnum {
  V_POINT,
  V_CONVEX
};

struct SolverParams {
  const char* cmdName;
  int strategy;
  int probType;
  int valueRepr;
  const char *probName;
  double targetPrecision;
  bool useFastParser;
  bool useHeuristic;
  bool forceLowerBound;
  bool forceUpperBoundActionSelection;

  SolverParams(void);
  void setStrategy(const char* strategyName);
  void setProbType(const char* probTypeName);
  void setValueRepr(const char* valueReprName);
  void inferMissingValues(void);
};

struct SolverObjects {
  Solver* solver;
  IncrementalBounds* bounds;
  MDP* problem;
  AbstractSim* sim;
};

void constructSolverObjects(SolverObjects& obj, const SolverParams& p);

}; // namespace zmdp

#endif // INCsolverUtils_h

/***************************************************************************
 * REVISION HISTORY:
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2006/04/27 23:18:48  trey
 * removed unnecessary #include of Interleave.h
 *
 * Revision 1.2  2006/04/27 23:07:54  trey
 * added bounds to SolverObjects data structure
 *
 * Revision 1.1  2006/04/27 20:20:21  trey
 * factored some interface code out of zmdpBenchmark.cc (moved to solverUtils) so it could be easily shared with zmdpSolve.cc
 *
 *
 ***************************************************************************/