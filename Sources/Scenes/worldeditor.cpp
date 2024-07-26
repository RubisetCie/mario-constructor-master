/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <fstream>
#include <cmath>
#include <cstdlib>

#ifdef DEBUGMODE
#include <iostream>
#endif

extern "C"
{
    #include <fmod.h>
    #ifndef LINUX
    #include <windows.h>
    #include <shlwapi.h>
    #include <shlobj.h>
    #else
    #include <unistd.h>
    #include <libgen.h>
    #endif
}

#ifdef LINUX
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QMessageBox>
#endif

#include "../../Headers/globals.hpp"
#include "../../Headers/Core/matrix.hpp"
#include "../../Headers/Graphics/button.hpp"
#include "../../Headers/Graphics/case.hpp"
#include "../../Headers/Graphics/lightentity.hpp"
#include "../../Headers/Graphics/markerentity.hpp"

#define FADESPEED 4
#define CAMSPEED 8
#define CAMACCEL 0.25
#define CAMDECEL 0.15
#define MOUSEINFLUENCE 24

using namespace sf;
using namespace std;

static bool InitAssets();
static void UpdateAssets();

static void Erasement_Light();
static void Erasement_Marker();

static void mergeTextures();

static void checkResources(const char* worldURL, bool saveAs);
bool checkwLoadResources(ifstream& worldFile, const char* filename);
static void addNewResource(string filename, unsigned short id);
static void removeResource(unsigned short id);

typedef struct TextureData
{
    RectangleShape rectangle;
    unsigned int id;
} TextureData;

typedef struct EyecandyData
{
    Sprite sprite;
    unsigned int id;
} EyecandyData;

list <TextureData*> listTextures;
list <EyecandyData*> listEyecandies;
list <Light_Entity*> listLights;
list <Marker_Entity*> listMarkers;

vector <Vertex> worldMarkerLink;

string worldDir;
string* wresourcesArray[2];

char wfilegrab[MAX_PATH];

void* selectedEntity;

Texture* worldGridTex[2];
Texture* worldCursor;
Texture* worldPanelTex;
Texture* worldButtonTex[2];
Texture* worldText[3];
Texture* worldInfo[4];
Texture* worldMoveable;
Texture* worldTexmap;
Texture* worldEyemap;
Texture* worldReturnButtonTex;
Texture* worldPanelTexb;

RectangleShape* worldGrid[2];
RectangleShape* worldBack;
RectangleShape* worldTexSelection;

Sprite* selectionCandy;
Sprite* worldCursorSpr;
Sprite* worldTexmapSpr;
Sprite* worldEyemapSpr;
Sprite* worldTextSpr[6];
Sprite* worldPanel[2];

Button* worldReturnButton;
Button* worldButton[10];
Button* worldInfoButton[30];

Vertex panelwInfo[36];

Case* worldEyeCases[81];

Matrix* matrixMarker;

View* worldCamera;

Vector2f worldtexturePos;
Vector2i worldcamPos;
Vector2f worldCamSpeed;

Vector2i mapScale;

unsigned int edlastButton;

unsigned char worldBackground;
unsigned char worldMusic;
unsigned char worldLives;

Color currentLightColor;

bool currentLight_TexIllum;
bool currentLight_EyeIllum;
bool currentMarker_IsLevel;
unsigned int currentMarker_Number;
unsigned int currentTexture;

FMOD_SOUND* worldedMusic;

FMOD_SOUND* edSwitchVis;
FMOD_SOUND* edbuttonClick;
FMOD_SOUND* editemPut;
FMOD_SOUND* editemDelete;
FMOD_SOUND* editemSelection;
FMOD_SOUND* edsaveSound;

FMOD_CHANNEL* previewWorldMusicChannel;

bool panelVisible;
bool gobackToTitle;
bool blockWorldMouse;
bool blockWorldEditor;
bool textFadeOut;
bool placementAllowed;

unsigned char flickTimer;

enum EditType {NONE, TEXTURE, EYECANDY, LIGHT, MARKER} currentEditing;
enum MenuType {EDITOR, TEXTURES, EYECANDIES, TITLE, MUSIC} currentScreen;

extern Uint8 fadeAlpha;
extern RectangleShape* fadeRect;

extern Text* helpText;
extern Font* helpFont;

static unsigned int GetTextureNum(unsigned int input);

static void Button_Textures();
static void Button_Eyecandies();
static void Button_Lights();
static void Button_Markers();
static void Button_Title();

static void Button_Return();

static void Eyecandy_Chosen(unsigned int id);

static void Light_Color();
static void Light_TexIllumin();
static void Light_EyeIllumin();

static void Mark_SetTurn();
static void Mark_SetLevel();

static void Background_Default();
static void Background_Import();

static void Music_Edit();
static void Music_World1();
static void Music_Cave();
static void Music_Underground();
static void Music_Rockland();
static void Music_Starland();
static void Music_StarPlatforms();
static void Music_Underwater();
static void Music_SelectSave();
static void Music_GhostHouse();
static void Music_Castle1();
static void Music_Castle2();
static void Music_Yoshi1();
static void Music_Yoshi2();
static void Music_Map1();
static void Music_Map2();
static void Music_Map3();
static void Music_Fastrun1();
static void Music_Fastrun2();
static void Music_InDaKitchen();
static void Music_Yky();
static void Music_YetBonus();
static void Music_Apoplexy();
static void Music_Import();

static void Size_Edit();

static void Info_New();
static void Info_Save();
static void Info_SaveAs();
static void Info_Load();

static void World_Save(const string& filename);
static void World_Load(const string& filename);

