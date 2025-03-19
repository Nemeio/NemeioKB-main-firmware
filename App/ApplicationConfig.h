/*
 * ApplicationConfig.h
 *
 *  Created on: Oct 24, 2017
 *      Author: cgarnier
 */

#ifndef APPLICATIONCONFIG_H_
#define APPLICATIONCONFIG_H_

#include "keyIDConfig.h"
#include <string>
#include "Version.h"
#include "FileSystemPartitions.hpp"

inline constexpr std::size_t MAX_DISPLAY_COMPRESSED_FILE_SIZE = 64 * 1024;

inline constexpr char PARAMETERS_FILE_NAME[] = "keyboardParameters";

inline constexpr std::size_t LOG_BUFFER_SIZE = 1024;
inline constexpr char EVENT_LOG_FILE_NAME[] = "eventLog";
inline constexpr FileSystemPartitionLabel EVENT_LOG_PART = FileSystemPartitionLabel::MAIN;


#endif /* APPLICATIONCONFIG_H_ */
