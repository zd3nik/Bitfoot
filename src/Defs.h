//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>, All rights reserved
//----------------------------------------------------------------------------

#ifndef BITFOOT_DEFS_H
#define BITFOOT_DEFS_H

#include "senjo/Platform.h"
#ifdef WIN32
#include <mmintrin.h>
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#pragma warning(disable:4146)
#endif

namespace bitfoot
{

//----------------------------------------------------------------------------
#define COLOR_OF(x)       ((x) & ColorMask)
#define IS_COLOR(x)       (((x) == White) || ((x) == Black))
#define NOT_SQUARE(x)     ((x) & ~0x3F)
#define IS_SQUARE(x)      (!NOT_SQUARE(x))
#define IS_PIECE(x)       (((x) >= WhitePawn) && ((x) <= BlackKing))
#define IS_MOVE_TYPE(x)   (((x) >= Normal) && ((x) <= CastleLong))
#define IS_X(x)           (((x) >= 'a') && ((x) <= 'h'))
#define IS_Y(x)           (((x) >= '1') && ((x) <= '8'))
#define TO_X(x)           ((x) - 'a')
#define TO_Y(x)           ((x) - '1')
#define XC(x)             ((x) % 8)
#define YC(x)             ((x) / 8)
#define SQR(x,y)          ((x) + (8 * (y)))
#define LOW_BIT(x)        ((x) & -(x))
#define MULTI_BIT(x)      ((x) & ((x) - 1))
#define SINGLE_BIT(x)     ((x) && ((x) == LOW_BIT(x)))
#define SINGLE_BITX(x)    ((x) == LOW_BIT(x))
#define BELOW(x)          ((x) - 1)
#define ABOVEX(x)         ((x) ^ -(x))
#define ABOVE(x)          ((x) ? ABOVEX(x) : _ALL)

//----------------------------------------------------------------------------
#ifdef USE_SHIFT
#define BIT(x) (1ULL << (x))
#else
#define BIT(x) _BITMAP[x]
extern const uint64_t _BITMAP[64];
#endif

//----------------------------------------------------------------------------
typedef bool Color;

//----------------------------------------------------------------------------
enum ColorName {
  White,
  Black
};

//----------------------------------------------------------------------------
enum SquareName {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  NoSquare
};

//----------------------------------------------------------------------------
enum Direction
{
  SouthWest  = -9,
  South      = -8,
  SouthEast  = -7,
  West       = -1,
  OtherDir   =  0,
  East       =  1,
  NorthWest  =  7,
  North      =  8,
  NorthEast  =  9
};

//----------------------------------------------------------------------------
enum PieceValue {
  PawnValue   = 100,
  KnightValue = 300,
  BishopValue = 320,
  RookValue   = 500,
  QueenValue  = 975
};

//----------------------------------------------------------------------------
enum BasePieceType
{
  Pawn   = 2,
  Knight = 4,
  Bishop = 6,
  Rook   = 8,
  Queen  = 10,
  King   = 12
};

//----------------------------------------------------------------------------
enum PieceType
{
  WhitePawn   = (White|Pawn),
  BlackPawn   = (Black|Pawn),
  WhiteKnight = (White|Knight),
  BlackKnight = (Black|Knight),
  WhiteBishop = (White|Bishop),
  BlackBishop = (Black|Bishop),
  WhiteRook   = (White|Rook),
  BlackRook   = (Black|Rook),
  WhiteQueen  = (White|Queen),
  BlackQueen  = (Black|Queen),
  WhiteKing   = (White|King),
  BlackKing   = (Black|King)
};

//----------------------------------------------------------------------------
enum MoveType
{
  NoMove,
  Normal,
  PawnPush,
  PawnLung,
  PawnCapture,
  EnPassant,
  KingMove,
  CastleShort,
  CastleLong
};

//----------------------------------------------------------------------------
enum MoveGenStage
{
  NotStarted,
  PromosAndCaptures,
  QuietMoves,
  Finished
};

//----------------------------------------------------------------------------
enum MoveGenType
{
  AllMoves,
  Quiescence
};

//----------------------------------------------------------------------------
enum CastleType {
  WhiteShort = 0x02,
  WhiteLong  = 0x04,
  BlackShort = 0x08,
  BlackLong  = 0x10
};

//----------------------------------------------------------------------------
enum Limits
{
  MoveTypeCount  = 8,
  PieceTypeCount = 14,
  MaxPlies       = 100,
  MaxMoves       = 128,
  StartMaterial  = ((8 * PawnValue) + (2 * KnightValue) +
                    (2 * BishopValue) + (2 * RookValue) +  QueenValue),
  WinningScore   = 30000,
  MateScore      = 31000,
  Infinity       = 32000
};

//----------------------------------------------------------------------------
enum StateMask {
  ColorMask       = 0x001,
  WhiteCastleMask = (WhiteShort|WhiteLong),
  BlackCastleMask = (BlackShort|BlackLong),
  CastleMask      = (WhiteCastleMask|BlackCastleMask),
  StateMask       = (ColorMask|CastleMask),
  Draw            = 0x020,
  Check           = 0x040
};

//----------------------------------------------------------------------------
enum BitMask {
  OneBit          = 0x1,
  TwoBits         = 0x3,
  ThreeBits       = 0x7,
  FourBits        = 0xF,
  FiveBits        = 0x1F,
  SixBits         = 0x3F,
  SevenBits       = 0x7F,
  EightBits       = 0xFF
};

//----------------------------------------------------------------------------
static inline std::string SqrStr(const int sqr)
{
  assert(IS_SQUARE(sqr));
  std::string str;
  str += ('a' + XC(sqr));
  str += ('1' + YC(sqr));
  return str;
}

//----------------------------------------------------------------------------
static inline int BitCount(uint64_t x)
{
#ifdef _WIN32
  return static_cast<int>(_mm_popcnt_u64(x));
#else
  return __builtin_popcountll(x);
#endif
}

//----------------------------------------------------------------------------
static inline int LowSquare(const uint64_t x)
{
#ifdef WIN32
  unsigned long index;
  _BitScanForward64(&index, x);
  return static_cast<int>(index);
#else
#ifdef ASM_POPCNT
  unsigned long index;
  __asm__
  (
      "bsfq %[x], %[index]"
      :[index] "=r" (index)
      :[x] "mr" (x)
  );
  return static_cast<int>(index);
#else
  return static_cast<int>(__builtin_ctzll(x));
#endif
#endif
}

//----------------------------------------------------------------------------
static inline int HighSquare(const uint64_t x)
{
#ifdef WIN32
  unsigned long index;
  _BitScanReverse64(&index, x);
  return static_cast<int>(index);
#else
#ifdef ASM_POPCNT
  unsigned long index;
  __asm__
  (
      "bsrq %[x], %[index]"
      :[index] "=r" (index)
      :[x] "mr" (x)
  );
  return static_cast<int>(index);
#else
  return static_cast<int>(H8 - __builtin_clzll(x));
#endif
#endif
}

//----------------------------------------------------------------------------
static inline uint64_t HighBitNonZero(const uint64_t x)
{
  return BIT(HighSquare(x));
}

//----------------------------------------------------------------------------
static inline uint64_t HighBit(const uint64_t x)
{
  return (x ? BIT(HighSquare(x)) : 0ULL);
}

//----------------------------------------------------------------------------
static inline int PopLowSquare(uint64_t& x, int& result)
{
  result = LowSquare(x);
  x ^= LOW_BIT(x);
  return result;
}

//----------------------------------------------------------------------------
static inline int PopHighSquare(uint64_t& x, int& result)
{
  result = HighSquare(x);
  x ^= HighBitNonZero(x);
  return result;
}

} // namespace bitfoot

#endif // BITFOOT_DEFS_H
