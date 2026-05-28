#ifndef CADVANCEDSTUDENT_H
#define CADVANCEDSTUDENT_H

#include "IAdvancedStudent.h"
#include "StudentImpl.h"

class AdvancedStudent final : public StudentImpl<IAdvancedStudent>
{
public:
	AdvancedStudent() = default;

	AdvancedStudent(
		const std::string& surname,
		const std::string& name,
		const std::string& patronymic,
		const std::string& address,
		const std::string& universityName,
		const std::string& studentId,
		const std::string& thesisTopic)
		: StudentImpl(surname, name, patronymic, address, universityName, studentId)
		, m_thesisTopic(thesisTopic)
	{
	}

	std::string GetThesisTopic() const override;
	void SetThesisTopic(const std::string& thesisTopic) override;

private:
	std::string m_thesisTopic;
};

#endif // CADVANCEDSTUDENT_H