bool Scene::WorldEditor()
{
    bool exitLoop = false;

    if (!InitAssets())
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to initialize the assets on the World Editor !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Assets Error !"), QStringLiteral("Failed to initialize the assets on the World Editor !"), QMessageBox::Ok);
        messageBox.exec();
#endif
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

                case Event::KeyPressed :

                    if (blockWorldEditor)
                        break;

                    switch (eventSystem.key.code)
                    {
                        case Keyboard::Escape :
                        {
                            extern bool showCursor;
                            mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
                            if (MessageBox(NULL, "Do you want to back to the Title Screen ?\nAll unsaved work will be lost !", "Back to the Title ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
#else
                            QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Back to the Title ?"), QStringLiteral("Do you want to back to the Title Screen ?\nAll unsaved work will be lost !"), QMessageBox::Yes | QMessageBox::No);
                            if (messageBox.exec() == QMessageBox::Yes)
#endif
                            {
                                gobackToTitle = true;
                                blockWorldEditor = true;

                                fadeRect->setPosition(worldcamPos.x-320, worldcamPos.y-240);
                            }
                            mainWindow->setMouseCursorVisible(showCursor);
                            break;
                        }
                        case Keyboard::Space :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(8), edSwitchVis, 0, NULL);

                            if (panelVisible)
                                panelVisible = false;
                            else
                                panelVisible = true;

                            break;

                        case Keyboard::Delete :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            switch (currentEditing)
                            {
                                case LIGHT  : Erasement_Light(); break;
                                case MARKER : Erasement_Marker(); break;
                                default     : break;
                            }

                            break;

                        case Keyboard::BackSpace :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            switch (currentEditing)
                            {
                                case LIGHT  : Erasement_Light(); break;
                                case MARKER : Erasement_Marker(); break;
                                default     : break;
                            }

                            break;

                        case Keyboard::S :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            if (eventSystem.key.control && worldtexturePos.x == -1)
                                Info_Save();

                            break;

                        case Keyboard::L :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            if (eventSystem.key.control && worldtexturePos.x == -1)
                                Info_Load();

                            break;
                        case Keyboard::N :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            if (eventSystem.key.control && worldtexturePos.x == -1)
                            {
                                extern bool showCursor;
                                mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
                                if (MessageBox(NULL, "You are going to reset the World.\nAll your placements will be deleted.\nAre you sure ?", "New World", MB_ICONQUESTION | MB_TASKMODAL | MB_YESNO) == IDYES)
#else
                                QMessageBox messageBox(QMessageBox::Question, QStringLiteral("New World"), QStringLiteral("You are going to reset the World.\nAll your placements will be deleted.\nAre you sure ?"), QMessageBox::Yes | QMessageBox::No);
                                if (messageBox.exec() == QMessageBox::Yes)
#endif
                                    Info_New();
                                mainWindow->setMouseCursorVisible(showCursor);
                            }

                            break;
                        default : break;
                    }

                    break;
                case Event::MouseButtonPressed :
                    switch (eventSystem.mouseButton.button)
                    {
                        case Mouse::Left :

                        if (blockWorldEditor)
                            break;

                        switch (currentScreen)
                        {
                            case EDITOR :
                                switch (currentEditing)
                                {
                                    case TEXTURE :
                                        if (panelVisible)
                                        {
                                            if (mpos_absolute.x < 156 && mpos_absolute.y > 448)
                                                break;
                                        }

                                        worldtexturePos = Vector2f(roundf((mpos_relative.x - 8)/16) * 16, roundf((mpos_relative.y - 8)/16)* 16);

                                        if (worldtexturePos.x < 0)
                                            worldtexturePos.x = 0;

                                        if (worldtexturePos.y < 0)
                                            worldtexturePos.y = 0;

                                        #ifdef DEBUGMODE
                                        cout << "Mouse Click Position X : " << mpos_relative.x << endl;
                                        cout << "Mouse Click Position Y : " << mpos_relative.y << endl;
                                        #endif

                                        break;
                                    case EYECANDY :
                                        if (panelVisible)
                                        {
                                            if (mpos_absolute.x < 156 && mpos_absolute.y > 448)
                                                break;
                                        }

                                        if (listEyecandies.size() == 128)
                                        {
#ifndef LINUX
                                            MessageBox(NULL, "You cannot place more than 128 Eyecandies !", "Eyecandies Overflow !", MB_TASKMODAL | MB_OK | MB_ICONEXCLAMATION);
#else
                                            QMessageBox messageBox(QMessageBox::Information, QStringLiteral("Eyecandies Overflow !"), QStringLiteral("You cannot place more than 128 Eyecandies !"), QMessageBox::Ok);
                                            messageBox.exec();
#endif
                                        }
                                        else
                                        {
                                            Vector2u eyecandySize(worldEyecandies[currentTexture]->getSize());
                                            Vector2f pos(worldTexSelection->getPosition());

                                            bool placement = true;

                                            for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
                                            {
                                                FloatRect eyeRect((*it)->sprite.getGlobalBounds());

                                                if (eyeRect.intersects(FloatRect(pos, static_cast<Vector2f>(eyecandySize))))
                                                {
                                                    placement = false;
                                                    break;
                                                }
                                            }

                                            if (!placement)
                                                break;

                                            EyecandyData* newEyecandy = new EyecandyData {Sprite(*worldEyecandies[currentTexture]), currentTexture};
                                            newEyecandy->sprite.setPosition(pos);

                                            listEyecandies.emplace_back(newEyecandy);

                                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), editemPut, 0, NULL);
                                        }

                                        break;
                                    case LIGHT :

                                        if (!placementAllowed)
                                            break;

                                        if (panelVisible)
                                        {
                                            if ((mpos_absolute.x < 156 && mpos_absolute.y > 448) || (mpos_absolute.x > 534 && mpos_absolute.y > 448))
                                                break;
                                        }

                                        if (listLights.size() == 64)
                                        {
#ifndef LINUX
                                            MessageBox(NULL, "You cannot place more than 64 Lamps !", "Lights Overflow !", MB_TASKMODAL | MB_OK | MB_ICONEXCLAMATION);
#else
                                            QMessageBox messageBox(QMessageBox::Information, QStringLiteral("Lights Overflow !"), QStringLiteral("You cannot place more than 64 Lamps !"), QMessageBox::Ok);
                                            messageBox.exec();
#endif
                                        }
                                        else
                                        {
                                            Vector2f currentPlacement(mpos_relative - Vector2f(16, 16));

                                            if (currentPlacement.x < 0)
                                                currentPlacement.x = 0;
                                            else if (currentPlacement.x > (mapScale.x * 32) - 32)
                                                currentPlacement.x = (mapScale.x * 32) - 32;

                                            if (currentPlacement.y < 0)
                                                currentPlacement.y = 0;
                                            else if (currentPlacement.y > (mapScale.y * 32) - 32)
                                                currentPlacement.y = (mapScale.y * 32) - 32;

                                            Light_Entity* newLight = new Light_Entity(entTex[0], worldMoveable, currentPlacement, currentLightColor, currentLight_TexIllum, currentLight_EyeIllum);

                                            listLights.emplace_back(newLight);

                                            selectedEntity = newLight;
                                            selectionCandy->setPosition(currentPlacement);

                                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), editemPut, 0, NULL);
                                        }

                                        break;

                                    case MARKER :
                                        {
                                        Vector2f cursorPosu(worldCursorSpr->getPosition());

                                        if (panelVisible)
                                        {
                                            if ((mpos_absolute.x < 156 && mpos_absolute.y > 448) || (mpos_absolute.x > 566 && mpos_absolute.y > 448))
                                                break;
                                        }

                                        if (matrixMarker->getValue(cursorPosu.x / 16, cursorPosu.y / 16) == 0)
                                        {
                                            if (listMarkers.size() == 32)
                                            {
#ifndef LINUX
                                                MessageBox(NULL, "You cannot place more than 32 Markers !", "Markers Overflow !", MB_TASKMODAL | MB_OK | MB_ICONEXCLAMATION);
#else
                                                QMessageBox messageBox(QMessageBox::Information, QStringLiteral("Markers Overflow !"), QStringLiteral("You cannot place more than 32 Markers !"), QMessageBox::Ok);
                                                messageBox.exec();
#endif
                                            }
                                            else
                                            {
                                                listMarkers.emplace_back(new Marker_Entity(entTex[1], cursorPosu - Vector2f(12, 10), currentMarker_IsLevel, currentMarker_Number));

                                                currentMarker_Number++;

                                                worldMarkerLink.emplace_back(Vertex(cursorPosu, Color(128, 255, 255)));

                                                matrixMarker->setValue(cursorPosu.x / 16, cursorPosu.y / 16, 1);

                                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), editemPut, 0, NULL);
                                            }
                                        }
                                        else
                                        {
                                            for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
                                            {
                                                Vector2f entityPos((*it)->getPosition());

                                                if (entityPos == cursorPosu - Vector2f(12, 10))
                                                {
                                                    if (selectedEntity != (*it))
                                                    {
                                                        selectedEntity = (*it);
                                                        selectionCandy->setPosition(entityPos.x - 4, entityPos.y - 6);

                                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), editemSelection, 0, NULL);
                                                    }
                                                    else
                                                        selectedEntity = NULL;
                                                }
                                            }
                                        }
                                        }

                                        break;

                                    default : break;
                                }
                                break;
                            case TEXTURES :
                                    if ((mpos_absolute.x > 80 && mpos_absolute.x < 560 && mpos_absolute.y > 64 && mpos_absolute.y < 320) ||
                                        (mpos_absolute.x > 144 && mpos_absolute.x < 464 && mpos_absolute.y > 319 && mpos_absolute.y < 352) ||
                                        (mpos_absolute.x > 112 && mpos_absolute.x < 496 && mpos_absolute.y > 351 && mpos_absolute.y < 448))
                                    {
                                        Vector2f cursorPos(worldTexSelection->getPosition());
                                        unsigned int texturePos = (((cursorPos.x-80) - (worldcamPos.x-320)) / 32) + ((((cursorPos.y-64) - (worldcamPos.y-240)) / 32) * 15);

                                        currentTexture = GetTextureNum(texturePos);

                                        worldLightSpr[4]->setColor(Color(255, 255, 255, 128));
                                        worldLightSpr[2]->setColor(Color::White);

                                        currentEditing = TEXTURE;

                                        #ifdef DEBUGMODE
                                        cout << "CursorPos X : " << cursorPos.x << endl;
                                        cout << "CursorPos Y : " << cursorPos.y << endl;

                                        cout << "Worldcam X : " << worldcamPos.x << endl;
                                        cout << "Worldcam Y : " << worldcamPos.y << endl;

                                        cout << "The selected Tile is : " << texturePos << endl;
                                        cout << "The related Texture is : " << currentTexture << endl;
                                        #endif

                                        worldTexSelection->setTexture(worldTextures[currentTexture-1]);

                                        selectedEntity = NULL;

                                        currentScreen = EDITOR;
                                    }

                                    break;

                                default : break;
                            }

                            break;

                            case Mouse::Right :

                            if (currentScreen != EDITOR || blockWorldEditor)
                                break;

                            switch (currentEditing)
                            {
                                case TEXTURE :

                                    if (!listTextures.empty())
                                    {
                                        for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
                                        {
                                            Vector2f entityPos((*it)->rectangle.getPosition());
                                            Vector2f entitySize((*it)->rectangle.getSize());

                                            if (FloatRect(entityPos, entitySize).contains(mpos_relative))
                                            {
                                                delete *it;

                                                listTextures.erase(it);

                                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), editemDelete, 0, NULL);

                                                break;
                                            }
                                        }
                                    }
                                    break;

                                case EYECANDY :

                                    if (!listEyecandies.empty())
                                    {
                                        for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
                                        {
                                            FloatRect entitySize((*it)->sprite.getGlobalBounds());

                                            if (entitySize.contains(mpos_relative))
                                            {
                                                delete *it;

                                                listEyecandies.erase(it);

                                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), editemDelete, 0, NULL);

                                                break;
                                            }
                                        }
                                    }
                                    break;

                                case LIGHT :

                                    selectedEntity = NULL;
                                    break;

                                case MARKER :

                                    selectedEntity = NULL;
                                    break;

                                default : break;
                            }

                            break;

                            default : break;
                        }

                        break;
                case Event::MouseMoved :

                    mpos_absolute = Vector2i((eventSystem.mouseMove.x - wpos.x) / windowScale, (eventSystem.mouseMove.y - wpos.y) / windowScale);
                    mpos_relative = mainTexture.mapPixelToCoords(mpos_absolute);

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

        {
            bool focus = mainWindow->hasFocus();

            if (Mouse::isButtonPressed(Mouse::Left) && focus)
            {
                if (mousePressed < 2)
                    mousePressed++;
            }
            else
            {
                mousePressed = 0;
                blockWorldMouse = false;
            }
        }

        switch (currentScreen)
        {
            case EDITOR :

                if (!blockWorldEditor)
                    UpdateAssets();

                placementAllowed = true;

                if (worldBackground == 1)
                    mainTexture.clear(Color::Black);

                mainTexture.draw(*worldBack);

                renderPasses[0]->clear(Color::Transparent);
                renderPasses[4]->clear(Color::Transparent);

                worldLightSpr[1]->setPosition(worldcamPos.x - 320, worldcamPos.y - 240);
                worldLightSpr[2]->setPosition(worldcamPos.x - 320, worldcamPos.y - 240);
                worldLightSpr[3]->setPosition(worldcamPos.x - 320, worldcamPos.y - 240);
                worldLightSpr[4]->setPosition(worldcamPos.x - 320, worldcamPos.y - 240);

                renderPasses[0]->setView(*worldCamera);
                renderPasses[2]->setView(*worldCamera);
                renderPasses[3]->setView(*worldCamera);
                renderPasses[4]->setView(*worldCamera);

                if (!listLights.empty())
                {
                    if (currentEditing == LIGHT)
                    {
                        renderPasses[2]->clear(Color(96, 96, 96));
                        renderPasses[3]->clear(Color(96, 96, 96));
                    }
                    else
                    {
                        renderPasses[2]->clear(Color(192, 192, 192));
                        renderPasses[3]->clear(Color(192, 192, 192));
                    }

                    for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
                    {
                        if (!(*it)->m_littextures && !(*it)->m_liteyecandies)
                            continue;

                        Vector2f litPos((*it)->getPosition() + Vector2f(16, 16));
                        float litRad = (*it)->getRadius();

                        if ((litPos.x + litRad < worldcamPos.x - 320 && litPos.y + litRad < worldcamPos.y - 240 && litPos.x - litRad > worldcamPos.x + 320 && litPos.y - litRad > worldcamPos.y + 240))
                            continue;

                        worldLightSpr[0]->setPosition(litPos);

                        float lightradius = litRad / 48;

                        worldLightSpr[0]->setScale(lightradius, lightradius);
                        worldLightSpr[0]->setColor((*it)->m_lightcolor);

                        if ((*it)->m_littextures)
                            renderPasses[2]->draw(*worldLightSpr[0], BlendAdd);

                        if ((*it)->m_liteyecandies)
                            renderPasses[3]->draw(*worldLightSpr[0], BlendAdd);
                    }

                    renderPasses[2]->display();
                    renderPasses[3]->display();
                }
                else
                {
                    if (currentEditing == LIGHT)
                    {
                        renderPasses[2]->clear(Color(128, 128, 128));
                        renderPasses[3]->clear(Color(128, 128, 128));
                    }
                    else
                    {
                        renderPasses[2]->clear(Color::White);
                        renderPasses[3]->clear(Color::White);
                    }
                }

                if (!listEyecandies.empty())
                {
                    for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
                    {
                        FloatRect entityBound((*it)->sprite.getGlobalBounds());

                        if (entityBound.left + entityBound.width > worldcamPos.x-320 && entityBound.left < worldcamPos.x+320 && entityBound.top + entityBound.height > worldcamPos.y-240 && entityBound.top < worldcamPos.y+240)
                            renderPasses[4]->draw((**it).sprite);
                    }
                }

                if (!listTextures.empty())
                {
                    for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
                    {
                        Vector2f entityPos((*it)->rectangle.getPosition());
                        Vector2f entitySize((*it)->rectangle.getSize());

                        if (entityPos.x + entitySize.x > worldcamPos.x-320 && entityPos.x < worldcamPos.x+320 && entityPos.y + entitySize.y > worldcamPos.y-240 && entityPos.y < worldcamPos.y+240)
                            renderPasses[0]->draw((**it).rectangle);
                    }
                }

                renderPasses[0]->display();
                renderPasses[4]->display();

                if (useShaders)
                {
                    renderPasses[1]->clear(Color::Transparent);
                    renderPasses[5]->clear(Color::Transparent);

                    renderPasses[1]->setView(*worldCamera);
                    renderPasses[5]->setView(*worldCamera);

                    renderPassSpr[0]->setPosition(worldcamPos.x - 320, worldcamPos.y - 240);
                    renderPassSpr[1]->setPosition(worldcamPos.x - 320, worldcamPos.y - 240);

                    if (currentEditing != EYECANDY)
                        renderPasses[5]->draw(*worldLightSpr[2]);

                    if (currentEditing != TEXTURE)
                        renderPasses[5]->draw(*worldLightSpr[4]);

                    renderPasses[5]->display();

                    renderPasses[1]->draw(*renderPassSpr[0], glowShaderH);
                    renderPasses[1]->display();

                    mainTexture.draw(*renderPassSpr[1], glowShaderV);
                }

                renderPasses[0]->draw(*worldLightSpr[1], BlendMultiply);
                renderPasses[4]->draw(*worldLightSpr[3], BlendMultiply);

                mainTexture.draw(*worldLightSpr[4]);
                mainTexture.draw(*worldLightSpr[2]);

                if (!listLights.empty())
                {
                    for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
                    {
                        Vector2f entityPos((*it)->getPosition());

                        if (entityPos.x + 32 > worldcamPos.x-320 && entityPos.x < worldcamPos.x+320 && entityPos.y + 32 > worldcamPos.y-240 && entityPos.y < worldcamPos.y+240)
                        {
                            if (currentEditing == LIGHT)
                            {
                                if (!blockWorldEditor)
                                    (*it)->update();

                                (*it)->drawb(&mainTexture);
                            }

                            mainTexture.draw(**it);
                        }
                    }
                }

                if (!listMarkers.empty())
                {
                    for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
                    {
                        Vector2f entityPos((*it)->getPosition());

                        if (entityPos.x + 16 > worldcamPos.x-320 && entityPos.x < worldcamPos.x+320 && entityPos.y + 16 > worldcamPos.y-240 && entityPos.y < worldcamPos.y+240)
                            mainTexture.draw(**it);
                    }
                }

                if (!blockWorldEditor)
                {
                    if (flickTimer > 7)
                        flickTimer = 0;
                    else
                        flickTimer++;
                }

                if (currentEditing != LIGHT)
                    mainTexture.draw(*worldGrid[0]);

                switch (currentEditing)
                {
                    case TEXTURE :
                        if (worldtexturePos.x > -1)
                        {
                            Vector2f rectSize(roundf((mpos_relative.x - worldtexturePos.x + 8) / 16) * 16, roundf((mpos_relative.y - worldtexturePos.y + 8) / 16) * 16);

                            Vector2f newPosition = worldtexturePos;

                            bool canPlaced = true;

                            if (panelVisible)
                            {
                                if (mpos_absolute.x <= 156 && mpos_absolute.y >= 448)
                                    worldtexturePos.x = -1;
                            }

                            if (rectSize.x <= 0)
                            {
                                newPosition.x += 16;
                                rectSize.x -= 32;
                            }

                            if (rectSize.y <= 0)
                            {
                                newPosition.y += 16;
                                rectSize.y -= 32;
                            }

                            if (rectSize.x >= 0)
                            {
                                if (newPosition.x + rectSize.x > mapScale.x * 32)
                                    rectSize.x = (mapScale.x * 32) - newPosition.x;
                            }
                            else
                            {
                                if (newPosition.x + rectSize.x < 0)
                                    rectSize.x = -newPosition.x;
                            }

                            if (rectSize.y >= 0)
                            {
                                if (newPosition.y + rectSize.y > mapScale.y * 32)
                                    rectSize.y = (mapScale.y * 32) - newPosition.y;
                            }
                            else
                            {
                                if (newPosition.y + rectSize.y < 0)
                                    rectSize.y = -newPosition.y;
                            }

                            if (!listTextures.empty())
                            {
                                Vector2f tempPos = newPosition;
                                Vector2f tempSize = rectSize;

                                if (rectSize.x >= 0)
                                {
                                    tempPos.x += 1;
                                    tempSize.x -= 2;
                                }
                                else
                                {
                                    tempPos.x -= 1;
                                    tempSize.x += 2;
                                }

                                if (rectSize.y >= 0)
                                {
                                    tempPos.y += 1;
                                    tempSize.y -= 2;
                                }
                                else
                                {
                                    tempPos.y -= 1;
                                    tempSize.y += 2;
                                }

                                FloatRect tempRect(tempPos, tempSize);

                                for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
                                {
                                    FloatRect textureRect((*it)->rectangle.getPosition(), (*it)->rectangle.getSize());

                                    if (tempRect.intersects(textureRect))
                                    {
                                        worldTexSelection->setFillColor(Color(255, 0, 0, 128));

                                        canPlaced = false;

                                        break;
                                    }
                                }
                            }

                            if (canPlaced)
                                worldTexSelection->setFillColor(Color(0, 255, 0, 128));

                            worldTexSelection->setPosition(newPosition);
                            worldTexSelection->setSize(rectSize);
                            worldTexSelection->setTextureRect(IntRect(0, 0, abs(rectSize.x), abs(rectSize.y)));

                            mainTexture.draw(*worldTexSelection);

                            if (mousePressed == 0)
                            {
                                if (canPlaced)
                                {
                                    if (listTextures.size() == 256)
                                    {
#ifndef LINUX
                                        MessageBox(NULL, "You cannot place more than 256 Texture rectangles !", "Textures Overflow !", MB_TASKMODAL | MB_OK | MB_ICONEXCLAMATION);
#else
                                        QMessageBox messageBox(QMessageBox::Information, QStringLiteral("Textures Overflow !"), QStringLiteral("You cannot place more than 256 Texture rectangles !"), QMessageBox::Ok);
                                        messageBox.exec();
#endif
                                        worldtexturePos.x = -1;
                                    }
                                    else
                                    {
                                        TextureData* newTexture = new TextureData {RectangleShape(Vector2f(abs(rectSize.x), abs(rectSize.y))), currentTexture};
                                        const Texture* rectTexture = worldTexSelection->getTexture();

                                        Vector2f newSize = newPosition;

                                        if (rectSize.x < 0)
                                            newSize.x += rectSize.x;

                                        if (rectSize.y < 0)
                                            newSize.y += rectSize.y;

                                        newTexture->rectangle.setPosition(newSize);
                                        newTexture->rectangle.setTexture(rectTexture);
                                        newTexture->rectangle.setTextureRect(IntRect(0, 0, abs(rectSize.x), abs(rectSize.y)));

                                        worldtexturePos.x = -1;

                                        listTextures.emplace_back(newTexture);

                                        mergeTextures();

                                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), editemPut, 0, NULL);
                                    }
                                }
                                else
                                    worldtexturePos.x = -1;
                            }
                        }

                        break;
                    case EYECANDY :
                        {
                            Vector2f rectSize(worldTexSelection->getSize());
                            Vector2f newPos;

                            if (Keyboard::isKeyPressed(Keyboard::LShift))
                                newPos = Vector2f(roundf(mpos_relative.x / 16) * 16, roundf(mpos_relative.y / 16) * 16) - Vector2f(rectSize.x / 2, rectSize.y / 2);
                            else
                                newPos = mpos_relative - Vector2f(rectSize.x / 2, rectSize.y / 2);

                            if (newPos.x < 0)
                                newPos.x = 0;

                            if (newPos.y < 0)
                                newPos.y = 0;

                            if (newPos.x > (mapScale.x * 32) - rectSize.x)
                                newPos.x = (mapScale.x * 32) - rectSize.x;

                            if (newPos.y > (mapScale.y * 32) - rectSize.y)
                                newPos.y = (mapScale.y * 32) - rectSize.y;

                            worldTexSelection->setPosition(newPos);
                        }

                        mainTexture.draw(*worldTexSelection);

                        break;
                    case MARKER :
                        if (flickTimer == 0)
                            worldCursorSpr->setColor(Color(255, 64, 16));
                        else if (flickTimer == 4)
                            worldCursorSpr->setColor(Color(255, 128, 64));

                        mainTexture.draw(*worldCursorSpr);

                        if (!listMarkers.empty())
                            mainTexture.draw(&worldMarkerLink.front(), worldMarkerLink.size(), LinesStrip);

                        break;
                    default : break;
                }

                if (selectedEntity != NULL)
                {
                    if (flickTimer == 0)
                        selectionCandy->setColor(Color::White);
                    else if (flickTimer == 4)
                        selectionCandy->setColor(Color::Yellow);

                    mainTexture.draw(*selectionCandy);
                }

                if (panelVisible)
                {
                    mainTexture.draw(*worldPanel[0]);

                    switch (currentEditing)
                    {
                        case LIGHT :
                            mainTexture.draw(*worldPanel[1]);

                            mainTexture.draw(*worldButton[5]);
                            mainTexture.draw(*worldButton[6]);
                            mainTexture.draw(*worldButton[7]);

                            break;
                        case MARKER :
                            mainTexture.draw(*worldPanel[1]);

                            mainTexture.draw(*worldButton[8]);
                            mainTexture.draw(*worldButton[9]);

                            break;
                        default : break;
                    }

                    mainTexture.draw(*worldButton[0]);
                    mainTexture.draw(*worldButton[1]);
                    mainTexture.draw(*worldButton[2]);
                    mainTexture.draw(*worldButton[3]);
                    mainTexture.draw(*worldButton[4]);

                    if (helpText->getString() != "")
                        mainTexture.draw(*helpText);

                    if (!blockWorldEditor)
                    {
                        worldButton[0]->update();
                        worldButton[1]->update();
                        worldButton[2]->update();
                        worldButton[3]->update();
                        worldButton[4]->update();

                        switch (currentEditing)
                        {
                            case LIGHT :
                                worldButton[5]->update();
                                worldButton[6]->update();
                                worldButton[7]->update();

                                break;
                            case MARKER :
                                worldButton[8]->update();
                                worldButton[9]->update();

                                break;
                            default : break;
                        }
                    }
                }

                break;

            case TEXTURES :

                if (!blockWorldEditor)
                {
                    Uint8 selectionAlpha = worldTexSelection->getFillColor().a;

                    worldReturnButton->update();

                    if ((mpos_absolute.x > 80 && mpos_absolute.x < 560 && mpos_absolute.y > 64 && mpos_absolute.y < 320) ||
                        (mpos_absolute.x > 144 && mpos_absolute.x < 464 && mpos_absolute.y > 319 && mpos_absolute.y < 352) ||
                        (mpos_absolute.x > 112 && mpos_absolute.x < 496 && mpos_absolute.y > 351 && mpos_absolute.y < 448))
                    {
                        worldTexSelection->setPosition((roundf((mpos_absolute.x - 16) / 32) * 32) + (worldcamPos.x - 304), (roundf(mpos_absolute.y / 32) * 32) + (worldcamPos.y - 240));

                        if (textFadeOut)
                        {
                            if (selectionAlpha > 32)
                            {
                                selectionAlpha -= 32;
                                worldTexSelection->setFillColor(Color(255, 255, 0, selectionAlpha));
                            }
                            else
                            {
                                worldTexSelection->setFillColor(Color(255, 255, 0, 0));
                                textFadeOut = false;
                            }
                        }
                        else
                        {
                            if (selectionAlpha < 223)
                            {
                                selectionAlpha += 32;
                                worldTexSelection->setFillColor(Color(255, 255, 0, selectionAlpha));
                            }
                            else
                            {
                                worldTexSelection->setFillColor(Color(255, 255, 0, 255));
                                textFadeOut = true;
                            }
                        }
                    }
                    else
                        worldTexSelection->setPosition(-64, -64);

                    if (helpText->getString() != "")
                        helpText->setString("");
                }

                mainTexture.clear(Color(173, 116, 84));

                mainTexture.draw(*worldTexmapSpr);

                worldGrid[1]->setPosition(worldcamPos.x - 240, worldcamPos.y - 176);
                worldGrid[1]->setSize(Vector2f(481, 257));
                worldGrid[1]->setTextureRect(IntRect(0, 0, 481, 257));

                mainTexture.draw(*worldGrid[1]);

                worldGrid[1]->setPosition(worldcamPos.x - 176, worldcamPos.y + 80);
                worldGrid[1]->setSize(Vector2f(321, 32));
                worldGrid[1]->setTextureRect(IntRect(0, 0, 321, 32));

                mainTexture.draw(*worldGrid[1]);

                worldGrid[1]->setPosition(worldcamPos.x - 208, worldcamPos.y + 112);
                worldGrid[1]->setSize(Vector2f(385, 97));
                worldGrid[1]->setTextureRect(IntRect(0, 0, 385, 97));

                mainTexture.draw(*worldTextSpr[4]);

                mainTexture.draw(*worldGrid[1]);

                mainTexture.draw(*worldTexSelection);

                mainTexture.draw(*worldReturnButton);

                break;

            case EYECANDIES :

                if (!blockWorldEditor)
                {
                    Uint8 selectionAlpha = worldTexSelection->getFillColor().a;

                    if (textFadeOut)
                    {
                        if (selectionAlpha > 32)
                        {
                            selectionAlpha -= 32;
                            worldTexSelection->setFillColor(Color(255, 255, 0, selectionAlpha));
                        }
                        else
                        {
                            worldTexSelection->setFillColor(Color(255, 255, 0, 0));
                            textFadeOut = false;
                        }
                    }
                    else
                    {
                        if (selectionAlpha < 223)
                        {
                            selectionAlpha += 32;
                            worldTexSelection->setFillColor(Color(255, 255, 0, selectionAlpha));
                        }
                        else
                        {
                            worldTexSelection->setFillColor(Color(255, 255, 0, 255));
                            textFadeOut = true;
                        }
                    }

                    worldReturnButton->update();

                    if (helpText->getString() != "")
                        helpText->setString("");

                    for (register unsigned int i = 0; i < 81; i++)
                        worldEyeCases[i]->update(worldTexSelection, Eyecandy_Chosen);
                }

                mainTexture.clear(Color(173, 116, 84));

                mainTexture.draw(*worldEyemapSpr);

                for (register unsigned int i = 0; i < 81; i++)
                    mainTexture.draw(*worldEyeCases[i]);

                mainTexture.draw(*worldTexSelection);

                mainTexture.draw(*worldReturnButton);

                break;

            case TITLE :

                if (!blockWorldEditor)
                {
                    worldInfoButton[0]->update();
                    worldInfoButton[1]->update();
                    worldInfoButton[2]->update();
                    worldInfoButton[3]->update();
                    worldInfoButton[4]->update();
                    worldInfoButton[5]->update();

                    worldInfoButton[29]->update();

                    worldReturnButton->update();

                    if (helpText->getString() != "")
                        helpText->setString("");
                }

                mainTexture.clear(Color(173, 116, 84));

                mainTexture.draw(panelwInfo, 36, Quads, worldPanelTexb);

                mainTexture.draw(*worldTextSpr[0]);
                mainTexture.draw(*worldTextSpr[1]);
                mainTexture.draw(*worldTextSpr[2]);
                mainTexture.draw(*worldTextSpr[3]);

                mainTexture.draw(*worldInfoButton[0]);
                mainTexture.draw(*worldInfoButton[1]);
                mainTexture.draw(*worldInfoButton[2]);
                mainTexture.draw(*worldInfoButton[3]);
                mainTexture.draw(*worldInfoButton[4]);
                mainTexture.draw(*worldInfoButton[5]);

                mainTexture.draw(*worldInfoButton[29]);

                mainTexture.draw(*worldReturnButton);

                break;
            case MUSIC :
                if (!blockWorldEditor)
                {
                    worldInfoButton[6]->update();
                    worldInfoButton[7]->update();
                    worldInfoButton[8]->update();
                    worldInfoButton[9]->update();
                    worldInfoButton[10]->update();
                    worldInfoButton[11]->update();
                    worldInfoButton[12]->update();
                    worldInfoButton[13]->update();
                    worldInfoButton[14]->update();
                    worldInfoButton[15]->update();
                    worldInfoButton[16]->update();
                    worldInfoButton[17]->update();
                    worldInfoButton[18]->update();
                    worldInfoButton[19]->update();
                    worldInfoButton[20]->update();
                    worldInfoButton[21]->update();
                    worldInfoButton[22]->update();
                    worldInfoButton[23]->update();
                    worldInfoButton[24]->update();
                    worldInfoButton[25]->update();
                    worldInfoButton[26]->update();
                    worldInfoButton[27]->update();
                    worldInfoButton[28]->update();

                    worldReturnButton->update();
                }

                mainTexture.clear(Color(173, 116, 84));

                mainTexture.draw(*worldInfoButton[6]);
                mainTexture.draw(*worldInfoButton[7]);
                mainTexture.draw(*worldInfoButton[8]);
                mainTexture.draw(*worldInfoButton[9]);
                mainTexture.draw(*worldInfoButton[10]);
                mainTexture.draw(*worldInfoButton[11]);
                mainTexture.draw(*worldInfoButton[12]);
                mainTexture.draw(*worldInfoButton[13]);
                mainTexture.draw(*worldInfoButton[14]);
                mainTexture.draw(*worldInfoButton[15]);
                mainTexture.draw(*worldInfoButton[16]);
                mainTexture.draw(*worldInfoButton[17]);
                mainTexture.draw(*worldInfoButton[18]);
                mainTexture.draw(*worldInfoButton[19]);
                mainTexture.draw(*worldInfoButton[20]);
                mainTexture.draw(*worldInfoButton[21]);
                mainTexture.draw(*worldInfoButton[22]);
                mainTexture.draw(*worldInfoButton[23]);
                mainTexture.draw(*worldInfoButton[24]);
                mainTexture.draw(*worldInfoButton[25]);
                mainTexture.draw(*worldInfoButton[26]);
                mainTexture.draw(*worldInfoButton[27]);
                mainTexture.draw(*worldInfoButton[28]);

                mainTexture.draw(*worldReturnButton);

                mainTexture.draw(*worldTextSpr[5]);

                break;
        }

        if (gobackToTitle)
        {
            if (fadeAlpha < 255 - FADESPEED)
                fadeAlpha += FADESPEED;
            else
            {
                fadeAlpha = 255;

                exitLoop = true;
                targetScene = SCENE_TITLE;

                fadeRect->setPosition(0, 0);

                mainTexture.setView(View(Vector2f(320, 240), Vector2f(640, 480)));
            }
        }
        else
        {
            if (fadeAlpha > FADESPEED)
                fadeAlpha -= FADESPEED;
            else
                fadeAlpha = 0;
        }

        fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

        if (fadeAlpha > 0)
            mainTexture.draw(*fadeRect);

        if (exitLoop)
            mainTexture.clear(Color::Black);

        cursor->setPosition(mpos_relative);

        mainTexture.draw(*cursor);

        if (exitLoop)
            mainTexture.draw(*loading);

        mainTexture.display();

        mainWindow->clear(Color::Black);
        mainWindow->draw(textureRender);
        mainWindow->display();
    }

    for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
        delete *it;

    for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
        delete *it;

    for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
        delete *it;

    for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
        delete *it;

    listTextures.clear();
    listEyecandies.clear();
    listLights.clear();
    listMarkers.clear();

    worldMarkerLink.clear();

    if (wresourcesArray[0] != NULL)
        delete wresourcesArray[0];

    if (wresourcesArray[1] != NULL)
        delete wresourcesArray[1];

    delete renderPasses[0];

    if (renderPasses[1] != NULL)
        delete renderPasses[1];

    delete renderPasses[2];
    delete renderPasses[3];
    delete renderPasses[4];

    if (renderPasses[5] != NULL)
        delete renderPasses[5];

    if (renderPassSpr[0] != NULL)
        delete renderPassSpr[0];

    if (renderPassSpr[1] != NULL)
        delete renderPassSpr[1];

    if (glowShaderH != NULL)
        delete glowShaderH;

    if (glowShaderV != NULL)
        delete glowShaderV;

    delete worldLightTex;
    delete worldReturnButtonTex;
    delete worldGridTex[0];
    delete worldGridTex[1];
    delete worldTexmap;
    delete worldEyemap;
    delete worldCursor;
    delete worldPanelTex;
    delete worldPanelTexb;
    delete worldBackTex[0];

    delete worldText[0];
    delete worldText[1];
    delete worldText[2];

    delete worldInfo[0];
    delete worldInfo[1];
    delete worldInfo[2];
    delete worldInfo[3];

    if (worldBackTex[1] != NULL)
        delete worldBackTex[1];

    for (register unsigned int i = 0; i < 166; i++)
        delete worldTextures[i];

    for (register unsigned int i = 0; i < 81; i++)
    {
        delete worldEyecandies[i];
        delete worldEyeCases[i];
    }

    delete worldButtonTex[0];
    delete worldButtonTex[1];

    delete worldMoveable;

    delete entTex[0];
    delete entTex[1];
    delete entTex[2];

    delete worldGrid[0];
    delete worldGrid[1];
    delete worldTexSelection;
    delete worldBack;
    delete worldLightSpr[0];
    delete worldLightSpr[1];
    delete worldLightSpr[2];
    delete worldLightSpr[3];
    delete worldLightSpr[4];
    delete worldTextSpr[0];
    delete worldTextSpr[1];
    delete worldTextSpr[2];
    delete worldTextSpr[3];
    delete worldTextSpr[4];
    delete worldTextSpr[5];

    delete selectionCandy;

    delete worldPanel[0];
    delete worldPanel[1];

    delete worldReturnButton;

    delete worldCursorSpr;
    delete worldTexmapSpr;
    delete worldEyemapSpr;

    delete matrixMarker;

    delete worldButton[0];
    delete worldButton[1];
    delete worldButton[2];
    delete worldButton[3];
    delete worldButton[4];
    delete worldButton[5];
    delete worldButton[6];
    delete worldButton[7];
    delete worldButton[8];
    delete worldButton[9];

    delete worldInfoButton[0];
    delete worldInfoButton[1];
    delete worldInfoButton[2];
    delete worldInfoButton[3];
    delete worldInfoButton[4];
    delete worldInfoButton[5];
    delete worldInfoButton[6];
    delete worldInfoButton[7];
    delete worldInfoButton[8];
    delete worldInfoButton[9];
    delete worldInfoButton[10];
    delete worldInfoButton[11];
    delete worldInfoButton[12];
    delete worldInfoButton[13];
    delete worldInfoButton[14];
    delete worldInfoButton[15];
    delete worldInfoButton[16];
    delete worldInfoButton[17];
    delete worldInfoButton[18];
    delete worldInfoButton[19];
    delete worldInfoButton[20];
    delete worldInfoButton[21];
    delete worldInfoButton[22];
    delete worldInfoButton[23];
    delete worldInfoButton[24];
    delete worldInfoButton[25];
    delete worldInfoButton[26];
    delete worldInfoButton[27];
    delete worldInfoButton[28];
    delete worldInfoButton[29];

    delete worldCamera;

    delete helpFont;
    delete helpText;

    delete fadeRect;

    #ifndef NOMUSIC
    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_Sound_Release(musicSamples[0]);
        FMOD_Sound_Release(musicSamples[1]);
        FMOD_Sound_Release(musicSamples[2]);
        FMOD_Sound_Release(musicSamples[3]);
        FMOD_Sound_Release(musicSamples[4]);
        FMOD_Sound_Release(musicSamples[5]);
        FMOD_Sound_Release(musicSamples[6]);
        FMOD_Sound_Release(musicSamples[7]);
        FMOD_Sound_Release(musicSamples[8]);
        FMOD_Sound_Release(musicSamples[9]);
        FMOD_Sound_Release(musicSamples[10]);
        FMOD_Sound_Release(musicSamples[11]);
        FMOD_Sound_Release(musicSamples[12]);
        FMOD_Sound_Release(musicSamples[13]);
        FMOD_Sound_Release(musicSamples[14]);
        FMOD_Sound_Release(musicSamples[15]);
        FMOD_Sound_Release(musicSamples[16]);
        FMOD_Sound_Release(musicSamples[17]);
        FMOD_Sound_Release(musicSamples[18]);
        FMOD_Sound_Release(musicSamples[19]);
        FMOD_Sound_Release(musicSamples[20]);
        FMOD_Sound_Release(musicSamples[21]);

        if (musicSamples[22] != NULL)
            FMOD_Sound_Release(musicSamples[22]);
    }
    #endif

    FMOD_Sound_Release(edSwitchVis);
    FMOD_Sound_Release(edbuttonClick);
    FMOD_Sound_Release(editemPut);
    FMOD_Sound_Release(editemDelete);
    FMOD_Sound_Release(editemSelection);
    FMOD_Sound_Release(edsaveSound);

    if (editorMusic)
    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(musicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(musicChannel);

        FMOD_Sound_Release(worldedMusic);
    }

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    int musicNumber = (rand() % 4) + 1;

    selectedEntity = NULL;

    worldcamPos = Vector2i(320, 240);
    worldCamSpeed = Vector2f(0, 0);

    worldtexturePos = Vector2f(-1, -1);

    mapScale = Vector2i(20, 15);

    worldBackground = 0;
    worldMusic = 0;
    worldLives = 4;

    currentLightColor = Color::White;
    currentLight_TexIllum = true;
    currentLight_EyeIllum = true;
    currentMarker_IsLevel = false;
    currentMarker_Number = 0;

    wresourcesArray[0] = NULL;
    wresourcesArray[1] = NULL;

    helpFont = new Font();

    if (!helpFont->loadFromFile("Data/Fonts/Font_EditorHelp.ttf"))
        return false;

    helpText = new Text("", *helpFont, 20);
    helpText->setFillColor(Color(255, 255, 255, 255));
    helpText->setStyle(Text::Bold | Text::Italic);

    panelVisible = true;
    blockWorldMouse = false;
    blockWorldEditor = false;
    gobackToTitle = false;
    textFadeOut = false;
    placementAllowed = true;

    flickTimer = 0;

    currentEditing = NONE;
    currentScreen = EDITOR;

    worldDir.clear();

    matrixMarker = new Matrix(41, 31);

    panelwInfo[0].texCoords = Vector2f(0, 0);
    panelwInfo[1].texCoords = Vector2f(16, 0);
    panelwInfo[2].texCoords = Vector2f(16, 16);
    panelwInfo[3].texCoords = Vector2f(0, 16);
    panelwInfo[4].texCoords = Vector2f(16, 0);
    panelwInfo[5].texCoords = Vector2f(32, 0);
    panelwInfo[6].texCoords = Vector2f(32, 16);
    panelwInfo[7].texCoords = Vector2f(16, 16);
    panelwInfo[8].texCoords = Vector2f(32, 0);
    panelwInfo[9].texCoords = Vector2f(48, 0);
    panelwInfo[10].texCoords = Vector2f(48, 16);
    panelwInfo[11].texCoords = Vector2f(32, 16);
    panelwInfo[12].texCoords = Vector2f(0, 16);
    panelwInfo[13].texCoords = Vector2f(16, 16);
    panelwInfo[14].texCoords = Vector2f(16, 32);
    panelwInfo[15].texCoords = Vector2f(0, 32);
    panelwInfo[16].texCoords = Vector2f(16, 16);
    panelwInfo[17].texCoords = Vector2f(32, 16);
    panelwInfo[18].texCoords = Vector2f(32, 32);
    panelwInfo[19].texCoords = Vector2f(16, 32);
    panelwInfo[20].texCoords = Vector2f(32, 16);
    panelwInfo[21].texCoords = Vector2f(48, 16);
    panelwInfo[22].texCoords = Vector2f(48, 32);
    panelwInfo[23].texCoords = Vector2f(32, 32);
    panelwInfo[24].texCoords = Vector2f(0, 32);
    panelwInfo[25].texCoords = Vector2f(16, 32);
    panelwInfo[26].texCoords = Vector2f(16, 48);
    panelwInfo[27].texCoords = Vector2f(0, 48);
    panelwInfo[28].texCoords = Vector2f(16, 32);
    panelwInfo[29].texCoords = Vector2f(32, 32);
    panelwInfo[30].texCoords = Vector2f(32, 48);
    panelwInfo[31].texCoords = Vector2f(16, 48);
    panelwInfo[32].texCoords = Vector2f(32, 32);
    panelwInfo[33].texCoords = Vector2f(48, 32);
    panelwInfo[34].texCoords = Vector2f(48, 48);
    panelwInfo[35].texCoords = Vector2f(32, 48);

    {
        Image tempImg;

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Grid.bmp"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldGridTex[0] = new Texture;
        worldGridTex[0]->loadFromImage(tempImg);
        worldGridTex[0]->setRepeated(true);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Editor_Grid.bmp"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldGridTex[1] = new Texture;
        worldGridTex[1]->loadFromImage(tempImg);
        worldGridTex[1]->setRepeated(true);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Cursor.bmp"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldCursor = new Texture;
        worldCursor->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Editor_StringBox.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldPanelTex = new Texture;
        worldPanelTex->loadFromImage(tempImg);

        worldReturnButtonTex = new Texture;

        if (!worldReturnButtonTex->loadFromFile("Data/Gfx/TitleScreen/Button_Return.png"))
            allright = false;

        worldPanelTexb = new Texture;

        if (!worldPanelTexb->loadFromFile("Data/Gfx/Editor/Editor_Panel.png"))
            allright = false;

        worldText[0] = new Texture;

        if (!worldText[0]->loadFromFile("Data/Gfx/Editor/Texts/Worldmap_Infos.png"))
            allright = false;

        worldText[1] = new Texture;

        if (!worldText[1]->loadFromFile("Data/Gfx/Editor/Texts/Worldmap_Textures.png"))
            allright = false;

        worldText[2] = new Texture;

        if (!worldText[2]->loadFromFile("Data/Gfx/Editor/Texts/Music_Help.png"))
            allright = false;

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Buttons/Button_WorldFile.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldInfo[0] = new Texture;
        worldInfo[0]->loadFromImage(tempImg);

        worldInfo[1] = new Texture;

        if (!worldInfo[1]->loadFromFile("Data/Gfx/Editor/Buttons/Button_WorldBackgrounds.png"))
            allright = false;

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Buttons/Button_Edit.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldInfo[2] = new Texture;
        worldInfo[2]->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Buttons/Button_Musics.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldInfo[3] = new Texture;
        worldInfo[3]->loadFromImage(tempImg);

        worldBackTex[0] = new Texture;

        worldLightTex = new Texture;

        if (!worldLightTex->loadFromFile("Data/Gfx/Worldmap/Worldmap_LightHalo.png"))
            allright = false;

        if (!worldBackTex[0]->loadFromFile("Data/Gfx/Worldmap/Worldmap_Background.jpg"))
            allright = false;

        worldBackTex[0]->setRepeated(true);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Textures.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldTexmap = new Texture;
        worldTexmap->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Eyecandies.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldEyemap = new Texture;
        worldEyemap->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Types.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldButtonTex[0] = new Texture;
        worldButtonTex[0]->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Parameters.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldButtonTex[1] = new Texture;
        worldButtonTex[1]->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Editor_Moveable.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        worldMoveable = new Texture;
        worldMoveable->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Light.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        entTex[0] = new Texture;
        entTex[0]->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Marker.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        entTex[1] = new Texture;
        entTex[1]->loadFromImage(tempImg);

        if (!tempImg.loadFromFile("Data/Gfx/Editor/Worldmap/Worldmap_Cases.png"))
            allright = false;

        tempImg.createMaskFromColor(Color::Magenta);

        entTex[2] = new Texture;
        entTex[2]->loadFromImage(tempImg);

        for (register unsigned int i = 0; i < 166; i++)
        {
            char* fileurl = new char[43];

            sprintf(fileurl, "Data/Gfx/Worldmap/Textures/Texture_%d.png", i+1);

            worldTextures[i] = new Texture;

            if (!worldTextures[i]->loadFromFile(fileurl))
                allright = false;

            worldTextures[i]->setRepeated(true);

            delete[] fileurl;
        }

        for (register unsigned int i = 0; i < 81; i++)
        {
            char* fileurl = new char[46];

            sprintf(fileurl, "Data/Gfx/Worldmap/Eyecandies/Eyecandy_%d.png", i+1);

            worldEyecandies[i] = new Texture;

            if (!worldEyecandies[i]->loadFromFile(fileurl))
                allright = false;

            delete[] fileurl;
        }
    }

    worldBackTex[1] = NULL;

    worldTexmapSpr = new Sprite(*worldTexmap);
    worldEyemapSpr = new Sprite(*worldEyemap);

    worldGrid[0] = new RectangleShape(Vector2f(672, 512));
    worldGrid[0]->setTexture(worldGridTex[0]);
    worldGrid[0]->setTextureRect(IntRect(0, 0, 672, 512));
    worldGrid[0]->setFillColor(Color(0, 0, 0, 128));

    worldGrid[1] = new RectangleShape(Vector2f(480, 256));
    worldGrid[1]->setTexture(worldGridTex[1]);
    worldGrid[1]->setTextureRect(IntRect(0, 0, 480, 256));

    worldTexSelection = new RectangleShape(Vector2f(32, 32));
    worldTexSelection->setFillColor(Color(255, 255, 0, 128));

    worldBack = new RectangleShape(Vector2f(640, 480));
    worldBack->setTexture(worldBackTex[0]);
    worldBack->setTextureRect(IntRect(0, 0, 640, 480));

    selectionCandy = new Sprite(*worldMoveable);
    selectionCandy->setColor(Color::White);

    worldLightSpr[0] = new Sprite(*worldLightTex);
    worldLightSpr[0]->setOrigin(64, 64);

    worldTextSpr[0] = new Sprite(*worldText[0], IntRect(0, 0, 174, 27));
    worldTextSpr[1] = new Sprite(*worldText[0], IntRect(0, 27, 174, 27));
    worldTextSpr[2] = new Sprite(*worldText[0], IntRect(0, 54, 174, 26));
    worldTextSpr[3] = new Sprite(*worldText[0], IntRect(0, 80, 174, 27));
    worldTextSpr[4] = new Sprite(*worldText[1]);
    worldTextSpr[5] = new Sprite(*worldText[2]);

    worldTextSpr[4]->setOrigin(122, 0);
    worldTextSpr[5]->setOrigin(238, 0);

    worldCursorSpr = new Sprite(*worldCursor);
    worldCursorSpr->setColor(Color(255, 64, 16));
    worldCursorSpr->setOrigin(4, 4);

    worldPanel[0] = new Sprite(*worldPanelTex);
    worldPanel[0]->setScale(-1, 1);

    worldPanel[1] = new Sprite(*worldPanelTex);

    worldReturnButton = new Button(*worldReturnButtonTex, Button_Return, "");

    worldButton[0] = new Button(*worldButtonTex[0], Button_Textures, "Place Textures", IntRect(0, 0, 26, 26));
    worldButton[1] = new Button(*worldButtonTex[0], Button_Eyecandies, "Place Eyecandies", IntRect(26, 0, 26, 26));
    worldButton[2] = new Button(*worldButtonTex[0], Button_Lights, "Place Lamps", IntRect(52, 0, 26, 26));
    worldButton[3] = new Button(*worldButtonTex[0], Button_Markers, "Place Markers", IntRect(78, 0, 26, 26));
    worldButton[4] = new Button(*worldButtonTex[0], Button_Title, "Preferences", IntRect(104, 0, 26, 26));

    worldButton[5] = new Button(*worldButtonTex[1], Light_Color, "Set Lamp's color", IntRect(0, 0, 26, 26));
    worldButton[6] = new Button(*worldButtonTex[1], Light_TexIllumin, "Enable/Disable Texture Illumination", IntRect(0, 26, 26, 26));
    worldButton[7] = new Button(*worldButtonTex[1], Light_EyeIllumin, "Enable/Disable Eyecandy Illumination", IntRect(0, 52, 26, 26));

    worldButton[8] = new Button(*worldButtonTex[1], Mark_SetTurn, "Set as simple Turn Point", IntRect(26, 0, 26, 26));
    worldButton[9] = new Button(*worldButtonTex[1], Mark_SetLevel, "Set as a Level's Marker", IntRect(26, 26, 26, 26));

    worldInfoButton[0] = new Button(*worldInfo[0], Info_Save, "", IntRect(0, 0, 128, 44));
    worldInfoButton[1] = new Button(*worldInfo[0], Info_SaveAs, "", IntRect(0, 44, 128, 44));
    worldInfoButton[2] = new Button(*worldInfo[0], Info_Load, "", IntRect(0, 88, 128, 44));
    worldInfoButton[3] = new Button(*worldInfo[1], Background_Default, "", IntRect(0, 0, 185, 68));
    worldInfoButton[4] = new Button(*worldInfo[1], Background_Import, "", IntRect(185, 0, 185, 68));
    worldInfoButton[5] = new Button(*worldInfo[2], Music_Edit, "");
    worldInfoButton[6] = new Button(*worldInfo[3], Music_World1, "", IntRect(0, 0, 200, 32));
    worldInfoButton[7] = new Button(*worldInfo[3], Music_Cave, "", IntRect(0, 32, 200, 32));
    worldInfoButton[8] = new Button(*worldInfo[3], Music_Underground, "", IntRect(0, 64, 200, 32));
    worldInfoButton[9] = new Button(*worldInfo[3], Music_Rockland, "", IntRect(0, 96, 200, 32));
    worldInfoButton[10] = new Button(*worldInfo[3], Music_Starland, "", IntRect(0, 128, 200, 32));
    worldInfoButton[11] = new Button(*worldInfo[3], Music_StarPlatforms, "", IntRect(0, 160, 200, 32));
    worldInfoButton[12] = new Button(*worldInfo[3], Music_Underwater, "", IntRect(0, 192, 200, 32));
    worldInfoButton[13] = new Button(*worldInfo[3], Music_SelectSave, "", IntRect(0, 224, 200, 32));
    worldInfoButton[14] = new Button(*worldInfo[3], Music_GhostHouse, "", IntRect(0, 256, 200, 32));
    worldInfoButton[15] = new Button(*worldInfo[3], Music_Castle1, "", IntRect(0, 288, 200, 32));
    worldInfoButton[16] = new Button(*worldInfo[3], Music_Castle2, "", IntRect(0, 320, 200, 32));
    worldInfoButton[17] = new Button(*worldInfo[3], Music_Yoshi1, "", IntRect(0, 352, 200, 32));
    worldInfoButton[18] = new Button(*worldInfo[3], Music_Yoshi2, "", IntRect(0, 384, 200, 32));
    worldInfoButton[19] = new Button(*worldInfo[3], Music_Map1, "", IntRect(0, 416, 200, 32));
    worldInfoButton[20] = new Button(*worldInfo[3], Music_Map2, "", IntRect(0, 448, 200, 32));
    worldInfoButton[21] = new Button(*worldInfo[3], Music_Map3, "", IntRect(0, 480, 200, 32));
    worldInfoButton[22] = new Button(*worldInfo[3], Music_Fastrun1, "", IntRect(0, 512, 200, 32));
    worldInfoButton[23] = new Button(*worldInfo[3], Music_Fastrun2, "", IntRect(0, 544, 200, 32));
    worldInfoButton[24] = new Button(*worldInfo[3], Music_InDaKitchen, "", IntRect(0, 576, 200, 32));
    worldInfoButton[25] = new Button(*worldInfo[3], Music_Yky, "", IntRect(0, 608, 200, 32));
    worldInfoButton[26] = new Button(*worldInfo[3], Music_YetBonus, "", IntRect(0, 640, 200, 32));
    worldInfoButton[27] = new Button(*worldInfo[3], Music_Apoplexy, "", IntRect(0, 672, 200, 32));
    worldInfoButton[28] = new Button(*worldInfo[3], Music_Import, "", IntRect(0, 704, 200, 32));
    worldInfoButton[29] = new Button(*worldInfo[2], Size_Edit, "");

    worldEyeCases[0] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 0);
    worldEyeCases[1] = new Case(*entTex[2], IntRect(96, 0, 32, 96), 1);
    worldEyeCases[2] = new Case(*entTex[2], IntRect(32, 0, 64, 32), 2);
    worldEyeCases[3] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 3);
    worldEyeCases[4] = new Case(*entTex[2], IntRect(96, 0, 32, 96), 4);
    worldEyeCases[5] = new Case(*entTex[2], IntRect(32, 0, 64, 32), 5);
    worldEyeCases[6] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 6);
    worldEyeCases[7] = new Case(*entTex[2], IntRect(96, 0, 32, 96), 7);
    worldEyeCases[8] = new Case(*entTex[2], IntRect(32, 0, 64, 32), 8);
    worldEyeCases[9] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 9);
    worldEyeCases[10] = new Case(*entTex[2], IntRect(96, 0, 32, 96), 10);
    worldEyeCases[11] = new Case(*entTex[2], IntRect(32, 0, 64, 32), 11);
    worldEyeCases[12] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 12);
    worldEyeCases[13] = new Case(*entTex[2], IntRect(96, 0, 32, 96), 13);
    worldEyeCases[14] = new Case(*entTex[2], IntRect(32, 0, 64, 32), 14);
    worldEyeCases[15] = new Case(*entTex[2], IntRect(0, 96, 160, 32), 15);
    worldEyeCases[16] = new Case(*entTex[2], IntRect(0, 96, 160, 32), 16);
    worldEyeCases[17] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 17);
    worldEyeCases[18] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 18);
    worldEyeCases[19] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 19);
    worldEyeCases[20] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 20);
    worldEyeCases[21] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 21);
    worldEyeCases[22] = new Case(*entTex[2], IntRect(32, 0, 64, 32), 22);
    worldEyeCases[23] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 23);
    worldEyeCases[24] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 24);
    worldEyeCases[25] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 25);
    worldEyeCases[26] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 26);
    worldEyeCases[27] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 27);
    worldEyeCases[28] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 28);
    worldEyeCases[29] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 29);
    worldEyeCases[30] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 30);
    worldEyeCases[31] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 31);
    worldEyeCases[32] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 32);
    worldEyeCases[33] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 33);
    worldEyeCases[34] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 34);
    worldEyeCases[35] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 35);
    worldEyeCases[36] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 36);
    worldEyeCases[37] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 37);
    worldEyeCases[38] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 38);
    worldEyeCases[39] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 39);
    worldEyeCases[40] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 40);
    worldEyeCases[41] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 41);
    worldEyeCases[42] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 42);
    worldEyeCases[43] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 43);
    worldEyeCases[44] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 44);
    worldEyeCases[45] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 45);
    worldEyeCases[46] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 46);
    worldEyeCases[47] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 47);
    worldEyeCases[48] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 48);
    worldEyeCases[49] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 49);
    worldEyeCases[50] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 50);
    worldEyeCases[51] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 51);
    worldEyeCases[52] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 52);
    worldEyeCases[53] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 53);
    worldEyeCases[54] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 54);
    worldEyeCases[55] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 55);
    worldEyeCases[56] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 56);
    worldEyeCases[57] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 57);
    worldEyeCases[58] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 58);
    worldEyeCases[59] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 59);
    worldEyeCases[60] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 60);
    worldEyeCases[61] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 61);
    worldEyeCases[62] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 62);
    worldEyeCases[63] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 63);
    worldEyeCases[64] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 64);
    worldEyeCases[65] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 65);
    worldEyeCases[66] = new Case(*entTex[2], IntRect(0, 32, 32, 64), 66);
    worldEyeCases[67] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 67);
    worldEyeCases[68] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 68);
    worldEyeCases[69] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 69);
    worldEyeCases[70] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 70);
    worldEyeCases[71] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 71);
    worldEyeCases[72] = new Case(*entTex[2], IntRect(32, 32, 64, 64), 72);
    worldEyeCases[73] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 73);
    worldEyeCases[74] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 74);
    worldEyeCases[75] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 75);
    worldEyeCases[76] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 76);
    worldEyeCases[77] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 77);
    worldEyeCases[78] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 78);
    worldEyeCases[79] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 79);
    worldEyeCases[80] = new Case(*entTex[2], IntRect(0, 0, 32, 32), 80);

    worldReturnButton->setSound(&edbuttonClick);

    worldButton[0]->setSound(&edbuttonClick);
    worldButton[1]->setSound(&edbuttonClick);
    worldButton[2]->setSound(&edbuttonClick);
    worldButton[3]->setSound(&edbuttonClick);
    worldButton[4]->setSound(&edbuttonClick);
    worldButton[5]->setSound(&edbuttonClick);
    worldButton[6]->setSound(&edbuttonClick);
    worldButton[7]->setSound(&edbuttonClick);
    worldButton[8]->setSound(&edbuttonClick);
    worldButton[9]->setSound(&edbuttonClick);

    worldInfoButton[0]->setSound(&edbuttonClick);
    worldInfoButton[1]->setSound(&edbuttonClick);
    worldInfoButton[2]->setSound(&edbuttonClick);
    worldInfoButton[3]->setSound(&edbuttonClick);
    worldInfoButton[4]->setSound(&edbuttonClick);
    worldInfoButton[5]->setSound(&edbuttonClick);
    worldInfoButton[6]->setSound(NULL);
    worldInfoButton[7]->setSound(NULL);
    worldInfoButton[8]->setSound(NULL);
    worldInfoButton[9]->setSound(NULL);
    worldInfoButton[10]->setSound(NULL);
    worldInfoButton[11]->setSound(NULL);
    worldInfoButton[12]->setSound(NULL);
    worldInfoButton[13]->setSound(NULL);
    worldInfoButton[14]->setSound(NULL);
    worldInfoButton[15]->setSound(NULL);
    worldInfoButton[16]->setSound(NULL);
    worldInfoButton[17]->setSound(NULL);
    worldInfoButton[18]->setSound(NULL);
    worldInfoButton[19]->setSound(NULL);
    worldInfoButton[20]->setSound(NULL);
    worldInfoButton[21]->setSound(NULL);
    worldInfoButton[22]->setSound(NULL);
    worldInfoButton[23]->setSound(NULL);
    worldInfoButton[24]->setSound(NULL);
    worldInfoButton[25]->setSound(NULL);
    worldInfoButton[26]->setSound(NULL);
    worldInfoButton[27]->setSound(NULL);
    worldInfoButton[28]->setSound(NULL);
    worldInfoButton[29]->setSound(&edbuttonClick);

    worldCamera = new View(Vector2f(320, 240), Vector2f(640, 480));

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

	{
		FMOD_RESULT result;

		result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Menu2.wav", FMOD_DEFAULT, NULL, &edbuttonClick);

		if (result != FMOD_OK)
			allright = false;

		result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Editor1.wav", FMOD_DEFAULT, NULL, &editemPut);

		if (result != FMOD_OK)
			allright = false;

		result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Editor2.wav", FMOD_DEFAULT, NULL, &editemDelete);

		if (result != FMOD_OK)
			allright = false;

        result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Menu1.wav", FMOD_DEFAULT, NULL, &editemSelection);

		if (result != FMOD_OK)
			allright = false;

		result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Editor3.wav", FMOD_DEFAULT, NULL, &edSwitchVis);

		if (result != FMOD_OK)
			allright = false;

		result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/1Up.wav", FMOD_DEFAULT, NULL, &edsaveSound);

		if (result != FMOD_OK)
			allright = false;

        #ifndef NOMUSIC
        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/World1.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[0]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Cave.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[1]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Underground.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[2]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Rockland.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[3]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Starland.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[4]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Yoshi3.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[5]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Underwater.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[6]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/SelectSave.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[7]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/GhostHouse.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[8]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Castle.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[9]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Castle2.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[10]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Yoshi1.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[11]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Yoshi2.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[12]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Map1.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[13]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Map2.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[14]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Map3.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[15]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Fastrun1.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[16]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Fastrun2.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[17]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/InDaKitchen.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[18]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Yky.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[19]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/YetAnotherBonus.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[20]);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Apoplexy.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[21]);

        if (result != FMOD_OK)
            allright = false;

        musicSamples[22] = NULL;
        #endif
	}

    if (editorMusic)
    {
        FMOD_RESULT result;
        string filename;

        switch (musicNumber)
        {
            case 1 : filename = "Data/Sfx/Musics/EditorScreen1.mp3"; break;
            case 2 : filename = "Data/Sfx/Musics/EditorScreen2.mp3"; break;
            case 3 : filename = "Data/Sfx/Musics/EditorScreen3.mp3"; break;
            case 4 : filename = "Data/Sfx/Musics/EditorScreen4.mp3"; break;
        }

        result = FMOD_System_CreateStream(soundSystem,
                                filename.c_str(),
                                FMOD_LOOP_NORMAL |
                                FMOD_SOFTWARE |
                                FMOD_2D,
                                NULL,
                                &worldedMusic);

        if (result != FMOD_OK)
            allright = false;

        result = FMOD_System_PlaySound(soundSystem,
                              static_cast<FMOD_CHANNELINDEX>(20),
                              worldedMusic,
                              0,
                              &musicChannel);

        if (result != FMOD_OK)
            allright = false;
    }

    useShaders = Shader::isAvailable();

    renderPasses[0] = new RenderTexture;
    renderPasses[0]->create(640, 480);

    renderPasses[2] = new RenderTexture;
    renderPasses[2]->create(640, 480);

    renderPasses[3] = new RenderTexture;
    renderPasses[3]->create(640, 480);

    renderPasses[4] = new RenderTexture;
    renderPasses[4]->create(640, 480);

    worldLightSpr[1] = new Sprite(renderPasses[2]->getTexture());
    worldLightSpr[2] = new Sprite(renderPasses[0]->getTexture());
    worldLightSpr[3] = new Sprite(renderPasses[3]->getTexture());
    worldLightSpr[4] = new Sprite(renderPasses[4]->getTexture());

    if (enableShaders)
    {
        if (!useShaders)
        {
#ifndef LINUX
            StrCpy(trayIcon.szInfoTitle, "Shaders unavailable");
            StrCpy(trayIcon.szInfo, "Your current Graphics Driver doesn't support Shader Model 2.0 !");

            Shell_NotifyIcon(NIM_MODIFY, &trayIcon);
#endif

            renderPasses[1] = NULL;
            renderPasses[5] = NULL;

            renderPassSpr[0] = NULL;
            renderPassSpr[1] = NULL;

            glowShaderH = NULL;
            glowShaderV = NULL;
        }
        else
        {
            renderPasses[1] = new RenderTexture;
            renderPasses[1]->create(640, 480);

            renderPasses[5] = new RenderTexture;
            renderPasses[5]->create(640, 480);

            renderPassSpr[0] = new Sprite(renderPasses[5]->getTexture());
            renderPassSpr[1] = new Sprite(renderPasses[1]->getTexture());

            glowShaderH = new Shader;
            glowShaderV = new Shader;

            if (!glowShaderH->loadFromFile("Data/Shaders/Shader_DefaultVertex.txt", "Data/Shaders/Shader_WorldBlurH.txt"))
                allright = false;

            if (!glowShaderV->loadFromFile("Data/Shaders/Shader_DefaultVertex.txt", "Data/Shaders/Shader_WorldBlurV.txt"))
                allright = false;
        }
    }
    else
    {
        useShaders = false;

        renderPasses[1] = NULL;
        renderPasses[5] = NULL;

        glowShaderH = NULL;
        glowShaderV = NULL;
    }

    return allright;
}

