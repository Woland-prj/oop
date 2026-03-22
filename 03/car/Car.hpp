#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>
#include <ostream>
#include <unordered_map>
enum class CarDirection
{
	Forward,
	Backward,
	Standing
};

std::ostream& operator<<(std::ostream& out, CarDirection dir);

class Car
{
public:
	bool IsTurnedOn() const;
	CarDirection GetDirection() const;
	int8_t GetSpeed() const;
	int8_t GetGear() const;
	void TurnOnEngine();
	void TurnOffEngine();
	void SetGear(int8_t gear);
	void SetSpeed(int8_t speed);

private:
	enum class Gear
	{
		Reverse = -1,
		Neutral = 0,
		First = 1,
		Second,
		Third,
		Fourth,
		Fifth
	};

	using SpeedRange = std::pair<uint8_t, uint8_t>;

	static constexpr auto MIN_AVALIABLE_GEAR = static_cast<int8_t>(Gear::Reverse);
	static constexpr auto MAX_AVALIABLE_GEAR = static_cast<int8_t>(Gear::Fifth);

	static const std::unordered_map<Gear, SpeedRange> GEAR_SPEED_MATCH_TABLE;

	static bool IsGearMatchSpeed(Gear gear, uint8_t absSpeed);

	Gear m_gear = Gear::Neutral;
	int8_t m_speed = 0;
	bool m_isEngineOn = false;
};

#endif // CAR_HPP
