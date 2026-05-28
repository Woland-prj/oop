#ifndef IADVANCEDSTUDENT_H
#define IADVANCEDSTUDENT_H

#include "IStudent.h"

class IAdvancedStudent : public IStudent
{
public:
	~IAdvancedStudent() override = default;

	virtual std::string GetThesisTopic() const = 0;
	virtual void SetThesisTopic(const std::string& thesisTopic) = 0;
};

#endif // IADVANCEDSTUDENT_H
