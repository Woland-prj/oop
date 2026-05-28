#include "Worker.h"

std::string Worker::GetSpecialty() const
{
	return m_specialty;
}

void Worker::SetSpecialty(const std::string& specialty)
{
	m_specialty = specialty;
}
