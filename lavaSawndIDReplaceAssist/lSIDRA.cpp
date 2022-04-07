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

	namespace brawl
	{
        extern std::ofstream outStream = std::ofstream();
        extern const std::string inputFilename = "bankMapping.txt";
        extern const std::string outputFilename = "sound.txt";

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
                int a5Mult = exSoundbankID - lava::brawl::lowerBankIDBound;
                int initialOffset = seInitialInfoIndex;
                for (int _infoIndex = 0; _infoIndex < seSectionLength; _infoIndex++)
                {
                    result.push_back(((_infoIndex + initialOffset) + seInfoIndexOffset + (0xA5 * a5Mult)));
                }
                for (int _infoIndex = 0; _infoIndex < vcSectionLength; _infoIndex++)
                {
                    result.push_back(((_infoIndex + initialOffset) + vcInfoIndexOffset + (0xA5 * a5Mult)));
                }
            }

            return result;
        }

        bool outputSEMatchData(unsigned int baseSoundbank, unsigned int targetSoundbank)
        {
            bool result = 0;

            if (isValidBankID(baseSoundbank) && isValidBankID(targetSoundbank))
            {
                result = 1;

                int a5Mult1 = baseSoundbank - lava::brawl::lowerBankIDBound;
                int a5Mult2 = targetSoundbank - lava::brawl::lowerBankIDBound;
                int initialOffset = seInitialInfoIndex;

                for (int _infoIndex = 0; _infoIndex < seSectionLength; _infoIndex++)
                {
                    std::cout << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + seInfoIndexOffset + (0xA5 * a5Mult1)), IDPaddingLength) << " ";
                    std::cout << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + seInfoIndexOffset + (0xA5 * a5Mult2)), IDPaddingLength) << " \n";
                    outStream << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + seInfoIndexOffset + (0xA5 * a5Mult1)), IDPaddingLength) << " ";
                    outStream << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + seInfoIndexOffset + (0xA5 * a5Mult2)), IDPaddingLength) << " \n";
                }
            }

            return result;
        }
        bool outputVCMatchData(unsigned int baseSoundbank, unsigned int targetSoundbank)
        {
            bool result = 0;

            if (isValidBankID(baseSoundbank) && isValidBankID(targetSoundbank))
            {
                result = 1;

                int a5Mult1 = baseSoundbank - lava::brawl::lowerBankIDBound;
                int a5Mult2 = targetSoundbank - lava::brawl::lowerBankIDBound;
                int initialOffset = vcInitialInfoIndex;

                for (int _infoIndex = 0; _infoIndex < vcSectionLength; _infoIndex++)
                {
                    std::cout << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + vcInfoIndexOffset + (0xA5 * a5Mult1)), IDPaddingLength) << " ";
                    std::cout << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + vcInfoIndexOffset + (0xA5 * a5Mult2)), IDPaddingLength) << " \n";
                    outStream << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + vcInfoIndexOffset + (0xA5 * a5Mult1)), IDPaddingLength) << " ";
                    outStream << "0x" << lava::numToHexStringWithPadding(((_infoIndex + initialOffset) + vcInfoIndexOffset + (0xA5 * a5Mult2)), IDPaddingLength) << " \n";
                }
            }

            return result;
        }
        bool outputMatchData(unsigned int baseSoundbank, unsigned int targetSoundbank)
        {
            bool result = outputSEMatchData(baseSoundbank, targetSoundbank);
            std::cout << "\n";
            outStream << "\n";
            result &= outputVCMatchData(baseSoundbank, targetSoundbank);

            return result;
        }
	}
}