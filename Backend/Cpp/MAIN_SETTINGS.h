#include        		<stdio.h>
#include        		<stdlib.h>
#include        		<iostream>
#include        		<vector>
#include        		<thread>
#include        		<string>
#include        		<fstream>
#include				<algorithm>





#define         	mint                        	__int8           	//	micro int, or "mint"... I thought it was cute
#define         	uMint                       	unsigned __int8                        
#define         	uS                          	unsigned short 

#define         	MINT_CONFIG_FILE_PATH       	"../../../Waiters/config.mints"
#define         	FAC_CONFIG_FILE_PATH       		"../../../Waiters/Faction.fucc"		// "factions-universelles cadres et comment on le citer"... Nothing else
#define				STATUS_READ_FILE_PATH			"../../../Waiters/Keep_Reading.bool"	// This should be read to determine if we should exit or not, determined by a 1 or 0
#define				NEW_REPORT_FILE_PATH			"../../../Waiters/News.report"			// This should be used to update the stage (GUI/Front-End) about any updates!
#define				WORLD_FILE_PATH					"../../../Waiters/map.world"
#define				WORLD_BACKUP					"../../../Waiters/backup.world"

#define         	LOW_SEVERITY_ERROR          	"\nMinor Tomfoolery error"
#define         	MED_SEVERITY_ERROR          	"\nWell... Shit, Error"
#define         	HIGH_SEVERITY_ERROR         	"\nOH GOD HELP ME ERROR"

#define         	NEW_LINE                    	printf("\n")
#define         	THIN_LINE                   	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -")
#define         	THICK_LINE                  	printf("\n###############################################################################################")

#define 			SLEEP(x)						std::this_thread::sleep_for(std::chrono::seconds(x) )
#define 			NAP(x)							std::this_thread::sleep_for(std::chrono::milliseconds(x) )

#define     		HIGH_LOG      					if(LOG_LEVEL >= 3){
#define     		MED_LOG       					if(LOG_LEVEL >= 2){
#define     		LOW_LOG       					if(LOG_LEVEL >= 1){
#define     		END_LOG             			}


unsigned int		SECONDS_PER_SECOND			=	1;		// this is mostly for Speed control... a bigger number means the program will run slower but use up less resources hopefully
bool				HIGH_PERFORMANCE_MODE		=	0;
#define				IF_HIGH_PERFORMANCE				if(HIGH_PERFORMANCE_MODE)	// 	My laptop may not be the fastest thing in the world...
#define				IF_LOW_PERFORMANCE				if(!HIGH_PERFORMANCE_MODE)	//	in fact it's not fast at all actually...
																				//	But damn it! it certainly tries! and hopefully this should help it!
																				//	This was all tested on an Intel Celeron N6000 at 20% battery life most of the time
																





// This is just to see if factions can work or not...
bool										Permission_to_work 	= false; 
bool										Exiting 			= false; 
	



//	The military structure of a faction could be organised in 3 distinct ways...
//		1.	Disorganised: 	This would be your Bandits and Mercenaries... No central planning at all.
//
//		2.	Migrant:		This would be your Roma and wildlife, if we ever wanted to make them a faction...
//							these people move constantly to empty spots only
//
//		3.	Organised:		Normal Military as we've come to know them, has central planning 

enum	Faction_Structure		{	Disorganised,	Migrant,	Organised	};	



// 	They're up here to make them publicly accessable from now on
enum Climates 	{ Cold = 'C', 	Mild = 'M', 	Hot = 'H' 	}; 	
enum Storms 	{ Blizzard, Terrential_Rain, Heatwave, Emmission }; 
//	We'll be seeing this mostly in World.h


