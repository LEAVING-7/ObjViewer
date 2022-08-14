#pragma once
#include <cstdint>
#include <spdlog/spdlog.h>

using u8    = std::uint8_t;
using u16   = std::uint16_t;
using u32   = std::uint32_t;
using u64   = std::uint64_t;
using i8    = std::int8_t;
using i16   = std::int16_t;
using i32   = std::int32_t;
using i64   = std::int64_t;
using ccstr = char const *;

#define FILE_LINE_FMT "[{}:{}] "
#define FILE_LINE_ARG __FILE__, __LINE__

#define LOG(logLevel, fmt, ...)                                                \
  do {                                                                         \
    spdlog::log(spdlog::level::logLevel, fmt, __VA_ARGS__);                    \
  } while (0)

#define LOG_ERR(fmt, ...)      LOG(err, fmt, __VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) LOG(critical, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)     LOG(info, fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...)    LOG(debug, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)     LOG(warn, fmt, __VA_ARGS__)

#ifdef _DEBUG

#define LOG_LOC(logLevel, fmt, ...)                                            \
  do {                                                                         \
    spdlog::log(spdlog::level ::logLevel, FILE_LINE_FMT fmt, FILE_LINE_ARG,    \
                __VA_ARGS__);                                                  \
  } while (0)

#else

#define LOG_LOC(logLevel, fmt, ...) LOG(logLevel, fmt, ...)

#endif

#define LOG_LOC_ERR(fmt, ...)      LOG_LOC(err, fmt, __VA_ARGS__)
#define LOG_LOC_CRITICAL(fmt, ...) LOG_LOC(critical, fmt, __VA_ARGS__)
#define LOG_LOC_INFO(fmt, ...)     LOG_LOC(info, fmt, __VA_ARGS__)
#define LOG_LOC_DEBUG(fmt, ...)    LOG_LOC(debug, fmt, __VA_ARGS__)
#define LOG_LOC_WARN(fmt, ...)     LOG_LOC(warn, fmt, __VA_ARGS__)

#define CHECK_GLFW_ERROR(err)                                                  \
  do {                                                                         \
    if (err != GLFW_NO_ERROR) {                                                \
      LOG_LOC_INFO("glfw error: {}", glfwGetError(nullptr));                   \
    }                                                                          \
  } while (0);

std::optional<std::vector<u8>> readFromFile(std::string const &filename,
                                            ccstr              mode);