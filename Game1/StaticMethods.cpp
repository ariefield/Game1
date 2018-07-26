#include "StaticMethods.h"


StaticMethods::StaticMethods()
{
}


StaticMethods::~StaticMethods()
{

}

void StaticMethods::fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}

std::string StaticMethods::readFile(const char * filePath)
{
	std::string codeAsString;
	std::ifstream file;
	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		file.open(filePath);
		std::stringstream stream;
		// read file's buffer contents into stream
		stream << file.rdbuf();
		// close file handlers
		file.close();
		// convert stream into string
		codeAsString = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return codeAsString;
}

std::string StaticMethods::readFileDeprecated(const char * filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

