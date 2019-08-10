/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <fstream>

#ifdef DEBUGMODE
#include <iostream>
#endif

extern "C"
{
    #include <FMODEx/fmod.h>
    #include <windows.h>
    #include <shlwapi.h>
    #include <shlobj.h>
}

#include "../../Headers/globals.hpp"
#include "../../Headers/Graphics/emitter.hpp"
#include "../../Headers/Sceneries/scenery_bush.hpp"

#define FADESPEED 4
#define SIZESELECTED 0.85

using namespace sf;
using namespace std;

static bool InitAssets();
static void UpdateAssets();
static void LoadTilemap();
static void LoadBackgrounds();
static bool RewriteParameters();

LRESULT CALLBACK dialogNumLevels(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK dialogNumLifes(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

VertexArray* tileMap;
VertexArray* gradient[4];

Texture* tileSet;
Texture* titleTex;
Texture* buttonTex[6];
Texture* buttonEdTex[4];
Texture* buttonScTex[6];
Texture* buttonPlTex[4];
Texture* buttonOpTex[8];
Texture* buttonRetTex;
Texture* particle;
Texture* bush;

Sprite* title;
Sprite* button[6];
Sprite* buttonEd[4];
Sprite* buttonSc[6];
Sprite* buttonPl[4];
Sprite* buttonOp[10];
Sprite* buttonRet;

Emitter* titleShine;

Scenery_Bush* greenBush;

FMOD_SOUND* mouseSound;
FMOD_SOUND* clickSound;
FMOD_SOUND* enterLevel;
FMOD_SOUND* createScenario;

bool animDone;
bool menuBlock;

unsigned int timeb;

float titleSpeed;
float buttonSpeed[6];

unsigned char levels;
unsigned char lifes;

extern Uint8 fadeAlpha;
extern RectangleShape* fadeRect;

bool buttonSelect[10];

extern Texture* circleFadeTex;
extern Sprite* circleSpr;
extern RectangleShape* blackBorder;
extern bool fadeCircle;

typedef enum Menus {MAINMENU,
                    EDITMENU,
                    CREATESCENARIOMENU,
                    PLAYSCENARIOMENU,
                    OPTIONMENU,
                    EXITTOEDITOR,
                    EXITTOPLAY,
                    EXITTOWORLD}
                    Menus;

Menus menu;

bool Scene::Title()
{
    bool exitLoop = false;

    if (!InitAssets())
    {
        MessageBox(NULL, "Failed to initialize the assets on the Title !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);

        mainWindow->close();

        exitLoop = true;
    }

    Menus menuToGo = MAINMENU;

    Event eventSystem;

    while (!exitLoop)
    {
        while (mainWindow->pollEvent(eventSystem))
        {
            switch (eventSystem.type)
            {
                case Event::Closed      :
                    exitLoop = true;

                    mainWindow->close();

                    break;

                case Event::KeyPressed  :

                    if (eventSystem.key.code == Keyboard::Escape)
                    {
                        if (fadeCircle)
                            break;

                        if (MessageBox(NULL, "Do you want to quit ?", "Quit ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
                        {
                            exitLoop = true;

                            mainWindow->close();
                        }

                        break;
                    }

                    if (!animDone)
                    {
                        timeb = 300;
                        animDone = true;

                        title->setPosition(320, 87);

                        button[0]->setPosition(148, 242);
                        button[1]->setPosition(320, 242);
                        button[2]->setPosition(492, 242);
                        button[3]->setPosition(148, 378);
                        button[4]->setPosition(320, 378);
                        button[5]->setPosition(492, 378);
                    }

                    break;

                case Event::MouseMoved  :

                    mpos_absolute = Vector2i((eventSystem.mouseMove.x - wpos.x) / windowScale, (eventSystem.mouseMove.y - wpos.y) / windowScale);

                    if (animDone && !menuBlock)
                    {
                        switch (menu)
                        {
                            case MAINMENU :

                                if (mpos_absolute.x > 74 && mpos_absolute.x < 222 && mpos_absolute.y > 192  && mpos_absolute.y < 292)
                                {
                                    if (!buttonSelect[0])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[0] = true;
                                }
                                else
                                    buttonSelect[0] = false;

                                if (mpos_absolute.x > 246 && mpos_absolute.x < 394 && mpos_absolute.y > 192  && mpos_absolute.y < 292)
                                {
                                    if (!buttonSelect[1])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[1] = true;
                                }
                                else
                                    buttonSelect[1] = false;

                                if (mpos_absolute.x > 418 && mpos_absolute.x < 562 && mpos_absolute.y > 192  && mpos_absolute.y < 292)
                                {
                                    if (!buttonSelect[2])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[2] = true;
                                }
                                else
                                    buttonSelect[2] = false;

                                if (mpos_absolute.x > 74 && mpos_absolute.x < 222 && mpos_absolute.y > 328  && mpos_absolute.y < 428)
                                {
                                    if (!buttonSelect[3])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[3] = true;
                                }
                                else
                                    buttonSelect[3] = false;

                                if (mpos_absolute.x > 246 && mpos_absolute.x < 394 && mpos_absolute.y > 328  && mpos_absolute.y < 428)
                                {
                                    if (!buttonSelect[4])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[4] = true;
                                }
                                else
                                    buttonSelect[4] = false;

                                if (mpos_absolute.x > 418 && mpos_absolute.x < 562 && mpos_absolute.y > 328  && mpos_absolute.y < 428)
                                {
                                    if (!buttonSelect[5])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[5] = true;
                                }
                                else
                                    buttonSelect[5] = false;

                                break;

                            case EDITMENU :

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 111  && mpos_absolute.y < 209)
                                {
                                    if (!buttonSelect[0])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[0] = true;
                                }
                                else
                                    buttonSelect[0] = false;

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 231  && mpos_absolute.y < 329)
                                {
                                    if (!buttonSelect[1])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[1] = true;
                                }
                                else
                                    buttonSelect[1] = false;

                                if (mpos_absolute.x > 4 && mpos_absolute.x < 78 && mpos_absolute.y > 410  && mpos_absolute.y < 476)
                                {
                                    if (!buttonSelect[7])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[7] = true;
                                }
                                else
                                    buttonSelect[7] = false;

                                break;

                            case CREATESCENARIOMENU :

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 25  && mpos_absolute.y < 135)
                                {
                                    if (!buttonSelect[0])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[0] = true;
                                }
                                else
                                    buttonSelect[0] = false;

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 145  && mpos_absolute.y < 255)
                                {
                                    if (!buttonSelect[1])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[1] = true;
                                }
                                else
                                    buttonSelect[1] = false;

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 265  && mpos_absolute.y < 375)
                                {
                                    if (!buttonSelect[2])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[2] = true;
                                }
                                else
                                    buttonSelect[2] = false;

                                if (mpos_absolute.x > 4 && mpos_absolute.x < 78 && mpos_absolute.y > 410  && mpos_absolute.y < 476)
                                {
                                    if (!buttonSelect[7])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[7] = true;
                                }
                                else
                                    buttonSelect[7] = false;

                                break;

                            case PLAYSCENARIOMENU :

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 105  && mpos_absolute.y < 215)
                                {
                                    if (!buttonSelect[0])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[0] = true;
                                }
                                else
                                    buttonSelect[0] = false;

                                if (mpos_absolute.x > 32 && mpos_absolute.x < 228 && mpos_absolute.y > 225  && mpos_absolute.y < 335)
                                {
                                    if (!buttonSelect[1])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[1] = true;
                                }
                                else
                                    buttonSelect[1] = false;

                                if (mpos_absolute.x > 4 && mpos_absolute.x < 78 && mpos_absolute.y > 410  && mpos_absolute.y < 476)
                                {
                                    if (!buttonSelect[7])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[7] = true;
                                }
                                else
                                    buttonSelect[7] = false;

                                break;

                            case OPTIONMENU :

                                if (mpos_absolute.x > 100 && mpos_absolute.x < 220 && mpos_absolute.y > 67  && mpos_absolute.y < 133)
                                {
                                    if (!buttonSelect[0])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[0] = true;
                                }
                                else
                                    buttonSelect[0] = false;

                                if (mpos_absolute.x > 260 && mpos_absolute.x < 380 && mpos_absolute.y > 67  && mpos_absolute.y < 133)
                                {
                                    if (!buttonSelect[1])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[1] = true;
                                }
                                else
                                    buttonSelect[1] = false;

                                if (mpos_absolute.x > 420 && mpos_absolute.x < 540 && mpos_absolute.y > 67  && mpos_absolute.y < 133)
                                {
                                    if (!buttonSelect[2])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[2] = true;
                                }
                                else
                                    buttonSelect[2] = false;

                                if (mpos_absolute.x > 180 && mpos_absolute.x < 300 && mpos_absolute.y > 207  && mpos_absolute.y < 273)
                                {
                                    if (!buttonSelect[3])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[3] = true;
                                }
                                else
                                    buttonSelect[3] = false;

                                if (mpos_absolute.x > 340 && mpos_absolute.x < 460 && mpos_absolute.y > 207  && mpos_absolute.y < 273)
                                {
                                    if (!buttonSelect[4])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[4] = true;
                                }
                                else
                                    buttonSelect[4] = false;

                                if (mpos_absolute.x > 180 && mpos_absolute.x < 300 && mpos_absolute.y > 347  && mpos_absolute.y < 413)
                                {
                                    if (!buttonSelect[8])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[8] = true;
                                }
                                else
                                    buttonSelect[8] = false;

                                if (mpos_absolute.x > 340 && mpos_absolute.x < 460 && mpos_absolute.y > 347  && mpos_absolute.y < 413)
                                {
                                    if (!buttonSelect[9])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[9] = true;
                                }
                                else
                                    buttonSelect[9] = false;

                                if (mpos_absolute.x > 4 && mpos_absolute.x < 78 && mpos_absolute.y > 410  && mpos_absolute.y < 476)
                                {
                                    if (!buttonSelect[7])
                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), mouseSound, 0, NULL);

                                    buttonSelect[7] = true;
                                }
                                else
                                    buttonSelect[7] = false;

                                break;
                        }
                    }

                    break;

                case Event::MouseButtonPressed :

                    if (!animDone)
                    {
                        timeb = 300;
                        animDone = true;

                        title->setPosition(320, 87);

                        button[0]->setPosition(148, 242);
                        button[1]->setPosition(320, 242);
                        button[2]->setPosition(492, 242);
                        button[3]->setPosition(148, 378);
                        button[4]->setPosition(320, 378);
                        button[5]->setPosition(492, 378);
                    }

                    if (menuBlock || eventSystem.mouseButton.button != Mouse::Left)
                        break;

                    switch (menu)
                    {
                        case MAINMENU :

                            if (buttonSelect[0])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                buttonSelect[0] = false;

                                menuToGo = EDITMENU;
                            }
                            else if (buttonSelect[1])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                buttonSelect[1] = false;

                                menuToGo = CREATESCENARIOMENU;
                            }
                            else if (buttonSelect[2])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                buttonSelect[2] = false;

                                menuToGo = OPTIONMENU;
                            }
                            else if (buttonSelect[3])
                            {
                                OPENFILENAME dialogParms;
                                TCHAR personalPath[MAX_PATH];
                                TCHAR filename[MAX_PATH] = "";

                                SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                StrCat(personalPath, "\\Mario Constructor Master\\Levels");

                                ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                dialogParms.lStructSize = sizeof(OPENFILENAME);
                                dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                dialogParms.hInstance = *mainInstance;
                                dialogParms.lpstrFile = filename;
                                dialogParms.nFilterIndex = 1;
                                dialogParms.nMaxFile = sizeof(filename);
                                dialogParms.lpstrInitialDir = personalPath;
                                dialogParms.lpstrTitle = "Choose the Level to play :";
                                dialogParms.lpstrFilter = "Constructor Master Levels (.cml)\0*.cml\0";
                                dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                if (GetOpenFileName(&dialogParms))
                                {
                                    ifstream levelFile;

                                    levelFile.open(filename, ios::binary);

                                    if (!levelFile.good())
                                        MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                                    else
                                    {
                                        if (checkLoadResources(levelFile, filename))
                                        {
                                            fileToLoad = filename;
                                            loadingType = 0;

                                            menuBlock = true;
                                            menuToGo = EXITTOPLAY;

                                            fadeCircle = true;

                                            circleFadeTex = new Texture;

                                            if (!circleFadeTex->loadFromFile("Data/Gfx/CircleFade.bmp"))
                                            {
                                                MessageBox(NULL, "Error ! Failed to load Texture :\nData/Gfx/CircleFade.bmp", "Failed to load Texture", MB_OK | MB_ICONERROR | MB_TASKMODAL);

                                                mainWindow->close();

                                                return false;
                                            }

                                            circleFadeTex->setSmooth(true);

                                            circleSpr = new Sprite(*circleFadeTex);
                                            circleSpr->setPosition(320, 240);
                                            circleSpr->setScale(1.5, 1.5);
                                            circleSpr->setOrigin(240, 240);

                                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), enterLevel, 0, NULL);

                                            FMOD_Channel_Stop(musicChannel);
                                        }

                                        levelFile.close();
                                    }
                                }

                                buttonSelect[3] = false;
                            }
                            else if (buttonSelect[4])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                buttonSelect[4] = false;

                                menuToGo = PLAYSCENARIOMENU;
                            }
                            else if (buttonSelect[5])
                            {
                                int userAnswer;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                userAnswer = MessageBox(NULL, "Do you want to quit ?", "Quit ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL);

                                if (userAnswer == IDYES)
                                {
                                    exitLoop = true;

                                    mainWindow->close();

                                    break;
                                }
                            }

                            break;

                        case EDITMENU :

                            if (buttonSelect[0])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                fileToLoad.clear();

                                menuBlock = true;
                                menuToGo = EXITTOEDITOR;

                                buttonSelect[0] = false;
                            }
                            else if (buttonSelect[1])
                            {
                                OPENFILENAME dialogParms;
                                TCHAR personalPath[MAX_PATH];
                                TCHAR filename[MAX_PATH] = "";

                                SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                StrCat(personalPath, "\\Mario Constructor Master\\Levels");

                                ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                dialogParms.lStructSize = sizeof(OPENFILENAME);
                                dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                dialogParms.hInstance = *mainInstance;
                                dialogParms.lpstrFile = filename;
                                dialogParms.nFilterIndex = 1;
                                dialogParms.nMaxFile = sizeof(filename);
                                dialogParms.lpstrInitialDir = personalPath;
                                dialogParms.lpstrTitle = "Choose the Level to edit :";
                                dialogParms.lpstrFilter = "Constructor Master Levels (.cml)\0*.cml\0";
                                dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                if (GetOpenFileName(&dialogParms))
                                {
                                    ifstream levelFile;

                                    levelFile.open(filename, ios::binary);

                                    if (!levelFile.good())
                                        MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                                    else
                                    {
                                        if (checkLoadResources(levelFile, filename))
                                        {
                                            fileToLoad = filename;

                                            menuBlock = true;
                                            menuToGo = EXITTOEDITOR;
                                        }

                                        levelFile.close();
                                    }
                                }

                                buttonSelect[1] = false;
                            }
                            else if (buttonSelect[7])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                menuToGo = MAINMENU;

                                buttonSelect[7] = false;
                            }

                            break;

                        case CREATESCENARIOMENU :

                            if (buttonSelect[0])
                            {
                                OPENFILENAME dialogParms;
                                TCHAR personalPath[MAX_PATH];
                                TCHAR filegrab[MAX_PATH] = "";

                                SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                StrCat(personalPath, "\\Mario Constructor Master\\Scenarios");

                                ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                dialogParms.lStructSize = sizeof(OPENFILENAME);
                                dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                dialogParms.hInstance = *mainInstance;
                                dialogParms.lpstrFile = filegrab;
                                dialogParms.nFilterIndex = 1;
                                dialogParms.nMaxFile = sizeof(filegrab);
                                dialogParms.lpstrInitialDir = personalPath;
                                dialogParms.lpstrTitle = "Choose the emplacement for the Scenario :";
                                dialogParms.lpstrFilter = "Constructor Master Scenarios (.cms)\0*.cms\0";
                                dialogParms.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                if (GetSaveFileName(&dialogParms))
                                {
                                    levels = 255;
                                    lifes = 255;

                                    PathRenameExtension(filegrab, ".cms");

                                    ofstream scenario(filegrab, ios::binary);

                                    if (scenario.good())
                                    {
                                        char CMLid[5];

                                        CMLid[0] = 'C';
                                        CMLid[1] = 'M';
                                        CMLid[2] = 'S';
                                        CMLid[3] = EDITOR_VERSION;
                                        CMLid[4] = 1;

                                        scenario.write(CMLid, 5);

                                        DialogBox(*mainInstance, MAKEINTRESOURCE(35), mainWindow->getSystemHandle(), reinterpret_cast<DLGPROC>(dialogNumLevels));

                                        if (levels != 255)
                                        {
                                            char num[32];
                                            ifstream level;

                                            OPENFILENAME dialogParms;
                                            TCHAR personalPath[MAX_PATH];
                                            TCHAR relativePath[MAX_PATH];
                                            TCHAR dirPath[MAX_PATH];

                                            SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                            StrCat(personalPath, "\\Mario Constructor Master\\Levels");

                                            ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                            dialogParms.lStructSize = sizeof(OPENFILENAME);
                                            dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                            dialogParms.hInstance = *mainInstance;
                                            dialogParms.lpstrFile = filegrab;
                                            dialogParms.nFilterIndex = 1;
                                            dialogParms.nMaxFile = sizeof(filegrab);
                                            dialogParms.lpstrInitialDir = personalPath;
                                            dialogParms.lpstrFilter = "Constructor Master Levels (.cml)\0*.cml\0";
                                            dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

                                            strcpy(dirPath, filegrab);

                                            PathRemoveFileSpec(dirPath);

                                            DialogBox(*mainInstance, MAKEINTRESOURCE(65), mainWindow->getSystemHandle(), reinterpret_cast<DLGPROC>(dialogNumLifes));

                                            if (lifes != 255)
                                            {
                                                scenario.write(reinterpret_cast<char*>(&lifes), 1);

                                                for (register unsigned char i = 0; i < levels; i++)
                                                {
                                                    sprintf(num, "Select the level num. %u :", i+1);

                                                    dialogParms.lpstrTitle = num;
                                                    ZeroMemory(filegrab, MAX_PATH);

                                                    if (GetOpenFileName(&dialogParms))
                                                    {
                                                        level.open(filegrab, ios::binary);

                                                        if (!level.good())
                                                        {
                                                            MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            if (checkLoadResources(level, filegrab))
                                                            {
                                                                if (PathRelativePathTo(relativePath, dirPath, FILE_ATTRIBUTE_DIRECTORY, filegrab, FILE_ATTRIBUTE_NORMAL))
                                                                    scenario << relativePath;
                                                                else
                                                                {
                                                                    MessageBox(NULL, "Error ! Failed to create Relative Path !", "Can't create Relative Path !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                                                                    level.close();
                                                                    break;
                                                                }

                                                                scenario.write("", 1);
                                                            }
                                                        }

                                                        level.close();
                                                    }
                                                    else
                                                        break;
                                                }

                                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), createScenario, 0, NULL);
                                            }
                                        }
                                    }

                                    scenario.close();
                                }

                                buttonSelect[0] = false;
                            }
                            else if (buttonSelect[1])
                            {
                                OPENFILENAME dialogParms;
                                TCHAR personalPath[MAX_PATH];
                                TCHAR filegrab[MAX_PATH] = "";

                                SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                StrCat(personalPath, "\\Mario Constructor Master\\Scenarios");

                                ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                dialogParms.lStructSize = sizeof(OPENFILENAME);
                                dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                dialogParms.hInstance = *mainInstance;
                                dialogParms.lpstrFile = filegrab;
                                dialogParms.nFilterIndex = 1;
                                dialogParms.nMaxFile = sizeof(filegrab);
                                dialogParms.lpstrInitialDir = personalPath;
                                dialogParms.lpstrTitle = "Choose the emplacement for the Scenario :";
                                dialogParms.lpstrFilter = "Constructor Master Scenarios (.cms)\0*.cms\0";
                                dialogParms.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                if (GetSaveFileName(&dialogParms))
                                {
                                    levels = 255;
                                    lifes = 255;

                                    PathRenameExtension(filegrab, ".cms");

                                    ofstream scenario(filegrab, ios::binary);

                                    if (scenario.good())
                                    {
                                        char CMLid[5];

                                        CMLid[0] = 'C';
                                        CMLid[1] = 'M';
                                        CMLid[2] = 'S';
                                        CMLid[3] = EDITOR_VERSION;
                                        CMLid[4] = 0;

                                        scenario.write(CMLid, 5);

                                        DialogBox(*mainInstance, MAKEINTRESOURCE(35), mainWindow->getSystemHandle(), reinterpret_cast<DLGPROC>(dialogNumLevels));

                                        if (levels != 255)
                                        {
                                            char num[32];
                                            ifstream level;

                                            OPENFILENAME dialogParms;
                                            TCHAR personalPath[MAX_PATH];

                                            SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                            StrCat(personalPath, "\\Mario Constructor Master\\Levels");

                                            ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                            dialogParms.lStructSize = sizeof(OPENFILENAME);
                                            dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                            dialogParms.hInstance = *mainInstance;
                                            dialogParms.lpstrFile = filegrab;
                                            dialogParms.nFilterIndex = 1;
                                            dialogParms.nMaxFile = sizeof(filegrab);
                                            dialogParms.lpstrInitialDir = personalPath;
                                            dialogParms.lpstrFilter = "Constructor Master Levels (.cml)\0*.cml\0";
                                            dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

                                            DialogBox(*mainInstance, MAKEINTRESOURCE(65), mainWindow->getSystemHandle(), reinterpret_cast<DLGPROC>(dialogNumLifes));

                                            if (lifes != 255)
                                            {
                                                scenario.write(reinterpret_cast<char*>(&lifes), 1);

                                                for (register unsigned char i = 0; i < levels; i++)
                                                {
                                                    sprintf(num, "Select the level num. %u :", i+1);

                                                    dialogParms.lpstrTitle = num;
                                                    ZeroMemory(filegrab, MAX_PATH);

                                                    if (GetOpenFileName(&dialogParms))
                                                    {
                                                        level.open(filegrab, ios::binary);

                                                        if (!level.good())
                                                        {
                                                            MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            if (checkLoadResources(level, filegrab))
                                                            {
                                                                level.seekg(0, ios::beg);

                                                                while (!level.eof())
                                                                {
                                                                    level.read(num, 1);
                                                                    scenario.write(num, 1);
                                                                }
                                                            }
                                                        }

                                                        level.close();
                                                    }
                                                    else
                                                        break;
                                                }

                                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), createScenario, 0, NULL);
                                            }
                                        }
                                    }

                                    scenario.close();
                                }

                                buttonSelect[1] = false;
                            }
                            else if (buttonSelect[2])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                menuToGo = EXITTOWORLD;

                                buttonSelect[2] = false;
                            }
                            else if (buttonSelect[7])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                menuToGo = MAINMENU;

                                buttonSelect[7] = false;
                            }

                            break;

                        case PLAYSCENARIOMENU :

                            if (buttonSelect[0])
                            {
                                OPENFILENAME dialogParms;
                                TCHAR personalPath[MAX_PATH];
                                TCHAR filename[MAX_PATH] = "";

                                SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                StrCat(personalPath, "\\Mario Constructor Master\\Scenarios");

                                ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                dialogParms.lStructSize = sizeof(OPENFILENAME);
                                dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                dialogParms.hInstance = *mainInstance;
                                dialogParms.lpstrFile = filename;
                                dialogParms.nFilterIndex = 1;
                                dialogParms.nMaxFile = sizeof(filename);
                                dialogParms.lpstrInitialDir = personalPath;
                                dialogParms.lpstrTitle = "Choose the Scenario to play :";
                                dialogParms.lpstrFilter = "Constructor Master Scenarios (.cms)\0*.cms\0";
                                dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                if (GetOpenFileName(&dialogParms))
                                {
                                    ifstream scenarioFile;

                                    scenarioFile.open(filename, ios::binary);

                                    if (!scenarioFile.good())
                                        MessageBox(NULL, "Failed to open the Scenario file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                                    else
                                    {
                                        char CMSid[5];
                                        bool correct = true;

                                        scenarioFile.read(CMSid, 5);

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
                                            scenarioToLoad = filename;
                                            loadingType = CMSid[4] + 1;

                                            #ifdef DEBUGMODE
                                            cout << static_cast<unsigned int>(loadingType) << endl;
                                            #endif // DEBUGMODE

                                            menuBlock = true;
                                            menuToGo = EXITTOPLAY;

                                            fadeCircle = true;

                                            circleFadeTex = new Texture;

                                            if (!circleFadeTex->loadFromFile("Data/Gfx/CircleFade.bmp"))
                                            {
                                                MessageBox(NULL, "Error ! Failed to load Texture :\nData/Gfx/CircleFade.bmp", "Failed to load Texture", MB_OK | MB_ICONERROR | MB_TASKMODAL);

                                                mainWindow->close();

                                                return false;
                                            }

                                            circleFadeTex->setSmooth(true);

                                            circleSpr = new Sprite(*circleFadeTex);
                                            circleSpr->setPosition(320, 240);
                                            circleSpr->setScale(1.5, 1.5);
                                            circleSpr->setOrigin(240, 240);

                                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), enterLevel, 0, NULL);

                                            FMOD_Channel_Stop(musicChannel);
                                        }

                                        scenarioFile.close();
                                    }
                                }
                                buttonSelect[0] = false;
                            }
                            else if (buttonSelect[1])
                            {
                                OPENFILENAME dialogParms;
                                TCHAR personalPath[MAX_PATH];
                                TCHAR filename[MAX_PATH] = "";

                                SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

                                StrCat(personalPath, "\\Mario Constructor Master\\Worlds");

                                ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

                                dialogParms.lStructSize = sizeof(OPENFILENAME);
                                dialogParms.hwndOwner = mainWindow->getSystemHandle();
                                dialogParms.hInstance = *mainInstance;
                                dialogParms.lpstrFile = filename;
                                dialogParms.nFilterIndex = 1;
                                dialogParms.nMaxFile = sizeof(filename);
                                dialogParms.lpstrInitialDir = personalPath;
                                dialogParms.lpstrTitle = "Choose the World to play :";
                                dialogParms.lpstrFilter = "Constructor Master Worlds (.cmw)\0*.cmw\0";
                                dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                if (GetOpenFileName(&dialogParms))
                                {
                                    ifstream scenarioFile;

                                    scenarioFile.open(filename, ios::binary);

                                    if (!scenarioFile.good())
                                        MessageBox(NULL, "Failed to open the Scenario file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
                                    else
                                    {
                                        char CMWid[4];
                                        bool correct = true;

                                        scenarioFile.read(CMWid, 4);

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
                                            scenarioToLoad = filename;
                                            loadingType = 4;

                                            menuBlock = true;
                                            menuToGo = EXITTOPLAY;

                                            fadeCircle = true;

                                            circleFadeTex = new Texture;

                                            if (!circleFadeTex->loadFromFile("Data/Gfx/CircleFade.bmp"))
                                            {
                                                MessageBox(NULL, "Error ! Failed to load Texture :\nData/Gfx/CircleFade.bmp", "Failed to load Texture", MB_OK | MB_ICONERROR | MB_TASKMODAL);

                                                mainWindow->close();

                                                return false;
                                            }

                                            circleFadeTex->setSmooth(true);

                                            circleSpr = new Sprite(*circleFadeTex);
                                            circleSpr->setPosition(320, 240);
                                            circleSpr->setScale(1.5, 1.5);
                                            circleSpr->setOrigin(240, 240);

                                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), enterLevel, 0, NULL);

                                            FMOD_Channel_Stop(musicChannel);
                                        }

                                        scenarioFile.close();
                                    }
                                }
                                buttonSelect[1] = false;
                            }
                            else if (buttonSelect[7])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                menuToGo = MAINMENU;

                                buttonSelect[7] = false;
                            }

                            break;

                        case OPTIONMENU :

                            if (buttonSelect[0])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                mainWindow->setSize(Vector2u(640, 480));
                                mainWindow->setView(View(FloatRect(0, 0, 640, 480)));

                                textureRender.setPosition(0, 0);
                                textureRender.setScale(1, 1);

                                windowScale = 1;
                                wpos = Vector2f(0, 0);

                                if (!RewriteParameters())
                                    MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                            }
                            else if (buttonSelect[1])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                mainWindow->setSize(Vector2u(1280, 960));
                                mainWindow->setView(View(FloatRect(0, 0, 1280, 960)));

                                textureRender.setPosition(0, 0);
                                textureRender.setScale(2, 2);

                                windowScale = 2;
                                wpos = Vector2f(0, 0);

                                if (!RewriteParameters())
                                    MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                            }
                            else if (buttonSelect[2])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                mainWindow->setSize(Vector2u(2560, 1920));
                                mainWindow->setView(View(FloatRect(0, 0, 2560, 1920)));

                                textureRender.setPosition(0, 0);
                                textureRender.setScale(4, 4);

                                windowScale = 4;
                                wpos = Vector2f(0, 0);

                                if (!RewriteParameters())
                                    MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                            }
                            else if (buttonSelect[3])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                buttonSelect[3] = false;

                                if (!editorMusic)
                                {
                                    editorMusic = true;

                                    if (!RewriteParameters())
                                        MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                                }
                            }
                            else if (buttonSelect[4])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                buttonSelect[4] = false;

                                if (editorMusic)
                                {
                                    editorMusic = false;

                                    if (!RewriteParameters())
                                        MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                                }
                            }
                            else if (buttonSelect[8])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                buttonSelect[8] = false;

                                if (!editorMouseScrolling)
                                {
                                    editorMouseScrolling = true;

                                    if (!RewriteParameters())
                                        MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                                }
                            }
                            else if (buttonSelect[9])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                buttonSelect[9] = false;

                                if (editorMouseScrolling)
                                {
                                    editorMouseScrolling = false;

                                    if (!RewriteParameters())
                                        MessageBox(NULL, "Failed to save the parameters into the file :\n\"Parms.dat\" !", "File error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
                                }
                            }
                            else if (buttonSelect[7])
                            {
                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), clickSound, 0, NULL);

                                menuBlock = true;
                                menuToGo = MAINMENU;

                                buttonSelect[7] = false;
                            }

                            break;
                    }

                    break;

                case Event::Resized :

                    windowScale = min(eventSystem.size.width / 640.0, eventSystem.size.height / 480.0);
                    wpos = Vector2f((eventSystem.size.width - (windowScale * 640)) / 2, (eventSystem.size.height - (windowScale * 480)) / 2);

                    mainWindow->setView(View(FloatRect(0, 0, eventSystem.size.width, eventSystem.size.height)));

                    textureRender.setPosition(wpos);
                    textureRender.setScale(windowScale, windowScale);

                    break;

                default             : break;
            }
        }

        UpdateAssets();

        switch (menuToGo)
        {
            case MAINMENU   :

                if (menu != MAINMENU)
                {
                    if (fadeAlpha < 255 - FADESPEED)
                        fadeAlpha += FADESPEED;
                    else
                    {
                        fadeAlpha = 255;
                        menuBlock = false;
                        menu = MAINMENU;
                    }
                }
                else
                {
                    if (fadeAlpha > FADESPEED)
                        fadeAlpha -= FADESPEED;
                    else
                        fadeAlpha = 0;
                }

                break;

            case EDITMENU   :

                if (menu != EDITMENU)
                {
                    if (fadeAlpha < 255 - FADESPEED)
                        fadeAlpha += FADESPEED;
                    else
                    {
                        fadeAlpha = 255;
                        menuBlock = false;
                        menu = EDITMENU;
                    }
                }
                else
                {
                    if (fadeAlpha > FADESPEED)
                        fadeAlpha -= FADESPEED;
                    else
                        fadeAlpha = 0;
                }

                break;

            case CREATESCENARIOMENU   :

                if (menu != CREATESCENARIOMENU)
                {
                    if (fadeAlpha < 255 - FADESPEED)
                        fadeAlpha += FADESPEED;
                    else
                    {
                        fadeAlpha = 255;
                        menuBlock = false;
                        menu = CREATESCENARIOMENU;
                    }
                }
                else
                {
                    if (fadeAlpha > FADESPEED)
                        fadeAlpha -= FADESPEED;
                    else
                        fadeAlpha = 0;
                }

                break;

            case PLAYSCENARIOMENU   :

                if (menu != PLAYSCENARIOMENU)
                {
                    if (fadeAlpha < 255 - FADESPEED)
                        fadeAlpha += FADESPEED;
                    else
                    {
                        fadeAlpha = 255;
                        menuBlock = false;
                        menu = PLAYSCENARIOMENU;
                    }
                }
                else
                {
                    if (fadeAlpha > FADESPEED)
                        fadeAlpha -= FADESPEED;
                    else
                        fadeAlpha = 0;
                }

                break;

            case OPTIONMENU   :

                if (menu != OPTIONMENU)
                {
                    if (fadeAlpha < 255 - FADESPEED)
                        fadeAlpha += FADESPEED;
                    else
                    {
                        fadeAlpha = 255;
                        menuBlock = false;
                        menu = OPTIONMENU;
                    }
                }
                else
                {
                    if (fadeAlpha > FADESPEED)
                        fadeAlpha -= FADESPEED;
                    else
                        fadeAlpha = 0;
                }

                break;

            case EXITTOEDITOR  :

                if (fadeAlpha < 255 - FADESPEED)
                {
                    float volume;

                    FMOD_Channel_GetVolume(musicChannel, &volume);

                    fadeAlpha += FADESPEED;
                    volume -= 0.016;

                    FMOD_Channel_SetVolume(musicChannel, volume);
                }
                else
                {
                    FMOD_Channel_Stop(musicChannel);

                    exitLoop = true;
                    targetScene = SCENE_EDITOR;
                }

                break;

            case EXITTOPLAY  :

                if (fadeAlpha > FADESPEED)
                    fadeAlpha -= FADESPEED;
                else
                    fadeAlpha = 0;

                {
                    float circleScale = circleSpr->getScale().x;

                    if (circleScale > 0.01)
                        circleSpr->scale(0.96, 0.96);
                    else
                    {
                        exitLoop = true;
                        targetScene = SCENE_INGAME;
                    }
                }

                break;

            case EXITTOWORLD :

                if (fadeAlpha < 255 - FADESPEED)
                {
                    float volume;

                    FMOD_Channel_GetVolume(musicChannel, &volume);

                    fadeAlpha += FADESPEED;
                    volume -= 0.016;

                    FMOD_Channel_SetVolume(musicChannel, volume);
                }
                else
                {
                    FMOD_Channel_Stop(musicChannel);

                    exitLoop = true;
                    targetScene = SCENE_WORLDEDITOR;
                }

                break;
        }

        if (animDone)
        {
            if (timeb < 12)
                timeb++;
            else
                timeb = 0;
        }
        else
        {
            if (timeb < 300)
                timeb++;
        }

        switch (menu)
        {
            case MAINMENU :

                mainTexture.draw(*gradient[0]);

                mainTexture.draw(*tileMap, tileSet);
                mainTexture.draw(*greenBush);
                mainTexture.draw(*title);

                if (timeb > 175 || animDone)
                {
                    for (register unsigned int i = 0; i < 6; i++)
                        mainTexture.draw(*button[i]);
                }

                if (animDone)
                {
                    if (timeb == 12)
                        titleShine->emit(particle);

                    titleShine->update();

                    mainTexture.draw(*titleShine, particle);
                }

                if (frame_Water < 23)
                    frame_Water++;
                else
                    frame_Water = 0;

                greenBush->update();

                break;

            case EDITMENU :

                mainTexture.draw(*gradient[1]);

                for (register unsigned int i = 0; i < 4; i++)
                    mainTexture.draw(*buttonEd[i]);

                mainTexture.draw(*buttonRet);

                break;

            case CREATESCENARIOMENU :

                mainTexture.draw(*gradient[1]);

                for (register unsigned int i = 0; i < 6; i++)
                    mainTexture.draw(*buttonSc[i]);

                mainTexture.draw(*buttonRet);

                break;

            case PLAYSCENARIOMENU :

                mainTexture.draw(*gradient[3]);

                for (register unsigned int i = 0; i < 4; i++)
                    mainTexture.draw(*buttonPl[i]);

                mainTexture.draw(*buttonRet);

                break;

            case OPTIONMENU :

                mainTexture.draw(*gradient[2]);

                for (register unsigned int i = 0; i < 10; i++)
                    mainTexture.draw(*buttonOp[i]);

                mainTexture.draw(*buttonRet);

                break;
        }

        if (fadeAlpha > 0)
            mainTexture.draw(*fadeRect);

        if (fadeCircle)
        {
            Vector2f circleSize = circleSpr->getScale();

            mainTexture.draw(*circleSpr, BlendMultiply);

            blackBorder->setPosition(0, 0);
            blackBorder->setSize(Vector2f(320 - (240 * circleSize.x), 480));

            mainTexture.draw(*blackBorder);

            blackBorder->setPosition(320 + (240 * circleSize.x), 0);
            blackBorder->setSize(Vector2f(320 - (240 * circleSize.x), 480));

            mainTexture.draw(*blackBorder);

            blackBorder->setPosition(0, 0);
            blackBorder->setSize(Vector2f(640, 240 - (240 * circleSize.y)));

            mainTexture.draw(*blackBorder);

            blackBorder->setPosition(0, 240 + (240 * circleSize.y));
            blackBorder->setSize(Vector2f(640, 240 - (240 * circleSize.y)));

            mainTexture.draw(*blackBorder);
        }

        if (exitLoop)
            mainTexture.clear(Color::Black);

        cursor->setPosition(static_cast<Vector2f>(mpos_absolute));

        mainTexture.draw(*cursor);

        if (exitLoop)
            mainTexture.draw(*loading);

        mainTexture.display();

        mainWindow->clear(Color::Black);
        mainWindow->draw(textureRender);
        mainWindow->display();
    }

    delete tileMap;
    delete fadeRect;

    delete tileSet;

    if (circleFadeTex != NULL)
        delete circleFadeTex;

    if (circleSpr != NULL)
        delete circleSpr;

    delete titleTex;
    delete buttonTex[0];
    delete buttonTex[1];
    delete buttonTex[2];
    delete buttonTex[3];
    delete buttonTex[4];
    delete buttonTex[5];

    delete buttonEdTex[0];
    delete buttonEdTex[1];
    delete buttonEdTex[2];
    delete buttonEdTex[3];

    delete buttonScTex[0];
    delete buttonScTex[1];
    delete buttonScTex[2];
    delete buttonScTex[3];
    delete buttonScTex[4];
    delete buttonScTex[5];

    delete buttonPlTex[0];
    delete buttonPlTex[1];
    delete buttonPlTex[2];
    delete buttonPlTex[3];

    delete buttonOpTex[0];
    delete buttonOpTex[1];
    delete buttonOpTex[2];
    delete buttonOpTex[3];
    delete buttonOpTex[4];
    delete buttonOpTex[5];
    delete buttonOpTex[6];
    delete buttonOpTex[7];

    delete blackBorder;

    delete buttonRetTex;

    delete particle;

    delete bush;

    delete title;
    delete button[0];
    delete button[1];
    delete button[2];
    delete button[3];
    delete button[4];
    delete button[5];

    delete buttonEd[0];
    delete buttonEd[1];
    delete buttonEd[2];
    delete buttonEd[3];

    delete buttonSc[0];
    delete buttonSc[1];
    delete buttonSc[2];
    delete buttonSc[3];
    delete buttonSc[4];
    delete buttonSc[5];

    delete buttonPl[0];
    delete buttonPl[1];
    delete buttonPl[2];
    delete buttonPl[3];

    delete buttonOp[0];
    delete buttonOp[1];
    delete buttonOp[2];
    delete buttonOp[3];
    delete buttonOp[4];
    delete buttonOp[5];
    delete buttonOp[6];
    delete buttonOp[7];
    delete buttonOp[8];
    delete buttonOp[9];

    delete buttonRet;

    delete gradient[0];
    delete gradient[1];
    delete gradient[2];
    delete gradient[3];

    delete titleShine;

    delete greenBush;

    FMOD_Sound_Release(clickSound);
    FMOD_Sound_Release(mouseSound);
    FMOD_Sound_Release(enterLevel);
    FMOD_Sound_Release(createScenario);

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    Image tileset;
    Image bushImg;
    FMOD_RESULT result;

    animDone = false;
    menuBlock = false;

    fadeCircle = false;

    timeb = 0;

    titleSpeed = 7;

    frame_Water = 0;

    buttonSpeed[0] = 11;
    buttonSpeed[1] = 11;
    buttonSpeed[2] = 11;
    buttonSpeed[3] = 11;
    buttonSpeed[4] = 11;
    buttonSpeed[5] = 11;

    buttonSelect[0] = false;
    buttonSelect[1] = false;
    buttonSelect[2] = false;
    buttonSelect[3] = false;
    buttonSelect[4] = false;
    buttonSelect[5] = false;
    buttonSelect[6] = false;
    buttonSelect[7] = false;
    buttonSelect[8] = false;
    buttonSelect[9] = false;

    menu = MAINMENU;

    if (!tileset.loadFromFile("Data/Gfx/Tilesets/Tileset_Default.png"))
        allright = false;

    tileset.createMaskFromColor(Color::Magenta);

    tileSet = new Texture;
    tileSet->loadFromImage(tileset, IntRect(0, 0, 640, 416));

    titleTex = new Texture;

    if (!titleTex->loadFromFile("Data/Gfx/TitleScreen/Title.png"))
        allright = false;

    circleFadeTex = NULL;
    circleSpr = NULL;

    buttonTex[0] = new Texture;
    buttonTex[1] = new Texture;
    buttonTex[2] = new Texture;
    buttonTex[3] = new Texture;
    buttonTex[4] = new Texture;
    buttonTex[5] = new Texture;

    buttonEdTex[0] = new Texture;
    buttonEdTex[1] = new Texture;
    buttonEdTex[2] = new Texture;
    buttonEdTex[3] = new Texture;

    buttonScTex[0] = new Texture;
    buttonScTex[1] = new Texture;
    buttonScTex[2] = new Texture;
    buttonScTex[3] = new Texture;
    buttonScTex[4] = new Texture;
    buttonScTex[5] = new Texture;

    buttonPlTex[0] = new Texture;
    buttonPlTex[1] = new Texture;
    buttonPlTex[2] = new Texture;
    buttonPlTex[3] = new Texture;

    buttonOpTex[0] = new Texture;
    buttonOpTex[1] = new Texture;
    buttonOpTex[2] = new Texture;
    buttonOpTex[3] = new Texture;
    buttonOpTex[4] = new Texture;
    buttonOpTex[5] = new Texture;
    buttonOpTex[6] = new Texture;
    buttonOpTex[7] = new Texture;

    buttonRetTex = new Texture;

    if (!buttonTex[0]->loadFromFile("Data/Gfx/TitleScreen/Button_Edit.png"))
        allright = false;

    if (!buttonTex[1]->loadFromFile("Data/Gfx/TitleScreen/Button_CreateScenario.png"))
        allright = false;

    if (!buttonTex[2]->loadFromFile("Data/Gfx/TitleScreen/Button_Options.png"))
        allright = false;

    if (!buttonTex[3]->loadFromFile("Data/Gfx/TitleScreen/Button_PlayLevel.png"))
        allright = false;

    if (!buttonTex[4]->loadFromFile("Data/Gfx/TitleScreen/Button_PlayScenario.png"))
        allright = false;

    if (!buttonTex[5]->loadFromFile("Data/Gfx/TitleScreen/Button_Quit.png"))
        allright = false;

    if (!buttonEdTex[0]->loadFromFile("Data/Gfx/TitleScreen/Editor/Button_CreateLevel.png"))
        allright = false;

    if (!buttonEdTex[1]->loadFromFile("Data/Gfx/TitleScreen/Editor/Button_EditLevel.png"))
        allright = false;

    if (!buttonEdTex[2]->loadFromFile("Data/Gfx/TitleScreen/Editor/Editor_Create.png"))
        allright = false;

    if (!buttonEdTex[3]->loadFromFile("Data/Gfx/TitleScreen/Editor/Editor_Load.png"))
        allright = false;

    if (!buttonScTex[0]->loadFromFile("Data/Gfx/TitleScreen/Scenario/Button_CreateSuite.png"))
        allright = false;

    if (!buttonScTex[1]->loadFromFile("Data/Gfx/TitleScreen/Scenario/Button_CreateCompil.png"))
        allright = false;

    if (!buttonScTex[2]->loadFromFile("Data/Gfx/TitleScreen/Scenario/Button_CreateMap.png"))
        allright = false;

    if (!buttonScTex[3]->loadFromFile("Data/Gfx/TitleScreen/Scenario/Scenario_Suite.png"))
        allright = false;

    if (!buttonScTex[4]->loadFromFile("Data/Gfx/TitleScreen/Scenario/Scenario_Compile.png"))
        allright = false;

    if (!buttonScTex[5]->loadFromFile("Data/Gfx/TitleScreen/Scenario/Scenario_Map.png"))
        allright = false;

    if (!buttonPlTex[0]->loadFromFile("Data/Gfx/TitleScreen/Play/Button_PlaySuite.png"))
        allright = false;

    if (!buttonPlTex[1]->loadFromFile("Data/Gfx/TitleScreen/Play/Button_PlayMap.png"))
        allright = false;

    if (!buttonPlTex[2]->loadFromFile("Data/Gfx/TitleScreen/Play/Play_Suite.png"))
        allright = false;

    if (!buttonPlTex[3]->loadFromFile("Data/Gfx/TitleScreen/Play/Play_Map.png"))
        allright = false;

    if (!buttonOpTex[0]->loadFromFile("Data/Gfx/TitleScreen/Options/Button_640x480.png"))
        allright = false;

    if (!buttonOpTex[1]->loadFromFile("Data/Gfx/TitleScreen/Options/Button_1280x960.png"))
        allright = false;

    if (!buttonOpTex[2]->loadFromFile("Data/Gfx/TitleScreen/Options/Button_2560x1920.png"))
        allright = false;

    if (!buttonOpTex[3]->loadFromFile("Data/Gfx/TitleScreen/Options/Button_Yes.png"))
        allright = false;

    if (!buttonOpTex[4]->loadFromFile("Data/Gfx/TitleScreen/Options/Button_No.png"))
        allright = false;

    if (!buttonOpTex[5]->loadFromFile("Data/Gfx/TitleScreen/Options/Option_Size.png"))
        allright = false;

    if (!buttonOpTex[6]->loadFromFile("Data/Gfx/TitleScreen/Options/Option_Music.png"))
        allright = false;

    if (!buttonOpTex[7]->loadFromFile("Data/Gfx/TitleScreen/Options/Option_Mouse.png"))
        allright = false;

    if (!buttonRetTex->loadFromFile("Data/Gfx/TitleScreen/Button_Return.png"))
        allright = false;

    buttonTex[0]->setSmooth(true);
    buttonTex[1]->setSmooth(true);
    buttonTex[2]->setSmooth(true);
    buttonTex[3]->setSmooth(true);
    buttonTex[4]->setSmooth(true);
    buttonTex[5]->setSmooth(true);

    buttonEdTex[0]->setSmooth(true);
    buttonEdTex[1]->setSmooth(true);

    buttonScTex[0]->setSmooth(true);
    buttonScTex[1]->setSmooth(true);
    buttonScTex[2]->setSmooth(true);

    buttonPlTex[0]->setSmooth(true);
    buttonPlTex[1]->setSmooth(true);

    buttonOpTex[0]->setSmooth(true);
    buttonOpTex[1]->setSmooth(true);
    buttonOpTex[2]->setSmooth(true);
    buttonOpTex[3]->setSmooth(true);
    buttonOpTex[4]->setSmooth(true);

    buttonRetTex->setSmooth(true);

    title = new Sprite(*titleTex);

    button[0] = new Sprite(*buttonTex[0]);
    button[1] = new Sprite(*buttonTex[1]);
    button[2] = new Sprite(*buttonTex[2]);
    button[3] = new Sprite(*buttonTex[3]);
    button[4] = new Sprite(*buttonTex[4]);
    button[5] = new Sprite(*buttonTex[5]);

    buttonEd[0] = new Sprite(*buttonEdTex[0]);
    buttonEd[1] = new Sprite(*buttonEdTex[1]);
    buttonEd[2] = new Sprite(*buttonEdTex[2]);
    buttonEd[3] = new Sprite(*buttonEdTex[3]);

    buttonSc[0] = new Sprite(*buttonScTex[0]);
    buttonSc[1] = new Sprite(*buttonScTex[1]);
    buttonSc[2] = new Sprite(*buttonScTex[2]);
    buttonSc[3] = new Sprite(*buttonScTex[3]);
    buttonSc[4] = new Sprite(*buttonScTex[4]);
    buttonSc[5] = new Sprite(*buttonScTex[5]);

    buttonPl[0] = new Sprite(*buttonPlTex[0]);
    buttonPl[1] = new Sprite(*buttonPlTex[1]);
    buttonPl[2] = new Sprite(*buttonPlTex[2]);
    buttonPl[3] = new Sprite(*buttonPlTex[3]);

    buttonOp[0] = new Sprite(*buttonOpTex[0]);
    buttonOp[1] = new Sprite(*buttonOpTex[1]);
    buttonOp[2] = new Sprite(*buttonOpTex[2]);
    buttonOp[3] = new Sprite(*buttonOpTex[3]);
    buttonOp[4] = new Sprite(*buttonOpTex[4]);
    buttonOp[5] = new Sprite(*buttonOpTex[5]);
    buttonOp[6] = new Sprite(*buttonOpTex[6]);
    buttonOp[7] = new Sprite(*buttonOpTex[7]);
    buttonOp[8] = new Sprite(*buttonOpTex[3]);
    buttonOp[9] = new Sprite(*buttonOpTex[4]);

    buttonRet = new Sprite(*buttonRetTex);

    blackBorder = new RectangleShape;
    blackBorder->setFillColor(Color::Black);

    title->setPosition(320, -80);

    title->setOrigin(300, 70);

    button[0]->setPosition(-100, -80);
    button[1]->setPosition(320, -80);
    button[2]->setPosition(740, -80);
    button[3]->setPosition(-100, 700);
    button[4]->setPosition(320, 700);
    button[5]->setPosition(740, 700);

    buttonEd[0]->setPosition(130, 160);
    buttonEd[1]->setPosition(130, 280);
    buttonEd[2]->setPosition(260, 160);
    buttonEd[3]->setPosition(260, 280);

    buttonSc[0]->setPosition(130, 80);
    buttonSc[1]->setPosition(130, 200);
    buttonSc[2]->setPosition(130, 320);
    buttonSc[3]->setPosition(260, 72);
    buttonSc[4]->setPosition(260, 192);
    buttonSc[5]->setPosition(260, 320);

    buttonPl[0]->setPosition(130, 160);
    buttonPl[1]->setPosition(130, 280);
    buttonPl[2]->setPosition(260, 160);
    buttonPl[3]->setPosition(260, 280);

    buttonOp[0]->setPosition(160, 100);
    buttonOp[1]->setPosition(320, 100);
    buttonOp[2]->setPosition(480, 100);
    buttonOp[3]->setPosition(240, 240);
    buttonOp[4]->setPosition(400, 240);
    buttonOp[5]->setPosition(8, 30);
    buttonOp[6]->setPosition(8, 170);
    buttonOp[7]->setPosition(8, 310);
    buttonOp[8]->setPosition(240, 390);
    buttonOp[9]->setPosition(400, 390);

    buttonRet->setPosition(41, 443);

    button[0]->setOrigin(77, 50);
    button[1]->setOrigin(77, 50);
    button[2]->setOrigin(76, 50);
    button[3]->setOrigin(77, 50);
    button[4]->setOrigin(76, 50);
    button[5]->setOrigin(77, 50);

    buttonEd[0]->setOrigin(98, 55);
    buttonEd[1]->setOrigin(98, 55);
    buttonEd[2]->setOrigin(0, 20);
    buttonEd[3]->setOrigin(0, 37);

    buttonSc[0]->setOrigin(98, 55);
    buttonSc[1]->setOrigin(98, 55);
    buttonSc[2]->setOrigin(98, 55);
    buttonSc[3]->setOrigin(0, 37);
    buttonSc[4]->setOrigin(0, 36);
    buttonSc[5]->setOrigin(0, 43);

    buttonPl[0]->setOrigin(98, 55);
    buttonPl[1]->setOrigin(98, 55);
    buttonPl[2]->setOrigin(0, 37);
    buttonPl[3]->setOrigin(0, 42);

    buttonOp[0]->setOrigin(60, 33);
    buttonOp[1]->setOrigin(59, 33);
    buttonOp[2]->setOrigin(60, 33);
    buttonOp[3]->setOrigin(60, 33);
    buttonOp[4]->setOrigin(60, 33);
    buttonOp[5]->setOrigin(0, 25);
    buttonOp[6]->setOrigin(0, 20);
    buttonOp[7]->setOrigin(0, 20);
    buttonOp[8]->setOrigin(60, 33);
    buttonOp[9]->setOrigin(60, 33);

    buttonRet->setOrigin(37, 33);

    tileMap = new VertexArray(Quads);

    LoadTilemap();

    gradient[0] = new VertexArray(Quads, 4);
    gradient[1] = new VertexArray(Quads, 4);
    gradient[2] = new VertexArray(Quads, 4);
    gradient[3] = new VertexArray(Quads, 4);

    LoadBackgrounds();

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

    particle = new Texture;

    if (!particle->loadFromFile("Data/Gfx/TitleScreen/Shining.png"))
        allright = false;

    if (!bushImg.loadFromFile("Data/Gfx/Sceneries/Scenery_GreenBush.png"))
        allright = false;

    bushImg.createMaskFromColor(Color::Magenta);

    bush = new Texture;
    bush->loadFromImage(bushImg);

    titleShine = new Emitter(600, 140);
    titleShine->setPosition(20, 16);

    greenBush = new Scenery_Bush(*bush);
    greenBush->setPosition(Vector2f(364, 386));

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/Menu1.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &mouseSound);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/Menu2.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &clickSound);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/EnterLevel.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &enterLevel);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem,
                                     "Data/Sfx/1Up.wav",
                                     FMOD_DEFAULT,
                                     NULL,
                                     &createScenario);

    if (result != FMOD_OK)
        allright = false;

    {
        FMOD_BOOL isPlayin;

        FMOD_Channel_IsPlaying(musicChannel, &isPlayin);

        if (!isPlayin)
        {
            result = FMOD_System_PlaySound(soundSystem,
                                  static_cast<FMOD_CHANNELINDEX>(20),
                                  music,
                                  0,
                                  &musicChannel);

            if (result != FMOD_OK)
                allright = false;
        }
    }

    return allright;
}

