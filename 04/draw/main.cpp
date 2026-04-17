#include "app/Runner.h"
#include <iostream>
#include <fstream>

// можно вынести интерфейс как абстрактный класс
int main(int argc, char* argv[])
{
	const auto config = App::ParseArguments(argc, argv);

	if (config.shouldExitEarly()) {
		return 0;
	}

	std::ifstream inputFile;
	std::ofstream outputFile;

	std::istream& input = config.inputPath.empty()
		? std::cin
		: (inputFile.open(config.inputPath), inputFile);

	std::ostream& output = config.outputPath.empty()
		? std::cout
		: (outputFile.open(config.outputPath), outputFile);

	if (!input) {
		std::cerr << "Error: cannot open input: " << config.inputPath << "\n";
		return 1;
	}
	if (!output && !config.outputPath.empty()) {
		std::cerr << "Error: cannot open output: " << config.outputPath << "\n";
		return 1;
	}

	const auto result = App::Run(input, output, config);
	return result.hasErrors ? 1 : 0;
}