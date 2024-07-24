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
    #include <fmod.h>
    #ifndef LINUX
    #include <windows.h>
    #include <shlwapi.h>
    #else
    #include <fmodlinux.h>
    #include <unistd.h>
    #endif
}

#ifdef LINUX
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#endif

#include "../Headers/Core/gscene.hpp"
#include "../Headers/gglobals.hpp"

using namespace sf;
using namespace std;

#ifndef LINUX
bool CheckMutex(HANDLE* mutex, const string& uid);
#endif
bool InitSFML();
bool InitFMOD();

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
unsigned char target_lives = -1;

float windowScale = 1;

Vector2f wpos;
Vector2i mpos_absolute;

bool enableShaders = true;
bool showCursor;

char procPath[MAX_PATH];

#ifndef LINUX
int WinMain(HINSTANCE thisInstance, HINSTANCE, LPSTR args, int showMode)
{
    HANDLE hMutex;

    if (!CheckMutex(&hMutex, "Global\\{8bd777b9-b2a1-4d17-9ce5-082b9f08b026}"))
        return 1;

    // Compute the process path
    GetModuleFileName(NULL, procPath, MAX_PATH);
    PathRemoveFileSpec(procPath);
#else
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Compute the process path
    getcwd(procPath, MAX_PATH);
#endif
#ifndef LINUX
    keybindings[0] = GetPrivateProfileInt("INPUT KEY BINDINGS", "left", 71, ".\\inputParams.ini");
    keybindings[1] = GetPrivateProfileInt("INPUT KEY BINDINGS", "right", 72, ".\\inputParams.ini");
    keybindings[2] = GetPrivateProfileInt("INPUT KEY BINDINGS", "down", 74, ".\\inputParams.ini");
    keybindings[3] = GetPrivateProfileInt("INPUT KEY BINDINGS", "up", 73, ".\\inputParams.ini");
    keybindings[4] = GetPrivateProfileInt("INPUT KEY BINDINGS", "jump", 25, ".\\inputParams.ini");
    keybindings[5] = GetPrivateProfileInt("INPUT KEY BINDINGS", "run", 23, ".\\inputParams.ini");
#else
    {
        QSettings settings("./inputParams.ini", QSettings::IniFormat);
        settings.beginGroup("INPUT KEY BINDINGS");
        keybindings[0] = settings.value("left", QVariant(72)).toInt();
        keybindings[1] = settings.value("right", QVariant(72)).toInt();
        keybindings[2] = settings.value("down", QVariant(74)).toInt();
        keybindings[3] = settings.value("up", QVariant(73)).toInt();
        keybindings[4] = settings.value("jump", QVariant(25)).toInt();
        keybindings[5] = settings.value("run", QVariant(23)).toInt();
    }
#endif

    targetScene = SCENE_LOGO;

    if (!InitSFML())
    {
#ifndef LINUX
        CloseHandle(hMutex);
#endif
        return 1;
    }

    if (!InitFMOD())
    {
#ifndef LINUX
        CloseHandle(hMutex);
#endif
        delete loadingTex;
        delete loading;
        delete cursorTex;
        delete cursor;

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

    delete loadingTex;
    delete loading;
    delete cursorTex;
    delete cursor;

    delete mainWindow;

    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(musicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(musicChannel);
    }

    FMOD_Sound_Release(music);
#ifndef LINUX
    // Causes crash under Linux, at least with the last version of FModEx API...
    // The system will handle the freeing on this one.
    FMOD_System_Release(soundSystem);
#endif

#ifndef LINUX
    CloseHandle(hMutex);
#endif

    return 0;
}

#ifndef LINUX
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
#endif

bool InitSFML()
{
    Image icon, cursorImg, loadingImg;
    ContextSettings settings;

#ifndef LINUX
    int value[3];
    char title[128];
    TCHAR filePath[MAX_PATH];

    value[0] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "vsync", 1, ".\\contextParams.ini");
    value[1] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "filter", 0, ".\\contextParams.ini");

    if (GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "shaders", 1, ".\\contextParams.ini") == 0)
        enableShaders = false;

    value[2] = GetPrivateProfileInt("GAME CONFIGURATION", "playable_type", 0, ".\\game.ini");

    target_lives = GetPrivateProfileInt("GAME CONFIGURATION", "lives", 4, ".\\game.ini");

    GetPrivateProfileString("GAME CONFIGURATION", "playable_file", "", filePath, MAX_PATH, ".\\game.ini");
    GetPrivateProfileString("GAME CONFIGURATION", "title", "Game Title", title, 128, ".\\game.ini");
#else
    QSettings ctxsets("./contextParams.ini", QSettings::IniFormat);
    QSettings gamesets("./game.ini", QSettings::IniFormat);
    ctxsets.beginGroup("CONTEXT INITIALIZATION PARAMETERS");
    gamesets.beginGroup("GAME CONFIGURATION");

    if (ctxsets.value("shaders", QVariant(1)).toInt() == 0)
        enableShaders = false;

    const char *filePath = gamesets.value("playable_file", QVariant("")).toString().toUtf8().constData();
    target_lives = gamesets.value("lives", QVariant(4)).toUInt();
#endif

#ifdef DEBUGMODE
    cout << filePath << endl;
