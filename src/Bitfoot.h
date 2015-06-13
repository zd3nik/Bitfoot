//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>, All rights reserved
//----------------------------------------------------------------------------

#ifndef BITFOOT_H
#define BITFOOT_H

#include "senjo/ChessEngine.h"
#include "senjo/Output.h"
#include "HashTable.h"
#include "Diff.h"
#include "Stats.h"

namespace bitfoot
{

class Bitfoot : public senjo::ChessEngine
{
public:
  //--------------------------------------------------------------------------
  // senjo::ChessEngine methods (implemented in Bitfoot.cpp)
  //--------------------------------------------------------------------------
  std::string GetEngineName() const;
  std::string GetEngineVersion() const;
  std::string GetAuthorName() const;
  std::string GetCountryName() const;
  std::string GetFEN() const;
  std::list<senjo::EngineOption> GetOptions() const;
  bool SetEngineOption(const std::string& name, const std::string& value);
  bool IsInitialized() const;
  bool WhiteToMove() const;
  const char* SetPosition(const char* fen);
  const char* MakeMove(const char* str);
  void PrintBoard() const;
  void Initialize();
  void ClearSearchData();
  void PonderHit();
  void Quit();
  void ResetStatsTotals();
  void ShowStatsTotals() const;
  void GetStats(int* depth,
                int* seldepth = NULL,
                uint64_t* nodes = NULL,
                uint64_t* qnodes = NULL,
                uint64_t* msecs = NULL,
                int* movenum = NULL,
                char* move = NULL,
                const size_t movelen = 0) const;

protected:
  //--------------------------------------------------------------------------
  // senjo::ChessEngine methods (implemented in Bitfoot.cpp)
  //--------------------------------------------------------------------------
  uint64_t MyPerft(const int depth);
  std::string MyGo(const int depth,
                   const int movestogo = 0,
                   const uint64_t movetime = 0,
                   const uint64_t wtime = 0, const uint64_t winc = 0,
                   const uint64_t btime = 0, const uint64_t binc = 0,
                   std::string* ponder = NULL);

private:
  static void PrintBitmap(const uint64_t map);

  //--------------------------------------------------------------------------
  static const int      _PIECE_SQR[PieceTypeCount - 2][64];
  static const int      _VALUE_OF[PieceTypeCount];
  static const int      _TOUCH[64];
  static const int      _ATK_WEIGHT[100];
  static const uint64_t _ALL;
  static const uint64_t _CENTER4;
  static const uint64_t _CENTER16;
  static const uint64_t _CORNER12;
  static const uint64_t _DARK;
  static const uint64_t _LIGHT;
  static const uint64_t _MIDDLE;
  static const uint64_t _OUTPOST[2];
  static const uint64_t _FILE[8];
  static const uint64_t _RANK[8];
  static const uint64_t _EAST[64];
  static const uint64_t _NORTH[64];
  static const uint64_t _SOUTH[64];
  static const uint64_t _WEST[64];
  static const uint64_t _PAWN_ATK[2][64];
  static const uint64_t _ADJACENT[64];
  static const uint64_t _KING_ATK[64];
  static const uint64_t _KING_ZONE[64];
  static const uint64_t _KNIGHT_ATK[64];
  static const uint64_t _NORTH_EAST[64];
  static const uint64_t _NORTH_WEST[64];
  static const uint64_t _SOUTH_EAST[64];
  static const uint64_t _SOUTH_WEST[64];
  static const uint64_t _WIDE_NORTH[64];
  static const uint64_t _WIDE_SOUTH[64];

  //--------------------------------------------------------------------------
  static bool                _ext;            // check extensions
  static bool                _iid;            // internal iterative deepening
  static bool                _initialized;    // is the engine initialized?
  static bool                _nmp;            // null move pruning
  static bool                _oneReply;       // one reply extenstions
  static char                _hist[0x10000];  // move performance history
  static int                 _board[64];      // piece positions
  static int                 _contempt;       // contempt for draw value
  static int                 _delta;          // delta pruning margin
  static int                 _depth;          // current root search depth
  static int                 _drawScore[2];   // score for getting a draw
  static int                 _lmr;            // late move reduction
  static int                 _movenum;        // current root search move number
  static int                 _rzr;            // razoring delta
  static int                 _seldepth;       // current selective search depth
  static int                 _tempo;          // tempo bonus for side to move
  static int                 _test;           // new feature test value
  static std::string         _currmove;       // current root search move
  static int64_t             _hashSize;       // transposition table byte size
  static Bitfoot              _node[MaxPlies]; // the node stack
  static std::set<uint64_t>  _seen;           // position keys already seen
  static TranspositionTable  _tt;             // info about visited positions
  static Stats               _stats;          // misc counters
  static Stats               _totalStats;     // sum of misc counters
  static senjo::EngineOption _optHash;        // hash size option
  static senjo::EngineOption _optClearHash;   // clear hash option
  static senjo::EngineOption _optContempt;    // contempt for draw option
  static senjo::EngineOption _optDelta;       // delta pruning margin option
  static senjo::EngineOption _optEXT;         // check extensions option
  static senjo::EngineOption _optIID;         // intrnl iterative deepening opt
  static senjo::EngineOption _optLMR;         // late move reductions option
  static senjo::EngineOption _optNMP;         // null move pruning option
  static senjo::EngineOption _optOneReply;    // one reply extensions option
  static senjo::EngineOption _optRZR;         // razoring delta option
  static senjo::EngineOption _optTempo;       // tempo bonus option
  static senjo::EngineOption _optTest;        // new feature testing option

  //--------------------------------------------------------------------------
  struct PawnInfo {
    uint64_t pawns;
    uint64_t atks;
    uint64_t backward;
    uint64_t connected;
    uint64_t passed;
    uint64_t closed;
    uint64_t behind;
    uint64_t front;
    int      count;
    int      score;
  };

  //--------------------------------------------------------------------------
  // unchanging variables
  //--------------------------------------------------------------------------
  Bitfoot*  parent;
  Bitfoot*  child;
  int      ply;

  //--------------------------------------------------------------------------
  // variables updates by Exec() - in approximate order of update
  //--------------------------------------------------------------------------
  uint64_t pc[PieceTypeCount];
  Move     lastMove;
  int      lastPieceMoved;
  int      capture;
  int      mcount;
  uint64_t effected;
  int      state;
  int      ep;
  int      king[2];
  int      material[2];
  int      sqrVal[2];
  int      rcount;
  uint64_t pieceKey;
  uint64_t positionKey;
  uint64_t kcross[2];
  uint64_t kdiags[2];
  uint64_t chkrs;
  PawnInfo pinfo[2];

  //--------------------------------------------------------------------------
  // variables updated by Evaluate() - which is called by Exec()
  //--------------------------------------------------------------------------
#ifndef NDEBUG
  struct EvalInfo {
    int pc[PieceTypeCount];
    int loose[2];
    int coverage[2];
    int space[2];
  };
  EvalInfo evals;
#endif
  uint64_t atks[PieceTypeCount];
  uint64_t pinned[2];
  uint64_t slider[64];
  int      atkCount[2];
  int      atkScore[2];
  int      standPat;

  //--------------------------------------------------------------------------
  // variables updated by move generator
  //--------------------------------------------------------------------------
  Move     moves[MaxMoves];
  uint64_t movegenKey;
  int      moveStage;
  int      moveCount;
  int      moveIndex;
  int      sortIndex;

  //--------------------------------------------------------------------------
  // variables updated by search methods
  //--------------------------------------------------------------------------
  int  extended;
  int  reduced;
  int  nullMoveOk;
  int  pvCount;
  Move killer[2];
  Move pv[MaxPlies];

  //--------------------------------------------------------------------------
  inline uint64_t Empty()       const { return ~(pc[White] | pc[Black]); }
  inline uint64_t Occupied()    const { return (pc[White] | pc[Black]); }
  inline bool     InCheck()     const { return (state & Check); }

  //--------------------------------------------------------------------------
  inline bool IsDraw() const {
    return ((state & Draw) || (rcount >= 100) || _seen.count(positionKey));
  }

  //--------------------------------------------------------------------------
  inline float RemainingMaterial(const Color color) const {
    return (static_cast<float>(material[color]) / StartMaterial);
  }

  //--------------------------------------------------------------------------
  inline int SquareValue(const int piece, const int sqr) const {
    assert(IS_PIECE(piece));
    assert(IS_SQUARE(sqr));
    assert(piece < King);
    return _PIECE_SQR[piece][sqr];
  }

  //--------------------------------------------------------------------------
  static inline int ValueOf(const int piece) {
    assert(!piece || IS_PIECE(piece));
    return _VALUE_OF[piece];
  }

  //--------------------------------------------------------------------------
  static inline int Touch(const int sqr) {
    assert(IS_SQUARE(sqr));
    return _TOUCH[sqr];
  }

  //--------------------------------------------------------------------------
  inline Color ColorToMove() const {
    return static_cast<Color>(COLOR_OF(state));
  }

  //--------------------------------------------------------------------------
  void SetHashSize(const int64_t mbytes) {
    if (!_tt.Resize(mbytes)) {
      senjo::Output() << "cannot allocate hash table of " << mbytes << " MB";
    }
  }

  //--------------------------------------------------------------------------
  void ClearHash() {
    _tt.Clear();
  }

  //--------------------------------------------------------------------------
  void ClearHistory() {
    memset(_hist, 0, sizeof(_hist));
  }

  //--------------------------------------------------------------------------
  void ClearKillers() {
    killer[0].Clear();
    killer[1].Clear();
    for (int i = 0; i < MaxPlies; ++i) {
      _node[i].killer[0].Clear();
      _node[i].killer[1].Clear();
    }
  }

  //--------------------------------------------------------------------------
  inline void IncHistory(const Move& move, const bool check, const int depth) {
    if (!check && (depth > 0)) {
      const int idx = move.GetHistoryIndex();
      const int val = (_hist[idx] + depth + 2);
      _hist[idx] = static_cast<char>(std::min<int>(val, 40));
    }
  }

  //--------------------------------------------------------------------------
  inline void DecHistory(const Move& move, const bool check) {
    if (!check) {
      const int idx = move.GetHistoryIndex();
      const int val = (_hist[idx] - 1);
      _hist[idx] = static_cast<char>(std::max<int>(val, -2));
    }
  }

  //--------------------------------------------------------------------------
  inline void AddKiller(const Move& move) {
    if (move != killer[0]) {
      killer[1] = killer[0];
      killer[0] = move;
    }
  }

  //--------------------------------------------------------------------------
  inline bool IsKiller(const Move& move) const {
    return ((move == killer[0]) || (move == killer[1]));
  }

  //--------------------------------------------------------------------------
  inline void UpdatePV(const Move& move) {
    pv[0] = move;
    if (child) {
      if ((pvCount = (child->pvCount + 1)) > 1) {
        assert(pvCount <= MaxPlies);
        memcpy((pv + 1), child->pv, (child->pvCount * sizeof(Move)));
      }
    }
    else {
      pvCount = 1;
    }
  }

