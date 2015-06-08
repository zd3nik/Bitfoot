//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>, All rights reserved
//----------------------------------------------------------------------------

#ifndef BITFOOT_HASH_H
#define BITFOOT_HASH_H

#include "Move.h"

namespace bitfoot
{

//----------------------------------------------------------------------------
extern const uint64_t _HASH[PieceTypeCount][64];

//----------------------------------------------------------------------------
struct HashEntry
{
public:
  enum Flag {
    // pimary flags
    Checkmate   = 0x01,
    Stalemate   = 0x02,
    UpperBound  = 0x03,
    ExactScore  = 0x04,
    LowerBound  = 0x05,
    PrimaryMask = 0x07,

    // other flags
    Extended    = 0x08,
    FromPV      = 0x10,
    OtherMask   = 0x18
  };

  //--------------------------------------------------------------------------
  //! \return The primary flag assigned to this entry
  //--------------------------------------------------------------------------
  int GetPrimaryFlag() const {
    return (flags & HashEntry::PrimaryMask);
  }

  //--------------------------------------------------------------------------
  //! \return true if the Extended flag is set on this entry
  //--------------------------------------------------------------------------
  bool HasExtendedFlag() const {
    return (flags & HashEntry::Extended);
  }

  //--------------------------------------------------------------------------
  //! \return true if the FromPV flag is set on this entry
  //--------------------------------------------------------------------------
  bool HasPvFlag() const {
    return (flags & HashEntry::FromPV);
  }

  uint64_t positionKey;
  uint32_t moveBits;
  int16_t  score;
  uint8_t  depth;
  uint8_t  flags;
};

//----------------------------------------------------------------------------
class TranspositionTable
{
public:
  //--------------------------------------------------------------------------
  TranspositionTable()
    : keyMask(0),
      entries(NULL)
  { }

  //--------------------------------------------------------------------------
  ~TranspositionTable() {
    delete[] entries;
    entries = NULL;
    keyMask = 0;
  }

  //--------------------------------------------------------------------------
  bool Resize(const size_t mbytes) {
    delete[] entries;
    entries = NULL;
    keyMask = 0;

    const uint64_t bytes   = (mbytes * 1024 * 1024);
    const uint64_t count   = (bytes / sizeof(HashEntry));
    const uint64_t highBit = HighBit(count + 1);

    // if highBit is 0 we've shifted beyond size_t bit count (e.g. too big!)
    if (!highBit) {
      return false;
    }

    keyMask = (highBit - 1);
    if (!keyMask) {
      return true;
    }
    if (!(entries = new HashEntry[keyMask + 1])) {
      return false;
    }

    Clear();
    return true;
  }

  //--------------------------------------------------------------------------
  void Clear() {
    ResetCounters();
    if (entries) {
      memset(entries, 0, (sizeof(HashEntry) * (keyMask + 1)));
    }
  }

  //--------------------------------------------------------------------------
  HashEntry* Probe(const uint64_t key) {
    if (key && entries) {
      HashEntry* entry = (entries + (key & keyMask));
      if (entry->positionKey == key) {
        _hits++;
        return entry;
      }
    }
    return NULL;
  }

  //--------------------------------------------------------------------------
  void Store(const uint64_t key,
             const Move& bestmove,
             const int depth,
             const int primaryFlag,
             const int otherFlags)
  {
    assert(bestmove.IsValid());
    assert(abs(bestmove.GetScore()) < Infinity);
    assert((depth >= 0) && (depth < 256));
    assert((primaryFlag == HashEntry::LowerBound) ||
           (primaryFlag == HashEntry::UpperBound) ||
           (primaryFlag == HashEntry::ExactScore));
    assert(!(otherFlags & ~HashEntry::OtherMask));

    if (key && entries) { // TODO && (key != tt->key or depth >= tt->depth)
      _stores++;
      HashEntry* entry   = (entries + (key & keyMask));
      entry->positionKey = key;
      entry->moveBits    = bestmove.GetBits();
      entry->score       = static_cast<int16_t>(bestmove.GetScore());
      entry->depth       = static_cast<uint8_t>(depth);
      entry->flags       = static_cast<uint8_t>(primaryFlag | otherFlags);
    }
  }

  //--------------------------------------------------------------------------
  void StoreCheckmate(const uint64_t key) {
    if (key && entries) {
      _checkmates++;
      HashEntry* entry   = (entries + (key & keyMask));
      entry->positionKey = key;
      entry->moveBits    = 0;
      entry->score       = Infinity;
      entry->depth       = 0;
      entry->flags       = HashEntry::Checkmate;
    }
  }

  //--------------------------------------------------------------------------
  void StoreStalemate(const uint64_t key) {
    if (key && entries) {
      _stalemates++;
      HashEntry* entry   = (entries + (key & keyMask));
      entry->positionKey = key;
      entry->moveBits    = 0;
      entry->score       = 0;
      entry->depth       = 0;
      entry->flags       = HashEntry::Stalemate;
    }
  }

  //--------------------------------------------------------------------------
  void ResetCounters() {
    _stores = 0;
    _hits = 0;
    _checkmates = 0;
    _stalemates = 0;
  }

  //--------------------------------------------------------------------------
  uint64_t GetStores() const { return _stores; }
  uint64_t GetHits() const { return _hits; }
  uint64_t GetCheckmates() const { return _checkmates; }
  uint64_t GetStalemates() const { return _stalemates; }

private:
  static uint64_t _stores;
  static uint64_t _hits;
  static uint64_t _checkmates;
  static uint64_t _stalemates;

  size_t     keyMask;
  HashEntry* entries;
};

} // namespace bitfoot

#endif // BITFOOT_HASH_H