static void UpdateAssets()
{
    int camX = 0;
    int camY = 0;

    int camMouseX = 0;
    int camMouseY = 0;

    if (mainWindow->hasFocus())
    {
        bool focus = mainWindow->hasFocus();

        bool keyLeft = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[0])) && focus;
        bool keyRight = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[1])) && focus;
        bool keyDown = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[2])) && focus;
        bool keyUp = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[3])) && focus;

        if (keyLeft && !keyRight)
            camX = -1;
        else if (keyRight && !keyLeft)
            camX = 1;

        if (keyUp && !keyDown)
            camY = -1;
        else if (keyDown && !keyUp)
            camY = 1;
    }

    if (!editorMouseScrolling)
        goto LBL_WSKIPMOUSECONTROL;

    if (panelVisible)
    {
        if (mainWindow->hasFocus())
        {
            switch (currentEditing)
            {
                case LIGHT :
                    if (mpos_absolute.x < MOUSEINFLUENCE && mpos_absolute.y < 448)
                        camMouseX = -1;
                    else if (mpos_absolute.x > 640-MOUSEINFLUENCE && mpos_absolute.y < 448)
                        camMouseX = 1;

                    if (mpos_absolute.y < MOUSEINFLUENCE)
                        camMouseY = -1;
                    else if (mpos_absolute.y > 480-MOUSEINFLUENCE && mpos_absolute.x > 156 && mpos_absolute.x < 534)
                        camMouseY = 1;
                    break;
                case MARKER :
                    if (mpos_absolute.x < MOUSEINFLUENCE && mpos_absolute.y < 448)
                        camMouseX = -1;
                    else if (mpos_absolute.x > 640-MOUSEINFLUENCE && mpos_absolute.y < 448)
                        camMouseX = 1;

                    if (mpos_absolute.y < MOUSEINFLUENCE)
                        camMouseY = -1;
                    else if (mpos_absolute.y > 480-MOUSEINFLUENCE && mpos_absolute.x > 156 && mpos_absolute.x < 566)
                        camMouseY = 1;
                    break;
                default :
                    if (mpos_absolute.x < MOUSEINFLUENCE && mpos_absolute.y < 448)
                        camMouseX = -1;
                    else if (mpos_absolute.x > 640-MOUSEINFLUENCE)
                        camMouseX = 1;

                    if (mpos_absolute.y < MOUSEINFLUENCE)
                        camMouseY = -1;
                    else if (mpos_absolute.y > 480-MOUSEINFLUENCE && mpos_absolute.x > 156)
                        camMouseY = 1;
                    break;
            }
        }
    }
    else
    {
        if (mainWindow->hasFocus())
        {
            if (mpos_absolute.x < MOUSEINFLUENCE)
                camMouseX = -1;
            else if (mpos_absolute.x > 640-MOUSEINFLUENCE)
                camMouseX = 1;

            if (mpos_absolute.y < MOUSEINFLUENCE)
                camMouseY = -1;
            else if (mpos_absolute.y > 480-MOUSEINFLUENCE)
                camMouseY = 1;
        }
    }

    if (camX == 0)
    {
        if (camMouseX == -1)
            camX = -1;
        else if (camMouseX == 1)
            camX = 1;
    }

    if (camY == 0)
    {
        if (camMouseY == -1)
            camY = -1;
        else if (camMouseY == 1)
            camY = 1;
    }

    LBL_WSKIPMOUSECONTROL :

    switch (camX)
    {
        case -1:
            if (worldCamSpeed.x > - (CAMSPEED - CAMACCEL))
                worldCamSpeed.x -= CAMACCEL;
            else
                worldCamSpeed.x = -CAMSPEED;
            break;
        case 0:
            if (worldCamSpeed.x < -CAMDECEL)
                worldCamSpeed.x += CAMDECEL;
            else if (worldCamSpeed.x > CAMDECEL)
                worldCamSpeed.x -= CAMDECEL;
            else if (worldCamSpeed.x != 0)
                worldCamSpeed.x = 0;
            break;
        case 1:
            if (worldCamSpeed.x < CAMSPEED - CAMACCEL)
                worldCamSpeed.x += CAMACCEL;
            else
                worldCamSpeed.x = CAMSPEED;
            break;
    }

    switch (camY)
    {
        case -1:
            if (worldCamSpeed.y > - (CAMSPEED - CAMACCEL))
                worldCamSpeed.y -= CAMACCEL;
            else
                worldCamSpeed.y = -CAMSPEED;
            break;
        case 0:
            if (worldCamSpeed.y < -CAMDECEL)
                worldCamSpeed.y += CAMDECEL;
            else if (worldCamSpeed.y > CAMDECEL)
                worldCamSpeed.y -= CAMDECEL;
            else if (worldCamSpeed.y != 0)
                worldCamSpeed.y = 0;
            break;
        case 1:
            if (worldCamSpeed.y < CAMSPEED - CAMACCEL)
                worldCamSpeed.y += CAMACCEL;
            else
                worldCamSpeed.y = CAMSPEED;
            break;
    }

    worldcamPos.x += roundf(worldCamSpeed.x);
    worldcamPos.y += roundf(worldCamSpeed.y);

    worldCamera->setCenter(worldcamPos.x, worldcamPos.y);

    if (worldcamPos.x < 320)
    {
        worldcamPos.x = 320;

        if (worldCamSpeed.x < 0)
            worldCamSpeed.x = 0;

        worldCamera->setCenter(320, worldcamPos.y);
    }
    else if (worldcamPos.x > (mapScale.x*32) - 320)
    {
        worldcamPos.x = (mapScale.x*32) - 320;

        if (worldCamSpeed.x > 0)
            worldCamSpeed.x = 0;

        worldCamera->setCenter(worldcamPos.x, worldcamPos.y);
    }

    if (worldcamPos.y < 240)
    {
        worldcamPos.y = 240;

        if (worldCamSpeed.y < 0)
            worldCamSpeed.y = 0;

        worldCamera->setCenter(worldcamPos.x, 240);
    }
    else if (worldcamPos.y > (mapScale.y*32) - 240)
    {
        worldcamPos.y = (mapScale.y*32) - 240;

        if (worldCamSpeed.y > 0)
            worldCamSpeed.y = 0;

        worldCamera->setCenter(worldcamPos.x, worldcamPos.y);
    }

    if (currentEditing != LIGHT)
        worldGrid[0]->setPosition((roundf(worldcamPos.x / 16) * 16) - 336, (roundf(worldcamPos.y / 16) * 16) - 256);

    if (currentEditing == MARKER)
    {
        Vector2f newPosition(roundf((mpos_relative.x + 1) / 16) * 16, roundf((mpos_relative.y + 1) / 16) * 16);

        if (newPosition.x < 0)
            newPosition.x = 0;
        else if (newPosition.x > mapScale.x * 32)
            newPosition.x = mapScale.x * 32;

        if (newPosition.y < 0)
            newPosition.y = 0;
        else if (newPosition.y > mapScale.y * 32)
            newPosition.y = mapScale.y * 32;

        worldCursorSpr->setPosition(newPosition);
    }

    if (fadeAlpha != 0)
        fadeRect->setPosition(worldcamPos.x-320, worldcamPos.y-240);

    mainTexture.setView(*worldCamera);
    mpos_relative = mainTexture.mapPixelToCoords(mpos_absolute);

    if (helpText->getString() != "")
    {
        FloatRect textBounds(helpText->getGlobalBounds());
        Uint8 textColor = helpText->getFillColor().r;

        helpText->setOrigin(textBounds.width / 2, 0);
        helpText->setPosition(mpos_relative.x, mpos_relative.y-22);

        textBounds = helpText->getGlobalBounds();

        if (textFadeOut)
        {
            if (textColor > 8)
            {
                textColor -= 8;
                helpText->setFillColor(Color(textColor, textColor, 0, 255));
            }
            else
            {
                helpText->setFillColor(Color(0, 0, 0, 255));
                textFadeOut = false;
            }
        }
        else
        {
            if (textColor < 247)
            {
                textColor += 8;
                helpText->setFillColor(Color(textColor, textColor, 0, 255));
            }
            else
            {
                helpText->setFillColor(Color(255, 255, 0, 255));
                textFadeOut = true;
            }
        }

        if (textBounds.left < worldcamPos.x-320)
            helpText->setPosition((worldcamPos.x-320) + (textBounds.width/2), mpos_relative.y-22);
        else if (textBounds.left + textBounds.width > worldcamPos.x+320)
            helpText->setPosition((worldcamPos.x+320) - (textBounds.width/2), mpos_relative.y-22);
    }

    if (panelVisible)
    {
        worldPanel[0]->setPosition(worldcamPos.x-164, worldcamPos.y+208);

        switch (currentEditing)
        {
            case LIGHT :
                worldPanel[1]->setPosition(worldcamPos.x+220, worldcamPos.y+208);

                worldButton[5]->setPosition(worldcamPos.x+260, worldcamPos.y+252);
                worldButton[6]->setPosition(worldcamPos.x+288, worldcamPos.y+252);
                worldButton[7]->setPosition(worldcamPos.x+316, worldcamPos.y+252);
                break;
            case MARKER :
                worldPanel[1]->setPosition(worldcamPos.x+248, worldcamPos.y+208);

                worldButton[8]->setPosition(worldcamPos.x+288, worldcamPos.y+252);
                worldButton[9]->setPosition(worldcamPos.x+316, worldcamPos.y+252);
                break;
            default : break;
        }

        worldButton[0]->setPosition(worldcamPos.x-252, worldcamPos.y+226);
        worldButton[1]->setPosition(worldcamPos.x-224, worldcamPos.y+226);
        worldButton[2]->setPosition(worldcamPos.x-196, worldcamPos.y+226);
        worldButton[3]->setPosition(worldcamPos.x-168, worldcamPos.y+226);
        worldButton[4]->setPosition(worldcamPos.x-140, worldcamPos.y+226);
    }
}

