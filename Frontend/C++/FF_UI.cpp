#include "FF_UI.h"
#include "imgui.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

#define DANGER_COLOUR ImVec4(1.0f, 0.23f, 0.23f, 1.0f)
#define WARNING_COLOUR ImVec4(1.0f, 0.6f, 0.0f, 1.0f)

char        MINT_CONFIG_FILE_PATH[1024] = "../../../../API/config.mints";
char        FAC_CONFIG_FILE_PATH[1024] = "../../../../API/Faction.fucc";


static unsigned int times_rendered = 0;

//copied from main_settings.h

//	The military structure of a faction could be organised in 3 distinct ways...
//		1.	Disorganised: 	This would be your Bandits and Mercenaries... No central planning at all.
//
//		2.	Migrant:		This would be your Roma and wildlife, if we ever wanted to make them a faction...
//							these people move constantly to empty spots only
//
//		3.	Organised:		Normal Military as we've come to know them, has central planning 

enum	Faction_Structure { Disorganised, Migrant, Organised };



// 	They're up here to make them publicly accessable from now on
enum Climates { Cold = 'C', Mild = 'M', Hot = 'H' };
enum Storms { Blizzard, Terrential_Rain, Heatwave, Emmission };
//	We'll be seeing this mostly in World.h


/////// 	  Now we getting into some... Undefined stuff...		//////
unsigned _int8  							HOW_MANY_FACTIONS = 1;		//
unsigned _int8   							WORLD_WIDTH = 5;			//
unsigned _int8   							WORLD_HEIGHT = 5;			//
unsigned _int8								Random_Strength_Range_MAX;	//
unsigned _int8								Random_Strength_Range_MIN;	//
unsigned short 								WORLD_SIZE = 25;			//
bool										High_Performance_Mode;		//
std::vector<std::string>        			Faction_Names;   			//
std::vector<std::string>					Faction_Initials;  			//
std::vector<unsigned short>        			Faction_Homes; 				//
std::vector<unsigned short>					Starting_Strengths;			//
std::vector<bool>							Essential_List;				//
std::vector<Faction_Structure>				Faction_Structures;			//
//   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  //


/* Thank you https ://rextester.com/SLYX81102 */
std::string execute(std::string cmd)
{
    std::string file_name = "CMD_READ.txt";
    std::system((cmd + " > " + file_name).c_str()); // redirect output to file

    // open file for input, return string containing characters in the file
    std::ifstream file(file_name);

    return
    {
        std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()
    };
}


namespace Faction_Fight_UI
{

    static unsigned __int8 WORLD_WIDTH;
    bool    alread_read_mints = false;
    bool    alread_read_fuccs = false;
    bool    found_mints = false;
    bool    found_fuccs = false;
    static bool show_app_metrics = false;
    static bool show_app_stack_tool = false;
    static bool show_app_style_editor = false;
    static bool show_app_about = false;


    
    struct Co_Ordinates
    {
        int X;
        int Y;
    };

    Co_Ordinates Int_To_Co_Ordinates(unsigned short Index)
    {
        if (Index >= WORLD_SIZE)
        {
            printf("\nInt to cordinates conversion function was given an invalid index to convert, maybe it's too big\n");
            return Co_Ordinates{ -1, -1 }; // if we EVER see -1, tom foolery has occured
        }

        Co_Ordinates temp_co;
        temp_co.X = Index % WORLD_WIDTH;
        temp_co.Y = WORLD_HEIGHT - (Index / WORLD_WIDTH) - 1;
        return temp_co;
    }

    int Co_Ordinates_to_Int(Co_Ordinates co_ords)
    {
        if ((co_ords.X == -1) || (co_ords.Y == -1)) /////	HEY MAKE SURE THIS WORKS CAUSE I CHANGED IT
        {
            printf("\ncordinates to int conversion function was given an invalid coordinate to convert\n");
            return -1;
        }

        return (WORLD_WIDTH * (WORLD_HEIGHT - co_ords.Y - 1)) + co_ords.X;
    }
    


