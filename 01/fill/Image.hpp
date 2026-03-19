#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <istream>
#include <ostream>
#include <vector>

using Image = std::vector<std::string>;

constexpr size_t k_imgMaxSize = 100;

constexpr char k_imgStartCh = 'O';
constexpr char k_imgFillCh = '.';
constexpr char k_imgEmptyCh = ' ';

Image LoadImage(std::istream& in);
void SaveImage(const Image& img, std::ostream& out);

#endif // IMAGE_HPP
