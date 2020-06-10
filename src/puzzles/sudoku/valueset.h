#pragma once
#include <cinttypes>
#include <string>
#include <iostream>
using namespace std;

#define MASK0 0xFFFFFFFFFFFFFFFF
#define NBITS 64

class ValueSet
{	
/* 
  Set operations implemented using bitmaps. 
*/
private:
	uint64_t bitmap;
	uint64_t mask;
    int32_t nMax;

/* msb and popcnt:
   Portable functions for finding leftmost bit, and counting bits.
   Both of these operations could be optimized by using native 
   instructions (e.g. POPCNT, LZCNT)
 */
	inline int msb( uint64_t x ) const
	{
		/* Return position of leftmost bit in x.
		   Uses a trick from Knuth's 'Art of Computer Programming'
		   vol 4A, p142 ('Working with Leftmost Bits')
		*/
		double dx = (double)x;
		return ( *(uint64_t*)(&dx) - 0x3FF0000000000000)>>52;
	}
#define MU0 0x5555555555555555
#define MU1 0x3333333333333333
#define MU2 0x0f0f0f0f0f0f0f0f
#define MU3 0x00ff00ff00ff00ff
#define MU4 0x0000ffff0000ffff
#define MU5 0x00000000ffffffff
	inline int lsb( uint64_t x ) const
	{
		/* Return position of rightmost bit in x.
		   Uses a trick from Knuth's 'Art of Computer Programming'
		   vol 4A, p141 ('Working with the Rightmost Bits')
		*/
		uint64_t y   = (uint64_t)(-(int64_t)x);
		y           &= x;
		uint64_t q   = y&MU5;
		uint64_t rho = q == 0 ? 32 : 0;
		q            = y&MU4;
		uint64_t t   = rho + 16;
		if (q == 0)
			rho = t;
		q            = y&MU3;
		t            = rho + 8;
		if (q == 0)
			rho = t;
		q            = y&MU2;
		t            = rho + 4;
		if (q == 0)
			rho = t;
		q            = y&MU1;
		t            = rho + 2;
		if (q == 0)
			rho = t;
		q            = y&MU0;
		t            = rho + 1;
		if (q == 0)
			rho = t;
		return (int)rho;
	}
#define A 0x0101010101010101
	inline int popcnt( uint64_t x ) const
	{
		/* Count the non-zero bits in x
		   Uses a trick from Knuth's 'Art of Computer Programming'
		   vol 4A, p143 ('Sideways Addition')
		*/
		uint64_t y = x - ((x>>1)&MU0);
		y = (y&MU1) + ( (y>>2)&MU1);
		y = (y + (y>>4))&MU2;
		return (A*y) >> 56;
	}

public:
	ValueSet( int32_t nMax, uint64_t initialVal = 0) : nMax(nMax), mask(MASK0 >> (NBITS - nMax)), bitmap(initialVal) {}
	ValueSet() : bitmap(0) {}

	void Init( int32_t nnMax ) 
	{
		bitmap = 0;
		nMax = nnMax;
		mask = MASK0 >> (NBITS - nMax);
	};

	void Clear() { bitmap = 0; }

	void Add(uint64_t v) { bitmap |= v; }
	void Remove(uint64_t v) { bitmap &= (mask & ~v); }
	
	void IdxAdd(int i) { Add((uint64_t)1 << (i-1)); }
	void IdxRemove(int i) { Remove((uint64_t)1 << (i-1)); }

	std::string toString( const std::string &alphabet ) const
	{
		// for debugging - make a human-readable string of the cell set
		if (Empty())
			return std::string("-");
		else
		{
			if ( Fixed() )
			{
				return alphabet.substr(Index(),1);
			}
			else
			{
				std::string retVal = std::string("(");
				for (uint64_t i = 0; i < nMax; i++)	
				{
					if (bitmap&((uint64_t)1 << i))
						retVal = retVal + alphabet.substr((unsigned int)i,1);
				}
				retVal = retVal + ")";
				return retVal;
			}
		}
	}
	bool Contains(uint64_t val) const
	{
		return (val&bitmap) != 0;
	}

	bool Contains(const ValueSet &other) const
	{
		return (bitmap&other.bitmap) != 0;
	}
	int Count() const
	{
		// return number of bits set to 1
		return popcnt(bitmap);
	}
	bool Fixed() const
	{
		// return true if the set has only one value (i.e. one bit set in the bitmap)
		return ((uint64_t)1<<Index() == bitmap);
	}
	bool Empty() const
	{
		return bitmap == (uint64_t)0;
	}
	int Index() const
	{
		return msb(bitmap);
	}
	int Highest() const
	{
		return Empty() ? 0 : msb(bitmap)+1;
	}
	int Lowest() const
	{
		return Empty() ? 0 : lsb(bitmap)+1;
	}
	ValueSet Union( const ValueSet &other ) const
	{ 
		return ValueSet( nMax, bitmap | other.bitmap);
	}
	ValueSet Intersection(const ValueSet &other) const
	{
		return ValueSet(nMax, bitmap & other.bitmap);
	}
	ValueSet Difference( const ValueSet &other ) const
	{
		return ValueSet(nMax, mask & (bitmap & ~other.bitmap));
	}
	ValueSet Complement()  const
	{
		return ValueSet(nMax, mask & (~bitmap));
	}
	ValueSet operator+(const ValueSet &other)  const
	{
		return Union(other);
	}
	ValueSet operator^(const ValueSet &other) const
	{
		return Intersection(other);
	}
	ValueSet operator-(const ValueSet &other) const
	{
		return Difference(other);
	}
	void operator += (const ValueSet &other)
	{
		bitmap |= other.bitmap;
	}
	void operator ^= (const ValueSet &other)
	{
		bitmap &= other.bitmap;
	}
	void operator -= (const ValueSet &other)
	{
		bitmap = mask & (bitmap & (~other).bitmap);
	}
	ValueSet operator~() const
	{
		return Complement();
	}
	void operator <<= (int shift)
	{
		bitmap <<= shift;
	}
	bool operator == (const ValueSet &other) const
	{
		return bitmap == other.bitmap;
	}
};
