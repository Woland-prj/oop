#include "CarDriver.hpp"
#include <charconv>
#include <sstream>

CarDriver::CarDriver(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	InitCommands();
}

void CarDriver::Drive(Car& car)
{
	std::string line;
	while (std::getline(m_input, line))
	{
		HandleCommand(line, car);
	}
}

void CarDriver::InitCommands()
{
	m_commands["Info"] = WrapNoArg(&CarDriver::InfoCmd);
	m_commands["EngineOn"] = WrapNoArg(&CarDriver::EngineOnCmd);
	m_commands["EngineOff"] = WrapNoArg(&CarDriver::EngineOffCmd);
	m_commands["SetGear"] = WrapArg(&CarDriver::SetGearCmd);
	m_commands["SetSpeed"] = WrapArg(&CarDriver::SetSpeedCmd);
}

void CarDriver::HandleCommand(const std::string& line, Car& car)
{
	std::istringstream iss(line);
	std::string command;
	iss >> command;

	if (command.empty())
		return;

	auto it = m_commands.find(command);
	if (it == m_commands.end())
	{
		m_output << "Unknown command\n";
		return;
	}

	try
	{
		it->second(iss, car);
	}
	catch (const std::exception& ex)
	{
		m_output << ex.what() << '\n';
	}
}

CarDriver::Handler CarDriver::WrapNoArg(void (CarDriver::*method)(Car&))
{
	return [this, method](std::istringstream&, Car& car) {
		(this->*method)(car);
	};
}

CarDriver::Handler CarDriver::WrapArg(void (CarDriver::*method)(Car&, int8_t))
{
	return [this, method](std::istringstream& iss, Car& car) {
		std::string arg;
		if (!(iss >> arg))
			throw std::invalid_argument("Invalid command argument");

		int8_t value = ParseInt(arg);

		(this->*method)(car, value);
	};
}

int8_t CarDriver::ParseInt(std::string_view arg)
{
	if (arg.empty())
		throw std::invalid_argument("Invalid command argument");

	int value{};
	auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), value);

	if (ec != std::errc() || ptr != arg.data() + arg.size())
		throw std::invalid_argument("Invalid command argument");

	return static_cast<int8_t>(value);
}

void CarDriver::InfoCmd(Car& car)
{
	m_output << "Engine: " << (car.IsTurnedOn() ? "on" : "off") << '\n';
	m_output << "Direction: " << car.GetDirection() << '\n';
	m_output << "Speed: " << static_cast<int>(car.GetSpeed()) << '\n';
	m_output << "Gear: " << static_cast<int>(car.GetGear()) << '\n';
}

void CarDriver::EngineOnCmd(Car& car)
{
	car.TurnOnEngine();
}

void CarDriver::EngineOffCmd(Car& car)
{
	car.TurnOffEngine();
}

void CarDriver::SetGearCmd(Car& car, int8_t gear)
{
	car.SetGear(gear);
}

void CarDriver::SetSpeedCmd(Car& car, int8_t speed)
{
	car.SetSpeed(speed);
}
