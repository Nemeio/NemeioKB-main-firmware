#include "FileSystemTrimming.hpp"
#include "assertTools.h"
#include "systemUtils.h"
#include "Application.hpp"
#include <sstream>
#include <utility>
#include <string.h>
#include <algorithm>

#include "pinConfig.h"
#include "main.h"

#include "i2c.h"
#include "usb_device.h"
#include "usbd_custom_composite.h"
#include "stm32l4xx_hal_tim.h"

#include "stm32l4xx_hal_flash.h"
#include "stm32l4xx_hal_flash_ex.h"

#include "timing_macros.h"

#include "../Libraries/NemeioKeypad/NemeioKeypad.h"
#include "KeyProtocolDefs.hpp"
#include "usbd_desc.h"

#include "etl/algorithm.h"
#include "etl/iterator.h"

bool operator==(ButtonID const& a, ButtonID const& b);

void Application::RegisterHIDMessageListener()
{
    computerHIDMessageHandler.registerHID(mHardwareAssembly.getBleHidItem());
    computerHIDMessageHandler.registerHID(mHardwareAssembly.getUsbItem());
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem().registerHIDKeyMessageHandler(
        computerHIDMessageHandler);
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem().registerHIDKeyMessageHandler(
        mDoubleClickKeyHandlerAssembly.getItem());
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem().registerHIDKeyMessageHandler(
        mLayoutModifiersStateAssembly.getLayoutModifiersStateItem());
}

void Application::RegisterCustomKeyProtocolMessageListener()
{
    customKeyProtocolMessageHandler.registerCustomKeyProtocol(mCustomKeyProtocol);
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem()
        .registerCustomProtocolKeyMessageHandler(customKeyProtocolMessageHandler);
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem()
        .registerCustomProtocolKeyMessageHandler(mDoubleClickKeyHandlerAssembly.getItem());
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem()
        .registerCustomProtocolKeyMessageHandler(
            mLayoutModifiersStateAssembly.getLayoutModifiersStateItem());
}

void Application::RegisterPairingKeyMessageListener()
{
    mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem().registerPairingKeyMessageHandler(
        mBLEPairing);
}

