#ifndef CAR_DRIVER_HPP
#define CAR_DRIVER_HPP

#include "Car.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

class CarDriver
{
public:
	CarDriver(std::istream& input, std::ostream& output);

	void Drive(Car& car);

private:
	using Handler = std::function<void(std::istringstream&, Car&)>;

	std::istream& m_input;
	std::ostream& m_output;

	std::unordered_map<std::string, Handler> m_commands;

	void InitCommands();
	void HandleCommand(const std::string& line, Car& car);

	void InfoCmd(Car& car);
	void EngineOnCmd(Car& car);
	void EngineOffCmd(Car& car);
	void SetGearCmd(Car& car, int8_t gear);
	void SetSpeedCmd(Car& car, int8_t speed);

	Handler WrapNoArg(void (CarDriver::*method)(Car&));
	Handler WrapArg(void (CarDriver::*method)(Car&, int8_t));

	int8_t ParseInt(std::string_view arg);
};

#endif
