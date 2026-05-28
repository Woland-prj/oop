#ifndef CSTUDENT_H
#define CSTUDENT_H

#include "IStudent.h"
#include "StudentImpl.h"

class Student final : public StudentImpl<IStudent>
{
public:
	using StudentImpl::StudentImpl;
};

#endif // CSTUDENT_H
