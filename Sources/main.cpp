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
    #include <libgen.h>
    #endif
}

#ifdef LINUX
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#endif

#include "../Headers/Core/scene.hpp"
#include "../Headers/globals.hpp"

using namespace sf;
using namespace std;

#ifndef LINUX
bool CheckMutex(HANDLE* mutex, const string& uid);
#endif
bool InitSFML();
bool InitFMOD();

#ifndef LINUX
LRESULT CALLBACK dialogProcOpenLevel(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
#endif

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
bool showCursor;

char mousePressed = 0;
char rightPressed = 0;
char middlPressed = 0;

char procPath[MAX_PATH];

#ifndef LINUX
HINSTANCE* mainInstance;
NOTIFYICONDATA trayIcon;

COLORREF colorArray[16] = {0};

int WinMain(HINSTANCE thisInstance, HINSTANCE, LPSTR args, int showMode)
{
    HANDLE hMutex;

    if (!CheckMutex(&hMutex, "Global\\{d294751d-86f3-4dd8-bd58-9c08b3de524d}"))
        return 1;

    mainInstance = &thisInstance;
#else
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
#endif
    // Compute the process path
#ifndef LINUX
    GetModuleFileName(NULL, procPath, MAX_PATH);
    PathRemoveFileSpec(procPath);

    if (args[0] != 0)
    {
        ifstream levelFile;

        TCHAR filename[MAX_PATH];
        TCHAR filePath[MAX_PATH];
        TCHAR resourceURL[MAX_PATH];
        TCHAR getString[MAX_PATH];

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
#else
    getcwd(procPath, MAX_PATH);

    if (argc > 1)
    {
        ifstream levelFile;

        char filePath[MAX_PATH];
        char resourceURL[MAX_PATH];
        char getString[MAX_PATH];

        levelFile.open(argv[1], ios::binary);
#endif

        if (!levelFile.good())
        {
#ifndef LINUX
            MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the Level file !"), QMessageBox::Ok);
            messageBox.exec();
#endif
            return 1;
        }

        char CMLid[4];
        levelFile.read(CMLid, 4);

        if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
        {
#ifndef LINUX
            MessageBox(NULL, "This file is not a valid CML Level !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CML Level !"), QMessageBox::Ok);
            messageBox.exec();
#endif
            levelFile.close();
            return 1;
        }

        if (CMLid[3] != EDITOR_VERSION)
        {
#ifndef LINUX
            MessageBox(NULL, "This Level was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This Level was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
            messageBox.exec();
#endif
            levelFile.close();
            return 1;
        }

#ifndef LINUX
        strcpy(filePath, filename);
        PathRemoveFileSpec(filePath);
        SetCurrentDirectory(filePath);
#else
        strcpy(filePath, argv[1]);
        dirname(filePath);
        chdir(filePath);
#endif
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
#ifndef LINUX
                    TCHAR bufferPath[MAX_PATH];
                    PathCanonicalize(bufferPath, getString);
                    sprintf(resourceURL, "%s\\%s", filePath, bufferPath);
#else
                    sprintf(resourceURL, "%s\\%s", filePath, getString);
#endif
                }
                else
                    strcpy(resourceURL, getString);

                checkFile.open(resourceURL);

                if (!checkFile.good())
                {
#ifndef LINUX
                    TCHAR messageText[512];
                    sprintf(messageText, "The resource is not found :\n%s !", getString);
                    MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Resource not found !"), QString("The resource is not found :\n%1 !").arg(getString), QMessageBox::Ok);
                    messageBox.exec();
#endif
                    levelFile.close();
                    return 1;
                }
                else
                    checkFile.close();
            }
        }

        levelFile.close();
#ifndef LINUX
        fileToLoad = filename;
#else
        fileToLoad = argv[1];
#endif

#ifndef LINUX
        // Reset the current directory :
        SetCurrentDirectory(procPath);
#else
        // Reset the current directory :
        chdir(procPath);
#endif
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

#ifndef LINUX
    {
        ifstream colorFile;

        colorFile.open("Data/Save/Colors.dat", ios::binary);

        if (colorFile.good())
        {
            colorFile.read(reinterpret_cast<char*>(colorArray), sizeof(COLORREF) * 16);
            colorFile.close();
        }
    }
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

#ifdef DEBUGMODE
    targetScene = SCENE_EDITOR;
#else
    if (fileToLoad.empty())
        targetScene = SCENE_LOGO;
    else
    {
#ifndef LINUX
        DialogBox(*mainInstance, MAKEINTRESOURCE(49), NULL, reinterpret_cast<DLGPROC>(dialogProcOpenLevel));
#else
        QMessageBox messageBox;
        messageBox.setWindowTitle(QStringLiteral("Open a Level"));
        messageBox.setText(QStringLiteral("What do you want to do with this Level ?"));
        QAbstractButton* buttonEdit = messageBox.addButton(QStringLiteral("Edit"), QMessageBox::AcceptRole);
        QAbstractButton* buttonPlay = messageBox.addButton(QStringLiteral("Play"), QMessageBox::AcceptRole);
        messageBox.exec();
        if (messageBox.clickedButton() == buttonEdit)
            targetScene = SCENE_EDITOR;
        else if (messageBox.clickedButton() == buttonPlay)
            targetScene = SCENE_INGAME;
#endif
    }
#endif

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
#ifndef LINUX
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
#endif
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
#ifndef LINUX
    Shell_NotifyIcon(NIM_DELETE, &trayIcon);
#endif
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
    int value[2];
    value[0] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "vsync", 1, ".\\contextParams.ini");
    value[1] = GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "filter", 0, ".\\contextParams.ini");

    if (GetPrivateProfileInt("CONTEXT INITIALIZATION PARAMETERS", "shaders", 1, ".\\contextParams.ini") == 0)
        enableShaders = false;
#else
    QSettings ctxsets("./contextParams.ini", QSettings::IniFormat);
    ctxsets.beginGroup("CONTEXT INITIALIZATION PARAMETERS");

    if (ctxsets.value("shaders", QVariant(1)).toInt() == 0)
        enableShaders = false;
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
                                "Mario Constructor Master",
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

#ifndef LINUX
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
#endif
