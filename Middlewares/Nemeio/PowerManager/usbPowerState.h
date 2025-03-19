/*
 * usbPowerState.h
 *
 *  Created on: Mar 26, 2018
 *      Author: amongatcheucheu
 */

#ifndef NEMEIO_POWERMANAGER_USBPOWERSTATE_H_
#define NEMEIO_POWERMANAGER_USBPOWERSTATE_H_

enum USB_POWER_CONNECTION_STATE
{
    NONE,
    PLUG_IN_PC,
    PLUG_IN_PC_SUSPENDED,
    PLUG_IN_POWER_SUPPLY_ONLY,
    PLUG_OUT
};

#ifdef __cplusplus
extern "C" {
#endif
void pushUsbState(enum USB_POWER_CONNECTION_STATE sate);
void notifyUsbSuspend();
void notifyUsbResume();
void notifyUsbConnect();
void notifyUsbDisconnect();
#ifdef __cplusplus
}
#endif

#endif /* NEMEIO_POWERMANAGER_USBPOWERSTATE_H_ */
