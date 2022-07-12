#include "Faction_AI.h"

FACTION* ID_to_Faction(uMint ID_to_find)
{
	return &ALL_FACTIONS.at(ID_to_find);
}



void Test_mints()
{
	printf("Faction amount: %d\n", 		HOW_MANY_FACTIONS);
	printf("Faction Log level: %d\n", 	LOG_LEVEL);
	printf("World Size is %d wide, %d high\n", 	WORLD_WIDTH, WORLD_HEIGHT);
	printf("Performance mode is %d\n", HIGH_PERFORMANCE_MODE);
	printf("Speed divider is %d\n", SECONDS_PER_SECOND);
}


/*
void Check_Work_Permission_For_Faction()
{
	while (!Exiting)
	{
		if (Permission_to_work == true)
		{	continue; 	}

		else
		{	
			for (uMint i = 0; i < HOW_MANY_FACTIONS; i++)
			{
				ALL_FACTIONS.at(i).TAKE_A_BREAK(SECONDS_PER_SECOND*1000);
			}	
		}

		SLEEP(SECONDS_PER_SECOND*2);
	}
}
*/


void Write_Map_to_Waiters_File()
{
	while (true)
	{
		Permission_to_work = false;

		std::vector<Tile>	world_copy;
		world_copy.resize(WORLD_SIZE);

		for (uS i = 0; i < WORLD_SIZE; i++)
		{
			world_copy[i].ID_of_Faction_Here 				= WORLD[i].ID_of_Faction_Here;
			world_copy[i].Military_Presense 				= WORLD[i].Military_Presense;
			world_copy[i].Base_Traversing_Difficulty 		= WORLD[i].Base_Traversing_Difficulty;
			world_copy[i].Traversing_difficulty_Modifier 	= WORLD[i].Traversing_difficulty_Modifier;
			world_copy[i].Climate 							= WORLD[i].Climate;
			
			IF_LOW_PERFORMANCE { NAP(SECONDS_PER_SECOND*100);	}
		}
		
		IF_HIGH_PERFORMANCE	{	Permission_to_work = true;	}
		IF_LOW_PERFORMANCE	{	NAP(SECONDS_PER_SECOND*50);	}

		std::ofstream world_file(WORLD_FILE_PATH);

		if (!world_file) // if we couldn't open the map.world file
		{
			bool opened = false;

			for (uMint i = 0; i < 10; i++)
			{
				SLEEP(SECONDS_PER_SECOND);
				world_file.open(WORLD_FILE_PATH);
				if (world_file)
				{
					opened = true;
					break;
				}
			}

			if (!opened)
			{
				HIGH_LOG
					printf("\n %s \n", MED_SEVERITY_ERROR);
					printf("Lots of difficulty is being had getting the map file open!\n");
				END_LOG

				
				for (uMint i = 0; i < 10; i++)
				{
					SLEEP(SECONDS_PER_SECOND);
					world_file.open(WORLD_FILE_PATH);
					if (world_file)
					{
						opened = true;
						break;
					}
				}
			}//end if

			if (!opened)
			{
				printf("\n %s \n", HIGH_SEVERITY_ERROR);
				printf("CANT WRITE TO WORLD FILE!\nEXITING");
				SLEEP(SECONDS_PER_SECOND);
				exit(12);
			}
		}//end if
	

		world_file << "ID \t Local Force Size \t Traversing Difficulty \t Traversing Modifier \t Climate \n";

		printf("\nwriting now!");
		for (uS i = 0; i < WORLD_SIZE; i++)
		{
			HIGH_LOG
				std::cout << "\nwriting: "	<< (int)world_copy[i].ID_of_Faction_Here				<< " | "
											<< (int)world_copy[i].Military_Presense					<< " | "
											<< (int)world_copy[i].Base_Traversing_Difficulty		<< " | "
											<< (int)world_copy[i].Traversing_difficulty_Modifier	<< " | "
											<< (char)(world_copy[i].Climate);
			END_LOG

			world_file << (int)world_copy[i].ID_of_Faction_Here 	<< "\t | "
			<< (int)world_copy[i].Military_Presense					<< "\t\t\t\t | "
			<< (int)world_copy[i].Base_Traversing_Difficulty		<< "\t\t\t\t\t | "
			<< (int)world_copy[i].Traversing_difficulty_Modifier	<< "\t\t\t\t\t | "
			<< (char)(world_copy[i].Climate)						<< "\n";	
			
			//Writing was VERY CPU intensive, hopefully this nap helps
			IF_HIGH_PERFORMANCE	{	NAP(SECONDS_PER_SECOND*5);		}
			IF_LOW_PERFORMANCE	{	NAP(SECONDS_PER_SECOND*500);	}
		}

		world_file << "[END REACHED... READ/WRITE SUCCESS... YAY]";
		world_file.close();


		//		https://stackoverflow.com/a/10195497
		//	May you have wonderful dreams and prosper in life: 
		//	Merci beaucoup et faire des bon rêves chaque jour mon pote!
		//  谢谢我的朋友！您太棒了! 对不起我不知道汉语... 我是学生中文 :) 
		
		std::ifstream  src(WORLD_FILE_PATH, std::ios::binary);
		std::ofstream  dst(WORLD_BACKUP,   	std::ios::binary);
		dst << src.rdbuf();
		src.close();
		dst.close();

		world_copy.~vector(); 	// Dont need it anymore, hopefully the space will be freed up for something else 
		IF_HIGH_PERFORMANCE	{	SLEEP(SECONDS_PER_SECOND*3);	}
		IF_LOW_PERFORMANCE	{	SLEEP(SECONDS_PER_SECOND*15);	}
		
		IF_LOW_PERFORMANCE	{	Permission_to_work = true;	}
	}
	

}



