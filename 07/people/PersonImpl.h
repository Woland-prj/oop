#ifndef CPERSONIMPL_H
#define CPERSONIMPL_H

#include <string>

template <typename Base>
class PersonImpl : public Base
{
public:
	PersonImpl() = default;

	PersonImpl(
		const std::string& surname,
		const std::string& name,
		const std::string& patronymic,
		const std::string& address)
		: m_surname(surname)
		, m_name(name)
		, m_patronymic(patronymic)
		, m_address(address)
	{
	}

	std::string GetSurname() const override
	{
		return m_surname;
	}

	std::string GetName() const override
	{
		return m_name;
	}

	std::string GetPatronymic() const override
	{
		return m_patronymic;
	}

	std::string GetAddress() const override
	{
		return m_address;
	}

	void SetSurname(const std::string& surname) override
	{
		m_surname = surname;
	}

	void SetName(const std::string& name) override
	{
		m_name = name;
	}

	void SetPatronymic(const std::string& patronymic) override
	{
		m_patronymic = patronymic;
	}

	void SetAddress(const std::string& address) override
	{
		m_address = address;
	}

private:
	std::string m_surname;
	std::string m_name;
	std::string m_patronymic;
	std::string m_address;
};

#endif // CPERSONIMPL_H