static void UpdateAssets()
{
    fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

    if (!animDone)
    {
        if (timeb > 80 && timeb < 175)
        {
            if (titleSpeed > 0)
            {
                title->move(0, titleSpeed);
                titleSpeed -= 0.15;
            }
            else
            {
                title->setPosition(320, 87);
                titleSpeed = 0;
            }
        }
        else if (timeb > 175)
        {
            if (buttonSpeed[0] > 0)
            {
                button[0]->move(buttonSpeed[0], buttonSpeed[0] * 1.3);
                buttonSpeed[0] -= 0.25;
            }
            else
            {
                button[0]->setPosition(148, 242);
                buttonSpeed[0] = 0;
            }

            if (timeb > 190)
            {
                if (buttonSpeed[1] > 0)
                {
                    button[1]->move(0, buttonSpeed[1] * 1.3);
                    buttonSpeed[1] -= 0.25;
                }
                else
                {
                    button[1]->setPosition(320, 242);
                    buttonSpeed[1] = 0;
                }
            }
            if (timeb > 205)
            {
                if (buttonSpeed[2] > 0)
                {
                    button[2]->move(-buttonSpeed[2], buttonSpeed[2] * 1.3);
                    buttonSpeed[2] -= 0.25;
                }
                else
                {
                    button[2]->setPosition(492, 242);
                    buttonSpeed[2] = 0;
                }
            }
            if (timeb > 210)
            {
                if (buttonSpeed[3] > 0)
                {
                    button[3]->move(buttonSpeed[3], buttonSpeed[3] * -1.3);
                    buttonSpeed[3] -= 0.25;
                }
                else
                {
                    button[3]->setPosition(148, 378);
                    buttonSpeed[3] = 0;
                }
            }
            if (timeb > 225)
            {
                if (buttonSpeed[4] > 0)
                {
                    button[4]->move(0, buttonSpeed[4] * -1.3);
                    buttonSpeed[4] -= 0.25;
                }
                else
                {
                    button[4]->setPosition(320, 378);
                    buttonSpeed[4] = 0;
                }
            }
            if (timeb > 240)
            {
                if (buttonSpeed[5] > 0)
                {
                    button[5]->move(-buttonSpeed[5], buttonSpeed[5] * -1.3);
                    buttonSpeed[5] -= 0.25;
                }
                else
                {
                    button[5]->setPosition(492, 378);
                    buttonSpeed[5] = 0;

                    animDone = true;
                }
            }
        }
    }
    else
    {
        unsigned int i;

        float scale = buttonRet->getScale().x;

        switch (menu)
        {
            case MAINMENU :

                for (i = 0; i < 6; i++)
                {
                    float scale = button[i]->getScale().x;

                    if (buttonSelect[i])
                    {
                        if (scale > SIZESELECTED + 0.05)
                            scale -= 0.05;
                        else
                            scale = SIZESELECTED;
                    }
                    else
                    {
                        if (scale < 0.95)
                            scale += 0.05;
                        else
                            scale = 1;
                    }

                    button[i]->setScale(scale, scale);
                }

                break;

            case EDITMENU :

                for (i = 0; i < 2; i++)
                {
                    float scale = buttonEd[i]->getScale().x;

                    if (buttonSelect[i])
                    {
                        if (scale > SIZESELECTED + 0.05)
                            scale -= 0.05;
                        else
                            scale = SIZESELECTED;
                    }
                    else
                    {
                        if (scale < 0.95)
                            scale += 0.05;
                        else
                            scale = 1;
                    }

                    buttonEd[i]->setScale(scale, scale);
                }

                if (buttonSelect[7])
                {
                    if (scale > SIZESELECTED + 0.05)
                        scale -= 0.05;
                    else
                        scale = SIZESELECTED;
                }
                else
                {
                    if (scale < 0.95)
                        scale += 0.05;
                    else
                        scale = 1;
                }

                buttonRet->setScale(scale, scale);

                break;

            case CREATESCENARIOMENU :

                for (i = 0; i < 3; i++)
                {
                    float scale = buttonSc[i]->getScale().x;

                    if (buttonSelect[i])
                    {
                        if (scale > SIZESELECTED + 0.05)
                            scale -= 0.05;
                        else
                            scale = SIZESELECTED;
                    }
                    else
                    {
                        if (scale < 0.95)
                            scale += 0.05;
                        else
                            scale = 1;
                    }

                    buttonSc[i]->setScale(scale, scale);
                }

                if (buttonSelect[7])
                {
                    if (scale > SIZESELECTED + 0.05)
                        scale -= 0.05;
                    else
                        scale = SIZESELECTED;
                }
                else
                {
                    if (scale < 0.95)
                        scale += 0.05;
                    else
                        scale = 1;
                }

                buttonRet->setScale(scale, scale);

                break;

            case PLAYSCENARIOMENU :

                for (i = 0; i < 2; i++)
                {
                    float scale = buttonPl[i]->getScale().x;

                    if (buttonSelect[i])
                    {
                        if (scale > SIZESELECTED + 0.05)
                            scale -= 0.05;
                        else
                            scale = SIZESELECTED;
                    }
                    else
                    {
                        if (scale < 0.95)
                            scale += 0.05;
                        else
                            scale = 1;
                    }

                    buttonPl[i]->setScale(scale, scale);
                }

                if (buttonSelect[7])
                {
                    if (scale > SIZESELECTED + 0.05)
                        scale -= 0.05;
                    else
                        scale = SIZESELECTED;
                }
                else
                {
                    if (scale < 0.95)
                        scale += 0.05;
                    else
                        scale = 1;
                }

                buttonRet->setScale(scale, scale);

                break;

            case OPTIONMENU :

                for (i = 0; i < 5; i++)
                {
                    float scale = buttonOp[i]->getScale().x;

                    if (buttonSelect[i])
                    {
                        if (scale > SIZESELECTED + 0.05)
                            scale -= 0.05;
                        else
                            scale = SIZESELECTED;
                    }
                    else
                    {
                        if (scale < 0.95)
                            scale += 0.05;
                        else
                            scale = 1;
                    }

                    buttonOp[i]->setScale(scale, scale);
                }

                for (i = 8; i < 10; i++)
                {
                    float scale = buttonOp[i]->getScale().x;

                    if (buttonSelect[i])
                    {
                        if (scale > SIZESELECTED + 0.05)
                            scale -= 0.05;
                        else
                            scale = SIZESELECTED;
                    }
                    else
                    {
                        if (scale < 0.95)
                            scale += 0.05;
                        else
                            scale = 1;
                    }

                    buttonOp[i]->setScale(scale, scale);
                }

                if (buttonSelect[7])
                {
                    if (scale > SIZESELECTED + 0.05)
                        scale -= 0.05;
                    else
                        scale = SIZESELECTED;
                }
                else
                {
                    if (scale < 0.95)
                        scale += 0.05;
                    else
                        scale = 1;
                }

                buttonRet->setScale(scale, scale);

                break;
        }
    }
}