    void get_set_ints_from_config(bool* read_mint_already, bool* status)
    {
        if (*read_mint_already == false)
        {
            *status = false;
            printf("\nOpening and reading .mints file!\n");


            /*  		find and open the config file		*/
            std::ifstream  		config(MINT_CONFIG_FILE_PATH);

            // 	As the name implies, this will just																
            //	act as a placeholder for all our lines															
            std::string		    temp_string;

            //	Will we extract the values in the next line, yay or nay?										
            bool                extract_next_line = false;

            //	And if we will extract that line, what data will we set											
            enum    var_types
            {
                how_many_factions, rand_strength_range_max, rand_strength_range_min, e_Log_Level, world_width, world_height,
                performance_mode, speed_divider
            };

            var_types next_var;


            /*	Oh god hope that the file works and is valid?	*/
            if (!config)
            {

                for (int i = 0; i < 5; i++)
                {
                    if (config)
                    {
                        *status = true;
                        break; // yay! it worked 
                    }
                    else
                    {
                        printf("\nAh fiddlestciks! We couldn't open the mints file \ntrying to open file again! \nWish us luck");
                        *status = false;
                        config.open(MINT_CONFIG_FILE_PATH);
                    }//end else
                }//end child if

                if (!config)
                {
                    *status = false;
                    *read_mint_already = true;
                    return;
                }

            }// end file check... hope it worked																										



            /*		Yay! The file's been found
                Time to read the config file line by line...
                And save the line into the temporary string
                    variable we made earlier!				*/
            while (std::getline(config, temp_string))
            {

                //	only do this if we've found a value to set													
                if (extract_next_line)
                {

                    /*	What variable will
                        we be setting?		*/
                    switch (next_var)
                    {
                        case how_many_factions:
                            HOW_MANY_FACTIONS = (unsigned _int8)stoi(temp_string);
                            extract_next_line = false;
                            printf("Read Faction count\n");
                            break;

                        case rand_strength_range_max:
                            Random_Strength_Range_MAX = (unsigned _int8)stoi(temp_string);
                            extract_next_line = false;
                            printf("Read strength range max\n");
                            break;

                        case rand_strength_range_min:
                            Random_Strength_Range_MIN = (unsigned _int8)stoi(temp_string);
                            extract_next_line = false;
                            printf("Read strength range min\n");
                            break;


                        case e_Log_Level:
                            extract_next_line = false;
                            printf("Read log level\n");
                            break;


                        case world_width:
                            WORLD_WIDTH = (unsigned _int8)stoi(temp_string);
                            extract_next_line = false;
                            printf("Read width\n");
                            break;

						case performance_mode:
							High_Performance_Mode = stoi(temp_string);
							extract_next_line = false;
							printf("Read Performance Mode\n");
							break;


                        case world_height:
                            WORLD_HEIGHT = (unsigned _int8)stoi(temp_string);
                            extract_next_line = false;
                            printf("Read height\n");
                            break;



                        default:
                            //printf("\nERROR IN get_set_ints_from_config SWITCH PART");
                            break;

                    }//end switch																					

                }//end if-line extraction																		



                //	check to see if it's a comment first														
                if (temp_string[0] == '#' || temp_string[0] == '\0' || temp_string[0] == '\n')
                {
                    continue;
                }


                //	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	//			
                //								HAVE WE FOUND SOMETHING?							//			
                //	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	

                else if (temp_string.find("[FACTION COUNT]") != std::string::npos)
                {
                    next_var = how_many_factions;
                    extract_next_line = true;	// Start extraction									
                }


                else if (temp_string.find("[LOG LEVEL]") != std::string::npos)
                {
                    next_var = e_Log_Level;
                    extract_next_line = true;
                }


                else if (temp_string.find("[WORLD WIDTH]") != std::string::npos)
                {
                    next_var = world_width;
                    extract_next_line = true;
                }


                else if (temp_string.find("[WORLD HEIGHT]") != std::string::npos)
                {
                    next_var = world_height;
                    extract_next_line = true;
                }

                else if (temp_string.find("[RANDOM STRENGTH RANGE MAX]") != std::string::npos)
                {
                    next_var = rand_strength_range_max;
                    extract_next_line = true;
                }

                else if (temp_string.find("[RANDOM STRENGTH RANGE MIN]") != std::string::npos)
                {
                    next_var = rand_strength_range_min;
                    extract_next_line = true;
                }

                else if (temp_string.find("[USE HIGH PERFORMANCE MODE? 0 or 1]") != std::string::npos)
                {
                    next_var = performance_mode;
                    extract_next_line = true;
                }

                else if (temp_string.find("[SPEED DIVIDER]") != std::string::npos)
                {
                    next_var = speed_divider;
                    extract_next_line = true;
                }

            }//end while																						


            config.close();

            *status = true;
            *read_mint_already = true;
            WORLD_SIZE = WORLD_HEIGHT * WORLD_WIDTH;
        }
    }



