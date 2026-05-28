#ifndef ISTUDENT_H
#define ISTUDENT_H

#include "IPerson.h"

class IStudent : public IPerson
{
public:
	~IStudent() override = default;

	virtual std::string GetUniversityName() const = 0;
	virtual std::string GetStudentId() const = 0;

	virtual void SetUniversityName(const std::string& universityName) = 0;
	virtual void SetStudentId(const std::string& studentId) = 0;
};

#endif // ISTUDENT_H
