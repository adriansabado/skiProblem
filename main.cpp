#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void getInput(const std::string& fileName, std::vector<std::vector<uint16_t>>& skiMap)
{
	std::ifstream file(fileName);

	uint16_t row, column;

	std::string line;
	std::getline(file, line);
	std::istringstream iss(line);
	iss >> row >> column;

	std::vector<uint16_t> rowContainer;
	uint16_t temp;
	for (auto i = 0; i < row; ++i)
	{
		std::getline(file, line);
		iss.str(line);
		iss.clear();
		for (auto j = 0; j < column; ++j)
		{
			if (!(iss >> temp)) std::cerr << "Error from input encountered\n";
			rowContainer.push_back(temp);
		}
		skiMap.push_back(rowContainer);
		rowContainer.clear();
	}
}

int main()
{
	std::vector<std::vector<uint16_t>> skiMap;
	getInput("map.txt", skiMap);

	for (auto i = 0; i < skiMap.size(); ++i)
	{
		for (auto j = 0; j < skiMap[0].size(); ++j)
		{
			std::cout << skiMap[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}