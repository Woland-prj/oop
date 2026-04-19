#include "Runner.h"
#include "analyzer/ShapeAnalyzer.h"
#include "basic_types/IShape.h"
#include "canvas/ICanvasDrawable.h"
#include "canvas/SfmlCanvas.h"
#include "parser/ShapeParser.h"

#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>

constexpr std::string_view DEFAULT_WINDOW_TITLE = "Geometric Shapes";
constexpr unsigned int DEFAULT_FRAMERATE = 60;
constexpr double DEFAULT_RENDER_SCALE = 1.0;
constexpr unsigned int DEFAULT_ANTI_ALIASING_LEVEL = 8;

namespace App
{

void PrintUsage(std::ostream& out, const char* programName)
{
	out << "Usage: " << programName << " [OPTIONS]\n"
		<< "Options:\n"
		<< "  -r, --render          Enable SFML visualization\n"
		<< "  -i, --input FILE      Read from FILE instead of stdin\n"
		<< "  -o, --output FILE     Write to FILE instead of stdout\n"
		<< "  -h, --help            Show this help\n"
		<< "\nExamples:\n"
		<< "  echo 'rectangle 0 0 10 10 ff0000 00ff00' | " << programName << "\n"
		<< "  " << programName << " --render < shapes.txt\n";
}

Config ParseArguments(int argc, char* argv[])
{
	Config config;

	for (int i = 1; i < argc; ++i)
	{
		const std::string arg = argv[i];

		if (arg == "--render" || arg == "-r")
		{
			config.enableRender = true;
		}
		else if (arg == "--help" || arg == "-h")
		{
			config.showHelp = true;
		}
		else if ((arg == "--input" || arg == "-i") && i + 1 < argc)
		{
			config.inputPath = argv[++i];
		}
		else if ((arg == "--output" || arg == "-o") && i + 1 < argc)
		{
			config.outputPath = argv[++i];
		}
		else
		{
			std::cerr << "Unknown option: " << arg << "\n";
			config.showHelp = true;
		}
	}

	if (config.showHelp)
	{
		PrintUsage(std::cout, argv[0]);
	}

	return config;
}

AnalysisResult Run(std::istream& input, std::ostream& output, const Config& config)
{
	AnalysisResult result;

	try
	{
		const auto [shapes, drawables] = ShapeParser::ParseAll(input);

		if (shapes.empty())
		{
			output << "Warning: no shapes parsed from input\n";
			return result;
		}

		output << "Parsed " << shapes.size() << " shape(s)\n\n";

		result.maxAreaShape = ShapeAnalyzer::FindMaxArea(shapes);
		result.minPerimeterShape = ShapeAnalyzer::FindMinPerimeter(shapes);

		if (result.maxAreaShape)
		{
			output << "=== Figure with MAXIMUM area ===\n";
			output << result.maxAreaShape->ToString() << "\n\n";
		}

		if (result.minPerimeterShape)
		{
			output << "=== Figure with MINIMUM perimeter ===\n";
			output << result.minPerimeterShape->ToString() << "\n";
		}

		if (config.enableRender && !drawables.empty())
		{
			output << "\nLaunching visualizer...\n";
			output.flush();
			RunRender(drawables);
		}
	}
	catch (const std::exception& ex)
	{
		result.hasErrors = true;
		result.errorMessage = ex.what();
		output << "Error: " << ex.what() << "\n";
	}

	return result;
}

void RunRender(const std::vector<std::shared_ptr<ICanvasDrawable>>& shapes, uint32_t windowWidth, uint32_t windowHeight)
{
	try
	{
		sf::ContextSettings settings;
		settings.antiAliasingLevel = DEFAULT_ANTI_ALIASING_LEVEL;

		sf::RenderWindow window(
			sf::VideoMode({ windowWidth, windowHeight }),
			std::string(DEFAULT_WINDOW_TITLE),
			sf::Style::Default);
		window.setFramerateLimit(DEFAULT_FRAMERATE);

		SFMLCanvas canvas(window);
		canvas.SetOffset(windowWidth / 2.0, windowHeight / 2.0);
		canvas.SetScale(DEFAULT_RENDER_SCALE);

		while (window.isOpen())
		{
			while (const std::optional event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.close();
				else if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
					if (kp->scancode == sf::Keyboard::Scancode::Escape)
						window.close();
			}

			canvas.Clear();

			for (const auto& shape : shapes)
			{
				// использовать shared_ptr вместо dynamic_cast
				if (const auto* drawable = dynamic_cast<const ICanvasDrawable*>(shape.get()))
				{
					try
					{
						drawable->Draw(canvas);
					}
					catch (const SFMLCanvas::RenderError& ex)
					{
						std::cerr << "Render warning: " << ex.what() << "\n";
					}
					catch (const SFMLCanvas::OutOfBoundsError& ex)
					{
						std::cerr << "Out of bounds: " << ex.what() << "\n";
					}
				}
			}
			window.display();
		}
	}
	catch (const sf::Exception& ex)
	{
		throw std::runtime_error(std::string("SFML error: ") + ex.what());
	}
}

} // namespace App