Application::Application(AssemblyHardware& hardwareAssembly,
                         FileSystemAssembly& fileSystemAssembly,
                         IScreen& screen,
                         AssemblyHAL& assembly,
                         Displayer& displayer,
                         SystemUpdate& systemUpdate,
                         DeviceProperties deviceProperties,
                         TinyDeflateDecompressor& decompressor,
                         Logger& logger,
                         AppCommProxy& appComm,
                         ISharedBufferContainer& shareBufferContainer)
    : mHardwareAssembly(hardwareAssembly)
    , mFileSystemAssembly(fileSystemAssembly)
    , mAssembly(assembly)
    , mDisplayer(displayer)
    , mScreen(screen)
    , mLogger(logger)
    , mBleActivationStrategy(mHardwareAssembly.getBleItem())
    , mConnectivityState({&appComm, &mBleActivationStrategy, &mKeyLockStatesDispatcher})
    , mCustomKeyProtocol(mKeyPressedCmd, computerHIDMessageHandler)
    , mKeepAliveManager(mHardwareAssembly.getWorkQueueNormalPrio())
    , mFactoryReset(*this, mHardwareAssembly.getSystemItem(), mHardwareAssembly.getBleItem())
    , mDeviceProperties(deviceProperties)
    , mBrightnessManager(mHardwareAssembly.getLedController())
    , mBrightnessButtonHandler(mBrightnessManager, *this)
    , mBrightnessDemoModeHandler(mBrightnessManager)
    , mBrightnessPowerStateHandler(mBrightnessManager)
    , mPowerManagerAssembly(mHardwareAssembly.getSystemItem(),
                            mHardwareAssembly.getPowerController(),
                            {&mBrightnessPowerStateHandler,
                             &mHardwareAssembly.getBleItem(),
                             &mHardwareAssembly.getUsbItem(),
                             &mConnectivityState,
                             this})
    , mConfigurationFileSystem(
          mFileSystemAssembly.getPartitionFileSystem(FileSystemPartitionLabel::MAIN))
    , mFactoryConfigurationFileSystem(
          mFileSystemAssembly.getPartitionFileSystem(FileSystemPartitionLabel::MAIN))
    , mConfigurationStorageAssembly(mConfigurationFileSystem,
                                    mFactoryConfigurationFileSystem,
                                    shareBufferContainer)
    , mUpdateStatusCmd(appComm)
    , mKeepAliveCmd(appComm, mKeepAliveManager, mKeyboardStatus, mKeepAliveRespCodec)
    , mSystemResetCmd(appComm, mHardwareAssembly.getSystemItem())
    , mConfChangedCmd(appComm)
    , mSendDataCmd(appComm)
    , mApplyConfigurationCmd(appComm, mConfigurationSwitcherAssembly.getConfigurationSwitcher())
    , mKeyPressedCmd(appComm)
    , mFormatFlashCmd(appComm, *this)
    , mBatteryInformationCmd(appComm, mAssembly.getBattery())
    , mVersionsCmd(appComm, mHardwareAssembly.getBleItem(), mScreen.getAssociatedScreenController())
    , mReceiveDataCmd(appComm)
    , mSetCommModeCmd(appComm, mKeyProtocolManagerAssembly.getKeyProtocolManagerItem())
    , mSerialNumberCmd(appComm, mDeviceProperties.getSerialNumber())
    , mKeyboardParametersCmd(appComm, *this, mKeyboardParametersCodec, shareBufferContainer)
    , mFactoryResetCmd(appComm, mFactoryReset, mHardwareAssembly.getSystemItem())
    , mValidateUserConfigurationCmd(AppCommDefs::AppCommCmd::VALIDATEUSERCONFIGURATION,
                                    appComm,
                                    mConfigurationStorageAssembly
                                        .getUserConfigurationWritersContainer(),
                                    configuration::VALID_USER_CONFIGURATION_DIRECTORY,
                                    mConfigurationStorageAssembly.getUserConfigurationStorage())
    , mValidateFactoryUserConfigurationCmd(
          AppCommDefs::AppCommCmd::VALIDATEFACTORYUSERCONFIGURATION,
          appComm,
          mConfigurationStorageAssembly.getFactoryUserConfigurationWritersContainer(),
          configuration::VALID_FACTORY_USER_CONFIGURATION_DIRECTORY,
          mConfigurationStorageAssembly.getFactoryUserConfigurationStorage())
    , mDeleteLayoutCmd(AppCommDefs::AppCommCmd::DELETELAYOUT,
                       appComm,
                       mConfigurationStorageAssembly.getLayoutStorage())
    , mDeleteFactoryLayoutCmd(AppCommDefs::AppCommCmd::DELETEFACTORYLAYOUT,
                              appComm,
                              mConfigurationStorageAssembly.getFactoryLayoutStorage())
    , mDeleteUserConfigurationCmd(AppCommDefs::AppCommCmd::DELETEUSERCONFIGURATION,
                                  appComm,
                                  mConfigurationStorageAssembly.getUserConfigurationStorage())
    , mDeleteFactoryUserConfigurationCmd(AppCommDefs::AppCommCmd::DELETEFACTORYUSERCONFIGURATION,
                                         appComm,
                                         mConfigurationStorageAssembly
                                             .getFactoryUserConfigurationStorage())
    , mApplyUserConfigurationCmd(appComm, mConfigurationSwitcherAssembly.getConfigurationSwitcher())

    , mSendDataCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mSendDataCmd)
    , mApplyConfigurationCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                         mApplyConfigurationCmd)
    , mFormatFlashCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mFormatFlashCmd)
    , mVersionsCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mVersionsCmd)
    , mReceiveDataCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mReceiveDataCmd)
    , mSetCommModeCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mSetCommModeCmd)
    , mSerialNumberCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mSerialNumberCmd)
    , mKeyboardParametersCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                         mKeyboardParametersCmd)
    , mFactoryResetCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mFactoryResetCmd)
    , mValidateUserConfigurationCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                                mValidateUserConfigurationCmd)
    , mValidateFactoryUserConfigurationCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                                       mValidateFactoryUserConfigurationCmd)
    , mDeleteLayoutCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(), mDeleteLayoutCmd)
    , mDeleteFactoryLayoutCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                          mDeleteFactoryLayoutCmd)
    , mDeleteUserConfigurationCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                              mDeleteUserConfigurationCmd)
    , mDeleteFactoryUserConfigurationCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                                     mDeleteFactoryUserConfigurationCmd)
    , mApplyUserConfigurationCmdPowerManaged(mPowerManagerAssembly.getPowerManagerItem(),
                                             mApplyUserConfigurationCmd)
    , mFirmwarePackageUpdateCmd(systemUpdate)
    , mSendUserConfigurationCmd(AppCommDefs::SENDDATAID_USER_CONFIGURATION,
                                mConfigurationStorageAssembly.getUserConfigurationWritersContainer(),
                                mConfigurationFileSystem,
                                configuration::NEW_USER_CONFIGURATION_DIRECTORY)
    , mSendFactoryUserConfigurationCmd(AppCommDefs::SENDDATAID_FACTORY_USER_CONFIGURATION,
                                       mConfigurationStorageAssembly
                                           .getFactoryUserConfigurationWritersContainer(),
                                       mFactoryConfigurationFileSystem,
                                       configuration::NEW_USER_CONFIGURATION_DIRECTORY)
    , mSendLayoutConfigurationCmd(AppCommDefs::SENDDATAID_LAYOUT_CONFIGURATION,
                                  mConfigurationStorageAssembly
                                      .getUserConfigurationWritersContainer())
    , mSendFactoryLayoutConfigurationCmd(AppCommDefs::SENDDATAID_FACTORY_LAYOUT_CONFIGURATION,
                                         mConfigurationStorageAssembly
                                             .getFactoryUserConfigurationWritersContainer())
    , mSendLayoutCmd(AppCommDefs::SENDDATAID_LAYOUT,
                     mConfigurationFileSystem,
                     configuration::LAYOUT_DIRECTORY,
                     mConfigurationStorageAssembly.getLayoutWritersContainer())
    , mSendFactoryLayoutCmd(AppCommDefs::SENDDATAID_FACTORY_LAYOUT,
                            mConfigurationFileSystem,
                            configuration::FACTORY_LAYOUT_DIRECTORY,
                            mConfigurationStorageAssembly.getFactoryLayoutWritersContainer())
    , mReceiveEventLogCmd(mFileSystemAssembly.getPartitionFileSystem(EVENT_LOG_PART),
                          EVENT_LOG_FILE_NAME)
    , mReceiveUserConfigurationCmd(AppCommDefs::ReceiveDataID::RCVDATAID_USER_CONFIG,
                                   mConfigurationStorageAssembly.getUserConfigurationStorage())
    , mReceiveFactoryUserConfigurationCmd(AppCommDefs::ReceiveDataID::RCVDATAID_FACTORY_USER_CONFIG,
                                          mConfigurationStorageAssembly
                                              .getFactoryUserConfigurationStorage())
    , mUuidListEncoderContainer(mLayoutListCodec)
    , mReceiveLayoutListCmd(AppCommDefs::ReceiveDataID::RCVDATAID_LAYOUT_LIST,
                            mConfigurationStorageAssembly.getLayoutStorage(),
                            mUuidListEncoderContainer)
    , mReceiveFactoryLayoutListCmd(AppCommDefs::ReceiveDataID::RCVDATAID_FACTORY_LAYOUT_LIST,
                                   mConfigurationStorageAssembly.getFactoryLayoutStorage(),
                                   mUuidListEncoderContainer)
    , mReceiveUserConfigurationListCmd(AppCommDefs::ReceiveDataID::RCVDATAID_USER_CONFIG_LIST,
                                       mConfigurationStorageAssembly.getUserConfigurationStorage(),
                                       mUuidListEncoderContainer)
    , mReceiveFactoryUserConfigurationListCmd(
          AppCommDefs::ReceiveDataID::RCVDATAID_FACTORY_USER_CONFIG_LIST,
          mConfigurationStorageAssembly.getFactoryUserConfigurationStorage(),
          mUuidListEncoderContainer)
    , mDoubleClickKeyHandlerAssembly(mHardwareAssembly.getWorkQueueNormalPrio())
    , mConfigurationSwitcherAssembly(*this,
                                     mConfigurationStorageAssembly.getConfigurationStorageContext(),
                                     mConfigurationStorageAssembly
                                         .getFactoryConfigurationStorageContext(),
                                     {&mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem(),
                                      &mConfChangedCmd})
    , mBatteryStateMachine(mAssembly.getLedInformationItem())
    , mKeyboardParametersStorage(mFileSystemAssembly.getPartitionFileSystem(
                                     FileSystemPartitionLabel::MAIN),
                                 mKeyboardParametersCodec,
                                 shareBufferContainer)
    , mDemo(mBrightnessManager,
            mConfigurationSwitcherAssembly.getConfigurationSwitcher(),
            mHardwareAssembly.getWorkQueueNormalPrio())
    , mBLEPairing({&mKeyProtocolManagerAssembly.getKeyProtocolManagerItem(), &mDisplayer},
                  mHardwareAssembly.getBleItem())
    , mBlePowerManagement(mPowerManagerAssembly.getPowerManagerItem())
    , mLayoutModifiersStateAssembly(mDisplayer)
    , mKeyLockStatesDispatcher(mHardwareAssembly.getUsbItem(),
                               mHardwareAssembly.getBleHidItem(),
                               {&mAssembly.getLedInformationItem(),
                                &mLayoutModifiersStateAssembly.getLayoutModifiersStateItem()})
{
    mDoubleClickKeyHandlerAssembly.getItem().registerObserver(
        mConfigurationSwitcherAssembly.getConfigurationSwitcher());

    RegisterHIDMessageListener();
    RegisterCustomKeyProtocolMessageListener();
    RegisterPairingKeyMessageListener();
    mKeepAliveManager.registerObserver(mKeyProtocolManagerAssembly.getKeyProtocolManagerItem());

    mKeyProtocolManagerAssembly.getKeyProtocolManagerItem().registerKeyProtocolListener(
        mConfigurationSwitcherAssembly.getConfigurationSwitcher());
    mKeyProtocolManagerAssembly.getKeyProtocolManagerItem().registerKeyProtocolListener(
        mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem());

    mAssembly.getKeypadScanCodeConverterItem().registerKeyListener(
        mPowerManagerAssembly.getPowerManagerItem());
    mAssembly.getKeypadScanCodeConverterItem().registerKeyListener(mDemo);
    mAssembly.getKeypadScanCodeConverterItem().registerKeyListener(
        mKeyMessageDispatcherAssembly.getKeyMessageDispatcherItem());

#ifdef SYS_FAIL_LOG_DEMO
    mHardwareAssembly.getKeypadScanCodeConverterItem().registerKeyListener(mSysFailLogDemo);
#endif

    mAssembly.getBattery().registerBatteryLevelListener(mPowerManagerAssembly.getPowerManagerItem());

    mPowerManagerAssembly.getPowerManagerItem().registerUsbObserver(mConnectivityState);
    mPowerManagerAssembly.getPowerManagerItem().registerUsbObserver(mBatteryStateMachine);
    mPowerManagerAssembly.getPowerManagerItem().registerBatteryLevelObserver(mBatteryStateMachine);

    mDemo.registerListener(mPowerManagerAssembly.getPowerManagerItem());
    mDemo.registerListener(mBrightnessDemoModeHandler);
    mDemo.registerListener(mConfigurationSwitcherAssembly.getConfigurationSwitcher());

    /* Commands that don't need to keep connection alive */
    appComm.registerRxListener(mKeepAliveCmd);
    appComm.registerRxListener(mSystemResetCmd);
    appComm.registerRxListener(mBatteryInformationCmd);

    /* Commands that need to keep connection alive */
    appComm.registerRxListener(mSendDataCmdPowerManaged);
    appComm.registerRxListener(mApplyConfigurationCmdPowerManaged);
    appComm.registerRxListener(mFormatFlashCmdPowerManaged);
    appComm.registerRxListener(mVersionsCmdPowerManaged);
    appComm.registerRxListener(mReceiveDataCmdPowerManaged);
    appComm.registerRxListener(mSetCommModeCmdPowerManaged);
    appComm.registerRxListener(mSerialNumberCmdPowerManaged);
    appComm.registerRxListener(mKeyboardParametersCmdPowerManaged);
    appComm.registerRxListener(mFactoryResetCmdPowerManaged);
    appComm.registerRxListener(mValidateUserConfigurationCmdPowerManaged);
    appComm.registerRxListener(mValidateFactoryUserConfigurationCmdPowerManaged);
    appComm.registerRxListener(mDeleteLayoutCmdPowerManaged);
    appComm.registerRxListener(mDeleteFactoryLayoutCmdPowerManaged);
    appComm.registerRxListener(mDeleteUserConfigurationCmdPowerManaged);
    appComm.registerRxListener(mDeleteFactoryUserConfigurationCmdPowerManaged);
    appComm.registerRxListener(mApplyUserConfigurationCmdPowerManaged);

    mSendDataCmd.registerStreamDataListener(mFirmwarePackageUpdateCmd);
    mSendDataCmd.registerStreamDataListener(mSendUserConfigurationCmd);
    mSendDataCmd.registerStreamDataListener(mSendFactoryUserConfigurationCmd);
    mSendDataCmd.registerStreamDataListener(mSendLayoutConfigurationCmd);
    mSendDataCmd.registerStreamDataListener(mSendFactoryLayoutConfigurationCmd);
    mSendDataCmd.registerStreamDataListener(mSendLayoutCmd);
    mSendDataCmd.registerStreamDataListener(mSendFactoryLayoutCmd);

    mReceiveDataCmd.registerStreamDataListener(mReceiveEventLogCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveUserConfigurationCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveFactoryUserConfigurationCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveUserConfigurationCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveFactoryUserConfigurationCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveLayoutListCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveFactoryLayoutListCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveUserConfigurationListCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveFactoryUserConfigurationListCmd);
    mReceiveDataCmd.registerStreamDataListener(mReceiveTraceLogCmd);

    mHardwareAssembly.getUsbItem().registerKeyLockStatesListener(mKeyLockStatesDispatcher);
    mHardwareAssembly.getBleHidItem().registerKeyLockStatesListener(mKeyLockStatesDispatcher);

    systemUpdate.registerComponentUpdateListener(*this);
    systemUpdate.registerListener(*this);

    mHardwareAssembly.getBleItem().add_observer(mKeyLockStatesDispatcher);
    mHardwareAssembly.getBleItem().add_observer(mAssembly.getLedInformationItem());
    mHardwareAssembly.getBleItem().add_observer(mBlePowerManagement);

    mConfigurationStorageAssembly.registerToUserConfigurationStorageEvents(
        mConfigurationSwitcherAssembly.getConfigurationSwitcher());
    mValidateUserConfigurationCmd.add_observer(
        mConfigurationSwitcherAssembly.getConfigurationSwitcher());
    mValidateFactoryUserConfigurationCmd.add_observer(
        mConfigurationSwitcherAssembly.getConfigurationSwitcher());
}

