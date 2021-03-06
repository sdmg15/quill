/**
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/LogLevel.h"
#include <cstdint>

namespace quill
{
namespace detail
{

/**
 * Captures and stores information about a logging event in compile time
 * This information is later passed to the LogRecord runtime class
 */
class StaticLogRecordInfo
{
public:
  constexpr StaticLogRecordInfo(const char* lineno, char const* pathname, char const* func, char const* message_format, LogLevel level)
    : _func(func),
      _pathname(pathname),
      _filename(_extract_source_file_name(_pathname)),
      _message_format(message_format),
      _lineno(lineno),
      _level(level)
  {
  }

  /**
   * @return The function name
   */
  QUILL_NODISCARD constexpr char const* func() const noexcept { return _func; }

  /**
   * @return The full pathname of the source file where the logging call was made.
   */
  QUILL_NODISCARD constexpr char const* pathname() const noexcept { return _pathname; }

  /**
   * @return Short portion of the path name
   */
  QUILL_NODISCARD constexpr char const* filename() const noexcept { return _filename; }

  /**
   * @return The user provided format
   */
  QUILL_NODISCARD constexpr char const* message_format() const noexcept { return _message_format; }

  /**
   * @return The line number
   */
  QUILL_NODISCARD constexpr char const* lineno() const noexcept { return _lineno; }

  /**
   * @return The log level of this logging event as an enum
   */
  QUILL_NODISCARD constexpr LogLevel level() const noexcept { return _level; }

  /**
   * @return  The log level of this logging event as a string
   */
  QUILL_NODISCARD constexpr char const* level_as_str() const noexcept
  {
    return _log_level_to_string(_level);
  }

private:
  QUILL_NODISCARD static constexpr char const* _str_end(char const* str) noexcept
  {
    return *str ? _str_end(str + 1) : str;
  }

  QUILL_NODISCARD static constexpr bool _str_slant(char const* str) noexcept
  {
    return *str == path_delimiter ? true : (*str ? _str_slant(str + 1) : false);
  }

  QUILL_NODISCARD static constexpr char const* _r_slant(char const* str) noexcept
  {
    return *str == path_delimiter ? (str + 1) : _r_slant(str - 1);
  }

  QUILL_NODISCARD static constexpr char const* _extract_source_file_name(char const* str) noexcept
  {
    return _str_slant(str) ? _r_slant(_str_end(str)) : str;
  }

  QUILL_NODISCARD static constexpr char const* _log_level_to_string(LogLevel log_level)
  {
    constexpr std::array<char const*, 9> log_levels_strings = {
      {"LOG_TRACE_L3", "LOG_TRACE_L2", "LOG_TRACE_L1", "LOG_DEBUG   ", "LOG_INFO    ",
       "LOG_WARNING ", "LOG_ERROR   ", "LOG_CRITICAL", "LOG_NONE"}};

    using log_lvl_t = std::underlying_type<LogLevel>::type;
    return log_levels_strings[static_cast<log_lvl_t>(log_level)];
  }

private:
#if defined(_WIN32)
  static constexpr char path_delimiter = '\\';
#else
  static constexpr char path_delimiter = '/';
#endif

  char const* _func;
  char const* _pathname;
  char const* _filename;
  char const* _message_format;
  char const* _lineno;
  LogLevel _level;
};

} // namespace detail
} // namespace quill