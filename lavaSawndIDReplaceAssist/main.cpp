#include "lSIDRA.h"


int stringToNum(const std::string& stringIn, bool allowNeg, int defaultVal)
{
	int result = defaultVal;
	std::string manipStr = stringIn;
	int base = (manipStr.find("0x") == 0) ? 16 : 10;
	char* res = nullptr;
	result = std::strtoul(manipStr.c_str(), &res, base);
	if (res != (manipStr.c_str() + manipStr.size()))
	{
		result = defaultVal;
	}
	if (result < 0 && !allowNeg)
	{
		result = defaultVal;
	}
	return result;
}

std::pair<std::size_t, std::size_t> parseIDPair(std::string pairStringIn)
{
	std::pair<std::size_t, std::size_t> result = { SIZE_MAX, SIZE_MAX };

	// Removes any space characters
	std::string manipStr = "";
	for (std::size_t i = 0; i < pairStringIn.size(); i++)
	{
		if (!std::isspace(pairStringIn[i]))
		{
			manipStr += pairStringIn[i];
		}
	}

	std::size_t delimLoc = manipStr.find(':');
	if (delimLoc != std::string::npos && delimLoc > 0 && delimLoc < (manipStr.size() - 1))
	{
		std::pair<std::size_t, std::size_t> newEntry = { SIZE_MAX, SIZE_MAX };
		newEntry.first = stringToNum(manipStr.substr(0, delimLoc), 1, SIZE_MAX);
		newEntry.second = stringToNum(manipStr.substr(delimLoc + 1), 1, SIZE_MAX);
		if (newEntry.first != SIZE_MAX)
		{
			if (newEntry.second != SIZE_MAX)
			{
				result = newEntry;
			}
		}
	}

	return result;
}
bool validateNewHatEntry(const std::pair<std::string, std::pair<std::size_t, std::size_t>>& pairIn, std::ostream& output)
{
	bool result = 0;
	std::string errorMessage = "";
	std::string rangeMessage = "Valid range is 0x" +
		lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, lava::brawl::IDPaddingLength) + " through 0x" +
		lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, lava::brawl::IDPaddingLength) + ".";

	if (lava::brawl::isValidBankID(pairIn.second.first))
	{
		if (lava::brawl::isValidBankID(pairIn.second.second))
		{
			result = 1;
			errorMessage = "";
		}
		else
		{
			errorMessage = "Specified Target Bank ID is outside of the valid range." + rangeMessage;
		}
	}
	else
	{
		errorMessage = "Specified Source Bank ID is outside of the valid range." + rangeMessage;
	}
	if (result == 0)
	{
		output << "[ERROR] \"" << pairIn.first << "\" (Source Bank ID 0x" <<
			lava::numToHexStringWithPadding(pairIn.second.first, lava::brawl::IDPaddingLength) << ", Target Bank ID 0x" <<
			lava::numToHexStringWithPadding(pairIn.second.second, lava::brawl::IDPaddingLength) << ")\n";
		output << "\t" << errorMessage;
	}
	return result;
}
std::vector<std::pair<std::string, std::pair<std::size_t, std::size_t>>> parseInput(std::string inputFilePath, std::ostream& output)
{
	std::vector<std::pair<std::string, std::pair<std::size_t, std::size_t>>> result{};

	std::ifstream banksIn(inputFilePath, std::ios_base::in);
	if (banksIn.is_open())
	{
		std::string currentLine = "";
		std::string manipStr = "";
		while (std::getline(banksIn, currentLine))
		{
			// Disregard the current line if it's empty, or is marked as a comment
			if (!currentLine.empty() && currentLine[0] != '#' && currentLine[0] != '/')
			{
				manipStr = "";
				bool inQuote = 0;
				bool doEscapeChar = 0;
				for (std::size_t i = 0; i < currentLine.size(); i++)
				{
					if (currentLine[i] == '\"' && !doEscapeChar)
					{
						inQuote = !inQuote;
					}
					else if (currentLine[i] == '\\')
					{
						doEscapeChar = 1;
					}
					else if (inQuote || !std::isspace(currentLine[i]))
					{
						doEscapeChar = 0;
						manipStr += currentLine[i];
					}
				}

				std::pair<std::string, std::pair<std::size_t, std::size_t>> newEntry = { "NEW_UNRECOGNIZED", {SIZE_MAX, SIZE_MAX} };

				std::size_t nameIDDelimLoc = manipStr.find('=');
				if (nameIDDelimLoc != std::string::npos && nameIDDelimLoc < (manipStr.size() - 1))
				{
					newEntry.first = manipStr.substr(0, nameIDDelimLoc);
				}
				manipStr = manipStr.substr(nameIDDelimLoc + 1, std::string::npos);
				newEntry.second = parseIDPair(manipStr);
				if (validateNewHatEntry(newEntry, output))
				{
					output << "[LOADED] \"" << newEntry.first << "\" (Source Bank ID 0x" <<
						lava::numToHexStringWithPadding(newEntry.second.first, lava::brawl::IDPaddingLength) << ", Target Bank ID 0x" <<
						lava::numToHexStringWithPadding(newEntry.second.second, lava::brawl::IDPaddingLength) << ")\n";
					result.push_back(newEntry);
				}
			}
		}
	}

	return result;
}


int main()
{
	std::ofstream* output = &lava::brawl::outStream;
	output->open(lava::brawl::outputFilename, std::ios_base::out);

	std::vector<std::pair<std::string, std::pair<unsigned int, unsigned int>>> toProcess = parseInput(lava::brawl::inputFilename, std::cout);

	for (std::size_t i = 0; i < toProcess.size(); i++)
	{
		auto currEntry = &toProcess[i];
		*output << "# " << currEntry->first << " (SFX Bank " << currEntry->second.first << " -> Bank " << currEntry->second.second << ")\n";
		*output << "# SE Section\n";
		lava::brawl::outputSEMatchData(currEntry->second.first + 7, currEntry->second.second + 7);
		*output << "\n# VC Section\n";
		lava::brawl::outputVCMatchData(currEntry->second.first + 7, currEntry->second.second + 7);
		*output << "\n";
	}


	return 0;
}