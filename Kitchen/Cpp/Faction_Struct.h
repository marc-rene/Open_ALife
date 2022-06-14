#include    "World.h"




/*
    This is all the Factions abilities but with no logic.
    Think of this header file like the human body...
    We can define abilities like "breathe", "Move", "blink",
    But it's only until we get to Faction_AI.h that all of that comes together
    to make an actual thinking person. 

    Here we define the ship, But Faction_AI.h is defining the captain!
*/



enum   	Faction_Status         	{   Invigorated,	Normal,		Sufficient, 	Sluggish,   Critical,   DEAD    };
uMint                   Constructor_Count = 0;


struct FACTION
{
    std::string                     Fac_Name; // wanted to make this a char array but we're dealing with unknown values at runtime, array allocation is impossible without black magic... std::string will have to do
	std::string						Callsign;
    uS                              Fac_Selected_Force_Location;
    uS                              Fac_Home_Location;  
	
    std::thread*                    HEART_ptr;      //  cant do anything without a heart

    Faction_Status  				Current_Status	=	Normal;
	Faction_Structure				Military_Structure;
    uMint                           Fac_ID;     
    uMint                           Fac_Team = -1;


    ~FACTION()              //  Faction Deconstroctor
    {
        HIGH_LOG
            printf("\nDeconstructor called");
        END_LOG
    }



    FACTION() //  The default constructor... should be used at start
    {   
        HIGH_LOG
            printf("\ndefault constroctor being used %d times", Constructor_Count);
        END_LOG

        Fac_ID              =       Constructor_Count;

        Fac_Name           			=   Faction_Names[Fac_ID];
        Fac_Home_Location           =   Faction_Homes[Fac_ID];
        Fac_Selected_Force_Location = 	Faction_Homes[Fac_ID];
		Military_Structure			=	Faction_Structures[Fac_ID];

        WORLD[Fac_Home_Location].ID_of_Faction_Here = Fac_ID;
        WORLD[Fac_Home_Location].Military_Presense = Starting_Strengths.at(Fac_ID);

		Callsign = Faction_Initials.at(Fac_ID);
		
		//Clean it up just incase code tomfoolery occured
		Callsign.erase(std::remove(Callsign.begin(), Callsign.end(), '\t'), Callsign.end());
		Callsign.erase(std::remove(Callsign.begin(), Callsign.end(), ' '), Callsign.end());
		

        Constructor_Count++;
    }            
                  


	FACTION(uMint id,	std::string name, 	std::string callsign,	uS starting_strength,	uS starting_tile,	std::thread* thread_to_use,	Faction_Structure military_structure,	 uMint logging_level)
	{
		Fac_ID 						= 	id;
		Fac_Name 					= 	name;
		Callsign 					= 	callsign;
		Fac_Selected_Force_Location = 	starting_strength;
		Fac_Home_Location 			= 	starting_tile;
		HEART_ptr 					= 	thread_to_use;
		Military_Structure 			= 	military_structure;

		HIGH_LOG
			printf("\nReplacement Faction being made\n");
		END_LOG
		
	}