    void get_set_fac_details_from_config(bool* local_read_fucc_already, bool* success)
    {
        if (*local_read_fucc_already == false)
        {
            printf("\nOpening Fucc file");
            unsigned _int8 counter = 0; //DEBUG ONLY

            std::ifstream   config(FAC_CONFIG_FILE_PATH);    //  find and open the faction config file											
            std::string     temp_string;
            bool start_extraction = false;

            if (!config)
            {
                for (unsigned _int8 i = 0; i < 10; i++)
                {
                    if (config)
                    {
                        *success = true;
                        break; // yay! it worked 
                    }
                    else
                    {
                        //printf("\nAh fiddlestciks! We couldn't open the fuccs file \ntrying to open file again! \nWish us luck");
                        //printf("\nCHECK MAIN_SETTINGS.h AND SEE IF THE RELATIVE PATHS ARE CORRECT... Visual Studio and VSCode cause Tomfoolery here!!!");
                        config.open(FAC_CONFIG_FILE_PATH);
                    }//end else

                }//end child if


                if (!config) 	// is it still not working??? 
                {
                    printf("\nWE STILL CANT OPEN OR FIND THE FUCCS!!! \n");
					*success = false;
                }//end child if					

            }// end file check... hope it worked
            else
            {
                *success = true;
            }
          

            //extraction of file... if its found
            if (*success == true)
            {

                while (std::getline(config, temp_string))
                {
                    // 	Is the line line a comment?																										
                    if (temp_string[0] == '#')
                    {
                        continue;
                    }

                    //	Have we reached the end																									
                    else if (temp_string.find("[END]") != std::string::npos)
                    {
                        break;
                    }

                    //	the Go-ahead been given, we've read START READ																																
                    else if (start_extraction)
                    {
                        counter++;

                        unsigned short div_indexs[5];
                        unsigned _int8 stage = 0;
                        temp_string.erase(remove(temp_string.begin(), temp_string.end(), '\t'), temp_string.end());	// get rid of tabs

                        //check for | first and figure out where they are																				
                        for (unsigned short i = 0; i < temp_string.length(); i++)
                        {
                            if (temp_string[i] == '|')
                            {
                                div_indexs[stage] = i;
                                stage++;
                            }
                        }//end for																														



                        /*	Faction Names, Insignias, Faction Homes, Starting Strength, Faction Structures	*/
                        // 	the name will be the first thing we find, add that first...																													
                        Faction_Names.push_back(temp_string.substr(0, div_indexs[0]));


                        //callsign	
                        Faction_Initials.push_back(temp_string.substr((div_indexs[0] + 1), (div_indexs[1] - div_indexs[0]) - 1));


                        //home
                        unsigned short temp_home;

                        if (temp_string.find("Random Home") != std::string::npos)
                        {
                            unsigned short counter = 0;
                            bool unclean = true;
                            bool change_needed;

                            do
                            {
                                change_needed = false;

                                if (counter > 200)
                                {
                                    for (unsigned short x = WORLD_SIZE - 1; x >= 0; x--)
                                    {
                                        for (unsigned short y = 0; y < HOW_MANY_FACTIONS; y++)
                                        {
                                            if (x == Faction_Homes[y])
                                            {
                                                continue;
                                            }
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
                                else if (counter > 50)
                                {
                                    temp_home = Faction_Homes.at(rand() % (HOW_MANY_FACTIONS / 2)) - (rand() % 3) + 1;
                                    printf("\nLOOP ERROR %d times", counter);
                                }
                                else
                                {
                                    temp_home = rand() % WORLD_SIZE;
                                }

                                for (unsigned short i = 0; i < HOW_MANY_FACTIONS; i++)
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
                            std::string temp_home_s = temp_string.substr((div_indexs[1] + 1), (div_indexs[2] - div_indexs[1]) - 1); //the plus 1's are her
                            temp_home = (unsigned short)stol(temp_home_s);
                        }
                        Faction_Homes.push_back(temp_home);


                        //strength
                        unsigned _int8 temp_STRNTH;

                        if (temp_string.find("Random Strength") != std::string::npos)
                        {
                            temp_STRNTH = Random_Strength_Range_MIN + (rand() % (Random_Strength_Range_MAX - Random_Strength_Range_MIN));
                        }
                        else
                        {
                            std::string temp_STRNTH_s = temp_string.substr((div_indexs[2] + 1), (div_indexs[3] - div_indexs[2]) - 1);
                            temp_STRNTH = (unsigned _int8)stol(temp_STRNTH_s);
                        }

                        Starting_Strengths.push_back(temp_STRNTH);


                        ////		Time to set up our Faction structures		////															
                        if (temp_string.find("~Disorganised") != std::string::npos)
                        {
                            Faction_Structures.push_back(Disorganised);
                        }

                        else if (temp_string.find("~Organised") != std::string::npos)
                        {
                            Faction_Structures.push_back(Organised);
                        }

                        else if (temp_string.find("~Migrant") != std::string::npos)
                        {
                            Faction_Structures.push_back(Migrant);
                        }


                    }//end extraction																													



                    /*		Start Extracting Data once we read the go ahead! 	*/
                    else if (temp_string.find("[START READ]") != std::string::npos)
                    {
                        start_extraction = true;
                    }

                }
            }


            *local_read_fucc_already = true;
            config.close();
        }

    }



    void ShowDockSpace()
    {
        // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
        // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
        // In this specific demo, we are not using DockSpaceOverViewport() because:
        // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
        // - we allow the host window to have padding (when opt_padding == true)
        // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
        // TL;DR; this demo is more complicated than what you would normally use.
        // If we removed all the options we are showcasing, this demo would become:
        //     void ShowExampleAppDockSpace()
        //     {
        //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //     }

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &opt_fullscreen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                if (ImGui::MenuItem("Quit", "Alt+F4"))
                {
                    exit(0);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }


    void Render_Settings()
    {

    }


    void Render_UI()
    {
        times_rendered++;

        ShowDockSpace();

        get_set_ints_from_config(&alread_read_mints, &found_mints);
        get_set_fac_details_from_config(&alread_read_fuccs, &found_fuccs);

        ImGuiWindowFlags Main_Window_Flags = 0;

        if (true)       Main_Window_Flags |= ImGuiWindowFlags_MenuBar;



        if (show_app_metrics) { ImGui::ShowMetricsWindow(&show_app_metrics); }
        if (show_app_stack_tool) { ImGui::ShowStackToolWindow(&show_app_stack_tool); }
        if (show_app_about) { ImGui::ShowAboutWindow(&show_app_about); }

        if (show_app_style_editor)
        {
            ImGui::Begin("Dear ImGui Style Editor", &show_app_style_editor);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }




        ImGui::Begin("Home", NULL, Main_Window_Flags);



        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Metrics/Debugger", NULL, &show_app_metrics);
                ImGui::MenuItem("Stack Tool", NULL, &show_app_stack_tool);
                ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
                ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }




        static ImGuiInputTextFlags text_flags = !ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine | !ImGuiInputTextFlags_ReadOnly;
        ImGui::CheckboxFlags("Read-only the file paths? ", &text_flags, ImGuiInputTextFlags_ReadOnly);
        ImGui::Text("We are reading all details from the following:");

        ImGui::InputText(".mints config file", MINT_CONFIG_FILE_PATH, 1024, text_flags);
        ImGui::SameLine();
        if (ImGui::Button("Reset to default path"))
        {
            printf("\nmint button pressed");
            char default_path[32] = "../../../../API/config.mints\0";
            for (unsigned _int8 i = 0; i < 32; i++)
            {
                MINT_CONFIG_FILE_PATH[i] = default_path[i];
            }
        }

        ImGui::InputText(".fuccs Factions file", FAC_CONFIG_FILE_PATH, 1024, text_flags);
        ImGui::SameLine();
        if (ImGui::Button("Reset to default path ###"))
        {
            printf("\nfucc button pressed");
            char default_path[32] = "../../../../API/Faction.fucc\0";
            for (unsigned _int8 i = 0; i < 32; i++)
            {
                FAC_CONFIG_FILE_PATH[i] = default_path[i];
            }
        }



        if (ImGui::Button("Rescan for .mints"))
        {
            alread_read_mints = false;
            found_mints = false;
            get_set_ints_from_config(&alread_read_mints, &found_mints);
        }
        ImGui::SameLine();
        if (found_mints)
        {
            ImGui::Text("Reading World Parametres from %s", MINT_CONFIG_FILE_PATH);
        }
        else
        {
            ImGui::TextColored(DANGER_COLOUR, "WE CANT FIND THOSE DAMN MINTS! MAKE SURE THE .MINTS PATH IS CORRECT");
        }



        if (ImGui::Button("Rescan for .fucc"))
        {
            Faction_Names.clear();
            Faction_Initials.clear();
            Faction_Homes.clear();
            Starting_Strengths.clear();
            Essential_List.clear();
            Faction_Structures.clear();

            alread_read_fuccs = false;
            found_fuccs = false;
            get_set_fac_details_from_config(&alread_read_fuccs, &found_fuccs);
        }
        ImGui::SameLine();
        if (found_fuccs)
        {
            ImGui::Text("Reading Faction Details from %s", FAC_CONFIG_FILE_PATH);
        }
        else
        {
            ImGui::TextColored(DANGER_COLOUR, "WE CANT FIND THOSE DAMN FUCCS! MAKE SURE THE .FUCC PATH IS CORRECT");
        }


        ImGui::Separator();

        ImGui::Text("Details are as follows:");

        ImGui::BulletText("CWD is ");
        ImGui::SameLine();
        
        ImGui::Text(execute("cd ").c_str() );

        if (found_mints)
        {
            ImGui::BulletText("World Width, World Height\t: %d, %d", WORLD_WIDTH, WORLD_HEIGHT);
            ImGui::BulletText("Number of Active Factions\t: %d", HOW_MANY_FACTIONS);
            if (High_Performance_Mode)
            {
                ImGui::BulletText("Performance Mode\t\t\t : FAST MODE");
            }
            else
            {
                ImGui::BulletText("Performance Mode\t\t\t : Power Saver Mode");
            }
        }
        else
        {
            ImGui::TextColored(WARNING_COLOUR, "Please make sure that your .mint file has been found!");
        }



        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
        static ImVec2 cell_padding(3.0f, 6.0f);
        static bool show_widget_frame_bg = true;

        ImGui::SliderFloat2("CellPadding", &cell_padding.x, 0.0f, 10.0f, "%.0f");
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

        if (found_mints)
        {
            if (ImGui::BeginTable("table1", WORLD_WIDTH, flags))
            {
                unsigned short index = 0;

                for (int row = 0; row < WORLD_HEIGHT; row++)
                {
                    ImGui::TableNextRow();
                    for (int column = 0; column < WORLD_WIDTH; column++)
                    {
                        ImGui::TableSetColumnIndex(column);
                        ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f));
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);

                        ImGui::Text("%d,%d = %d", Int_To_Co_Ordinates(index).X, Int_To_Co_Ordinates(index).Y, index);
                        index++;
                    }
                }
                ImGui::EndTable();
            }
        }



        ImGui::PopStyleVar();
        ImGui::End();
    }
}
