#include "gtest/gtest.h"

#include "KeyboardParametersCmd.hpp"
#include "MockAppCommTxListener.hpp"
#include "MockApplication.hpp"
#include "MockKeyboardParametersCodec.hpp"
#include "KeyboardParametersTestValues.hpp"
#include "SharedBufferContainer.hpp"
#include "gtest_helpers.hpp"

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::Invoke;
using ::testing::Return;

class KeyboardParametersCmdTest : public ::testing::Test
{
protected:
    KeyboardParametersCmdTest()
        : keyboardParametersCmd(txListener, application, codec, sharedBufferContainer){};

    MockAppCommTxListener txListener;
    MockApplication application;
    MockKeyboardParametersCodec codec;
    SharedBufferContainer sharedBufferContainer;
    KeyboardParametersCmd keyboardParametersCmd;
    etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE> mBrightnessDefaultList =
        TEST_BRIGHTNESS_LIST;
};

TEST_F(KeyboardParametersCmdTest, GetKeyboardParametersNominal)
{
    KeyboardParameters keyboardParameters;
    etl::array<std::byte, 3> dummyBuffer{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};
    etl::array_view<const uint8_t> dummyBufferU8{reinterpret_cast<const uint8_t*>(
                                                     dummyBuffer.data()),
                                                 dummyBuffer.size()};

    auto encode = [this, &dummyBuffer](const KeyboardParameters& keyboardParameters,
                                       etl::array_view<std::byte> data)
    {
        std::copy(dummyBuffer.begin(), dummyBuffer.end(), data.begin());
        return dummyBuffer.size();
    };
    EXPECT_CALL(application, getKeyboardParam).Times(1).WillOnce(Return(keyboardParameters));

    EXPECT_CALL(codec, encode(keyboardParameters, _)).Times(1).WillOnce(Invoke(encode));

    std::vector<uint8_t> cmd;
    cmd.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::GET));

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::GET));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_SUCCESS));
    resp.insert(resp.end(), dummyBufferU8.begin(), dummyBufferU8.end());

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, SetKeyboardParametersNominal)
{
    KeyboardParameters expectedKeyboardParameters;
    expectedKeyboardParameters.setInactiveTime(13);
    expectedKeyboardParameters.setDemoMode(true);

    etl::array<std::byte, 3> dummyBuffer{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};
    etl::array_view<const uint8_t> dummyBufferU8{reinterpret_cast<const uint8_t*>(
                                                     dummyBuffer.data()),
                                                 dummyBuffer.size()};
    std::vector<uint8_t> cmd;

    cmd.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::SET));
    cmd.insert(cmd.end(), dummyBufferU8.begin(), dummyBufferU8.end());

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::SET));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_SUCCESS));

    auto decode = [this, &expectedKeyboardParameters](etl::array_view<const std::byte> data,
                                                      KeyboardParameters& keyboardParameters)
    {
        keyboardParameters = expectedKeyboardParameters;
        return true;
    };
    EXPECT_CALL(codec, decode(etl::array_view<const std::byte>(dummyBuffer), _))
        .Times(1)
        .WillOnce(Invoke(decode));

    EXPECT_CALL(application, getKeyboardParam()).Times(1).WillOnce(Return(KeyboardParameters()));
    EXPECT_CALL(application, setKeyboardParam(Eq(expectedKeyboardParameters))).Times(1);

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, EmptyCmd)
{
    std::vector<uint8_t> cmd;

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::UNDEFINED));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_MALFORMED_DATA));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, WrongCmdId)
{
    std::vector<uint8_t> cmd;
    constexpr uint8_t WRONG_CMD_ID = 123;
    cmd.push_back(WRONG_CMD_ID);

    std::vector<uint8_t> resp;
    resp.push_back(WRONG_CMD_ID);
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_MALFORMED_DATA));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, GetKeyboardParametersWrongCmdExcessData)
{
    std::vector<uint8_t> cmd;
    cmd.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::GET));
    cmd.push_back(0);

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::GET));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_MALFORMED_DATA));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, GetKeyboardParametersEncodeError)
{
    KeyboardParameters keyboardParameters;

    EXPECT_CALL(application, getKeyboardParam).Times(1).WillOnce(Return(keyboardParameters));

    EXPECT_CALL(codec, encode).Times(1).WillOnce(Return(-1));

    std::vector<uint8_t> cmd;
    cmd.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::GET));

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::GET));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_UNEXPECTED));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, SetKeyboardParametersWrongData)
{
    std::vector<uint8_t> cmd;
    etl::array<std::byte, 3> dummyBuffer{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};
    etl::array_view<const uint8_t> dummyBufferU8{reinterpret_cast<const uint8_t*>(
                                                     dummyBuffer.data()),
                                                 dummyBuffer.size()};

    cmd.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::SET));
    cmd.insert(cmd.end(), dummyBufferU8.begin(), dummyBufferU8.end());

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::SET));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_INVALID_CONTENT));

    EXPECT_CALL(codec, decode).Times(1).WillOnce(Return(false));

    EXPECT_CALL(application, getKeyboardParam()).Times(1).WillOnce(Return(KeyboardParameters()));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}

TEST_F(KeyboardParametersCmdTest, SetKeyboardParametersInvalidParameters)
{
    KeyboardParameters invalidKeyboardParameters;
    invalidKeyboardParameters.setLedPowerActiveLevel(255);

    etl::array<std::byte, 3> dummyBuffer{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};
    etl::array_view<const uint8_t> dummyBufferU8{reinterpret_cast<const uint8_t*>(
                                                     dummyBuffer.data()),
                                                 dummyBuffer.size()};
    std::vector<uint8_t> cmd;

    cmd.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::SET));
    cmd.insert(cmd.end(), dummyBufferU8.begin(), dummyBufferU8.end());

    std::vector<uint8_t> resp;
    resp.push_back(static_cast<uint8_t>(KeyboardParametersCmd::KeyboardParametersCmdId::SET));
    resp.push_back(static_cast<uint8_t>(AppCommDefs::APPCOMMERR_INVALID_CONTENT));

    auto decode = [this, &invalidKeyboardParameters](etl::array_view<const std::byte> data,
                                                     KeyboardParameters& keyboardParameters)
    {
        keyboardParameters = invalidKeyboardParameters;
        return true;
    };
    EXPECT_CALL(codec, decode).Times(1).WillOnce(Invoke(decode));

    EXPECT_CALL(application, getKeyboardParam()).Times(1).WillOnce(Return(KeyboardParameters()));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    keyboardParametersCmd.dataReceived(cmd.data(), cmd.size());
}