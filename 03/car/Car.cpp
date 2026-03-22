#include "Car.hpp"
#include <cstdint>
#include <stdexcept>

const std::unordered_map<Car::Gear, Car::SpeedRange> Car::GEAR_SPEED_MATCH_TABLE = {
	{ Gear::Reverse, { 0, 20 } },
	{ Gear::Neutral, { 0, 150 } },
	{ Gear::First, { 0, 30 } },
	{ Gear::Second, { 20, 50 } },
	{ Gear::Third, { 30, 60 } },
	{ Gear::Fourth, { 40, 90 } },
	{ Gear::Fifth, { 50, 150 } }
};

uint8_t GetAbsSpeed(int8_t speed);

bool Car::IsTurnedOn() const
{
	return m_isEngineOn;
}

CarDirection Car::GetDirection() const
{
	if (m_speed == 0)
		return CarDirection::Standing;
	if (m_speed < 0)
		return CarDirection::Backward;
	return CarDirection::Forward;
}

int8_t Car::GetSpeed() const
{
	return m_speed < 0 ? -m_speed : m_speed;
}

int8_t Car::GetGear() const
{
	return static_cast<int8_t>(m_gear);
}

void Car::TurnOnEngine()
{
	m_isEngineOn = true;
}

void Car::TurnOffEngine()
{
	if (!m_isEngineOn)
		return;

	if (m_gear != Gear::Neutral || m_speed != 0)
		throw std::runtime_error("Сar must be stopped and in neutral gear");
}

void Car::SetGear(int8_t gear)
{
	if (gear < MIN_AVALIABLE_GEAR || gear > MAX_AVALIABLE_GEAR)
		throw std::invalid_argument("Invalid gear");

	if (!m_isEngineOn)
		throw std::runtime_error("Сannot set gear while engine is off");

	const auto newGear = static_cast<Gear>(gear);
	if (m_gear == newGear)
		return;

	if (!IsGearMatchSpeed(newGear, GetAbsSpeed(m_speed)))
		throw std::runtime_error("Unsuitable current speed");

	if (newGear == Gear::Reverse && m_speed != 0)
		throw std::runtime_error("Cannot reverse while moving");

	if (newGear != Gear::Neutral && m_speed < 0)
		throw std::runtime_error("Unsuitable current speed");

	m_gear = newGear;
}

void Car::SetSpeed(int8_t speed)
{
	if (speed < 0)
		throw std::invalid_argument("Speed cannot be negative");
	if (!m_isEngineOn)
		throw std::runtime_error("Cannot set speed while engine is off");

	auto curSpeed = GetAbsSpeed(m_speed);

	if (speed == curSpeed)
		return;

	if (m_gear == Gear::Neutral && speed > curSpeed)
		throw std::runtime_error("Cannot accelerate on neutral");

	if (!IsGearMatchSpeed(m_gear, speed))
		throw std::runtime_error("Speed is out of gear range");

	m_speed = (m_speed < 0 || m_gear == Gear::Reverse) ? -speed : speed;
}

bool Car::IsGearMatchSpeed(Gear gear, uint8_t absSpeed)
{
	const auto [minSpeed, maxSpeed] = GEAR_SPEED_MATCH_TABLE.at(gear);
	return (absSpeed >= minSpeed && absSpeed <= maxSpeed);
}

uint8_t GetAbsSpeed(int8_t speed)
{
	return speed < 0 ? -speed : speed;
}

std::ostream& operator<<(std::ostream& out, CarDirection dir)
{
	switch (dir)
	{
	case CarDirection::Forward:
		out << "forward";
		break;
	case CarDirection::Backward:
		out << "backward";
		break;
	case CarDirection::Standing:
		out << "standing still";
		break;
	}
	return out;
}