/////// 	  Now we getting into some... Undefined stuff...		//////
uMint  										HOW_MANY_FACTIONS;			//
uMint   									LOG_LEVEL;					//
uMint   									WORLD_WIDTH;				//
uMint   									WORLD_HEIGHT;				//
uMint										Random_Strength_Range_MAX;	//
uMint										Random_Strength_Range_MIN;	//
uS 											WORLD_SIZE;					//
std::vector<std::string>        			Faction_Names;   			//
std::vector<std::string>					Faction_Initials;  			//
std::vector<uS>        						Faction_Homes; 				//
std::vector<uS>								Starting_Strengths;			//
std::vector<bool>							Essential_List;				//
std::vector<Faction_Structure>				Faction_Structures;			//
//   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  //









////////////////////			THIS FUNCTION IS COMPLETELY RESPONSIBLE FOR		 		///////////////
////////////////////			THE READING AND SETTING OF OUR INTEGER VALUES			///////////////
																										
void get_set_ints_from_config()																			
{																										
																										
	/*  		find and open the config file		*/													
    std::ifstream  		config(MINT_CONFIG_FILE_PATH);													
																										
	// 	As the name implies, this will just																
	//	act as a placeholder for all our lines															
	std::string		    temp_string;																	
																										
	//	Will we extract the values in the next line, yay or nay?										
    bool                extract_next_line   =   false;													
																										
	//	And if we will extract that line, what data will we set											
	enum    var_types 	                                                                               
    {	how_many_factions, 	rand_strength_range_max,	rand_strength_range_min,	e_Log_Level, 	world_width,	world_height,
		performance_mode, 	speed_divider	};			           
                                                                                                       
    var_types next_var;																					
																										
																											
	/*	Oh god hope that the file works and is valid?	*/													
    if (!config)																							
    {	
		for (uMint i = 0; i < 10; i++)
		{
			if (config)
			{
				break; // yay! it worked 
			}
			else
			{
				printf(LOW_SEVERITY_ERROR);
				printf("\nAh fiddlestciks! We couldn't open the mints file \ntrying to open file again! \nWish us luck");
				printf("\nCHECK MAIN_SETTINGS.h AND SEE IF THE RELATIVE PATHS ARE CORRECT... Visual Studio and Visual Studio Code cause Tomfoolery here!!!");
				config.open(MINT_CONFIG_FILE_PATH);
				SLEEP(SECONDS_PER_SECOND);
			}//end else
				
			SLEEP( SECONDS_PER_SECOND*5 );	
		}//end child if


		if (!config) 	// is it still not working??? 
		{
			printf(HIGH_SEVERITY_ERROR);
			printf("\nWE STILL CANT OPEN OR FIND THE MINTS!!! \nI HAVE FAILED YOU ANAKIN");
			exit(4);
		}//end child if					

    }// end file check... hope it worked																										
																										
																										
																										
	/*		Yay! The file's been found																		
		Time to read the config file line by line...														
		And save the line into the temporary string 														
			variable we made earlier!				*/ 														
    while (std::getline(	config,		temp_string ))														
    {																										
																										
		//	only do this if we've found a value to set													
        if (extract_next_line)																			
        {																								
																										
			/*	What variable will 																		
				we be setting?		*/																	
			switch (next_var)																			
			{																							
				case how_many_factions:							       								
					HOW_MANY_FACTIONS = (uMint)stoi(temp_string);        								
					extract_next_line = false;															
					break;		

				case rand_strength_range_max:
					Random_Strength_Range_MAX = (uMint)stoi(temp_string);        								
					extract_next_line = false;															
					break;

				case rand_strength_range_min:
					Random_Strength_Range_MIN = (uMint)stoi(temp_string);        								
					extract_next_line = false;															
					break;																		
																										
																										
				case e_Log_Level:																	
					LOG_LEVEL = (uMint)stoi(temp_string);     									
					extract_next_line = false;															
					break;																				
																										
																										
				case world_width:																		
					WORLD_WIDTH = (uMint)stoi(temp_string);     										
					extract_next_line = false;															
					break;																				
				
				
				case world_height:																		
					WORLD_HEIGHT = (uMint)stoi(temp_string);     										
					extract_next_line = false;															
					break;						

				case performance_mode:
					HIGH_PERFORMANCE_MODE = stoi(temp_string);
					extract_next_line = false;
					break;		

				case speed_divider:
					SECONDS_PER_SECOND = stoi(temp_string);
					extract_next_line = false;
					break;																				
																										
				default:																				
					printf("\nERROR IN get_set_ints_from_config SWITCH PART");							
					break;																				
																										
			}//end switch																				
																										
        }//end if-line extraction																		
																										
																										
																										
        //	check to see if it's a comment first														
        if (temp_string[0] == '#')																		
        {	continue;	}																				
																										
																										
		//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	//			
		//								HAVE WE FOUND SOMETHING?							//			
		//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	
        else if (temp_string.find("[FACTION COUNT]") != std::string::npos )					
        {																					
            next_var = how_many_factions;													
			extract_next_line = true;	// Start extraction									
        }																					
																							
																							
		else if (temp_string.find("[LOG LEVEL]") != std::string::npos )				
        {																					
            next_var = e_Log_Level;													
			extract_next_line = true;														
        }																					
																							
																							
		else if (temp_string.find("[WORLD WIDTH]") != std::string::npos )					
        {																					
            next_var = world_width;															
			extract_next_line = true;														
        }      																				
		
		
		else if (temp_string.find("[WORLD HEIGHT]") != std::string::npos )					
        {																					
            next_var = world_height;														
			extract_next_line = true;														
        }      

		else if (temp_string.find("[RANDOM STRENGTH RANGE MAX]") != std::string::npos )					
        {																					
            next_var = rand_strength_range_max;														
			extract_next_line = true;														
        }      								

		else if (temp_string.find("[RANDOM STRENGTH RANGE MIN]") != std::string::npos )					
        {																					
            next_var = rand_strength_range_min;														
			extract_next_line = true;														
        }    

		else if (temp_string.find("[USE HIGH PERFORMANCE MODE? 0 or 1]") != std::string::npos )
		{
			next_var = performance_mode;
			extract_next_line = true;
		}  

		else if (temp_string.find("[SPEED DIVIDER]") != std::string::npos )
		{
			next_var = speed_divider;
			extract_next_line = true;
		}  																				
		//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	//			
																										
	}//end while																						
																										
																																																		
	config.close();

	WORLD_SIZE = WORLD_HEIGHT*WORLD_WIDTH;																
																										
}																										
																										
