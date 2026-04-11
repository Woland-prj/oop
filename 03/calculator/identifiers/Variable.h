#ifndef VARIABLE_H
#define VARIABLE_H

#include <optional>
#include <string>

class Variable
{
public:
	explicit Variable(std::string name);

	std::optional<double> GetValue() const;
	void SetValue(double value);
	void ClearValue();
	const std::string& GetName() const;

private:
	std::string m_name;
	std::optional<double> m_value;
};

#endif // VARIABLE_H
