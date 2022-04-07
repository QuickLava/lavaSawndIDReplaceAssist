#include "lSIDRA.h"


int stringToNum(const std::string& stringIn, bool allowNeg, int defaultVal)
{
	int result = defaultVal;
	std::string manipStr = stringIn;
	int base = ((manipStr.find("0x") == 0) || (manipStr.find("0X") == 0)) ? 16 : 10;
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

std::size_t decision(const std::string optionsIn)
{
	std::size_t result = std::string::npos;
	char keyIn = ' ';
	while (result == std::string::npos)
	{
		keyIn = _getch();
		result = optionsIn.find(keyIn);
	}
	return result;
}
bool yesNoDecision(char yesKey, char noKey)
{
	char keyIn = ' ';
	yesKey = std::tolower(yesKey);
	noKey = std::tolower(noKey);
	while (keyIn != yesKey && keyIn != noKey)
	{
		keyIn = _getch();
		keyIn = std::tolower(keyIn);
	}
	return (keyIn == yesKey);
}

unsigned long getBankIDInput()
{
	unsigned long result = ULONG_MAX;
	bool idGood = 0;
	std::string input = "";
	while (!idGood)
	{
		std::cout << "\t";
		input = "";
		std::cin >> input;
		result = stringToNum(input, 0, ULONG_MAX);
		idGood = lava::brawl::isValidBankID(result);
		if (!idGood)
		{
			std::cout << "\tBad ID provided, please enter another.\n";
			std::cout << "\tEnsure you enter a valid EX Character SFX ID between " << lava::brawl::lowerBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, 0x04) << ") and " << lava::brawl::higherBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, 0x04) << ").\n";
		}
	}
	return result;
}

bool doSFXIDPrint(const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName)
{
	bool result = 0;
	std::ofstream output(outputFileName, std::ios_base::out);
	std::cout << "\tWriting output to \"" << outputFileName << "\"... ";
	if (output.is_open())
	{
		for (std::size_t u = 0; u < sourceIDList.size(); u++)
		{
			output << "0x" << lava::numToHexStringWithPadding(sourceIDList[u], lava::brawl::IDPaddingLength) << " 0x" << lava::numToHexStringWithPadding(destinationIDList[u], lava::brawl::IDPaddingLength) << "\n";
		}
		if (output.good())
		{
			result = 1;
			std::cout << "Success!\n";
		}
		else
		{
			std::cout << "Failure! Output stream became corrupted!\n";
		}
	}
	else
	{
		std::cout << "Failure! Unable to open file for output!\n";
	}
	return result;
}

bool snakeToEXPort()
{
	bool result = 0;
	std::cout << "Enter the Destination EX Character's Soundbank ID: \n";
	unsigned long destEXID = getBankIDInput();
	std::vector<unsigned long> sourceSFXIDs = lava::brawl::getSnakeIDList();
	std::vector<unsigned long> destSFXIDs = lava::brawl::getIDList(destEXID);
	std::string outputFileName = lava::brawl::outputDirectory + "SNAKE_TO_0x" + lava::numToHexStringWithPadding(destEXID, 0x03) + "_sound.txt";
	doSFXIDPrint(sourceSFXIDs, destSFXIDs, outputFileName);
	return result;
}
bool EXToEXPort()
{
	bool result = 0;
	std::cout << "Enter the Source EX Character's Soundbank ID: \n";
	unsigned long sourceEXID = getBankIDInput();
	std::cout << "Enter the Destination EX Character's Soundbank ID: \n";
	unsigned long destEXID = getBankIDInput();
	std::vector<unsigned long> sourceSFXIDs = lava::brawl::getIDList(sourceEXID);
	std::vector<unsigned long> destSFXIDs = lava::brawl::getIDList(destEXID);
	std::string outputFileName = lava::brawl::outputDirectory + "0x" + lava::numToHexStringWithPadding(sourceEXID, 0x03) + "_TO_0x" + lava::numToHexStringWithPadding(destEXID, 0x03) + "_sound.txt";
	doSFXIDPrint(sourceSFXIDs, destSFXIDs, outputFileName);
	return result;
}
bool EXToSnakePort()
{
	bool result = 0;
	std::cout << "Enter the Source EX Character's Soundbank ID: \n";
	unsigned long sourceEXID = getBankIDInput();
	std::vector<unsigned long> sourceSFXIDs = lava::brawl::getIDList(sourceEXID);
	std::vector<unsigned long> destSFXIDs = lava::brawl::getSnakeIDList();
	std::string outputFileName = lava::brawl::outputDirectory + "0x" + lava::numToHexStringWithPadding(sourceEXID, 0x03) + "_TO_SNAKE_sound.txt";
	doSFXIDPrint(sourceSFXIDs, destSFXIDs, outputFileName);
	return result;
}

int main()
{
	std::cout << "lavaSFXIDReplaceAssist\n\n";
	bool repeatProgram = 1;
	while (repeatProgram)
	{
		std::cout << "Select Action: [1: Snake->EX] [2: EX->EX] [3: EX->Snake] [0: Exit]\n";
		bool advance = 0;
		std::size_t mode = decision("1230");
		switch (mode)
		{
			case 0:
			{
				snakeToEXPort();
				advance = 1;
				break;
			}
			case 1:
			{
				EXToEXPort();
				advance = 1;
				break;
			}
			case 2:
			{
				EXToSnakePort();
				advance = 1;
				break;
			}
			case 3:
			{
				advance = 1;
				repeatProgram = 0;
				break;
			}
			default:
			{
				advance = 0;
				break;
			}
		}
	}

	return 0;
}