void Application::FormatFlash()
{
    mFileSystemAssembly.format();

    SystemReset();
}

void Application::run()
{
    m_bRunning = true;

    TRACE_FUNC_ENTRY();

    mLogger.checkForSysFailEvents();
    mAssembly.getBattery().startBatteryMeasurement();

    mKeyboardParametersStorage.readKeyboardParameters(mKeyboardParameters);

    applyKeyboardParam(mKeyboardParameters);

    /* Turn on frontlight only if not demo mode */
    if(!mKeyboardParameters.isDemoMode())
    {
        mBrightnessManager
            .setBrightnessPercentageProgressive(mKeyboardParameters.getLedPowerActiveLevel(), 2);
    }

    mPowerManagerAssembly.getPowerManagerItem().init();

    if(mConfigurationSwitcherAssembly.getConfigurationSwitcher().init()
       != configuration::Ret::SUCCESS)
    {
        mDisplayer.clearCompleteScreen();
    }

    mKeyProtocolManagerAssembly.getKeyProtocolManagerItem().init();

    mPowerManagerAssembly.getPowerManagerItem().launch();

    TRACE_FUNC_EXIT();

    mAssembly.getKeypadItem().run();

    for(;;)
    {
        // Since we should reach this code only in case of shutdown, we do not refresh the watchdog here
        osDelay(osWaitForever);
    }
}

