#ifndef __APPLICATION_HPP_
#define __APPLICATION_HPP_

#include <map>
#include <string>
#include <stdint.h>
#include <vector>

#include <Configuration.hpp>

#include "NullChipSelector.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "AssemblyHAL.h"
#include "ApplicationConfig.h"
#include "PowerManagerActivityCmdDecorator.hpp"
#include "FileSystemTrimming.hpp"
#include "ConfigurationSwitcherAssembly.hpp"
#include "KeyLockStatesDispatcher.hpp"
#include "PowerManagerAssembly.hpp"
#include "Decompression/TinyDeflateDecompressor.hpp"
#include "KeyMessageDispatcherAssembly.hpp"
#include "CustomKeyProtocol.hpp"
#include "KeyProtocolManagerAssembly.hpp"
#include "KeepAliveManager.hpp"
#include "Demo.hpp"
#include "AppCommProxy/AppCommProxy.hpp"
#include "USBSerial.hpp"
#include "SendDataCmd.hpp"
#include "KeepAliveCmd.hpp"
#include "ApplyConfigurationCmd.hpp"
#include "ConfChangedCmd.hpp"
#include "KeyPressedCmd.hpp"
#include "FormatFlashCmd.hpp"
#include "VersionsCmd.hpp"
#include "KeyboardParametersStorage.hpp"
#include "FirmwareUpdateAssembly.hpp"
#include "Logger.hpp"
#include "ReceiveData/ReceiveDataCmd.hpp"
#include "ReceiveEventLogCmd.hpp"
#include "SysFailLogDemo.hpp"
#include "SetCommModeCmd.hpp"
#include "ComputerHIDMessageHandler.h"
#include "CustomKeyProtocolMessageHandler.hpp"
#include "DoubleClickKeyHandlerAssembly.hpp"
#include "BleSerial.hpp"
#include "SerialNumber/SerialNumberCmd.hpp"
#include "BLEPairing/BLEPairing.hpp"
#include "KeyboardParametersCmd.hpp"
#include "Displayer.hpp"
#include "ConnectivityState.hpp"
#include "SystemResetCmd.hpp"
#include "BatteryStateMachine.hpp"
#include "BatteryInformationCmd.hpp"
#include "LayoutModifiersStateAssembly.hpp"
#include "IApplication.hpp"
#include "FactoryReset.hpp"
#include "FactoryResetCmd.hpp"
#include "FirmwarePackageUpdateCmd.hpp"
#include "UpdateStatusCmd.hpp"
#include "FirmwareEnvironmentRedundant.hpp"
#include "cpp_helper.hpp"
#include "DeviceProperties.hpp"
#include "BrightnessManager.hpp"
#include "BrightnessButtonsHandler.hpp"
#include "BrightnessPowerStateHandler.hpp"
#include "BrightnessDemoModeHandler.hpp"
#include "IBrightnessListener.hpp"
#include "WorkQueue.hpp"
#include "BleActivationStrategy.hpp"
#include "SendLayoutCmd.hpp"
#include "ConfigurationStorageAssembly.hpp"
#include "ReceiveUserConfigurationCmd.hpp"
#include "DeleteLayoutCmd.hpp"
#include "DeleteUserConfigurationCmd.hpp"
#include "ApplyUserConfigurationCmd.hpp"
#include "SendUserConfigurationCmd.hpp"
#include "SendLayoutConfigurationCmd.hpp"
#include "ValidateUserConfigurationCmd.hpp"
#include "ReceiveUuidListCmd.hpp"
#include "CborUuidListCodec.hpp"
#include "CborKeyboardParametersCodec.hpp"
#include "ISharedBufferContainer.hpp"
#include "BlePowerManagement.hpp"
#include "ReceiveTraceLogCmd.hpp"
#include "TraceLogger.hpp"
#include "KeyboardStatus.hpp"
#include "CborKeepAliveRespCodec.hpp"

