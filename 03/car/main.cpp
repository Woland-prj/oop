#include "Car.hpp"
#include "CarDriver.hpp"
#include <iostream>

int main()
{
	Car car;
	CarDriver driver(std::cin, std::cout);

	driver.Drive(car);

	return 0;
}
