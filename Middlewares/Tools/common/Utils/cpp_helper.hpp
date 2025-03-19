#ifndef TOOLS_UTILS_CPP_HELPER_HPP
#define TOOLS_UTILS_CPP_HELPER_HPP

#define COPYABLE(className, defaultOrDelete) \
    className(const className&) = defaultOrDelete; \
    className& operator=(const className&) = defaultOrDelete
#define MOVABLE(className, defaultOrDelete) \
    className(className&&) noexcept = defaultOrDelete; \
    className& operator=(className&&) noexcept = defaultOrDelete
#define COPYABLE_MOVABLE(className, defaultOrDelete) \
    COPYABLE(className, defaultOrDelete); \
    MOVABLE(className, defaultOrDelete)

#endif
