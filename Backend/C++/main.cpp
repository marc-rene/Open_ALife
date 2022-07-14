#include "UTILITY_ITEMS.h"

//		the path of includes should look like this
//		
//		            MAIN_SETTINGS.h
//		                 |
//		               World.h
//		                 |
//		            Faction Struct.h
//		                 |
//		             Faction AI.h
//		                 |
//		            UTILITY Items.h
//		                 |
//		               Here :)
//
//
//
//			It probably would have made more sense to write everything into the one file 
//		but oh sweet jesus christ on a tricycle, that would be hell to develop in!
//		I also understand that having mulitple #includes in the one cpp file might've made sense
//		but in this context this works perfectly fine! :)
//		
//
//			The purpose of this project is to make an open source version of the
//		A-Life system in the S.T.A.L.K.E.R video game francise that can be implemented into any 
//		application that may benefit from it, such as video games and/or visualisers.


//
//
//			The world is comprised into a 2d grid with different factions, Humans, Wildlife, etc...
//		can fight for territory, simply just exist in the world, and/or interact with it in numberous different ways.  
// 
//
//		The code is seperated into 3 clear-cut sections, and named using a restaurant as a metaphor:
//			1. 	The backend (what you are seeing here, is called the kitchen)
//			2.	the API (the waitors)
//			3.	the frontend (What will send in the starting parametres, is called the customers because they will be the ones who see the food)
//		
//			The starting parametres should be sent in from the frontend, and read in from our waiters/API files.
//		The backend/kitchen should then start operation based on those parametres passed in from the Waiters/API
//		and begin simulation, saving its state to the Waiter/API Files.
//		Hopefully everything should be easy enough to figure out and apologies if not.
//
//
//
//		PLEASE send back feedback if you have any, I can be found at the following:
//			Email: 		cesar.hannin@hotmail.com
//			Instagram:	just.cesar.man
//			Reddit:		I_dont_like_pears
//			Discord:	Djagamoe #3385
//
//	Good hunting S.T.A.L.K.E.R						;)




int main() 
{

	// Get all of our starting parametres from the Waiters / API files
    INIT_WORLD(); 
	
	
	// Now lets initialise our threads! 
	// Each faction gets their own
	std::vector<std::thread> thr_pool(HOW_MANY_FACTIONS);	
	

	// call the faction constructor X amount of times
	ALL_FACTIONS.resize(HOW_MANY_FACTIONS);	


	// Make all threads in our pool start the faction AI, we pass in a reference to that pool
	Startup_Threads(&thr_pool);


	// 	Dedicate a thread to just writing to the map
	std::thread Map_Write_thread(Write_Map_to_Waiters_File);


	//	Dedicate a thread to determine if we should still be alive or not!
	std::thread Stay_Alive_Check(Check_to_See_If_We_Should_Still_Be_Running);
	//	...and Do NOT continue until the Stay_Alive check finishes or fails
	Stay_Alive_Check.join();


	//////////////////	DEBUGING CODE GOES HERE	//////////////////


	test_co_ordinates_conversion(0);
	test_co_ordinates_conversion(100);





	
	HIGH_LOG
		printf("\n\nyay all done\n\n");
	END_LOG

	//////////////////////////////////////////////////////////////


	return 0;
}