////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////			EXACT SAME AS THE ABOVE FUNCTION, EXCEPT WE'RE GETTING FACTION DETAILS			////////////////////	

void get_set_fac_details_from_config()																										
{		
	uMint counter = 0; //DEBUG ONLY

	std::ifstream   config( FAC_CONFIG_FILE_PATH );    //  find and open the faction config file											
	std::string     temp_string;																											
	bool start_extraction = false;																											
																																																																				
    if (!config)																							
    {	
		for (uMint i = 0; i < 10; i++)
		{
			if (config)
			{
				break; // yay! it worked 
			}
			else
			{
				printf(LOW_SEVERITY_ERROR);
				printf("\nAh fiddlestciks! We couldn't open the fuccs file \ntrying to open file again! \nWish us luck");
				printf("\nCHECK MAIN_SETTINGS.h AND SEE IF THE RELATIVE PATHS ARE CORRECT... Visual Studio and VSCode cause Tomfoolery here!!!");
				config.open(MINT_CONFIG_FILE_PATH);
				SLEEP(SECONDS_PER_SECOND);
			}//end else
				
			SLEEP( SECONDS_PER_SECOND*5 );	
		}//end child if


		if (!config) 	// is it still not working??? 
		{
			printf(HIGH_SEVERITY_ERROR);
			printf("\nWE STILL CANT OPEN OR FIND THE FUCCS!!! \n");
			exit(19);
		}//end child if					

    }// end file check... hope it worked																										


	while (std::getline(config,	temp_string))																								
	{																																		
		// 	Is the line line a comment?																										
		if (temp_string[0] == '#')																											
		{	continue;	}																													
																															
		//	Have we reached the end																									
		else if (temp_string.find("[END]") != std::string::npos)																			
		{	return;		}																													

		//	Go ahead been given, we've read START READ																																
		else if (start_extraction)																												
		{		
			counter++;

			HIGH_LOG 	printf("\nWe on the %dth guy\n", counter);	END_LOG																													
			
			uS div_indexs[5];																												
			uMint stage = 0;																												
			temp_string.erase( remove( temp_string.begin(), temp_string.end(), '\t' ), temp_string.end());	// get rid of tabs
																																		
			//check for | first and figure out where they are																				
			for (uS i = 0; i < temp_string.length(); i++)																					
			{																																
				if (temp_string[i] == '|')																									
				{																															
					div_indexs[stage] = i;																									
					stage++;																												
				}																															
			}//end for																														

																																		
																																		
			/*	Faction Names, Insignias, Faction Homes, Starting Strength, Faction Structures	*/	
			// 	the name will be the first thing we find, add that first...																													
			Faction_Names.push_back(temp_string.substr(0, div_indexs[0]) );																	
																																		

			//callsign	
			Faction_Initials.push_back(temp_string.substr( (div_indexs[0] + 1), (div_indexs[1] - div_indexs[0]) - 1 ) );


			//home
			uS temp_home;

			if (temp_string.find("Random Home") != std::string::npos)
			{
				HIGH_LOG
					printf("\nGenerating random home\n");
				END_LOG

				uS counter = 0;
				bool unclean = true;
				bool change_needed;
				
				do
				{
					change_needed = false;

					if (counter > 200)
					{
						HIGH_LOG
							printf("\n%s : BRO WTF! Either random here sucks or Im just a bad programmer!\n", LOW_SEVERITY_ERROR);
							printf("\nWe doing the crappy method of randomness");
						END_LOG

						for (uS x = WORLD_SIZE-1; x >= 0; x--)
						{
							for (uS y = 0; y < HOW_MANY_FACTIONS; y++)
							{
								if( x == Faction_Homes[y] )
								{ continue; }
								else
								{
									temp_home = x;
									unclean = false;
									change_needed = false;
									break;
								}
							}
						}
						
					}
					else if (counter > 50 )
					{
						temp_home = Faction_Homes.at(rand() % (HOW_MANY_FACTIONS/2) ) - (rand() % 3) + 1;
						printf("\nLOOP ERROR %d times", counter);
					}
					else
					{
						temp_home = rand() % WORLD_SIZE;
					}

					for (uS i = 0; i < HOW_MANY_FACTIONS; i++)
					{
						if (temp_home == Faction_Homes[i])
						{
							change_needed = true;
							break;
						}
					}//end for

					if (!change_needed)
					{
						unclean = false;
					}

					counter++;
				}//end do
				while (unclean);	

			}//end if

			else
			{																															
				std::string temp_home_s = temp_string.substr( (div_indexs[1] + 1), (div_indexs[2] - div_indexs[1]) - 1 ); //the plus 1's are her
				temp_home = (uS)stol(temp_home_s);																							
			}
			Faction_Homes.push_back(temp_home);																								


			//strength
			uMint temp_STRNTH;	

			if (temp_string.find("Random Strength") != std::string::npos)
			{	
				HIGH_LOG
					printf("\nGenerating random strength\n");
				END_LOG

				temp_STRNTH = Random_Strength_Range_MIN + ( rand() % (Random_Strength_Range_MAX-Random_Strength_Range_MIN) );
			}	
			else
			{																												
				std::string temp_STRNTH_s = temp_string.substr( (div_indexs[2] + 1), (div_indexs[3] - div_indexs[2]) - 1 );						
				temp_STRNTH = (uMint)stol(temp_STRNTH_s);																					
			}

			Starting_Strengths.push_back(temp_STRNTH);																						
																																		
																																		
			////////		Time to set up our Faction structures		//////////															
			if (temp_string.find("~Disorganised") != std::string::npos)			//															
			{	Faction_Structures.push_back(Disorganised);	}					//															
																				//															
																				//															
			else if (temp_string.find("~Organised") != std::string::npos)		//															
			{	Faction_Structures.push_back(Organised);	}					//															
																				//															
																				//															
			else if (temp_string.find("~Migrant") != std::string::npos)			//															
			{	Faction_Structures.push_back(Migrant);		}					//															
			//////////////////////////////////////////////////////////////////////															
																																		
		}//end extraction																													
																																		
																																		
																																		
		/*		Start Extracting Data once we read the go ahead! 	*/																		
		else if (temp_string.find("[START READ]") != std::string::npos)																			
		{	start_extraction = true;	}																									
																																		
	}																																		
																																		
																																		
	config.close();																															
																																		
}