static void LoadTilemap()
{
    register unsigned int i;

    for (i = 0; i < 20; i++)
    {
        if (i == 0)
        {
            tileMap->append(Vertex(Vector2f(0, 416), Color::White, Vector2f(0, 0)));
            tileMap->append(Vertex(Vector2f(32, 416), Color::White, Vector2f(32, 0)));
            tileMap->append(Vertex(Vector2f(32, 448), Color::White, Vector2f(32, 32)));
            tileMap->append(Vertex(Vector2f(0, 448), Color::White, Vector2f(0, 32)));
        }
        else if (i == 19)
        {
            tileMap->append(Vertex(Vector2f(i * 32, 416), Color::White, Vector2f(64, 0)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 416), Color::White, Vector2f(96, 0)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 448), Color::White, Vector2f(96, 32)));
            tileMap->append(Vertex(Vector2f(i * 32, 448), Color::White, Vector2f(64, 32)));
        }
        else
        {
            tileMap->append(Vertex(Vector2f(i * 32, 416), Color::White, Vector2f(32, 0)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 416), Color::White, Vector2f(64, 0)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 448), Color::White, Vector2f(64, 32)));
            tileMap->append(Vertex(Vector2f(i * 32, 448), Color::White, Vector2f(32, 32)));
        }
    }

    for (i = 0; i < 20; i++)
    {
        if (i == 0)
        {
            tileMap->append(Vertex(Vector2f(0, 448), Color::White, Vector2f(0, 32)));
            tileMap->append(Vertex(Vector2f(32, 448), Color::White, Vector2f(32, 32)));
            tileMap->append(Vertex(Vector2f(32, 480), Color::White, Vector2f(32, 64)));
            tileMap->append(Vertex(Vector2f(0, 480), Color::White, Vector2f(0, 64)));
        }
        else if (i == 19)
        {
            tileMap->append(Vertex(Vector2f(i * 32, 448), Color::White, Vector2f(64, 32)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 448), Color::White, Vector2f(96, 32)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 480), Color::White, Vector2f(96, 64)));
            tileMap->append(Vertex(Vector2f(i * 32, 480), Color::White, Vector2f(64, 64)));
        }
        else
        {
            tileMap->append(Vertex(Vector2f(i * 32, 448), Color::White, Vector2f(32, 32)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 448), Color::White, Vector2f(64, 32)));
            tileMap->append(Vertex(Vector2f((i+1) * 32, 480), Color::White, Vector2f(64, 64)));
            tileMap->append(Vertex(Vector2f(i * 32, 480), Color::White, Vector2f(32, 64)));
        }
    }

    tileMap->append(Vertex(Vector2f(56, 320), Color::White, Vector2f(512, 320)));
    tileMap->append(Vertex(Vector2f(88, 320), Color::White, Vector2f(544, 320)));
    tileMap->append(Vertex(Vector2f(88, 416), Color::White, Vector2f(544, 416)));
    tileMap->append(Vertex(Vector2f(56, 416), Color::White, Vector2f(512, 416)));

    tileMap->append(Vertex(Vector2f(230, 352), Color::White, Vector2f(480, 352)));
    tileMap->append(Vertex(Vector2f(262, 352), Color::White, Vector2f(512, 352)));
    tileMap->append(Vertex(Vector2f(262, 416), Color::White, Vector2f(512, 416)));
    tileMap->append(Vertex(Vector2f(230, 416), Color::White, Vector2f(480, 416)));

    tileMap->append(Vertex(Vector2f(468, 384), Color::White, Vector2f(480, 288)));
    tileMap->append(Vertex(Vector2f(628, 384), Color::White, Vector2f(640, 288)));
    tileMap->append(Vertex(Vector2f(628, 416), Color::White, Vector2f(640, 320)));
    tileMap->append(Vertex(Vector2f(468, 416), Color::White, Vector2f(480, 320)));
}

