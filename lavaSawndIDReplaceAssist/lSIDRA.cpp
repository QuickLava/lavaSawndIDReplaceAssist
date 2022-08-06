#include "lSIDRA.h"

namespace lava
{
    std::string numToHexStringWithPadding(std::size_t numIn, std::size_t paddingLength)
    {
        std::stringstream convBuff;
        convBuff << std::hex << numIn;
        std::string result = convBuff.str();
        for (int i = 0; i < result.size(); i++)
        {
            result[i] = std::toupper(result[i]);
        }
        if (result.size() < paddingLength)
        {
            result = std::string(paddingLength - result.size(), '0') + result;
        }
        return result;
    }
    std::string numToDecStringWithPadding(std::size_t numIn, std::size_t paddingLength)
    {
        std::string result = std::to_string(numIn);
        if (result.size() < paddingLength)
        {
            result = std::string(paddingLength - result.size(), '0') + result;
        }
        return result;
    }
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

	namespace brawl
	{
		bool ppexModeEnabled = 0;

		void setPPEXMode(bool enabled)
		{
			ppexModeEnabled = enabled;
		}
		bool getPPEXMode()
		{
			return ppexModeEnabled;
		}

        bool isWithinRange(unsigned int valueIn, unsigned int lowerBound, unsigned int higherBound)
        {
            return (lowerBound <= valueIn && valueIn <= higherBound);
        }
        bool isValidBankID(unsigned int bankIDIn)
        {
            return (isWithinRange(bankIDIn, lowerBankIDBound, higherBankIDBound));
        }

        std::vector<unsigned long> getIDList(unsigned long exSoundbankID)
        {
            std::vector<unsigned long> result;

            if (isValidBankID(exSoundbankID))
            {
                int initialIndex = 0xA5 * (exSoundbankID - lava::brawl::lowerBankIDBound);
                for (int _infoIndex = vcInitialInfoIndex; _infoIndex < vcSectionLength + vcInitialInfoIndex; _infoIndex++)
                {
                    result.push_back((_infoIndex + vcInfoIndexOffset + initialIndex));
                }
                for (int _infoIndex = seInitialInfoIndex; _infoIndex < seSectionLength + seInitialInfoIndex; _infoIndex++)
                {
                    result.push_back((_infoIndex + seInfoIndexOffset + initialIndex));
                }
            }

            return result;
        }
        std::vector<unsigned long> getSnakeIDList()
        {
            std::vector<unsigned long> result{};

            for (int _infoIndex = vcInitialInfoIndex; _infoIndex < vcInitialInfoIndex + vcSectionLength; _infoIndex++)
            {
                result.push_back(_infoIndex);
            }
            for (int _infoIndex = seInitialInfoIndex; _infoIndex < seInitialInfoIndex + seSectionLength; _infoIndex++)
            {
                result.push_back(_infoIndex);
            }

            return result;
        }