class Application : public IApplication,
                    public IPowerManageable,
                    public IUpdateListener,
                    public IUpdateStatusListener,
                    public IBrightnessListener
{
public:
    Application(AssemblyHardware& hardwareAssembly,
                FileSystemAssembly& fileSystemAssembly,
                IScreen& screen,
                AssemblyHAL& assembly,
                Displayer& displayer,
                SystemUpdate& systemUpdate,
                DeviceProperties deviceProperties,
                TinyDeflateDecompressor& decompressor,
                Logger& logger,
                AppCommProxy& appComm,
                ISharedBufferContainer& shareBufferContainer);
    COPYABLE_MOVABLE(Application, delete);

    void onBrightnessChanged(uint8_t brightnessPercent) final;

    void RegisterHIDMessageListener();
    void RegisterCustomKeyProtocolMessageListener();
    void RegisterPairingKeyMessageListener();
    void FormatFlash() final;

    void run();

    void setBrightness(uint8_t brightnessPercent);

    bool applyLayoutConfiguration(
        ILayoutStorage& storage,
        const configuration::LayoutConfiguration& layoutConfiguration) final;

    KeyboardParameters getKeyboardParam() final;
    void setKeyboardParam(const KeyboardParameters& params) final;
    void factoryReset() final;

    void active() final;
    void inactive() final;
    void longInactive() final;
    void sleep() final;
    void powerOff() final;
    void suspend() final;

private:
    static constexpr std::size_t NB_CONFCHANGED_LISTENERS = 2;
    static constexpr std::size_t NB_CONNECTIVITY_LISTENERS = 3;
    static constexpr std::size_t NB_BLE_PAIRING_LISTENERS = 2;
    static constexpr std::size_t NB_KEY_LOCK_STATES_DISPATCHER_LISTENERS = 2;
    static constexpr std::size_t NB_POWER_MANAGEABLES = 5;

    static constexpr uint32_t POWER_MANAGER_LONG_INACTIVE_TIMEOUT_SEC = 15;
    static constexpr uint32_t POWER_MANAGER_USB_DISCONNECTED_LONG_INACTIVE_TIMEOUT_SEC = 15;

    static constexpr uint8_t BRIGHTNESS_REFERENCE_MAX_LEVEL_PERCENT = 50;

    static constexpr size_t KEYBOARD_PARAMETERS_SHARED_BUFFER_SIZE = 600;

    bool m_bRunning = false;

    AssemblyHardware& mHardwareAssembly;
    FileSystemAssembly& mFileSystemAssembly;
    AssemblyHAL& mAssembly;
    Displayer& mDisplayer;
    IScreen& mScreen;
    Logger& mLogger;

    BleActivationStrategy mBleActivationStrategy;
    ConnectivityState<NB_CONNECTIVITY_LISTENERS> mConnectivityState;
    ComputerHIDMessageHandler computerHIDMessageHandler;
    CustomKeyProtocolMessageHandler customKeyProtocolMessageHandler;
    KeyMessageDispatcherAssembly mKeyMessageDispatcherAssembly;
    CustomKeyProtocol mCustomKeyProtocol;
    KeyProtocolManagerAssembly mKeyProtocolManagerAssembly;
    KeepAliveManager mKeepAliveManager;
    FactoryReset mFactoryReset;
    DeviceProperties mDeviceProperties;
    BrightnessManager mBrightnessManager;
    BrightnessButtonsHandler mBrightnessButtonHandler;
    BrightnessDemoModeHandler mBrightnessDemoModeHandler;
    BrightnessPowerStateHandler mBrightnessPowerStateHandler;
    PowerManagerAssembly<NB_POWER_MANAGEABLES> mPowerManagerAssembly;
    KeyboardStatus mKeyboardStatus{logger::TraceLogger::instance()};

    fs::IFileSystem& mConfigurationFileSystem;
    fs::IFileSystem& mFactoryConfigurationFileSystem;
    ConfigurationStorageAssembly mConfigurationStorageAssembly;

    CborKeyboardParametersCodec mKeyboardParametersCodec;
    CborKeepAliveRespCodec mKeepAliveRespCodec;

    /* Notification commands */
    UpdateStatusCmd mUpdateStatusCmd;

    /* AT commands : Do not keep connection alive */
    KeepAliveCmd mKeepAliveCmd;
    SystemResetCmd mSystemResetCmd;
    ConfChangedCmd mConfChangedCmd;

    /* AT commands : Keep connection alive */
    SendDataCmd mSendDataCmd;
    ApplyConfigurationCmd mApplyConfigurationCmd;
    KeyPressedCmd mKeyPressedCmd;
    FormatFlashCmd mFormatFlashCmd;
    BatteryInformationCmd mBatteryInformationCmd;
    VersionsCmd mVersionsCmd;
    ReceiveDataCmd mReceiveDataCmd;
    SetCommModeCmd mSetCommModeCmd;
    SerialNumberCmd mSerialNumberCmd;
    KeyboardParametersCmd mKeyboardParametersCmd;
    FactoryResetCmd mFactoryResetCmd;
    ValidateUserConfigurationCmd mValidateUserConfigurationCmd;
    ValidateUserConfigurationCmd mValidateFactoryUserConfigurationCmd;
    DeleteLayoutCmd mDeleteLayoutCmd;
    DeleteLayoutCmd mDeleteFactoryLayoutCmd;
    DeleteUserConfigurationCmd mDeleteUserConfigurationCmd;
    DeleteUserConfigurationCmd mDeleteFactoryUserConfigurationCmd;
    ApplyUserConfigurationCmd mApplyUserConfigurationCmd;

    /* AT commands : Keep connection alive with powermanager decorator */
    PowerManagerActivityCmdDecorator mSendDataCmdPowerManaged;
    PowerManagerActivityCmdDecorator mApplyConfigurationCmdPowerManaged;
    PowerManagerActivityCmdDecorator mFormatFlashCmdPowerManaged;
    PowerManagerActivityCmdDecorator mVersionsCmdPowerManaged;
    PowerManagerActivityCmdDecorator mReceiveDataCmdPowerManaged;
    PowerManagerActivityCmdDecorator mSetCommModeCmdPowerManaged;
    PowerManagerActivityCmdDecorator mSerialNumberCmdPowerManaged;
    PowerManagerActivityCmdDecorator mKeyboardParametersCmdPowerManaged;
    PowerManagerActivityCmdDecorator mFactoryResetCmdPowerManaged;
    PowerManagerActivityCmdDecorator mValidateUserConfigurationCmdPowerManaged;
    PowerManagerActivityCmdDecorator mValidateFactoryUserConfigurationCmdPowerManaged;
    PowerManagerActivityCmdDecorator mDeleteLayoutCmdPowerManaged;
    PowerManagerActivityCmdDecorator mDeleteFactoryLayoutCmdPowerManaged;
    PowerManagerActivityCmdDecorator mDeleteUserConfigurationCmdPowerManaged;
    PowerManagerActivityCmdDecorator mDeleteFactoryUserConfigurationCmdPowerManaged;
    PowerManagerActivityCmdDecorator mApplyUserConfigurationCmdPowerManaged;

    /* AT "Send Data" subcommands */
    FirmwarePackageUpdateCmd mFirmwarePackageUpdateCmd;
    SendUserConfigurationCmd mSendUserConfigurationCmd;
    SendUserConfigurationCmd mSendFactoryUserConfigurationCmd;
    SendLayoutConfigurationCmd mSendLayoutConfigurationCmd;
    SendLayoutConfigurationCmd mSendFactoryLayoutConfigurationCmd;
    SendLayoutCmd mSendLayoutCmd;
    SendLayoutCmd mSendFactoryLayoutCmd;
    ReceiveEventLogCmd mReceiveEventLogCmd;
    ReceiveUserConfigurationCmd mReceiveUserConfigurationCmd;
    ReceiveUserConfigurationCmd mReceiveFactoryUserConfigurationCmd;
    CborUuidListCodec mLayoutListCodec;
    UuidListEncoderContainer mUuidListEncoderContainer;
    ReceiveUuidListCmd mReceiveLayoutListCmd;
    ReceiveUuidListCmd mReceiveFactoryLayoutListCmd;
    ReceiveUuidListCmd mReceiveUserConfigurationListCmd;
    ReceiveUuidListCmd mReceiveFactoryUserConfigurationListCmd;
    ReceiveTraceLogCmd mReceiveTraceLogCmd{logger::TraceLogger::instance()};

    DoubleClickKeyHandlerAssembly mDoubleClickKeyHandlerAssembly;
    ConfigurationSwitcherAssembly<NB_CONFCHANGED_LISTENERS> mConfigurationSwitcherAssembly;
    BatteryStateMachine mBatteryStateMachine;
    KeyboardParametersStorage mKeyboardParametersStorage;
    KeyboardParameters mKeyboardParameters;
    Demo mDemo;


    TimerHandle_t frontlightTimer;
#ifdef SYS_FAIL_LOG_DEMO
    SysFailLogDemo mSysFailLogDemo;
#endif

    BLEPairing<NB_BLE_PAIRING_LISTENERS> mBLEPairing;
    BlePowerManagement mBlePowerManagement;

    LayoutModifiersStateAssembly mLayoutModifiersStateAssembly;
    KeyLockStatesDispatcher<NB_KEY_LOCK_STATES_DISPATCHER_LISTENERS> mKeyLockStatesDispatcher;

    bool drawConfiguration(const Configuration& config);
    void applyKeyboardParam(const KeyboardParameters& keyboardParameters);

    AppErrCode convertCfgMgrErrCode(CfgMgrErrCode cfgMgrRet) const;

    void initUSB();

    void onUpdateEvent(UpdateStatus status) final;
    void notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus) final;
};

#endif //__APPLICATION_HPP_
