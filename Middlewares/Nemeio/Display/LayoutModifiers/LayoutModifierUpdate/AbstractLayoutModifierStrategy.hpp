/*
 * AbstractLayoutModifierStrategy.hpp
 *
 *  Created on: Aug 3, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERS_LAYOUTMODIFIERUPDATE_ABSTRACTLAYOUTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERS_LAYOUTMODIFIERUPDATE_ABSTRACTLAYOUTMODIFIERSTRATEGY_HPP_

#include "ILayoutModifierStrategy.hpp"

class AbstractLayoutModifierStrategy : public ILayoutModifierStrategy
{
public:
    explicit AbstractLayoutModifierStrategy() = default;
    virtual ~AbstractLayoutModifierStrategy() = default;
};


#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERS_LAYOUTMODIFIERUPDATE_ABSTRACTLAYOUTMODIFIERSTRATEGY_HPP_ */
