//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>
//----------------------------------------------------------------------------

#include "senjo/Output.h"
#include "Stats.h"

using namespace senjo;

namespace bitfoot
{

//----------------------------------------------------------------------------
void Stats::Clear()
{
  statCount     = 1;
  snodes        = 0;
  qnodes        = 0;
  chkExts       = 0;
  threatExts    = 0;
  oneReplyExts  = 0;
  hashExts      = 0;
  execs         = 0;
  qexecs        = 0;
  deltaCount    = 0;
  futility      = 0;
  rzrCount      = 0;
  rzrEarlyOut   = 0;
  rzrCutoffs    = 0;
  iidCount      = 0;
  nullMoves     = 0;
  nmCutoffs     = 0;
  nmThreats     = 0;
  nmrCandidates = 0;
  nmReductions  = 0;
  nmrBackfires  = 0;
  lateMoves     = 0;
  lmCandidates  = 0;
  lmReductions  = 0;
  lmResearches  = 0;
  lmConfirmed   = 0;
  lmAlphaIncs   = 0;
}

//----------------------------------------------------------------------------
Stats& Stats::operator+=(const Stats& other) {
  statCount     += 1;
  snodes        += other.snodes;
  qnodes        += other.qnodes;
  chkExts       += other.chkExts;
  threatExts    += other.threatExts;
  oneReplyExts  += other.oneReplyExts;
  hashExts      += other.hashExts;
  execs         += other.execs;
  qexecs        += other.qexecs;
  deltaCount    += other.deltaCount;
  futility      += other.futility;
  rzrCount      += other.rzrCount;
  rzrEarlyOut   += other.rzrEarlyOut;
  rzrCutoffs    += other.rzrCutoffs;
  iidCount      += other.iidCount;
  nullMoves     += other.nullMoves;
  nmCutoffs     += other.nmCutoffs;
  nmThreats     += other.nmThreats;
  nmrCandidates += other.nmrCandidates;
  nmReductions  += other.nmReductions;
  nmrBackfires  += other.nmrBackfires;
  lateMoves     += other.lateMoves;
  lmCandidates  += other.lmCandidates;
  lmReductions  += other.lmReductions;
  lmResearches  += other.lmResearches;
  lmConfirmed   += other.lmConfirmed;
  lmAlphaIncs   += other.lmAlphaIncs;
  return *this;
}

//----------------------------------------------------------------------------
static uint64_t Avg(const uint64_t total, const uint64_t count) {
  if (count) {
    return ((total + (count - 1)) / count);
  }
  return total;
}

//----------------------------------------------------------------------------
Stats Stats::Average() const {
  Stats avg;
  avg.snodes        = Avg(snodes,       statCount);
  avg.qnodes        = Avg(qnodes,       statCount);
  avg.chkExts       = Avg(chkExts,      statCount);
  avg.threatExts    = Avg(threatExts,   statCount);
  avg.oneReplyExts  = Avg(oneReplyExts, statCount);
  avg.hashExts      = Avg(hashExts,     statCount);
  avg.execs         = Avg(execs,        statCount);
  avg.qexecs        = Avg(qexecs,       statCount);
  avg.deltaCount    = Avg(deltaCount,   statCount);
  avg.futility      = Avg(futility,     statCount);
  avg.rzrCount      = Avg(rzrCount,     statCount);
  avg.rzrEarlyOut   = Avg(rzrEarlyOut,  statCount);
  avg.rzrCutoffs    = Avg(rzrCutoffs,   statCount);
  avg.iidCount      = Avg(iidCount,     statCount);
  avg.nullMoves     = Avg(nullMoves,    statCount);
  avg.nmCutoffs     = Avg(nmCutoffs,    statCount);
  avg.nmThreats     = Avg(nmThreats,    statCount);
  avg.nmrCandidates = Avg(nmrCandidates,statCount);
  avg.nmReductions  = Avg(nmReductions, statCount);
  avg.nmrBackfires  = Avg(nmrBackfires, statCount);
  avg.lateMoves     = Avg(lateMoves,    statCount);
  avg.lmCandidates  = Avg(lmCandidates, statCount);
  avg.lmReductions  = Avg(lmReductions, statCount);
  avg.lmResearches  = Avg(lmResearches, statCount);
  avg.lmConfirmed   = Avg(lmConfirmed,  statCount);
  avg.lmAlphaIncs   = Avg(lmAlphaIncs,  statCount);
  return avg;
}

//----------------------------------------------------------------------------
void Stats::Print() {
  if (chkExts || threatExts || oneReplyExts || hashExts) {
    Output() << chkExts << " check extensions, "
             << threatExts << " threat extensions, "
             << oneReplyExts << " one reply extensions, "
             << hashExts << " hashed extensions";
  }

  Output() << execs << " execs, "
           << qexecs << " qexecs (" << Percent(qexecs, execs) << "%)";

  const uint64_t searches = (snodes + qnodes);
  Output() << snodes << " searches (" << Percent(snodes, searches) << "%), "
           << qnodes << " qsearches (" << Percent(qnodes, searches) << "%)";

  if (deltaCount) {
    Output() << deltaCount << " delta pruned ("
             << Percent(deltaCount, qnodes) << "%)";
  }

  if (rzrCount) {
    Output() << rzrCount << " razor attempts, "
             << rzrEarlyOut << " early out ("
             << Percent(rzrEarlyOut, rzrCount) << "%), "
             << rzrCutoffs << " cutoffs ("
             << Percent(rzrCutoffs, rzrCount) << "%)";
  }

  if (futility) {
    Output() << futility << " futility prunings";
  }

  if (nullMoves) {
    Output() << nullMoves << " null moves, "
             << nmCutoffs << " cutoffs ("
             << Percent(nmCutoffs, nullMoves) << "%), "
             << nmThreats << " threats ("
             << Percent(nmThreats, nullMoves) << "%)";
  }

  if (nmrCandidates) {
    Output() << nmrCandidates << " nmr candidates, "
             << nmReductions << " reduced ("
             << Percent(nmReductions, nmrCandidates) << "%), "
             << nmrBackfires << " backfires ("
             << Percent(nmrBackfires, nmReductions) << "%)";
  }

  if (iidCount) {
    Output() << iidCount << " IID searches";
  }

  Output() << lateMoves << " late moves ("
           << Percent(lateMoves, execs) << "%), "
           << lmAlphaIncs << " increase alpha ("
           << Percent(lmAlphaIncs, lateMoves) << "%)";

  if (lmCandidates) {
    Output() << lmCandidates << " lmr candidates ("
             << Percent(lmCandidates, lateMoves) << "%), "
             << lmReductions << " reduced ("
             << Percent(lmReductions, lmCandidates) << "%)";
    Output() << lmResearches << " lmr alpha increases ("
             << Percent(lmResearches, lmReductions) << "%), "
             << lmConfirmed << " confirmed ("
             << Percent(lmConfirmed, lmResearches) << "%)";
  }
}

} // namespace bitfoot
