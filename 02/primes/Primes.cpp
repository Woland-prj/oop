#include <set>
#include <vector>

std::set<int> GeneratePrimeNumbersSet(int upperBound)
{
	std::set<int> primes;

	if (upperBound < 2)
		return primes;

	std::vector<bool> sieve(upperBound + 1, true);

	sieve[0] = false;
	sieve[1] = false;

	for (int i = 2; i * i <= upperBound; ++i)
	{
		if (!sieve[i])
			continue;

		for (int j = i * i; j <= upperBound; j += i)
			sieve[j] = false;
	}

	for (int i = 2; i <= upperBound; ++i)
	{
		if (sieve[i])
			primes.insert(i);
	}

	return primes;
}
