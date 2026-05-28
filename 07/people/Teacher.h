#ifndef CTEACHER_H
#define CTEACHER_H

#include "ITeacher.h"
#include "PersonImpl.h"

class Teacher final : public PersonImpl<ITeacher>
{
public:
	Teacher() = default;

	Teacher(
		const std::string& surname,
		const std::string& name,
		const std::string& patronymic,
		const std::string& address,
		const std::string& subject)
		: PersonImpl(surname, name, patronymic, address)
		, m_subject(subject)
	{
	}

	std::string GetSubject() const override;
	void SetSubject(const std::string& subject) override;

private:
	std::string m_subject;
};

#endif // CTEACHER_H