static unsigned int GetTextureNum(unsigned int input)
{
    switch (input)
    {
        default : return 1;
        case 1 : return 2;
        case 2 : return 3;
        case 3 : return 7;
        case 4 : return 8;
        case 5 : return 9;
        case 6 : return 13;
        case 7 : return 14;
        case 8 : return 15;
        case 9 : return 19;
        case 10 : return 20;
        case 11 : return 21;
        case 12 : return 25;
        case 13 : return 26;
        case 14 : return 27;
        case 15 : return 4;
        case 16 : return 5;
        case 17 : return 6;
        case 18 : return 10;
        case 19 : return 11;
        case 20 : return 12;
        case 21 : return 16;
        case 22 : return 17;
        case 23 : return 18;
        case 24 : return 22;
        case 25 : return 23;
        case 26 : return 24;
        case 27 : return 28;
        case 28 : return 29;
        case 29 : return 30;
        case 30 : return 31;
        case 31 : return 32;
        case 32 : return 33;
        case 33 : return 37;
        case 34 : return 38;
        case 35 : return 39;
        case 36 : return 43;
        case 37 : return 44;
        case 38 : return 45;
        case 39 : return 49;
        case 40 : return 50;
        case 41 : return 51;
        case 42 : return 55;
        case 43 : return 56;
        case 44 : return 57;
        case 45 : return 34;
        case 46 : return 35;
        case 47 : return 36;
        case 48 : return 40;
        case 49 : return 41;
        case 50 : return 42;
        case 51 : return 46;
        case 52 : return 47;
        case 53 : return 48;
        case 54 : return 52;
        case 55 : return 53;
        case 56 : return 54;
        case 57 : return 58;
        case 58 : return 59;
        case 59 : return 60;
        case 60 : return 61;
        case 61 : return 62;
        case 62 : return 63;
        case 63 : return 67;
        case 64 : return 68;
        case 65 : return 69;
        case 66 : return 73;
        case 67 : return 74;
        case 68 : return 75;
        case 69 : return 82;
        case 70 : return 83;
        case 71 : return 84;
        case 72 : return 152;
        case 73 : return 153;
        case 74 : return 154;
        case 75 : return 64;
        case 76 : return 65;
        case 77 : return 66;
        case 78 : return 70;
        case 79 : return 71;
        case 80 : return 72;
        case 81 : return 76;
        case 82 : return 77;
        case 83 : return 78;
        case 84 : return 85;
        case 85 : return 86;
        case 86 : return 87;
        case 87 : return 155;
        case 88 : return 156;
        case 89 : return 157;
        case 90 : return 93;
        case 91 : return 91;
        case 92 : return 92;
        case 93 : return 96;
        case 94 : return 94;
        case 95 : return 95;
        case 96 : return 79;
        case 97 : return 80;
        case 98 : return 81;
        case 99 : return 88;
        case 100 : return 89;
        case 101 : return 90;
        case 102 : return 158;
        case 103 : return 159;
        case 104 : return 166;
        case 105 : return 133;
        case 106 : return 134;
        case 107 : return 135;
        case 108 : return 136;
        case 109 : return 137;
        case 110 : return 138;
        case 111 : return 139;
        case 112 : return 140;
        case 113 : return 141;
        case 114 : return 162;
        case 115 : return 163;
        case 116 : return 164;
        case 117 : return 165;
        case 118 : return 160;
        case 119 : return 161;
        case 122 : return 142;
        case 123 : return 143;
        case 124 : return 144;
        case 125 : return 145;
        case 126 : return 146;
        case 127 : return 147;
        case 128 : return 148;
        case 129 : return 149;
        case 130 : return 150;
        case 131 : return 151;
        case 136 : return 97;
        case 137 : return 98;
        case 138 : return 103;
        case 139 : return 104;
        case 140 : return 109;
        case 141 : return 110;
        case 142 : return 115;
        case 143 : return 116;
        case 144 : return 121;
        case 145 : return 122;
        case 146 : return 127;
        case 147 : return 128;
        case 151 : return 99;
        case 152 : return 100;
        case 153 : return 105;
        case 154 : return 106;
        case 155 : return 111;
        case 156 : return 112;
        case 157 : return 117;
        case 158 : return 118;
        case 159 : return 123;
        case 160 : return 124;
        case 161 : return 129;
        case 162 : return 130;
        case 166 : return 101;
        case 167 : return 102;
        case 168 : return 107;
        case 169 : return 108;
        case 170 : return 113;
        case 171 : return 114;
        case 172 : return 119;
        case 173 : return 120;
        case 174 : return 125;
        case 175 : return 126;
        case 176 : return 131;
        case 177 : return 132;
    }
}

