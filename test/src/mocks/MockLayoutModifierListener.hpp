#ifndef MOCKS_MOCKLAYOUTMODIFIERLISTENER_HPP_
#define MOCKS_MOCKLAYOUTMODIFIERLISTENER_HPP_

#include "gmock/gmock.h"
#include "ILayoutModifierListener.hpp"

class MockLayoutModifierListener : public ILayoutModifierListener
{
public:
    MOCK_METHOD(void,
                layoutModifiersStateChanged,
                (const LayoutModifiersDefs::LayoutModifierFlags& flags),
                (override));
};

#endif /* MOCKS_MOCKLAYOUTMODIFIERLISTENER_HPP_ */