static void LoadBackgrounds()
{
    for (register unsigned int i = 0; i < 4; i++)
    {
        (*gradient[i])[0].position = Vector2f(0, 0);
        (*gradient[i])[1].position = Vector2f(640, 0);
        (*gradient[i])[2].position = Vector2f(640, 480);
        (*gradient[i])[3].position = Vector2f(0, 480);
    }

    (*gradient[0])[0].color = Color(0, 111, 223);
    (*gradient[0])[1].color = Color(0, 111, 223);
    (*gradient[0])[2].color = Color(242, 253, 252);
    (*gradient[0])[3].color = Color(242, 253, 252);

    (*gradient[1])[0].color = Color(63, 72, 204);
    (*gradient[1])[1].color = Color(63, 72, 204);
    (*gradient[1])[2].color = Color(193, 233, 242);
    (*gradient[1])[3].color = Color(193, 233, 242);

    (*gradient[2])[0].color = Color(255, 128, 64);
    (*gradient[2])[1].color = Color(255, 128, 64);
    (*gradient[2])[2].color = Color(238, 232, 208);
    (*gradient[2])[3].color = Color(238, 232, 208);

    (*gradient[3])[0].color = Color(0, 85, 0);
    (*gradient[3])[1].color = Color(0, 85, 0);
    (*gradient[3])[2].color = Color(211, 231, 213);
    (*gradient[3])[3].color = Color(211, 231, 213);
}

