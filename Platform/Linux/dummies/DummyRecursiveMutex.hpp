
#ifndef DUMMIES_DUMMYRECURSIVEMUTEX_HPP_
#define DUMMIES_DUMMYRECURSIVEMUTEX_HPP_

#include "AbstractRecursiveMutex.hpp"

class DummyRecursiveMutex : public AbstractRecursiveMutex
{
public:
    bool unlock() { return true; };

private:
    bool doLock(const DelayMs& timeout) { return true; };
};

#endif /* MOCKS_MOCKAPPCOMMTXLISTENER_HPP_ */
