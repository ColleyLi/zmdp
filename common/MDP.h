/********** tell emacs we use -*- c++ -*- style comments *******************
 $Revision: 1.3 $  $Author: trey $  $Date: 2006-02-01 01:09:37 $
   
 @file    MDP.h
 @brief   No brief

 Copyright (c) 2005, Trey Smith. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * The software may not be sold or incorporated into a commercial
   product without specific prior written permission.
 * The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 ***************************************************************************/

#ifndef INCMDP_h
#define INCMDP_h

#include <iostream>
#include <string>
#include <vector>

#include "zmdpCommonDefs.h"
#include "zmdpCommonTypes.h"

using namespace sla;

namespace zmdp {

struct AbstractBound;

// Represents an MDP where state is continuous, time is discrete,
// actions are discrete, and the possible outcomes of an action form a
// discrete probability distribution.  This data structure can
// represent the belief MDP corresponding to a discrete POMDP.
struct MDP {
  int numStateDimensions, numActions, numOutcomes;
  double discount;

  virtual ~MDP(void) {}

  int getNumStateDimensions(void) const { return numStateDimensions; }
  int getNumActions(void) const { return numActions; }
  int getNumOutcomes(void) const { return numOutcomes; }
  double getDiscount(void) const { return discount; }

  // returns the initial state
  virtual const state_vector& getInitialState(void) const = 0;

  // returns true if state is terminal
  virtual bool getIsTerminalState(const state_vector& s) const = 0;

  // sets result to be the vector of outcome probabilities when from
  // state s action a is selected
  virtual outcome_prob_vector& getOutcomeProbVector(outcome_prob_vector& result,
						    const state_vector& s, int a)
       const = 0;

  // sets result to be the next state when from state s action a is
  // selected and outcome o results
  virtual state_vector& getNextState(state_vector& result, const state_vector& s, int a,
				     int o) const = 0;

  // returns the expected immediate reward when from state s action a is selected
  virtual double getReward(const state_vector& s, int a) const = 0;

  // returns a new lower bound or upper bound that is valid for
  // this MDP.  notes:
  // * the resulting bound must be initialized before it is used, and
  //   initialization may take significant computation time.
  // * some types of MDP might not define one of these bounds, which
  //   could be signaled by returning NULL.  so far this hasn't been
  //   explored.
  virtual AbstractBound* newLowerBound(void) const = 0;
  virtual AbstractBound* newUpperBound(void) const = 0;
};

}; // namespace zmdp

#endif // INCMDP_h

/***************************************************************************
 * REVISION HISTORY:
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2006/01/31 20:09:11  trey
 * added generic mechanism for getting bounds
 *
 * Revision 1.1  2006/01/28 03:01:05  trey
 * initial check-in
 *
 *
 ***************************************************************************/