static void Button_Textures()
{
    worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

    worldTexmapSpr->setPosition(worldcamPos.x - 240, worldcamPos.y - 176);

    worldTextSpr[4]->setPosition(worldcamPos.x, worldcamPos.y - 224);

    worldTexSelection->setFillColor(Color(255, 255, 0, 128));
    worldTexSelection->setSize(Vector2f(32, 32));
    worldTexSelection->setTexture(NULL);

    currentScreen = TEXTURES;
}

static void Button_Eyecandies()
{
    worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

    worldEyemapSpr->setPosition(worldcamPos.x - 256, worldcamPos.y - 224);

    worldEyeCases[0]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y - 224));
    worldEyeCases[1]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y - 160));
    worldEyeCases[2]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y - 224));
    worldEyeCases[3]->setPosition(Vector2f(worldcamPos.x - 224, worldcamPos.y - 224));
    worldEyeCases[4]->setPosition(Vector2f(worldcamPos.x - 224, worldcamPos.y - 160));
    worldEyeCases[5]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y - 192));
    worldEyeCases[6]->setPosition(Vector2f(worldcamPos.x - 192, worldcamPos.y - 224));
    worldEyeCases[7]->setPosition(Vector2f(worldcamPos.x - 192, worldcamPos.y - 160));
    worldEyeCases[8]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y - 160));
    worldEyeCases[9]->setPosition(Vector2f(worldcamPos.x - 160, worldcamPos.y - 224));
    worldEyeCases[10]->setPosition(Vector2f(worldcamPos.x - 160, worldcamPos.y - 160));
    worldEyeCases[11]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y - 128));
    worldEyeCases[12]->setPosition(Vector2f(worldcamPos.x - 128, worldcamPos.y - 224));
    worldEyeCases[13]->setPosition(Vector2f(worldcamPos.x - 128, worldcamPos.y - 160));
    worldEyeCases[14]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y - 96));
    worldEyeCases[15]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y - 64));
    worldEyeCases[16]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y - 32));
    worldEyeCases[17]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y - 64));
    worldEyeCases[18]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y - 64));
    worldEyeCases[19]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y - 224));
    worldEyeCases[20]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y - 160));
    worldEyeCases[21]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y - 224));
    worldEyeCases[22]->setPosition(Vector2f(worldcamPos.x + 160, worldcamPos.y - 224));
    worldEyeCases[23]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y - 160));
    worldEyeCases[24]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y - 160));
    worldEyeCases[25]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y - 128));
    worldEyeCases[26]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y - 128));
    worldEyeCases[27]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y - 96));
    worldEyeCases[28]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y - 96));
    worldEyeCases[29]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y - 64));
    worldEyeCases[30]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y - 64));
    worldEyeCases[31]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y - 32));
    worldEyeCases[32]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y - 96));
    worldEyeCases[33]->setPosition(Vector2f(worldcamPos.x, worldcamPos.y - 96));
    worldEyeCases[34]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y));
    worldEyeCases[35]->setPosition(Vector2f(worldcamPos.x - 224, worldcamPos.y));
    worldEyeCases[36]->setPosition(Vector2f(worldcamPos.x - 192, worldcamPos.y));
    worldEyeCases[37]->setPosition(Vector2f(worldcamPos.x - 160, worldcamPos.y));
    worldEyeCases[38]->setPosition(Vector2f(worldcamPos.x - 128, worldcamPos.y));
    worldEyeCases[39]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y + 32));
    worldEyeCases[40]->setPosition(Vector2f(worldcamPos.x - 224, worldcamPos.y + 32));
    worldEyeCases[41]->setPosition(Vector2f(worldcamPos.x - 192, worldcamPos.y + 32));
    worldEyeCases[42]->setPosition(Vector2f(worldcamPos.x - 160, worldcamPos.y + 32));
    worldEyeCases[43]->setPosition(Vector2f(worldcamPos.x - 128, worldcamPos.y + 32));
    worldEyeCases[44]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y + 32));
    worldEyeCases[45]->setPosition(Vector2f(worldcamPos.x - 64, worldcamPos.y + 32));
    worldEyeCases[46]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y + 32));
    worldEyeCases[47]->setPosition(Vector2f(worldcamPos.x, worldcamPos.y + 32));
    worldEyeCases[48]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y + 32));
    worldEyeCases[49]->setPosition(Vector2f(worldcamPos.x + 128, worldcamPos.y + 32));
    worldEyeCases[50]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y));
    worldEyeCases[51]->setPosition(Vector2f(worldcamPos.x - 64, worldcamPos.y));
    worldEyeCases[52]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y));
    worldEyeCases[53]->setPosition(Vector2f(worldcamPos.x, worldcamPos.y));
    worldEyeCases[54]->setPosition(Vector2f(worldcamPos.x - 256, worldcamPos.y + 96));
    worldEyeCases[55]->setPosition(Vector2f(worldcamPos.x - 224, worldcamPos.y + 96));
    worldEyeCases[56]->setPosition(Vector2f(worldcamPos.x - 192, worldcamPos.y + 96));
    worldEyeCases[57]->setPosition(Vector2f(worldcamPos.x - 160, worldcamPos.y + 96));
    worldEyeCases[58]->setPosition(Vector2f(worldcamPos.x + 224, worldcamPos.y + 32));
    worldEyeCases[59]->setPosition(Vector2f(worldcamPos.x + 192, worldcamPos.y + 32));
    worldEyeCases[60]->setPosition(Vector2f(worldcamPos.x + 192, worldcamPos.y + 64));
    worldEyeCases[61]->setPosition(Vector2f(worldcamPos.x - 128, worldcamPos.y + 96));
    worldEyeCases[62]->setPosition(Vector2f(worldcamPos.x - 96, worldcamPos.y + 96));
    worldEyeCases[63]->setPosition(Vector2f(worldcamPos.x - 64, worldcamPos.y + 96));
    worldEyeCases[64]->setPosition(Vector2f(worldcamPos.x - 32, worldcamPos.y + 96));
    worldEyeCases[65]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y - 32));
    worldEyeCases[66]->setPosition(Vector2f(worldcamPos.x + 96, worldcamPos.y - 32));
    worldEyeCases[67]->setPosition(Vector2f(worldcamPos.x + 32, worldcamPos.y + 96));
    worldEyeCases[68]->setPosition(Vector2f(worldcamPos.x + 96, worldcamPos.y - 96));
    worldEyeCases[69]->setPosition(Vector2f(worldcamPos.x + 128, worldcamPos.y - 32));
    worldEyeCases[70]->setPosition(Vector2f(worldcamPos.x + 192, worldcamPos.y - 32));
    worldEyeCases[71]->setPosition(Vector2f(worldcamPos.x + 96, worldcamPos.y - 224));
    worldEyeCases[72]->setPosition(Vector2f(worldcamPos.x + 96, worldcamPos.y - 160));
    worldEyeCases[73]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y + 96));
    worldEyeCases[74]->setPosition(Vector2f(worldcamPos.x + 96, worldcamPos.y + 96));
    worldEyeCases[75]->setPosition(Vector2f(worldcamPos.x + 128, worldcamPos.y + 96));
    worldEyeCases[76]->setPosition(Vector2f(worldcamPos.x + 160, worldcamPos.y + 96));
    worldEyeCases[77]->setPosition(Vector2f(worldcamPos.x + 64, worldcamPos.y + 128));
    worldEyeCases[78]->setPosition(Vector2f(worldcamPos.x + 96, worldcamPos.y + 128));
    worldEyeCases[79]->setPosition(Vector2f(worldcamPos.x + 128, worldcamPos.y + 128));
    worldEyeCases[80]->setPosition(Vector2f(worldcamPos.x + 160, worldcamPos.y + 128));

    worldTexSelection->setFillColor(Color(255, 255, 0, 128));
    worldTexSelection->setSize(Vector2f(32, 32));
    worldTexSelection->setPosition(-64, -64);
    worldTexSelection->setTexture(NULL);

    currentScreen = EYECANDIES;
}

static void Button_Lights()
{
    if (currentEditing != LIGHT)
        selectedEntity = NULL;

    currentEditing = LIGHT;

    if (!listLights.empty())
    {
        for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
            (*it)->setAlpha(255);
    }

    if (!listMarkers.empty())
    {
        for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
            (*it)->setAlpha(128);
    }

    worldLightSpr[2]->setColor(Color::White);
    worldLightSpr[4]->setColor(Color::White);
}

static void Button_Markers()
{
    if (currentEditing != MARKER)
        selectedEntity = NULL;

    currentEditing = MARKER;

    if (!listLights.empty())
    {
        for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
            (*it)->setAlpha(128);
    }

    if (!listMarkers.empty())
    {
        for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
            (*it)->setAlpha(255);
    }

    worldLightSpr[2]->setColor(Color::White);
    worldLightSpr[4]->setColor(Color::White);
}

