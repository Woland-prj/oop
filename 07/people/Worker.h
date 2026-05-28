#ifndef CWORKER_H
#define CWORKER_H

#include "IWorker.h"
#include "PersonImpl.h"

class Worker final : public PersonImpl<IWorker>
{
public:
	Worker() = default;

	Worker(
		const std::string& surname,
		const std::string& name,
		const std::string& patronymic,
		const std::string& address,
		const std::string& specialty)
		: PersonImpl(surname, name, patronymic, address)
		, m_specialty(specialty)
	{
	}

	std::string GetSpecialty() const override;
	void SetSpecialty(const std::string& specialty) override;

private:
	std::string m_specialty;
};

#endif // CWORKER_H