		bool outputSoundTXT(std::ostream& output, const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName)
		{
			bool result = 0;
			if (output.good())
			{
				for (std::size_t u = 0; u < sourceIDList.size(); u++)
				{
					output << "0x" << lava::numToHexStringWithPadding(sourceIDList[u], 0x04) << " 0x" << lava::numToHexStringWithPadding(destinationIDList[u], 0x04) << "\n";
				}
				if (output.good())
				{
					result = 1;
				}
			}
			return result;
		}
		bool outputLMPRPatch(std::ostream& output, const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName)
		{
			bool result = 0;
			if (output.good())
			{
				output << "<?xml version=\"1.0\"?>\n";
				output << "<!--This patch was produced automatically using " << lava::brawl::programDisplayName << " " << lava::brawl::version << ".-->\n";
				output << "<!--It is intended for use with lavaMontyPythonReimp (Github Page: https://github.com/QuickLava/lavaMontyPythonReimp/ ).-->\n";
				output << "<!--Note: Remember to change Death and Damage SFX IDs in PSACompressor after running this patch!-->\n";
				output << "<movesetPatch name = \"" << outputFileName << "\">\n";
				output << "\t<targets>\n";
				output << "\t\t<command name = \"Sound Effect\" signature = \"0x0A000100\" paramIndex = \"0\"/>\n";
				output << "\t\t<command name = \"Sound Effect (Transient)\" signature = \"0x0A020100\" paramIndex = \"0\"/>\n";
				output << "\t\t<command name = \"Stop Sound Effect\" signature = \"0x0A030100\" paramIndex = \"0\"/>\n";
				output << "\t\t<command name = \"Victory\" signature = \"0x0A050100\" paramIndex = \"0\"/>\n";
				output << "\t\t<command name = \"Other Sound Effect (landing)\" signature = \"0x0A090100\" paramIndex = \"0\"/>\n";
				output << "\t\t<command name = \"Other Sound Effect (impact)\" signature = \"0x0A0A0100\" paramIndex = \"0\"/>\n";
				output << "\t\t<command name = \"Stepping Sound Effect\" signature = \"0x0A010100\" paramIndex = \"0\"/>\n";
				output << "\t</targets>\n";
				output << "\t<modifications>\n";
				output << "\t</modifications>\n";
				output << "\t<directApply name = \"SFX ID Replacement\">\n";

				for (std::size_t u = 0; u < sourceIDList.size(); u++)
				{
					output << "\t\t" << lava::numToHexStringWithPadding(sourceIDList[u], 0x08) << ", " << lava::numToHexStringWithPadding(destinationIDList[u], 0x08) << "\n";
				}

				output << "\t</directApply>\n";
				output << "</movesetPatch>\n";
				result = output.good();

				if (output.good())
				{
					result = 1;
				}
			}

			return result;
		}
		bool initiatePrintingOutput(const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName)
		{
			bool result = 0;

			std::cout << "Select Patch Mode: [1: \"sound.txt\" (for P+EX Porting Tools)] [0: Cancel]\n\t";
			std::size_t patchMode = decision("10");
			switch (patchMode)
			{
				case 0:
				{
					std::string outputFilePath = lava::brawl::outputDirectory + outputFileName + lava::brawl::soundTXTSuffix;
					std::cout << "Writing output to \"" << outputFilePath << "\"... ";
					std::ofstream output(outputFilePath);
					if (output.is_open())
					{
						result = outputSoundTXT(output, sourceIDList, destinationIDList, outputFileName);
						if (result)
						{
							std::cout << "Success!\n\n";
						}
						else
						{
							std::cerr << "Failure! Output stream became corrupt!\n\n";
						}
					}
					else
					{
						std::cerr << "Failure! Unable to open output stream, couldn't write output!\n\n";
					}
					break;
				}
				/*case 1:
				{
					std::string outputFilePath = lava::brawl::outputDirectory + outputFileName + lava::brawl::lMPRSuffix;
					std::cout << "Writing output to \"" << outputFilePath << "\"... ";
					std::ofstream output(outputFilePath);
					result = outputLMPRPatch(output, sourceIDList, destinationIDList, outputFileName);
					break;
				}*/
				case 1:
				{
					std::cout << "Skipping Output.\n";
					break;
				}
				default:
				{
					std::cout << "Invalid patchMode (" << patchMode << ") specified!\n";
					break;
				}
			}
			return result;
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
				if (ppexModeEnabled)
				{
					result += 7;
				}
				idGood = lava::brawl::isValidBankID(result);
				if (!idGood)
				{
					std::cout << "\tBad ID provided, please enter another.\n";
					std::cout << "\tEnsure you enter a valid EX Character Soundbank ID between " << lava::brawl::lowerBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::lowerBankIDBound, 0x04) << ") and " << lava::brawl::higherBankIDBound << " (0x" << lava::numToHexStringWithPadding(lava::brawl::higherBankIDBound, 0x04) << ").\n";
				}
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
			std::string outputFileName = "SNAKE_TO_0x";
			if (ppexModeEnabled)
			{
				outputFileName += lava::numToHexStringWithPadding(destEXID - 7, 0x03);
				outputFileName += "_PPEX";
			}
			else
			{
				outputFileName += lava::numToHexStringWithPadding(destEXID, 0x03);
			}
			initiatePrintingOutput(sourceSFXIDs, destSFXIDs, outputFileName);
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
			std::string outputFileName = "0x";
			if (ppexModeEnabled)
			{
				outputFileName += lava::numToHexStringWithPadding(sourceEXID - 7, 0x03);
			}
			else
			{
				outputFileName += lava::numToHexStringWithPadding(sourceEXID, 0x03);
			}
			outputFileName += "_TO_0x";
			if (ppexModeEnabled)
			{
				outputFileName += lava::numToHexStringWithPadding(destEXID - 7, 0x03);
				outputFileName += "_PPEX";
			}
			else
			{
				outputFileName += lava::numToHexStringWithPadding(destEXID, 0x03);
			}
			initiatePrintingOutput(sourceSFXIDs, destSFXIDs, outputFileName);
			return result;
		}
		bool EXToSnakePort()
		{
			bool result = 0;
			std::cout << "Enter the Source EX Character's Soundbank ID: \n";
			unsigned long sourceEXID = getBankIDInput();
			std::vector<unsigned long> sourceSFXIDs = lava::brawl::getIDList(sourceEXID);
			std::vector<unsigned long> destSFXIDs = lava::brawl::getSnakeIDList();
			std::string outputFileName = "0x";
			if (ppexModeEnabled)
			{
				outputFileName += lava::numToHexStringWithPadding(sourceEXID - 7, 0x03);
			}
			else
			{
				outputFileName += lava::numToHexStringWithPadding(sourceEXID, 0x03);
			}
			outputFileName += "_TO_SNAKE";
			if (ppexModeEnabled)
			{
				outputFileName += "_PPEX";
			}
			initiatePrintingOutput(sourceSFXIDs, destSFXIDs, outputFileName);
			return result;
		}