static void Button_Title()
{
    panelwInfo[0].position = Vector2f(worldcamPos.x - 314, worldcamPos.y - 220);
    panelwInfo[1].position = Vector2f(worldcamPos.x - 300, worldcamPos.y - 220);
    panelwInfo[2].position = Vector2f(worldcamPos.x - 300, worldcamPos.y - 204);
    panelwInfo[3].position = Vector2f(worldcamPos.x - 314, worldcamPos.y - 204);

    panelwInfo[4].position = Vector2f(worldcamPos.x - 300, worldcamPos.y - 220);
    panelwInfo[5].position = Vector2f(worldcamPos.x + 300, worldcamPos.y - 220);
    panelwInfo[6].position = Vector2f(worldcamPos.x + 300, worldcamPos.y - 204);
    panelwInfo[7].position = Vector2f(worldcamPos.x - 300, worldcamPos.y - 204);

    panelwInfo[8].position = Vector2f(worldcamPos.x + 300, worldcamPos.y - 220);
    panelwInfo[9].position = Vector2f(worldcamPos.x + 314, worldcamPos.y - 220);
    panelwInfo[10].position = Vector2f(worldcamPos.x + 314, worldcamPos.y - 204);
    panelwInfo[11].position = Vector2f(worldcamPos.x + 300, worldcamPos.y - 204);

    panelwInfo[12].position = Vector2f(worldcamPos.x - 314, worldcamPos.y - 204);
    panelwInfo[13].position = Vector2f(worldcamPos.x - 300, worldcamPos.y - 204);
    panelwInfo[14].position = Vector2f(worldcamPos.x - 300, worldcamPos.y + 170);
    panelwInfo[15].position = Vector2f(worldcamPos.x - 314, worldcamPos.y + 170);

    panelwInfo[16].position = Vector2f(worldcamPos.x - 300, worldcamPos.y - 204);
    panelwInfo[17].position = Vector2f(worldcamPos.x + 300, worldcamPos.y - 204);
    panelwInfo[18].position = Vector2f(worldcamPos.x + 300, worldcamPos.y + 170);
    panelwInfo[19].position = Vector2f(worldcamPos.x - 300, worldcamPos.y + 170);

    panelwInfo[20].position = Vector2f(worldcamPos.x + 300, worldcamPos.y - 204);
    panelwInfo[21].position = Vector2f(worldcamPos.x + 314, worldcamPos.y - 204);
    panelwInfo[22].position = Vector2f(worldcamPos.x + 314, worldcamPos.y + 170);
    panelwInfo[23].position = Vector2f(worldcamPos.x + 300, worldcamPos.y + 170);

    panelwInfo[24].position = Vector2f(worldcamPos.x - 314, worldcamPos.y + 170);
    panelwInfo[25].position = Vector2f(worldcamPos.x - 300, worldcamPos.y + 170);
    panelwInfo[26].position = Vector2f(worldcamPos.x - 300, worldcamPos.y + 186);
    panelwInfo[27].position = Vector2f(worldcamPos.x - 314, worldcamPos.y + 186);

    panelwInfo[28].position = Vector2f(worldcamPos.x - 300, worldcamPos.y + 170);
    panelwInfo[29].position = Vector2f(worldcamPos.x + 300, worldcamPos.y + 170);
    panelwInfo[30].position = Vector2f(worldcamPos.x + 300, worldcamPos.y + 186);
    panelwInfo[31].position = Vector2f(worldcamPos.x - 300, worldcamPos.y + 186);

    panelwInfo[32].position = Vector2f(worldcamPos.x + 300, worldcamPos.y + 170);
    panelwInfo[33].position = Vector2f(worldcamPos.x + 314, worldcamPos.y + 170);
    panelwInfo[34].position = Vector2f(worldcamPos.x + 314, worldcamPos.y + 186);
    panelwInfo[35].position = Vector2f(worldcamPos.x + 300, worldcamPos.y + 186);

    worldInfoButton[0]->setPosition(worldcamPos.x-180, worldcamPos.y+180);
    worldInfoButton[1]->setPosition(worldcamPos.x, worldcamPos.y+180);
    worldInfoButton[2]->setPosition(worldcamPos.x+180, worldcamPos.y+180);
    worldInfoButton[3]->setPosition(worldcamPos.x-42, worldcamPos.y-60);
    worldInfoButton[4]->setPosition(worldcamPos.x+227, worldcamPos.y-60);
    worldInfoButton[5]->setPosition(worldcamPos.x, worldcamPos.y+48);
    worldInfoButton[6]->setPosition(worldcamPos.x-210, worldcamPos.y+200);
    worldInfoButton[7]->setPosition(worldcamPos.x-210, worldcamPos.y+233);
    worldInfoButton[8]->setPosition(worldcamPos.x-210, worldcamPos.y+266);
    worldInfoButton[9]->setPosition(worldcamPos.x-210, worldcamPos.y+299);
    worldInfoButton[10]->setPosition(worldcamPos.x-210, worldcamPos.y+332);
    worldInfoButton[11]->setPosition(worldcamPos.x-210, worldcamPos.y+365);
    worldInfoButton[12]->setPosition(worldcamPos.x-210, worldcamPos.y+398);
    worldInfoButton[13]->setPosition(worldcamPos.x-210, worldcamPos.y+431);
    worldInfoButton[14]->setPosition(worldcamPos.x-210, worldcamPos.y+464);
    worldInfoButton[15]->setPosition(worldcamPos.x-210, worldcamPos.y+497);
    worldInfoButton[16]->setPosition(worldcamPos.x-210, worldcamPos.y+530);
    worldInfoButton[17]->setPosition(worldcamPos.x, worldcamPos.y+200);
    worldInfoButton[18]->setPosition(worldcamPos.x, worldcamPos.y+233);
    worldInfoButton[19]->setPosition(worldcamPos.x, worldcamPos.y+266);
    worldInfoButton[20]->setPosition(worldcamPos.x, worldcamPos.y+299);
    worldInfoButton[21]->setPosition(worldcamPos.x, worldcamPos.y+332);
    worldInfoButton[22]->setPosition(worldcamPos.x, worldcamPos.y+365);
    worldInfoButton[23]->setPosition(worldcamPos.x, worldcamPos.y+398);
    worldInfoButton[24]->setPosition(worldcamPos.x, worldcamPos.y+431);
    worldInfoButton[25]->setPosition(worldcamPos.x, worldcamPos.y+464);
    worldInfoButton[26]->setPosition(worldcamPos.x, worldcamPos.y+497);
    worldInfoButton[27]->setPosition(worldcamPos.x, worldcamPos.y+530);
    worldInfoButton[28]->setPosition(worldcamPos.x+210, worldcamPos.y+200);
    worldInfoButton[29]->setPosition(worldcamPos.x, worldcamPos.y-160);

    worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

    worldTextSpr[0]->setPosition(worldcamPos.x-304, worldcamPos.y-206);
    worldTextSpr[1]->setPosition(worldcamPos.x-304, worldcamPos.y-126);
    worldTextSpr[2]->setPosition(worldcamPos.x-304, worldcamPos.y);
    worldTextSpr[3]->setPosition(worldcamPos.x-304, worldcamPos.y+82);
    worldTextSpr[5]->setPosition(worldcamPos.x, worldcamPos.y-226);

    currentScreen = TITLE;

    #ifdef DEBUGMODE
    cout << "Init End" << endl;
    #endif
}

static void Button_Return()
{
    if (currentScreen == MUSIC)
    {
        currentScreen = TITLE;

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);
    }
    else
    {
        if (currentScreen != TITLE)
            currentEditing = NONE;

        currentScreen = EDITOR;
    }
}

static void Eyecandy_Chosen(unsigned int id)
{
    Vector2u texSize(worldEyecandies[id]->getSize());

    currentTexture = id;

    worldTexSelection->setSize(static_cast<Vector2f>(texSize));
    worldTexSelection->setTexture(worldEyecandies[id]);
    worldTexSelection->setTextureRect(IntRect(0, 0, texSize.x, texSize.y));
    worldTexSelection->setFillColor(Color(255, 255, 255, 128));

    worldLightSpr[2]->setColor(Color(255, 255, 255, 128));
    worldLightSpr[4]->setColor(Color::White);

    selectedEntity = NULL;

    currentScreen = EDITOR;
    currentEditing = EYECANDY;
}

static void Light_Color()
{
    extern bool showCursor;
    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
    CHOOSECOLOR colorChooser;
    COLORREF colorRef;

    if (selectedEntity == NULL)
        colorRef = RGB(currentLightColor.r, currentLightColor.g, currentLightColor.b);
    else
    {
        const Light_Entity* tempLight = reinterpret_cast<Light_Entity*>(selectedEntity);
        colorRef = RGB(tempLight->m_lightcolor.r, tempLight->m_lightcolor.g, tempLight->m_lightcolor.b);
    }

    ZeroMemory(&colorChooser, sizeof(CHOOSECOLOR));

    colorChooser.lStructSize = sizeof(CHOOSECOLOR);
    colorChooser.hwndOwner = mainWindow->getSystemHandle();
    colorChooser.rgbResult = colorRef;
    colorChooser.lpCustColors = colorArray;
    colorChooser.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_SHOWHELP;

    if (ChooseColor(&colorChooser))
    {
        currentLightColor = Color(GetRValue(colorChooser.rgbResult), GetGValue(colorChooser.rgbResult), GetBValue(colorChooser.rgbResult));

        if (selectedEntity != NULL)
            reinterpret_cast<Light_Entity*>(selectedEntity)->setColor(GetRValue(colorChooser.rgbResult), GetGValue(colorChooser.rgbResult), GetBValue(colorChooser.rgbResult));
    }

    ofstream colorPrefs;

    colorPrefs.open("Data/Save/Colors.dat", ios::binary);

    if (colorPrefs.good())
    {
        if(!colorPrefs.write(reinterpret_cast<char*>(&colorArray), sizeof(COLORREF) * 16))
            MessageBox(NULL, "Failed to save the Preferences Colors !", "Failed to save colors !", MB_TASKMODAL | MB_OK | MB_ICONERROR);

        colorPrefs.close();
    }
#else
    QColor colorRef;

    if (selectedEntity == NULL)
        colorRef.setRgb(currentLightColor.r, currentLightColor.g, currentLightColor.b);
    else
    {
        const Light_Entity* tempLight = reinterpret_cast<Light_Entity*>(selectedEntity);
        colorRef.setRgb(tempLight->m_lightcolor.r, tempLight->m_lightcolor.g, tempLight->m_lightcolor.b);
    }

    QColorDialog colorChooser(colorRef);
    if (colorChooser.exec())
    {
        currentLightColor = Color(colorChooser.currentColor().red(), colorChooser.currentColor().green(), colorChooser.currentColor().blue());

        if (selectedEntity != NULL)
            reinterpret_cast<Light_Entity*>(selectedEntity)->setColor(colorChooser.currentColor().red(), colorChooser.currentColor().green(), colorChooser.currentColor().blue());
    }
#endif
    mainWindow->setMouseCursorVisible(showCursor);
}

static void Light_TexIllumin()
{
    if (selectedEntity == NULL)
    {
        if (currentLight_TexIllum == false)
            currentLight_TexIllum = true;
        else
            currentLight_TexIllum = false;
    }
    else
    {
        bool currentState = reinterpret_cast<Light_Entity*>(selectedEntity)->m_littextures;

        if (currentState == false)
            reinterpret_cast<Light_Entity*>(selectedEntity)->setTexIllumination(true);
        else
            reinterpret_cast<Light_Entity*>(selectedEntity)->setTexIllumination(false);
    }
}

static void Light_EyeIllumin()
{
    if (selectedEntity == NULL)
    {
        if (currentLight_EyeIllum == false)
            currentLight_EyeIllum = true;
        else
            currentLight_EyeIllum = false;
    }
    else
    {
        bool currentState = reinterpret_cast<Light_Entity*>(selectedEntity)->m_liteyecandies;

        if (currentState == false)
            reinterpret_cast<Light_Entity*>(selectedEntity)->setEyeIllumination(true);
        else
            reinterpret_cast<Light_Entity*>(selectedEntity)->setEyeIllumination(false);
    }
}

static void Mark_SetTurn()
{
    if (selectedEntity == NULL)
        currentMarker_IsLevel = false;
    else
        reinterpret_cast<Marker_Entity*>(selectedEntity)->setIsLevel(false);
}

static void Mark_SetLevel()
{
    if (selectedEntity == NULL)
        currentMarker_IsLevel = true;
    else
        reinterpret_cast<Marker_Entity*>(selectedEntity)->setIsLevel(true);
}

static void Background_Default()
{
    worldBackground = 0;

    worldBack->setTexture(worldBackTex[0]);

    if (worldBackTex[1] != NULL)
    {
        delete worldBackTex[1];
        worldBackTex[1] = NULL;
    }

    removeResource(0);

    worldInfoButton[3]->freeze();
}

static void Background_Import()
{
    extern bool showCursor;
    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
    OPENFILENAME dialogParms;

    ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

    dialogParms.lStructSize = sizeof(OPENFILENAME);
    dialogParms.hwndOwner = mainWindow->getSystemHandle();
    dialogParms.hInstance = *mainInstance;
    dialogParms.lpstrFile = wfilegrab;
    dialogParms.nFilterIndex = 1;
    dialogParms.nMaxFile = sizeof(wfilegrab);
    dialogParms.lpstrTitle = "Import a valid background :";
    dialogParms.lpstrFilter = "Supported Image Formats\0*.bmp;*.dds;*.jpg;*.png;*.tga;*.psd\0";
    dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&dialogParms))
#else
    const QString filename(QFileDialog::getOpenFileName(NULL, QStringLiteral("Import a valid background :"), QDir::homePath(), QStringLiteral("Supported Image Formats (*.bmp *.dds *.jpg *.png *.tga *.psd)")));
    if (!filename.isEmpty())
#endif
    {
        if (worldBackTex[1] != NULL)
            delete worldBackTex[1];

        worldBackTex[1] = new Texture;

        if (!worldBackTex[1]->loadFromFile(wfilegrab))
        {
#ifndef LINUX
            MessageBox(NULL, "The file format is not supported !", "File unreadable !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("File unreadable !"), QStringLiteral("The file format is not supported !"), QMessageBox::Ok);
            messageBox.exec();
#endif
            delete worldBackTex[1];

            worldBackTex[1] = NULL;

            return;
        }

        worldBackTex[1]->setRepeated(true);
        worldBack->setTexture(worldBackTex[1]);

        addNewResource(wfilegrab, 1);

        worldBackground = 1;

        worldInfoButton[3]->freeze();
    }
    mainWindow->setMouseCursorVisible(showCursor);
}

static void Music_Edit()
{
    if (editorMusic)
        FMOD_Channel_Stop(musicChannel);

    edlastButton = 0;

    worldReturnButton->setPosition(worldcamPos.x+282, worldcamPos.y+206);

    currentScreen = MUSIC;
}

static void Music_World1()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 1)
    {
        worldMusic = 0;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[6]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 1;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[0], 0, &previewWorldMusicChannel);
    }
}

static void Music_Cave()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 2)
    {
        worldMusic = 1;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[7]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 2;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[1], 0, &previewWorldMusicChannel);
    }
}

static void Music_Underground()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 3)
    {
        worldMusic = 2;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[8]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 3;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[2], 0, &previewWorldMusicChannel);
    }
}

static void Music_Rockland()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 4)
    {
        worldMusic = 3;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[9]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 4;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[3], 0, &previewWorldMusicChannel);
    }
}

static void Music_Starland()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 5)
    {
        worldMusic = 4;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[10]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 5;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[4], 0, &previewWorldMusicChannel);
    }
}

static void Music_StarPlatforms()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 6)
    {
        worldMusic = 5;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[11]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 6;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[5], 0, &previewWorldMusicChannel);
    }
}

static void Music_Underwater()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 7)
    {
        worldMusic = 6;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[12]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 7;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[6], 0, &previewWorldMusicChannel);
    }
}

static void Music_SelectSave()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 8)
    {
        worldMusic = 7;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[13]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 8;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[7], 0, &previewWorldMusicChannel);
    }
}

static void Music_GhostHouse()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 9)
    {
        worldMusic = 8;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[14]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 9;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[8], 0, &previewWorldMusicChannel);
    }
}

static void Music_Castle1()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 10)
    {
        worldMusic = 9;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[15]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 10;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[9], 0, &previewWorldMusicChannel);
    }
}

static void Music_Castle2()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 11)
    {
        worldMusic = 10;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[16]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 11;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[10], 0, &previewWorldMusicChannel);
    }
}

static void Music_Yoshi1()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 12)
    {
        worldMusic = 11;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[17]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 12;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[11], 0, &previewWorldMusicChannel);
    }
}

static void Music_Yoshi2()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 13)
    {
        worldMusic = 12;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[18]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 13;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[12], 0, &previewWorldMusicChannel);
    }
}

static void Music_Map1()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 14)
    {
        worldMusic = 13;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[19]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 14;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[13], 0, &previewWorldMusicChannel);
    }
}

static void Music_Map2()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 15)
    {
        worldMusic = 14;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[20]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 15;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[14], 0, &previewWorldMusicChannel);
    }
}

static void Music_Map3()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 16)
    {
        worldMusic = 15;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[21]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 16;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[15], 0, &previewWorldMusicChannel);
    }
}

static void Music_Fastrun1()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 17)
    {
        worldMusic = 16;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[22]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 17;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[16], 0, &previewWorldMusicChannel);
    }
}

static void Music_Fastrun2()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 18)
    {
        worldMusic = 17;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[23]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 18;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[17], 0, &previewWorldMusicChannel);
    }
}

static void Music_InDaKitchen()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 19)
    {
        worldMusic = 18;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[24]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 19;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[18], 0, &previewWorldMusicChannel);
    }
}

static void Music_Yky()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 20)
    {
        worldMusic = 19;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[25]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 20;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[19], 0, &previewWorldMusicChannel);
    }
}

static void Music_YetBonus()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 21)
    {
        worldMusic = 20;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[26]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 21;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[20], 0, &previewWorldMusicChannel);
    }
}

static void Music_Apoplexy()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 22)
    {
        worldMusic = 21;

        FMOD_Channel_Stop(previewWorldMusicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        removeResource(1);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[27]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        edlastButton = 22;

        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[21], 0, &previewWorldMusicChannel);
    }
}

static void Music_Import()
{
    FMOD_BOOL isPlaying;

    if (edlastButton == 23)
    {
        worldMusic = 22;

        addNewResource(wfilegrab, 0);

        FMOD_Channel_Stop(previewWorldMusicChannel);

        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), edbuttonClick, 0, NULL);

        if (editorMusic)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), worldedMusic, 0, &musicChannel);

        worldInfoButton[28]->freeze();

        worldReturnButton->setPosition(worldcamPos.x-282, worldcamPos.y+206);

        currentScreen = TITLE;
    }
    else
    {
        extern bool showCursor;
        mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
        OPENFILENAME dialogParms;

        ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

        dialogParms.lStructSize = sizeof(OPENFILENAME);
        dialogParms.hwndOwner = mainWindow->getSystemHandle();
        dialogParms.hInstance = *mainInstance;
        dialogParms.lpstrFile = wfilegrab;
        dialogParms.nFilterIndex = 1;
        dialogParms.nMaxFile = sizeof(wfilegrab);
        dialogParms.lpstrTitle = "Import a valid music :";
        dialogParms.lpstrFilter = "All Sound Formats\0*.aiff;*.asf;*.asx;*.dls;*.flac;*.fsb;*.it;*.m3u;*.mid;*.mod;*.mp2;*.mp3;*.ogg;*.pls;*.s3m;*.vag;*.wav;*.wax;*.wma;*.xm;*.xma\0";
        dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;
#endif
        FMOD_Channel_IsPlaying(previewWorldMusicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(previewWorldMusicChannel);

#ifndef LINUX
        if (GetOpenFileName(&dialogParms))
#else
        const QString filename(QFileDialog::getOpenFileName(NULL, QStringLiteral("Import a valid music :"), QDir::homePath(), QStringLiteral("All Sound Formats (*.aiff *.asf *.asx *.dls *.flac *.fsb *.it *.m3u *.mid *.mod *.mp2 *.mp3 *.ogg *.pls *.s3m *.vag *.wav *.wax *.wma *.xm *.xma)")));
        if (!filename.isEmpty())
#endif
        {
            FMOD_RESULT result;

            edlastButton = 23;

            if (musicSamples[22] != NULL)
            {
                FMOD_Sound_Release(musicSamples[22]);
                musicSamples[22] = NULL;
            }
#ifdef LINUX
            strcpy(wfilegrab, filename.toLocal8Bit().constData());
#endif
            result = FMOD_System_CreateStream(soundSystem, wfilegrab, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[22]);

            if (result == FMOD_OK)
            {
                FMOD_RESULT resutl;

                resutl = FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), musicSamples[22], 0, &previewWorldMusicChannel);

                if (resutl != FMOD_OK)
                {
#ifndef LINUX
                    MessageBox(NULL, "The file cannot be played !", "File unplayable !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                    QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("File unplayable !"), QStringLiteral("The file cannot be played !"), QMessageBox::Ok);
                    messageBox.exec();
#endif
                }
            }
            else
            {
#ifndef LINUX
                MessageBox(NULL, "The file format is not supported !", "File unreadable !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("File unreadable !"), QStringLiteral("The file format is not supported !"), QMessageBox::Ok);
                messageBox.exec();
#endif
            }
        }
        mainWindow->setMouseCursorVisible(showCursor);
    }
}

