#include "Faction_Struct.h"


void Check_Work_Permission(FACTION* faction_ptr)
{
	while (!Exiting)
	{
		if (Permission_to_work == true)
		{	continue; 	}

		else
		{
			faction_ptr->TAKE_A_BREAK(SECONDS_PER_SECOND*500);
		}
	}
}


void FACTION_START(FACTION* faction_ptr)
{
	bool show_pause_message = true;
	std::thread


    while (faction_ptr->Current_Status	!=  DEAD)      
    {
		if (Permission_to_work == true)
		{
			show_pause_message = true;

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


		
			NAP(50);
		}//end if

		else
		{
			if (show_pause_message)
			{
				HIGH_LOG
					std::cout << faction_ptr->Fac_Name << " is paused\n";
				END_LOG

				show_pause_message = false;
			}
		
			NAP(250);
		}
    }//end while
}