bool operator==(ButtonID const& a, ButtonID const& b)
{
    if((a.x == b.x) && (a.y == b.y))
        return true;
    else
        return false;
}

void Application::setBrightness(uint8_t brightnessPercent)
{
    mBrightnessManager.setBrightnessPercentage(brightnessPercent);
}

bool Application::applyLayoutConfiguration(
    ILayoutStorage& storage, const configuration::LayoutConfiguration& layoutConfiguration)
{
    bool success = false;

    etl::optional<configuration::Layout> layout;

    if(configuration::Ret ret = storage.getLayout(layoutConfiguration.getLayoutHash(), layout);
       ret == configuration::Ret::SUCCESS && layout.has_value())
    {
        mDisplayer.drawLayout(layout.value(),
                              mKeyboardParameters.isHighQualityModifier(),
                              layoutConfiguration.getOperatingSystem());
        success = true;
    }
    return success;
}

KeyboardParameters Application::getKeyboardParam()
{
    return mKeyboardParameters;
}

void Application::applyKeyboardParam(const KeyboardParameters& keyboardParameters)
{
    mPowerManagerAssembly.getPowerManagerItem().updateTimers(
        keyboardParameters.getInactiveTime(),
        keyboardParameters.getSleepTime(), /* Long inactive matches "sleep mode" for user */
        keyboardParameters.getSleepTime()
            + POWER_MANAGER_LONG_INACTIVE_TIMEOUT_SEC, /* Enters sleep mode if no communication from desktop app */
        keyboardParameters.getInactiveTimeUSBDisconnected(),
        keyboardParameters
            .getSleepTimeUSBDisconnected(), /* Long inactive matches "sleep mode" for user */
        keyboardParameters.getSleepTimeUSBDisconnected()
            + POWER_MANAGER_USB_DISCONNECTED_LONG_INACTIVE_TIMEOUT_SEC, /* Enters sleep mode if no communication from desktop app */
        keyboardParameters.getPowerOffTimeUSBDisconnected()
            + POWER_MANAGER_USB_DISCONNECTED_LONG_INACTIVE_TIMEOUT_SEC); /* Add POWER_MANAGER_USB_DISCONNECTED_LONG_INACTIVE_TIMEOUT_SEC before shutdown but prevent value to be invalid (< to sleep time) */

    mBrightnessManager.setReferenceMaxBrightness(BRIGHTNESS_REFERENCE_MAX_LEVEL_PERCENT);

    auto brightnessSteps = keyboardParameters.getBrightnessStepList();
    if(auto it = etl::find(brightnessSteps.begin(),
                           brightnessSteps.end(),
                           keyboardParameters.getLedPowerActiveLevel());
       it != brightnessSteps.end())
    {
        mBrightnessButtonHandler.setBrightnessStepList(keyboardParameters.getBrightnessStepList(),
                                                       etl::distance(brightnessSteps.begin(), it));
    }

    onBrightnessChanged(keyboardParameters.getLedPowerActiveLevel());

    mScreen.getAssociatedScreenController().forceCleanAfterNFastDisplay(
        keyboardParameters.getCleanRefreshPeriod());
    mScreen.getAssociatedScreenController().forceCleanAfterNBlackBackgroundDisplay(
        keyboardParameters.getBlackBackgroundCleanRefreshPeriod());

    mAssembly.getLedInformationItem().setLedConfiguration(keyboardParameters);

    mBatteryStateMachine.setLowBatteryLevelThresholdPercent(
        keyboardParameters.getLowBatteryLevelThresholdPercent());

    /* Demo mode */
    mDemo.setDemoMode(keyboardParameters.isDemoMode());
    if(!keyboardParameters.isDemoMode())
    {
        mDoubleClickKeyHandlerAssembly.getItem().registerObserver(mBrightnessButtonHandler);
    }
    else
    {
        mDoubleClickKeyHandlerAssembly.getItem().unregisterObserver(mBrightnessButtonHandler);
    }
}

