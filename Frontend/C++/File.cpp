#include <iostream>
#include <fstream>
#include <windows.h>
#include <iostream>
#include <shobjidl.h>


// Thank you https ://rextester.com/SLYX81102 
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

/*
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow, std::wstring* str_save_to)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    
                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        *str_save_to = pszFilePath;
                        //MessageBoxW(NULL, *str_save_to, L"File Path", MB_OK);
                        
                        CoTaskMemFree(pszFilePath);

                       
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return 0;
}
*/