LRESULT CALLBACK dialogNumLevels(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND handleEdit;
    TCHAR textBuffer[2];

    switch(message)
    {
        case WM_INITDIALOG :
            SendDlgItemMessage(windowHandle, 39, UDM_SETRANGE, 0, MAKELPARAM(2, 32));
            SendDlgItemMessage(windowHandle, 39, UDM_SETPOS, 0, 2);
            return TRUE;

        case WM_COMMAND :
            switch(wParam)
            {
                case 40 :
                    EndDialog(windowHandle, 0);

                    handleEdit = GetDlgItem(windowHandle, 38);
                    GetWindowText(handleEdit, textBuffer, 2);

                    levels = max(min(atoi(textBuffer), 32), 2);

                    return TRUE;
                case 41 :
                    EndDialog(windowHandle, 0);

                    return TRUE;
            }

            break;
    }

    return FALSE;
}

LRESULT CALLBACK dialogNumLifes(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND handleEdit;
    TCHAR textBuffer[2];

    switch(message)
    {
        case WM_INITDIALOG :
            SendDlgItemMessage(windowHandle, 69, UDM_SETRANGE, 0, MAKELPARAM(0, 99));
            SendDlgItemMessage(windowHandle, 69, UDM_SETPOS, 0, 4);
            return TRUE;

        case WM_COMMAND :
            switch(wParam)
            {
                case 70 :
                    EndDialog(windowHandle, 0);

                    handleEdit = GetDlgItem(windowHandle, 68);
                    GetWindowText(handleEdit, textBuffer, 2);

                    lifes = max(min(atoi(textBuffer), 99), 0);

                    return TRUE;
                case 71 :
                    EndDialog(windowHandle, 0);

                    return TRUE;
            }

            break;
    }

    return FALSE;
}

