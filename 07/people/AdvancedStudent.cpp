#include "AdvancedStudent.h"

std::string AdvancedStudent::GetThesisTopic() const
{
	return m_thesisTopic;
}

void AdvancedStudent::SetThesisTopic(const std::string& thesisTopic)
{
	m_thesisTopic = thesisTopic;
}
