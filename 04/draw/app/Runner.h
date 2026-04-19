#ifndef APP_RUNNER_H
#define APP_RUNNER_H

#include "canvas/ICanvasDrawable.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

class IShape;

namespace App
{

struct AnalysisResult
{
	const IShape* maxAreaShape = nullptr;
	const IShape* minPerimeterShape = nullptr;
	bool hasErrors = false;
	std::string errorMessage;
};

struct Config
{
	bool enableRender = false;
	bool showHelp = false;
	std::string inputPath;
	std::string outputPath;

	bool shouldExitEarly() const { return showHelp; }
};

Config ParseArguments(int argc, char* argv[]);
void PrintUsage(std::ostream& out, const char* programName);

AnalysisResult Run(std::istream& input, std::ostream& output, const Config& config);

void RunRender(const std::vector<std::shared_ptr<ICanvasDrawable>>& shapes,
	uint32_t windowWidth = 800, uint32_t windowHeight = 600);

} // namespace App

#endif // APP_RUNNER_H