		bool hfInitiatePrintingOutput(const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFilePath, unsigned long patchMode)
		{
			bool result = 0;

			switch (patchMode)
			{
				case 0:
				{
					std::cout << "Writing output to \"" << outputFilePath << "\"... ";
					std::ofstream output(outputFilePath);
					result = outputSoundTXT(output, sourceIDList, destinationIDList, outputFilePath);
					break;
				}
				/*case 1:
				{
					std::cout << "Writing output to \"" << outputFilePath << "\"... ";
					std::ofstream output(outputFilePath);
					result = outputLMPRPatch(output, sourceIDList, destinationIDList, outputFilePath);
					break;
				}*/
				default:
				{
					std::cout << "Invalid patchMode (" << patchMode << ") specified!\n";
					break;
				}
			}
			return result;
		}
		bool hfSnakeToEXPort(unsigned long destinationID, std::string outputFilepath, unsigned long patchMode)
		{
			std::vector<unsigned long> sourceSFXIDs = lava::brawl::getSnakeIDList();
			std::vector<unsigned long> destSFXIDs = lava::brawl::getIDList(destinationID);
			return hfInitiatePrintingOutput(sourceSFXIDs, destSFXIDs, outputFilepath, patchMode);
		}
		bool hfEXToEXPort(unsigned long sourceID, unsigned long destinationID, std::string outputFilePath, unsigned long patchMode)
		{
			std::vector<unsigned long> sourceSFXIDs = lava::brawl::getIDList(sourceID);
			std::vector<unsigned long> destSFXIDs = lava::brawl::getIDList(destinationID);
			return hfInitiatePrintingOutput(sourceSFXIDs, destSFXIDs, outputFilePath, patchMode);
		}
		bool hfEXToSnakePort(unsigned long sourceID, std::string outputFilePath, unsigned long patchMode)
		{
			std::vector<unsigned long> sourceSFXIDs = lava::brawl::getIDList(sourceID);
			std::vector<unsigned long> destSFXIDs = lava::brawl::getSnakeIDList();
			return hfInitiatePrintingOutput(sourceSFXIDs, destSFXIDs, outputFilePath, patchMode);
		}
	}
}