    ///////////     "Captain! Can we go here?"      ///////////  
    void    Move   (   Direction  Where_to_captain    )            
    {
        HIGH_LOG
            NEW_LINE;
			std::cout << Fac_Name << " is starting move function\n ";
        END_LOG

		Co_Ordinates location = Int_To_Co_Ordinates(Fac_Selected_Force_Location);

        /*
        YO YO
        Our map looks like this by the way:
        0   1   2   3   4           N
        5   6   7   8   9           ^
        10  11  12  13  14     W <  +  > E
        15  16  17  18  19          v
        20  21  22  23  24          S

        */
       
        switch (Where_to_captain) 
        {
            ///////////         NORTH           ///////////
            case North:
                if (location.Y == WORLD_HEIGHT - 1)
                {
                    // "NO! We just went off the map!
                    MED_LOG
                        NEW_LINE;
                        printf(LOW_SEVERITY_ERROR);  
						std::cout << Fac_Name << "Cant go north!\n";
                    END_LOG
                }

                else
                {
					LOW_LOG
						std::cout << "From " << Fac_Selected_Force_Location << ", ";
					END_LOG

					location.Y += 1;
                    Fac_Selected_Force_Location = Co_Ordinates_to_Int(location);
                    LOW_LOG
						std::cout << Fac_Name << " has gone North and is at " << Fac_Selected_Force_Location <<"\n";
                    END_LOG
                }
                break;
            


            ///////////         EAST            ///////////
            case East:
                if ( location.X == WORLD_WIDTH - 1 )
                {
                    MED_LOG
                        NEW_LINE;
                        printf(LOW_SEVERITY_ERROR);  
						std::cout << Fac_Name << "Cant go East!\n";
                    END_LOG
                }

                else
                {
					LOW_LOG
						std::cout << "From " << Fac_Selected_Force_Location << ", ";
					END_LOG
					
					location.X += 1;
					Fac_Selected_Force_Location = Co_Ordinates_to_Int(location);
                    LOW_LOG
						std::cout << Fac_Name << " has gone East and is at " << Fac_Selected_Force_Location <<"\n";
                    END_LOG
                }
                break;
                    


            ///////////         SOUTH           ///////////
            case South:
                if (location.Y == 0)
                {
                    // "NO! We just went off the map!
                    MED_LOG
                        NEW_LINE;
                        printf(LOW_SEVERITY_ERROR);  
						std::cout << Fac_Name << "Cant go South!\n";
                    END_LOG
                }

                else
                {
					LOW_LOG
						std::cout << "From " << Fac_Selected_Force_Location << ", ";
					END_LOG
					
					location.Y -= 1;
                    Fac_Selected_Force_Location = Co_Ordinates_to_Int(location);
                    LOW_LOG
						std::cout << Fac_Name << " has gone South and is at " << Fac_Selected_Force_Location <<"\n";
                    END_LOG
                }
                break;



            ///////////         WEST            ///////////
            case West:
                if ( location.X == 0  )
                {
                    MED_LOG
                        NEW_LINE;
                        printf(LOW_SEVERITY_ERROR);  
						std::cout << Fac_Name << "Cant go West!\n";
                    END_LOG
                }

                else
                {
					LOW_LOG
						std::cout << "From " << Fac_Selected_Force_Location << ", ";
					END_LOG
					
					location.X -= 1;
                    Fac_Selected_Force_Location = Co_Ordinates_to_Int(location);
                    LOW_LOG
						std::cout << Fac_Name << " has gone West and is at " << Fac_Selected_Force_Location <<"\n";
                    END_LOG
                }
                break;



            default:        // Some real tomfoolerly has happened for us to reach this point! Wtf happened?
                NEW_LINE;
                printf("%s: \nBIG FUCK UP IN FACTION MOVE FUNCTION\n\n\n",    MED_SEVERITY_ERROR );
                break;
        } // end switch



        HIGH_LOG
            NEW_LINE;
			std::cout << Fac_Name << " completed move function\n";
        END_LOG
    }
    ///////////////////////////////////////////////////////////



	/////////// 	"How much Memory are we using?"	///////////
    // Must be updated
	void   MemSTAT ()
    {
        NEW_LINE;	NEW_LINE;	THICK_LINE;
			std::cout << "\n\nMemory Status of " << Fac_Name << " ( " << (int)Fac_ID << " ) : " << "\n";
        NEW_LINE;  				THIN_LINE;

        printf("\nFac_Name:\t\t\t%d bytes",                     sizeof(Fac_Name)                        );
        printf("\nCallsign:\t\t\t%d bytes",                		sizeof(Callsign)                   		);
        printf("\nFac_Selected_Force_Location:\t%d bytes",      sizeof(Fac_Selected_Force_Location)     );
        printf("\nFac_Home_Location:\t\t%d bytes",              sizeof(Fac_Home_Location)               );
        printf("\nHeart Thread pointer:\t\t%d bytes",         	sizeof(HEART_ptr)                   	);
        printf("\nCurrent status:\t\t\t%d bytes",               sizeof(Current_Status)                  );
        printf("\nMilitary structure:\t\t%d bytes",           	sizeof(Military_Structure)             	);
        printf("\nFac_ID:\t\t\t\t%d bytes",                     sizeof(Fac_ID)                          );
        printf("\nFac_Team:\t\t\t%d bytes",                     sizeof(Fac_Team)                        );
        NEW_LINE;    THIN_LINE;
        
        uS add =  	sizeof(Fac_Name)
				+	sizeof(Callsign)
				+	sizeof(Fac_Selected_Force_Location)
				+	sizeof(Fac_Home_Location)
				+	sizeof(HEART_ptr)
				+	sizeof(Current_Status)
				+	sizeof(Military_Structure)
				+	sizeof(Fac_ID)
				+	sizeof(Fac_Team);          
				  
        printf("\n\nTotal added: %d bytes\n",	add );
        printf("Total allocated: %d bytes\n\n",	sizeof(FACTION)	);
        
        THICK_LINE;        NEW_LINE;        NEW_LINE;
    }
	///////////////////////////////////////////////////////////



    uS  Territory_Count ()
    {
        uS Land_Count = 0; 

        HIGH_LOG
			NEW_LINE;
            std::cout << Fac_Name << " is starting the Counting of territories\n"; 
        END_LOG

          
        for (uS i = 0;  i < WORLD_SIZE;  i++)
        {
            if( WORLD[i].ID_of_Faction_Here     ==      Fac_ID)
            {
                Land_Count++;
            }
        }
        

        HIGH_LOG
			std::cout << "\n" << Fac_Name << " has " << Land_Count << " tiles\n"; 
        END_LOG


        return Land_Count;
    }



	void TAKE_A_BREAK(uS miliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
	}
};




std::vector<FACTION> ALL_FACTIONS;