static bool RewriteParameters()
{
    ofstream parmFile;

    parmFile.open("Data/Save/Parms.dat");

    if (parmFile.good())
    {
        parmFile << windowScale << endl;
        parmFile << editorMusic << endl;
        parmFile << editorMouseScrolling << endl;

        parmFile.close();
    }
    else
        return false;

    return true;
}

bool checkLoadResources(ifstream& levelFile, LPCSTR filename)
{
    ifstream checkFile;

    TCHAR filePath[MAX_PATH];
    TCHAR getString[MAX_PATH];
    TCHAR messageText[512];

    char CMLid[4];

    strcpy(filePath, filename);

    PathRemoveFileSpec(filePath);
    SetCurrentDirectory(filePath);

    levelFile.read(CMLid, 4);

    if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'L')
    {
        MessageBox(NULL, "Error ! This file is not a valid CML Level !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

        return false;
    }

    if (CMLid[3] > EDITOR_VERSION)
    {
        MessageBox(NULL, "Error ! This Level was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

        return false;
    }

    for (register unsigned int i = 0; i < 5; i++)
    {
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

            checkFile.open(getString);

            if (!checkFile.good())
            {
                sprintf(messageText, "Error ! The resource is not found :\n%s !", getString);

                MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONQUESTION | MB_OK);

                return false;
            }
            else
                checkFile.close();
        }
    }

    // Reset the current directory :
    GetModuleFileName(NULL, filePath, MAX_PATH);
    PathRemoveFileSpec(filePath);
    SetCurrentDirectory(filePath);

    return true;
}
