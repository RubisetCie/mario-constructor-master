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

#include "../../Headers/gglobals.hpp"

#define FADESPEED 4

using namespace sf;
using namespace std;

/*static bool InitAssets();

Texture* titleTex;
Sprite* title;

FMOD_SOUND* mouseSound;
FMOD_SOUND* clickSound;
FMOD_SOUND* enterLevel;
FMOD_SOUND* createScenario;

bool menuBlock;

unsigned int timeb;

extern Uint8 fadeAlpha;
extern RectangleShape* fadeRect;*/

bool Scene::Title()
{
    /*bool exitLoop = false;

    if (!InitAssets())
    {
        MessageBox(NULL, "Failed to initialize the assets on the Title !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);

        mainWindow->close();

        exitLoop = true;
    }

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

        fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

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
        {
            mainTexture.clear(Color::Black);
            mainTexture.draw(*loading);
        }

        mainTexture.display();

        mainWindow->clear(Color::Black);
        mainWindow->draw(textureRender);
        mainWindow->display();
    }

    delete fadeRect;

    if (circleFadeTex != NULL)
        delete circleFadeTex;

    if (circleSpr != NULL)
        delete circleSpr;

    delete titleTex;

    delete blackBorder;

    delete title;

    FMOD_Sound_Release(clickSound);
    FMOD_Sound_Release(mouseSound);
    FMOD_Sound_Release(enterLevel);
    FMOD_Sound_Release(createScenario);*/

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    /*FMOD_RESULT result;

    menuBlock = false;
    fadeCircle = false;

    timeb = 0;

    titleTex = new Texture;

    if (!titleTex->loadFromFile("Data/Gfx/TitleScreen/Title.png"))
        allright = false;

    circleFadeTex = NULL;
    circleSpr = NULL;

    title = new Sprite(*titleTex);

    blackBorder = new RectangleShape;
    blackBorder->setFillColor(Color::Black);

    title->setPosition(320, -80);
    title->setOrigin(300, 70);

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(musicChannel, &isPlaying);

        if (!isPlaying)
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
        allright = false;*/

    return allright;
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