#ifndef LINUX
LRESULT CALLBACK dialogProcSize(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND handleEdit[3];
    TCHAR textBufferx[3];
    TCHAR textBuffery[3];
    TCHAR textBufferl[2];

    unsigned int number = 0;

    switch(message)
    {
        case WM_INITDIALOG :
            SendDlgItemMessage(windowHandle, 58, UDM_SETRANGE, 0, MAKELPARAM(20, 200));
            SendDlgItemMessage(windowHandle, 60, UDM_SETRANGE, 0, MAKELPARAM(15, 150));
            SendDlgItemMessage(windowHandle, 73, UDM_SETRANGE, 0, MAKELPARAM(0, 99));

            SendDlgItemMessage(windowHandle, 58, UDM_SETPOS, 0, mapScale.x);
            SendDlgItemMessage(windowHandle, 60, UDM_SETPOS, 0, mapScale.y);
            SendDlgItemMessage(windowHandle, 73, UDM_SETPOS, 0, worldLives);

            return TRUE;

        case WM_COMMAND :
            switch(wParam)
            {
                case 61 :
                    EndDialog(windowHandle, 0);

                    handleEdit[0] = GetDlgItem(windowHandle, 57);
                    handleEdit[1] = GetDlgItem(windowHandle, 59);
                    handleEdit[2] = GetDlgItem(windowHandle, 72);

                    GetWindowText(handleEdit[0], textBufferx, 3);
                    GetWindowText(handleEdit[1], textBuffery, 3);
                    GetWindowText(handleEdit[2], textBufferl, 2);

                    mapScale.x = max(min(atoi(textBufferx), 200), 20);
                    mapScale.y = max(min(atoi(textBuffery), 150), 15);
                    worldLives = max(min(atoi(textBufferl), 99), 0);

                    worldBack->setSize(Vector2f(mapScale.x * 32, mapScale.y * 32));
                    worldBack->setTextureRect(IntRect(0, 0, mapScale.x * 32, mapScale.y * 32));

                    matrixMarker->resize((mapScale.x * 2) + 1, (mapScale.y * 2) + 1);

                    if (!listTextures.empty())
                    {
                        vector<list<TextureData*>::iterator> texToErase;

                        for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
                        {
                            FloatRect texBounds((*it)->rectangle.getGlobalBounds());

                            if (texBounds.left + texBounds.width > mapScale.x * 32 || texBounds.top + texBounds.height > mapScale.y * 32)
                                texToErase.emplace_back(it);
                        }

                        for (vector<list<TextureData*>::iterator>::iterator it = texToErase.begin(); it != texToErase.end(); it++)
                            listTextures.erase(*it);

                        texToErase.clear();
                    }

                    if (!listEyecandies.empty())
                    {
                        vector<list<EyecandyData*>::iterator> eyeToErase;

                        for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
                        {
                            FloatRect eyeBounds((*it)->sprite.getGlobalBounds());

                            if (eyeBounds.left + eyeBounds.width > mapScale.x * 32 || eyeBounds.top + eyeBounds.height > mapScale.y * 32)
                                eyeToErase.emplace_back(it);
                        }

                        for (vector<list<EyecandyData*>::iterator>::iterator it = eyeToErase.begin(); it != eyeToErase.end(); it++)
                            listEyecandies.erase(*it);

                        eyeToErase.clear();
                    }

                    if (!listLights.empty())
                    {
                        vector<list<Light_Entity*>::iterator> lightToErase;

                        for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
                        {
                            Vector2f pos((*it)->getPosition());

                            if (pos.x + 32 > mapScale.x * 32 || pos.y + 32 > mapScale.y * 32)
                                lightToErase.emplace_back(it);
                            else
                                (*it)->mapResized(mapScale);
                        }

                        for (vector<list<Light_Entity*>::iterator>::iterator it = lightToErase.begin(); it != lightToErase.end(); it++)
                            listLights.erase(*it);

                        lightToErase.clear();
                    }

                    if (!listMarkers.empty())
                    {
                        vector<list<Marker_Entity*>::iterator> markerToErase;

                        for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
                        {
                            Vector2f pos((*it)->getPosition());

                            if (pos.x > mapScale.x * 32 || pos.y > mapScale.y * 32)
                                markerToErase.emplace_back(it);
                        }

                        for (vector<list<Marker_Entity*>::iterator>::iterator it = markerToErase.begin(); it != markerToErase.end(); it++)
                            listMarkers.erase(*it);

                        markerToErase.clear();
                        worldMarkerLink.clear();

                        for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
                        {
                            Vector2f pos((*it)->getPosition() + Vector2f(12, 10));

                            (*it)->setNumber(number);

                            worldMarkerLink.emplace_back(Vertex(pos, Color(128, 255, 255)));

                            number++;
                        }
                    }

                    selectedEntity = NULL;

                    return TRUE;
                case 62 :
                    EndDialog(windowHandle, 0);

                    return TRUE;
            }

            break;
    }

    return FALSE;
}
#endif

static void Info_New()
{
    for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
        delete *it;

    for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
        delete *it;

    for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
        delete *it;

    for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
        delete *it;

    listTextures.clear();
    listEyecandies.clear();
    listLights.clear();
    listMarkers.clear();

    worldMarkerLink.clear();

    currentLightColor = Color::White;
    currentLight_TexIllum = true;
    currentLight_EyeIllum = true;
    currentMarker_IsLevel = false;
    currentMarker_Number = 0;

    delete matrixMarker;

    if (wresourcesArray[0] != NULL)
    {
        delete wresourcesArray[0];
        wresourcesArray[0] = NULL;
    }

    if (wresourcesArray[1] != NULL)
    {
        delete wresourcesArray[1];
        wresourcesArray[1] = NULL;
    }

    if (worldBackTex[1] != NULL)
    {
        delete worldBackTex[1];

        worldBackTex[1] = NULL;
    }

    if (musicSamples[22] != NULL)
    {
        FMOD_Sound_Release(musicSamples[22]);
        musicSamples[22] = NULL;
    }

    worldCamSpeed = Vector2f(0, 0);
    worldcamPos = Vector2i(320, 240);

    mapScale = Vector2i(20, 15);

    matrixMarker = new Matrix(41, 31);

    worldBackground = 0;
    worldMusic = 0;
    worldLives = 4;

    worldDir.clear();

    worldBack->setSize(Vector2f(640, 480));
    worldBack->setTextureRect(IntRect(0, 0, 640, 480));
    worldBack->setTexture(worldBackTex[0]);

    selectedEntity = NULL;
    currentEditing = NONE;
}

static void Size_Edit()
{
    extern bool showCursor;
    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
    DialogBox(*mainInstance, MAKEINTRESOURCE(54), mainWindow->getSystemHandle(), reinterpret_cast<DLGPROC>(dialogProcSize));
#else
    QDialog dialog;
    QFormLayout form(&dialog);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    if (dialog.exec() == QDialog::Accepted)
    {
        /*mapScale.x = max(min(x, 200), 20);
        mapScale.y = max(min(y, 150), 15);
        worldLives = max(min(l, 99), 0);

        worldBack->setSize(Vector2f(mapScale.x * 32, mapScale.y * 32));
        worldBack->setTextureRect(IntRect(0, 0, mapScale.x * 32, mapScale.y * 32));

        matrixMarker->resize((mapScale.x * 2) + 1, (mapScale.y * 2) + 1);

        if (!listTextures.empty())
        {
            vector<list<TextureData*>::iterator> texToErase;

            for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
            {
                FloatRect texBounds((*it)->rectangle.getGlobalBounds());

                if (texBounds.left + texBounds.width > mapScale.x * 32 || texBounds.top + texBounds.height > mapScale.y * 32)
                    texToErase.emplace_back(it);
            }

            for (vector<list<TextureData*>::iterator>::iterator it = texToErase.begin(); it != texToErase.end(); it++)
                listTextures.erase(*it);

            texToErase.clear();
        }

        if (!listEyecandies.empty())
        {
            vector<list<EyecandyData*>::iterator> eyeToErase;

            for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
            {
                FloatRect eyeBounds((*it)->sprite.getGlobalBounds());

                if (eyeBounds.left + eyeBounds.width > mapScale.x * 32 || eyeBounds.top + eyeBounds.height > mapScale.y * 32)
                    eyeToErase.emplace_back(it);
            }

            for (vector<list<EyecandyData*>::iterator>::iterator it = eyeToErase.begin(); it != eyeToErase.end(); it++)
                listEyecandies.erase(*it);

            eyeToErase.clear();
        }

        if (!listLights.empty())
        {
                        vector<list<Light_Entity*>::iterator> lightToErase;

                        for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
                        {
                            Vector2f pos((*it)->getPosition());

                            if (pos.x + 32 > mapScale.x * 32 || pos.y + 32 > mapScale.y * 32)
                                lightToErase.emplace_back(it);
                            else
                                (*it)->mapResized(mapScale);
                        }

                        for (vector<list<Light_Entity*>::iterator>::iterator it = lightToErase.begin(); it != lightToErase.end(); it++)
                            listLights.erase(*it);

                        lightToErase.clear();
        }

        if (!listMarkers.empty())
        {
            vector<list<Marker_Entity*>::iterator> markerToErase;

            for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
            {
                Vector2f pos((*it)->getPosition());

                if (pos.x > mapScale.x * 32 || pos.y > mapScale.y * 32)
                    markerToErase.emplace_back(it);
            }

            for (vector<list<Marker_Entity*>::iterator>::iterator it = markerToErase.begin(); it != markerToErase.end(); it++)
                listMarkers.erase(*it);

            markerToErase.clear();
            worldMarkerLink.clear();

            for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
            {
                Vector2f pos((*it)->getPosition() + Vector2f(12, 10));

                (*it)->setNumber(number);

                worldMarkerLink.emplace_back(Vertex(pos, Color(128, 255, 255)));

                number++;
            }
        }

        selectedEntity = NULL;*/
    }
    mainWindow->setMouseCursorVisible(showCursor);
#endif
}

static void Info_Save()
{
    if (worldDir.empty())
    {
        extern bool showCursor;
        mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
        OPENFILENAME dialogParms;
        TCHAR personalPath[MAX_PATH];

        SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

        strcat(personalPath, "\\Mario Constructor Master\\Worlds");

        ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

        dialogParms.lStructSize = sizeof(OPENFILENAME);
        dialogParms.hwndOwner = mainWindow->getSystemHandle();
        dialogParms.hInstance = *mainInstance;
        dialogParms.lpstrFile = wfilegrab;
        dialogParms.nFilterIndex = 1;
        dialogParms.nMaxFile = sizeof(wfilegrab);
        dialogParms.lpstrInitialDir = personalPath;
        dialogParms.lpstrTitle = "Save a World :";
        dialogParms.lpstrFilter = "Constructor Master Worlds (.cmw)\0*.cmw\0";
        dialogParms.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

        if (GetSaveFileName(&dialogParms))
        {
            PathRenameExtension(wfilegrab, ".cmw");
            worldDir = wfilegrab;
#else
        const QString filename(QFileDialog::getSaveFileName(NULL, QStringLiteral("Save a World :"), QDir::homePath() + QStringLiteral("/Mario Constructor Master/Worlds"), QStringLiteral("Constructor Master Worlds (.cmw)")));
        if (!filename.isEmpty())
        {
            worldDir = filename.toStdString();
#endif
            checkResources(worldDir.c_str(), false);
            World_Save(worldDir);
        }
        mainWindow->setMouseCursorVisible(showCursor);
    }
    else
        World_Save(worldDir);
}

static void Info_SaveAs()
{
    extern bool showCursor;
    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
    OPENFILENAME dialogParms;
    TCHAR personalPath[MAX_PATH];

    SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

    strcat(personalPath, "\\Mario Constructor Master\\Worlds");

    ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

    dialogParms.lStructSize = sizeof(OPENFILENAME);
    dialogParms.hwndOwner = mainWindow->getSystemHandle();
    dialogParms.hInstance = *mainInstance;
    dialogParms.lpstrFile = wfilegrab;
    dialogParms.nFilterIndex = 1;
    dialogParms.nMaxFile = sizeof(wfilegrab);
    dialogParms.lpstrInitialDir = personalPath;
    dialogParms.lpstrTitle = "Save a World :";
    dialogParms.lpstrFilter = "Constructor Master Worlds (.cmw)\0*.cmw\0";
    dialogParms.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&dialogParms))
    {
        PathRenameExtension(wfilegrab, ".cmw");
        worldDir = wfilegrab;
#else
    const QString filename(QFileDialog::getSaveFileName(NULL, QStringLiteral("Save a World :"), QDir::homePath() + QStringLiteral("/Mario Constructor Master/Worlds"), QStringLiteral("Constructor Master Worlds (.cmw)")));
    if (!filename.isEmpty())
    {
        worldDir = filename.toStdString();
#endif
        checkResources(worldDir.c_str(), true);
        World_Save(worldDir);
    }
    mainWindow->setMouseCursorVisible(showCursor);
}

static void Info_Load()
{
    extern bool showCursor;
    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
    OPENFILENAME dialogParms;
    TCHAR personalPath[MAX_PATH];

    SHGetFolderPath(mainWindow->getSystemHandle(), CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, personalPath);

    strcat(personalPath, "\\Mario Constructor Master\\Worlds");

    ZeroMemory(&dialogParms, sizeof(OPENFILENAME));

    dialogParms.lStructSize = sizeof(OPENFILENAME);
    dialogParms.hwndOwner = mainWindow->getSystemHandle();
    dialogParms.hInstance = *mainInstance;
    dialogParms.lpstrFile = wfilegrab;
    dialogParms.nFilterIndex = 1;
    dialogParms.nMaxFile = sizeof(wfilegrab);
    dialogParms.lpstrInitialDir = personalPath;
    dialogParms.lpstrTitle = "Load a World :";
    dialogParms.lpstrFilter = "Constructor Master Worlds (.cmw)\0*.cmw\0";
    dialogParms.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&dialogParms))
#else
    const QString filename(QFileDialog::getOpenFileName(NULL, QStringLiteral("Load a World :"), QDir::homePath() + QStringLiteral("/Mario Constructor Master/Worlds"), QStringLiteral("Constructor Master Worlds (.cmw)")));
    if (!filename.isEmpty())
#endif
    {
        worldCamera->setCenter(320, 240);
        mainTexture.setView(*worldCamera);
#ifndef LINUX
        World_Load(wfilegrab);
#else
        World_Load(filename.toLocal8Bit().constData());
#endif
        if (currentScreen == TITLE)
            Button_Title();
    }
    mainWindow->setMouseCursorVisible(showCursor);
}

static void Erasement_Light()
{
    if (selectedEntity != NULL)
    {
        const char* matcherb = reinterpret_cast<Light_Entity*>(selectedEntity)->identifier;
        char* matcher;

        for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
        {
            matcher = (*it)->identifier;

            if (matcher == matcherb)
            {
                delete *it;
                listLights.erase(it);

                selectedEntity = NULL;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), editemDelete, 0, NULL);

                break;
            }
        }
    }
}

static void Erasement_Marker()
{
    bool onedeleted = false;

    vector<Vertex>::iterator iteration = worldMarkerLink.begin();
    list<Marker_Entity*>::iterator deletedone;

    if (selectedEntity != NULL)
    {
        const char* matcherb = reinterpret_cast<Marker_Entity*>(selectedEntity)->identifier;
        char* matcher;

        for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
        {
            if (!onedeleted)
            {
                matcher = (*it)->identifier;

                if (matcher == matcherb)
                {
                    Vector2f matcherPos((*it)->getPosition());

                    matrixMarker->setValue((matcherPos.x + 12) / 16, (matcherPos.y+10) / 16, 0);

                    onedeleted = true;

                    deletedone = it;

                    worldMarkerLink.erase(iteration);

                    selectedEntity = NULL;

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), editemDelete, 0, NULL);
                }
            }
            else
                (*it)->setNumber();

            iteration++;
        }

        currentMarker_Number--;

        if (onedeleted)
        {
            delete *deletedone;
            listMarkers.erase(deletedone);
        }
    }
}

static void World_Save(const string& filename)
{
    ofstream worldFile;

    worldFile.open(filename.c_str(), ios::binary);

    if (!worldFile.good())
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to create the World file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to create the World file !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return;
    }

    {
        char CMLid[4];

        CMLid[0] = 'C';
        CMLid[1] = 'M';
        CMLid[2] = 'W';
        CMLid[3] = EDITOR_VERSION;

        worldFile.write(CMLid, 4);
    }

    {
        char nullChar = '\0';

        for (register unsigned int i = 0; i < 2; i++)
        {
            if (wresourcesArray[i] == NULL)
                worldFile.write(&nullChar, 1);
            else
                worldFile.write(wresourcesArray[i]->c_str(), wresourcesArray[i]->size() + 1);
        }
    }

    worldFile.write(reinterpret_cast<char*>(&mapScale.x), 4);
    worldFile.write(reinterpret_cast<char*>(&mapScale.y), 4);

    worldFile.write(reinterpret_cast<char*>(&worldBackground), 1);
    worldFile.write(reinterpret_cast<char*>(&worldMusic), 1);
    worldFile.write(reinterpret_cast<char*>(&worldLives), 1);

    {
        unsigned short listSize = listTextures.size();

        worldFile.write(reinterpret_cast<char*>(&listSize), 2);

        for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
        {
            FloatRect texBounds((*it)->rectangle.getGlobalBounds());
            unsigned int texId = (*it)->id;

            worldFile.write(reinterpret_cast<char*>(&texId), 4);
            worldFile.write(reinterpret_cast<char*>(&texBounds.left), 4);
            worldFile.write(reinterpret_cast<char*>(&texBounds.top), 4);
            worldFile.write(reinterpret_cast<char*>(&texBounds.width), 4);
            worldFile.write(reinterpret_cast<char*>(&texBounds.height), 4);
        }
    }

    {
        unsigned short listSize = listEyecandies.size();

        worldFile.write(reinterpret_cast<char*>(&listSize), 2);

        for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
        {
            Vector2f pos((*it)->sprite.getPosition());
            unsigned int texId = (*it)->id;

            worldFile.write(reinterpret_cast<char*>(&texId), 4);
            worldFile.write(reinterpret_cast<char*>(&pos.x), 4);
            worldFile.write(reinterpret_cast<char*>(&pos.y), 4);
        }
    }

    {
        unsigned short listSize = listLights.size();

        worldFile.write(reinterpret_cast<char*>(&listSize), 2);

        for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
        {
            LightData data;

            (*it)->save(data);

            worldFile.write(reinterpret_cast<char*>(&data), sizeof(LightData));
        }
    }

    {
        unsigned short listSize = listMarkers.size();

        worldFile.write(reinterpret_cast<char*>(&listSize), 2);

        for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
        {
            MarkerData data;

            (*it)->save(data);

            worldFile.write(reinterpret_cast<char*>(&data), sizeof(MarkerData));
        }
    }

    worldFile.close();

    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), edsaveSound, 0, NULL);
}

