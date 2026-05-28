#include "Pupil.h"

std::string Pupil::GetSchoolName() const
{
	return m_schoolName;
}

std::string Pupil::GetSchoolClass() const
{
	return m_schoolClass;
}

void Pupil::SetSchoolName(const std::string& schoolName)
{
	m_schoolName = schoolName;
}

void Pupil::SetSchoolClass(const std::string& schoolClass)
{
	m_schoolClass = schoolClass;
}
