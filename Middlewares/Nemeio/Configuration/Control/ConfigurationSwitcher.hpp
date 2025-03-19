/*
 * ConfigurationManager.h
 *
 *  Created on: Mar 22, 2018
 *      Author: amongatcheucheu
 */

#ifndef CONFIGURATIONSWITCHER_HPP_
#define CONFIGURATIONSWITCHER_HPP_

#include <vector>
#include <string>
#include <stdint.h>
#include "IConfigurationSwitcher.hpp"
#include "IKeyProtocolListener.hpp"
#include "IDoubleClickKeyListener.hpp"
#include "IDemoListener.hpp"
#include "IApplication.hpp"
#include "IConfigurationChangeNotifier.hpp"
#include "ListenerSubject/ListenerCollection.hpp"
#include "RecursiveMutex.hpp"
#include "ConfigurationStorageContext.hpp"
#include "UserConfigurationStorageObserver.hpp"
#include "UserConfigurationUpdateObserver.hpp"
#include "IUserConfigurationNavigationFactory.hpp"

enum class CfgMgrErrCode
{
    SUCCESS,
    UNEXPECTED,
    NOTFOUND,
    DRAW,
    FS,
    PROTECTLEVEL,
    MODE
};

class ConfigurationSwitcher : public IConfigurationSwitcher,
                              public IDoubleClickKeyListener,
                              public IKeyProtocolListener,
                              public IDemoListener,
                              public UserConfigurationStorageObserver,
                              public UserConfigurationUpdateObserver
{
public:
    ConfigurationSwitcher(IUserConfigurationNavigationFactory& userConfigurationNavigationFactory,
                          IApplication& application,
                          ConfigurationStorageContext& configurationStorageContext,
                          ConfigurationStorageContext& factoryConfigurationStorageContext,
                          IConfigurationChangeNotifier& confChangeNotifier);
    ~ConfigurationSwitcher() final = default;

    configuration::Ret init();

    void simpleClicked(const ButtonID& button) final;
    void doubleClicked(const ButtonID& button) final;

    void keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode keyProtocolMode) final;

    void demoStart() final;
    void demoStop() final;

    configuration::Ret selectUserConfiguration(
        uuid::UuidView userConfigurationId,
        etl::optional<uuid::UuidView> layoutConfigurationId) final;
    configuration::Ret selectLayoutConfiguration(uuid::UuidView id) final;
    configuration::Ret setNextConfiguration() final;

    void notification(const UserConfigurationUpdateEvent& event) final;
    void notification(const UserConfigurationDeleteEvent& event) final;

private:
    static constexpr ButtonID CHANGE_CONF_BUTTON{ButtonID(BTN_DOWN)};

    IUserConfigurationNavigationFactory& mUserConfigurationNavigationFactory;
    IApplication& mApplication;
    ConfigurationStorageContext& mConfigurationStorageContext;
    ConfigurationStorageContext& mFactoryConfigurationStorageContext;
    etl::optional<etl::reference_wrapper<IUserConfigurationNavigation>>
        mCurrentUserConfigurationNavigation;

    bool mbDemoMode{false};
    bool mbCustomMode{false};
    IConfigurationChangeNotifier& mConfChangeNotifier;
    RecursiveMutex mMutex;

    void changeConfigurationButtonPressed(const ButtonID& button,
                                          IUserConfigurationNavigation::SwitchDirection direction);
    configuration::Ret changeConfigurationIterate(
        IUserConfigurationNavigation::SwitchDirection direction);
    void updateUserConfigurationNavigationCustom();

    configuration::Ret selectFirstUserConfiguration();
    configuration::Ret selectFirstUserConfigurationFromContext(
        ConfigurationStorageContext& storageContext);
    configuration::Ret selectUserConfigurationFromContext(
        ConfigurationStorageContext& storageContext,
        uuid::UuidView id,
        etl::optional<uuid::UuidView> layoutConfigurationId);

    void userConfigurationUpdatedOrDeleted(uuid::UuidView id);
};

#endif /* CONFIGURATIONSWITCHER_HPP_ */
