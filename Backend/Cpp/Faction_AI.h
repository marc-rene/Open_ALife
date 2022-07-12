#include "Faction_Struct.h"





void FACTION_START(FACTION* faction_ptr)
{
    while (faction_ptr->Current_Status	!=  DEAD)      
    {
		HIGH_LOG
			std::cout << faction_ptr->Fac_Name << " is working on thread " << std::this_thread::get_id() << "\n";
		END_LOG
		
		//	DEBUG... Please Change
		int decision = rand() % 4;
		switch (decision)
		{
			case 0:
				faction_ptr->Move(North);
				break;
			case 1:
				faction_ptr->Move(East);
				break;
			case 2:
				faction_ptr->Move(South);
				break;
			case 3:
				faction_ptr->Move(West);
				break;
			
			default:
				printf("\nfuck\n");
				break;
		}//end switch
		
		SLEEP(SECONDS_PER_SECOND*2);
	}//end while		
}