void test_co_ordinates_conversion(uS index)
{
	Co_Ordinates temp_co_ords = Int_To_Co_Ordinates(index);
	uS temp_us = Co_Ordinates_to_Int(temp_co_ords);
	
	printf("\n%d to Co-ords is %d,%d\n", index, temp_co_ords.X, temp_co_ords.Y);
	printf("Converted back, %d, %d returns %d\n", temp_co_ords.X, temp_co_ords.Y, temp_us);
} 	



void Show_Land_Share(uMint faction_id)
{
	uS tempUs 	= 	ID_to_Faction(faction_id)->Territory_Count();
	float percent	= 	(100 / (float)WORLD_SIZE ) * (float)tempUs;
	std::cout << ID_to_Faction(faction_id)->Fac_Name << " owns " << tempUs << " (~" << percent << ") of the world\n"; 
} 
void Show_Land_Share()
{
	for (uMint i = 0; i < HOW_MANY_FACTIONS; i++)
	{
		Show_Land_Share(i);
	}
}



void print_factions()
{
	NEW_LINE;		THIN_LINE;
		printf("\n\tNAME\t\t\t\tHOME\tStrength\tStructure\n");
	THIN_LINE;		NEW_LINE;


	for (int i = 0; i < HOW_MANY_FACTIONS; i++)
	{
		std::cout << i << ":\t" << ALL_FACTIONS[i].Fac_Name << " (" ; 
		for (uMint x = 0; x < ALL_FACTIONS[i].Callsign.length(); x++)
		{
		 	std::cout << ALL_FACTIONS[i].Callsign[x];
			if ( x != ALL_FACTIONS[i].Callsign.length() - 1 )
			{
				std::cout << "."; 
			}
			
		}

		
		
		std::cout << ")\t\t: " << ALL_FACTIONS[i].Fac_Home_Location;
		std::cout << "\t: " << ALL_FACTIONS[i].Selected_Force_Location << "\t: ";
		switch (ALL_FACTIONS[i].Military_Structure)
		{
			case Disorganised:
				std::cout << "Disorganised";
				break;
			case Migrant:
				std::cout << "Migrant";
				break;
			case Organised:
				std::cout << "Organised";
				break;
			default:
				break;
		}
		std::cout << std::endl;
	}
}