////////////////////////			Should we Exit or not?			////////////////////	

void Check_to_See_If_We_Should_Still_Be_Running()
{
	/*	The way this should work is by opening the Keep_Reading.bool file
		and exit if a 1 is read, or ignore if nothing is read				*/
	bool Stay_Alive = 	true;
	bool First_Run	=	true;

	do
	{
		std::fstream	file(STATUS_READ_FILE_PATH);
		
		if (!file)	// Was the file opened successfully?
		{
			for (uMint i = 0; i < 10; i++)
			{
				if (file)
				{
					break; // yay! it worked 
				}

				else
				{
					printf(LOW_SEVERITY_ERROR);
					printf("\nAh fiddlestciks! We couldn't open the status file to determine if we should keep going or not\ntrying to open file again!\nWish us luck");
					file.open(STATUS_READ_FILE_PATH);
					SLEEP(SECONDS_PER_SECOND);
				}//end else
				
				SLEEP( SECONDS_PER_SECOND*5 );	
			}//end child if

			if (!file) 	// is it still not working??? 
			{
				printf(HIGH_SEVERITY_ERROR);
				printf("\nWE CANT DETERMINE IF WE WILL CONTINUE ON OR NOT!!! \nPRAY HARDER NEXT TIME");
				printf("\n\nCHECK TO SEE IF Keep_Reading.bool IS IN THE CURTAINS FOLDER!\n");
				exit(-1);
			}//end child if

		}//end parent if
		
		std::string temp_string;

		std::getline(file,	temp_string);	// read the file and copy the output into temp_string


		if ( temp_string.find("Continue") != std::string::npos )
		{
			//We keep swimming
			SLEEP(SECONDS_PER_SECOND*10);
		}

		else if ( temp_string.find("Exit") != std::string::npos )
		{
			//Nevermind, lets close!
			Stay_Alive	=	false;
			Exiting = true;
			printf("\n\n\t\tATTENTION!\tWE GONNA SELF-DESTRUCT NOW!\n\t\t\tEXITING");
			SLEEP(SECONDS_PER_SECOND);
			printf(".");
			SLEEP(SECONDS_PER_SECOND);
			printf(".");
			SLEEP(SECONDS_PER_SECOND);
			printf(".");
			SLEEP(SECONDS_PER_SECOND);
			printf("\n\nBye bye :)");
			SLEEP(SECONDS_PER_SECOND);
		}

		else
		{
			if (LOG_LEVEL >= 3)
			{
				NEW_LINE;
				THIN_LINE;
				NEW_LINE;
				printf(MED_SEVERITY_ERROR);
				printf("\nWe couldn't make sense of the Keep_Reading file so we just gonna assume that it's a 'continue' there");
				THIN_LINE;	
			}

			file.close();
			SLEEP(SECONDS_PER_SECOND);

			std::ofstream out_file(STATUS_READ_FILE_PATH);
			out_file << "Continue";
			out_file.close();
		}
		
		if(First_Run)
		{
			Permission_to_work = true;
			First_Run = false;
		}

		file.close();
		SLEEP( SECONDS_PER_SECOND*10 );

	} while (Stay_Alive);
}//end function



////////////////////////			THE GUI MUST BE TOLD!			////////////////////	

// If an oncoming storm is approaching
//void News_Report(Weathers )



////////////////////////////////////////////////////////////////////////////////////////



