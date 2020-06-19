#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Path
{
	uint32_t longestPath;
	uint16_t dropInHeight;
	std::string path;
};
struct DropPoint
{
	uint16_t height;
	Path path;
};

void getInput(const std::string& fileName, std::vector<std::vector<DropPoint>>& skiMap)
{
	std::ifstream file(fileName);

	uint16_t row, column;

	std::string line;
	std::getline(file, line);

	std::istringstream iss(line);
	iss >> row >> column;

	std::vector<DropPoint> rowContainer;
	uint16_t temp;
	for (auto i = 0; i < row; ++i)
	{
		std::getline(file, line);
		iss.str(line);
		iss.clear();
		for (auto j = 0; j < column; ++j)
		{
			if (!(iss >> temp)) std::cerr << "Error from input encountered\n";
			rowContainer.push_back({temp, {0xFFFFFFFF, 0xFFFF, ""}});
		}
		skiMap.push_back(rowContainer);
		rowContainer.clear();
	}
}

Path getLongestPath(std::vector<std::vector<DropPoint>>& skiMap, 
				   const uint16_t row, const uint16_t column, const uint16_t& parentValue)
{
	uint16_t height = skiMap[row][column].height;
	Path currentLongest{0, 0, ""};
	Path fromHere{1, (uint16_t)(parentValue - height), std::to_string(height)};
	bool hasValid = false;

	if (row > 0 && skiMap[row - 1][column].height < height)
	{
		hasValid = true;
		if (skiMap[row - 1][column].path.longestPath != 0xFFFFFFFF) currentLongest = skiMap[row - 1][column].path;
		else currentLongest = getLongestPath(skiMap, row - 1, column, height);
	}
	if (column + 1 < skiMap[row].size() && skiMap[row][column + 1].height < height)
	{
		hasValid = true;
		Path pathEast;
		if (skiMap[row][column + 1].path.longestPath != 0xFFFFFFFF) pathEast = skiMap[row][column + 1].path;
		else
		{
			pathEast = getLongestPath(skiMap, row, column + 1, height);
		}
		if (pathEast.longestPath > currentLongest.longestPath) currentLongest = pathEast;
		else if (pathEast.longestPath == currentLongest.longestPath &&
				 pathEast.dropInHeight > currentLongest.dropInHeight)
		{
			currentLongest = pathEast;
		}
	}
	if (row + 1 < skiMap.size() && skiMap[row + 1][column].height < height)
	{
		hasValid = true;
		Path pathSouth;
		if (skiMap[row + 1][column].path.longestPath != 0xFFFFFFFF) pathSouth = skiMap[row + 1][column].path;
		else
		{
			Path pathSouth = getLongestPath(skiMap, row + 1, column, height);
			if (pathSouth.longestPath > currentLongest.longestPath) currentLongest = pathSouth;
			else if (pathSouth.longestPath == currentLongest.longestPath &&
					 pathSouth.dropInHeight > currentLongest.dropInHeight)
			{
				currentLongest = pathSouth;
			}
		}
	}
	if (column > 0 && skiMap[row][column - 1].height < height)
	{
		hasValid = true;
		Path pathWest;
		if (skiMap[row][column - 1].path.longestPath != 0xFFFFFFFF) pathWest = skiMap[row][column - 1].path;
		else
		{
			Path pathWest = getLongestPath(skiMap, row, column - 1, height);
			if (pathWest.longestPath > currentLongest.longestPath) currentLongest = pathWest;
			else if (pathWest.longestPath == currentLongest.longestPath &&
					 pathWest.dropInHeight > currentLongest.dropInHeight)
			{
				currentLongest = pathWest;
			}
		}
	}
	if (hasValid)
	{
		fromHere.longestPath += currentLongest.longestPath;
		fromHere.dropInHeight += currentLongest.dropInHeight;
		fromHere.path += " " + currentLongest.path;
	}
	return fromHere;
}

int main()
{
	std::vector<std::vector<DropPoint>> skiMap;
	getInput("map.txt", skiMap);

	Path currentLongest;

	for (auto i = 0; i < skiMap.size(); ++i)
	{
		for (auto j = 0; j < skiMap[i].size(); ++j)
		{
			if (skiMap[i][j].path.longestPath == 0xFFFFFFFF)
			{
				Path currentPath = getLongestPath(skiMap, i, j, skiMap[i][j].height);
				// std::cout << currentPath.longestPath << " for " << i << " " << j << std::endl;
				if (currentPath.longestPath > currentLongest.longestPath) currentLongest = currentPath;
				else if (currentPath.longestPath == currentLongest.longestPath &&
						 currentPath.dropInHeight > currentLongest.longestPath)
				{
					currentLongest = currentPath;
				}
			}

		}
	}
	std::cout << currentLongest.path << std::endl;
	std::cout << currentLongest.longestPath << std::endl;
	std::cout << currentLongest.dropInHeight << std::endl;

	// for (auto i = 0; i < skiMap.size(); ++i)
	// {
	// 	for (auto j = 0; j < skiMap[i].size(); ++j)
	// 	{
	// 		std::cout << skiMap[i][j].height << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	return 0;
}