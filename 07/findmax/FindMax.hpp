#ifndef FIND_MAX_HPP
#define FIND_MAX_HPP
#include <vector>

template <typename T, typename Less>
bool FindMax(std::vector<T> const& arr, T& maxValue, Less const& less)
{
	if (arr.empty())
		return false;

	std::size_t maxIdx = 0;
	for (std::size_t i = 1; i < arr.size(); ++i)
	{
		if (less(arr[maxIdx], arr[i]))
			maxIdx = i;
	}

	maxValue = arr[maxIdx];
	return true;
}

#endif // FIND_MAX_HPP
