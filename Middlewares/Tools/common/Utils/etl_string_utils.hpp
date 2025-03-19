#pragma once

#include "etl/string_view.h"
#include "etl/array_view.h"
#include "strlen.h"

namespace etl_string_utils
{
/**
  * @brief  Convert an etl::array_view to an etl::string_view.
  * @param  arrayView The input etl::array_view, can be non null-terminated.
  * @retval The etl::string_view
  */
inline etl::string_view arrayToStringView(etl::array_view<const char> arrayView)
{
    size_t size = strnlen(arrayView.data(), arrayView.size());
    return etl::string_view(arrayView.data(), arrayView.data() + size);
}

} // namespace etl_string_utils