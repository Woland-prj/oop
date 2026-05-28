#ifndef ITEACHER_H
#define ITEACHER_H

#include "IPerson.h"

class ITeacher : public IPerson
{
public:
	~ITeacher() override = default;

	virtual std::string GetSubject() const = 0;
	virtual void SetSubject(const std::string& subject) = 0;
};

#endif // ITEACHER_H
