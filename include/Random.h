/*
 *
 * Random.h
 *
 * Header file for random number class
 * intended to fill an array for sorting
 *
 *
 */
#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

class Random {
public:
  Random();
  Random(unsigned int seed);
  // [lowbound, highbound]
  int  Next(int lowbound=0, int highbound=RAND_MAX-1) const;
  // [0, 1)
  double Unit() const;
  void Refresh();
  void SetSeed(unsigned int seed);
  unsigned int GetSeed() const;

private:
    unsigned int m_seed;
};

#endif
