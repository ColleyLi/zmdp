/********** tell emacs we use -*- c++ -*- style comments *******************
 $Revision: 1.1 $  $Author: trey $  $Date: 2006-10-19 19:32:08 $
   
 @file    StateLog.cc
 @brief   No brief

 Copyright (c) 2006, Trey Smith.

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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>

#include "StateLog.h"
#include "MatrixUtils.h"

using namespace std;
using namespace sla;
using namespace MatrixUtils;

namespace zmdp {

static std::string stripWhiteSpace(const std::string& s)
{
  string::size_type p1, p2;
  p1 = s.find_first_not_of(" \t");
  if (string::npos == p1) {
    return s;
  } else {
    p2 = s.find_last_not_of(" \t")+1;
    return s.substr(p1, p2-p1);
  }
}

StateIndex::StateIndex(int _numStateDimensions) :
  numStateDimensions(_numStateDimensions)
{}

StateIndex::~StateIndex(void)
{
  FOR_EACH (e, entries) {
    delete *e;
  }
}

int StateIndex::getStateId(const state_vector& s)
{
  string hs = hashable(s);
  typeof(lookup.begin()) pr = lookup.find(hs);
  if (lookup.end() == pr) {
    state_vector* sCopyP = new state_vector;
    copy(*sCopyP, s);
    entries.push_back(sCopyP);
    int id = entries.size()-1;
    lookup[hs] = id;
    return id;
  } else {
    return pr->second;
  }
}

void StateIndex::writeToFile(const std::string& outFile) const
{
  std::ofstream out(outFile.c_str());
  if (!out) {
    fprintf(stderr, "ERROR: couldn't open %s for writing: %s\n",
	    outFile.c_str(), strerror(errno));
    exit(EXIT_FAILURE);
  }

  out << "# This file is an index of states (or POMDP beliefs) referenced in other log\n"
      << "# files.  Each state is given a unique id number in the line of the form\n"
      << "# 'state <id>'.  (Note that the id numbers are guaranteed to appear in order\n"
      << "# starting with 0.)  Subsequent lines after each 'state' line specify the\n"
      << "# state as a compressed vector.  Each line has the form '<index> <value>'\n"
      << "# specifying one entry of the vector, and entries that do not appear have\n"
      << "# value 0.\n";

  FOR (i, entries.size()) {
    out << "state " << i << endl;
    const state_vector& v = *entries[i];
    FOR_CV (v) {
      out << CV_INDEX(v) << " " << setprecision(20) << CV_VAL(v) << endl;
    }
  }

  out.close();
}

void StateIndex::readFromFile(const std::string& inFile)
{
  std::ifstream in(inFile.c_str());
  if (!in) {
    fprintf(stderr, "ERROR: couldn't open %s for reading: %s\n",
	    inFile.c_str(), strerror(errno));
    exit(EXIT_FAILURE);
  }

  entries.clear();
  lookup.clear();

  char buf[1024];
  std::string s;
  state_vector accum(numStateDimensions);
  int id;
  double val;
  int lnum = 0;
  while (!in.eof()) {
    in.getline(buf, sizeof(buf));
    lnum++;
    
    // strip whitespace, ignore empty lines and comments
    s = stripWhiteSpace(buf);
    if (0 == s.size()) continue;
    if ('#' == s[0]) continue;

    if (string::npos != s.find("state")) {
      if (accum.filled() > 0) {
	accum.canonicalize();
	getStateId(accum);
	accum.clear();
      }
    } else {
      if (2 != sscanf(s.c_str(), "%d %lf", &id, &val)) {
	fprintf(stderr, "ERROR: %s:%d: syntax error, expected '<index> <val>'\n",
		inFile.c_str(), lnum);
	exit(EXIT_FAILURE);
      }
      accum.push_back(id, val);
    }
  }
}

StateLog::StateLog(StateIndex* _index) :
  index(_index)
{}

void StateLog::addState(const state_vector& s)
{
  entries.push_back(index->getStateId(s));
}

void StateLog::writeToFile(const std::string& outFile) const
{
  std::ofstream out(outFile.c_str());
  if (!out) {
    fprintf(stderr, "ERROR: couldn't open %s for writing: %s\n",
	    outFile.c_str(), strerror(errno));
    exit(EXIT_FAILURE);
  }

  out << "# This file is a log of backups.  All backups performed by the heuristic\n"
      << "# search algorithm appear in order.  The line for a backup gives the\n"
      << "# index number of the backed up state; these index numbers reference states\n"
      << "# listed in the state index file.\n";

  FOR_EACH (e, entries) {
    out << (*e) << endl;
  }

  out.close();
}

void StateLog::readFromFile(const std::string& inFile)
{
  std::ifstream in(inFile.c_str());
  if (!in) {
    fprintf(stderr, "ERROR: couldn't open %s for reading: %s\n",
	    inFile.c_str(), strerror(errno));
    exit(EXIT_FAILURE);
  }

  entries.clear();

  char buf[1024];
  std::string s;
  int id;
  int lnum = 0;
  while (!in.eof()) {
    in.getline(buf, sizeof(buf));
    lnum++;
    
    // strip whitespace, ignore empty lines and comments
    s = stripWhiteSpace(buf);
    if (0 == s.size()) continue;
    if ('#' == s[0]) continue;

    if (1 != sscanf(s.c_str(), "%d", &id)) {
      fprintf(stderr, "ERROR: %s:%d: syntax error, expected '<index>'\n",
	      inFile.c_str(), lnum);
      exit(EXIT_FAILURE);
    }
    entries.push_back(id);
  }
}

}; // namespace zmdp

/***************************************************************************
 * REVISION HISTORY:
 * $Log: not supported by cvs2svn $
 *
 ***************************************************************************/