#endif

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
#ifndef LINUX
        MessageBox(NULL, "Failed to load the icon !\nCheck this file : Data/Gfx/Icon.bmp", "Loading error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Loading error !"), QStringLiteral("Failed to load the icon !\nCheck this file : Data/Gfx/Icon.bmp"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    icon.createMaskFromColor(Color::Magenta);

    if (!loadingImg.loadFromFile("Data/Gfx/Loading.bmp"))
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to load the loading image !\nCheck this file : Data/Gfx/Loading.bmp", "Loading error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Loading error !"), QStringLiteral("Failed to load the loading image !\nCheck this file : Data/Gfx/Loading.bmp"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    loadingImg.createMaskFromColor(Color::Magenta);

    if (!cursorImg.loadFromFile("Data/Gfx/Cursor.png"))
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to load the cursor !\nCheck this file : Data/Gfx/Cursor.png", "Loading error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Loading error !"), QStringLiteral("Failed to load the cursor !\nCheck this file : Data/Gfx/Cursor.png"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    cursorImg.createMaskFromColor(Color::Magenta);

    mainTexture.create(640, 480);

#ifndef LINUX
    if (value[1] != 0)
#else
    if (ctxsets.value("filter", QVariant(0)).toInt() != 0)
#endif
        mainTexture.setSmooth(true);

    textureRender.setTexture(mainTexture.getTexture());

    mainWindow = new RenderWindow(VideoMode(640, 480, 32),
#ifndef LINUX
                                title,
#else
                                gamesets.value("title", QVariant("Game Title")).toString().toUtf8().constData(),
#endif
                                Style::Default,
                                settings);

    mainWindow->setIcon(32, 32, icon.getPixelsPtr());
#ifndef DEBUGMODE
#ifndef LINUX
    showCursor = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "cursor", 0, ".\\contextParams.ini") != 0;
#else
    showCursor = ctxsets.value("cursor", QVariant(0)).toInt() != 0;
#endif
    mainWindow->setMouseCursorVisible(showCursor);
#endif
    mainWindow->setKeyRepeatEnabled(false);

    if (mainWindow == NULL)
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to create the Render Window !", "SFML error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("SFML error !"), QStringLiteral("Failed to create the Render Window !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

#ifndef DEBUGMODE
#ifndef LINUX
    if (value[0] != 0)
#else
    if (ctxsets.value("vsync", QVariant(1)).toInt() != 0)
#endif
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

    /*if (title[0] == '\0')
    {
        MessageBox(NULL, "No playable has been specified !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
        return false;
    }*/

    ifstream testFile;

#ifndef LINUX
    switch (value[2])
#else
    switch (gamesets.value("playable_type", QVariant(0)).toInt())
#endif
    {
        case 0 :
            testFile.open(filePath, ios::binary);

            if (!testFile.good())
            {
#ifndef LINUX
                MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the Level file !"), QMessageBox::Ok);
                messageBox.exec();
#endif
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
#ifndef LINUX
                MessageBox(NULL, "Failed to open the Scenario file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the Scenario file !"), QMessageBox::Ok);
                messageBox.exec();
#endif
                return false;
            }
            else
            {
                char CMSid[5];
                bool correct = true;

                testFile.read(CMSid, 5);

                if (CMSid[0] != 'C' || CMSid[1] != 'M' || CMSid[2] != 'S')
                {
#ifndef LINUX
                    MessageBox(NULL, "This file is not a valid CMS Scenario !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CMS Scenario !"), QMessageBox::Ok);
                    messageBox.exec();
#endif
                    correct = false;
                }

                if (CMSid[3] > EDITOR_VERSION)
                {
#ifndef LINUX
                    MessageBox(NULL, "This Scenario was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This Scenario was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
                    messageBox.exec();
#endif
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
#ifndef LINUX
                MessageBox(NULL, "Failed to open the World file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the World file !"), QMessageBox::Ok);
                messageBox.exec();
#endif
                return false;
            }
            else
            {
                char CMWid[4];
                bool correct = true;

                testFile.read(CMWid, 4);

                if (CMWid[0] != 'C' || CMWid[1] != 'M' || CMWid[2] != 'W')
                {
#ifndef LINUX
                    MessageBox(NULL, "This file is not a valid CMW World !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CMW World !"), QMessageBox::Ok);
                    messageBox.exec();
#endif
                    correct = false;
                }

                if (CMWid[3] > EDITOR_VERSION)
                {
#ifndef LINUX
                    MessageBox(NULL, "This World was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This World was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
                    messageBox.exec();
#endif
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
#ifndef LINUX
        MessageBox(NULL, "Failed to create the FMOD_System !", "FMOD Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("FMOD Error !"), QStringLiteral("Failed to create the FMOD_System !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

#ifdef LINUX
    // Set output to ALSA in Linux
    result = FMOD_System_SetOutput(soundSystem, FMOD_OUTPUTTYPE_ALSA);

    if (result != FMOD_OK)
    {
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("FMOD Error !"), QStringLiteral("Failed to set the FMOD output mode !"), QMessageBox::Ok);
        messageBox.exec();
    }
#endif

#ifdef LINUX
    // Set output to ALSA in Linux
    result = FMOD_System_SetOutput(soundSystem, FMOD_OUTPUTTYPE_ALSA);

    if (result != FMOD_OK)
    {
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("FMOD Error !"), QStringLiteral("Failed to set the FMOD output mode !"), QMessageBox::Ok);
        messageBox.exec();
    }

#pragma pack(push, n)
    FMOD_LINUX_EXTRADRIVERDATA driverData;
    driverData.output_driver_arguments = 0;
    driverData.record_driver_arguments = 0;
#pragma pack(pop)
    result = FMOD_System_Init(soundSystem,
                              21,
                              FMOD_INIT_NORMAL,
                              &driverData);
#else
    result = FMOD_System_Init(soundSystem,
                              21,
                              FMOD_INIT_NORMAL,
                              NULL);
#endif
    if (result != FMOD_OK)
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to initialize the FMOD_System !", "FMOD Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("FMOD Error !"), QStringLiteral("Failed to initialize the FMOD_System !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    return true;
}
