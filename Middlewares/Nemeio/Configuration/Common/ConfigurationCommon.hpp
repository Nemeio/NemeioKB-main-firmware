#pragma once

namespace configuration
{
enum class Ret
{
    SUCCESS,
    UNEXPECTED,
    INVALID_DATA,
    NOT_FOUND,
    FS,
    NO_SPACE,
    STATE
};

inline constexpr size_t MAX_USER_CONFIGURATIONS = 2;
inline constexpr size_t MAX_LAYOUTS = 50;

inline constexpr char USER_CONFIGURATION_FILENAME[] = ".ucfg";
inline constexpr char LAYOUT_CONFIGURATION_EXTENSION[] = ".lcfg";
inline constexpr char LAYOUT_EXTENSION[] = ".lyt";

inline constexpr char NEW_USER_CONFIGURATION_DIRECTORY[] = "newUcfg";

inline constexpr char VALID_USER_CONFIGURATION_DIRECTORY[] = "ucfg";
inline constexpr char VALID_FACTORY_USER_CONFIGURATION_DIRECTORY[] = "f_ucfg";

inline constexpr char LAYOUT_DIRECTORY[] = "layout";
inline constexpr char FACTORY_LAYOUT_DIRECTORY[] = "f_layout";

inline constexpr size_t MAX_DIRECTORY_PATH_SIZE = 44;
inline constexpr size_t MAX_METADATA_FILE_SIZE_BYTES = 6;
} // namespace configuration