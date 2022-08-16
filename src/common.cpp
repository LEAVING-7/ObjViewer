#include "common.hpp"

std::optional<std::vector<u8>> readFromFile(std::string const& filename,
                                            ccstr              mode) {
  FILE* fp = ::fopen(filename.c_str(), mode);
  if (fp == nullptr) {
    LOG_INFO("Open {} failed: {}", filename, strerror(errno));
    return std::nullopt;
  } else {
    ::fseek(fp, 0, SEEK_END);
    auto            filesize = ::ftell(fp);
    std::vector<u8> ret(filesize);
    ::rewind(fp);
    ::fread(ret.data(), filesize, 1, fp);
    ::fclose(fp);
    return std::make_optional(std::move(ret));
  }
}

VkResultChecker::VkResultChecker(VkResult result) {
  assert(result == VK_SUCCESS);
}
VkResultChecker& VkResultChecker::operator=(VkResult result) {
  assert(result == VK_SUCCESS);
  return *this;
}