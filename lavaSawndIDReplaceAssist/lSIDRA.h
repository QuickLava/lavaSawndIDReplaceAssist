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

	namespace brawl
	{
        const std::string outputDirectory = "./Mappings/";

        constexpr unsigned int IDPaddingLength = 0x04;

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
        bool outputSoundMapping(std::ofstream& output);
        //bool outputLMPRPatch(std::ofstream& output);

        /*bool outputSEMatchData(unsigned int baseSoundbank, unsigned int targetSoundbank);
        bool outputVCMatchData(unsigned int baseSoundbank, unsigned int targetSoundbank);
        bool outputMatchData(unsigned int baseSoundbank, unsigned int targetSoundbank);*/
    }
}

#endif