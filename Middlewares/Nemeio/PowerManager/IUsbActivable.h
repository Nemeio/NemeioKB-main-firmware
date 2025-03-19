/*
 * IUsbActivable.h
 *
 *  Created on: Mar 28, 2018
 *      Author: amongatcheucheu
 */

#ifndef POWERMANAGER_IUSBACTIVABLE_H_
#define POWERMANAGER_IUSBACTIVABLE_H_

class IUsbActivable
{
public:
    IUsbActivable() = default;
    virtual ~IUsbActivable() = default;

    virtual void usbPluggedInComputer() = 0;
    virtual void usbPluggedInComputerSuspended() = 0;
    virtual void usbPluggedInPowerSupplyOnly() = 0;
    virtual void usbPluggedOut() = 0;
};

#endif /* POWERMANAGER_IUSBACTIVABLE_H_ */
