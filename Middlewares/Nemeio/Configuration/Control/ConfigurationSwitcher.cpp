/*
 * ConfigurationManager.cpp
 *
 *  Created on: Mar 22, 2018
 *      Author: amongatcheucheu
 */

#include "ConfigurationSwitcher.hpp"
#include "ConcatenatedCollection.hpp"
#include "AutoLock.h"
#include "UuidListIteratorWrapper.hpp"

using namespace configuration;

ConfigurationSwitcher::ConfigurationSwitcher(
    IUserConfigurationNavigationFactory& userConfigurationNavigationFactory,
    IApplication& application,
    ConfigurationStorageContext& configurationStorageContext,
    ConfigurationStorageContext& factoryConfigurationStorageContext,
    IConfigurationChangeNotifier& confChangeNotifier)
    : mUserConfigurationNavigationFactory(userConfigurationNavigationFactory)
    , mApplication(application)
    , mConfigurationStorageContext(configurationStorageContext)
    , mFactoryConfigurationStorageContext(factoryConfigurationStorageContext)
    , mConfChangeNotifier(confChangeNotifier)
{
}

Ret ConfigurationSwitcher::init()
{
    return selectFirstUserConfiguration();
}

void ConfigurationSwitcher::simpleClicked(const ButtonID& button)
{
    AutoLock autoLock(mMutex);

    changeConfigurationButtonPressed(button, IUserConfigurationNavigation::SwitchDirection::FORWARD);
}

void ConfigurationSwitcher::doubleClicked(const ButtonID& button)
{
    AutoLock autoLock(mMutex);

    changeConfigurationButtonPressed(button,
                                     IUserConfigurationNavigation::SwitchDirection::BACKWARD);
}

void ConfigurationSwitcher::keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode keyProtocolMode)
{
    AutoLock autoLock(mMutex);

    mbCustomMode = (keyprotocoldefs::KeyProtocolMode::CUSTOM == keyProtocolMode);

    updateUserConfigurationNavigationCustom();
}

void ConfigurationSwitcher::demoStart()
{
    mbDemoMode = true;
}

void ConfigurationSwitcher::demoStop()
{
    mbDemoMode = false;
}

Ret ConfigurationSwitcher::selectUserConfiguration(
    uuid::UuidView userConfigurationId, etl::optional<uuid::UuidView> layoutConfigurationId)
{
    AutoLock autoLock(mMutex);

    return selectUserConfigurationFromContext(mConfigurationStorageContext,
                                              userConfigurationId,
                                              layoutConfigurationId);
}

Ret ConfigurationSwitcher::selectLayoutConfiguration(uuid::UuidView id)
{
    AutoLock autoLock(mMutex);

    if(!mCurrentUserConfigurationNavigation.has_value())
    {
        return Ret::NOT_FOUND;
    }

    Ret ret = mCurrentUserConfigurationNavigation.value().get().applyId(id);

    return ret;
}

Ret ConfigurationSwitcher::setNextConfiguration()
{
    AutoLock autoLock(mMutex);

    Ret ret = changeConfigurationIterate(IUserConfigurationNavigation::SwitchDirection::FORWARD);

    return ret;
}

void ConfigurationSwitcher::notification(const UserConfigurationUpdateEvent& event)
{
    AutoLock autoLock(mMutex);

    userConfigurationUpdatedOrDeleted(event.id);
}

void ConfigurationSwitcher::notification(const UserConfigurationDeleteEvent& event)
{
    AutoLock autoLock(mMutex);

    userConfigurationUpdatedOrDeleted(event.id);
}

void ConfigurationSwitcher::changeConfigurationButtonPressed(
    const ButtonID& button, IUserConfigurationNavigation::SwitchDirection direction)
{
    if(mbDemoMode)
    {
        return;
    }

    if(button == CHANGE_CONF_BUTTON)
    {
        changeConfigurationIterate(direction);
    }
}

Ret ConfigurationSwitcher::changeConfigurationIterate(
    IUserConfigurationNavigation::SwitchDirection direction)
{
    if(!mCurrentUserConfigurationNavigation.has_value())
    {
        return Ret::NOT_FOUND;
    }

    return mCurrentUserConfigurationNavigation.value().get().applyNext(direction);
}

void ConfigurationSwitcher::updateUserConfigurationNavigationCustom()
{
    if(!mCurrentUserConfigurationNavigation.has_value())
    {
        return;
    }

    if(mbCustomMode)
    {
        mCurrentUserConfigurationNavigation.value().get().enableCustom();
    }
    else
    {
        mCurrentUserConfigurationNavigation.value().get().disableCustom();
    }
}

Ret ConfigurationSwitcher::selectFirstUserConfiguration()
{
    Ret ret = selectFirstUserConfigurationFromContext(mConfigurationStorageContext);

    if(ret != Ret::SUCCESS)
    {
        ret = selectFirstUserConfigurationFromContext(mFactoryConfigurationStorageContext);
    }

    return ret;
}

Ret ConfigurationSwitcher::selectFirstUserConfigurationFromContext(
    ConfigurationStorageContext& storageContext)
{
    UuidListIteratorWrapper itWrapper(storageContext.getUserConfigurationStorage());

    if(!itWrapper.it().has_value())
    {
        return Ret::NOT_FOUND;
    }

    auto firstConfiguration = itWrapper.it().value().get().next();

    if(!firstConfiguration.has_value())
    {
        return Ret::NOT_FOUND;
    }

    Ret ret = selectUserConfigurationFromContext(storageContext,
                                                 uuid::UuidView(firstConfiguration.value()),
                                                 etl::nullopt);

    return ret;
}

configuration::Ret ConfigurationSwitcher::selectUserConfigurationFromContext(
    ConfigurationStorageContext& storageContext,
    uuid::UuidView id,
    etl::optional<uuid::UuidView> layoutConfigurationId)
{
    Ret ret = Ret::NOT_FOUND;

    if(storageContext.getUserConfigurationStorage().isExistingUserConfiguration(id))
    {
        mCurrentUserConfigurationNavigation.emplace(
            mUserConfigurationNavigationFactory.createUserConfigurationNavigation(
                id, storageContext, mApplication, mConfChangeNotifier));

        updateUserConfigurationNavigationCustom();

        if(layoutConfigurationId.has_value())
        {
            ret = mCurrentUserConfigurationNavigation.value().get().applyId(
                layoutConfigurationId.value());
            if(ret != Ret::SUCCESS)
            {
                mCurrentUserConfigurationNavigation.value().get().applyFirst();
            }
        }
        else
        {
            ret = mCurrentUserConfigurationNavigation.value().get().applyFirst();
        }
    }

    return ret;
}

void ConfigurationSwitcher::userConfigurationUpdatedOrDeleted(uuid::UuidView id)
{
    if(!mCurrentUserConfigurationNavigation.has_value()
       || mCurrentUserConfigurationNavigation.value().get().getCurrentUserConfigurationId() == id)
    {
        mCurrentUserConfigurationNavigation.reset();
        selectFirstUserConfiguration();
    }
}