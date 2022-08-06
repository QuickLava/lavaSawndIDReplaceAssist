#include "lSIDRA.h"

const std::string nullArgumentString = "-";
bool isNullArg(const char* argIn)
{
	return (argIn != nullptr) ? (strcmp(argIn, nullArgumentString.c_str()) == 0) : 0;
}

int main(int argc, char** argv)
{
	std::cout << lava::brawl::programDisplayName << " " << lava::brawl::version << "\n";

	// If no arguments are provided
	if (argc < 2)
	{
		std::cout << "Important Note: When using with P+EX, remember to add 7 to your characters' Soundbank IDs (assigned in FighterXX.dat).\n\n";
		bool repeatProgram = 1;
		while (repeatProgram)
		{
			std::cout << "Select Action: [1: Snake->EX] [2: EX->EX] [3: EX->Snake] [0: Exit]\n";
			bool advance = 0;
			std::size_t mode = lava::decision("1230");
			switch (mode)
			{
				case 0:
				{
					lava::brawl::snakeToEXPort();
					advance = 1;
					break;
				}
				case 1:
				{
					lava::brawl::EXToEXPort();
					advance = 1;
					break;
				}
				case 2:
				{
					lava::brawl::EXToSnakePort();
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
	}
	// Otherwise, run headless version.
	else
	{
		try
		{
			int patchMode = 0;
			// args: progloc snaketoex destID outputfile(opt) patchMode(opt)
			if (strcmp("snaketoex", argv[1]) == 0 && argc >= 3)
			{
				std::cout << "Converting Snake IDs to EX IDs:\n";
				int destinationID = lava::stringToNum(argv[2], 0, INT_MAX);
				if (lava::brawl::isValidBankID(destinationID))
				{
					std::string outputFilePath = "SNAKE_TO_0x" + lava::numToHexStringWithPadding(destinationID, 0x03) + ".txt";
					std::cout << "Using Destination ID " 
						<< lava::numToDecStringWithPadding(destinationID, 3) << " / 0x" << lava::numToHexStringWithPadding(destinationID, 3) 
						<< "...\n";
					if (argc >= 4 && !isNullArg(argv[3]))
					{
						outputFilePath = argv[3];
					}
					/*
					if (argc >= 5 && !isNullArg(argv[4]))
					{
						patchMode = std::stoi(argv[4]);
					}*/
					if (lava::brawl::hfSnakeToEXPort(destinationID, outputFilePath, patchMode))
					{
						std::cout << "Success!\n";
					}
					else
					{
						std::cerr << "Failure!\n";
					}
				}
				else
				{
					std::cerr << "[ERROR] Unable to complete Snake-To-EX port: Invalid Destination SFX ID provided!\n";
					std::cout << "\tEnsure you enter a valid EX Character SFX ID between "
						<< lava::brawl::lowerBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, 0x04) << ") and "
						<< lava::brawl::higherBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, 0x04) << ").\n";
				}
			}
			// args: progloc extoex sourceID destID outputfile(opt) patchMode(opt)
			else if (strcmp("extoex", argv[1]) == 0 && argc >= 4)
			{
				std::cout << "Converting EX IDs to EX IDs:\n";
				int sourceID = lava::stringToNum(argv[2], 0, INT_MAX);
				if (lava::brawl::isValidBankID(sourceID))
				{
					int destinationID = lava::stringToNum(argv[3], 0, INT_MAX);
					if (lava::brawl::isValidBankID(destinationID))
					{
						std::string outputFilePath = "0x" + lava::numToHexStringWithPadding(sourceID, 0x03) + "_TO_0x" + lava::numToHexStringWithPadding(destinationID, 0x03);
						std::cout << "Using Source ID " 
							<< lava::numToDecStringWithPadding(sourceID, 3) << " / 0x" << lava::numToHexStringWithPadding(sourceID, 3) 
							<< " and Destination ID " 
							<< lava::numToDecStringWithPadding(destinationID, 3) << " / 0x" << lava::numToHexStringWithPadding(destinationID, 3) 
							<< "...\n";
						if (argc >= 5 && !isNullArg(argv[4]))
						{
							outputFilePath = argv[4];
						}
						/*
						if (argc >= 6 && !isNullArg(argv[5]))
						{
							patchMode = std::stoi(argv[5]);
						}*/
						if (lava::brawl::hfEXToEXPort(sourceID, destinationID, outputFilePath, patchMode))
						{
							std::cout << "Success!\n";
						}
						else
						{
							std::cerr << "Failure!\n";
						}
					}
					else
					{
						std::cerr << "[ERROR] Unable to complete EX-To-EX port: Invalid Destination SFX ID provided!\n";
						std::cout << "\tEnsure you enter a valid EX Character SFX ID between " 
							<< lava::brawl::lowerBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, 0x04) << ") and " 
							<< lava::brawl::higherBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, 0x04) << ").\n";
					}
				}
				else
				{
					std::cerr << "[ERROR] Unable to complete EX-To-EX port: Invalid Source SFX ID provided!\n";
					std::cout << "\tEnsure you enter a valid EX Character SFX ID between "
						<< lava::brawl::lowerBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, 0x04) << ") and "
						<< lava::brawl::higherBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, 0x04) << ").\n";
				}
			}
			// args: progloc extosnake sourceID outputfile(opt) patchMode(opt)
			else if (strcmp("extosnake", argv[1]) == 0 && argc >= 3)
			{
				std::cout << "Converting EX IDs to Snake IDs:\n";
				int sourceID = lava::stringToNum(argv[2], 0, INT_MAX);
				if (lava::brawl::isValidBankID(sourceID))
				{
					std::string outputFilePath = "0x" + lava::numToHexStringWithPadding(sourceID, 0x03) + "_TO_SNAKE";
					std::cout << "Using Source ID "
						<< lava::numToDecStringWithPadding(sourceID, 3) << " / 0x" << lava::numToHexStringWithPadding(sourceID, 3)
						<< "...\n";
					if (argc >= 4 && !isNullArg(argv[3]))
					{
						outputFilePath = argv[3];
					}
					/*
					if (argc >= 5 && !isNullArg(argv[4]))
					{
						patchMode = std::stoi(argv[4]);
					}*/
					if (lava::brawl::hfEXToSnakePort(sourceID, outputFilePath, patchMode))
					{
						std::cout << "Success!\n";
					}
					else
					{
						std::cerr << "Failure!\n";
					}
				}
				else
				{
					std::cerr << "[ERROR] Unable to complete EX-To-Snake port: Invalid Destination SFX ID provided!\n";
					std::cout << "\tEnsure you enter a valid EX Character SFX ID between "
						<< lava::brawl::lowerBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, 0x04) << ") and "
						<< lava::brawl::higherBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, 0x04) << ").\n";
				}
			}
			else
			{
				std::cout << "Invalid operation argument set supplied:\n";
				for (unsigned long i = 0; i < argc; i++)
				{
					std::cout << "\tArgv[" << i << "]: " << argv[i] << "\n";
				}
				std::cout << "Please provide one of the following sets of arguments!\n";
				std::cout << "To create a patch converting Snake SFX IDs to EX SFX IDs:\n";
				std::cout << "\snaketoex {DESTINATION_EX_SAWND_ID} {OUTPUT_PATH, optional}\n";
				std::cout << "To create a patch converting EX SFX IDs to EX SFX IDs:\n";
				std::cout << "\snaketoex {SOURCE_EX_SAWND_ID} {DESTINATION_EX_SAWND_ID} {OUTPUT_PATH, optional}\n";
				std::cout << "To create a patch converting EX SFX IDs to SNAKE SFX IDs:\n";
				std::cout << "\snaketoex {SOURCE_EX_SAWND_ID} {OUTPUT_PATH, optional}\n";
			}
		}
		catch (std::exception e)
		{
			std::cerr << "EXCEPTION: ";
			std::cerr << e.what();
		}
	}
	return 0;
}