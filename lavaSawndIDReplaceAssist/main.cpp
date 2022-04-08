#include "lSIDRA.h"

int main()
{
	std::cout << lava::brawl::programDisplayName << " " << lava::brawl::version << "\n";
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

	return 0;
}