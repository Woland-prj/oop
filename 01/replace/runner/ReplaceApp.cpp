#include "ReplaceApp.h"
#include "stream_processor/StreamIO.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void RunFile(char* argv[])
{
	const std::string inputFileName = argv[1];
	const std::string outputFileName = argv[2];
	std::string searchStr = argv[3];
	std::string replaceStr = argv[4];

	if (inputFileName == outputFileName)
		throw std::invalid_argument("Input and output files cannot be same file");

	std::ifstream input(inputFileName);
	if (!input.is_open())
		throw std::runtime_error("Cannot open file for reading: " + inputFileName);

	std::ofstream output(outputFileName);
	if (!output.is_open())
	{
		input.close();
		throw std::runtime_error("Cannot open file for writing: " + outputFileName);
	}

	StreamIO::Process(input, output, std::move(searchStr), std::move(replaceStr));
}

void RunCli()
{
	std::string searchStr, replaceStr;

	if (!std::getline(std::cin, searchStr))
		throw std::runtime_error("Failed to read search string");
	if (!std::getline(std::cin, replaceStr))
		throw std::runtime_error("Failed to read replace string");

	StreamIO::Process(std::cin, std::cout, std::move(searchStr), std::move(replaceStr));
}

void RunHelp()
{
	std::cout << "replace - substring replacement utility\n"
				 "\n"
				 "Usage:\n"
				 "  replace                            # Interactive mode\n"
				 "  replace -h                         # Show this help\n"
				 "  replace <in> <out> <search> <replace>  # File mode\n"
				 "\n"
				 "Modes:\n"
				 "  Interactive:\n"
				 "    - Line 1: search string\n"
				 "    - Line 2: replacement string\n"
				 "    - Following lines: text to process (Ctrl+D to finish)\n"
				 "\n"
				 "  File mode:\n"
				 "    Processes <in> file, replaces all occurrences of <search>\n"
				 "    with <replace>, writes result to <out>.\n"
				 "\n"
				 "Example:\n"
				 "  replace input.txt output.txt 'foo' 'bar'\n";
}

void ReplaceApp::Run(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) != "-h")
		throw std::invalid_argument("Unknown argument: " + std::string(argv[1]));
	if (argc != 1 && argc != 2 && argc != 5)
		throw std::invalid_argument("Need 4 arguments or 1 or zero");

	switch (argc)
	{
	case 1:
		RunCli();
		break;
	case 2:
		RunHelp();
		break;
	case 5:
		RunFile(argv);
		break;
	}
}
