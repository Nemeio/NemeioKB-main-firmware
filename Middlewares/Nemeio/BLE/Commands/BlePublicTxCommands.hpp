#pragma once

#include "etl/functional.h"
#include "etl/variant.h"

#include "BleGetFwVersionCmd.hpp"
#include "BleSerialDataCmd.hpp"
#include "BleFactoryResetCmd.hpp"
#include "BleDfuStartCmd.hpp"
#include "BleHidInputReportCmd.hpp"
#include "BleHidReportDescriptorCmd.hpp"
#include "BleSetRadioModeCmd.hpp"
#include "BleDfuCommitVersionCmd.hpp"
#include "BleNumericComparisonCmd.hpp"
#include "BleDfuDataCmd.hpp"

// The commands that can be used from entire Nemeio application, outside BleChip class
// Other commands are private they should only be used internally by BleChip class

// The commands relative to BLE, that are available when the BLE chip has been configured
using BleReadyForBleTxCommandRef = etl::variant<etl::reference_wrapper<BleSerialDataCmd>,
                                                etl::reference_wrapper<BleHidInputReportCmd>,
                                                etl::reference_wrapper<BleNumericComparisonCmd>>;

// The commands that are available when the BLE chip application is started
using BleApplicationTxCommandRef = etl::variant<etl::reference_wrapper<BleDfuCommitVersionCmd>,
                                                etl::reference_wrapper<BleSetRadioModeCmd>>;

// The requests that are available when the BLE chip application is started
using BleApplicationTxRequestRef = etl::variant<etl::reference_wrapper<BleGetFwVersionCmd>,
                                                etl::reference_wrapper<BleFactoryResetCmd>>;

// The commands that are available when the BLE chip is in DFU mode
using BleDfuModeTxCommandRef = etl::variant<etl::reference_wrapper<BleDfuDataCmd>>;