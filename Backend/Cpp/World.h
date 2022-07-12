#include    "MAIN_SETTINGS.h"

enum            Direction                   	{   North,  East,   South,  West    };

struct Tile;
std::vector<Tile>   WORLD;
	




struct  Tile
{

private:

	static void apply_attrition(Climates which_climate_to_affect) 
	{
		do
		{
			for (uS i = 0; i < WORLD_SIZE; i++)
		
			if ( WORLD[i].Climate 	==	which_climate_to_affect)
			{
				WORLD[i].Military_Presense -= WORLD[i].Change_Amount_per_second;
			}	
			SLEEP(SECONDS_PER_SECOND);
		}	
		while (true);
	}


public:

	uMint       ID_of_Faction_Here;
	short		Base_Traversing_Difficulty; 
	short		Traversing_difficulty_Modifier; 
	short		Change_Amount_per_second;
    short       Military_Presense;  
	Climates	Climate;



	static void Start_Storm_in_Climate_X(Climates which_climate_to_affect, mint intensity, int duration_in_seconds)	// Start a weather event for only a certain climate with X intensity
	{
		for (uS i = 0; i < WORLD_SIZE; i++)
		{
			if (WORLD[i].Climate == which_climate_to_affect)
			{
				WORLD[i].Change_Amount_per_second = intensity;
			}
		}

		std::thread storm_thr(apply_attrition, which_climate_to_affect);
		
		storm_thr.detach();
		SLEEP(duration_in_seconds*SECONDS_PER_SECOND);
		storm_thr.~thread();
	}	



	short GET_TRAVERSING_DIFFICULTY()
	{
		return Base_Traversing_Difficulty + Traversing_difficulty_Modifier;
	}



	void Trigger_an_Emission(short intensity)
	{
		if (Military_Presense -= intensity <= 0)
		{
			Military_Presense = 0;
		} 
		else
		{
			Military_Presense -= intensity;
		}
	}
};



struct Co_Ordinates
{
	int X;
	int Y;
};

Co_Ordinates Int_To_Co_Ordinates(uS Index)
{
	if (Index >= WORLD_SIZE)
	{
		printf(LOW_SEVERITY_ERROR);
		printf("\nInt to cordinates conversion function was given an invalid index to convert, maybe it's too big\n");
		return Co_Ordinates {-1, -1}; // if we EVER see -1, tom foolery has occured
	}
	
	Co_Ordinates temp_co;
	temp_co.X = Index % WORLD_WIDTH;
	temp_co.Y = WORLD_HEIGHT - (Index / WORLD_WIDTH) -1;
	return temp_co;
}

int Co_Ordinates_to_Int(Co_Ordinates co_ords)
{
	if ( (co_ords.X == -1) || (co_ords.Y == -1) ) ////////////////////////////////////	HEY CESAR MAKE SURE THIS WORKS CAUSE YOU CHANGED IT
	{
		printf(LOW_SEVERITY_ERROR);
		printf("\ncordinates to int conversion function was given an invalid coordinate to convert\n");
		return -1;		
	}
	
	return (WORLD_WIDTH * (WORLD_HEIGHT - co_ords.Y - 1) ) + co_ords.X;
}