void Startup_Threads(std::vector<std::thread>* thread_pool,	bool show_id = false)
{
	for (uMint i = 0; i < HOW_MANY_FACTIONS; i++)
	{
		/* I forgot how this exactly works? lambda is confusing */
		thread_pool->at(i) = std::thread
		( 
			[=] { 
					FACTION_START(&ALL_FACTIONS.at(i)); 
				} 
		);	
		
		if (show_id)
		{
			std::cout << "\nID: "<< thread_pool->at(i).get_id();
		}//end if
	}//end for

	
	for (uMint i = 0; i < HOW_MANY_FACTIONS; i++)
	{
		ALL_FACTIONS[i].HEART_ptr = &thread_pool->at(i);
		thread_pool->at(i).detach();
	}

	//std::thread faction_permission_check_thread(Check_Work_Permission_For_Faction);
	//faction_permission_check_thread.detach();
}



void Check_for_out_of_Bounds_Faction_Starting_Positions()
{
	bool does_anything_need_to_done = false;
	
	for (uS i = 0; i < HOW_MANY_FACTIONS; i++)
	{
		if (Faction_Homes[i] >= WORLD_SIZE) // will this feller be set out of bounds?
		{
			does_anything_need_to_done = true;
			break;
		}
	}

	if (does_anything_need_to_done)
	{
		printf(HIGH_SEVERITY_ERROR);
		printf("\nHEY! Someone's home is out of bounds! Adjust their home location or resize world!\n\n");
		exit(1);	
	}
}



void Clear_World()
{
	for (int i = 0; i < WORLD_SIZE; i++)
	{
		WORLD[i].ID_of_Faction_Here 			= 0;
		WORLD[i].Military_Presense 				= 0;
		WORLD[i].Base_Traversing_Difficulty 	= 0;
		WORLD[i].Traversing_difficulty_Modifier = 0;
		WORLD[i].Change_Amount_per_second 		= 0;
	
		if ( i <= (WORLD_SIZE/3) )
		{
			WORLD[i].Climate = Cold;
		}
		else if ( i <= ( (WORLD_SIZE/3) * 2 ) )
		{
			WORLD[i].Climate = Mild;
		}
		else
		{
			WORLD[i].Climate = Hot;
		}
	}//end for
}



void INIT_WORLD()
{
	srand(420);
	
	HIGH_LOG 	printf("\nInitialising world");	END_LOG
	
	get_set_ints_from_config();			// 	set up our ints, like width, height, faction count, etc...
	HIGH_LOG	printf("\nGot mints!");			END_LOG
	
	get_set_fac_details_from_config();	//	get faction details, read it like a csv, get names, structures, etc...
	HIGH_LOG	printf("\nGot Fuccs!");			END_LOG

	Test_mints();

	Check_for_out_of_Bounds_Faction_Starting_Positions();
	HIGH_LOG	printf("\nChecked for out-of-bounds!");	END_LOG

	HIGH_LOG	printf("\nReserving world space");	END_LOG
	WORLD.resize(WORLD_SIZE);	// Just to try make performance nicer!
	
	HIGH_LOG	printf("\nReserved world space");	END_LOG
	Clear_World();
	
	HIGH_LOG	printf("\nINIT WORLD() All done");	END_LOG
}



void print_map(uMint mode)	//0: Show Faction IDs	1: Local military presense	2: Callsign of the local faction	3: Just the index
{
	enum world_detail_type {ID, Military_pressence, Initials, Number };

	world_detail_type show_type = static_cast<world_detail_type>(mode);

	for (uS i = 0; i < WORLD_SIZE; i++)
	{
		if (i % WORLD_WIDTH == 0)
		{
			std::cout << std::endl;
		}


		switch (show_type)
		{
			case ID:
				printf("%d\t", WORLD[i].ID_of_Faction_Here);
				break;
			case Military_pressence:
				printf("%d\t", WORLD[i].Military_Presense);
				break;
			case Initials:
				std::cout << ID_to_Faction(WORLD[i].ID_of_Faction_Here)->Callsign << "\t";
				break;
			case Number:
				std::cout << i << "\t";
				break;
				
			
			default:
				break;
		}//end switch
	}//end for loop	
}