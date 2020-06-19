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
				   const uint16_t row, const uint16_t column, const uint16_t& parentValue);
void evaluateOnePath(const uint16_t row, const uint16_t column, Path& currentLongest,
	                 std::vector<std::vector<DropPoint>>& skiMap, const uint16_t height);

void evaluateOnePath(const uint16_t row, const uint16_t column, Path& currentLongest,
	                 std::vector<std::vector<DropPoint>>& skiMap, const uint16_t height)
{
	Path path;
	if (skiMap[row][column].path.longestPath != 0xFFFFFFFF) path = skiMap[row][column].path;
	else
	{
		path = getLongestPath(skiMap, row, column, height);
	}
	if (path.longestPath > currentLongest.longestPath) currentLongest = path;
	else if (path.longestPath == currentLongest.longestPath &&
			 path.dropInHeight > currentLongest.dropInHeight)
	{
		currentLongest = path;
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
		evaluateOnePath(row - 1, column, currentLongest, skiMap, height);
	}
	if (column + 1 < skiMap[row].size() && skiMap[row][column + 1].height < height)
	{
		hasValid = true;
		evaluateOnePath(row, column + 1, currentLongest, skiMap, height);
	}
	if (row + 1 < skiMap.size() && skiMap[row + 1][column].height < height)
	{
		hasValid = true;
		evaluateOnePath(row + 1, column, currentLongest, skiMap, height);
	}
	if (column > 0 && skiMap[row][column - 1].height < height)
	{
		hasValid = true;
		evaluateOnePath(row, column - 1, currentLongest, skiMap, height);
	}
	if (hasValid)
	{
		fromHere.longestPath += currentLongest.longestPath;
		fromHere.dropInHeight += currentLongest.dropInHeight;
		fromHere.path += " " + currentLongest.path;
	}
	skiMap[row][column].path = fromHere;
	return fromHere;
}

int main()
{
	std::vector<std::vector<DropPoint>> skiMap;
	getInput("map.txt", skiMap);

	Path currentLongest{0, 0, ""};

	for (auto i = 0; i < skiMap.size(); ++i)
	{
		for (auto j = 0; j < skiMap[i].size(); ++j)
		{
			Path currentPath;
			if (skiMap[i][j].path.longestPath == 0xFFFFFFFF) currentPath = getLongestPath(skiMap, i, j, skiMap[i][j].height);
			else currentPath = skiMap[i][j].path;

			if (currentPath.longestPath > currentLongest.longestPath) currentLongest = currentPath;
			else if (currentPath.longestPath == currentLongest.longestPath &&
					 currentPath.dropInHeight > currentLongest.longestPath)
			{
				currentLongest = currentPath;
			}

		}
	}
	std::cout << currentLongest.path << std::endl;
	std::cout << currentLongest.longestPath << std::endl;
	std::cout << currentLongest.dropInHeight << std::endl;
	
	return 0;
}