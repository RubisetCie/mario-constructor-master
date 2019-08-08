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

#include "../Headers/Core/gscene.hpp"
#include "../Headers/gglobals.hpp"

using namespace sf;
using namespace std;

bool CheckMutex(HANDLE* mutex, const string& uid);
bool InitSFML();
bool InitFMOD();

RenderTexture mainTexture;
Sprite textureRender;

RenderWindow* mainWindow;
Texture* loadingTex;
Sprite* loading;

FMOD_SYSTEM* soundSystem;
FMOD_SOUND* music;
FMOD_CHANNEL* musicChannel;

Scenes targetScene;

string fileToLoad = "";
string scenarioToLoad = "";
char loadingType = 0;

float windowScale = 1;

Vector2f wpos;

bool enableShaders = true;

int WinMain(HINSTANCE thisInstance, HINSTANCE, LPSTR args, int showMode)
{
    HANDLE hMutex;

    if (!CheckMutex(&hMutex, "Global\\{8bd777b9-b2a1-4d17-9ce5-082b9f08b026}"))
        return 1;

    keybindings[0] = GetPrivateProfileInt("INPUT KEY BINDINGS", "left", 71, ".\\inputParams.ini");
    keybindings[1] = GetPrivateProfileInt("INPUT KEY BINDINGS", "right", 72, ".\\inputParams.ini");
    keybindings[2] = GetPrivateProfileInt("INPUT KEY BINDINGS", "down", 74, ".\\inputParams.ini");
    keybindings[3] = GetPrivateProfileInt("INPUT KEY BINDINGS", "up", 73, ".\\inputParams.ini");
    keybindings[4] = GetPrivateProfileInt("INPUT KEY BINDINGS", "jump", 25, ".\\inputParams.ini");
    keybindings[5] = GetPrivateProfileInt("INPUT KEY BINDINGS", "run", 23, ".\\inputParams.ini");

    targetScene = SCENE_LOGO;

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

        delete mainWindow;

        return 1;
    }

    /*{
        FMOD_RESULT result;

        result = FMOD_System_CreateStream(soundSystem,
                                "Data/Sfx/Musics/TitleScreen.mp3",
                                FMOD_LOOP_NORMAL |
                                FMOD_SOFTWARE |
                                FMOD_2D,
                                NULL,
                                &music);

        if (result != FMOD_OK)
            return false;
    }*/

    srand(time(NULL));

    while (mainWindow->isOpen())
    {
        switch(targetScene)
        {
            case SCENE_LOGO         : Scene::Logo(); break;
            case SCENE_TITLE        : Scene::Title(); break;
            case SCENE_INGAME       : Scene::Ingame(); break;
        }
    }

    delete mainWindow;
    delete loadingTex;
    delete loading;

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
    Image icon, loadingImg;
    ContextSettings settings;

    int value[3];
    char title[128];
    TCHAR filePath[MAX_PATH];

    value[0] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "vsync", 1, ".\\contextParams.ini");
    value[1] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "filter", 0, ".\\contextParams.ini");

    if (GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "shaders", 1, ".\\contextParams.ini") == 0)
        enableShaders = false;

    value[2] = GetPrivateProfileInt("GAME CONFIGURATION", "playable_type", 0, ".\\game.ini");

    GetPrivateProfileString("GAME CONFIGURATION", "playable_file", "", filePath, MAX_PATH, ".\\game.ini");
    GetPrivateProfileString("GAME CONFIGURATION", "title", "Game Title", title, 128, ".\\game.ini");

    #ifdef DEBUGMODE
    cout << filePath << endl;
    #endif // DEBUGMODE

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

    mainTexture.create(640, 480);

    if (value[1] == 1)
        mainTexture.setSmooth(true);

    textureRender.setTexture(mainTexture.getTexture());

    mainWindow = new RenderWindow(VideoMode(640, 480, 32),
                                title,
                                Style::Default,
                                settings);

    mainWindow->setIcon(48, 48, icon.getPixelsPtr());
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

    loading = new Sprite(*loadingTex);
    loading->setPosition(524, 462);

    if (title[0] == '\0')
    {
        MessageBox(NULL, "Error ! No playable has been specified !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
        return false;
    }

    ifstream testFile;

    switch (value[2])
    {
        case 0 :
            testFile.open(filePath, ios::binary);

            if (!testFile.good())
            {
                MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                return false;
            }
            else
            {
                if (checkLoadResources(testFile, filePath))
                {
                    fileToLoad = filePath;
                    loadingType = 0;

                    testFile.close();
                }
            }
            break;
        case 1 :
            testFile.open(filePath, ios::binary);

            if (!testFile.good())
            {
                MessageBox(NULL, "Failed to open the Scenario file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                return false;
            }
            else
            {
                char CMSid[5];
                bool correct = true;

                testFile.read(CMSid, 5);

                if (CMSid[0] != 'C' || CMSid[1] != 'M' || CMSid[2] != 'S')
                {
                    MessageBox(NULL, "Error ! This file is not a valid CMS Scenario !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

                    correct = false;
                }

                if (CMSid[3] > EDITOR_VERSION)
                {
                    MessageBox(NULL, "Error ! This Scenario was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

                    correct = false;
                }

                if (correct)
                {
                    scenarioToLoad = filePath;
                    loadingType = CMSid[4] + 1;
                }

                testFile.close();

                if (!correct)
                    return false;
            }
            break;
        case 2 :
            testFile.open(filePath, ios::binary);

            if (!testFile.good())
            {
                MessageBox(NULL, "Failed to open the World file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                return false;
            }
            else
            {
                char CMWid[4];
                bool correct = true;

                testFile.read(CMWid, 4);

                if (CMWid[0] != 'C' || CMWid[1] != 'M' || CMWid[2] != 'W')
                {
                    MessageBox(NULL, "Error ! This file is not a valid CMW World !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

                    correct = false;
                }

                if (CMWid[3] > EDITOR_VERSION)
                {
                    MessageBox(NULL, "Error ! This World was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

                    correct = false;
                }

                if (correct)
                {
                    scenarioToLoad = filePath;
                    loadingType = 4;
                }

                testFile.close();

                if (!correct)
                    return false;
            }
            break;
    }

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