static void World_Load(const string& filename)
{
    ifstream worldFile;

    worldFile.open(filename.c_str(), ios::binary);

    if (!worldFile.good())
    {
#ifndef LINUX
        MessageBox(NULL, "Failed to open the World file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("Failed to open the World file !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return;
    }

    if (!checkwLoadResources(worldFile, filename.c_str()))
    {
        worldFile.close();
        return;
    }

    for (list<TextureData*>::iterator it = listTextures.begin(); it != listTextures.end(); it++)
        delete *it;

    for (list<EyecandyData*>::iterator it = listEyecandies.begin(); it != listEyecandies.end(); it++)
        delete *it;

    for (list<Light_Entity*>::iterator it = listLights.begin(); it != listLights.end(); it++)
        delete *it;

    for (list<Marker_Entity*>::iterator it = listMarkers.begin(); it != listMarkers.end(); it++)
        delete *it;

    listTextures.clear();
    listEyecandies.clear();
    listLights.clear();
    listMarkers.clear();

    worldMarkerLink.clear();

    delete matrixMarker;

    if (wresourcesArray[0] != NULL)
    {
        delete wresourcesArray[0];
        wresourcesArray[0] = NULL;
    }

    if (wresourcesArray[1] != NULL)
    {
        delete wresourcesArray[1];
        wresourcesArray[1] = NULL;
    }

    if (worldBackTex[1] != NULL)
    {
        delete worldBackTex[1];

        worldBackTex[1] = NULL;
    }

    if (musicSamples[22] != NULL)
    {
        FMOD_Sound_Release(musicSamples[22]);
        musicSamples[22] = NULL;
    }

    worldCamSpeed = Vector2f(0, 0);
    worldcamPos = Vector2i(320, 240);

    worldFile.seekg(4, ios::beg);

    worldDir = filename;

    currentLightColor = Color::White;
    currentLight_TexIllum = true;
    currentLight_EyeIllum = true;
    currentMarker_IsLevel = false;
    currentMarker_Number = 0;

    {
        extern char procPath[MAX_PATH];

#ifndef LINUX
        TCHAR filePath[MAX_PATH];
        TCHAR getString[MAX_PATH];

        strcpy(filePath, filename.c_str());

        PathRemoveFileSpec(filePath);
        SetCurrentDirectory(filePath);

        for (register unsigned int i = 0; i < 2; i++)
        {
            worldFile.read(getString, 1);

            if (getString[0] != '\0')
            {
                worldFile.seekg(-1, ios::cur);

                for (register unsigned int j = 0; true; j++)
                {
                    worldFile.read(&getString[j], 1);

                    if (getString[j] == '\0')
                        break;
                }

                switch (i)
                {
                    case 0 :
                        FMOD_System_CreateStream(soundSystem, getString, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[22]);

                        wresourcesArray[1] = new string(getString);

                        break;

                    case 1 :
                        worldBackTex[1] = new Texture;
                        worldBackTex[1]->loadFromFile(getString);
                        worldBackTex[1]->setRepeated(true);

                        wresourcesArray[0] = new string(getString);

                        break;
                }
            }
        }

        // Reset the current directory :
        SetCurrentDirectory(procPath);
#else
        char getString[MAX_PATH];

        chdir(filename.substr(0, filename.find_last_of('/')).c_str());

        for (register unsigned int i = 0; i < 2; i++)
        {
            worldFile.read(getString, 1);

            if (getString[0] != '\0')
            {
                worldFile.seekg(-1, ios::cur);

                for (register unsigned int j = 0; true; j++)
                {
                    worldFile.read(&getString[j], 1);

                    if (getString[j] == '\0')
                        break;
                }

                switch (i)
                {
                    case 0 :
                        FMOD_System_CreateStream(soundSystem, getString, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[22]);

                        wresourcesArray[1] = new string(getString);

                        break;

                    case 1 :
                        worldBackTex[1] = new Texture;
                        worldBackTex[1]->loadFromFile(getString);
                        worldBackTex[1]->setRepeated(true);

                        wresourcesArray[0] = new string(getString);

                        break;
                }
            }
        }

        // Reset the current directory :
        chdir(procPath);
#endif
    }

    worldFile.read(reinterpret_cast<char*>(&mapScale.x), 4);
    worldFile.read(reinterpret_cast<char*>(&mapScale.y), 4);

    worldBack->setSize(Vector2f(mapScale.x * 32, mapScale.y * 32));
    worldBack->setTextureRect(IntRect(0, 0, mapScale.x * 32, mapScale.y * 32));

    matrixMarker = new Matrix((mapScale.x * 2) + 1, (mapScale.y * 2) + 1);

    worldFile.read(reinterpret_cast<char*>(&worldBackground), 1);
    worldFile.read(reinterpret_cast<char*>(&worldMusic), 1);
    worldFile.read(reinterpret_cast<char*>(&worldLives), 1);

    worldBack->setTexture(worldBackTex[worldBackground]);

    {
        unsigned short listSize;

        worldFile.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TextureData* currentData;

                unsigned int id;

                Vector2f pos;
                Vector2f size;

                worldFile.read(reinterpret_cast<char*>(&id), 4);
                worldFile.read(reinterpret_cast<char*>(&pos.x), 4);
                worldFile.read(reinterpret_cast<char*>(&pos.y), 4);
                worldFile.read(reinterpret_cast<char*>(&size.x), 4);
                worldFile.read(reinterpret_cast<char*>(&size.y), 4);

                RectangleShape tex(size);

                tex.setPosition(pos);
                tex.setTexture(worldTextures[id-1]);
                tex.setTextureRect(IntRect(0, 0, size.x, size.y));

                currentData = new TextureData {tex, id};

                listTextures.emplace_back(currentData);
            }
        }
    }

    {
        unsigned short listSize;

        worldFile.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                EyecandyData* currentData;

                unsigned int id;

                Vector2f pos;

                worldFile.read(reinterpret_cast<char*>(&id), 4);
                worldFile.read(reinterpret_cast<char*>(&pos.x), 4);
                worldFile.read(reinterpret_cast<char*>(&pos.y), 4);

                Sprite spr(*worldEyecandies[id]);

                spr.setPosition(pos);

                currentData = new EyecandyData {spr, id};

                listEyecandies.emplace_back(currentData);
            }
        }
    }

    {
        unsigned short listSize;

        worldFile.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                LightData currentData;
                Light_Entity* light;

                worldFile.read(reinterpret_cast<char*>(&currentData), sizeof(LightData));

                light = new Light_Entity(entTex[0], worldMoveable);
                light->load(currentData);

                listLights.emplace_back(light);
            }
        }
    }

    {
        unsigned short listSize;

        worldFile.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                MarkerData currentData;
                Marker_Entity* marker;
                Vector2f pos;

                worldFile.read(reinterpret_cast<char*>(&currentData), sizeof(MarkerData));

                pos = Vector2f(currentData.x + 12, currentData.y + 10);

                marker = new Marker_Entity(entTex[1]);
                marker->load(currentData);
                marker->setNumber(currentMarker_Number);

                listMarkers.emplace_back(marker);

                worldMarkerLink.emplace_back(Vertex(pos, Color(128, 255, 255)));

                matrixMarker->setValue(pos.x / 16, pos.y / 16, 1);

                currentMarker_Number++;
            }
        }
    }

    selectedEntity = NULL;
    currentEditing = NONE;

    worldFile.close();
}

static void mergeTextures()
{
    list<TextureData*>::iterator it_a;
    list<TextureData*>::iterator it_b;

    vector<list<TextureData*>::iterator> listRemove;

    unsigned int parsed;

    Vector2f pos_i, size_i, pos_s, size_s;

    bool reparse = true;

    while (reparse)
    {
        reparse = false;

        for (it_a = listTextures.begin(); it_a != listTextures.end(); it_a++)
        {
            parsed = (*it_a)->id;

            pos_i = (*it_a)->rectangle.getPosition();
            size_i = (*it_a)->rectangle.getSize();

            for (it_b = listTextures.begin(); it_b != listTextures.end(); it_b++)
            {
                #ifdef DEBUGMODE
                cout << "Parser: First : " << (*it_a) << endl;
                cout << "Parser: Secnd : " << (*it_b) << endl;
                #endif

                if ((*it_b) == (*it_a))
                    continue;

                pos_s = (*it_b)->rectangle.getPosition();
                size_s = (*it_b)->rectangle.getSize();

                if (parsed == (*it_b)->id)
                {
                    if (pos_i.x == pos_s.x && pos_s.y == (pos_i.y + size_i.y) && size_i.x == size_s.x)
                    {
                        reparse = true;

                        listRemove.emplace_back(it_b);

                        #ifdef DEBUGMODE
                        cout << "Parser: Remove one block." << endl;
                        #endif

                        (*it_a)->rectangle.setSize(Vector2f(size_s.x, size_i.y + size_s.y));
                        (*it_a)->rectangle.setTextureRect(IntRect(0, 0, size_s.x, size_i.y + size_s.y));
                        (*it_a)->rectangle.setPosition(pos_s.x, min(pos_i.y, pos_s.y));
                    }
                    else if (pos_i.y == pos_s.y && pos_s.x == (pos_i.x + size_i.x) && size_i.y == size_s.y)
                    {
                        reparse = true;

                        listRemove.emplace_back(it_b);

                        #ifdef DEBUGMODE
                        cout << "Parser: Remove one block." << endl;
                        #endif

                        (*it_a)->rectangle.setSize(Vector2f(size_i.x + size_s.x, size_s.y));
                        (*it_a)->rectangle.setTextureRect(IntRect(0, 0, size_i.x + size_s.x, size_s.y));
                        (*it_a)->rectangle.setPosition(min(pos_i.x, pos_s.x), pos_s.y);
                    }
                }
            }

            if (reparse)
                break;
        }

        #ifdef DEBUGMODE
        cout << "Parser: Remover stack : " << listRemove.size() << endl;
        #endif

        for (vector<list<TextureData*>::iterator>::iterator deleter = listRemove.begin(); deleter != listRemove.end(); deleter++)
            listTextures.erase(*deleter);

        listRemove.clear();

        #ifdef DEBUGMODE
        cout << "Parser: Did one loop" << endl;
        #endif
    }
}

static void checkResources(const char* worldURL, bool saveAs)
{
    char resourcePath[MAX_PATH];
    char resourceName[MAX_PATH];

    char worldPath[MAX_PATH];
#ifndef LINUX
    char worldName[MAX_PATH];
#else
    char worldNameTemp[MAX_PATH];
    char* worldName;
#endif
    char destName[MAX_PATH];

    for (register unsigned int i = 0; i < 2; i++)
    {
        if (wresourcesArray[i] != NULL)
        {
#ifndef LINUX
            char messageText[256];

            strcpy(resourcePath, wresourcesArray[i]->c_str());
            strcpy(resourceName, wresourcesArray[i]->c_str());

            strcpy(worldPath, worldURL);
            strcpy(worldName, worldURL);

            PathRemoveFileSpec(resourcePath);
            PathRemoveFileSpec(worldPath);

            strcpy(destName, worldPath);

            PathStripPath(resourceName);
            PathStripPath(worldName);

            if (PathIsRelative(wresourcesArray[i]->c_str()))
#else
            strcpy(resourcePath, wresourcesArray[i]->substr(0, wresourcesArray[i]->find_last_of('/')).c_str());
            strcpy(resourceName, wresourcesArray[i]->substr(wresourcesArray[i]->find_last_of('/') + 1).c_str());

            strcpy(worldPath, worldURL);
            strcpy(worldNameTemp, worldURL);

            dirname(worldPath);
            worldName = basename(worldNameTemp);

            strcpy(destName, worldPath);

            if (wresourcesArray[i]->c_str()[0] != '/')
#endif
            {
                if (saveAs)
                {
                    extern bool showCursor;
                    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
                    sprintf(messageText, "The resource :\n%s\nis referred by a Relative Path from the previous world's location.\nDo you want update the Relative Path ?", resourceName);

                    if (MessageBox(NULL, messageText, "Update the Path ?", MB_TASKMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES)
                    {
                        if (PathRelativePathTo(resourcePath, worldPath, FILE_ATTRIBUTE_DIRECTORY, wresourcesArray[i]->c_str(), FILE_ATTRIBUTE_NORMAL))
#else
                    QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Update the Path ?"), QString("The resource :\n%1\nis referred by a Relative Path from the previous world's location.\nDo you want update the Relative Path ?").arg(resourceName), QMessageBox::Yes | QMessageBox::No);
                    if (messageBox.exec() == QMessageBox::Yes)
                    {
                        QDir dirPath(worldPath);
                        QString relativePath(dirPath.relativeFilePath(wresourcesArray[i]->c_str()));

                        if (relativePath.isEmpty())
#endif
                        {
                            delete wresourcesArray[i];
#ifndef LINUX
                            wresourcesArray[i] = new string(resourcePath);
#else
                            wresourcesArray[i] = new string(relativePath.toStdString());
#endif
                        }
                        else
                        {
#ifndef LINUX
                            MessageBox(NULL, "Failed to recreate Relative Path !", "Can't create Relative Path !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
                            QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Can't create Relative Path !"), QStringLiteral("Failed to recreate Relative Path !"), QMessageBox::Ok);
                            messageBox.exec();
#endif
                        }
                    }
                    mainWindow->setMouseCursorVisible(showCursor);
                }
            }
            else
            {
                if (strcmp(worldPath, resourcePath) != 0)
                {
                    extern bool showCursor;
                    mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
                    sprintf(messageText, "The resource :\n%s\nis in another directory from the world.\nDo you want copy the resource into the world directory ?", resourceName);
                    if (MessageBox(NULL, messageText, "Copy the resource file ?", MB_TASKMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES)
#else
                    QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Copy the resource file ?"), QString("The resource :\n%1\nis in another directory from the world.\nDo you want copy the resource into the world directory ?").arg(resourceName), QMessageBox::Yes | QMessageBox::No);
                    if (messageBox.exec() == QMessageBox::Yes)
#endif
                    {
                        ifstream src;
                        ofstream dest;

                        src.open(*wresourcesArray[i], ios::binary);

                        if (!src.good())
                        {
#ifndef LINUX
                            MessageBox(NULL, "Failed to open the original resource file !", "Error !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                            QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Error !"), QStringLiteral("Failed to open the original resource file !"), QMessageBox::Ok);
                            messageBox.exec();
#endif
                            goto LBL_CHKRESWORLD;
                        }
#ifndef LINUX
                        PathAppend(destName, resourceName);
#else
                        strcat(destName, "/");
                        strcat(destName, resourceName);
#endif
                        dest.open(destName, ios::binary);

                        if (!dest.good())
                        {
#ifndef LINUX
                            MessageBox(NULL, "Failed to copy the resource !", "Error !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                            QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Error !"), QStringLiteral("Failed to copy the resource !"), QMessageBox::Ok);
                            messageBox.exec();
#endif
                            src.close();
                            goto LBL_CHKRESWORLD;
                        }

                        dest << src.rdbuf();

                        src.close();
                        dest.close();
                    }
                    else
                    {
#ifndef LINUX
                        PathAppend(destName, wresourcesArray[i]->c_str());
#else
                        strcat(destName, "/");
                        strcat(destName, wresourcesArray[i]->c_str());
#endif
                    }
                    mainWindow->setMouseCursorVisible(showCursor);

                    LBL_CHKRESWORLD :

#ifndef LINUX
                    if (PathRelativePathTo(resourcePath, worldPath, FILE_ATTRIBUTE_DIRECTORY, destName, FILE_ATTRIBUTE_NORMAL))
#else
                    QDir dirPath(worldPath);
                    QString relativePath(dirPath.relativeFilePath(destName));

                    if (relativePath.isEmpty())
#endif
                    {
                        delete wresourcesArray[i];
#ifndef LINUX
                        wresourcesArray[i] = new string(resourcePath);
#else
                        wresourcesArray[i] = new string(relativePath.toStdString());
#endif
                    }
                    else
                    {
#ifndef LINUX
                        MessageBox(NULL, "Failed to create Relative Path !\nYou must copy the file manually then re-import it !", "Can't create Relative Path !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
                        QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Can't create Relative Path !"), QStringLiteral("Failed to create Relative Path !\nYou must copy the file manually then re-import it !"), QMessageBox::Ok);
                        messageBox.exec();
#endif
                        delete wresourcesArray[i];
                        wresourcesArray[i] = new string(destName);
                    }
                }
            }
        }
    }
}

bool checkwLoadResources(ifstream& worldFile, const char* filename)
{
    extern char procPath[MAX_PATH];
    char filePath[MAX_PATH];
    char getString[MAX_PATH];
    char CMLid[4];
    ifstream checkFile;

    strcpy(filePath, filename);
#ifndef LINUX
    char messageText[512];

    PathRemoveFileSpec(filePath);
    SetCurrentDirectory(filePath);
#else
    dirname(filePath);
    chdir(filePath);
#endif
    worldFile.read(CMLid, 4);

    if (CMLid[0] != 'C' || CMLid[1] != 'M' || CMLid[2] != 'W')
    {
#ifndef LINUX
        MessageBox(NULL, "This file is not a valid CMW World !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This file is not a valid CMW World !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    if (CMLid[3] != EDITOR_VERSION)
    {
#ifndef LINUX
        MessageBox(NULL, "This World was made with an Higher Version of Mario Constructor Master Editor !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
#else
        QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Error !"), QStringLiteral("This World was made with an Higher Version of Mario Constructor Master Editor !"), QMessageBox::Ok);
        messageBox.exec();
#endif
        return false;
    }

    for (register unsigned int i = 0; i < 2; i++)
    {
        worldFile.read(getString, 1);

        if (getString[0] != '\0')
        {
            worldFile.seekg(-1, ios::cur);

            for (register unsigned int j = 0; true; j++)
            {
                worldFile.read(&getString[j], 1);

                if (getString[j] == '\0')
                    break;
            }

            checkFile.open(getString);

            if (!checkFile.good())
            {
#ifndef LINUX
                sprintf(messageText, "The resource is not found :\n%s !", getString);
                MessageBox(NULL, messageText, "Resource not found !", MB_TASKMODAL | MB_ICONERROR | MB_OK);
#else
                QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Resource not found !"), QString("The resource is not found :\n%1 !").arg(getString), QMessageBox::Ok);
                messageBox.exec();
#endif
                return false;
            }
            else
                checkFile.close();
        }
    }

    // Reset the current directory :
#ifndef LINUX
    SetCurrentDirectory(procPath);
#else
    chdir(procPath);
#endif
    return true;
}

static void addNewResource(string filename, unsigned short id)
{
    char filenamePath[MAX_PATH];
    char dirPath[MAX_PATH];

    if (wresourcesArray[id] != NULL)
    {
        delete wresourcesArray[id];
        wresourcesArray[id] = NULL;
    }

    strcpy(filenamePath, filename.c_str());
    strcpy(dirPath, worldDir.c_str());

#ifndef LINUX
    PathRemoveFileSpec(dirPath);
    PathRemoveFileSpec(filenamePath);
#else
    dirname(dirPath);
    dirname(filenamePath);
#endif

    if (!worldDir.empty())
    {
        if (filenamePath != dirPath)
        {
            extern bool showCursor;
            mainWindow->setMouseCursorVisible(true);
#ifndef LINUX
            char messageText[256];

            sprintf(messageText, "The resource :\n%s\nis in another directory from the world.\nDo you want copy the resource into the world directory ?", filename.c_str());
            if (MessageBox(NULL, messageText.c_str(), "Copy the resource file ?", MB_TASKMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES)
#else
            QMessageBox messageBox(QMessageBox::Question, QStringLiteral("Copy the resource file ?"), QString("The resource :\n%1\nis in another directory from the world.\nDo you want copy the resource into the world directory ?").arg(filename.c_str()), QMessageBox::Yes | QMessageBox::No);
            if (messageBox.exec() == QMessageBox::Yes)
#endif
            {
                char bufferData[MAX_PATH];

                ifstream src;
                ofstream dest;

                src.open(filename, ios::binary);

                if (!src.good())
                    goto LBL_ADDRESWORLD;

                sprintf(bufferData, "%s\\%s", dirPath, filenamePath);

                dest.open(bufferData, ios::binary);

                if (!dest.good())
                {
                    src.close();
                    goto LBL_ADDRESWORLD;
                }

                dest << src.rdbuf();

                src.close();
                dest.close();

                filename = bufferData;
            }
            mainWindow->setMouseCursorVisible(showCursor);
        }

        LBL_ADDRESWORLD :

        {
#ifndef LINUX
            TCHAR relativePath[MAX_PATH] = "";

            if (PathRelativePathTo(relativePath, dirPath, FILE_ATTRIBUTE_DIRECTORY, filename.c_str(), FILE_ATTRIBUTE_NORMAL))
                wresourcesArray[id] = new string(relativePath);
#else
            QDir dirPth(dirPath);
            QString relativePath(dirPth.relativeFilePath(filename.c_str()));

            if (relativePath.isEmpty())
                wresourcesArray[id] = new string(relativePath.toStdString());
#endif
            else
            {
#ifndef LINUX
                MessageBox(NULL, "Failed to create Relative Path !\nYou must copy the file manually then re-import it !", "Can't create Relative Path !", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
                QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("Can't create Relative Path !"), QStringLiteral("Failed to create Relative Path !\nYou must copy the file manually then re-import it !"), QMessageBox::Ok);
                messageBox.exec();
#endif
                wresourcesArray[id] = new string(filename);
            }
        }
    }
    else
        wresourcesArray[id] = new string(filename);

    #ifdef DEBUGMODE
    cout << *wresourcesArray[id] << endl;
    #endif
}

static void removeResource(unsigned short id)
{
    if (wresourcesArray[id] != NULL)
    {
        delete wresourcesArray[id];
        wresourcesArray[id] = NULL;
    }

    #ifdef DEBUGMODE
    cout << wresourcesArray[id] << endl;
    #endif
}
