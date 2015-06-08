//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>, All rights reserved
//----------------------------------------------------------------------------

#ifndef BITFOOT_MOVE_H
#define BITFOOT_MOVE_H

#include "Defs.h"

namespace bitfoot
{

class Move {
public:
  //--------------------------------------------------------------------------
  enum Shifts {
    FromShift  = 4,  ///< Number of bits the 'from' square is shifted left
    ToShift    = 10, ///< Number of bits the 'to' square is shifted left
    PcShift    = 16, ///< Number of bits the 'piece' value is shifted left
    CapShift   = 20, ///< Number of bits the 'cap' value is shifted left
    PromoShift = 24  ///< Number of bits the 'promo' value is shifted left
  };

  //--------------------------------------------------------------------------
  static bool LexicalCompare(const Move& a, const Move& b) {
    return (strcmp(a.ToString().c_str(), b.ToString().c_str()) < 0);
  }

  //--------------------------------------------------------------------------
  static bool ScoreCompare(const Move& a, const Move& b) {
    return (a.score < b.score);
  }

  //--------------------------------------------------------------------------
  Move(const uint32_t bits = 0, const int score = 0)
    : bits(bits),
      score(score)
  {
    assert(abs(score) <= Infinity);
  }

  //--------------------------------------------------------------------------
  Move(const Move& other)
    : bits(other.bits),
      score(other.score)
  {
    assert(abs(score) <= Infinity);
  }

  //--------------------------------------------------------------------------
  void Init(const uint32_t bits, const int score) {
    assert(bits);
    assert(abs(score) <= Infinity);
    this->bits = bits;
    this->score = static_cast<int32_t>(score);
    assert(IsValid());
  }

  //--------------------------------------------------------------------------
  void Init(const MoveType type,
            const int from,
            const int to,
            const int pc,
            const int cap = 0,
            const int promo = 0,
            const int score = 0)
  {
    assert(IS_MOVE_TYPE(type));
    assert(IS_SQUARE(from));
    assert(IS_SQUARE(to));
    assert(from != to);
    assert(IS_PIECE(pc));
    assert(!cap || ((cap >= Pawn) && (cap < King)));
    assert(!promo || ((promo >= Knight) && (promo < King)));
    assert(abs(score) <= Infinity);
    bits = (static_cast<uint32_t>(type)               |
            static_cast<uint32_t>(from  << FromShift) |
            static_cast<uint32_t>(to    << ToShift)   |
            static_cast<uint32_t>(pc    << PcShift)   |
            static_cast<uint32_t>(cap   << CapShift)  |
            static_cast<uint32_t>(promo << PromoShift));
    this->score = static_cast<int32_t>(score);
    assert(IsValid());
  }

  //--------------------------------------------------------------------------
  Move& operator=(const Move& other) {
    assert(abs(other.GetScore()) <= Infinity);
    bits = other.bits;
    score = other.score;
    return *this;
  }

  //--------------------------------------------------------------------------
  void SwapWith(Move& other) {
    const uint32_t tbits  = bits;
    const int32_t  tscore = score;
    bits  = other.bits;
    score = other.score;
    other.bits  = tbits;
    other.score = tscore;
  }

  //--------------------------------------------------------------------------
  int GetScore() const {
    return score;
  }

  //--------------------------------------------------------------------------
  int& Score() {
    return score;
  }

  //--------------------------------------------------------------------------
  uint32_t GetBits() const {
    return bits;
  }

  //--------------------------------------------------------------------------
  MoveType GetType() const {
    return static_cast<MoveType>(bits & FourBits);
  }

  //--------------------------------------------------------------------------
  int GetFrom() const {
    return static_cast<int>((bits >> FromShift) & SixBits);
  }

  //--------------------------------------------------------------------------
  int GetTo() const {
    return static_cast<int>((bits >> ToShift) & SixBits);
  }

  //--------------------------------------------------------------------------
  int GetPc() const {
    return static_cast<int>((bits >> PcShift) & FourBits);
  }

  //--------------------------------------------------------------------------
  int GetCap() const {
    return static_cast<int>((bits >> CapShift) & FourBits);
  }

  //--------------------------------------------------------------------------
  int GetPromo() const {
    return static_cast<int>((bits >> PromoShift) & FourBits);
  }

  //--------------------------------------------------------------------------
  bool IsValid() const {
    return (GetType() && (GetFrom() != GetTo()));
  }

  //--------------------------------------------------------------------------
  bool IsCapOrPromo() const {
    return (bits & 0xFF00000UL);
  }

  //--------------------------------------------------------------------------
  int GetHistoryIndex() const {
    return ((bits >> FromShift) & 0xFFFF);
  }

  //--------------------------------------------------------------------------
  bool operator==(const Move& other) const {
    return (bits && (bits == other.bits));
  }

  //--------------------------------------------------------------------------
  bool operator!=(const Move& other) const {
    return (!bits || (bits != other.bits));
  }

  //--------------------------------------------------------------------------
  bool operator<(const Move& other) const {
    return (score < other.score);
  }

  //--------------------------------------------------------------------------
  void Clear() {
    bits = 0;
    score = 0;
  }

  //--------------------------------------------------------------------------
  std::string ToString() const {
    std::string str;
    if (IsValid()) {
      str += SqrStr(GetFrom());
      str += SqrStr(GetTo());
      switch (GetPromo()) {
      case WhiteKnight: case BlackKnight: return (str + "n");
      case WhiteBishop: case BlackBishop: return (str + "b");
      case WhiteRook:   case BlackRook:   return (str + "r");
      case WhiteQueen:  case BlackQueen:  return (str + "q");
      default:
        break;
      }
    }
    return str;
  }

private:
  uint32_t bits;
  int32_t score;
};

} // namespace bitfoot

#endif // BITFOOT_MOVE_H
