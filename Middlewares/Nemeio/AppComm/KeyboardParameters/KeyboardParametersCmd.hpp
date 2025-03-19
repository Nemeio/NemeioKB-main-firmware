/*
 * KeyboardParametersCmd.hpp
 *
 *  Created on: Aug 23, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_KEYBOARDPARAMETERS_KEYBOARDPARAMETERSCMD_HPP_
#define APPCOMM_KEYBOARDPARAMETERS_KEYBOARDPARAMETERSCMD_HPP_

#include <algorithm>
#include "AppCommCmd.hpp"
#include "IApplication.hpp"
#include "utils.hpp"
#include "KeyboardParameters.hpp"
#include "IKeyboardParametersCodec.hpp"
#include "ISharedBufferContainer.hpp"

class KeyboardParametersCmd : public AppCommCmd
{
public:
    enum class KeyboardParametersCmdId : uint8_t
    {
        UNDEFINED = 0,
        GET,
        SET
    };

    KeyboardParametersCmd(IAppCommTxListener& appCommTxListener,
                          IApplication& application,
                          IKeyboardParametersCodec& codec,
                          ISharedBufferContainer& sharedBufferContainer);
    virtual ~KeyboardParametersCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    PACK((struct KeyboardParametersResponse
          {
              KeyboardParametersCmdId cmdId;
              AppCommDefs::AppCommErrCode ret;
              uint8_t payload[];

              KeyboardParametersResponse(KeyboardParametersCmdId cmdId_,
                                         AppCommDefs::AppCommErrCode ret_)
                  : cmdId(cmdId_)
                  , ret(ret_)
              {
              }
          }));

    void getParameters(etl::array_view<const std::byte> data);
    void setParameters(etl::array_view<const std::byte> data);

    void sendParameters(const KeyboardParameters& parameters);

    void sendRetCode(KeyboardParametersCmdId cmdId, AppCommDefs::AppCommErrCode ret);

    IApplication& m_application;
    IKeyboardParametersCodec& m_codec;
    ISharedBufferContainer& m_sharedBufferContainer;
};

#endif /* APPCOMM_KEYBOARDPARAMETERS_KEYBOARDPARAMETERSCMD_HPP_ */
