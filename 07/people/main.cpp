#include "AdvancedStudent.h"
#include "Pupil.h"
#include "Student.h"
#include "Teacher.h"
#include "Worker.h"

#include <iostream>
#include <memory>
#include <vector>

static void PrintPersonInfo(const IPerson& person)
{
	std::cout << person.GetSurname() << " "
			  << person.GetName() << " "
			  << person.GetPatronymic() << ", "
			  << person.GetAddress() << "\n";
}

int main()
{
	Pupil pupil("Ivanov", "Ivan", "Ivanovich", "Moscow", "School 42", "7b");
	Teacher teacher("Petrova", "Maria", "Sergeevna", "Saint Petersburg", "Mathematics");
	Student student("Sidorov", "Alexey", "Vladimirovich", "Novosibirsk", "NSU", "SB-12345");
	AdvancedStudent advancedStudent("Kuznetsova", "Elena", "Andreevna", "Kazan", "KFU", "AS-67890", "Programming");
	Worker worker("Smirnov", "Dmitry", "Olegovich", "Yekaterinburg", "Engineer");

	std::vector<std::unique_ptr<IPerson>> people;
	people.push_back(std::make_unique<Pupil>(pupil));
	people.push_back(std::make_unique<Teacher>(teacher));
	people.push_back(std::make_unique<Student>(student));
	people.push_back(std::make_unique<AdvancedStudent>(advancedStudent));
	people.push_back(std::make_unique<Worker>(worker));

	for (const auto& person : people)
	{
		PrintPersonInfo(*person);
	}

	std::cout << "\n";

	std::cout << "Teacher's subject: " << teacher.GetSubject() << "\n";
	std::cout << "Worker's specialty: " << worker.GetSpecialty() << "\n";
	std::cout << "Student's university: " << student.GetUniversityName() << "\n";
	std::cout << "Advanced student's thesis: " << advancedStudent.GetThesisTopic() << "\n";
	std::cout << "Pupil's school: " << pupil.GetSchoolName() << ", class: " << pupil.GetSchoolClass() << "\n";
}
