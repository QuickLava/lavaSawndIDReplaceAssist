#ifndef LAVA_SAWND_ID_REPLACE_ASSIST_H
#define LAVA_SAWND_ID_REPLACE_ASSIST_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>

namespace lava
{
    std::string numToHexStringWithPadding(std::size_t numIn, std::size_t paddingLength = 8);
    std::string numToDecStringWithPadding(std::size_t numIn, std::size_t paddingLength = 8);
    int stringToNum(const std::string& stringIn, bool allowNeg, int defaultVal);
	std::size_t decision(const std::string optionsIn);
	namespace brawl
	{
        const std::string programDisplayName = "lavaSawndIDReplaceAssist";
        const std::string version = "v1.0.0";
        const std::string outputDirectory = "./Soundbank_Mapping_Output/";
        const std::string soundTXTSuffix = "_sound.txt";
        const std::string lMPRSuffix = "_lMPR.xml";

        constexpr unsigned int lowerBankIDBound = 331;
        constexpr unsigned int higherBankIDBound = 587;
        constexpr unsigned int seSectionLength = 0x76;
        constexpr unsigned int seInitialInfoIndex = 0x18D8;
        constexpr unsigned int seInfoIndexOffset = 0x2757;
        constexpr unsigned int vcSectionLength = 0x2F;
        constexpr unsigned int vcInitialInfoIndex = 0xA34;
        constexpr unsigned int vcInfoIndexOffset = 0x35CC;

        bool isWithinRange(unsigned int valueIn, unsigned int lowerBound, unsigned int higherBound);
        bool isValidBankID(unsigned int bankIDIn);

        std::vector<unsigned long> getIDList(unsigned long exSoundbankID);
        std::vector<unsigned long> getSnakeIDList();

		bool outputSoundTXT(std::ostream& output, const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName);
		bool outputLMPRPatch(std::ostream& output, const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName);
		bool initiatePrintingOutput(const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFileName);

		unsigned long getBankIDInput();
		bool snakeToEXPort();
        bool EXToEXPort();
        bool EXToSnakePort();
        
        // Hands-free Functions
        // Equivalents to the above functions which don't use prompts.
        bool hfInitiatePrintingOutput(const std::vector<unsigned long>& sourceIDList, const std::vector<unsigned long>& destinationIDList, std::string outputFilePath, unsigned long patchMode);
        bool hfSnakeToEXPort(unsigned long destinationID, std::string outputFilepath, unsigned long patchMode);
        bool hfEXToEXPort(unsigned long sourceID, unsigned long destinationID, std::string outputFilePath, unsigned long patchMode);
        bool hfEXToSnakePort(unsigned long sourceID, std::string outputFilePath, unsigned long patchMode);
    }
}

#endif