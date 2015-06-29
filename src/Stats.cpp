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
  rzrCount      = 0;
  rzrCutoffs    = 0;
  iidCount      = 0;
  iidBeta       = 0;
  nullMoves     = 0;
  nmCutoffs     = 0;
  nmThreats     = 0;
  lateMoves     = 0;
  lmCandidates  = 0;
  lmReductions  = 0;
  lmDoubleRed   = 0;
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
  rzrCount      += other.rzrCount;
  rzrCutoffs    += other.rzrCutoffs;
  iidCount      += other.iidCount;
  iidBeta       += other.iidBeta;
  nullMoves     += other.nullMoves;
  nmCutoffs     += other.nmCutoffs;
  nmThreats     += other.nmThreats;
  lateMoves     += other.lateMoves;
  lmCandidates  += other.lmCandidates;
  lmReductions  += other.lmReductions;
  lmDoubleRed   += other.lmDoubleRed;
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
  avg.rzrCount      = Avg(rzrCount,     statCount);
  avg.rzrCutoffs    = Avg(rzrCutoffs,   statCount);
  avg.iidCount      = Avg(iidCount,     statCount);
  avg.iidBeta       = Avg(iidBeta,      statCount);
  avg.nullMoves     = Avg(nullMoves,    statCount);
  avg.nmCutoffs     = Avg(nmCutoffs,    statCount);
  avg.nmThreats     = Avg(nmThreats,    statCount);
  avg.lateMoves     = Avg(lateMoves,    statCount);
  avg.lmCandidates  = Avg(lmCandidates, statCount);
  avg.lmReductions  = Avg(lmReductions, statCount);
  avg.lmDoubleRed   = Avg(lmDoubleRed,  statCount);
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
             << rzrCutoffs << " confirmed ("
             << Percent(rzrCutoffs, rzrCount) << "%)";
  }

  if (nullMoves) {
    Output() << nullMoves << " null moves, "
             << nmCutoffs << " cutoffs ("
             << Percent(nmCutoffs, nullMoves) << "%), "
             << nmThreats << " threats ("
             << Percent(nmThreats, nullMoves) << "%)";
  }

  if (iidCount) {
    Output() << iidCount << " IID searches, " << iidBeta << " failed high ("
             << Percent(iidBeta, iidCount) << "%)";
  }

  Output() << lateMoves << " late moves ("
           << Percent(lateMoves, execs) << "%), "
           << lmAlphaIncs << " increase alpha ("
           << Percent(lmAlphaIncs, lateMoves) << "%)";

  if (lmCandidates) {
    Output() << lmCandidates << " lmr candidates ("
             << Percent(lmCandidates, lateMoves) << "%), "
             << lmReductions << " reduced ("
             << Percent(lmReductions, lateMoves) << "%), "
             << lmDoubleRed << " double ("
             << Percent(lmDoubleRed, lmReductions) << "%)";
    Output() << lmResearches << " lmr alpha increases ("
             << Percent(lmResearches, lmReductions) << "%), "
             << lmConfirmed << " confirmed ("
             << Percent(lmConfirmed, lmResearches) << "%)";
  }
}

} // namespace bitfoot
