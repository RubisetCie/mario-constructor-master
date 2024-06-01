/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

extern "C"
{
    #include <fmod.h>
    #include <windows.h>
}

#include "../../Headers/Core/scene.hpp"
#include "../../Headers/globals.hpp"

#define FADESPEED 4

using namespace sf;
using namespace std;

static bool InitAssets();

Texture* logoRubisEtCie;
Texture* logoMarioVariable3410;

Sprite* sprRubisEtCie;
Sprite* sprMarioVariable3410;

RectangleShape* fadeRect;

Uint8 fadeAlpha;

bool Scene::Logo()
{
    typedef enum Logos {RUBISETCIE,
                        MARIOVARIABLE3410}
                        Logos;

    typedef enum States {ENTRANCE_R,
                         END_R,
                         ENTRANCE_M,
                         END_M}
                         States;

    bool exitLoop = false;

    if (!InitAssets())
    {
        MessageBox(NULL, "Failed to initialize the assets on the Logo !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);

        mainWindow->close();

        exitLoop = true;
    }

    Logos logoNumb = RUBISETCIE;
    States state = ENTRANCE_R;

    unsigned int timeb = 0;

    Event eventSystem;

    /**The following code do : Define the main loop of the scene.*/

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
                        exitLoop = true;

                        mainWindow->close();

                        break;
                    }

                    if (state == ENTRANCE_R)
                    {
                        timeb = 0;
                        state = END_R;
                    }
                    else if (state == ENTRANCE_M)
                    {
                        timeb = 0;
                        state = END_M;
                    }

                    break;

                case Event::MouseMoved          :

                    mpos_absolute = Vector2i((eventSystem.mouseMove.x - wpos.x) / windowScale, (eventSystem.mouseMove.y - wpos.y) / windowScale);

                    break;

                case Event::MouseButtonPressed  :

                    if (state == ENTRANCE_R)
                    {
                        timeb = 0;
                        state = END_R;
                    }
                    else if (state == ENTRANCE_M)
                    {
                        timeb = 0;
                        state = END_M;
                    }

                    break;

                case Event::Resized :

                    windowScale = min(eventSystem.size.width / 640.0, eventSystem.size.height / 480.0);
                    wpos = Vector2f((eventSystem.size.width - (windowScale * 640)) / 2, (eventSystem.size.height - (windowScale * 480)) / 2);

                    mainWindow->setView(View(FloatRect(0, 0, eventSystem.size.width, eventSystem.size.height)));

                    textureRender.setPosition(wpos);
                    textureRender.setScale(windowScale, windowScale);

                    break;

                default                 : break;
            }
        }

        switch (state)
        {
            case ENTRANCE_R :

                if (fadeAlpha > FADESPEED)
                    fadeAlpha -= FADESPEED;
                else
                    fadeAlpha = 0;

                if (timeb > 200)
                    state = END_R;

                break;

            case END_R      :

                if (fadeAlpha < 255-FADESPEED)
                    fadeAlpha += FADESPEED;
                else
                {
                    fadeAlpha = 255;
                    timeb = 0;
                    logoNumb = MARIOVARIABLE3410;
                    state = ENTRANCE_M;
                }

                break;

            case ENTRANCE_M :

                if (fadeAlpha > FADESPEED)
                    fadeAlpha -= FADESPEED;
                else
                    fadeAlpha = 0;

                if (timeb > 200)
                {
                    timeb = 0;
                    state = END_M;
                }

                break;

            case END_M      :

                if (fadeAlpha < 255-FADESPEED)
                    fadeAlpha += FADESPEED;
                else
                {
                    targetScene = SCENE_TITLE;

                    exitLoop = true;
                }

                break;
        }

        fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

        timeb++;

        switch (logoNumb)
        {
            case RUBISETCIE         : mainTexture.draw(*sprRubisEtCie); break;
            case MARIOVARIABLE3410  : mainTexture.draw(*sprMarioVariable3410); break;
        }

        if (fadeAlpha > 0)
            mainTexture.draw(*fadeRect);

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

    delete logoRubisEtCie;
    delete logoMarioVariable3410;

    delete sprRubisEtCie;
    delete sprMarioVariable3410;

    delete fadeRect;

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    fadeAlpha = 255;

    logoRubisEtCie = new Texture;
    if (!logoRubisEtCie->loadFromFile("Data/Gfx/Logos/Logo_RubisEtCie.jpg"))
        allright = false;

    logoMarioVariable3410 = new Texture;
    if (!logoMarioVariable3410->loadFromFile("Data/Gfx/Logos/Logo_MarioVariable3410.jpg"))
        allright = false;

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

    sprRubisEtCie = new Sprite(*logoRubisEtCie);
    sprMarioVariable3410 = new Sprite(*logoMarioVariable3410);

    {
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

        result = FMOD_System_PlaySound(soundSystem,
                              static_cast<FMOD_CHANNELINDEX>(20),
                              music,
                              0,
                              &musicChannel);

        if (result != FMOD_OK)
            allright = false;
    }

    return allright;
}
