#ifndef UTF8_VALIDATION_H
#define UTF8_VALIDATION_H

#include <cstddef>
#include <string>

namespace utf8
{

unsigned int ExtractCodepoint(const std::string& str, size_t pos, size_t seqLen);
size_t GetValidSequenceLength(const std::string& str, size_t pos);
bool IsValid(const std::string& str);

} // namespace utf8

#endif // UTF8_VALIDATION_H
