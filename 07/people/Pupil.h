#ifndef CPUPIL_H
#define CPUPIL_H

#include "IPupil.h"
#include "PersonImpl.h"

class Pupil final : public PersonImpl<IPupil>
{
public:
	Pupil() = default;

	Pupil(
		const std::string& surname,
		const std::string& name,
		const std::string& patronymic,
		const std::string& address,
		const std::string& schoolName,
		const std::string& schoolClass)
		: PersonImpl(surname, name, patronymic, address)
		, m_schoolName(schoolName)
		, m_schoolClass(schoolClass)
	{
	}

	std::string GetSchoolName() const override;
	std::string GetSchoolClass() const override;

	void SetSchoolName(const std::string& schoolName) override;
	void SetSchoolClass(const std::string& schoolClass) override;

private:
	std::string m_schoolName;
	std::string m_schoolClass;
};

#endif // CPUPIL_H
