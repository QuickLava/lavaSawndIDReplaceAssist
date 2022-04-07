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
	}
}