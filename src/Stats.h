//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>
//----------------------------------------------------------------------------

#ifndef BITFOOT_STATS_H
#define BITFOOT_STATS_H

#include "senjo/Platform.h"

namespace bitfoot
{

struct Stats
{
  Stats() { Clear(); }

  void Clear();
  void Print();
  Stats Average() const;
  Stats& operator+=(const Stats& other);

  uint64_t snodes;        // Search() calls
  uint64_t qnodes;        // QSearch() calls
  uint64_t chkExts;       // check extensions
  uint64_t threatExts;    // threat extensions
  uint64_t oneReplyExts;  // one reply extensions
  uint64_t hashExts;      // extensions from hash
  uint64_t execs;         // Exec() calls
  uint64_t qexecs;        // delta candidates
  uint64_t deltaCount;    // delta prunings
  uint64_t futility;      // futility prunings
  uint64_t rzrCount;      // razoring attempts
  uint64_t rzrEarlyOut;   // razoring early descent into qsearch
  uint64_t rzrCutoffs;    // successful razorings
  uint64_t iidCount;      // IID searches
  uint64_t nullMoves;     // ExecNullMove() calls
  uint64_t nmCutoffs;     // null moves cutoffs
  uint64_t nmThreats;     // null moves threat detections
  uint64_t nmrCandidates; // null move reduction attempts
  uint64_t nmReductions;  // null move reductions
  uint64_t nmrBackfires;  // null move reduction backfires
  uint64_t lateMoves;     // late move count
  uint64_t lmCandidates;  // lmr candidates
  uint64_t lmReductions;  // late move reductions
  uint64_t lmResearches;  // lmReductions re-searched at full depth
  uint64_t lmConfirmed;   // lmResearches alpha increases confirmed
  uint64_t lmAlphaIncs;   // late moves that increase alpha
  uint64_t statCount;     // number of stats summed into this instance
};

} // namespace bitfoot

#endif // STATS_H
