#ifndef IWORKER_H
#define IWORKER_H

#include "IPerson.h"

class IWorker : public IPerson
{
public:
	~IWorker() override = default;

	virtual std::string GetSpecialty() const = 0;
	virtual void SetSpecialty(const std::string& specialty) = 0;
};

#endif // IWORKER_H
