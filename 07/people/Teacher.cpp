#include "Teacher.h"

std::string Teacher::GetSubject() const
{
	return m_subject;
}

void Teacher::SetSubject(const std::string& subject)
{
	m_subject = subject;
}
