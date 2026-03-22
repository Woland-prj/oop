#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "Car.hpp"

using namespace std;

TEST_CASE("Car initial state", "[init]")
{
	Car car;

	REQUIRE_FALSE(car.IsTurnedOn());
	REQUIRE(car.GetSpeed() == 0);
	REQUIRE(car.GetGear() == 0);
	REQUIRE(car.GetDirection() == CarDirection::Standing);
}

TEST_CASE("Engine can be turned on", "[engine]")
{
	Car car;

	REQUIRE_NOTHROW(car.TurnOnEngine());
	REQUIRE(car.IsTurnedOn());
}

TEST_CASE("Engine can be turned off only at neutral and zero speed", "[engine]")
{
	Car car;
	car.TurnOnEngine();

	REQUIRE_NOTHROW(car.TurnOffEngine());
}

TEST_CASE("Cannot turn off engine when moving", "[engine]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(10);

	REQUIRE_THROWS_WITH(car.TurnOffEngine(), "Сar must be stopped and in neutral gear");
}

TEST_CASE("Cannot turn off engine if not in neutral", "[engine]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);

	REQUIRE_THROWS_WITH(car.TurnOffEngine(), "Сar must be stopped and in neutral gear");
}

TEST_CASE("Cannot set gear when engine is off", "[gear]")
{
	Car car;

	REQUIRE_THROWS_WITH(car.SetGear(1), "Сannot set gear while engine is off");
}

TEST_CASE("Invalid gear throws", "[gear]")
{
	Car car;
	car.TurnOnEngine();

	REQUIRE_THROWS_AS(car.SetGear(10), invalid_argument);
	REQUIRE_THROWS_AS(car.SetGear(-2), invalid_argument);
}

TEST_CASE("Valid gear switching", "[gear]")
{
	Car car;
	car.TurnOnEngine();

	REQUIRE_NOTHROW(car.SetGear(1));
	REQUIRE(car.GetGear() == 1);
}

TEST_CASE("Cannot set reverse while moving", "[gear]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(10);

	REQUIRE_THROWS_WITH(car.SetGear(-1), "Cannot reverse while moving");
}

TEST_CASE("Cannot switch gear if speed not in range", "[gear]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(10);

	REQUIRE_THROWS_WITH(car.SetGear(2), "Unsuitable current speed");
}

TEST_CASE("Can switch to neutral at any speed", "[gear]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(20);

	REQUIRE_NOTHROW(car.SetGear(0));
	REQUIRE(car.GetGear() == 0);
}

TEST_CASE("Cannot set speed when engine is off", "[speed]")
{
	Car car;

	REQUIRE_THROWS_WITH(car.SetSpeed(10), "Cannot set speed while engine is off");
}

TEST_CASE("Cannot set negative speed", "[speed]")
{
	Car car;
	car.TurnOnEngine();

	REQUIRE_THROWS_AS(car.SetSpeed(-1), invalid_argument);
}

TEST_CASE("Cannot accelerate on neutral", "[speed]")
{
	Car car;
	car.TurnOnEngine();

	REQUIRE_THROWS_WITH(car.SetSpeed(10), "Cannot accelerate on neutral");
}

TEST_CASE("Can decrease speed on neutral", "[speed]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(20);

	car.SetGear(0);

	REQUIRE_NOTHROW(car.SetSpeed(10));
	REQUIRE(car.GetSpeed() == 10);
}

TEST_CASE("Speed must match gear range", "[speed]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);

	REQUIRE_THROWS_WITH(car.SetSpeed(40), "Speed is out of gear range");
}

TEST_CASE("Valid speed change", "[speed]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);

	REQUIRE_NOTHROW(car.SetSpeed(20));
	REQUIRE(car.GetSpeed() == 20);
}

TEST_CASE("Direction forward", "[direction]")
{
	Car car;
	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(10);

	REQUIRE(car.GetDirection() == CarDirection::Forward);
}

TEST_CASE("Direction standing still", "[direction]")
{
	Car car;
	REQUIRE(car.GetDirection() == CarDirection::Standing);
}

TEST_CASE("Reverse gear produces backward direction", "[direction]")
{
	Car car;
	car.TurnOnEngine();

	car.SetGear(-1);
	car.SetSpeed(10);

	REQUIRE(car.GetDirection() == CarDirection::Backward);
}

TEST_CASE("Switch from reverse to forward only at zero speed", "[gear]")
{
	Car car;
	car.TurnOnEngine();

	car.SetGear(-1);
	car.SetSpeed(10);

	car.SetGear(0);

	REQUIRE_THROWS_WITH(car.SetGear(1), "Unsuitable current speed");
}

TEST_CASE("Full valid scenario", "[integration]")
{
	Car car;

	car.TurnOnEngine();
	car.SetGear(1);
	car.SetSpeed(30);

	REQUIRE(car.GetSpeed() == 30);
	REQUIRE(car.GetDirection() == CarDirection::Forward);

	car.SetGear(0);
	car.SetSpeed(10);

	REQUIRE(car.GetSpeed() == 10);

	car.SetSpeed(0);
	car.SetGear(0);
	REQUIRE_NOTHROW(car.TurnOffEngine());
}
