Bitfoot
======

UCI Chess engine by Shawn Chidester <zd3nik@gmail.com>

This is [Clubfoot](https://github.com/zd3nik/Clubfoot) with bitboards.  I intend to keep this engine and Clubfoot as similar as possible in terms of search mechanics.  Only board representaion, move generation, and positional analsys should be different.  The use of bitboards makes Bitfoot much faster than Clubfoot.  It also makes positional evaluation easier/faster.  With Bitfoot being faster and more positionally aware than Clubfoot, Bitfoot is the stronger engine.

I also show a simpler way of using my [Senjo UCI Adapter](https://github.com/zd3nik/SenjoUCIAdapter) in this engine.  In Clubfoot the Senjo UCI Adapter is included as a git sub-module.  In this engine I simply include the Senjo source code in the Bitfoot project.  This makes cloning the Bitfoot project easier.  It also means I can make Bitfoot-specific customizations to the UCIAdapter code if I want without needing to push those changes back to the Senjo project.


Intent
-----

As with most of my projects I start out with one goal and end up with a different goal once I get the project fleshed out a bit.  This project started as a revamp of an older chess engine I wrote named Hiroki.  But I started by cloning Clubfoot and replacing the board representaion, move generation, and positional evaluation with Hiroki code.  Only I ended up doing the move generation differently than I did in Hiroki.  What made Hiroki unique was its move generator.  With that part lost this "revamped" version of Hiroki ceased to be Hirko and became simply Clubfoot with bitboards.  So that's what it is and that's how it shall stay.


A/B Bitboards
------------

Above/Below Bitboards.

I've experimented with several board representations and move generation techniques over the years.  For `bitboard` engines I've tried rotated bitboards and I've looked into magic bitboards.  I wasn't very happy with rotated bitboards.  The implementation is a bit messy as there is too much maintenance of bit tables involved, resulting in a lot of writes to memory which makes it relatively slow.  And although magic bitboards seem to be very popular I opted not to use them because I like to understand how my code works.  Magic bitboards are pretty cool, but I don't understand how multiplying a bitboard of occupied squares with a "magic number" yields a series of sequential high order bits equal to the number of spaces a slider can move.  My A/B bitboard approach is much more intuitive (to me) and performs better than my rotated bitboards implementations.

The Above/Below idea goes something like this:  In terms of raw bit positions all the bits in a given direction are either above or below the bit of the square you're originating from.  This makes finding the first intersection with an occupied square in any given direction rather simple.  If the direction is "above" the origin square find the lowest intersection bit in that direction.  If the direction is "below" the origin square find the highest intersection bit in that direction.

Finding intersection bits in a specific direction from any square is done by AND-ing a bitmask of all the bits in the desired direction from the origin square with the bitboard of all occupied squares.

### Above

Code for finding the first occupied square South-East from some square:

    SouthEastX(square) {
      uint64_t intersects = (_SOUTH_EAST[square] & OccupiedSquares);
      return HIGH_BIT(intersects);
    }

    _SOUTH_EAST[B7]   OccupiedSquares   intersects        HIGH_BIT(intersects)
    - - - - - - - -   X X - X X X X X   - - - - - - - -   - - - - - - - -
    - - - - - - - -   X X X X X X X X   - - - - - - - -   - - - - - - - -
    - - X - - - - -   - X - - - - - -   - - - - - - - -   - - - - - - - -
    - - - X - - - -   - - - - - - - -   - - - - - - - -   - - - - - - - -
    - - - - X - - -   - - - - X - - -   - - - - X - - -   - - - - X - - -
    - - - - - X - -   - - X - - - - -   - - - - - - - -   - - - - - - - -
    - - - - - - X -   X X X X - X X X   - - - - - - X -   - - - - - - - -
    - - - - - - - X   X - X X X X X X   - - - - - - - X   - - - - - - - -
    
Code for finding all unoccupied squares South-East from some square:

    SouthEastO(square) {
      uint64_t capture = SouthEastX(square);
      return (ABOVE(capture) & _SOUTH_EAST[square]);
    }

    capture           ABOVE(capture)    _SOUTH_EAST[B7]   result
    - - - - - - - -   X X X X X X X X   - - - - - - - -   - - - - - - - -
    - - - - - - - -   X X X X X X X X   - - - - - - - -   - - - - - - - -
    - - - - - - - -   X X X X X X X X   - - X - - - - -   - - X - - - - -
    - - - - - - - -   X X X X X X X X   - - - X - - - -   - - - X - - - -
    - - - - X - - -   - - - - - X X X   - - - - X - - -   - - - - - - - -
    - - - - - - - -   - - - - - - - -   - - - - - X - -   - - - - - - - -
    - - - - - - - -   - - - - - - - -   - - - - - - X -   - - - - - - - -
    - - - - - - - -   - - - - - - - -   - - - - - - - X   - - - - - - - -

The "ABOVE" macro:

    #define ABOVE(x) ((x) ^ -(x))

### Below

Code for finding the first occupied square North-East from some square:

    NorthEastX(square) {
      uint64_t intersects = (_NORTH_EAST[square] & OccupiedSquares);
      return LOW_BIT(intesects);
    }

    _NORTH_EAST[B2]   OccupiedSquares   intersects        LOW_BIT(intersects)
    - - - - - - - X   X X X X X X X X   - - - - - - - X   - - - - - - - -
    - - - - - - X -   X X X X - X - X   - - - - - - - -   - - - - - - - -
    - - - - - X - -   - - - - - - X -   - - - - - - - -   - - - - - - - -
    - - - - X - - -   - - - - X - - -   - - - - X - - -   - - - - X - - -
    - - - X - - - -   - - - - - - - -   - - - - - - - -   - - - - - - - -
    - - X - - - - -   - X - - - - - -   - - - - - - - -   - - - - - - - -
    - - - - - - - -   X X X X X X X X   - - - - - - - -   - - - - - - - -
    - - - - - - - -   X X - X X X X X   - - - - - - - -   - - - - - - - -

Code for finding all unoccupied squares North-East from some square:

    NorthEastO(square) {
      uint64_t capture = NorthEastX(square);
      return (BELOW(x) & _NORTH_EAST[square]);
    }

    capture           BELOW(capture)    _NORTH_EAST[B2]   result
    - - - - - - - -   - - - - - - - -   - - - - - - - X   - - - - - - - -
    - - - - - - - -   - - - - - - - -   - - - - - - X -   - - - - - - - -
    - - - - - - - -   - - - - - - - -   - - - - - X - -   - - - - - - - -
    - - - - X - - -   X X X X - - - -   - - - - X - - -   - - - - - - - -
    - - - - - - - -   X X X X X X X X   - - - X - - - -   - - - X - - - -
    - - - - - - - -   X X X X X X X X   - - X - - - - -   - - X - - - - -
    - - - - - - - -   X X X X X X X X   - - - - - - - -   - - - - - - - -
    - - - - - - - -   X X X X X X X X   - - - - - - - -   - - - - - - - -

The "BELOW" macro:

    #define BELOW(x) ((x) - 1)

