#ifndef CSTUDENTIMPL_H
#define CSTUDENTIMPL_H

#include "PersonImpl.h"

template <typename Base>
class StudentImpl : public PersonImpl<Base>
{
public:
	StudentImpl() = default;

	StudentImpl(
		const std::string& surname,
		const std::string& name,
		const std::string& patronymic,
		const std::string& address,
		const std::string& universityName,
		const std::string& studentId)
		: PersonImpl<Base>(surname, name, patronymic, address)
		, m_universityName(universityName)
		, m_studentId(studentId)
	{
	}

	std::string GetUniversityName() const override
	{
		return m_universityName;
	}

	std::string GetStudentId() const override
	{
		return m_studentId;
	}

	void SetUniversityName(const std::string& universityName) override
	{
		m_universityName = universityName;
	}

	void SetStudentId(const std::string& studentId) override
	{
		m_studentId = studentId;
	}

private:
	std::string m_universityName;
	std::string m_studentId;
};

#endif // CSTUDENTIMPL_H
