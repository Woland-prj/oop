#ifndef IPUPIL_H
#define IPUPIL_H

#include "IPerson.h"

class IPupil : public IPerson
{
public:
	~IPupil() override = default;

	virtual std::string GetSchoolName() const = 0;
	virtual std::string GetSchoolClass() const = 0;

	virtual void SetSchoolName(const std::string& schoolName) = 0;
	virtual void SetSchoolClass(const std::string& schoolClass) = 0;
};

#endif // IPUPIL_H