  //--------------------------------------------------------------------------
  void OutputPV(const int score, const int bound = 0) const {
    if (pvCount > 0) {
      const uint64_t msecs = (senjo::Now() - _startTime);
      senjo::Output out(senjo::Output::NoPrefix);

      const uint64_t nodes = (_stats.snodes + _stats.qnodes);
      out << "info depth " << _depth
          << " seldepth " << _seldepth
          << " nodes " << nodes
          << " time " << msecs
          << " nps " << static_cast<uint64_t>(senjo::Rate(nodes, msecs));

      if (bound) {
        out << " currmovenumber " << _movenum
            << " currmove " << _currmove;
      }

      if (abs(score) < MateScore) {
        out << " score cp " << score;
      }
      else {
        const int count = (Infinity - abs(score));
        const int mate = ((count + 1) / 2);
        out << " score mate " << ((score < 0) ? -mate : mate);
      }

      if (bound) {
        out << ((bound < 0) ? " upperbound" : " lowerbound");
      }
      else {
        out << " pv";
        for (int i = 0; i < pvCount; ++i) {
          const Move& move = pv[i];
          out << ' ' << move.ToString();
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  inline uint64_t EastO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_EAST[sqr] & (pc[White] | pc[Black]));
    return (_EAST[sqr] & BELOW(LOW_BIT(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t EastX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_EAST[sqr] & (pc[White] | pc[Black]));
    return LOW_BIT(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t EastXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_EAST[sqr] & (pc[White] | pc[Black]));
    x = LOW_BIT(x);
    return (x | (_EAST[sqr] & BELOW(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthEastO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH_EAST[sqr] & (pc[White] | pc[Black]));
    return (_NORTH_EAST[sqr] & BELOW(LOW_BIT(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthEastX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH_EAST[sqr] & (pc[White] | pc[Black]));
    return LOW_BIT(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthEastXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH_EAST[sqr] & (pc[White] | pc[Black]));
    x = LOW_BIT(x);
    return (x | (_NORTH_EAST[sqr] & BELOW(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH[sqr] & (pc[White] | pc[Black]));
    return (_NORTH[sqr] & BELOW(LOW_BIT(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthWestO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH_WEST[sqr] & (pc[White] | pc[Black]));
    return (_NORTH_WEST[sqr] & BELOW(LOW_BIT(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthWestX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH_WEST[sqr] & (pc[White] | pc[Black]));
    return LOW_BIT(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthWestXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH_WEST[sqr] & (pc[White] | pc[Black]));
    x = LOW_BIT(x);
    return (x | (_NORTH_WEST[sqr] & BELOW(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH[sqr] & (pc[White] | pc[Black]));
    return LOW_BIT(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t NorthXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_NORTH[sqr] & (pc[White] | pc[Black]));
    x = LOW_BIT(x);
    return (x | (_NORTH[sqr] & BELOW(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthEastO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH_EAST[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (_SOUTH_EAST[sqr] & ABOVE(x));
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthEastX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH_EAST[sqr] & (pc[White] | pc[Black]));
    return HighBit(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthEastXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH_EAST[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (x | (_SOUTH_EAST[sqr] & ABOVE(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (_SOUTH[sqr] & ABOVE(x));
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthWestO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH_WEST[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (_SOUTH_WEST[sqr] & ABOVE(x));
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthWestX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH_WEST[sqr] & (pc[White] | pc[Black]));
    return HighBit(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthWestXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH_WEST[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (x | (_SOUTH_WEST[sqr] & ABOVE(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH[sqr] & (pc[White] | pc[Black]));
    return HighBit(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t SouthXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_SOUTH[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (x | (_SOUTH[sqr] & ABOVE(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t WestO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_WEST[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (_WEST[sqr] & ABOVE(x));
  }

  //--------------------------------------------------------------------------
  inline uint64_t WestX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_WEST[sqr] & (pc[White] | pc[Black]));
    return HighBit(x);
  }

  //--------------------------------------------------------------------------
  inline uint64_t WestXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    uint64_t x = (_WEST[sqr] & (pc[White] | pc[Black]));
    x = HighBit(x);
    return (x | (_WEST[sqr] & ABOVE(x)));
  }

  //--------------------------------------------------------------------------
  inline uint64_t BishopO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthEastO(sqr) | NorthWestO(sqr) |
            SouthEastO(sqr) | SouthWestO(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t BishopX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthEastX(sqr) | NorthWestX(sqr) |
            SouthEastX(sqr) | SouthWestX(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t BishopXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthEastXO(sqr) | NorthWestXO(sqr) |
            SouthEastXO(sqr) | SouthWestXO(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t RookO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthO(sqr) | EastO(sqr) |
            SouthO(sqr) | WestO(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t RookX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthX(sqr) | EastX(sqr) |
            SouthX(sqr) | WestX(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t RookXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthXO(sqr) | EastXO(sqr) |
            SouthXO(sqr) | WestXO(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t QueenO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthO(sqr) | EastO(sqr) |
            SouthO(sqr) | WestO(sqr) |
            NorthEastO(sqr) | NorthWestO(sqr) |
            SouthEastO(sqr) | SouthWestO(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t QueenX(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthX(sqr) | EastX(sqr) |
            SouthX(sqr) | WestX(sqr) |
            NorthEastX(sqr) | NorthWestX(sqr) |
            SouthEastX(sqr) | SouthWestX(sqr));
  }

  //--------------------------------------------------------------------------
  inline uint64_t QueenXO(const int sqr) const {
    assert(IS_SQUARE(sqr));
    return (NorthXO(sqr) | EastXO(sqr) |
            SouthXO(sqr) | WestXO(sqr) |
            NorthEastXO(sqr) | NorthWestXO(sqr) |
            SouthEastXO(sqr) | SouthWestXO(sqr));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool OnlyHasPawns() const {
    return (pc[color] == (pc[color|Pawn] | pc[color|King]));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t BishopsQueens() const {
    return (pc[color|Bishop] | pc[color|Queen]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t MajorPieces() const {
    return (pc[color|Rook] | pc[color|Queen]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t MinorPieces() const {
    return (pc[color|Knight] | pc[color|Bishop]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t MajorsAndMinors() const {
    assert((pc[color] ^ pc[color|Pawn] ^ pc[color|King]) ==
           (MinorPieces<color>() | MajorPieces<color>()));
    return (pc[color] ^ pc[color|Pawn] ^ pc[color|King]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t RooksQueens() const {
    return (pc[color|Rook] | pc[color|Queen]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t Sliders() const {
    return (pc[color|Bishop] | pc[color|Rook] | pc[color|Queen]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t KingLines() const {
    return (kdiags[color] | kcross[color]);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline uint64_t FindPiecesGivingCheck() const {
    return ((_PAWN_ATK[color][king[color]] & pc[(!color)|Pawn]) |
            (_KNIGHT_ATK[king[color]] & pc[(!color)|Knight]) |
            (kdiags[color] & BishopsQueens<!color>()) |
            (kcross[color] & RooksQueens<!color>()));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool AttackedBy(const int sqr) const {
    assert(IS_SQUARE(sqr));
    const uint64_t BQ = (pc[color|Bishop] | pc[color|Queen]);
    const uint64_t RQ = (pc[color|Rook] | pc[color|Queen]);
    return ((_PAWN_ATK[!color][sqr] & pc[color|Pawn]) ||
            (_KNIGHT_ATK[sqr] & pc[color|Knight]) ||
            (_KING_ATK[sqr] & pc[color|King]) ||
            (NorthX(sqr) & RQ) ||
            (EastX(sqr) & RQ) ||
            (NorthEastX(sqr) & BQ) ||
            (NorthWestX(sqr) & BQ) ||
            (WestX(sqr) & RQ) ||
            (SouthX(sqr) & RQ) ||
            (SouthEastX(sqr) & BQ) ||
            (SouthWestX(sqr) & BQ));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool CanCastleKingSide() const {
    static const uint64_t MID = (color ? 0x6000000000000000ULL
                                       : 0x0000000000000060ULL);
    return ((state & (color ? BlackShort : WhiteShort))
            && ((Empty() & MID) == MID)
            && !(atks[!color] & (BIT(king[color]) | MID)));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool CanCastleQueenSide() const {
    static const uint64_t MID = (color ? 0x0E00000000000000ULL
                                       : 0x000000000000000EULL);
    return ((state & (color ? BlackLong : WhiteLong))
            && ((Empty() & MID) == MID)
            && !(atks[!color] & (MID << 1)));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool Pinned(const int from, const int to) const {
    assert(IS_SQUARE(from));
    assert(IS_SQUARE(to));
    assert(from != to);
    return ((BIT(from) & pinned[color]) &&
            (!(BIT(to) & pinned[color]) ||
             (abs(_diff.Dir(from, king[color])) != abs(_diff.Dir(from, to)))));
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool EpPinned(const int from, const int to) const {
    assert(IS_SQUARE(from));
    assert(IS_SQUARE(to));
    assert(from != to);
    const int cap = (to + (color ? North : South));
    assert(from != cap);
    assert(abs(from - cap) == 1);
    if ((BIT(from) | BIT(cap)) & kcross[color]) {
      switch (_diff.Dir(from, king[color])) {
      case West:
        if (from < cap) {
          return ((WestX(from) & BIT(king[color])) &&
                  (EastX(cap) & RooksQueens<!color>()));
        }
        return ((WestX(cap) & BIT(king[color])) &&
                (EastX(from) & RooksQueens<!color>()));
      case East:
        if (from < cap) {
          return ((EastX(cap) & BIT(king[color])) &&
                  (WestX(from) & RooksQueens<!color>()));
        }
        return ((EastX(from) & BIT(king[color])) &&
                (WestX(cap) & RooksQueens<!color>()));
      default:
        break;
      }
    }
    return false;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int GetPins(const int pin_value) {
    pinned[color] = 0;
    int score = 0;
    int sqr;
    uint64_t x;
    uint64_t p = (pc[color] & (kcross[color] | kdiags[color]));
    while (p) {
      PopLowSquare(p, sqr);
      switch (_diff.Dir(king[color], sqr)) {
      case SouthWest:
        if ((x = SouthWestXO(sqr)) & BishopsQueens<!color>()) {
          pinned[color] |= (x | (kdiags[color] & _SOUTH_WEST[king[color]]));
          score += pin_value;
        }
        break;
      case South:
        if ((x = SouthXO(sqr)) & RooksQueens<!color>()) {
          pinned[color] |= (x | (kcross[color] & _SOUTH[king[color]]));
          score += pin_value;
        }
        break;
      case SouthEast:
        if ((x = SouthEastXO(sqr)) & BishopsQueens<!color>()) {
          pinned[color] |= (x | (kdiags[color] & _SOUTH_EAST[king[color]]));
          score += pin_value;
        }
        break;
      case West:
        if ((x = WestXO(sqr)) & RooksQueens<!color>()) {
          pinned[color] |= (x | (kcross[color] & _WEST[king[color]]));
          score += pin_value;
        }
        break;
      case East:
        if ((x = EastXO(sqr)) & RooksQueens<!color>()) {
          pinned[color] |= (x | (kcross[color] & _EAST[king[color]]));
          score += pin_value;
        }
        break;
      case NorthEast:
        if ((x = NorthEastXO(sqr)) & BishopsQueens<!color>()) {
          pinned[color] |= (x | (kdiags[color] & _NORTH_EAST[king[color]]));
          score += pin_value;
        }
        break;
      case North:
        if ((x = NorthXO(sqr)) & RooksQueens<!color>()) {
          pinned[color] |= (x | (kcross[color] & _NORTH[king[color]]));
          score += pin_value;
        }
        break;
      case NorthWest:
        if ((x = NorthWestXO(sqr)) & BishopsQueens<!color>()) {
          pinned[color] |= (x | (kdiags[color] & _NORTH_WEST[king[color]]));
          score += pin_value;
        }
        break;
      default:
        assert(false);
      }
    }
    return score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int PawnEval() {
    uint64_t p = pc[color|Pawn];
    assert(p);

    uint64_t x;

    // get pawn info, calculate if needed
    // do not update PawnInfo with info relative to pieces other than pawns
    PawnInfo& info = pinfo[color];
    if (info.pawns != p) {
      assert(!info.pawns);
      assert(!info.atks);
      assert(!info.backward);
      assert(!info.connected);
      assert(!info.passed);
      assert(!info.closed);
      assert(!info.behind);
      assert(!info.front);
      assert(!info.score);
      assert(!info.count);

      info.pawns = p;
      info.count = BitCount(p);

      if (color) {
        info.atks = (((p & ~_FILE[0]) >> 9) | ((p & ~_FILE[7]) >> 7));
      }
      else {
        info.atks = (((p & ~_FILE[0]) << 7) | ((p & ~_FILE[7]) << 9));
      }

      while (p) {
        const int sqr = LowSquare(p);
        const uint64_t bit = LOW_BIT(p);
        p ^= bit;

        // keep track of space in front of pawns
        if (!(pc[color|Pawn] & (color ? _SOUTH[sqr] : _NORTH[sqr]))) {
          info.front |= (color ? _SOUTH[sqr] : _NORTH[sqr]);
        }

        // keep track of which files we have pawns on
        info.closed |= _FILE[XC(sqr)];

        // is it passed?
        if (color ? !(_WIDE_SOUTH[sqr] & pc[WhitePawn])
                  : !(_WIDE_NORTH[sqr] & pc[BlackPawn]))
        {
          // scored in PasserEval
          info.passed |= bit;
        }

        // bonus for potential passers
        else if (!(color ? (_SOUTH[sqr] & pc[WhitePawn])
                         : (_NORTH[sqr] & pc[BlackPawn])) &&
                 !(Occupied() & (color ? (bit >> 8) : (bit << 8))))
        {
          x = (color ? ((_WIDE_SOUTH[sqr] ^ _SOUTH[sqr]) & pc[WhitePawn])
                     : ((_WIDE_NORTH[sqr] ^ _NORTH[sqr]) & pc[BlackPawn]));
          if (x) {
            const int count = BitCount(x);
            x = (color ? ((_WIDE_NORTH[sqr] ^ _NORTH[sqr]) & pc[BlackPawn])
                       : ((_WIDE_SOUTH[sqr] ^ _SOUTH[sqr]) & pc[WhitePawn]));
            if (count >= BitCount(x)) {
              info.score += (8 * (color ? (7 - YC(sqr)) : YC(sqr)));
            }
          }
        }

        // is it supported?
        if (pc[color|Pawn] & (_ADJACENT[sqr] | _PAWN_ATK[!color][sqr])) {
          info.connected |= bit;
          info.behind |= (color ? _NORTH[sqr] : _SOUTH[sqr]);

          // will be counted twice if adjacent
          info.score += 4;
        }

        // is it giving support?
        else if (pc[color|Pawn] & _PAWN_ATK[color][sqr]) {
          info.connected |= bit;
          info.behind |= (color ? _NORTH[sqr] : _SOUTH[sqr]);
        }

        // is it backward/isolated?
        if (!(pc[color|Pawn] & (color ? (_WIDE_NORTH[sqr] ^ _NORTH[sqr])
                                      : (_WIDE_SOUTH[sqr] ^ _SOUTH[sqr]))))
        {
          info.backward |= bit;
          if ((x = (pc[(!color)|Pawn] &
                    _PAWN_ATK[color][sqr + (color ? South : North)])))
          {
            // guarded by enemy pawn(s)
            info.score -= (MULTI_BIT(x) ? 16 : 12);
          }
          else if (SquareValue((color|Pawn), sqr) < 12) {
            info.score -= 8;
          }

          // extra penalty if on an open file
          if (!(pc[(!color)|Pawn] & (color ? _SOUTH[sqr] : _NORTH[sqr]))) {
            info.score -= 8;
          }
        }

        // penalty if doubled, more so if backward/isolated
        if (pc[color|Pawn] & (color ? _NORTH[sqr] : _SOUTH[sqr])) {
          info.score -= ((bit & info.backward) ? 40 : 24);
        }
      }
    }

    assert(pinfo[color].pawns == pc[color|Pawn]);
    assert(pinfo[color].backward == (pinfo[color].backward & pc[color|Pawn]));
    assert(pinfo[color].connected == (pinfo[color].connected & pc[color|Pawn]));
    assert(pinfo[color].passed == (pinfo[color].passed & pc[color|Pawn]));
    assert(!(pinfo[color].pawns & ~pinfo[color].closed));
    assert(!(pinfo[color].pawns & pinfo[color].front));
    assert(pinfo[color].count == BitCount(pc[color|Pawn]));

    // track pawn attacks
    atks[color|Pawn] = info.atks;

    // are any pawns menacing the enemy king?
    if (info.atks & _KING_ZONE[king[!color]]) {
      atkCount[color]++;
    }

    // penalty for blocked central pawns on home square
    if (Occupied()
        & (color ? (BIT(D6) | BIT(E6)) : (BIT(D3) | BIT(E3)))
        & (color ? (info.pawns >> 8) : (info.pawns << 8)))
    {
      // do not update PawnInfo struct with info relative to non-pawns
      return (info.score - 16);
    }

    return info.score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int KnightEval() {
    uint64_t p = pc[color|Knight];
    assert(p);

    uint64_t x;
    int score = 0;

    const uint64_t available = ~(pc[color] | atks[(!color)|Pawn] |
        (pc[(!color)|Pawn] & atks[!color]));

    // redundant knights are worth slightly less
    if (MULTI_BIT(p)) {
      score -= 16;
    }

    // loner knights are worth less
    else if (SINGLE_BITX(p) && (p == MajorsAndMinors<color>())) {
      score -= 50;
    }

    while (p) {
      const int sqr = LowSquare(p);
      const uint64_t bit = LOW_BIT(p);
      p ^= bit;

      // update attack map
      atks[color|Knight] |= (x = _KNIGHT_ATK[sqr]);

      // is this piece menacing the enemy king
      if (x & (_KING_ATK[king[!color]] |
               (_KNIGHT_ATK[king[!color]] & ~pc[color])))
      {
        atkCount[color]++;
        atkScore[color] += 20;
      }

      // penalty for zero mobility, more if trapped in the corner
      if (!(x &= available)) {
        if (bit & _CORNER12) {
          score -= 32;
        }
        else {
          score -= 16;
        }
      }
      else {
        // penalty for knight on c3 with pawn on c2 and d file, unless 0-0-0
        if ((sqr == (color ? C6 : C3)) &&
            (pc[color|Pawn] & (color ? BIT(C7) : BIT(C2))) &&
            (_FILE[3] & pc[color|Pawn]) &&
            (XC(king[color]) > 2))
        {
          score -= 10;
        }

        // outpost bonus
        if ((bit & _OUTPOST[color]) &&
            !(pc[(!color)|Pawn] & (color ? (_WIDE_SOUTH[sqr] ^ _SOUTH[sqr])
                                         : (_WIDE_NORTH[sqr] ^ _NORTH[sqr]))))
        {
          // bigger bonus if no opposing minor pieces
          score += (MinorPieces<!color>() ? 8 : 16);

          // more if in front of a backward pawn
          if (bit & (color ? (pinfo[!color].backward << 8)
                           : (pinfo[!color].backward >> 8)))
          {
            score += 8;
          }

          // more if protected by pawn(s)
          if (bit & atks[color|Pawn]) {
            score += 8;
          }
        }
      }
    }

#ifndef NDEBUG
    evals.pc[color|Knight] = score;
#endif
    return score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int BishopEval() {
    uint64_t p = pc[color|Bishop];
    assert(p);

    uint64_t x;
    int count;
    int score = 0;

    const uint64_t available = ~(pc[color] | atks[(!color)|Pawn] |
        (pc[(!color)|Pawn] & atks[!color]));

    // bonus for having bishop pair (increases as pawns come off the board)
    if ((p & _LIGHT) && (p & _DARK)) {
      score += (48 - ((5 * (pinfo[White].count + pinfo[Black].count)) / 3));
    }

    // loner bishops are worth less
    else if (SINGLE_BITX(p) && (p == MajorsAndMinors<color>())) {
      score -= 50;
    }

    while (p) {
      const int sqr = LowSquare(p);
      const uint64_t bit = LOW_BIT(p);
      p ^= bit;

      if (!parent || (effected & (bit | parent->slider[sqr]))) {
        x = BishopXO(sqr);
      }
      else {
        x = parent->slider[sqr];
        assert(x == BishopXO(sqr));
      }

      // update attack map
      slider[sqr] = x;
      atks[color|Bishop] |= x;

      // is this piece menacing the enemy king
      if (x & (_KING_ATK[king[!color]] | (kdiags[!color] & ~pc[color]))) {
        atkCount[color]++;
        atkScore[color] += 20;
      }

      // penalty for zero mobility, more if trapped in the corner
      if (!(x &= available)) {
        if (bit & _CORNER12) {
          score -= 32;
        }
        else {
          score -= 16;
        }
      }
      else {
        // outpost bonus
        if ((bit & _OUTPOST[color]) &&
            !(pc[(!color)|Pawn] & (color ? (_WIDE_SOUTH[sqr] ^ _SOUTH[sqr])
                                         : (_WIDE_NORTH[sqr] ^ _NORTH[sqr]))))
        {
          // bigger bonus if no opposing minor pieces
          score += (MinorPieces<!color>() ? 8 : 16);

          // more if in front of a backward pawn
          if (bit & (color ? (pinfo[!color].backward << 8)
                           : (pinfo[!color].backward >> 8)))
          {
            score += 8;
          }

          // more if protected by pawn(s)
          if (bit & atks[color|Pawn]) {
            score += 8;
          }
        }
        else if ((sqr == (color ? B7 : B2)) &&
                 (pc[color|Pawn] & BIT(color ? B6 : B3)) &&
                 (pc[color|Pawn] & _PAWN_ATK[!color][color ? B6 : B3]))
        {
          // fianchetto bonus
          score += 8;
        }
        else if ((sqr == (color ? G7 : G2)) &&
                 (pc[color|Pawn] & BIT(color ? G6 : G3)) &&
                 (pc[color|Pawn] & _PAWN_ATK[!color][color ? G6 : G3]))
        {
          // fianchetto bonus
          score += 8;
        }
      }

      // penalty for 3 or more pawns on the same color square
      x = (pc[color|Pawn] & ((bit & _LIGHT) ? _LIGHT : _DARK));
      if (MULTI_BIT(x)) {
        if ((count = BitCount(x)) >= 3) {
          score -= (3 * BitCount(x));
          if ((x &= (((x & ~_FILE[7]) << 9) |
                     ((x & ~_FILE[7]) << 7) |
                     ((x & ~_FILE[0]) >> 7) |
                     ((x & ~_FILE[0]) >> 9))))
          {
            // extra penalty when those pawns are connected (make some holes!)
            score -= (6 * BitCount(x));
          }
        }
      }
    }

#ifndef NDEBUG
    evals.pc[color|Bishop] = score;
#endif
    return score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int RookEval() {
    uint64_t p = pc[color|Rook];
    assert(p);

    uint64_t x;
    uint64_t z;
    uint64_t connected;
    int score = 0;

    // penalty for developing rooks before minor pieces
    if (p & (x = (color ? (_ALL >> 16) : (_ALL << 16)))) {
      if ((x = (MinorPieces<color>() & ~x))) {
        score -= (8 * BitCount(x));
      }
    }

    const uint64_t available = ~(pc[color] | atks[(!color)|Pawn] |
        atks[(!color)|Knight] | atks[(!color)|Bishop] |
        ((pc[(!color)|Pawn] | MinorPieces<!color>()) & atks[!color]));

    while (p) {
      const int sqr = LowSquare(p);
      const uint64_t bit = LOW_BIT(p);
      p ^= bit;

      if (!parent || (effected & (bit | parent->slider[sqr]))) {
        x = RookXO(sqr);
      }
      else {
        x = parent->slider[sqr];
        assert(x == RookXO(sqr));
      }

      // update attack map
      slider[sqr] = x;
      atks[color|Rook] |= x;
      connected = (x & MajorPieces<color>());

      // is this piece menacing the enemy king
      if (x & (_KING_ATK[king[!color]] | (kcross[!color] & ~pc[color]))) {
        atkCount[color]++;
        atkScore[color] += 40;
      }

      // bonus for being behind passers
      if (x & (((color ? _SOUTH[sqr] : _NORTH[sqr]) & pinfo[color].passed) |
               ((color ? _NORTH[sqr] : _SOUTH[sqr]) & pinfo[!color].passed)))
      {
        score += 12;
      }

      // bonus for being on an open file
      else if (bit & ~(pinfo[color].closed | pinfo[!color].closed)) {
        if (_FILE[XC(sqr)] & BIT(king[!color])) {
          score += 12;
        }
        else {
          score += 10;
        }
      }

      // bonus for being on half open file
      else if (bit & ~pinfo[color].closed) {
        z = (color ? _SOUTH[sqr] : _NORTH[sqr]);
        if (z & (BIT(king[!color]) | pinfo[!color].backward)) {
          score += 10;
        }
        else {
          score += 8;
        }
      }

      // stuck behind own pawns, can't castle, on wrong side of king?
      else if (!(bit & pinfo[color].front) &&
               !(state & (color ? BlackCastleMask : WhiteCastleMask)) &&
               (pc[color|Pawn] & (color ? _SOUTH[sqr] : _NORTH[sqr])))
      {
        const int rx = XC(sqr);
        const int kx = XC(king[color]);
        if (kx >= 4) {
          // king side
          if (rx >= kx) {
            // worse if not connected to an open file
            if (!(x & _RANK[YC(sqr)] & ~pinfo[color].closed & available)) {
              score -= 50;
            }
            else {
              score -= 20;
            }
            connected = 0ULL;
          }
        }
        else {
          // queen side
          if (rx <= kx) {
            // worse if not connected to an open file
            if (!(x & _RANK[YC(sqr)] & ~pinfo[color].closed & available)) {
              score -= 50;
            }
            else {
              score -= 20;
            }
            connected = 0ULL;
          }
        }
      }

      // bonus for being connected (counted once for each rook)
      if (connected) {
        x &= available;
        if (MULTI_BIT(x)) {
          score += 4;
        }
      }
    }

#ifndef NDEBUG
    evals.pc[color|Rook] = score;
#endif
    return score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int QueenEval() {
    uint64_t p = pc[color|Queen];
    assert(p);

    uint64_t x;
    int score = 0;

    // penalty for developing queens before minor pieces
    if (p & (x = (color ? (_ALL >> 16) : (_ALL << 16)))) {
      if ((x = (MinorPieces<color>() & ~x))) {
        score -= (12 * BitCount(x));
      }
    }

    const uint64_t available = ~(pc[color] | atks[(!color)|Pawn] |
        atks[(!color)|Knight] | atks[(!color)|Bishop] | atks[(!color)|Rook] |
        ((pc[!color] ^ pc[(!color)|Queen] ^ pc[(!color)|King]) & atks[!color]));

    while (p) {
      const int sqr = LowSquare(p);
      const uint64_t bit = LOW_BIT(p);
      p ^= bit;

      if (!parent || (effected & (bit | parent->slider[sqr]))) {
        x = QueenXO(sqr);
      }
      else {
        x = parent->slider[sqr];
        assert(x == QueenXO(sqr));
      }

      // update attack map
      slider[sqr] = x;
      atks[color|Queen] |= x;

      // is this piece menacing the enemy king
      if (x & (_KING_ATK[king[!color]] | (KingLines<!color>() & ~pc[color]))) {
        atkCount[color]++;
        atkScore[color] += 80;
      }

      // penalty for zero mobility, more if trapped in the corner
      if (!(x &= available)) {
        if (bit & _CORNER12) {
          score -= 32;
        }
        else {
          score -= 16;
        }
      }
    }

#ifndef NDEBUG
    evals.pc[color|Queen] = score;
#endif
    return score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int KingEval() {
    const int sqr = king[color];
    assert(_board[sqr] == (color|King));
    assert(!atks[color|King]);

    int score = 0;
    int chksqr;
    uint64_t p;
    uint64_t w;
    uint64_t z;

    // apply king piece/square value here instead of Exec()
    const float ratio = RemainingMaterial(!color);
    const float midgame = (ratio * _PIECE_SQR[0][sqr]);
    const float endgame = ((1.0 - ratio) * _PIECE_SQR[1][sqr]);
    score += static_cast<int>(midgame + endgame);

    // zero mobility is always bad
    p = (_KING_ATK[sqr] & ~(pc[color] | atks[!color] | _KING_ATK[king[!color]]));
    if (!p) {
      score -= 20;
      if (_test && (_KNIGHT_ATK[sqr] & atks[(!color)|Knight] & ~atks[color])) {
        score -= 20;
        state |= (color ? WhiteThreat : BlackThreat);
      }
    }
    else if (_test) {
      // is there a potential mate threat?
      // NOTE: this routine is imperfect in many ways, but it should catch
      // a large bulk of real mate threats, such as back rank mates
      w = (~atks[color] &
           ((kcross[color] & (atks[(!color)|Rook] | atks[(!color)|Queen])) |
            (kdiags[color] & (atks[(!color)|Bishop] | atks[(!color)|Queen]))));
      while (w) {
        PopLowSquare(w, chksqr);
        z = _ALL;
        switch (_diff.Dir(sqr, chksqr)) {
        case SouthWest:
          if (p == (p & _NORTH_EAST[chksqr])) {
            z = (_SOUTH_WEST[sqr] & kdiags[color] & atks[color]);
          }
          break;
        case South:
          if (p == (p & _NORTH[chksqr])) {
            z = (_SOUTH[sqr] & kcross[color] & atks[color]);
          }
          break;
        case SouthEast:
          if (p == (p & _NORTH_WEST[chksqr])) {
            z = (_SOUTH_EAST[sqr] & kdiags[color] & atks[color]);
          }
          break;
        case West:
          if (p == (p & _EAST[chksqr])) {
            z = (_WEST[sqr] & kcross[color] & atks[color]);
          }
          break;
        case East:
          if (p == (p & _WEST[chksqr])) {
            z = (_EAST[sqr] & kcross[color] & atks[color]);
          }
          break;
        case NorthWest:
          if (p == (p & _SOUTH_EAST[chksqr])) {
            z = (_NORTH_WEST[sqr] & kdiags[color] & atks[color]);
          }
          break;
        case North:
          if (p == (p & _SOUTH[chksqr])) {
            z = (_NORTH[sqr] & kcross[color] & atks[color]);
          }
          break;
        case NorthEast:
          if (p == (p & _SOUTH_WEST[chksqr])) {
            z = (_NORTH_EAST[sqr] & kdiags[color] & atks[color]);
          }
          break;
        default:
          assert(false);
        }
        if (!z) {
          if ((z = (_KING_ATK[sqr] & BIT(chksqr) &
                    ~(atks[(!color)|Pawn] | atks[(!color)|Knight] |
                      _KING_ATK[king[!color]]))))
          {
            if ((!!atks[(!color)|Bishop] +
                 !!atks[(!color)|Rook] +
                 !!atks[(!color)|Queen]) >= 2)
            {
              z = 0;
            }
            // TODO handle xray defenders
          }
          if (!z) {
            score -= 20;
            state |= (color ? WhiteThreat : BlackThreat);
          }
        }
      }
      if ((w = (_KNIGHT_ATK[sqr] & atks[(!color)|Knight] & ~atks[color]))) {
        while (w) {
          PopLowSquare(w, chksqr);
          if ((p & _KNIGHT_ATK[chksqr]) == p) {
            score -= 20;
            state |= (color ? WhiteThreat : BlackThreat);
            break;
          }
        }
      }
    }

    // penalty for being in front of own pawns
    if (BIT(sqr) & pinfo[color].front) {
      assert(pc[color|Pawn] & (color ? _NORTH[sqr] : _SOUTH[sqr]));
      score -= static_cast<int>(10 + (40 * ratio));
    }

    // midgame considerations
    if (MajorsAndMinors<!color>()) {
      int mid = 0;

      // penalty for enemy pieces menacing the king (always do this FIRST!)
      if (atkCount[!color] > 1) {
        mid -= ((_ATK_WEIGHT[atkCount[!color]] * atkScore[!color]) / 100);
        if (atkCount[!color] == 2) {
          mid /= 2; // this is why this part must be done first
        }
      }

      // penalty for holes around king
      if ((p = (_KING_ZONE[sqr] & ~(BIT(sqr) | _RANK[color ? 7 : 0] |
                                    atks[color|Pawn] | pc[color|Pawn]))))
      {
        // bigger penalty if can't castle
        if (state & (color ? BlackCastleMask : WhiteCastleMask)) {
          mid -= (4 * BitCount(p));
        }
        else {
          mid -= (8 * BitCount(p));
        }

        // extra penalty for holes attacked by the enemy
        if ((p &= atks[!color])) {
          mid -= (6 * BitCount(p));

          // more for those that are not defended
          if ((p &= ~atks[color])) {
            mid -= (6 * BitCount(p));
          }
        }
      }

      // penalty for being on [half]open files
      switch (XC(sqr)) {
      case 0: // A file
      case 1: // B file
        mid -= ((6 * !(_FILE[0] & pc[WhitePawn])) +
                (6 * !(_FILE[0] & pc[BlackPawn])) +
                (6 * !(_FILE[1] & pc[WhitePawn])) +
                (6 * !(_FILE[1] & pc[BlackPawn])) +
                (6 * !(_FILE[2] & pc[WhitePawn])) +
                (6 * !(_FILE[2] & pc[BlackPawn])));
        break;

      case 2: // C file
        mid -= ((6 * !(_FILE[0] & pc[WhitePawn])) +
                (6 * !(_FILE[0] & pc[BlackPawn])) +
                (6 * !(_FILE[1] & pc[WhitePawn])) +
                (6 * !(_FILE[1] & pc[BlackPawn])) +
                (6 * !(_FILE[2] & pc[WhitePawn])) +
                (6 * !(_FILE[2] & pc[BlackPawn])));
        break;

      case 3: // D file
      case 4: // E file
        mid -= ((6 * !(_FILE[2] & pc[WhitePawn])) +
                (6 * !(_FILE[2] & pc[BlackPawn])) +
                (6 * !(_FILE[3] & pc[WhitePawn])) +
                (6 * !(_FILE[3] & pc[BlackPawn])) +
                (6 * !(_FILE[4] & pc[WhitePawn])) +
                (6 * !(_FILE[4] & pc[BlackPawn])) +
                (6 * !(_FILE[5] & pc[WhitePawn])) +
                (6 * !(_FILE[5] & pc[BlackPawn])));
        break;

      case 5: // F file
        mid -= ((6 * !(_FILE[5] & pc[WhitePawn])) +
                (6 * !(_FILE[5] & pc[BlackPawn])) +
                (6 * !(_FILE[6] & pc[WhitePawn])) +
                (6 * !(_FILE[6] & pc[BlackPawn])) +
                (6 * !(_FILE[7] & pc[WhitePawn])) +
                (6 * !(_FILE[7] & pc[BlackPawn])));
        break;

      case 6: // G file
      case 7: // H file
        mid -= ((6 * !(_FILE[5] & pc[WhitePawn])) +
                (6 * !(_FILE[5] & pc[BlackPawn])) +
                (6 * !(_FILE[6] & pc[WhitePawn])) +
                (6 * !(_FILE[6] & pc[BlackPawn])) +
                (6 * !(_FILE[7] & pc[WhitePawn])) +
                (6 * !(_FILE[7] & pc[BlackPawn])));
        break;
      }

      // weight midgame score by remaining enemy material
      score += static_cast<int>(ratio * mid);
    }

    atks[color|King] = _KING_ATK[sqr];

#ifndef NDEBUG
    evals.pc[color|King] = score;
#endif
    return score;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int PasserEval() {
    uint64_t p = pinfo[color].passed;
    uint64_t x;
    int xsqr;
    int dist;
    int bonus;
    int score = 0;

    assert(p);
    assert(pinfo[White].pawns == pc[WhitePawn]);
    assert(pinfo[Black].pawns == pc[BlackPawn]);
    assert(pinfo[White].passed == (pc[WhitePawn] & pinfo[White].passed));
    assert(pinfo[Black].passed == (pc[BlackPawn] & pinfo[Black].passed));

    static const int PROGRESS[8] = { 0, 12, 16, 24, 36, 56, 88, 0 };
    while (p) {
      const int sqr = LowSquare(p);
      const uint64_t bit = LOW_BIT(p);
      p ^= bit;

      const int y = YC(sqr);
      bonus = PROGRESS[color ? (7 - y) : y];

      // when no rooks or queens then king and knight suuport is needed
      if (!MajorPieces<color>()) {
        // passers like friends close by
        dist = 8;
        x = (pc[color|Knight] | pc[color|King]);
        while (x) {
          PopLowSquare(x, xsqr);
          dist = std::min<int>(dist, _diff.Dist(sqr, xsqr));
        }
        assert(dist < 8);
        bonus -= (8 * dist);
      }

      if (!MajorPieces<!color>()) {
        // passers like enemies far away
        x = (pc[(!color)|Knight] | pc[(!color)|King]);
        dist = 8;
        while (x) {
          PopLowSquare(x, xsqr);
          dist = std::min<int>(dist, _diff.Dist(sqr, xsqr));
        }
        assert(dist < 8);
        bonus += (8 * dist);
      }

      // bonus if it has support
      if (bit & pinfo[color].connected) {
        bonus += (bonus / 3);

        // extra bonus for connected passers (counted twice)
        if (pinfo[color].passed &
            (_ADJACENT[sqr] | _PAWN_ATK[color][sqr] | _PAWN_ATK[!color][sqr]))
        {
          bonus = static_cast<int>(bonus * 1.33);
        }
      }

      // reduce bonus if blocked
      if (Occupied() & BIT(sqr + (color ? South : North))) {
        bonus /= 2;
      }

      // reduce bonus if rook or queen behind it
      else if (MajorPieces<!color>() & (color ? NorthX(sqr) : SouthX(sqr))) {
        bonus /= 2;
      }

      // increase bonus if the path to promotion is completely unblocked
      else {
        x = (color ? _SOUTH[sqr] : _NORTH[sqr]);
        if (!(x & (Occupied() | atks[!color]))) {
          bonus += 20;
          if (OnlyHasPawns<!color>()) {
            // can it outrun the enemy king?
            xsqr = ((color ? A1 : A8) + XC(sqr)); // destination square
            if ((_diff.Dist(sqr, xsqr) + (ColorToMove() != color)) <
                (_diff.Dist(king[!color], xsqr)))
            {
              bonus += 200;
            }
          }
        }
      }

      score += bonus;
    }

#ifndef NDEBUG
    evals.pc[color|Pawn] = score;
#endif
    return score;
  }

  //--------------------------------------------------------------------------
  inline void VerifyMaterial() const {
    int count[2] = {0,0};
    int matsum[2] = {0,0};
    int sqrsum[2] = {0,0};
    int sqr;
    for (int piece = WhitePawn; piece < PieceTypeCount; ++piece) {
      uint64_t p = pc[piece];
      while (p) {
        PopLowSquare(p, sqr);
        count[COLOR_OF(piece)]++;
        if (piece < King) {
          matsum[COLOR_OF(piece)] += ValueOf(piece);
          sqrsum[COLOR_OF(piece)] += SquareValue(piece, sqr);
        }
      }
    }
    assert(count[White] == BitCount(pc[White]));
    assert(count[Black] == BitCount(pc[Black]));
    assert(matsum[White] == material[White]);
    assert(matsum[Black] == material[Black]);
    assert(sqrsum[White] == sqrVal[White]);
    assert(sqrsum[Black] == sqrVal[Black]);
  }

  //--------------------------------------------------------------------------
  inline void VerifySliderMaps() const {
    uint64_t p;
    int sqr;

    p = (pc[WhiteBishop] | pc[BlackBishop]);
    while (p) {
      PopLowSquare(p, sqr);
      assert(slider[sqr] == BishopXO(sqr));
    }

    p = (pc[WhiteRook] | pc[BlackRook]);
    while (p) {
      PopLowSquare(p, sqr);
      assert(slider[sqr] == RookXO(sqr));
    }

    p = (pc[WhiteQueen] | pc[BlackQueen]);
    while (p) {
      PopLowSquare(p, sqr);
      assert(slider[sqr] == QueenXO(sqr));
    }
  }

  //--------------------------------------------------------------------------
  inline uint64_t CalcHashKey() const {
    uint64_t key = 0;
    uint64_t p;
    int sqr;
    for (int piece = WhitePawn; piece <= BlackKing; ++piece) {
      p = pc[piece];
      if (piece >= WhiteKing) {
        assert(p == BIT(king[COLOR_OF(piece)]));
      }
      while (p) {
        key ^= _HASH[piece][PopLowSquare(p, sqr)];
      }
    }
    return key;
  }

  //--------------------------------------------------------------------------
  inline void VerifyPosition() const {
    assert(!(pc[White] & pc[Black]));
    assert(!(_KING_ATK[king[White]] & pc[BlackKing]));
    assert(!(_KING_ATK[king[Black]] & pc[WhiteKing]));
    assert(pc[White] & BIT(king[White]));
    assert(pc[Black] & BIT(king[Black]));
    assert(pc[WhiteKing] == BIT(king[White]));
    assert(pc[BlackKing] == BIT(king[Black]));
    assert(atks[WhiteKing] == _KING_ATK[king[White]]);
    assert(atks[BlackKing] == _KING_ATK[king[Black]]);
    assert(pc[White] == (pc[WhitePawn] | pc[WhiteKnight] | pc[WhiteBishop] |
                         pc[WhiteRook] | pc[WhiteQueen]  | pc[WhiteKing]));
    assert(pc[Black] == (pc[BlackPawn] | pc[BlackKnight] | pc[BlackBishop] |
                         pc[BlackRook] | pc[BlackQueen]  | pc[BlackKing]));

    if (ColorToMove()) {
      assert(chkrs == FindPiecesGivingCheck<Black>());
      assert(!FindPiecesGivingCheck<White>());
    }
    else {
      assert(chkrs == FindPiecesGivingCheck<White>());
      assert(!FindPiecesGivingCheck<Black>());
    }

    if (_debug) {
      VerifyMaterial();
      VerifySliderMaps();
      assert(pieceKey == CalcHashKey());
      assert(kdiags[White] == BishopXO(king[White]));
      assert(kdiags[Black] == BishopXO(king[Black]));
      assert(kcross[White] == RookXO(king[White]));
      assert(kcross[Black] == RookXO(king[Black]));
    }
  }

  //--------------------------------------------------------------------------
  int Evaluate() {
#ifndef NDEBUG
    assert(!(state & Draw));
    memset(&evals, 0, sizeof(evals));
#endif
    memset(atks, 0, sizeof(atks));
    atkCount[White] = 0;
    atkCount[Black] = 0;
    atkScore[White] = 0;
    atkScore[Black] = 0;

    // evaluate from white's perspective
    int eval = ((ColorToMove() ? -_tempo : _tempo) +
                material[White] -
                material[Black] +
                sqrVal[White] -
                sqrVal[Black] +
                GetPins<Black>(6) -
                GetPins<White>(6));

    // no pawns = bad
    if (pc[WhitePawn])   eval += PawnEval<White>(); else eval -= 50;
    if (pc[BlackPawn])   eval -= PawnEval<Black>(); else eval += 50;
    if (pc[WhiteKnight]) eval += KnightEval<White>();
    if (pc[BlackKnight]) eval -= KnightEval<Black>();
    if (pc[WhiteBishop]) eval += BishopEval<White>();
    if (pc[BlackBishop]) eval -= BishopEval<Black>();
    if (pc[WhiteRook])   eval += RookEval<White>();
    if (pc[BlackRook])   eval -= RookEval<Black>();
    if (pc[WhiteQueen])  eval += QueenEval<White>();
    if (pc[BlackQueen])  eval -= QueenEval<Black>();

    // fill in attack bitmaps sans king attacks
    atks[White] = (atks[WhitePawn] | atks[WhiteKnight] | atks[WhiteBishop] |
                   atks[WhiteRook] | atks[WhiteQueen]);
    atks[Black] = (atks[BlackPawn] | atks[BlackKnight] | atks[BlackBishop] |
                   atks[BlackRook] | atks[BlackQueen]);

    eval += KingEval<White>();
    eval -= KingEval<Black>();

    // finish populating attack bitmaps
    atks[WhiteKing] = _KING_ATK[king[White]];
    atks[BlackKing] = _KING_ATK[king[Black]];
    atks[White] = (atks[WhitePawn] | atks[WhiteKnight] | atks[WhiteBishop] |
                   atks[WhiteRook] | atks[WhiteQueen]  | atks[WhiteKing]);
    atks[Black] = (atks[BlackPawn] | atks[BlackKnight] | atks[BlackBishop] |
                   atks[BlackRook] | atks[BlackQueen]  | atks[BlackKing]);

    // now that attack bitmaps are complete evaluate passed pawns
    if (pinfo[White].passed) eval += PasserEval<White>();
    if (pinfo[Black].passed) eval -= PasserEval<Black>();

    // TODO use specialized eval function for particular piece configurations

    // bonus for board coverage
    uint64_t x;
    int count;
    x = (atks[WhitePawn]|atks[WhiteKnight]|atks[WhiteBishop]|atks[WhiteRook]|
         (atks[WhiteQueen] & (_CENTER16|atks[WhiteKing]|atks[BlackKing])));
    eval += (BitCount(x) + BitCount(x & _CENTER16) + BitCount(x & _CENTER4) +
             BitCount(x & (atks[WhiteKing]|atks[BlackKing])));
    x = (atks[BlackPawn]|atks[BlackKnight]|atks[BlackBishop]|atks[BlackRook]|
         (atks[BlackQueen] & (_CENTER16|atks[WhiteKing]|atks[BlackKing])));
    eval -= (BitCount(x) + BitCount(x & _CENTER16)  + BitCount(x & _CENTER4) +
             BitCount(x & (atks[WhiteKing]|atks[BlackKing])));

    // penalty for unprotected pawns, knights, and bishops
    if ((x = ((pc[WhitePawn]|MinorPieces<White>()) & ~atks[White]))) {
      eval -= (6 * BitCount(x));
    }
    if ((x = ((pc[BlackPawn]|MinorPieces<Black>()) & ~atks[Black]))) {
      eval += (6 * BitCount(x));
    }

    // are there any pawns on the board?
    if (pc[WhitePawn] || pc[BlackPawn]) {
      assert(pinfo[White].count == BitCount(pc[WhitePawn]));
      assert(pinfo[Black].count == BitCount(pc[BlackPawn]));

      // bonus for uncontested contiguous vertical space behind connected pawns
      x = (pinfo[White].behind & ~atks[Black]);
      x &= (x >> 8);
      eval += (2 * BitCount(x));
      x = (pinfo[Black].behind & ~atks[White]);
      x &= (x >> 8);
      eval -= (2 * BitCount(x));

      // increase value of 1 knight relative to # of pawns on the board
      count = ((4 * (pinfo[White].count + pinfo[Black].count)) / 3);
      if (pc[WhiteKnight]) {
        eval += count;
      }
      if (pc[BlackKnight]) {
        eval -= count;
      }

      // increase value of 1 rook as pawns come off the board
      count = ((4 * count) / 3); // inflate pawn count a bit more
      if (pc[WhiteRook]) {
        eval += (28 - count);
      }
      if (pc[BlackRook]) {
        eval -= (28 - count);
      }

      // reduce winning score relative to number of locked pawns
      if ((x = ((pinfo[White].connected << North) & pinfo[Black].connected))) {
        if ((x &= ~((x & ~_FILE[0]) >> 1))) {
          if ((count = BitCount(x)) > 2) {
            eval = ((eval * (10 - count)) / 8);
          }
        }
      }
    }

    // draw due to lack of mating material?
    const uint64_t whitePcs = MajorsAndMinors<White>();
    const uint64_t blackPcs = MajorsAndMinors<Black>();
    const bool whiteCanWin  = (pc[WhitePawn] || pc[WhiteRook] ||
                               pc[WhiteQueen] || MULTI_BIT(pc[WhiteBishop]) ||
                               (pc[WhiteKnight] && pc[WhiteBishop]) ||
                               (BitCount(whitePcs) > 2));
    const bool blackCanWin  = (pc[BlackPawn] || pc[BlackRook] ||
                               pc[BlackQueen] || MULTI_BIT(pc[BlackBishop]) ||
                               (pc[BlackKnight] && pc[BlackBishop]) ||
                               (BitCount(blackPcs) > 2));
    if (!whiteCanWin && !blackCanWin) {
      state |= Draw;
    }

    // reduce winning score if "winning" side can't win
    else if ((eval > 0) && !whiteCanWin) {
      eval = std::min<int>(24, (eval / 6));
    }
    else if ((eval < 0) && !blackCanWin) {
      eval = std::max<int>(-24, (eval / 6));
    }

    // reduce winning score if opposite color bishop ending
    else if (SINGLE_BIT(whitePcs) && SINGLE_BIT(blackPcs) &&
             (whitePcs == pc[WhiteBishop]) && (blackPcs == pc[BlackBishop]) &&
             (!(whitePcs & _LIGHT) != !(blackPcs & _LIGHT)))
    {
      eval /= 4;
    }
    else if (!pc[WhitePawn] && !pc[BlackPawn]) {
      // reduce winning score if R vs N|B|Q ending
      if (SINGLE_BIT(whitePcs) && SINGLE_BIT(blackPcs) &&
          ((whitePcs == pc[WhiteRook]) ^ (blackPcs == pc[BlackRook])))
      {
        eval /= 4;
      }

      // reduce winning score if R vs BB
      else if ((whitePcs == pc[WhiteRook]) && (blackPcs == pc[BlackBishop]) &&
               SINGLE_BIT(whitePcs) && (BitCount(blackPcs) == 2))
      {
        eval /= 5;
      }
      else if ((blackPcs == pc[BlackRook]) && (whitePcs == pc[WhiteBishop]) &&
               SINGLE_BIT(blackPcs) && (BitCount(whitePcs) == 2))
      {
        eval /= 5;
      }
    }

    // reduce winning score if P vs N|B, e.g. 8/6b1/6P1/8/4k3/6Kp/8/8 w - -
    else if (!blackPcs && SINGLE_BIT(pc[BlackPawn]) &&
             !pc[WhitePawn] && SINGLE_BIT(whitePcs) &&
             (whitePcs == MinorPieces<White>()))
    {
      eval /= 4;
    }
    else if (!whitePcs && SINGLE_BIT(pc[WhitePawn]) &&
             !pc[BlackPawn] && SINGLE_BIT(blackPcs) &&
             (blackPcs == MinorPieces<Black>()))
    {
      eval /= 4;
    }

    // bonus if opponent can't win and we're not in a clearly worse position
    else if (!blackCanWin && (eval > -24)) {
      eval += 150;
    }
    else if (!whiteCanWin && (eval < 24)) {
      eval -= 150;
    }

    // reduce winning score if rcount is getting large
    // NOTE: this destabilizes transposition table values
    //       because rcount is not encoded into positionKey
    if ((rcount > 25) && (abs(eval) > 8)) {
      eval = static_cast<int>(eval * (25.0 / rcount));
    }

    // round score to multiple of 8
//    eval = ((eval / 8) * 8);

    // standPat is just eval from persepctive of the side to move
    standPat = (ColorToMove() ? -eval : eval);

#ifndef NDEBUG
    VerifyPosition();
#endif
    return eval;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline int ValidateMove(const Move& move) const {
#ifdef NDEBUG
    int test = 0;
#define VASSERT(x) ++test; if (!(x)) return test;
#else
#define VASSERT(x) if (!(x)) { \
    PrintBoard(); \
    senjo::Output() << move.ToString(); \
    assert(false); \
  }
#endif
    const MoveType type = move.GetType();
    const int from      = move.GetFrom();
    const int to        = move.GetTo();
    const int piece     = move.GetPc();
    const int cap       = move.GetCap();
    const int promo     = move.GetPromo();

    VASSERT(color == ColorToMove());
    VASSERT(IS_SQUARE(from));
    VASSERT(IS_SQUARE(to));
    VASSERT(from != to);
    VASSERT(IS_PIECE(piece));
    VASSERT(COLOR_OF(piece) == color);
    VASSERT(piece == _board[from]);
    VASSERT(pc[color] & BIT(from));
    VASSERT(pc[piece] & BIT(from));
    VASSERT(!(pc[color] & BIT(to)));
    VASSERT(!(pc[piece] & BIT(to)));
    VASSERT(!cap || ((cap >= Pawn) && (cap < King)));
    VASSERT(!cap || (COLOR_OF(cap) == !color));
    VASSERT(!promo || ((promo >= Knight) && (promo < King)));
    VASSERT(!promo || (COLOR_OF(promo) == color));

    switch (type) {
    case Normal:
      VASSERT(!promo);
      if (cap) {
        VASSERT(cap == _board[to]);
        VASSERT(pc[!color] & BIT(to));
        VASSERT(pc[cap] & BIT(to));
      }
      else {
        VASSERT(!_board[to]);
        VASSERT(!(pc[!color] & BIT(to)));
      }
      break;
    case PawnPush:
      VASSERT(piece == (color|Pawn));
      VASSERT(!cap);
      VASSERT(!(BIT(from) & (_RANK[0] | _RANK[7])));
      VASSERT(to == (from + (color ? South : North)));
      VASSERT(!_board[to]);
      if (promo) {
        VASSERT(BIT(to) & _RANK[color ? 0 : 7]);
        VASSERT(!(pc[promo] & BIT(to)));
      }
      else {
        VASSERT(!(BIT(to) & _RANK[color ? 0 : 7]));
      }
      break;
    case PawnLung:
      VASSERT(piece == (color|Pawn));
      VASSERT(!cap);
      VASSERT(!promo);
      VASSERT(BIT(from) & _RANK[color ? 6 : 1]);
      VASSERT(to == (from + (2 * (color ? South : North))));
      VASSERT(!_board[from + (color ? South : North)]);
      VASSERT(!_board[to]);
      break;
    case PawnCapture:
      VASSERT(piece == (color|Pawn));
      VASSERT(cap && (cap == _board[to]));
      VASSERT(!(BIT(from) & (_RANK[0] | _RANK[7])));
      VASSERT(BIT(to) & _PAWN_ATK[color][from]);
      VASSERT(pc[!color] & BIT(to));
      VASSERT(pc[cap] & BIT(to));
      if (promo) {
        VASSERT(BIT(to) & _RANK[color ? 0 : 7]);
        VASSERT(cap != ((!color)|Pawn));
        VASSERT(!(pc[promo] & BIT(to)));
      }
      else {
        VASSERT(!(BIT(to) & _RANK[color ? 0 : 7]));
      }
      break;
    case EnPassant:
      VASSERT(piece == (color|Pawn));
      VASSERT(cap == ((!color)|Pawn));
      VASSERT(!promo);
      VASSERT(BIT(from) & _RANK[color ? 3 : 4]);
      VASSERT(BIT(to) & _PAWN_ATK[color][from]);
      VASSERT(to == ep);
      VASSERT(!_board[to]);
      VASSERT(_board[to + (color ? North : South)] == cap);
      VASSERT(!(pc[!color] & BIT(to)));
      VASSERT(!(pc[cap] & BIT(to)));
      VASSERT(pc[!color] & BIT(to + (color ? North : South)));
      VASSERT(pc[cap] & BIT(to + (color ? North : South)));
      break;
    case KingMove:
      VASSERT(piece == (color|King));
      VASSERT(!promo);
      VASSERT(BIT(to) & _KING_ATK[from]);
      VASSERT(!AttackedBy<!color>(to));
      if (cap) {
        VASSERT(cap == _board[to]);
        VASSERT(pc[!color] & BIT(to));
        VASSERT(pc[cap] & BIT(to));
      }
      else {
        VASSERT(!_board[to]);
        VASSERT(!(pc[!color] & BIT(to)));
      }
      break;
    case CastleShort:
      VASSERT(piece == (color|King));
      VASSERT(!cap);
      VASSERT(!promo);
      VASSERT(state & (color ? BlackShort : WhiteShort));
      VASSERT(from == (color ? E8 : E1));
      VASSERT(to == (color ? G8 : G1));
      VASSERT(!_board[color ? F8 : F1]);
      VASSERT(!_board[color ? G8 : G1]);
      VASSERT(_board[color ? H8 : H1] == (color|Rook));
      VASSERT(!AttackedBy<!color>(color ? E8 : E1));
      VASSERT(!AttackedBy<!color>(color ? F8 : F1));
      VASSERT(!AttackedBy<!color>(color ? G8 : G1));
      break;
    case CastleLong:
      VASSERT(piece == (color|King));
      VASSERT(!cap);
      VASSERT(!promo);
      VASSERT(state & (color ? BlackLong : WhiteLong));
      VASSERT(from == (color ? E8 : E1));
      VASSERT(to == (color ? C8 : C1));
      VASSERT(_board[color ? A8 : A1] == (color|Rook));
      VASSERT(!_board[color ? B8 : B1]);
      VASSERT(!_board[color ? C8 : C1]);
      VASSERT(!_board[color ? D8 : D1]);
      VASSERT(!AttackedBy<!color>(color ? C8 : C1));
      VASSERT(!AttackedBy<!color>(color ? D8 : D1));
      VASSERT(!AttackedBy<!color>(color ? E8 : E1));
      break;
    default:
      VASSERT(false);
    }
    return 0;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void Exec(const Move& move, Bitfoot& dest) const {
    assert(color == ColorToMove());
    assert(ValidateMove<color>(move) == 0);
    const MoveType type = move.GetType();
    const int from      = move.GetFrom();
    const int to        = move.GetTo();
    const int piece     = move.GetPc();
    const int cap       = move.GetCap();
    const int promo     = move.GetPromo();
    int epSqr;

    _stats.execs++;
    _seen.insert(positionKey);

    if (this != &dest) {
      memcpy(dest.pc, pc, sizeof(pc));
    }

    dest.lastMove       = move;
    dest.lastPieceMoved = piece;
    dest.capture        = cap;
    dest.mcount         = (mcount + 1);

    switch (type) {
    case NoMove:
      break;

    case Normal:
      dest.effected = (BIT(from) | BIT(to));
      dest.state = ((state ^ ColorMask) & Touch(from) & Touch(to));
      dest.ep = NoSquare;
      dest.king[White] = king[White];
      dest.king[Black] = king[Black];
      if (cap) {
        dest.material[!color] = (material[!color] - ValueOf(cap));
        dest.material[color] = material[color];
        dest.sqrVal[!color] = (sqrVal[!color] - SquareValue(cap, to));
        dest.sqrVal[color] = (sqrVal[color] -
                              SquareValue(piece, from) +
                              SquareValue(piece, to));
        dest.rcount = 0;
        dest.pieceKey = (pieceKey ^
                         _HASH[piece][from] ^
                         _HASH[piece][to] ^
                         _HASH[cap][to]);
        dest.pc[!color] ^= BIT(to);
        dest.pc[cap] ^= BIT(to);
      }
      else {
        dest.material[White] = material[White];
        dest.material[Black] = material[Black];
        dest.sqrVal[!color] = sqrVal[!color];
        dest.sqrVal[color] = (sqrVal[color] -
                              SquareValue(piece, from) +
                              SquareValue(piece, to));
        dest.rcount = (rcount + 1);
        dest.pieceKey = (pieceKey ^ _HASH[piece][from] ^ _HASH[piece][to]);
      }
      dest.pc[color] ^= dest.effected;
      dest.pc[piece] ^= dest.effected;
      _board[to] = piece;
      _board[from] = 0;
      break;

    case PawnPush:
      dest.effected = (BIT(from) | BIT(to));
      dest.state = ((state ^ ColorMask) & StateMask);
      dest.ep = NoSquare;
      dest.king[White] = king[White];
      dest.king[Black] = king[Black];
      if (promo) {
        dest.material[!color] = material[!color];
        dest.material[color] = (material[color] -
                                ValueOf(piece) +
                                ValueOf(promo));
        dest.sqrVal[!color] = sqrVal[!color];
        dest.sqrVal[color] = (sqrVal[color] -
                              SquareValue(piece, from) +
                              SquareValue(promo, to));
        dest.rcount = 0;
        dest.pieceKey = (pieceKey ^ _HASH[piece][from] ^ _HASH[promo][to]);
        dest.pc[color] ^= dest.effected;
        dest.pc[piece] ^= BIT(from);
        dest.pc[promo] ^= BIT(to);
        _board[to] = promo;
        _board[from] = 0;
      }
      else {
        dest.material[White] = material[White];
        dest.material[Black] = material[Black];
        dest.sqrVal[!color] = sqrVal[!color];
        dest.sqrVal[color] = (sqrVal[color] -
                              SquareValue(piece, from) +
                              SquareValue(piece, to));
        dest.rcount = 0;
        dest.pieceKey = (pieceKey ^ _HASH[piece][from] ^ _HASH[piece][to]);
        dest.pc[color] ^= dest.effected;
        dest.pc[piece] ^= dest.effected;
        _board[to] = piece;
        _board[from] = 0;
      }
      break;

    case PawnLung:
      dest.effected = (BIT(from) | BIT(to));
      dest.state = ((state ^ ColorMask) & StateMask);
      dest.ep = (from + (color ? South : North));
      dest.king[White] = king[White];
      dest.king[Black] = king[Black];
      dest.material[White] = material[White];
      dest.material[Black] = material[Black];
      dest.sqrVal[!color] = sqrVal[!color];
      dest.sqrVal[color] = (sqrVal[color] -
                            SquareValue(piece, from) +
                            SquareValue(piece, to));
      dest.rcount = 0;
      dest.pieceKey = (pieceKey ^ _HASH[piece][from] ^ _HASH[piece][to]);
      dest.pc[color] ^= dest.effected;
      dest.pc[piece] ^= dest.effected;
      _board[to] = piece;
      _board[from] = 0;
      break;

    case PawnCapture:
      dest.effected = (BIT(from) | BIT(to));
      dest.state = ((state ^ ColorMask) & Touch(from) & Touch(to));
      dest.ep = NoSquare;
      dest.king[White] = king[White];
      dest.king[Black] = king[Black];
      if (promo) {
        dest.material[!color] = (material[!color] - ValueOf(cap));
        dest.material[color] = (material[color] -
                                ValueOf(piece) +
                                ValueOf(promo));
        dest.sqrVal[!color] = (sqrVal[!color] - SquareValue(cap, to));
        dest.sqrVal[color] = (sqrVal[color] -
                              SquareValue(piece, from) +
                              SquareValue(promo, to));
        dest.rcount = 0;
        dest.pieceKey = (pieceKey ^
                         _HASH[piece][from] ^
                         _HASH[promo][to] ^
                         _HASH[cap][to]);
        dest.pc[color] ^= dest.effected;
        dest.pc[piece] ^= BIT(from);
        dest.pc[promo] ^= BIT(to);
        dest.pc[!color] ^= BIT(to);
        dest.pc[cap] ^= BIT(to);
        _board[to] = promo;
        _board[from] = 0;
      }
      else {
        dest.material[!color] = (material[!color] - ValueOf(cap));
        dest.material[color] = material[color];
        dest.sqrVal[!color] = (sqrVal[!color] - SquareValue(cap, to));
        dest.sqrVal[color] = (sqrVal[color] -
                              SquareValue(piece, from) +
                              SquareValue(piece, to));
        dest.rcount = 0;
        dest.pieceKey = (pieceKey ^
                         _HASH[piece][from] ^
                         _HASH[piece][to] ^
                         _HASH[cap][to]);
        dest.pc[color] ^= dest.effected;
        dest.pc[piece] ^= dest.effected;
        dest.pc[!color] ^= BIT(to);
        dest.pc[cap] ^= BIT(to);
        _board[to] = piece;
        _board[from] = 0;
      }
      break;

    case EnPassant:
      epSqr = (to + (color ? North : South));
      dest.effected = (BIT(from) | BIT(to) | BIT(epSqr));
      dest.state = ((state ^ ColorMask) & StateMask);
      dest.ep = NoSquare;
      dest.king[White] = king[White];
      dest.king[Black] = king[Black];
      dest.material[!color] = (material[!color] - ValueOf(cap));
      dest.material[color] = material[color];
      dest.sqrVal[!color] = (sqrVal[!color] - SquareValue(cap, epSqr));
      dest.sqrVal[color] = (sqrVal[color] -
                            SquareValue(piece, from) +
                            SquareValue(piece, to));
      dest.rcount = 0;
      dest.pieceKey = (pieceKey ^
                       _HASH[piece][from] ^
                       _HASH[piece][to] ^
                       _HASH[cap][epSqr]);
      dest.pc[color] ^= (BIT(from) | BIT(to));
      dest.pc[piece] ^= (BIT(from) | BIT(to));
      dest.pc[!color] ^= BIT(epSqr);
      dest.pc[cap] ^= BIT(epSqr);
      _board[to] = piece;
      _board[from] = 0;
      _board[epSqr] = 0;
      break;

    case KingMove:
      dest.effected = (BIT(from) | BIT(to));
      dest.state = ((state ^ ColorMask) & Touch(from) & Touch(to));
      dest.ep = NoSquare;
      dest.king[!color] = king[!color];
      dest.king[color] = to;
      if (cap) {
        dest.material[!color] = (material[!color] - ValueOf(cap));
        dest.material[color] = material[color];
        dest.sqrVal[!color] = (sqrVal[!color] - SquareValue(cap, to));
        dest.sqrVal[color] = sqrVal[color];
        dest.rcount = (rcount + 1);
        dest.pieceKey = (pieceKey ^
                         _HASH[piece][from] ^
                         _HASH[piece][to] ^
                         _HASH[cap][to]);
        dest.pc[!color] ^= BIT(to);
        dest.pc[cap] ^= BIT(to);
      }
      else {
        dest.material[White] = material[White];
        dest.material[Black] = material[Black];
        dest.sqrVal[White] = sqrVal[White];
        dest.sqrVal[Black] = sqrVal[Black];
        dest.rcount = (rcount + 1);
        dest.pieceKey = (pieceKey ^ _HASH[piece][from] ^ _HASH[piece][to]);
      }
      dest.pc[color] ^= dest.effected;
      dest.pc[piece] ^= dest.effected;
      _board[to] = piece;
      _board[from] = 0;
      break;

    case CastleShort:
      dest.effected = (BIT(from) | BIT(to) |
                       BIT(color ? F8 : F1) | BIT(color ? H8 : H1));
      dest.state = ((state ^ ColorMask) & Touch(from));
      dest.ep = NoSquare;
      dest.king[!color] = king[!color];
      dest.king[color] = to;
      dest.material[White] = material[White];
      dest.material[Black] = material[Black];
      dest.sqrVal[!color] = sqrVal[!color];
      dest.sqrVal[color] = (sqrVal[color] +
                            SquareValue((color|Rook), (color ? F8 : F1)) -
                            SquareValue((color|Rook), (color ? H8 : H1)));
      dest.rcount = 0;
      dest.pieceKey = (pieceKey ^
                       _HASH[piece][from] ^
                       _HASH[piece][to] ^
                       _HASH[color|Rook][color ? F8 : F1] ^
                       _HASH[color|Rook][color ? H8 : H1]);
      dest.pc[color] ^= dest.effected;
      dest.pc[piece] ^= (BIT(from) | BIT(to));
      dest.pc[color|Rook] ^= (BIT(color ? F8 : F1) | BIT(color ? H8 : H1));
      _board[color ? F8 : F1] = (color|Rook);
      _board[color ? H8 : H1] = 0;
      _board[to] = piece;
      _board[from] = 0;
      break;

    case CastleLong:
      dest.effected = (BIT(from) | BIT(to) |
                       BIT(color ? A8 : A1) | BIT(color ? D8 : D1));
      dest.state = ((state ^ ColorMask) & Touch(from));
      dest.ep = NoSquare;
      dest.king[!color] = king[!color];
      dest.king[color] = to;
      dest.material[White] = material[White];
      dest.material[Black] = material[Black];
      dest.sqrVal[!color] = sqrVal[!color];
      dest.sqrVal[color] = (sqrVal[color] +
                            SquareValue((color|Rook), (color ? D8 : D1)) -
                            SquareValue((color|Rook), (color ? A8 : A1)));
      dest.rcount = 0;
      dest.pieceKey = (pieceKey ^
                       _HASH[piece][from] ^
                       _HASH[piece][to] ^
                       _HASH[color|Rook][color ? A8 : A1] ^
                       _HASH[color|Rook][color ? D8 : D1]);
      dest.pc[color] ^= dest.effected;
      dest.pc[piece] ^= (BIT(from) | BIT(to));
      dest.pc[color|Rook] ^= (BIT(color ? A8 : A1) | BIT(color ? D8 : D1));
      _board[color ? A8 : A1] = 0;
      _board[color ? D8 : D1] = (color|Rook);
      _board[to] = piece;
      _board[from] = 0;
      break;
    }

    dest.positionKey = (dest.pieceKey ^
                        _HASH[0][dest.state & StateMask] ^
                        _HASH[1][dest.ep]);

    // update king cross lines effected by this move
    if ((piece == WhiteKing) || (kcross[White] & dest.effected)) {
      dest.kcross[White] = dest.RookXO(dest.king[White]);
    }
    else {
      dest.kcross[White] = kcross[White];
      assert(dest.kcross[White] == dest.RookXO(dest.king[White]));
    }
    if ((piece == BlackKing) || (kcross[Black] & dest.effected)) {
      dest.kcross[Black] = dest.RookXO(dest.king[Black]);
    }
    else {
      dest.kcross[Black] = kcross[Black];
      assert(dest.kcross[Black] == dest.RookXO(dest.king[Black]));
    }

    // update king diagonal lines effected by this move
    if ((piece == WhiteKing) || (kdiags[White] & dest.effected)) {
      dest.kdiags[White] = dest.BishopXO(dest.king[White]);
    }
    else {
      dest.kdiags[White] = kdiags[White];
      assert(dest.kdiags[White] == dest.BishopXO(dest.king[White]));
    }
    if ((piece == BlackKing) || (kdiags[Black] & dest.effected)) {
      dest.kdiags[Black] = dest.BishopXO(dest.king[Black]);
    }
    else {
      dest.kdiags[Black] = kdiags[Black];
      assert(dest.kdiags[Black] == dest.BishopXO(dest.king[Black]));
    }

    // update misc state flags
    if ((dest.chkrs = dest.FindPiecesGivingCheck<!color>())) {
      dest.state |= Check;
    }

    // reset pawn info structs?
    if ((dest.pinfo[White].pawns != dest.pc[WhitePawn]) ||
        (dest.pinfo[Black].pawns != dest.pc[BlackPawn]))
    {
      memset(dest.pinfo, 0, sizeof(dest.pinfo));
    }
    else {
      if (!dest.pinfo[White].pawns) {
        assert(!dest.pinfo[White].atks);
        assert(!dest.pinfo[White].backward);
        assert(!dest.pinfo[White].connected);
        assert(!dest.pinfo[White].passed);
        assert(!dest.pinfo[White].closed);
        assert(!dest.pinfo[White].behind);
        assert(!dest.pinfo[White].front);
        assert(!dest.pinfo[White].score);
        assert(!dest.pinfo[White].count);
      }
      if (!dest.pinfo[Black].pawns) {
        assert(!dest.pinfo[Black].atks);
        assert(!dest.pinfo[Black].backward);
        assert(!dest.pinfo[Black].connected);
        assert(!dest.pinfo[Black].passed);
        assert(!dest.pinfo[Black].closed);
        assert(!dest.pinfo[Black].behind);
        assert(!dest.pinfo[Black].front);
        assert(!dest.pinfo[Black].score);
        assert(!dest.pinfo[Black].count);
      }
    }

    dest.Evaluate();
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void ExecNullMove(Bitfoot& dest) const {
    assert(color == ColorToMove());
    assert(&dest != this);
    assert(!chkrs);

    _stats.nullMoves++;

    memcpy(dest.pc, pc, sizeof(pc));
    dest.lastMove.Clear();
    dest.lastPieceMoved  = 0;
    dest.capture         = 0;
    dest.mcount          = mcount;
    dest.effected        = effected;
    dest.state           = ((state ^ ColorMask) & StateMask);
    dest.ep              = NoSquare;
    dest.king[White]     = king[White];
    dest.king[Black]     = king[Black];
    dest.material[White] = material[White];
    dest.material[Black] = material[Black];
    dest.sqrVal[White]   = sqrVal[White];
    dest.sqrVal[Black]   = sqrVal[Black];
    dest.rcount          = 0;
    dest.pieceKey        = pieceKey;
    dest.positionKey     = (dest.pieceKey ^ _HASH[0][dest.state & StateMask]);
    dest.kcross[White]   = kcross[White];
    dest.kcross[Black]   = kcross[Black];
    dest.kdiags[White]   = kdiags[White];
    dest.kdiags[Black]   = kdiags[Black];
    dest.chkrs           = 0ULL;

    // reset pawn info structs?
    if ((dest.pinfo[White].pawns != dest.pc[WhitePawn]) ||
        (dest.pinfo[Black].pawns != dest.pc[BlackPawn]))
    {
      memset(dest.pinfo, 0, sizeof(dest.pinfo));
    }
#ifndef NDEBUG
    else {
      if (!dest.pinfo[White].pawns) {
        assert(!dest.pinfo[White].atks);
        assert(!dest.pinfo[White].backward);
        assert(!dest.pinfo[White].connected);
        assert(!dest.pinfo[White].passed);
        assert(!dest.pinfo[White].closed);
        assert(!dest.pinfo[White].behind);
        assert(!dest.pinfo[White].front);
        assert(!dest.pinfo[White].score);
        assert(!dest.pinfo[White].count);
      }
      if (!dest.pinfo[Black].pawns) {
        assert(!dest.pinfo[Black].atks);
        assert(!dest.pinfo[Black].backward);
        assert(!dest.pinfo[Black].connected);
        assert(!dest.pinfo[Black].passed);
        assert(!dest.pinfo[Black].closed);
        assert(!dest.pinfo[Black].behind);
        assert(!dest.pinfo[Black].front);
        assert(!dest.pinfo[Black].score);
        assert(!dest.pinfo[Black].count);
      }
    }
#endif

    dest.Evaluate();
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void Undo(const Move& move) const {
    assert(color == ColorToMove());
    assert(move.IsValid());
    assert(IS_PIECE(move.GetPc()));
    assert(IS_SQUARE(move.GetFrom()));
    assert(IS_SQUARE(move.GetTo()));
    assert(!move.GetCap() ||
           ((move.GetCap() >= Pawn) && (move.GetCap() < King)));
    assert(!move.GetPromo() ||
           ((move.GetPromo() >= Knight) && (move.GetCap() < King)));

    _board[move.GetFrom()] = move.GetPc();
    switch (move.GetType()) {
    case Normal:
      _board[move.GetTo()] = move.GetCap();
      break;
    case PawnPush:
    case PawnLung:
      assert(move.GetPc() == (color|Pawn));
      _board[move.GetTo()] = 0;
      break;
    case PawnCapture:
      assert(move.GetPc() == (color|Pawn));
      _board[move.GetTo()] = move.GetCap();
      break;
    case EnPassant:
      assert(move.GetPc() == (color|Pawn));
      assert(move.GetCap() == ((!color)|Pawn));
      _board[move.GetTo()] = 0;
      _board[move.GetTo() + (color ? North : South)] = ((!color)|Pawn);
      break;
    case KingMove:
      assert(move.GetPc() == (color|King));
      _board[move.GetTo()] = move.GetCap();
      break;
    case CastleShort:
      assert(move.GetPc() == (color|King));
      _board[move.GetTo()] = 0;
      _board[color ? H8 : H1] = (color|Rook);
      _board[color ? F8 : F1] = 0;
      break;
    case CastleLong:
      assert(move.GetPc() == (color|King));
      _board[move.GetTo()] = 0;
      _board[color ? A8 : A1] = (color|Rook);
      _board[color ? D8 : D1] = 0;
      break;
    default:
      assert(false);
    }
    _seen.erase(positionKey);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  int SmallestAttacker(const int to) const {
    assert(IS_SQUARE(to));

    uint64_t tmp;
    int sqr;

    if ((tmp = (_PAWN_ATK[!color][to] & pc[color|Pawn]))) {
      assert(_board[LowSquare(tmp)] == (color|Pawn));
      return LowSquare(tmp);
    }
    if ((tmp = (_KNIGHT_ATK[to] & pc[color|Knight]))) {
      assert(_board[LowSquare(tmp)] == (color|Knight));
      return LowSquare(tmp);
    }

    int queen = NoSquare;
    tmp = (BishopX(to) & (pc[color|Bishop] | pc[color|Queen]));
    while (tmp) {
      PopLowSquare(tmp, sqr);
      if (_board[sqr] == (color|Bishop)) {
        return sqr;
      }
      assert(_board[sqr] == (color|Queen));
      queen = sqr;
    }
    tmp = (RookX(to) & (pc[color|Rook] | pc[color|Queen]));
    while (tmp) {
      PopLowSquare(tmp, sqr);
      if (_board[sqr] == (color|Rook)) {
        return sqr;
      }
      assert(_board[sqr] == (color|Queen));
      queen = sqr;
    }

    if (queen != NoSquare) {
      return queen;
    }

    if ((tmp = (_KING_ATK[to] & pc[color|King]))) {
      assert(_board[LowSquare(tmp)] == (color|King));
      return LowSquare(tmp);
    }

    return NoSquare;
  }

  //--------------------------------------------------------------------------
  // TODO make non-recursive
  //      This implementation was done quickly - based on Clubfoot's SEE
  //      It should be possible to do things *much* more efficiently here
  template<Color color, bool forced>
  inline int StaticExchange(const int from, const int to) {
    assert(IS_SQUARE(from));
    assert(IS_SQUARE(to));
    assert(_board[from]);
    assert(_board[to] < King);
    assert(COLOR_OF(_board[from]) == color);
    assert(pc[color] & BIT(from));
    assert(pc[_board[from]] & BIT(from));

    const int cap = _board[to];
    if ((!forced && !cap) || Pinned<color>(from, to)) {
      return 0;
    }

    assert(forced || (COLOR_OF(cap) != color));
    int gain = ValueOf(cap);
    const int piece = _board[from];
    if (piece < King) {
      if (ValueOf(piece) >= gain) {
        const uint64_t fromBit = BIT(from);
        const uint64_t toBit = BIT(to);
        _board[to] = piece;
        _board[from] = 0;
        pc[color] ^= (fromBit | toBit);
        pc[piece] ^= (fromBit | toBit);
        if (cap) {
          assert(pc[color] & toBit);
          assert(pc[cap] & toBit);
          pc[!color] ^= toBit;
          pc[cap] ^= toBit;
        }
        const int tmp = SmallestAttacker<!color>(to);
        if (tmp != NoSquare) {
          gain -= StaticExchange<!color, false>(tmp, to);
          if (!forced && (gain < 0)) {
            gain = 0;
          }
        }
        _board[from] = piece;
        _board[to] = cap;
        pc[color] ^= (fromBit | toBit);
        pc[piece] ^= (fromBit | toBit);
        if (cap) {
          pc[!color] ^= toBit;
          pc[cap] ^= toBit;
        }
      }
    }
    else if (AttackedBy<!color>(to)) {
      gain = 0;
    }
    return gain;
    return 0;
  }

  //--------------------------------------------------------------------------
  template<Color color, MoveType type>
  inline void AddMove(const int piece,
                      const int from,
                      const int to,
                      const int cap = 0,
                      const int promo = 0)
  {
    assert((moveCount + 1) < MaxMoves);
    assert(IS_MOVE_TYPE(type));
    assert(IS_SQUARE(from));
    assert(IS_SQUARE(to));
    assert(IS_PIECE(piece));
    assert(COLOR_OF(piece) == color);
    assert(_board[from] == piece);
    assert(!cap || ((cap >= Pawn) && (cap < King)));
    assert(!cap || (COLOR_OF(cap) != color));
    assert(!promo || ((promo >= Knight) && (promo < King)));
    assert(!promo || (COLOR_OF(promo) == color));

    int score = (type < KingMove)
        ? (SquareValue(piece, to) - SquareValue(piece, from))
        : 0;

    if (((type == PawnPush) || (type == PawnCapture)) && promo) {
      score += (StaticExchange<color, true>(from, to) +
                ValueOf(promo) - PawnValue);
    }
    else {
      assert(!promo);
      switch (type) {
      case Normal:
        assert((piece >= Knight) && (piece < King));
        score += StaticExchange<color, true>(from, to);
        break;
      case PawnPush:
      case PawnLung:
        assert(piece == (color|Pawn));
        score += (10 + StaticExchange<color, true>(from, to));
        break;
      case PawnCapture:
        assert(piece == (color|Pawn));
        score += (15 + StaticExchange<color, true>(from, to));
        break;
      case EnPassant:
        assert(piece == (color|Pawn));
        assert(cap == ((!color)|Pawn));
        score += (15 + StaticExchange<color, true>(from, to));
        break;
      case KingMove:
        assert(piece == (color|King));
        assert(!AttackedBy<!color>(to));
        score += ValueOf(cap);
        break;
      case CastleShort:
      case CastleLong:
        assert(piece == (color|King));
        score += 25;
        break;
      default:
        assert(false);
        break;
      }
    }

    Move& move = moves[moveCount++];
    move.Init(type, from, to, piece, cap, promo, score);
    if (!move.IsCapOrPromo()) {
      if (IsKiller(move)) {
        move.Score() += 50;
      }
      else {
        move.Score() += _hist[move.GetHistoryIndex()];
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddKingMoves(uint64_t dests) {
    assert(_KING_ATK[king[color]] == atks[color|King]);
    if (!dests) {
      return;
    }

    const uint64_t sliders = (chkrs & Sliders<!color>());
    const int from = king[color];
    int to;

    dests &= (atks[color|King] & ~atks[!color] & ~pc[color]);
    if (sliders) {
      while (dests) {
        PopLowSquare(dests, to);
        switch (_diff.Dir(from, to)) {
        case SouthWest: if (sliders & _NORTH_EAST[from]) continue; break;
        case South:     if (sliders & _NORTH[from])      continue; break;
        case SouthEast: if (sliders & _NORTH_WEST[from]) continue; break;
        case West:      if (sliders & _EAST[from])       continue; break;
        case East:      if (sliders & _WEST[from])       continue; break;
        case NorthWest: if (sliders & _SOUTH_EAST[from]) continue; break;
        case North:     if (sliders & _SOUTH[from])      continue; break;
        case NorthEast: if (sliders & _SOUTH_WEST[from]) continue; break;
        default:
          assert(false);
        }
        AddMove<color, KingMove>((color|King), from, to, _board[to]);
      }
    }
    else {
      while (dests) {
        PopLowSquare(dests, to);
        AddMove<color, KingMove>((color|King), from, to, _board[to]);
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddKingChecks() {
    assert(_KING_ATK[king[color]] == atks[color|King]);
    assert(_KING_ATK[king[!color]] == atks[(!color)|King]);
    const int from = king[color];
    if (!(BIT(from) & KingLines<!color>())) {
      return;
    }

    int to;
    const int enemy = king[!color];
    const uint64_t destinations = (Empty() & atks[color|King] & ~atks[!color]);
    uint64_t flee_sqrs = (atks[(!color)|King] & ~atks[color] & ~pc[!color]);
    uint64_t dests = 0;

    // add discovered checks
    switch (_diff.Dir(from, enemy)) {
    case SouthWest:
      if (NorthEastX(from) & BishopsQueens<color>()) {
        dests = (destinations & ~_NORTH_EAST[enemy]);
        flee_sqrs &= ~_SOUTH_WEST[from];
      }
      break;
    case South:
      if (NorthX(from) & RooksQueens<color>()) {
        dests = (destinations & ~_NORTH[enemy]);
        flee_sqrs &= ~_SOUTH[from];
      }
      break;
    case SouthEast:
      if (NorthWestX(from) & BishopsQueens<color>()) {
        dests = (destinations & ~_NORTH_WEST[enemy]);
        flee_sqrs &= ~_SOUTH_EAST[from];
      }
      break;
    case West:
      if (EastX(from) & RooksQueens<color>()) {
        dests = (destinations & ~_EAST[enemy]);
        flee_sqrs &= ~_WEST[from];
      }
      break;
    case East:
      if (WestX(from) & RooksQueens<color>()) {
        dests = (destinations & ~_WEST[enemy]);
        flee_sqrs &= ~_EAST[from];
      }
      break;
    case NorthWest:
      if (SouthEastX(from) & BishopsQueens<color>()) {
        dests = (destinations & ~_SOUTH_EAST[enemy]);
        flee_sqrs &= ~_NORTH_WEST[from];
      }
      break;
    case North:
      if (SouthX(from) & RooksQueens<color>()) {
        dests = (destinations & ~_SOUTH[enemy]);
        flee_sqrs &= ~_NORTH[from];
      }
      break;
    case NorthEast:
      if (SouthWestX(from) & BishopsQueens<color>()) {
        dests = (destinations & ~_SOUTH_WEST[enemy]);
        flee_sqrs &= ~_NORTH_EAST[from];
      }
      break;
    default:
      return;
    }
    while (dests) {
      PopLowSquare(dests, to);
      if (!(flee_sqrs & ~_KING_ATK[to])) {
        AddMove<color, KingMove>((color|King), from, to);
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddCastleMoves() {
    if (CanCastleKingSide<color>()) {
      assert(king[color] == (color ? E8 : E1));
      assert(_board[color ? H8 : H1] == (color|Rook));
      AddMove<color, CastleShort>((color|King), king[color], (color ? G8 : G1));
    }
    if (CanCastleQueenSide<color>()) {
      assert(king[color] == (color ? E8 : E1));
      assert(_board[color ? A8 : A1] == (color|Rook));
      AddMove<color, CastleLong>((color|King), king[color], (color ? C8 : C1));
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddCastleChecks() {
    if (CanCastleKingSide<color>() &&
        (BIT(king[!color]) & (color ? SouthX(G8) : NorthX(G1))))
    {
      assert(king[color] == (color ? E8 : E1));
      assert(_board[color ? H8 : H1] == (color|Rook));
      AddMove<color, CastleShort>((color|King), king[color], (color ? G8 : G1));
    }
    if (CanCastleQueenSide<color>() &&
        (BIT(king[!color]) & (color ? SouthX(D8) : NorthX(D1))))
    {
      assert(king[color] == (color ? E8 : E1));
      assert(_board[color ? A8 : A1] == (color|Rook));
      AddMove<color, CastleLong>((color|King), king[color], (color ? C8 : C1));
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddEpCaps() {
    assert(ep != NoSquare);
    int from;
    uint64_t pawns = (_PAWN_ATK[!color][ep] & pc[color|Pawn]);
    while (pawns) {
      PopLowSquare(pawns, from);
      if (!Pinned<color>(from, ep) && !EpPinned<color>(from, ep)) {
        AddMove<color, EnPassant>((color|Pawn), from, ep, ((!color)|Pawn));
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color, bool under_promote>
  inline void AddPawnCaps(const uint64_t dests) {
    if (!dests || !pc[color|Pawn]) {
      return;
    }

    uint64_t dest;
    int from;
    int to;

    if (color) {
      dest = (dests & ((pc[color|Pawn] & ~_FILE[0]) >> 9) & pc[!color]);
    }
    else {
      dest = (dests & ((pc[color|Pawn] & ~_FILE[0]) << 7) & pc[!color]);
    }
    while (dest) {
      from = (PopLowSquare(dest, to) + (color ? NorthEast : SouthEast));
      assert(_board[to] && (COLOR_OF(_board[to]) == !color));
      if (!Pinned<color>(from, to)) {
        if (BIT(to) & _RANK[color ? 0 : 7]) {
          AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                      (color|Queen));
          if (under_promote) {
            AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                        (color|Rook));
            AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                        (color|Bishop));
            AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                        (color|Knight));
          }
        }
        else {
          AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to]);
        }
      }
    }

    if (color) {
      dest = (dests & ((pc[color|Pawn] & ~_FILE[7]) >> 7) & pc[!color]);
    }
    else {
      dest = (dests & ((pc[color|Pawn] & ~_FILE[7]) << 9) & pc[!color]);
    }
    while (dest) {
      from = (PopLowSquare(dest, to) + (color ? NorthWest : SouthWest));
      assert(_board[to] && (COLOR_OF(_board[to]) == !color));
      if (!Pinned<color>(from, to)) {
        if (BIT(to) & _RANK[color ? 0 : 7]) {
          AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                      (color|Queen));
          if (under_promote) {
            AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                        (color|Rook));
            AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                        (color|Bishop));
            AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to],
                                        (color|Knight));
          }
        }
        else {
          AddMove<color, PawnCapture>((color|Pawn), from, to, _board[to]);
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color, bool under_promote>
  inline void AddPawnPushes(const uint64_t dests) {
    if (!dests || !pc[color|Pawn]) {
      return;
    }

    uint64_t pushes = (Empty() &
                       (color ? (pc[color|Pawn] >> 8)
                              : (pc[color|Pawn] << 8)));

    uint64_t lunges = (Empty() & dests &
                       (color ? ((pushes & _RANK[5]) >> 8)
                              : ((pushes & _RANK[2]) << 8)));

    int from;
    int to;
    while (lunges) {
      from = (PopLowSquare(lunges, to) + (2 * (color ? North : South)));
      if (!Pinned<color>(from, to)) {
        AddMove<color, PawnLung>((color|Pawn), from, to);
      }
    }

    pushes &= dests;
    while (pushes) {
      from = (PopLowSquare(pushes, to) + (color ? North : South));
      if (!Pinned<color>(from, to)) {
        if (BIT(to) & _RANK[color ? 0 : 7]) {
          AddMove<color, PawnPush>((color|Pawn), from, to, 0, (color|Queen));
          if (under_promote) {
            AddMove<color, PawnPush>((color|Pawn), from, to, 0, (color|Rook));
            AddMove<color, PawnPush>((color|Pawn), from, to, 0, (color|Bishop));
            AddMove<color, PawnPush>((color|Pawn), from, to, 0, (color|Knight));
          }
        }
        else {
          AddMove<color, PawnPush>((color|Pawn), from, to);
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddPawnChecks() {
    if (!pc[color|Pawn]) {
      return;
    }

    const int enemy = king[!color];
    const uint64_t lung_rank = (Empty() & _RANK[color ? 4 : 3]);
    const uint64_t flee_sqrs =
        (atks[(!color)|King] & ~atks[color] & ~pc[!color]);

    int to;
    int from;
    uint64_t pushes = (Empty() & ~_RANK[color ? 0 : 7] &
                       (color ? (pc[color|Pawn] >> 8)
                              : (pc[color|Pawn] << 8)));

    while (pushes) {
      from = (PopLowSquare(pushes, to) + (color ? North : South));
      if (Pinned<color>(from, to)) {
        continue;
      }

      const uint64_t lung = (lung_rank & (color ? (BIT(to) >> 8)
                                                : (BIT(to) << 8)));
      if (!flee_sqrs) {
        // does it give check?
        if (BIT(to) & _PAWN_ATK[!color][enemy]) {
          AddMove<color, PawnPush>((color|Pawn), from, to);
          continue;
        }

        // does it give check with a lung?
        if (lung & _PAWN_ATK[!color][enemy]) {
          AddMove<color, PawnLung>((color|Pawn), from,
                                   (to + (color ? South : North)));
          continue;
        }
      }

      // does it give discovered check?
      uint64_t flee_sqrs_after_move = flee_sqrs;
      switch (_diff.Dir(from, enemy)) {
      case SouthWest:
        if (NorthEastX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH_WEST[from];
        }
        break;
      case SouthEast:
        if (NorthWestX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH_EAST[from];
        }
        break;
      case West:
        if (EastX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_WEST[from];
        }
        break;
      case East:
        if (WestX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_EAST[from];
        }
        break;
      case NorthWest:
        if (SouthEastX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH_WEST[from];
        }
        break;
      case NorthEast:
        if (SouthWestX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH_EAST[from];
        }
        break;
      default:
        continue;
      }

      if (!(flee_sqrs_after_move & ~_PAWN_ATK[color][to])) {
        AddMove<color, PawnPush>((color|Pawn), from, to);
      }
      if (lung && !(flee_sqrs_after_move & ~_PAWN_ATK[color][LowSquare(lung)])){
        AddMove<color, PawnLung>((color|Pawn), from,
                                 (to + (color ? South : North)));
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddKnightMoves(const uint64_t dests) {
    if (!dests || !pc[color|Knight]) {
      return;
    }

    uint64_t pieces = pc[color|Knight];
    uint64_t dest;
    int from;
    int to;

    while (pieces) {
      dest = (_KNIGHT_ATK[PopLowSquare(pieces, from)] & dests & ~pc[color]);
      while (dest) {
        if (!Pinned<color>(from, PopLowSquare(dest, to))) {
          AddMove<color, Normal>((color|Knight), from, to, _board[to]);
        }
      }
    }
  }

  //----------------------------------------------------------------------------
  template<Color color>
  inline void AddKnightChecks() {
    if (!pc[color|Knight]) {
      return;
    }

    const int enemy = king[!color];
    const uint64_t flee_sqrs =
        (atks[(!color)|King] & ~atks[color] & ~pc[!color]);

    int to;
    int from;
    uint64_t dests;
    uint64_t pieces = pc[color|Knight];

    while (pieces) {
      if (!(dests = (_KNIGHT_ATK[PopLowSquare(pieces, from)] & Empty()))) {
        continue;
      }

      // would this move give discovered check?
      uint64_t flee_sqrs_after_move = flee_sqrs;
      switch (_diff.Dir(from, enemy)) {
      case SouthWest:
        if (NorthEastX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH_WEST[from];
        }
        break;
      case South:
        if (NorthX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH[from];
        }
        break;
      case SouthEast:
        if (NorthWestX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH_EAST[from];
        }
        break;
      case West:
        if (EastX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_WEST[from];
        }
        break;
      case East:
        if (WestX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_EAST[from];
        }
        break;
      case NorthWest:
        if (SouthEastX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH_WEST[from];
        }
        break;
      case North:
        if (SouthX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH[from];
        }
        break;
      case NorthEast:
        if (SouthWestX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH_EAST[from];
        }
        break;
      default:
        // no discovered check, limit destinations to direct checks
        dests &= _KNIGHT_ATK[enemy];
      }

      while (dests) {
        if (!Pinned<color>(from, PopLowSquare(dests, to))) {
          if (!(flee_sqrs_after_move & ~_KNIGHT_ATK[to])) {
            AddMove<color, Normal>((color|Knight), from, to);
          }
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddBishopMoves(const uint64_t dests) {
    if (!dests || !pc[color|Bishop]) {
      return;
    }

    uint64_t pieces = pc[color|Bishop];
    uint64_t dest;
    int from;
    int to;

    while (pieces) {
      dest = (slider[PopLowSquare(pieces, from)] & dests & ~pc[color]);
      while (dest) {
        if (!Pinned<color>(from, PopLowSquare(dest, to))) {
          AddMove<color, Normal>((color|Bishop), from, to, _board[to]);
        }
      }
    }
  }

  //----------------------------------------------------------------------------
  template<Color color>
  inline void AddBishopChecks() {
    if (!pc[color|Bishop]) {
      return;
    }

    const int enemy = king[!color];
    const uint64_t flee_sqrs =
        (atks[(!color)|King] & ~atks[color] & ~pc[!color]);

    int to;
    int from;
    uint64_t dests;
    uint64_t pieces = pc[color|Bishop];

    while (pieces) {
      if (!(dests = (slider[PopLowSquare(pieces, from)] & Empty()))) {
        continue;
      }

      // would this move give discovered check?
      uint64_t flee_sqrs_after_move = flee_sqrs;
      switch (_diff.Dir(from, enemy)) {
      case South:
        if (NorthX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH[from];
        }
        break;
      case West:
        if (EastX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_WEST[from];
        }
        break;
      case East:
        if (WestX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_EAST[from];
        }
        break;
      case North:
        if (SouthX(from) & RooksQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH[from];
        }
        break;
      default:
        // no discovered check, limit destinations to direct checks
        dests &= kdiags[!color];
      }

      while (dests) {
        if (!Pinned<color>(from, PopLowSquare(dests, to))) {
          uint64_t tmp = flee_sqrs_after_move;
          if (tmp) {
            switch (_diff.Dir(to, enemy)) {
            case SouthWest: tmp &= ~_SOUTH_WEST[to]; break;
            case SouthEast: tmp &= ~_SOUTH_EAST[to]; break;
            case NorthWest: tmp &= ~_NORTH_WEST[to]; break;
            case NorthEast: tmp &= ~_NORTH_EAST[to]; break;
            default:
              break;
            }
          }
          if (!tmp) {
            AddMove<color, Normal>((color|Bishop), from, to);
          }
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddRookMoves(const uint64_t dests) {
    if (!dests || !pc[color|Rook]) {
      return;
    }

    uint64_t pieces = pc[color|Rook];
    uint64_t dest;
    int from;
    int to;

    while (pieces) {
      dest = (slider[PopLowSquare(pieces, from)] & dests & ~pc[color]);
      while (dest) {
        if (!Pinned<color>(from, PopLowSquare(dest, to))) {
          AddMove<color, Normal>((color|Rook), from, to, _board[to]);
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddRookChecks() {
    if (!pc[color|Rook]) {
      return;
    }

    const int enemy = king[!color];
    const uint64_t flee_sqrs =
        (atks[(!color)|King] & ~atks[color] & ~pc[!color]);

    int to;
    int from;
    uint64_t dests;
    uint64_t pieces = pc[color|Rook];

    while (pieces) {
      if (!(dests = (slider[PopLowSquare(pieces, from)] & Empty()))) {
        continue;
      }

      // does it give discovered check?
      uint64_t flee_sqrs_after_move = flee_sqrs;
      switch (_diff.Dir(from, enemy)) {
      case SouthWest:
        if (NorthEastX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH_WEST[from];
        }
        break;
      case SouthEast:
        if (NorthWestX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_SOUTH_EAST[from];
        }
        break;
      case NorthWest:
        if (SouthEastX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH_WEST[from];
        }
        break;
      case NorthEast:
        if (SouthWestX(from) & BishopsQueens<color>()) {
          flee_sqrs_after_move &= ~_NORTH_EAST[from];
        }
        break;
      default:
        // no discovered check, limit destinations to direct checks
        dests &= kcross[!color];
      }

      while (dests) {
        if (!Pinned<color>(from, PopLowSquare(dests, to))) {
          uint64_t tmp = flee_sqrs_after_move;
          if (tmp) {
            switch (_diff.Dir(to, enemy)) {
            case South: tmp &= ~_SOUTH[to]; break;
            case West:  tmp &= ~_WEST[to];  break;
            case East:  tmp &= ~_EAST[to];  break;
            case North: tmp &= ~_NORTH[to]; break;
            default:
              break;
            }
          }
          if (!tmp) {
            AddMove<color, Normal>((color|Rook), from, to);
          }
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void AddQueenMoves(const uint64_t dests) {
    if (!dests || !pc[color|Queen]) {
      return;
    }

    uint64_t pieces = pc[color|Queen];
    uint64_t dest;
    int from;
    int to;

    while (pieces) {
      dest = (slider[PopLowSquare(pieces, from)] & dests & ~pc[color]);
      while (dest) {
        if (!Pinned<color>(from, PopLowSquare(dest, to))) {
          AddMove<color, Normal>((color|Queen), from, to, _board[to]);
        }
      }
    }
  }

  //----------------------------------------------------------------------------
  template<Color color>
  inline void AddQueenChecks() {
    if (!pc[color|Queen]) {
      return;
    }

    const uint64_t flee_sqrs =
        (atks[(!color)|King] & ~atks[color] & ~pc[!color]);

    int to;
    int from;
    uint64_t tmp;
    uint64_t dests;
    uint64_t pieces = pc[color|Queen];

    while (pieces) {
      dests = (slider[PopLowSquare(pieces, from)] & Empty() &
               KingLines<!color>());
      while (dests) {
        if (!Pinned<color>(from, PopLowSquare(dests, to))) {
          switch (_diff.Dir(to, king[!color])) {
          case SouthWest:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_SOUTH_WEST[to]);
            break;
          case South:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_SOUTH[to]);
            break;
          case SouthEast:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_SOUTH_EAST[to]);
            break;
          case West:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_WEST[to]);
            break;
          case East:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_EAST[to]);
            break;
          case NorthWest:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_NORTH_WEST[to]);
            break;
          case North:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_NORTH[to]);
            break;
          case NorthEast:
            tmp = (flee_sqrs & ~_KING_ATK[to] & ~_NORTH_EAST[to]);
            break;
          default:
            tmp = (flee_sqrs & ~_KING_ATK[to]);
          }
          if (!tmp) {
            AddMove<color, Normal>((color|Queen), from, to);
          }
        }
      }
    }
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void GetCheckEvasions() {
    assert(SINGLE_BIT(chkrs));

    // can we give check via en passant?
    if ((ep != NoSquare) && (BIT(ep + (color ? North : South)) == chkrs)) {
      AddEpCaps<color>();
    }

    // add moves that capture the piece that's giving check
    if (atks[color] & chkrs) {
      AddPawnCaps   <color, true>(chkrs);
      AddKnightMoves<color>(chkrs);
      AddBishopMoves<color>(chkrs);
      AddRookMoves  <color>(chkrs);
      AddQueenMoves <color>(chkrs);
    }

    // move the king out of check
    AddKingMoves<color>(_ALL);

    // add moves that block the check
    const int sqr = king[color];
    if (!(chkrs & (atks[color|King] | _KNIGHT_ATK[sqr]))) {
      uint64_t dest = 0;
      switch (_diff.Dir(sqr, LowSquare(chkrs))) {
      case SouthWest: dest = (_SOUTH_WEST[sqr] & ABOVEX(chkrs)); break;
      case South:     dest = (_SOUTH[sqr]      & ABOVEX(chkrs)); break;
      case SouthEast: dest = (_SOUTH_EAST[sqr] & ABOVEX(chkrs)); break;
      case West:      dest = (_WEST[sqr]       & ABOVEX(chkrs)); break;
      case East:      dest = (_EAST[sqr]       & BELOW(chkrs));  break;
      case NorthWest: dest = (_NORTH_WEST[sqr] & BELOW(chkrs));  break;
      case North:     dest = (_NORTH[sqr]      & BELOW(chkrs));  break;
      case NorthEast: dest = (_NORTH_EAST[sqr] & BELOW(chkrs));  break;
      default:
        assert(false);
      }
      AddPawnPushes <color, true>(dest);
      AddKnightMoves<color>(dest);
      AddBishopMoves<color>(dest);
      AddRookMoves  <color>(dest);
      AddQueenMoves <color>(dest);
    }
  }

  //--------------------------------------------------------------------------
  template<Color color, bool under_promote>
  inline bool GetPromosAndCaptures() {
    const uint64_t caps = ((pc[!color] ^ pc[(!color)|King]) & atks[color]);

    AddPawnCaps   <color, under_promote>(caps);
    AddPawnPushes <color, under_promote>(Empty() & _RANK[color ? 0 : 7]);
    AddKnightMoves<color>(caps);
    AddBishopMoves<color>(caps);
    AddRookMoves  <color>(caps);
    AddQueenMoves <color>(caps);
    AddKingMoves  <color>(caps);

    if (ep != NoSquare) {
      AddEpCaps<color>();
    }

    return (moveCount > moveIndex);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool GetChecks() {
    AddQueenChecks <color>();
    AddRookChecks  <color>();
    AddBishopChecks<color>();
    AddKnightChecks<color>();
    AddPawnChecks  <color>();
    AddKingChecks  <color>();
    AddCastleChecks<color>();
    return (moveCount > moveIndex);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline bool GetQuietMoves() {
    AddCastleMoves<color>();
    AddPawnPushes <color, true>(Empty() & ~_RANK[color ? 0 : 7]);
    AddKnightMoves<color>(Empty());
    AddBishopMoves<color>(Empty());
    AddQueenMoves <color>(Empty());
    AddRookMoves  <color>(Empty());
    AddKingMoves  <color>(Empty());
    return (moveCount > moveIndex);
  }

  //--------------------------------------------------------------------------
  inline void SortMovesLexically() {
    std::sort(moves, (moves + moveCount), Move::LexicalCompare);
  }

  //--------------------------------------------------------------------------
  inline void ScootMoveToFront(int idx) {
    assert((idx >= 0) && (idx < moveCount));
    while (idx-- > 0) {
      moves[idx].SwapWith(moves[idx + 1]);
    }
  }

  //--------------------------------------------------------------------------
  inline void ResetMovegen(const uint64_t key) {
    movegenKey = key;
    moveStage  = PromosAndCaptures;
    moveCount  = 0;
    moveIndex  = 0;
    sortIndex  = 0;
  }

  //--------------------------------------------------------------------------
  inline void InitMovegen(const MoveGenType type) {
    static const uint64_t KEY[3] = {
      0ULL,
      0x7A5004E21D7C3B15ULL,
      0x39B82FF6054C9E34ULL
    };
    const uint64_t moveKey = (positionKey ^ KEY[type]);
    if (positionKey && (moveKey == movegenKey)) {
      moveIndex = 0;
    }
    else {
      ResetMovegen(moveKey);
    }
  }

  //--------------------------------------------------------------------------
  template<Color color, MoveGenType type>
  inline Move* GetNextMove(const int depth) {
    assert(color == ColorToMove());
    assert((moveStage >= PromosAndCaptures) && (moveStage <= Finished));
    assert((moveCount >= 0) && (moveCount < MaxMoves));
    assert(moveIndex >= 0);

    if (moveIndex >= moveCount) {
      assert(sortIndex == moveCount);
      switch (moveStage) {
      case PromosAndCaptures:
        assert(moveCount == 0);
        assert(moveIndex == 0);
        if (chkrs) {
          moveStage = Finished;
          if (MULTI_BIT(chkrs)) {
            AddKingMoves<color>(_ALL);
          }
          else {
            GetCheckEvasions<color>();
          }
          break;
        }
        else if (type == Quiescence) {
          moveStage = QuietMoves;
          if (GetPromosAndCaptures<color, false>()) { // under_promote=false
            break;
          }
        }
        else {
          moveStage = Finished;
          GetPromosAndCaptures<color, true>(); // under_promote=true
          GetQuietMoves<color>();
          break;
        }
      case QuietMoves:
        assert(type == Quiescence);
        moveStage = Finished;
        if (depth == 0) {
          GetChecks<color>();
        }
      case Finished:
        break;
      default:
        assert(false);
      }
    }

    if (moveIndex >= moveCount) {
      return NULL;
    }

    if (moveIndex >= sortIndex) {
      sortIndex++;
      int best_index = moveIndex;
      int best_score = moves[moveIndex].GetScore();
      for (int i = (moveIndex + 1); i < moveCount; ++i) {
        if (moves[i].GetScore() > best_score) {
          best_score = moves[i].GetScore();
          best_index = i;
        }
      }
      if (best_index > moveIndex) {
        moves[moveIndex].SwapWith(moves[best_index]);
      }
    }

    return (moves + moveIndex++);
  }

  //--------------------------------------------------------------------------
  template<Color color>
  inline void GenerateMoves() {
    InitMovegen(AllMoves);
    if (moveStage != Finished) {
      assert(moveStage == PromosAndCaptures);
      assert(moveCount == 0);
      assert(moveIndex == 0);
      assert(sortIndex == 0);
      if (chkrs) {
        if (MULTI_BIT(chkrs)) {
          AddKingMoves<color>(_ALL);
        }
        else {
          GetCheckEvasions<color>();
        }
      }
      else {
        GetPromosAndCaptures<color, true>(); // under_promote=true
        GetQuietMoves<color>();
      }
      moveStage = Finished;
    }
    moveIndex = sortIndex = 0;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  uint64_t PerftSearch(const int depth) {
    GenerateMoves<color>();
    if (!child || (depth <= 1)) {
      return moveCount;
    }

    uint64_t count = 0;

    for (; !_stop && (moveIndex < moveCount); ++moveIndex) {
      const Move& move = moves[moveIndex];
      Exec<color>(move, *child);
      count += child->PerftSearch<!color>(depth - 1);
      Undo<color>(move);
    }

    return count;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  uint64_t PerftSearchRoot(const int depth) {
    assert(_initialized);
    assert(ply == 0);
    assert(!parent);
    assert(child == _node);

    if (_debug) {
      PrintBoard();
      senjo::Output() << GetFEN();
    }

    GenerateMoves<color>();
    SortMovesLexically();

    uint64_t count = 0;

    if (child && (depth > 1)) {
      for (; !_stop && (moveIndex < moveCount); ++moveIndex) {
        const Move& move = moves[moveIndex];
        Exec<color>(move, *child);
        const uint64_t c = child->PerftSearch<!color>(depth - 1);
        Undo<color>(move);
        senjo::Output() << move.ToString() << ' ' << c << ' ' << move.GetScore();
        count += c;
      }
    }
    else {
      for (; !_stop && (moveIndex < moveCount); ++moveIndex) {
        senjo::Output() << moves[moveIndex].ToString() << " 1 "
                        << moves[moveIndex].GetScore();
        count++;
      }
    }

    return count;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  int QSearch(int alpha, const int beta, const int depth) {
    assert(alpha < beta);
    assert(abs(alpha) <= Infinity);
    assert(abs(beta) <= Infinity);
    assert(depth <= 0);

    _stats.qnodes++;
    if (ply > _seldepth) {
      _seldepth = ply;
    }

    pvCount = 0;
    if (IsDraw()) {
      return _drawScore[color];
    }

    // mate distance pruning and standPat beta cutoff when not in check
    const bool check = InCheck();
    int best = (check ? (ply - Infinity) : standPat);
    if ((best >= beta) || !child) {
      return best;
    }
    if (best > alpha) {
      alpha = best;
    }

    // do we have anything for this position in the transposition table?
    Move firstMove;
    HashEntry* entry = _tt.Probe(positionKey);
    if (entry) {
      switch (entry->GetPrimaryFlag()) {
      case HashEntry::Checkmate: return (ply - Infinity);
      case HashEntry::Stalemate: return _drawScore[color];
      case HashEntry::UpperBound:
        firstMove.Init(entry->moveBits, entry->score);
        assert(ValidateMove<color>(firstMove) == 0);
        if (entry->score <= alpha) {
          pv[0] = firstMove;
          pvCount = 1;
          return entry->score;
        }
        break;
      case HashEntry::ExactScore:
        firstMove.Init(entry->moveBits, entry->score);
        assert(ValidateMove<color>(firstMove) == 0);
        pv[0] = firstMove;
        pvCount = 1;
        if ((entry->score >= beta) && !firstMove.IsCapOrPromo()) {
          AddKiller(firstMove);
        }
        return entry->score;
      case HashEntry::LowerBound:
        firstMove.Init(entry->moveBits, entry->score);
        assert(ValidateMove<color>(firstMove) == 0);
        if (entry->score >= beta) {
          pv[0] = firstMove;
          pvCount = 1;
          if (!firstMove.IsCapOrPromo()) {
            AddKiller(firstMove);
          }
          return entry->score;
        }
        break;
      default:
        assert(false);
      }

      // don't use firstMove unless it's a cap or promo, or we're in check
      if (!check && !firstMove.IsCapOrPromo()) {
        firstMove.Clear();
      }
    }

    // search firstMove if we have it
    const int orig_alpha = alpha;
    if (firstMove.IsValid()) {
      _stats.qexecs++;
      Exec<color>(firstMove, *child);
      firstMove.Score() = -child->QSearch<!color>(-beta, -alpha, (depth - 1));
      Undo<color>(firstMove);
      if (_stop) {
        return beta;
      }
      if (firstMove.GetScore() >= best) {
        best = firstMove.GetScore();
        UpdatePV(firstMove);
        if (firstMove.GetScore() >= beta) {
          if (!firstMove.IsCapOrPromo()) {
            AddKiller(firstMove);
          }
          if (check) {
            firstMove.Score() = beta;
            _tt.Store(positionKey, firstMove, 0, HashEntry::LowerBound, 0);
          }
          return best;
        }
        if (firstMove.GetScore() > alpha) {
          alpha = firstMove.GetScore();
        }
      }
    }

    // search 'em
    Move* move;
    InitMovegen(Quiescence);
    while ((move = GetNextMove<color, Quiescence>(depth))) {
      if (firstMove == (*move)) {
        assert(firstMove.IsValid());
        continue;
      }

      _stats.qexecs++;
      Exec<color>(*move, *child);
      if (_delta && !check && (depth < 0) &&
          !move->GetPromo() && !child->InCheck() &&
          ((standPat + ValueOf(move->GetCap()) + _delta) < alpha))
      {
        Undo<color>(*move);
        if (_stop) {
          return beta;
        }
        _stats.deltaCount++;
        continue;
      }

      move->Score() = -child->QSearch<!color>(-beta, -alpha, (depth - 1));
      Undo<color>(*move);
      if (_stop) {
        return beta;
      }

      if (move->GetScore() > best) {
        best = move->GetScore();
        UpdatePV(*move);
        if (move->GetScore() >= beta) {
          if (!move->IsCapOrPromo()) {
            AddKiller(*move);
          }
          if (check) {
            move->Score() = beta;
            _tt.Store(positionKey, *move, 0, HashEntry::LowerBound, 0);
          }
          return best;
        }
        if (move->GetScore() > alpha) {
          alpha = move->GetScore();
        }
      }
    }

    assert(best <= alpha);
    assert(alpha < beta);

    if (check) {
      if (!moveCount) {
        assert(!firstMove.IsValid());
        assert(!pvCount);
        _tt.StoreCheckmate(positionKey);
        return (ply - Infinity);
      }
      if (pvCount > 0) {
        if (alpha > orig_alpha) {
          assert(pv[0].GetScore() == alpha);
          _tt.Store(positionKey, pv[0], 0, HashEntry::ExactScore, 0);
        }
        else {
          assert(alpha == orig_alpha);
          assert(pv[0].GetScore() <= alpha);
          pv[0].Score() = alpha;
          _tt.Store(positionKey, pv[0], 0, HashEntry::UpperBound, 0);
        }
      }
    }

    return best;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  int Search(int alpha, const int beta, int depth, const bool cutNode) {
    assert(alpha < beta);
    assert(abs(alpha) <= Infinity);
    assert(abs(beta) <= Infinity);
    assert(depth > 0);

    _stats.snodes++;
    extended = 0;
    reduced  = 0;
    pvCount  = 0;

    if (IsDraw()) {
      return _drawScore[color];
    }

    // mate distance pruning
    int best = (ply - Infinity);
    if ((best >= beta) || !child) {
      return best;
    }
    if (best > alpha) {
      alpha = best;
    }

    // extend depth if in check and previous ply not extended
    const bool check = InCheck();
    if (_ext && check && !parent->extended) {
      if (MULTI_BIT(chkrs)) {
        _stats.chkExts++;
        extended++;
        depth++;
      }
      else {
        uint64_t dests = (atks[color|King] & ~atks[!color] & ~pc[color]);
        if (dests) {
          const int from = LowSquare(chkrs);
          switch (_diff.Dir(from, king[color])) {
          case SouthWest: dests &= ~_SOUTH_WEST[from]; break;
          case South:     dests &= ~_SOUTH[from];      break;
          case SouthEast: dests &= ~_SOUTH_EAST[from]; break;
          case West:      dests &= ~_WEST[from];       break;
          case East:      dests &= ~_EAST[from];       break;
          case NorthWest: dests &= ~_NORTH_WEST[from]; break;
          case North:     dests &= ~_NORTH[from];      break;
          case NorthEast: dests &= ~_NORTH_EAST[from]; break;
          default:
            break;
          }
        }
        if (!MULTI_BIT(dests)) {
          _stats.chkExts++;
          extended++;
          depth++;
        }
      }
    }

    // extend depth if we are facing a new threat
    if ((_test > 1) && !extended && !parent->extended &&
        (state & (color ? WhiteThreat : BlackThreat)) &&
        !(parent->state & (color ? WhiteThreat : BlackThreat)))
    {
      _stats.threatExts++;
      extended++;
      depth++;
    }

    // do we have anything for this position in the transposition table?
    const bool pvNode = ((alpha + 1) < beta);
    HashEntry* entry = _tt.Probe(positionKey);
    Move firstMove;
    if (entry) {
      switch (entry->GetPrimaryFlag()) {
      case HashEntry::Checkmate: return (ply - Infinity);
      case HashEntry::Stalemate: return _drawScore[color];
      case HashEntry::UpperBound:
        firstMove.Init(entry->moveBits, entry->score);
        assert(ValidateMove<color>(firstMove) == 0);
        if ((!pvNode || entry->HasPvFlag()) &&
            (entry->depth >= depth) && (entry->score <= alpha))
        {
          pv[0] = firstMove;
          pvCount = 1;
          return entry->score;
        }
        if ((entry->depth >= (depth - 3)) && (entry->score < beta)) {
          nullMoveOk = 0;
        }
        break;
      case HashEntry::ExactScore:
        firstMove.Init(entry->moveBits, entry->score);
        assert(ValidateMove<color>(firstMove) == 0);
        if (entry->depth >= depth) {
          pv[0] = firstMove;
          pvCount = 1;
          if ((entry->score >= beta) && !firstMove.IsCapOrPromo()) {
            IncHistory(firstMove, check, entry->depth);
            AddKiller(firstMove);
          }
          return entry->score;
        }
        if ((entry->depth >= (depth - 3)) && (entry->score < beta)) {
          nullMoveOk = 0;
        }
        break;
      case HashEntry::LowerBound:
        firstMove.Init(entry->moveBits, entry->score);
        assert(ValidateMove<color>(firstMove) == 0);
        if ((!pvNode || entry->HasPvFlag()) &&
            (entry->depth >= depth) && (entry->score >= beta))
        {
          pv[0] = firstMove;
          pvCount = 1;
          if (!firstMove.IsCapOrPromo()) {
            IncHistory(firstMove, check, entry->depth);
            AddKiller(firstMove);
          }
          return entry->score;
        }
        break;
      default:
        assert(false);
      }
      if (entry->HasExtendedFlag() && !extended && !parent->extended) {
        _stats.hashExts++;
        extended++;
        depth++;
      }
    }

    // razoring
    // if we're well below alpha and q-search doesn't show a saving tactic
    // return q-search result
    int eval;
    if (_rzr && !check && !pvNode && !firstMove.IsValid() && (depth <= 2) &&
        (abs(alpha) < WinningScore) &&
        ((standPat + _rzr + (64 * (depth - 1))) < alpha))
    {
      _stats.rzrCount++;
      eval = QSearch<color>(alpha, beta, 0);
      if (_stop) {
        return beta;
      }
      if (eval <= alpha) {
        _stats.rzrCutoffs++;
        return eval;
      }
    }

    // null move pruning
    // if we can get a score >= beta without even making a move, return beta
    if (_nmp && nullMoveOk && !check && !pvNode && (depth > 1) &&
        (standPat >= beta) && (abs(beta) < WinningScore) &&
        MajorsAndMinors<color>())
    {
      ExecNullMove<color>(*child);
      child->nullMoveOk = 0;
      const int rdepth = std::max<int>(0, (depth - 3 - (depth / 6) -
                                           ((standPat - beta) >= 400)));
      eval = (rdepth > 0)
          ? -child->Search<!color>(-beta, (1 - beta), rdepth, false)
          : -child->QSearch<!color>(-beta, (1 - beta), 0);
      if (_stop) {
        return beta;
      }
      if (eval >= beta) {
        // TODO do verification search at high depths
        pvCount = 0;
        _stats.nmCutoffs++;
        return beta; // do not return eval
      }
      // TODO return alpha if threat detected and it was enabled by parent move
      // TODO if threat and NOT caused by parent move do parent->nullMoveOK = 0
    }

    // internal iterative deepening if no firstMove in transposition table
    if (_iid && !check && !firstMove.IsValid() && (beta < Infinity) &&
        (depth > (pvNode ? 3 : 5)))
    {
      assert(!pvCount);
      _stats.iidCount++;
      const int saved = nullMoveOk;
      nullMoveOk = 0;
      eval = Search<color>((beta - 1), beta, (depth - (pvNode ? 2 : 4)), true);
      nullMoveOk = saved;
      if (_stop || !pvCount) {
        return eval;
      }
      if (eval >= beta) {
        _stats.iidBeta++;
      }
      assert(pv[0].IsValid());
      firstMove = pv[0];
    }

    // make sure firstMove is populated
    Move* move;
    InitMovegen(AllMoves);
    if (!firstMove.IsValid()) {
      if (!(move = GetNextMove<color, AllMoves>(depth))) {
        assert(!moveCount);
        if (check) {
          _tt.StoreCheckmate(positionKey);
          return (ply - Infinity);
        }
        _tt.StoreStalemate(positionKey);
        return _drawScore[color];
      }
      firstMove = (*move);
      if (_oneReply && (moveCount == 1) && !extended) {
        _stats.oneReplyExts++;
        extended++;
        depth++;
      }
    }

    // search first move with full alpha/beta window
    const int orig_alpha = alpha;
    Exec<color>(firstMove, *child);
    child->nullMoveOk = 1;
    eval = (depth > 1)
        ? -child->Search<!color>(-beta, -alpha, (depth - 1), !cutNode)
        : -child->QSearch<!color>(-beta, -alpha, 0);
    Undo<color>(firstMove);
    if (_stop) {
      return beta;
    }
    if (eval > alpha) {
      alpha = eval;
    }
    if (eval >= best) {
      best = eval;
      UpdatePV(firstMove);
      if (eval >= beta) {
        if (!firstMove.IsCapOrPromo()) {
          IncHistory(firstMove, check, depth);
          AddKiller(firstMove);
        }
        firstMove.Score() = beta;
        _tt.Store(positionKey, firstMove, depth, HashEntry::LowerBound,
                  ((extended ? HashEntry::Extended : 0) |
                   (pvNode ? HashEntry::FromPV : 0)));
        return best;
      }
    }
    else if (!firstMove.IsCapOrPromo()) {
      DecHistory(firstMove, check);
    }

    bool lmr_ok = (_lmr && !pvNode && !check && (depth > (_lmr + 1)));
    int pvDepth = depth;
    int newDepth;

    // search remaining moves
    while ((move = GetNextMove<color, AllMoves>(depth))) {
      if (firstMove == (*move)) {
        assert(firstMove.IsValid());
        continue;
      }

      Exec<color>(*move, *child);

      // late move reductions
      _stats.lateMoves++;
      _stats.lmCandidates += lmr_ok;
      if (lmr_ok &&
          !move->IsCapOrPromo() &&
          !child->InCheck() &&
          !IsKiller(*move) &&
          !((move->GetPc() == (color|Pawn)) &&
            (YC(move->GetTo()) == (color ? 1 : 6))) &&
          (_hist[move->GetHistoryIndex()] < 0))
      {
        _stats.lmReductions++;
        reduced = _lmr;
        if ((depth > (reduced + 1)) && (_hist[move->GetHistoryIndex()] < -1)) {
          _stats.lmDoubleRed++;
          reduced++;
        }
      }
      else {
        reduced = 0;
      }

      // first search with a null window to quickly see if it improves alpha
      newDepth = (depth - 1 - reduced);
      child->nullMoveOk = 1;
      eval = (newDepth > 0)
          ? -child->Search<!color>(-(alpha + 1), -alpha, newDepth, true)
          : -child->QSearch<!color>(-(alpha + 1), -alpha, 0);
      child->nullMoveOk = 0;

      // re-search at full depth?
      if (!_stop && reduced && (eval > alpha)) {
        assert(depth > 1);
        _stats.lmResearches++;
        reduced = 0;
        eval = -child->Search<!color>(-(alpha + 1), -alpha, (depth - 1), false);
        if (!_stop && (eval > alpha)) {
          _stats.lmConfirmed++;
        }
      }

      // re-search with full window?
      if (!_stop && pvNode && (eval > alpha)) {
        assert(!reduced);
        eval = (depth > 1)
            ? -child->Search<!color>(-beta, -alpha, (depth - 1), false)
            : -child->QSearch<!color>(-beta, -alpha, 0);
      }

      Undo<color>(*move);
      if (_stop) {
        return beta;
      }
      if (eval > alpha) {
        alpha = eval;
        _stats.lmAlphaIncs++;
      }
      if (eval > best) {
        best = eval;
        UpdatePV(*move);
        pvDepth = (depth - reduced);
        if (eval >= beta) {
          if (!move->IsCapOrPromo()) {
            IncHistory(*move, check, pvDepth);
            AddKiller(*move);
          }
          move->Score() = beta;
          _tt.Store(positionKey, *move, pvDepth, HashEntry::LowerBound,
                    ((extended ? HashEntry::Extended : 0) |
                     (pvNode ? HashEntry::FromPV : 0)));
          return best;
        }
      }
      else if (!move->IsCapOrPromo()) {
        DecHistory(*move, check);
      }
    }

    assert(moveCount > 0);
    assert(best <= alpha);
    assert(alpha < beta);

    if (pvCount > 0) {
      pv[0].Score() = alpha;
      if (alpha > orig_alpha) {
        if (!pv[0].IsCapOrPromo()) {
          IncHistory(pv[0], check, pvDepth);
        }
        _tt.Store(positionKey, pv[0], pvDepth, HashEntry::ExactScore,
            ((extended ? HashEntry::Extended : 0) |
             (pvNode ? HashEntry::FromPV : 0)));
      }
      else {
        assert(alpha == orig_alpha);
        _tt.Store(positionKey, pv[0], pvDepth, HashEntry::UpperBound,
            ((extended ? HashEntry::Extended : 0) |
             (pvNode ? HashEntry::FromPV : 0)));
      }
    }

    return best;
  }

  //--------------------------------------------------------------------------
  template<Color color>
  std::string SearchRoot(const int depth) {
    assert(_initialized);
    assert(ply == 0);
    assert(!parent);
    assert(child == _node);

    extended = 0;
    reduced = 0;

    if (_debug) {
      PrintBoard();
      senjo::Output() << GetFEN();
    }

    GenerateMoves<color>();
    if (moveCount <= 0) {
      senjo::Output() << "No legal moves";
      return std::string();
    }
    while (GetNextMove<color, AllMoves>(1)) { ; } // sort 'em

    // move transposition table move (if any) to front of list
    if (moveCount > 1) {
      HashEntry* entry = _tt.Probe(positionKey);
      if (entry) {
        switch (entry->GetPrimaryFlag()) {
        case HashEntry::Checkmate:
        case HashEntry::Stalemate:
          assert(false);
          break;
        case HashEntry::UpperBound:
        case HashEntry::ExactScore:
        case HashEntry::LowerBound: {
          const Move ttMove(entry->moveBits, entry->score);
          assert(ValidateMove<color>(ttMove) == 0);
          for (int i = 0; i < moveCount; ++i) {
            if (moves[i] == ttMove) {
              ScootMoveToFront(i);
              break;
            }
          }
          break;
        }}
      }
    }

    // initial principal variation
    pvCount = 1;
    pv[0] = moves[0];

    // return immediately if we only have one move
    if (moveCount == 1) {
      OutputPV(pv[0].GetScore());
      return pv[0].ToString();
    }

    Move* move;
    bool  showPV = true;
    int   alpha;
    int   best = standPat;
    int   beta;
    int   delta;

    // iterative deepening
    for (int d = 0; !_stop && (d < depth); ++d) {
      _seldepth = _depth = (d + 1);
      child->nullMoveOk = (d > 0);

      showPV = true;
      delta  = 25;
      alpha  = std::max<int>((best - delta), -Infinity);
      beta   = std::min<int>((best + delta), +Infinity);

      for (moveIndex = 0; !_stop && (moveIndex < moveCount); ++moveIndex) {
        move      = (moves + moveIndex);
        _currmove = move->ToString();
        _movenum  = (moveIndex + 1);

        // aspiration window search
        VerifySliderMaps();
        Exec<color>(*move, *child);
        while (!_stop) {
          move->Score() = (_depth > 1)
              ? -child->Search<!color>(-beta, -alpha, (_depth - 1), false)
              : -child->QSearch<!color>(-beta, -alpha, 0);

          // expand aspiration window and re-search?
          if (!_stop && ((move->GetScore() >= beta) ||
                         ((move->GetScore() <= alpha) && (_movenum == 1))))
          {
            delta *= 20;
            if (move->GetScore() >= beta) {
              assert(move->GetScore() < Infinity);
              beta = std::min<int>((move->GetScore() + delta), Infinity);
              if ((senjo::Now() - _startTime) > 1000) {
                OutputPV(move->GetScore(), 1); // report lowerbound
              }
            }
            else {
              assert(move->GetScore() > -Infinity);
              alpha = std::max<int>((move->GetScore() - delta), -Infinity);
              if ((senjo::Now() - _startTime) > 1000) {
                OutputPV(move->GetScore(), -1); // report upperbound
              }
            }
            continue;
          }
          delta = 25;
          break;
        }
        Undo<color>(*move);

        // do we have a new principal variation?
        if (!_stop && ((_movenum == 1) || (move->GetScore() > best))) {
          UpdatePV(*move);
          OutputPV(move->GetScore());
          showPV = false;
          _tt.Store(positionKey, *move, _depth, HashEntry::ExactScore,
                    HashEntry::FromPV);

          // set null aspiration window now that we have a principal variation
          best = alpha = move->GetScore();
          beta = (alpha + 1);

          ScootMoveToFront(moveIndex);
        }
      }
    }

    if (showPV) {
      OutputPV(pv[0].GetScore());
    }

    return pv[0].ToString();
  }

  //--------------------------------------------------------------------------
  void InitSearch() {
    _currmove.clear();
    _stats.Clear();
    _tt.ResetCounters();

    _depth        = 0;
    _movenum      = 0;
    _seldepth     = 0;

    _drawScore[ColorToMove()] = -_contempt;
    _drawScore[!ColorToMove()] = _contempt;
  }
};

} // namespace bitfoot

#endif // BITFOOT_H