void Application::setKeyboardParam(const KeyboardParameters& params)
{
    if(params.isValid() && params != mKeyboardParameters)
    {
        mKeyboardParameters = params;
        mKeyboardParametersStorage.saveKeyboardParameters(mKeyboardParameters);
        applyKeyboardParam(mKeyboardParameters);
    }
}

void Application::active()
{
}

void Application::inactive()
{
}

void Application::longInactive()
{
}

void Application::sleep()
{
}

void Application::suspend()
{
}

void Application::powerOff()
{
    //1) save parameters
    mKeyboardParametersStorage.saveKeyboardParameters(mKeyboardParameters);
    //2) clear screen
    mDisplayer.clearCompleteScreen();
    //3) poweroff screen
    mDisplayer.powerOff();
    //4)disable keyboard pull up
    mAssembly.getKeypadItem().deinitGPIOS();
}

void Application::factoryReset()
{
    mKeyboardParametersStorage.deleteKeyboardParameters();
    mKeyboardParameters = KeyboardParameters();

    mConfigurationStorageAssembly.getUserConfigurationStorage().clear();
    mConfigurationStorageAssembly.getLayoutStorage().clear();

    setKeyboardParam(mKeyboardParameters);
}

void Application::onUpdateEvent(UpdateStatus status)
{
    switch(status)
    {
    case UpdateStatus::START:
        break;
    case UpdateStatus::END:
    case UpdateStatus::ERROR:
        mHardwareAssembly.getSystemItem().reset();
        break;
    default:
        break;
    }
}

void Application::notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus)
{
    mUpdateStatusCmd.notifyUpdateStatus(updateStatus);
}

void Application::onBrightnessChanged(uint8_t brightnessPercent)
{
    mKeyboardParameters.setLedPowerActiveLevel(brightnessPercent);
    mBrightnessPowerStateHandler
        .setBrightnessForPowerState(mKeyboardParameters.getLedPowerActiveLevel(),
                                    mKeyboardParameters.getLedPowerActiveLevel()
                                        * mKeyboardParameters.getLedPowerInactiveLevel() / 100);
}
