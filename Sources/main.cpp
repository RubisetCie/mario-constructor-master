/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <ctime>
#include <fstream>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

extern "C"
{
    #include <FMODEx/fmod.h>
    #include <windows.h>
    #include <shlwapi.h>
}

#include "../Headers/Core/scene.hpp"
#include "../Headers/globals.hpp"

using namespace sf;
using namespace std;

bool CheckMutex(HANDLE* mutex, const string& uid);
bool InitSFML();
bool InitFMOD();
LRESULT CALLBACK dialogProcOpenLevel(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

RenderTexture mainTexture;
Sprite textureRender;

RenderWindow* mainWindow;
Texture* loadingTex;
Texture* cursorTex;
Sprite* loading;
Sprite* cursor;

FMOD_SYSTEM* soundSystem;
FMOD_SOUND* music;
FMOD_CHANNEL* musicChannel;

Scenes targetScene;

string fileToLoad = "";
string scenarioToLoad = "";
char loadingType = 0;

float windowScale = 1;

Vector2f wpos;
Vector2f mpos_relative;
Vector2i mpos_absolute;

bool editorMusic = true;
bool editorMouseScrolling = true;
bool enableShaders = true;

char mousePressed = 0;
char rightPressed = 0;
char middlPressed = 0;

HINSTANCE* mainInstance;
NOTIFYICONDATA trayIcon;

COLORREF colorArray[16] = {0};

int WinMain(HINSTANCE thisInstance, HINSTANCE, LPSTR args, int showMode)
{
    HANDLE hMutex;

    if (!CheckMutex(&hMutex, "Global\\{d294751d-86f3-4dd8-bd58-9c08b3de524d}"))
        return 1;

    mainInstance = &thisInstance;

    if (args[0] != 0)
    {
        ifstream levelFile;

        TCHAR filename[MAX_PATH];
        TCHAR filePath[MAX_PATH];
        TCHAR resourceURL[MAX_PATH];
        TCHAR getString[MAX_PATH];

        char CMLid[4];

        register unsigned int k = 0;
        for (register unsigned int i = 0; i < MAX_PATH; i++)
        {
            if (args[i] != '"')
            {
                filename[k] = args[i];
                k++;
            }
        }

        levelFile.open(filename, ios::binary);

        if (!levelFile.good())
        {
            MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
            return 1;
        }

        levelFile.read(CMLid, 4);

        if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
        {
            MessageBox(NULL, "Error ! This file is not a valid CML Level !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

            levelFile.close();

            return 1;
        }

        if (CMLid[3] > EDITOR_VERSION)
        {
            MessageBox(NULL, "Error ! This Level was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

            levelFile.close();

            return 1;
        }

        strcpy(filePath, filename);

        PathRemoveFileSpec(filePath);

        for (register unsigned int i = 0; i < 5; i++)
        {
            ifstream checkFile;

            levelFile.read(getString, 1);

            if (getString[0] != '\0')
            {
                levelFile.seekg(-1, ios::cur);

                for (register unsigned int j = 0; true; j++)
                {
                    levelFile.read(&getString[j], 1);

                    if (getString[j] == '\0')
                        break;
                }

                if (getString[0] == '.')
                {
                    TCHAR bufferPath[MAX_PATH];

                    PathCanonicalize(bufferPath, getString);

                    sprintf(resourceURL, "%s\\%s", filePath, bufferPath);
                }
                else
                    strcpy(resourceURL, getString);

                checkFile.open(resourceURL);

                if (!checkFile.good())
                {
                    TCHAR messageText[512];

                    sprintf(messageText, "Error ! The resource is not found :\n%s !", getString);

                    MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONERROR | MB_OK);

                    levelFile.close();

                    return 1;
                }
                else
                    checkFile.close();
            }
        }

        levelFile.close();

        fileToLoad = filename;

        // Reset the current directory :
        GetModuleFileName(NULL, getString, MAX_PATH);
        PathRemoveFileSpec(getString);
        SetCurrentDirectory(getString);
    }

    {
        ifstream parmFile;

        parmFile.open("Data/Save/Parms.dat");

        if (parmFile.good())
        {
            parmFile >> windowScale;
            parmFile >> editorMusic;
            parmFile >> editorMouseScrolling;

            parmFile.close();
        }
    }

    {
        ifstream colorFile;

        colorFile.open("Data/Save/Colors.dat", ios::binary);

        if (colorFile.good())
        {
            colorFile.read(reinterpret_cast<char*>(colorArray), sizeof(COLORREF) * 16);

            colorFile.close();
        }
    }

    keybindings[0] = GetPrivateProfileInt("INPUT KEY BINDINGS", "left", 71, ".\\inputParams.ini");
    keybindings[1] = GetPrivateProfileInt("INPUT KEY BINDINGS", "right", 72, ".\\inputParams.ini");
    keybindings[2] = GetPrivateProfileInt("INPUT KEY BINDINGS", "down", 74, ".\\inputParams.ini");
    keybindings[3] = GetPrivateProfileInt("INPUT KEY BINDINGS", "up", 73, ".\\inputParams.ini");
    keybindings[4] = GetPrivateProfileInt("INPUT KEY BINDINGS", "jump", 25, ".\\inputParams.ini");
    keybindings[5] = GetPrivateProfileInt("INPUT KEY BINDINGS", "run", 23, ".\\inputParams.ini");

    #ifdef DEBUGMODE
    targetScene = SCENE_EDITOR;
    #else
    if (fileToLoad.empty())
        targetScene = SCENE_LOGO;
    else
        DialogBox(*mainInstance, MAKEINTRESOURCE(49), NULL, reinterpret_cast<DLGPROC>(dialogProcOpenLevel));
    #endif

    if (!InitSFML())
    {
        CloseHandle(hMutex);

        return 1;
    }

    if (!InitFMOD())
    {
        CloseHandle(hMutex);

        delete loadingTex;
        delete loading;
        delete cursorTex;
        delete cursor;

        delete mainWindow;

        return 1;
    }

    ZeroMemory(&trayIcon, sizeof(NOTIFYICONDATA));

    trayIcon.cbSize = sizeof(NOTIFYICONDATA);
    trayIcon.hWnd = mainWindow->getSystemHandle();
    trayIcon.uFlags = NIF_ICON | NIF_TIP | NIF_INFO;
    trayIcon.uTimeout = 8000;
    trayIcon.uID = 1;
    trayIcon.uVersion = NOTIFYICON_VERSION;
    trayIcon.hIcon = LoadIcon(*mainInstance, MAKEINTRESOURCE(1));
    trayIcon.dwInfoFlags = NIIF_ERROR;

    StrCpy(trayIcon.szTip, "Mario Constructor Master");

    Shell_NotifyIcon(NIM_ADD, &trayIcon);

    srand(time(NULL));

    while (mainWindow->isOpen())
    {
        switch(targetScene)
        {
            case SCENE_LOGO         : Scene::Logo(); break;
            case SCENE_TITLE        : Scene::Title(); break;
            case SCENE_EDITOR       : Scene::Editor(); break;
            case SCENE_WORLDEDITOR  : Scene::WorldEditor(); break;
            case SCENE_INGAME       : Scene::Ingame(); break;
        }
    }

    Shell_NotifyIcon(NIM_DELETE, &trayIcon);

    delete mainWindow;
    delete loadingTex;
    delete loading;
    delete cursorTex;
    delete cursor;

    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(musicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(musicChannel);
    }

    FMOD_Sound_Release(music);
    FMOD_System_Release(soundSystem);

    CloseHandle(hMutex);

    return 0;
}

bool CheckMutex(HANDLE* mutex, const string& uid)
{
    DWORD error;

    (*mutex) = CreateMutex(NULL, FALSE, uid.c_str());
    error = GetLastError();

    if (error)
    {
        MessageBox(NULL, "The game is already running.\nPlease, close the previous game before.", "Game already run", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    return true;
}

bool InitSFML()
{
    Image icon, cursorImg, loadingImg;
    ContextSettings settings;

    int value[2];

    value[0] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "vsync", 1, ".\\contextParams.ini");
    value[1] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "filter", 0, ".\\contextParams.ini");

    if (GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "shaders", 1, ".\\contextParams.ini") == 0)
        enableShaders = false;

    settings.antialiasingLevel = 0;

    #ifdef DEBUGMODE
        settings.attributeFlags = ContextSettings::Debug;
    #else
        settings.attributeFlags = ContextSettings::Default;
    #endif

    settings.majorVersion = 2;
    settings.minorVersion = 0;
    settings.stencilBits = 0;

    if (!icon.loadFromFile("Data/Gfx/Icon.bmp"))
    {
        MessageBox(NULL, "Failed to load the icon !\nCheck this file : Data/Gfx/Icon.bmp", "Loading error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    icon.createMaskFromColor(Color::Magenta);

    if (!loadingImg.loadFromFile("Data/Gfx/Loading.bmp"))
    {
        MessageBox(NULL, "Failed to load the loading image !\nCheck this file : Data/Gfx/Loading.bmp", "Loading error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    loadingImg.createMaskFromColor(Color::Magenta);

    if (!cursorImg.loadFromFile("Data/Gfx/Cursor.png"))
    {
        MessageBox(NULL, "Failed to load the cursor !\nCheck this file : Data/Gfx/Cursor.png", "Loading error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    cursorImg.createMaskFromColor(Color::Magenta);

    mainTexture.create(640, 480);

    if (value[1] == 1)
        mainTexture.setSmooth(true);

    textureRender.setTexture(mainTexture.getTexture());

    mainWindow = new RenderWindow(VideoMode(640, 480, 32),
                                "Mario Constructor Master",
                                Style::Default,
                                settings);

    mainWindow->setIcon(32, 32, icon.getPixelsPtr());
    #ifndef DEBUGMODE
    mainWindow->setMouseCursorVisible(false);
    #endif // DEBUGMODE
    mainWindow->setKeyRepeatEnabled(false);

    if (mainWindow == NULL)
    {
        MessageBox(NULL, "Failed to create the Render Window !", "SFML error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    #ifndef DEBUGMODE
    if (value[0])
        mainWindow->setVerticalSyncEnabled(true);
    else
        mainWindow->setFramerateLimit(60);
    #else
    mainWindow->setFramerateLimit(60);
    #endif

    {
        Vector2i winPos(mainWindow->getPosition() - Vector2i(160 * windowScale, 120 * windowScale));
        Vector2u newSize(640 * windowScale, 480 * windowScale);
        wpos = Vector2f((newSize.x - (windowScale * 640)) / 2, (newSize.y - (windowScale * 480)) / 2);

        mainWindow->setSize(newSize);
        mainWindow->setView(View(FloatRect(0, 0, newSize.x, newSize.y)));

        textureRender.setPosition(wpos);
        textureRender.setScale(windowScale, windowScale);

        if (winPos.x < 16)
            winPos.x = 16;

        if (winPos.y < 16)
            winPos.y = 16;

        mainWindow->setPosition(winPos);
    }

    loadingTex = new Texture;
    loadingTex->loadFromImage(loadingImg);

    cursorTex = new Texture;
    cursorTex->loadFromImage(cursorImg);

    loading = new Sprite(*loadingTex);
    loading->setPosition(524, 462);

    cursor = new Sprite(*cursorTex);

    return true;
}

bool InitFMOD()
{
    FMOD_RESULT result;

    result = FMOD_System_Create(&soundSystem);

    if (result != FMOD_OK)
    {
        MessageBox(NULL, "Failed to create the FMOD_System !", "FMOD Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    result = FMOD_System_Init(soundSystem,
                              21,
                              FMOD_INIT_NORMAL,
                              NULL);

    if (result != FMOD_OK)
    {
        MessageBox(NULL, "Failed to initialize the FMOD_System !", "FMOD Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return false;
    }

    return true;
}

LRESULT CALLBACK dialogProcOpenLevel(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_COMMAND :
            switch(wParam)
            {
                case 52 :
                    EndDialog(windowHandle, 0);

                    targetScene = SCENE_EDITOR;

                    return TRUE;
                case 53 :
                    EndDialog(windowHandle, 0);

                    targetScene = SCENE_INGAME;

                    return TRUE;
            }

            break;
    }

    return FALSE;
}
