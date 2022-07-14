#include "FF_UI.h"
#include "imgui.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


#define         	MINT_CONFIG_FILE_PATH       	"../../../../API/config.mints"
//#define         	FAC_CONFIG_FILE_PATH       		"Faction.fucks"



unsigned _int8  WORLD_WIDTH;
unsigned _int8  WORLD_HEIGHT;
unsigned short  WORLD_SIZE;
static unsigned int times_rendered = 0;





namespace Faction_Fight_UI
{

    static unsigned __int8 WORLD_WIDTH;
    bool    alread_read_mints = false;
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
    


    void get_set_ints_from_config(bool* read)
    {
        if (*read == false)
        {
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

                for (int i = 0; i < 10; i++)
                {
                    if (config)
                    {
                        break; // yay! it worked 
                    }
                    else
                    {
                        printf("\nAh fiddlestciks! We couldn't open the mints file \ntrying to open file again! \nWish us luck");
                        //system("dir ..\\..\\..\\..\\ ");
                        config.open(MINT_CONFIG_FILE_PATH);

                    }//end else


                }//end child if


                if (!config) 	// is it still not working??? 
                {
                    exit(4);
                }//end child if					

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

                    case world_width:
                        WORLD_WIDTH = (unsigned _int8)stoi(temp_string);
                        extract_next_line = false;
                        break;


                    case world_height:
                        WORLD_HEIGHT = (unsigned _int8)stoi(temp_string);
                        extract_next_line = false;
                        break;


                    default:
                        printf("\nERROR IN get_set_ints_from_config SWITCH PART");
                        break;

                    }//end switch																				

                }//end if-line extraction																		



                //	check to see if it's a comment first														
                if (temp_string[0] == '#')
                {
                    continue;
                }


                //	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	//			
                //								HAVE WE FOUND SOMETHING?							//			
                //	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	

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

            }//end while																						


            config.close();

            *read = true;
            WORLD_SIZE = WORLD_HEIGHT * WORLD_WIDTH;

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

        get_set_ints_from_config(&alread_read_mints);

        if (times_rendered > 5000)
        {
            times_rendered = 0;
            alread_read_mints = false;
        }

        ImGuiWindowFlags Main_Window_Flags = 0;

        if (true)       Main_Window_Flags |= ImGuiWindowFlags_MenuBar;
        


        if (show_app_metrics)       { ImGui::ShowMetricsWindow(&show_app_metrics); }
        if (show_app_stack_tool)    { ImGui::ShowStackToolWindow(&show_app_stack_tool); }
        if (show_app_about)         { ImGui::ShowAboutWindow(&show_app_about); }

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


        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                                        

        ImGui::Text("Hello! Your width and height is %d and %d", WORLD_WIDTH, WORLD_HEIGHT);


        static ImVec2 cell_padding(3.0f, 6.0f);
        static bool show_widget_frame_bg = true;

        ImGui::SliderFloat2("CellPadding", &cell_padding.x, 0.0f, 10.0f, "%.0f");
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

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



        ImGui::PopStyleVar();
        ImGui::End();
    }
}
