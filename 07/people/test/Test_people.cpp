#include <catch2/catch_test_macros.hpp>

#include "AdvancedStudent.h"
#include "Pupil.h"
#include "Student.h"
#include "Teacher.h"
#include "Worker.h"

TEST_CASE("CPupil properties via setters and getters", "[pupil]")
{
	Pupil pupil;
	pupil.SetSurname("Ivanov");
	pupil.SetName("Ivan");
	pupil.SetPatronymic("Ivanovich");
	pupil.SetAddress("Moscow");
	pupil.SetSchoolName("School 42");
	pupil.SetSchoolClass("7b");

	CHECK(pupil.GetSurname() == "Ivanov");
	CHECK(pupil.GetName() == "Ivan");
	CHECK(pupil.GetPatronymic() == "Ivanovich");
	CHECK(pupil.GetAddress() == "Moscow");
	CHECK(pupil.GetSchoolName() == "School 42");
	CHECK(pupil.GetSchoolClass() == "7b");
}

TEST_CASE("CPupil constructor with parameters", "[pupil]")
{
	Pupil pupil("Ivanov", "Ivan", "Ivanovich", "Moscow", "School 42", "7b");

	CHECK(pupil.GetSurname() == "Ivanov");
	CHECK(pupil.GetName() == "Ivan");
	CHECK(pupil.GetPatronymic() == "Ivanovich");
	CHECK(pupil.GetAddress() == "Moscow");
	CHECK(pupil.GetSchoolName() == "School 42");
	CHECK(pupil.GetSchoolClass() == "7b");
}

TEST_CASE("CTeacher properties", "[teacher]")
{
	Teacher teacher("Petrova", "Maria", "Sergeevna", "SPb", "Mathematics");

	CHECK(teacher.GetSurname() == "Petrova");
	CHECK(teacher.GetSubject() == "Mathematics");

	teacher.SetSubject("Physics");
	CHECK(teacher.GetSubject() == "Physics");
}

TEST_CASE("CStudent properties", "[student]")
{
	Student student("Sidorov", "Alexey", "Vladimirovich", "Novosibirsk", "NSU", "SB-12345");

	CHECK(student.GetSurname() == "Sidorov");
	CHECK(student.GetUniversityName() == "NSU");
	CHECK(student.GetStudentId() == "SB-12345");

	student.SetUniversityName("NSTU");
	CHECK(student.GetUniversityName() == "NSTU");

	student.SetStudentId("SB-99999");
	CHECK(student.GetStudentId() == "SB-99999");
}

TEST_CASE("CAdvancedStudent properties", "[advanced_student]")
{
	AdvancedStudent as("Kuznetsova", "Elena", "Andreevna", "Kazan", "KFU", "AS-67890", "Neural Networks");

	CHECK(as.GetSurname() == "Kuznetsova");
	CHECK(as.GetUniversityName() == "KFU");
	CHECK(as.GetStudentId() == "AS-67890");
	CHECK(as.GetThesisTopic() == "Neural Networks");

	as.SetThesisTopic("Machine Learning");
	CHECK(as.GetThesisTopic() == "Machine Learning");
}

TEST_CASE("CWorker properties", "[worker]")
{
	Worker worker("Smirnov", "Dmitry", "Olegovich", "Ekb", "Engineer");

	CHECK(worker.GetSurname() == "Smirnov");
	CHECK(worker.GetSpecialty() == "Engineer");

	worker.SetSpecialty("Senior Engineer");
	CHECK(worker.GetSpecialty() == "Senior Engineer");
}

TEST_CASE("Polymorphism via IPerson pointer", "[polymorphism]")
{
	std::unique_ptr<IPerson> person = std::make_unique<Worker>(
		"Smirnov", "Dmitry", "Olegovich", "Ekb", "Engineer");

	CHECK(person->GetSurname() == "Smirnov");
	CHECK(person->GetName() == "Dmitry");
	CHECK(person->GetPatronymic() == "Olegovich");
	CHECK(person->GetAddress() == "Ekb");

	person->SetSurname("Petrov");
	CHECK(person->GetSurname() == "Petrov");
}

TEST_CASE("Polymorphism via IStudent pointer", "[polymorphism]")
{
	std::unique_ptr<IStudent> student = std::make_unique<AdvancedStudent>(
		"Kuznetsova", "Elena", "Andreevna", "Kazan", "KFU", "67890", "Neural Networks");

	CHECK(student->GetSurname() == "Kuznetsova");
	CHECK(student->GetUniversityName() == "KFU");
	CHECK(student->GetStudentId() == "67890");
}

TEST_CASE("Default construction and empty strings", "[default]")
{
	Pupil pupil;
	CHECK(pupil.GetSurname().empty());
	CHECK(pupil.GetName().empty());
	CHECK(pupil.GetPatronymic().empty());
	CHECK(pupil.GetAddress().empty());
	CHECK(pupil.GetSchoolName().empty());
	CHECK(pupil.GetSchoolClass().empty());

	Teacher teacher;
	CHECK(teacher.GetSubject().empty());

	Student student;
	CHECK(student.GetUniversityName().empty());
	CHECK(student.GetStudentId().empty());

	AdvancedStudent as;
	CHECK(as.GetThesisTopic().empty());

	Worker worker;
	CHECK(worker.GetSpecialty().empty());
}
