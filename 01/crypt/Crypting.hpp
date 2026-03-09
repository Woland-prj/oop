#ifndef CRYPTING_HPP
#define CRYPTING_HPP

#include <cstdint>
#include <istream>
#include <ostream>

using CryptOperation = std::function<void(std::istream&, std::ostream&, uint8_t)>;

void Crypt(std::istream& in, std::ostream& out, uint8_t key);
void Decrypt(std::istream& in, std::ostream& out, uint8_t key);

#endif // CRYPTING_HPP
