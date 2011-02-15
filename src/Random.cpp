/*
 *
 * random.cpp
 *
 * Implimentation of random number generator class
 *
 *
 */
#include "Random.h"
#include <unistd.h>
#include <sys/time.h>
#include <cassert>

Random::Random() {
  Refresh();
}

Random::Random(unsigned int seed)
  : m_seed(seed) 
{
  srand(seed);
}

int Random::Next(int lowbound, int highbound) const {
  assert(highbound > lowbound);

  int m = highbound - lowbound + 1;
  return lowbound + static_cast<int>(m * (static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0)));
}

double Random::Unit() const {
  return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0);
}

void Random::Refresh() {
  timeval t;
  gettimeofday(&t, NULL);
  SetSeed(t.tv_usec);
  srand(t.tv_usec);
}

unsigned int Random::GetSeed() const
{
  return m_seed;
}

void Random::SetSeed(unsigned int seed)
{
  m_seed = seed;
}
