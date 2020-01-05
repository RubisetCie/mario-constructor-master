/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

extern "C"
{
    #include <shlwapi.h>
    #include <FMODEx/fmod.h>
}

#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <cfloat>

#ifdef DEBUGMODE
#include <iostream>

#define LEVEL_TO_LOAD "../Release/Levels/Centipede Railway.cml"
#endif

#include "../../Headers/Core/placeable.hpp"
#include "../../Headers/Core/matrix.hpp"
#include "../../Headers/Core/collider.hpp"
#include "../../Headers/Graphics/spritefont.hpp"
#ifdef PUBLISHER
#include "../../Headers/Core/gscene.hpp"
#include "../../Headers/Graphics/gtile.hpp"
#include "../../Headers/Graphics/gtrigger.hpp"
#include "../../Headers/Graphics/glightentity.hpp"
#include "../../Headers/Graphics/gmarkerentity.hpp"
#else
#include "../../Headers/Core/scene.hpp"
#include "../../Headers/Graphics/tile.hpp"
#include "../../Headers/Graphics/trigger.hpp"
#include "../../Headers/Graphics/lightentity.hpp"
#include "../../Headers/Graphics/markerentity.hpp"
#endif // PUBLISHER

#include "../../Headers/Bonuses/bonus_mushroomblock.hpp"
#include "../../Headers/Bonuses/bonus_flowerblock.hpp"
#include "../../Headers/Bonuses/bonus_beetblock.hpp"
#include "../../Headers/Bonuses/bonus_luiblock.hpp"
#include "../../Headers/Bonuses/bonus_starblock.hpp"
#include "../../Headers/Bonuses/bonus_lifeblock.hpp"
#include "../../Headers/Bonuses/bonus_poisonblock.hpp"
#include "../../Headers/Bonuses/bonus_mushroomblockinv.hpp"
#include "../../Headers/Bonuses/bonus_flowerblockinv.hpp"
#include "../../Headers/Bonuses/bonus_beetblockinv.hpp"
#include "../../Headers/Bonuses/bonus_luiblockinv.hpp"
#include "../../Headers/Bonuses/bonus_starblockinv.hpp"
#include "../../Headers/Bonuses/bonus_lifeblockinv.hpp"
#include "../../Headers/Bonuses/bonus_poisonblockinv.hpp"
#include "../../Headers/Bonuses/bonus_coin.hpp"
#include "../../Headers/Bonuses/bonus_coinblock.hpp"
#include "../../Headers/Bonuses/bonus_coinblockinv.hpp"
#include "../../Headers/Bonuses/bonus_brick.hpp"
#include "../../Headers/Bonuses/bonus_coinbrick.hpp"
#include "../../Headers/Bonuses/bonus_switch.hpp"
#include "../../Headers/Bonuses/bonus_toggle.hpp"

#include "../../Headers/Sceneries/scenery_static.hpp"
#include "../../Headers/Sceneries/scenery_bush.hpp"
#include "../../Headers/Sceneries/scenery_cloud.hpp"
#include "../../Headers/Sceneries/scenery_scroll.hpp"
#include "../../Headers/Sceneries/scenery_waterfall.hpp"

#include "../../Headers/Marks/mark_waterplane.hpp"
#include "../../Headers/Marks/mark_lavaplane.hpp"
#include "../../Headers/Marks/mark_checkpoint.hpp"
#include "../../Headers/Marks/mark_levelend.hpp"
#include "../../Headers/Marks/mark_platformredmoveh.hpp"
#include "../../Headers/Marks/mark_platformredtouchh.hpp"
#include "../../Headers/Marks/mark_platformredmovev.hpp"
#include "../../Headers/Marks/mark_platformredtouchv.hpp"
#include "../../Headers/Marks/mark_platformredfall.hpp"
#include "../../Headers/Marks/mark_platformshortmoveh.hpp"
#include "../../Headers/Marks/mark_platformshorttouchh.hpp"
#include "../../Headers/Marks/mark_platformshortmovev.hpp"
#include "../../Headers/Marks/mark_platformshorttouchv.hpp"
#include "../../Headers/Marks/mark_platformshortfall.hpp"
#include "../../Headers/Marks/mark_platformcloudmoveh.hpp"
#include "../../Headers/Marks/mark_platformcloudtouchh.hpp"
#include "../../Headers/Marks/mark_platformcloudmovev.hpp"
#include "../../Headers/Marks/mark_platformcloudtouchv.hpp"
#include "../../Headers/Marks/mark_platformcloudfall.hpp"
#include "../../Headers/Marks/mark_platformstonemoveh.hpp"
#include "../../Headers/Marks/mark_platformstonetouchh.hpp"
#include "../../Headers/Marks/mark_platformstonemovev.hpp"
#include "../../Headers/Marks/mark_platformstonetouchv.hpp"
#include "../../Headers/Marks/mark_platformstonefall.hpp"
#include "../../Headers/Marks/mark_spring.hpp"

#include "../../Headers/Sprites/sprite_mushroom.hpp"
#include "../../Headers/Sprites/sprite_flower.hpp"
#include "../../Headers/Sprites/sprite_beet.hpp"
#include "../../Headers/Sprites/sprite_lui.hpp"
#include "../../Headers/Sprites/sprite_star.hpp"
#include "../../Headers/Sprites/sprite_life.hpp"
#include "../../Headers/Sprites/sprite_goomba.hpp"
#include "../../Headers/Sprites/sprite_goombagrey.hpp"
#include "../../Headers/Sprites/sprite_spiny.hpp"
#include "../../Headers/Sprites/sprite_spinygrey.hpp"
#include "../../Headers/Sprites/sprite_buzzybeetle.hpp"
#include "../../Headers/Sprites/sprite_koopagreen.hpp"
#include "../../Headers/Sprites/sprite_koopared.hpp"
#include "../../Headers/Sprites/sprite_koopablue.hpp"
#include "../../Headers/Sprites/sprite_koopayellow.hpp"
#include "../../Headers/Sprites/sprite_paratroopagreenh.hpp"
#include "../../Headers/Sprites/sprite_paratrooparedh.hpp"
#include "../../Headers/Sprites/sprite_paratroopablueh.hpp"
#include "../../Headers/Sprites/sprite_paratroopagreenv.hpp"
#include "../../Headers/Sprites/sprite_paratrooparedv.hpp"
#include "../../Headers/Sprites/sprite_paratroopabluev.hpp"
#include "../../Headers/Sprites/sprite_paratroopagreenj.hpp"
#include "../../Headers/Sprites/sprite_paratrooparedj.hpp"
#include "../../Headers/Sprites/sprite_paratroopabluej.hpp"
#include "../../Headers/Sprites/sprite_paratroopayellow.hpp"
#include "../../Headers/Sprites/sprite_plantgreent.hpp"
#include "../../Headers/Sprites/sprite_plantgreenl.hpp"
#include "../../Headers/Sprites/sprite_plantgreenr.hpp"
#include "../../Headers/Sprites/sprite_plantgreenb.hpp"
#include "../../Headers/Sprites/sprite_plantfiret.hpp"
#include "../../Headers/Sprites/sprite_plantfirel.hpp"
#include "../../Headers/Sprites/sprite_plantfirer.hpp"
#include "../../Headers/Sprites/sprite_plantfireb.hpp"
#include "../../Headers/Sprites/sprite_plantgreyt.hpp"
#include "../../Headers/Sprites/sprite_plantgreyl.hpp"
#include "../../Headers/Sprites/sprite_plantgreyr.hpp"
#include "../../Headers/Sprites/sprite_plantgreyb.hpp"
#include "../../Headers/Sprites/sprite_cheepcheepred.hpp"
#include "../../Headers/Sprites/sprite_cheepcheepgreen.hpp"
#include "../../Headers/Sprites/sprite_cheepcheepblue.hpp"
#include "../../Headers/Sprites/sprite_cheepcheepyellow.hpp"
#include "../../Headers/Sprites/sprite_hammerbro.hpp"
#include "../../Headers/Sprites/sprite_silverbro.hpp"
#include "../../Headers/Sprites/sprite_firebro.hpp"
#include "../../Headers/Sprites/sprite_beetbro.hpp"
#include "../../Headers/Sprites/sprite_spikebro.hpp"
#include "../../Headers/Sprites/sprite_lakituwhite.hpp"
#include "../../Headers/Sprites/sprite_lakitudark.hpp"
#include "../../Headers/Sprites/sprite_lakitugold.hpp"
#include "../../Headers/Sprites/sprite_poison.hpp"
#include "../../Headers/Sprites/sprite_thwompb.hpp"
#include "../../Headers/Sprites/sprite_thwompt.hpp"
#include "../../Headers/Sprites/sprite_bowser.hpp"
#include "../../Headers/Sprites/sprite_staticplant.hpp"

#include "../../Headers/Hazards/hazard_lavat.hpp"
#include "../../Headers/Hazards/hazard_laval.hpp"
#include "../../Headers/Hazards/hazard_lavar.hpp"
#include "../../Headers/Hazards/hazard_lavab.hpp"
#include "../../Headers/Hazards/hazard_podoboot.hpp"
#include "../../Headers/Hazards/hazard_podoboob.hpp"
#include "../../Headers/Hazards/hazard_podobool.hpp"
#include "../../Headers/Hazards/hazard_podoboor.hpp"
#include "../../Headers/Hazards/hazard_podoboofountaint.hpp"
#include "../../Headers/Hazards/hazard_podoboofountainb.hpp"
#include "../../Headers/Hazards/hazard_podoboofountainl.hpp"
#include "../../Headers/Hazards/hazard_podoboofountainr.hpp"
#include "../../Headers/Hazards/hazard_spike.hpp"
#include "../../Headers/Hazards/hazard_cannonht.hpp"
#include "../../Headers/Hazards/hazard_cannonhb.hpp"
#include "../../Headers/Hazards/hazard_cannonvl.hpp"
#include "../../Headers/Hazards/hazard_cannonvr.hpp"
#include "../../Headers/Hazards/hazard_cannonhominght.hpp"
#include "../../Headers/Hazards/hazard_cannonhominghb.hpp"
#include "../../Headers/Hazards/hazard_cannonhomingvl.hpp"
#include "../../Headers/Hazards/hazard_cannonhomingvr.hpp"
#include "../../Headers/Hazards/hazard_rotodiscc.hpp"
#include "../../Headers/Hazards/hazard_rotodisccc.hpp"
#include "../../Headers/Hazards/hazard_rotodiscrc.hpp"
#include "../../Headers/Hazards/hazard_rotodiscrcc.hpp"
#include "../../Headers/Hazards/hazard_lavalauncherl.hpp"
#include "../../Headers/Hazards/hazard_lavalauncherr.hpp"
#include "../../Headers/Hazards/hazard_lavalaunchervl.hpp"
#include "../../Headers/Hazards/hazard_lavalaunchervr.hpp"
#include "../../Headers/Hazards/hazard_spikelaunchert.hpp"
#include "../../Headers/Hazards/hazard_spikelauncherb.hpp"
#include "../../Headers/Hazards/hazard_centipede.hpp"

#ifdef PUBLISHER
#include "../../Headers/gglobals.hpp"
#else
#include "../../Headers/globals.hpp"
#endif // PUBLISHER

#define WORLDMAP_CAMERASPEED 3
#define WORLDMAP_MARIOSPEED 0.7
#define WORLDMAP_MARIOSPEEDACC 4
#define WORLDMAP_PROGRESSINTERVAL 23
#define WORLDMAP_PROGRESSINTERVALACC 3

#define FADESPEED 1.5

using namespace sf;
using namespace std;

typedef struct Lamp
{
    Color color;

    float x;
    float y;

    float radius;

    bool litTextures;
    bool litEyecandies;
} Lamp;

Color levelBackColor;
Color levelBackColorb;

Vector2i mpScale;

Vector2i levelScale;
Vector2i levelScaleb;

Vector2f cameraPos;
Vector2f mapPos;

Vector2f worldNamePos;

Mark_Mario* player;

vector<Vertex> tilemapLayer1;
vector<Vertex> tilemapLayer2;
vector<Vertex> tilemapLayer3;

vector<Vertex> tilemapLayer1b;
vector<Vertex> tilemapLayer2b;
vector<Vertex> tilemapLayer3b;

list<Collider*> listCollider;
list<Collider*> listColliderb;

list<Placeable*> listPlaceables;
list<Placeable*> listPlaceablesb;

list<WarpsData*> listPassages;

list<Effect*> listEffect;

list<Placeable*>::iterator spriteIterator;
list<Placeable*>::iterator spriteIteratorb;

list<RectangleShape*> listTex;
list<Sprite*> listEye;
list<Lamp*> listLamps;
list<MarkerData*> listMark;
list<MarkerData*>::iterator levelsPassed;
unsigned char levelsFinished;
bool marksDelta_phase;
float marksDelta;

FMOD_SOUND* musicSamples[30];
FMOD_SOUND* sfxSamples[35];

FMOD_SOUND** musicZonea;
FMOD_SOUND** musicZoneb;
FMOD_SOUND** musicWorld;

unsigned char autoscroll;
unsigned char autoscroll_node;
unsigned char autoscroll_nodesaved;
float autoscrollSpeed;
float titlePos;
float titleSpd;

int timer;

vector<Vector2f> autoscrollPath;

float liquidStartHeight;
float liquidStartHeightb;
float liquidTargetHeight;
float liquidTargetHeightb;
float liquidSpeed;

float timerAlert;

vector<TriggerData*> liquidSpots;
vector<TriggerData*> liquidSpotsb;

ifstream level_file;
ifstream scenario_file;

bool koopaAttacks[6];
unsigned short koopaFrequences[6];
float koopaSpeeds[6];
unsigned char koopaTrail;
unsigned char koopaHealth;
unsigned char koopaHealth_saved;
unsigned char koopaMusic;
unsigned char koopaEngaged;

Texture* circleFadeTex;
Sprite* circleSpr;

bool fadeCircle;
bool exitToTitle;
bool nextLevel;
bool timerAlertPhase;
bool restartLevel;
bool zoneb;
bool useShaders;
char map_completed;

bool clearColora;
bool clearColorb;
bool clearColorw;

View* levelCamera;
View* mapCamera;
float levelCamera_shake;

RenderTexture* renderPasses[6];
Sprite* renderPassSpr[2];

Shader* glowShaderH;
Shader* glowShaderV;

Vertex gradientRectangle[4];
Vertex gradientRectangleb[4];

Texture* backgroundTxt[16];
Texture* sceneriesTxt[14];
Texture* effectTxt[16];
Texture* itemsTxt[11];
Texture* enemiesTxt[42];
Texture* hazardsTxt[11];
Texture* marksTxt[12];
Texture* marioTxt[6];

Texture* worldLightTex;
Texture* worldBackTex[2];
Texture* entTex[7];
Texture* worldTextures[166];
Texture* worldEyecandies[81];

Texture* hudTxt[7];

Texture* mainTileset;

Sprite* hudSpr[4];
Sprite* markSpr;
Sprite* sheetSpr;
Sprite* titleSpr;
Sprite* startSpr;
Sprite* completedSpr;
Sprite* marioSpr;
Sprite* worldLightSpr[5];
Vector2f marioSpr_speed;
VertexArray* mapTrace;
unsigned char mapTrace_timer;
unsigned char mapTrace_delimiter;
unsigned char marioSpr_validation;
float marioSpr_velocity;
VertexArray* hudBowser;
float hudBowser_descend;

SpriteFont* scoreFont;
SpriteFont* livesFont;
SpriteFont* coinsFont;
SpriteFont* worldFont;
SpriteFont* timerFont;

RectangleShape* blackBorder;

RectangleShape* mapBack;

RectangleShape* backgroundSpr;
RectangleShape* backgroundSprb;

RectangleShape* effectCloudsSpr[2];
RectangleShape* effectCloudsSprb[2];

RectangleShape* effectWeatherSpr;
RectangleShape* effectWeatherSprb;
Sprite* effectLightningSpr;

FloatRect blockhitter;
FloatRect spritehitter;
FloatRect enemystomper;
FloatRect enemykiller;
FloatRect blockkiller;
Collider* enemystomper_pt;
char enemykiller_fire;
bool enemykiller_create;
bool blockkiller_create;
bool spritehitter_create;

Matrix* collisionMatrix;
Matrix* collisionMatrixb;

Mark_LiquidPlane* watera;
Mark_LiquidPlane* waterb;

extern Uint8 fadeAlpha;
extern RectangleShape* fadeRect;
#ifdef PUBLISHER
extern unsigned char target_lives;
#endif

unsigned int effectCloudPos[2];
unsigned int effectLavafallPos;
unsigned int effectWeatherPos;
unsigned short effectLightning;
char effectWeatherType;
char effectWeatherTypeb;

streampos spriteDefStart;
int levelTimer;

unsigned int frame_Hud;
unsigned int frame_Water;
unsigned int frame_Lava;
unsigned int frame_Flame;
unsigned int frame_Podoboo;
unsigned int frame_Clouds;
unsigned int frame_Flower;
unsigned int frame_Rotodisc;
unsigned int frame_Mario;
unsigned int frame_Progress;
unsigned int frame_Start;

int keybindings[6];

unsigned char killcount;
unsigned char game_powerup;
unsigned char game_coins;
unsigned char game_lives;
unsigned int game_score;

unsigned char game_toggleSolidity[4];
unsigned char game_toggleSoliditySave[4];

float mario_gravity;
float sprite_gravity;
float star_gravity;

static bool InitAssets();

static bool Level_Load();
static void Level_Reload();
static void Level_Clear();
static bool World_Load();

bool Scene::Ingame()
{
    bool exitLoop = false;

    if (!InitAssets())
    {
        MessageBox(NULL, "Failed to initialize the assets on the Game !", "Assets Error !", MB_OK | MB_ICONERROR | MB_TASKMODAL);

        mainWindow->close();

        exitLoop = true;
    }

    Event eventSystem;

    if (loadingType == 4)   // Worldmap
    {
        while (!exitLoop)
        {
            LBL_TOWORLD :

            while (mainWindow->pollEvent(eventSystem))
            {
                switch (eventSystem.type)
                {
                    case Event::Closed     :
                        exitLoop = true;

                        mainWindow->close();

                        break;

                    case Event::KeyPressed :

                        if (exitToTitle)
                            break;

                        switch (eventSystem.key.code)
                        {
                            case Keyboard::Escape :

                                if (MessageBox(NULL, "Do you want to back to the Title Screen ?", "Back to the Title ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
                                    exitToTitle = true;

                                break;
                            default :
                                if (map_completed == 1)
                                {
                                    fadeRect->setPosition(mapPos.x - 320, mapPos.y - 240);
                                    map_completed = 2;
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

            bool focus = mainWindow->hasFocus();

            {
                if (marioSpr_speed != Vector2f(0, 0))
                    mapPos = marioSpr->getPosition() + Vector2f(8, 14);
                else
                {
                    if (map_completed == 0)
                    {
                        if (marioSpr_validation == 0)
                        {
                            bool keyLeft = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[0])) && focus;
                            bool keyRight = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[1])) && focus;
                            bool keyDown = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[2])) && focus;
                            bool keyUp = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[3])) && focus;

                            if (keyLeft && !keyRight)
                                mapPos.x -= WORLDMAP_CAMERASPEED;
                            else if (keyRight && !keyLeft)
                                mapPos.x += WORLDMAP_CAMERASPEED;

                            if (keyUp && !keyDown)
                                mapPos.y -= WORLDMAP_CAMERASPEED;
                            else if (keyDown && !keyUp)
                                mapPos.y += WORLDMAP_CAMERASPEED;
                        }
                        else
                        {
                            Vector2f targetPos(marioSpr->getPosition() + Vector2f(8, 14));

                            if (mapPos.x > targetPos.x + WORLDMAP_CAMERASPEED * 6)
                                mapPos.x -= WORLDMAP_CAMERASPEED * 6;
                            else if (mapPos.x < targetPos.x - WORLDMAP_CAMERASPEED * 6)
                                mapPos.x += WORLDMAP_CAMERASPEED * 6;

                            if (mapPos.y > targetPos.y + WORLDMAP_CAMERASPEED * 6)
                                mapPos.y -= WORLDMAP_CAMERASPEED * 6;
                            else if (mapPos.y < targetPos.y - WORLDMAP_CAMERASPEED * 6)
                                mapPos.y += WORLDMAP_CAMERASPEED * 6;
                        }
                    }
                }

                if (mapPos.x < 320)
                    mapPos.x = 320;

                if (mapPos.y < 240)
                    mapPos.y = 240;

                if (mapPos.x > (mpScale.x * 32) - 320)
                    mapPos.x = (mpScale.x * 32) - 320;

                if (mapPos.y > (mpScale.y * 32) - 240)
                    mapPos.y = (mpScale.y * 32) - 240;

                Vector2f roundedCameraPos(roundf(mapPos.x), roundf(mapPos.y));

                sheetSpr->setPosition(roundedCameraPos.x + 213, roundedCameraPos.y + 163);
                startSpr->setPosition(roundedCameraPos.x - 149, roundedCameraPos.y + 202);

                if (map_completed < 2)
                {
                    titlePos += titleSpd;

                    if (titlePos < -224)
                        titleSpd += 0.2;
                    else
                    {
                        titlePos = -224;
                        titleSpd = -titleSpd + 2;

                        if (titleSpd > -1.5)
                            titleSpd = -1.5;
                    }

                    titleSpr->setPosition(roundedCameraPos.x - 140, roundedCameraPos.y + roundf(titlePos));
                }

                if (map_completed == 1)
                    completedSpr->setPosition(roundedCameraPos.x - 102, titleSpr->getPosition().y + 58);

                mapCamera->setCenter(roundedCameraPos);
                mainTexture.setView(*mapCamera);
            }

            if (clearColorw)
                mainTexture.clear(Color::White);

            mainTexture.draw(*mapBack);

            renderPasses[0]->clear(Color::Transparent);
            renderPasses[4]->clear(Color::Transparent);

            worldLightSpr[1]->setPosition(mapPos.x - 320, mapPos.y - 240);
            worldLightSpr[2]->setPosition(mapPos.x - 320, mapPos.y - 240);
            worldLightSpr[3]->setPosition(mapPos.x - 320, mapPos.y - 240);
            worldLightSpr[4]->setPosition(mapPos.x - 320, mapPos.y - 240);

            renderPasses[0]->setView(*mapCamera);
            renderPasses[2]->setView(*mapCamera);
            renderPasses[3]->setView(*mapCamera);
            renderPasses[4]->setView(*mapCamera);

            if (!listLamps.empty())
            {
                renderPasses[2]->clear(Color(96, 96, 96));
                renderPasses[3]->clear(Color(96, 96, 96));

                for (list<Lamp*>::iterator it = listLamps.begin(); it != listLamps.end(); it++)
                {
                    if (!(*it)->litTextures && !(*it)->litEyecandies)
                        continue;

                    Vector2f litPos((*it)->x, (*it)->y);
                    float litRad = (*it)->radius;

                    if ((litPos.x + litRad < mapPos.x - 320 && litPos.y + litRad < mapPos.y - 240 && litPos.x - litRad > mapPos.x + 320 && litPos.y - litRad > mapPos.y + 240))
                        continue;

                    worldLightSpr[0]->setPosition(Vector2f(litPos.x + 16, litPos.y + 16));

                    float lightradius = litRad / 48;

                    worldLightSpr[0]->setScale(lightradius, lightradius);
                    worldLightSpr[0]->setColor((*it)->color);

                    if ((*it)->litTextures)
                        renderPasses[2]->draw(*worldLightSpr[0], BlendAdd);

                    if ((*it)->litEyecandies)
                        renderPasses[3]->draw(*worldLightSpr[0], BlendAdd);
                }

                renderPasses[2]->display();
                renderPasses[3]->display();
            }
            else
            {
                renderPasses[2]->clear(Color::White);
                renderPasses[3]->clear(Color::White);
            }

            if (!listEye.empty())
            {
                for (list<Sprite*>::iterator it = listEye.begin(); it != listEye.end(); it++)
                {
                    FloatRect entRect((*it)->getGlobalBounds());

                    if ((entRect.left + entRect.width > mapPos.x - 336 && entRect.top + entRect.height > mapPos.y - 256 && entRect.left < mapPos.x + 336 && entRect.top < mapPos.y + 256))
                        renderPasses[4]->draw(**it);
                }
            }

            if (!listTex.empty())
            {
                for (list<RectangleShape*>::iterator it = listTex.begin(); it != listTex.end(); it++)
                {
                    Vector2f entPos((*it)->getPosition());
                    Vector2f entSize((*it)->getSize());

                    if ((entPos.x + entSize.x > mapPos.x - 336 && entPos.y + entSize.y > mapPos.y - 256 && entPos.x < mapPos.x + 336 && entPos.y < mapPos.y + 256))
                        renderPasses[0]->draw(**it);
                }
            }

            renderPasses[0]->display();
            renderPasses[4]->display();

            if (useShaders)
            {
                renderPasses[1]->clear(Color::Transparent);
                renderPasses[5]->clear(Color::Transparent);

                renderPasses[1]->setView(*mapCamera);
                renderPasses[5]->setView(*mapCamera);

                renderPassSpr[0]->setPosition(mapPos.x - 320, mapPos.y - 240);
                renderPassSpr[1]->setPosition(mapPos.x - 320, mapPos.y - 240);

                renderPasses[5]->draw(*worldLightSpr[2]);
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

            if (map_completed == 0)
            {
                if (mapTrace->getVertexCount() > 0)
                    mainTexture.draw(*mapTrace, entTex[1]);

                if (!listMark.empty())
                {
                    unsigned char i = 0;

                    if (marksDelta_phase)
                    {
                        marksDelta += 0.5;
                        if (marksDelta >= 4)
                            marksDelta_phase = false;
                    }
                    else
                    {
                        marksDelta -= 0.75;
                        if (marksDelta < 0)
                            marksDelta_phase = true;
                    }

                    for (list<MarkerData*>::iterator it = listMark.begin(); it != listMark.end(); it++)
                    {
                        if (!(*it)->isLevel)
                            continue;

                        Vector2f entPos((*it)->x, (*it)->y - marksDelta);

                        if ((entPos.x > mapPos.x - 339 && entPos.y > mapPos.y - 263 && entPos.x < mapPos.x + 320 && entPos.y < mapPos.y + 248) && levelsFinished > i)
                        {
                            markSpr->setPosition(entPos.x, entPos.y);

                            mainTexture.draw(*markSpr);
                        }

                        i++;
                    }

                    if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keybindings[4])) && focus && marioSpr_validation == 0)
                    {
                        if (marioSpr_speed == Vector2f(0, 0))
                            marioSpr_validation = 1;
                        else
                            marioSpr_validation = 51;
                    }

                    if (marioSpr_speed == Vector2f(0, 0))
                    {
                        if (frame_Mario % 10 == 0)
                            marioSpr->setTextureRect(IntRect((frame_Mario / 10) * 16, 0, 16, 28));
                    }
                    else
                    {
                        marioSpr->move(marioSpr_speed);

                        Vector2f ppos(marioSpr->getPosition() + Vector2f(8, 14));
                        Vector2f mpos((*levelsPassed)->x + 10, (*levelsPassed)->y + 6);

                        // Progress points :
                        if (mapTrace_timer >= mapTrace_delimiter)
                        {
                            mapTrace_timer = 0;

                            if (frame_Progress > 6 || frame_Progress == 0)
                            {
                                mapTrace->append(Vertex(Vector2f(ppos.x - 6, ppos.y - 6), Color::White, Vector2f(12, 0)));
                                mapTrace->append(Vertex(Vector2f(ppos.x + 6, ppos.y - 6), Color::White, Vector2f(24, 0)));
                                mapTrace->append(Vertex(Vector2f(ppos.x + 6, ppos.y + 6), Color::White, Vector2f(24, 12)));
                                mapTrace->append(Vertex(Vector2f(ppos.x - 6, ppos.y + 6), Color::White, Vector2f(12, 12)));
                            }
                            else
                            {
                                mapTrace->append(Vertex(Vector2f(ppos.x - 6, ppos.y - 6), Color::White, Vector2f(0, 0)));
                                mapTrace->append(Vertex(Vector2f(ppos.x + 6, ppos.y - 6), Color::White, Vector2f(12, 0)));
                                mapTrace->append(Vertex(Vector2f(ppos.x + 6, ppos.y + 6), Color::White, Vector2f(12, 12)));
                                mapTrace->append(Vertex(Vector2f(ppos.x - 6, ppos.y + 6), Color::White, Vector2f(0, 12)));
                            }
                        }
                        else
                            mapTrace_timer++;

                        float delta = atan2(marioSpr_speed.y, marioSpr_speed.x) - atan2(mpos.y - ppos.y, mpos.x - ppos.x);

                        if (delta > 1.570796 || delta < -1.570796)
                        {
                            ppos = mpos - Vector2f(8, 14);
                            bool level = (*levelsPassed)->isLevel;

                            if (level)
                                fileToLoad = (*levelsPassed)->levelURL;

                            levelsPassed++;

                            marioSpr->setPosition(ppos);

                            if (levelsPassed == listMark.end())
                            {
                                map_completed = 1;

                                titlePos = -360;
                                titleSpd = 0;

                                frame_Mario = 0;
                                frame_Start = 0;

                                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), musicSamples[29], 0, &musicChannel);
                            }
                            else
                            {
                                mpos = Vector2f((*levelsPassed)->x + 10, (*levelsPassed)->y + 6);

                                if (level)
                                {
                                    marioSpr->setTextureRect(IntRect(0, 0, 16, 28));
                                    marioSpr_speed = Vector2f(0, 0);
                                    marioSpr_validation = 0;

                                    mapTrace_delimiter = WORLDMAP_PROGRESSINTERVAL;
                                    mapTrace_timer = 0;

                                    frame_Mario = 0;
                                }
                                else
                                {
                                    float angle = atan2(ppos.y - mpos.y + 14, ppos.x - mpos.x + 8);

                                    marioSpr_speed = Vector2f(-marioSpr_velocity * cos(angle), -marioSpr_velocity * sin(angle));
                                }
                            }
                        }

                        if (frame_Mario % 3 == 0)
                            marioSpr->setTextureRect(IntRect((frame_Mario / 3) * 16, 0, 16, 28));
                    }
                }

                if (frame_Progress == 6)
                {
                    size_t size = mapTrace->getVertexCount();
                    for (register unsigned int i = 0; i < size; i++)
                        (*mapTrace)[i].texCoords.x += 12;
                }
                else if (frame_Progress == 0)
                {
                    size_t size = mapTrace->getVertexCount();
                    for (register unsigned int i = 0; i < size; i++)
                        (*mapTrace)[i].texCoords.x -= 12;
                }
            }

            if (marioSpr_validation > 0)
            {
                if (marioSpr_validation < 51)
                    marioSpr_validation++;

                // Level entering :
                if (marioSpr_validation == 50)
                {
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
                    circleSpr->setPosition(mapPos);
                    circleSpr->setScale(1.5, 1.5);
                    circleSpr->setOrigin(240, 240);

                    marioSpr_validation = 66;

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[28], 0, NULL);

                    FMOD_Channel_Stop(musicChannel);
                }
                else if (marioSpr_validation == 51)
                {
                    if (mapTrace_timer == 0)
                    {
                        marioSpr_speed.x *= WORLDMAP_MARIOSPEEDACC / WORLDMAP_MARIOSPEED;
                        marioSpr_speed.y *= WORLDMAP_MARIOSPEEDACC / WORLDMAP_MARIOSPEED;
                        marioSpr_velocity = WORLDMAP_MARIOSPEEDACC;

                        mapTrace_delimiter = WORLDMAP_PROGRESSINTERVALACC;

                        marioSpr_validation = 52;
                    }
                }
            }

            if (map_completed == 0 && !listMark.empty())
                mainTexture.draw(*marioSpr);

            mainTexture.draw(*sheetSpr);
            mainTexture.draw(*titleSpr);

            if (map_completed > 0)
            {
                if (map_completed == 1)
                {
                    if (frame_Mario % 3 == 0)
                        completedSpr->setTextureRect(IntRect(0, (frame_Mario / 3) * 31, 204, 31));
                }

                mainTexture.draw(*completedSpr);
            }

            if (map_completed == 0)
            {
                if (marioSpr_speed == Vector2f(0, 0))
                {
                    if (frame_Start > 8 && marioSpr_validation == 0)
                        mainTexture.draw(*startSpr);

                    if (frame_Mario < 29)
                        frame_Mario++;
                    else
                        frame_Mario = 0;

                    if (frame_Start < 27)
                        frame_Start++;
                    else
                        frame_Start = 0;
                }
                else
                {
                    if (frame_Mario < 8)
                        frame_Mario++;
                    else
                        frame_Mario = 0;
                }
            }
            else
            {
                if (frame_Start == 0)
                {
                    if (frame_Mario < 26)
                        frame_Mario++;
                    else
                        frame_Start = 1;
                }
                else
                {
                    if (frame_Mario > 0)
                        frame_Mario--;
                    else
                        frame_Start = 0;
                }
            }

            if (map_completed == 2)
            {
                if (fadeAlpha < 255 - FADESPEED)
                    fadeAlpha += FADESPEED;
                else
                {
                    fadeRect->setPosition(0, 0);
                    fadeRect->setFillColor(Color(0, 0, 0, 255));

                    mainTexture.setView(View(Vector2f(320, 240), Vector2f(640, 480)));

                    fadeAlpha = 255;

                    targetScene = SCENE_TITLE;
                    exitLoop = true;
                }
            }

            fadeRect->setFillColor(Color(0, 0, 0, fadeAlpha));

            if (fadeAlpha > 0)
                mainTexture.draw(*fadeRect);

            if (fadeCircle)
            {
                Vector2f circleSize = circleSpr->getScale();

                circleSpr->setPosition(mapPos);
                mainTexture.draw(*circleSpr, BlendMultiply);

                blackBorder->setPosition(mapPos.x-320, mapPos.y-240);
                blackBorder->setSize(Vector2f(320 - (240 * circleSize.x), 480));

                mainTexture.draw(*blackBorder);

                blackBorder->setPosition(mapPos.x + (240 * circleSize.x), mapPos.y-240);
                blackBorder->setSize(Vector2f(320 - (240 * circleSize.x), 480));

                mainTexture.draw(*blackBorder);

                blackBorder->setPosition(mapPos.x-320, mapPos.y-240);
                blackBorder->setSize(Vector2f(640, 240 - (240 * circleSize.y)));

                mainTexture.draw(*blackBorder);

                blackBorder->setPosition(mapPos.x - 320, mapPos.y + (240 * circleSize.y));
                blackBorder->setSize(Vector2f(640, 240 - (240 * circleSize.y)));

                mainTexture.draw(*blackBorder);

                if (circleSize.x > 0.01)
                    circleSpr->scale(0.96, 0.96);
                else
                {
                    #ifdef DEBUGMODE
                    cout << "Loading level... : " << fileToLoad << endl;
                    #endif // DEBUGMODE
                    loadingType = 3;
                    nextLevel = true;

                    delete circleFadeTex;
                    delete circleSpr;

                    circleFadeTex = NULL;
                    circleSpr = NULL;

                    titlePos = -360;
                    titleSpd = 0;

                    fadeCircle = false;

                    Vector2f ppos(marioSpr->getPosition());

                    if (levelsPassed != listMark.end())
                    {
                        float angle = atan2(ppos.y - (*levelsPassed)->y + 8, ppos.x - (*levelsPassed)->x - 2);

                        marioSpr_speed = Vector2f(-WORLDMAP_MARIOSPEED * cos(angle), -WORLDMAP_MARIOSPEED * sin(angle));
                        marioSpr_velocity = WORLDMAP_MARIOSPEED;
                        marioSpr_validation = 0;

                        size_t size = mapTrace->getVertexCount() / 4;
                        for (register unsigned int i = 0; i < size; i++)
                        {
                            (*mapTrace)[(i*4)].texCoords.x = 12;
                            (*mapTrace)[(i*4)+1].texCoords.x = 24;
                            (*mapTrace)[(i*4)+2].texCoords.x = 24;
                            (*mapTrace)[(i*4)+3].texCoords.x = 12;
                        }

                        frame_Progress = 0;

                        levelsFinished++;
                    }
                    else
                    {
                        map_completed = 1;

                        frame_Mario = 0;
                        frame_Start = 0;
                    }

                    if (Level_Load())
                        goto LBL_TOLEVEL;
                    else
                    {
                        TCHAR temp[MAX_PATH];

                        loadingType = 4;

                        // Reset the current directory :
                        GetModuleFileName(NULL, temp, MAX_PATH);
                        PathRemoveFileSpec(temp);
                        SetCurrentDirectory(temp);
                    }
                }
            }

            mainTexture.display();

            if (frame_Progress < 11)
                frame_Progress++;
            else
                frame_Progress = 0;

            mainWindow->clear(Color::Black);
            mainWindow->draw(textureRender);
            mainWindow->display();
        }
    }
    else                    // Regular level
    {
        // Event loop :
        while (!exitLoop)
        {
            LBL_TOLEVEL :

            while (mainWindow->pollEvent(eventSystem))
            {
                switch (eventSystem.type)
                {
                    case Event::Closed      :
                        exitLoop = true;

                        mainWindow->close();

                        break;

                    case Event::KeyPressed :

                        if (exitToTitle)
                            break;

                        switch (eventSystem.key.code)
                        {
                            case Keyboard::Escape :

                                if (MessageBox(NULL, "Do you want to back to the Title Screen ?", "Back to the Title ?", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
                                    exitToTitle = true;

                                break;

                            #ifdef DEBUGMODE
                            case Keyboard::F3 : mainWindow->setFramerateLimit(4); break;
                            case Keyboard::F4 : mainWindow->setFramerateLimit(60); break;
                            #endif // DEBUGMODE
                            default :

                                if (player != NULL)
                                    player->fire(eventSystem.key.code);
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

            // Camera and timer :
            if (player != NULL)
            {
                Vector2f playerPos;

                if (autoscroll != zoneb + 1)
                {
                    if (koopaEngaged > 0)
                    {
                        playerPos = cameraPos;

                        if (koopaEngaged == 1)
                            playerPos.x += 0.85;
                        else
                            playerPos.x -= 0.85;

                        if (playerPos.x < 320)
                            playerPos.x = 320;

                        if (zoneb)
                        {
                            if (playerPos.x > (levelScaleb.x * 640) - 320)
                                playerPos.x = (levelScaleb.x * 640) - 320;
                        }
                        else
                        {
                            if (playerPos.x > (levelScale.x * 640) - 320)
                                playerPos.x = (levelScale.x * 640) - 320;
                        }

                        if (player->m_active)
                            cameraPos = playerPos;
                    }
                    else
                    {
                        playerPos = player->getPosition();

                        if (levelCamera_shake > 0)
                        {
                            playerPos.x += (rand() / (RAND_MAX / levelCamera_shake)) - levelCamera_shake / 2.0f;
                            playerPos.y += (rand() / (RAND_MAX / levelCamera_shake)) - levelCamera_shake / 2.0f;

                            levelCamera_shake -= 0.8;
                        }

                        if (playerPos.x < 299)
                            playerPos.x = 299;

                        if (playerPos.y < 208)
                            playerPos.y = 208;

                        if (zoneb)
                        {
                            if (playerPos.x > (levelScaleb.x * 640) - 341)
                                playerPos.x = (levelScaleb.x * 640) - 341;

                            if (playerPos.y > (levelScaleb.y * 480) - 272)
                                playerPos.y = (levelScaleb.y * 480) - 272;
                        }
                        else
                        {
                            if (playerPos.x > (levelScale.x * 640) - 341)
                                playerPos.x = (levelScale.x * 640) - 341;

                            if (playerPos.y > (levelScale.y * 480) - 272)
                                playerPos.y = (levelScale.y * 480) - 272;
                        }

                        if (player->m_active)
                            cameraPos = Vector2f(lerp(cameraPos.x, playerPos.x + 21, 0.5), lerp(cameraPos.y, playerPos.y + 32, 0.5));
                        else if (player->m_warp != NULL)
                            cameraPos = Vector2f(playerPos.x + 21, playerPos.y + 32);
                    }
                }
                else if (autoscrollSpeed > 0)
                {
                    Vector2f scrolltarget(autoscrollPath[autoscroll_node]);

                    playerPos = cameraPos;

                    float angle = atan2(scrolltarget.y - playerPos.y, scrolltarget.x - playerPos.x);

                    playerPos.x += cos(angle) * autoscrollSpeed;
                    playerPos.y += sin(angle) * autoscrollSpeed;

                    float delta = angle - atan2(scrolltarget.y - playerPos.y, scrolltarget.x - playerPos.x);

                    if (delta > 1.570796 || delta < -1.570796)
                    {
                        playerPos = autoscrollPath[autoscroll_node];

                        if (autoscroll_node < autoscrollPath.size()-1)
                            autoscroll_node++;
                        else
                            autoscrollSpeed = 0;
                    }

                    if (player->m_active)
                        cameraPos = playerPos;
                }

                if (timer < 2500 && timer > 2410)
                {
                    if (timerAlertPhase)
                    {
                        timerAlert -= 0.08;

                        if (timerAlert < 0.6)
                            timerAlertPhase = false;
                    }
                    else
                    {
                        timerAlert += 0.08;

                        if (timerAlert > 1)
                            timerAlertPhase = true;
                    }

                    hudSpr[3]->setScale(1, timerAlert);
                }
                else if (timerAlert < 0.92)
                {
                    timerAlert += 0.08;
                    hudSpr[3]->setScale(1, timerAlert);
                }

                if (timer == 2400)
                    hudSpr[3]->setScale(1, 1);

                if (player->m_active && koopaHealth > 0)
                {
                    if (timer > 0)
                    {
                        timer--;

                        if (timer == 2499)
                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(19), sfxSamples[29], 0, NULL);
                    }
                    else
                        player->death();
                }
            }

            {
                Vector2f roundedCameraPos(roundf(cameraPos.x), roundf(cameraPos.y));

                scoreFont->setPosition(Vector2f(roundedCameraPos.x - 167, roundedCameraPos.y - 204));
                coinsFont->setPosition(Vector2f(roundedCameraPos.x - 35, roundedCameraPos.y - 204));
                livesFont->setPosition(Vector2f(roundedCameraPos.x - 182, roundedCameraPos.y - 224));

                if (koopaEngaged > 0)
                {
                    if (hudBowser_descend < -160)
                        hudBowser_descend += 1;
                    else
                        hudBowser_descend = -160;

                    (*hudBowser)[0].position = Vector2f(roundedCameraPos.x + 200, roundedCameraPos.y + hudBowser_descend);
                    (*hudBowser)[1].position = Vector2f(roundedCameraPos.x + 264, roundedCameraPos.y + hudBowser_descend);
                    (*hudBowser)[2].position = Vector2f(roundedCameraPos.x + 264, roundedCameraPos.y + hudBowser_descend + 42);
                    (*hudBowser)[3].position = Vector2f(roundedCameraPos.x + 200, roundedCameraPos.y + hudBowser_descend + 42);

                    for (register unsigned char i = 0; i < koopaHealth; i++)
                    {
                        (*hudBowser)[(i*4)+4].position = Vector2f(roundedCameraPos.x - (9 * i) + 191, roundedCameraPos.y + hudBowser_descend + 7);
                        (*hudBowser)[(i*4)+5].position = Vector2f(roundedCameraPos.x - (9 * i) + 200, roundedCameraPos.y + hudBowser_descend + 7);
                        (*hudBowser)[(i*4)+6].position = Vector2f(roundedCameraPos.x - (9 * i) + 200, roundedCameraPos.y + hudBowser_descend + 39);
                        (*hudBowser)[(i*4)+7].position = Vector2f(roundedCameraPos.x - (9 * i) + 191, roundedCameraPos.y + hudBowser_descend + 39);
                    }
                }

                if (timer % 25 == 0 && player != NULL)
                {
                    if (player->m_active)
                    {
                        char timebuf[6]("");
                        sprintf(timebuf, "%d", timer / 25);

                        timerFont->setText(timebuf, 6, Vector2f(roundedCameraPos.x + 257, roundedCameraPos.y - 204), true);
                    }
                    else
                        timerFont->setPosition(Vector2f(roundedCameraPos.x + 257, roundedCameraPos.y - 204));
                }
                else
                    timerFont->setPosition(Vector2f(roundedCameraPos.x + 257, roundedCameraPos.y - 204));

                if (worldNamePos.x > -1)
                    worldFont->setPosition(Vector2f(roundedCameraPos.x - worldNamePos.x + 77, roundedCameraPos.y + worldNamePos.y));

                hudSpr[0]->setPosition(roundedCameraPos.x - 284, roundedCameraPos.y - 224);
                hudSpr[1]->setPosition(roundedCameraPos.x - 82, roundedCameraPos.y - 204);
                hudSpr[2]->setPosition(roundedCameraPos.x + 38, roundedCameraPos.y - 224);
                hudSpr[3]->setPosition(roundedCameraPos.x + 194, roundedCameraPos.y - 216);

                levelCamera->setCenter(roundedCameraPos);
                mainTexture.setView(*levelCamera);
            }

            if (zoneb)
            {
                if (clearColorb)
                    mainTexture.clear(levelBackColorb);

                mainTexture.draw(gradientRectangleb, 4, Quads);

                if (backgroundSprb != NULL)
                    mainTexture.draw(*backgroundSprb);

                if (effectCloudsSprb[1] != NULL)
                {
                    effectCloudsSprb[1]->setTextureRect(IntRect(effectCloudPos[0], 0, levelScaleb.x * 640, 63));
                    mainTexture.draw(*effectCloudsSprb[1]);
                }

                if (!tilemapLayer1b.empty())
                    mainTexture.draw(&tilemapLayer1b.front(), tilemapLayer1b.size(), Quads, mainTileset);

                if (!tilemapLayer2b.empty())
                    mainTexture.draw(&tilemapLayer2b.front(), tilemapLayer2b.size(), Quads, mainTileset);

                if (!listPlaceablesb.empty())
                {
                    vector<list<Placeable*>::iterator> toDelete;

                    for (list<Placeable*>::iterator it = listPlaceablesb.begin(); it != listPlaceablesb.end(); it++)
                    {
                        Vector2f entPos((*it)->getPosition());
                        Vector2f entSize((*it)->getSize());

                        (*it)->secureUpdate();

                        if (entPos.x + entSize.x > cameraPos.x - 352 && entPos.y + entSize.y > cameraPos.y - 272 && entPos.x < cameraPos.x + 352 && entPos.y < cameraPos.y + 272)
                            (*it)->update();

                        if ((*it)->m_destroyed)
                        {
                            toDelete.emplace_back(it);
                            continue;
                        }

                        if ((entPos.x + entSize.x > cameraPos.x - 320 && entPos.y + entSize.y > cameraPos.y - 240 && entPos.x < cameraPos.x + 320 && entPos.y < cameraPos.y + 240) || (*it)->getID() == ID_PLAYER)
                            mainTexture.draw(**it);
                    }

                    for (list<Placeable*>::iterator it = listPlaceablesb.begin(); it != listPlaceablesb.end(); it++)
                        (*it)->afterUpdate();

                    // Deletion loop :
                    for (vector<list<Placeable*>::iterator>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
                    {
                        delete **it;
                        listPlaceablesb.erase(*it);
                    }
                }

                if (player != NULL)
                {
                    player->secureUpdate();

                    mainTexture.draw(*player);
                }

                if (!listEffect.empty())
                {
                    vector<list<Effect*>::iterator> toDelete;

                    for (list<Effect*>::iterator it = listEffect.begin(); it != listEffect.end(); it++)
                    {
                        (*it)->update();

                        if ((*it)->m_destroyed)
                        {
                            toDelete.emplace_back(it);
                            continue;
                        }

                        mainTexture.draw(**it);
                    }

                    // Deletion loop :
                    for (vector<list<Effect*>::iterator>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
                    {
                        delete **it;
                        listEffect.erase(*it);
                    }
                }

                if (!tilemapLayer3b.empty())
                    mainTexture.draw(&tilemapLayer3b.front(), tilemapLayer3b.size(), Quads, mainTileset);

                if (!listColliderb.empty())
                {
                    vector<list<Collider*>::iterator> toDelete;

                    for (list<Collider*>::iterator it = listColliderb.begin(); it != listColliderb.end(); it++)
                    {
                        if ((*it)->destroyed)
                            toDelete.emplace_back(it);
                    }

                    // Deletion loop :
                    for (vector<list<Collider*>::iterator>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
                    {
                        delete **it;
                        listColliderb.erase(*it);
                    }
                }

                if (waterb != NULL)
                {
                    if (!liquidSpotsb.empty() && player != NULL)
                    {
                        for (vector<TriggerData*>::iterator it = liquidSpotsb.begin(); it != liquidSpotsb.end(); it++)
                        {
                            if ((liquidTargetHeightb > (*it)->targetHeight - FLT_EPSILON && liquidTargetHeightb < (*it)->targetHeight + FLT_EPSILON) || !player->m_active)
                                continue;

                            if (player->m_aabb.intersects(FloatRect((*it)->x, (*it)->y, (*it)->width, (*it)->height)))
                            {
                                liquidTargetHeightb = (*it)->targetHeight;

                                if (waterb->getType())
                                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(15), sfxSamples[24], 0, NULL);
                                else
                                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(17), sfxSamples[18], 0, NULL);
                            }
                        }
                    }

                    float liquidHeight = waterb->getHeight();

                    if (liquidTargetHeightb < liquidHeight - liquidSpeed)
                        waterb->setHeight(liquidHeight - liquidSpeed, levelScaleb);
                    else if (liquidTargetHeightb > liquidHeight + liquidSpeed)
                        waterb->setHeight(liquidHeight + liquidSpeed, levelScaleb);
                    else
                        waterb->setHeight(liquidTargetHeightb, levelScaleb);

                    waterb->update();
                    mainTexture.draw(*waterb);
                }

                if (effectWeatherSprb != NULL)
                {
                    effectWeatherSprb->setTextureRect(IntRect(effectWeatherPos, -effectWeatherPos, levelScaleb.x * 640, levelScaleb.y * 480));

                    if (effectWeatherTypeb == 2)
                    {
                        if (effectLightning > 0)
                            effectLightning--;
                        else
                        {
                            effectLightningSpr->setPosition(cameraPos.x + (rand() % 320) - 300, cameraPos.y - 240);
                            effectLightningSpr->setColor(Color::White);

                            effectLightning = (rand() % 350) + 100;
                            levelCamera_shake = (rand() % 4) + 10;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(17), sfxSamples[33], 0, NULL);
                        }

                        float lightning(effectLightningSpr->getColor().a);

                        if (lightning > 38)
                        {
                            mainTexture.draw(*effectLightningSpr, BlendAdd);

                            lightning -= 38;
                            effectLightningSpr->setColor(Color(255, 255, 255, lightning));
                        }
                    }

                    mainTexture.draw(*effectWeatherSprb);
                }

                if (effectCloudsSprb[0] != NULL)
                {
                    effectCloudsSprb[0]->setTextureRect(IntRect(effectCloudPos[1], 0, levelScaleb.x * 640, 63));
                    mainTexture.draw(*effectCloudsSprb[0]);
                }
            }
            else
            {
                if (clearColora)
                    mainTexture.clear(levelBackColor);

                mainTexture.draw(gradientRectangle, 4, Quads);

                if (backgroundSpr != NULL)
                    mainTexture.draw(*backgroundSpr);

                if (effectCloudsSpr[1] != NULL)
                {
                    effectCloudsSpr[1]->setTextureRect(IntRect(effectCloudPos[0], 0, levelScale.x * 640, 63));
                    mainTexture.draw(*effectCloudsSpr[1]);
                }

                if (!tilemapLayer1.empty())
                    mainTexture.draw(&tilemapLayer1.front(), tilemapLayer1.size(), Quads, mainTileset);

                if (!tilemapLayer2.empty())
                    mainTexture.draw(&tilemapLayer2.front(), tilemapLayer2.size(), Quads, mainTileset);

                if (!listPlaceables.empty())
                {
                    vector<list<Placeable*>::iterator> toDelete;

                    for (list<Placeable*>::iterator it = listPlaceables.begin(); it != listPlaceables.end(); it++)
                    {
                        Vector2f entPos((*it)->getPosition());
                        Vector2f entSize((*it)->getSize());

                        (*it)->secureUpdate();

                        if (entPos.x + entSize.x > cameraPos.x - 352 && entPos.y + entSize.y > cameraPos.y - 272 && entPos.x < cameraPos.x + 352 && entPos.y < cameraPos.y + 272)
                            (*it)->update();

                        if ((*it)->m_destroyed)
                        {
                            toDelete.emplace_back(it);
                            continue;
                        }

                        if ((entPos.x + entSize.x > cameraPos.x - 320 && entPos.y + entSize.y > cameraPos.y - 240 && entPos.x < cameraPos.x + 320 && entPos.y < cameraPos.y + 240) || (*it)->getID() == ID_PLAYER)
                            mainTexture.draw(**it);
                    }

                    for (list<Placeable*>::iterator it = listPlaceables.begin(); it != listPlaceables.end(); it++)
                        (*it)->afterUpdate();

                    // Deletion loop :
                    for (vector<list<Placeable*>::iterator>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
                    {
                        #ifdef DEBUGMODE
                        if ((**it)->getID() == ID_PLAYER)
                            cout << "Player deletion" << endl;
                        #endif // DEBUGMODE

                        delete **it;
                        listPlaceables.erase(*it);
                    }
                }

                if (player != NULL)
                {
                    player->secureUpdate();

                    mainTexture.draw(*player);
                }

                if (!listEffect.empty())
                {
                    vector<list<Effect*>::iterator> toDelete;

                    for (list<Effect*>::iterator it = listEffect.begin(); it != listEffect.end(); it++)
                    {
                        (*it)->update();

                        if ((*it)->m_destroyed)
                        {
                            toDelete.emplace_back(it);
                            continue;
                        }

                        mainTexture.draw(**it);
                    }

                    // Deletion loop :
                    for (vector<list<Effect*>::iterator>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
                    {
                        delete **it;
                        listEffect.erase(*it);
                    }
                }

                if (!tilemapLayer3.empty())
                    mainTexture.draw(&tilemapLayer3.front(), tilemapLayer3.size(), Quads, mainTileset);

                if (!listCollider.empty())
                {
                    vector<list<Collider*>::iterator> toDelete;

                    for (list<Collider*>::iterator it = listCollider.begin(); it != listCollider.end(); it++)
                    {
                        if ((*it)->destroyed)
                            toDelete.emplace_back(it);
                    }

                    // Deletion loop :
                    for (vector<list<Collider*>::iterator>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
                    {
                        delete **it;
                        listCollider.erase(*it);
                    }
                }

                if (watera != NULL)
                {
                    if (!liquidSpots.empty() && player != NULL)
                    {
                        for (vector<TriggerData*>::iterator it = liquidSpots.begin(); it != liquidSpots.end(); it++)
                        {
                            if ((liquidTargetHeight > (*it)->targetHeight - FLT_EPSILON && liquidTargetHeight < (*it)->targetHeight + FLT_EPSILON) || !player->m_active)
                                continue;

                            if (player->m_aabb.intersects(FloatRect((*it)->x, (*it)->y, (*it)->width, (*it)->height)))
                            {
                                liquidTargetHeight = (*it)->targetHeight;

                                if (watera->getType())
                                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(15), sfxSamples[24], 0, NULL);
                                else
                                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(17), sfxSamples[18], 0, NULL);
                            }
                        }
                    }

                    float liquidHeight = watera->getHeight();

                    if (liquidTargetHeight < liquidHeight - liquidSpeed)
                        watera->setHeight(liquidHeight - liquidSpeed, levelScale);
                    else if (liquidTargetHeight > liquidHeight + liquidSpeed)
                        watera->setHeight(liquidHeight + liquidSpeed, levelScale);
                    else
                        watera->setHeight(liquidTargetHeight, levelScale);

                    watera->update();
                    mainTexture.draw(*watera);
                }

                if (effectWeatherSpr != NULL)
                {
                    effectWeatherSpr->setTextureRect(IntRect(effectWeatherPos, -effectWeatherPos, levelScale.x * 640, levelScale.y * 480));

                    if (effectWeatherType == 2)
                    {
                        if (effectLightning > 0)
                            effectLightning--;
                        else
                        {
                            effectLightningSpr->setPosition(cameraPos.x + (rand() % 320) - 300, cameraPos.y - 240);
                            effectLightningSpr->setColor(Color::White);

                            effectLightning = (rand() % 350) + 100;
                            levelCamera_shake = (rand() % 4) + 10;

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(17), sfxSamples[33], 0, NULL);
                        }

                        float lightning(effectLightningSpr->getColor().a);

                        if (lightning > 38)
                        {
                            mainTexture.draw(*effectLightningSpr, BlendAdd);

                            lightning -= 38;
                            effectLightningSpr->setColor(Color(255, 255, 255, lightning));
                        }
                    }

                    mainTexture.draw(*effectWeatherSpr);
                }

                if (effectCloudsSpr[0] != NULL)
                {
                    effectCloudsSpr[0]->setTextureRect(IntRect(effectCloudPos[1], 0, levelScale.x * 640, 63));
                    mainTexture.draw(*effectCloudsSpr[0]);
                }
            }

            // Toggling the switch blocks :
            for (register unsigned int i = 0; i < 4; i++)
            {
                if (game_toggleSolidity[i] == 1)
                    game_toggleSolidity[i] = 0;
                else if (game_toggleSolidity[i] == 3)
                    game_toggleSolidity[i] = 2;
            }

            if (spritehitter.left > -64)
            {
                if (spritehitter_create)
                    spritehitter.left = -64;
                else
                    spritehitter_create = true;
            }

            if (enemykiller.left > -64)
            {
                if (enemykiller_create)
                    enemykiller.left = -64;
                else
                    enemykiller_create = true;
            }

            if (frame_Hud % 6 == 0)
                hudSpr[1]->setTextureRect(IntRect(0, (frame_Hud/6) * 16, 28, 16));

            if (effectCloudPos[0] < 63)
                effectCloudPos[0]++;
            else
                effectCloudPos[0] = 0;

            if (effectCloudPos[1] < 62)
                effectCloudPos[1] += 2;
            else
                effectCloudPos[1] = 0;

            if (effectLavafallPos < 28)
                effectLavafallPos += 4;
            else
                effectLavafallPos = 0;

            if (zoneb)
            {
                if (effectWeatherTypeb == 3)
                {
                    if (effectWeatherPos < 127)
                        effectWeatherPos += 4;
                    else
                        effectWeatherPos = 0;
                }
                else
                {
                    if (effectWeatherPos < 127)
                        effectWeatherPos += 6;
                    else
                        effectWeatherPos = 0;
                }
            }
            else
            {
                if (effectWeatherType == 3)
                {
                    if (effectWeatherPos < 127)
                        effectWeatherPos += 4;
                    else
                        effectWeatherPos = 0;
                }
                else
                {
                    if (effectWeatherPos < 127)
                        effectWeatherPos += 6;
                    else
                        effectWeatherPos = 0;
                }
            }

            if (frame_Podoboo < 5)
                frame_Podoboo++;
            else
                frame_Podoboo = 0;

            if (frame_Flame < 11)
                frame_Flame++;
            else
                frame_Flame = 0;

            if (frame_Hud < 17)
                frame_Hud++;
            else
                frame_Hud = 0;

            if (frame_Water < 23)
                frame_Water++;
            else
                frame_Water = 0;

            if (frame_Lava < 55)
                frame_Lava++;
            else
                frame_Lava = 0;

            if (frame_Clouds < 29)
                frame_Clouds++;
            else
                frame_Clouds = 0;

            if (frame_Flower < 15)
                frame_Flower++;
            else
                frame_Flower = 0;

            if (frame_Rotodisc < 24)
                frame_Rotodisc++;
            else
                frame_Rotodisc = 0;

            if (koopaEngaged)
                mainTexture.draw(*hudBowser, hudTxt[5]);

            mainTexture.draw(*hudSpr[0]);
            mainTexture.draw(*hudSpr[1]);
            mainTexture.draw(*hudSpr[2]);
            mainTexture.draw(*hudSpr[3]);

            mainTexture.draw(*scoreFont);
            mainTexture.draw(*livesFont);
            mainTexture.draw(*coinsFont);
            mainTexture.draw(*timerFont);

            if (worldNamePos.x > -1)
                mainTexture.draw(*worldFont);

            if (restartLevel)
                Level_Reload();

            if (exitToTitle)
            {
                fadeRect->setPosition(0, 0);
                fadeRect->setFillColor(Color(0, 0, 0, 255));

                mainTexture.setView(View(Vector2f(320, 240), Vector2f(640, 480)));

                fadeAlpha = 255;

                targetScene = SCENE_TITLE;
                exitLoop = true;
            }

            if (exitLoop || nextLevel)
            {
                mainTexture.clear(Color::Black);
                mainTexture.draw(*loading);
            }

            if (nextLevel)
            {
                Level_Clear();

                if (loadingType == 3)
                {
                    loadingType = 4;

                    if (levelsPassed != listMark.end())
                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicWorld, 0, &musicChannel);
                    else
                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), musicSamples[29], 0, &musicChannel);

                    goto LBL_TOWORLD;
                }
                else if (!Level_Load())
                {
                    fadeRect->setPosition(0, 0);
                    fadeRect->setFillColor(Color(0, 0, 0, 255));

                    mainTexture.setView(View(Vector2f(320, 240), Vector2f(640, 480)));

                    fadeAlpha = 255;

                    targetScene = SCENE_TITLE;
                    exitLoop = true;
                }
            }

            mainTexture.display();

            mainWindow->clear(Color::Black);
            mainWindow->draw(textureRender);
            mainWindow->display();
        }

        if (!nextLevel)
            Level_Clear();
    }

    if (nextLevel)
        nextLevel = false;

    if (level_file.is_open())
        level_file.close();

    if (scenario_file.is_open())
        scenario_file.close();

    scenarioToLoad.clear();

    for (list<RectangleShape*>::iterator it = listTex.begin(); it != listTex.end(); it++)
        delete *it;

    for (list<Sprite*>::iterator it = listEye.begin(); it != listEye.end(); it++)
        delete *it;

    for (list<Lamp*>::iterator it = listLamps.begin(); it != listLamps.end(); it++)
        delete *it;

    for (list<MarkerData*>::iterator it = listMark.begin(); it != listMark.end(); it++)
        delete *it;

    listTex.clear();
    listEye.clear();
    listLamps.clear();
    listMark.clear();

    delete levelCamera;
    delete mapCamera;

    delete worldLightTex;
    delete worldBackTex[0];

    delete entTex[0];
    delete entTex[1];
    delete entTex[2];
    delete entTex[3];
    delete entTex[4];
    delete entTex[5];
    delete entTex[6];

    delete markSpr;
    delete sheetSpr;
    delete startSpr;
    delete titleSpr;
    delete completedSpr;
    delete mapTrace;

    delete effectLightningSpr;

    if (circleFadeTex != NULL)
        delete circleFadeTex;

    if (circleSpr != NULL)
        delete circleSpr;

    delete blackBorder;

    if (worldBackTex[1] != NULL)
        delete worldBackTex[1];

    for (register unsigned int i = 0; i < 166; i++)
        delete worldTextures[i];

    for (register unsigned int i = 0; i < 81; i++)
        delete worldEyecandies[i];

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

    delete worldLightSpr[0];
    delete worldLightSpr[1];
    delete worldLightSpr[2];
    delete worldLightSpr[3];
    delete worldLightSpr[4];

    delete backgroundTxt[0];
    delete backgroundTxt[1];
    delete backgroundTxt[2];
    delete backgroundTxt[3];
    delete backgroundTxt[4];
    delete backgroundTxt[5];
    delete backgroundTxt[6];
    delete backgroundTxt[7];
    delete backgroundTxt[8];
    delete backgroundTxt[9];
    delete backgroundTxt[10];
    delete backgroundTxt[11];
    delete backgroundTxt[14];
    delete backgroundTxt[15];

    delete sceneriesTxt[0];
    delete sceneriesTxt[1];
    delete sceneriesTxt[2];
    delete sceneriesTxt[3];
    delete sceneriesTxt[4];
    delete sceneriesTxt[5];
    delete sceneriesTxt[6];
    delete sceneriesTxt[7];
    delete sceneriesTxt[8];
    delete sceneriesTxt[9];
    delete sceneriesTxt[10];
    delete sceneriesTxt[11];
    delete sceneriesTxt[12];
    delete sceneriesTxt[13];

    delete effectTxt[0];
    delete effectTxt[1];
    delete effectTxt[2];
    delete effectTxt[3];
    delete effectTxt[4];
    delete effectTxt[5];
    delete effectTxt[6];
    delete effectTxt[7];
    delete effectTxt[8];
    delete effectTxt[9];
    delete effectTxt[10];
    delete effectTxt[11];
    delete effectTxt[12];
    delete effectTxt[13];
    delete effectTxt[14];
    delete effectTxt[15];

    delete itemsTxt[0];
    delete itemsTxt[1];
    delete itemsTxt[2];
    delete itemsTxt[3];
    delete itemsTxt[4];
    delete itemsTxt[5];
    delete itemsTxt[6];
    delete itemsTxt[7];
    delete itemsTxt[8];
    delete itemsTxt[9];
    delete itemsTxt[10];

    delete enemiesTxt[0];
    delete enemiesTxt[1];
    delete enemiesTxt[2];
    delete enemiesTxt[3];
    delete enemiesTxt[4];
    delete enemiesTxt[5];
    delete enemiesTxt[6];
    delete enemiesTxt[7];
    delete enemiesTxt[8];
    delete enemiesTxt[9];
    delete enemiesTxt[10];
    delete enemiesTxt[11];
    delete enemiesTxt[12];
    delete enemiesTxt[13];
    delete enemiesTxt[14];
    delete enemiesTxt[15];
    delete enemiesTxt[16];
    delete enemiesTxt[17];
    delete enemiesTxt[18];
    delete enemiesTxt[19];
    delete enemiesTxt[20];
    delete enemiesTxt[21];
    delete enemiesTxt[22];
    delete enemiesTxt[23];
    delete enemiesTxt[24];
    delete enemiesTxt[25];
    delete enemiesTxt[26];
    delete enemiesTxt[27];
    delete enemiesTxt[28];
    delete enemiesTxt[29];
    delete enemiesTxt[30];
    delete enemiesTxt[31];
    delete enemiesTxt[32];
    delete enemiesTxt[33];
    delete enemiesTxt[34];
    delete enemiesTxt[35];
    delete enemiesTxt[36];
    delete enemiesTxt[37];
    delete enemiesTxt[38];
    delete enemiesTxt[39];
    delete enemiesTxt[40];
    delete enemiesTxt[41];

    delete hazardsTxt[0];
    delete hazardsTxt[1];
    delete hazardsTxt[2];
    delete hazardsTxt[3];
    delete hazardsTxt[4];
    delete hazardsTxt[5];
    delete hazardsTxt[6];
    delete hazardsTxt[7];
    delete hazardsTxt[8];
    delete hazardsTxt[9];
    delete hazardsTxt[10];

    delete marksTxt[0];
    delete marksTxt[1];
    delete marksTxt[2];
    delete marksTxt[3];
    delete marksTxt[4];
    delete marksTxt[5];
    delete marksTxt[6];
    delete marksTxt[7];
    delete marksTxt[8];
    delete marksTxt[9];
    delete marksTxt[10];
    delete marksTxt[11];

    delete marioTxt[0];
    delete marioTxt[1];
    delete marioTxt[2];
    delete marioTxt[3];
    delete marioTxt[4];
    delete marioTxt[5];

    delete hudTxt[0];
    delete hudTxt[1];
    delete hudTxt[2];
    delete hudTxt[3];
    delete hudTxt[4];
    delete hudTxt[5];
    delete hudTxt[6];

    delete mapBack;

    delete scoreFont;
    delete coinsFont;

    delete mainTileset;

    delete fadeRect;

    delete hudSpr[0];
    delete hudSpr[1];
    delete hudSpr[2];
    delete hudSpr[3];

    delete hudBowser;

    #ifndef NOMUSIC
    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(musicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(musicChannel);

        FMOD_Sound_Release(sfxSamples[0]);
        FMOD_Sound_Release(sfxSamples[1]);
        FMOD_Sound_Release(sfxSamples[2]);
        FMOD_Sound_Release(sfxSamples[3]);
        FMOD_Sound_Release(sfxSamples[4]);
        FMOD_Sound_Release(sfxSamples[5]);
        FMOD_Sound_Release(sfxSamples[6]);
        FMOD_Sound_Release(sfxSamples[7]);
        FMOD_Sound_Release(sfxSamples[8]);
        FMOD_Sound_Release(sfxSamples[9]);
        FMOD_Sound_Release(sfxSamples[10]);
        FMOD_Sound_Release(sfxSamples[11]);
        FMOD_Sound_Release(sfxSamples[12]);
        FMOD_Sound_Release(sfxSamples[13]);
        FMOD_Sound_Release(sfxSamples[14]);
        FMOD_Sound_Release(sfxSamples[15]);
        FMOD_Sound_Release(sfxSamples[16]);
        FMOD_Sound_Release(sfxSamples[17]);
        FMOD_Sound_Release(sfxSamples[18]);
        FMOD_Sound_Release(sfxSamples[19]);
        FMOD_Sound_Release(sfxSamples[20]);
        FMOD_Sound_Release(sfxSamples[21]);
        FMOD_Sound_Release(sfxSamples[22]);
        FMOD_Sound_Release(sfxSamples[23]);
        FMOD_Sound_Release(sfxSamples[24]);
        FMOD_Sound_Release(sfxSamples[25]);
        FMOD_Sound_Release(sfxSamples[26]);
        FMOD_Sound_Release(sfxSamples[27]);
        FMOD_Sound_Release(sfxSamples[28]);
        FMOD_Sound_Release(sfxSamples[29]);
        FMOD_Sound_Release(sfxSamples[30]);
        FMOD_Sound_Release(sfxSamples[31]);
        FMOD_Sound_Release(sfxSamples[32]);
        FMOD_Sound_Release(sfxSamples[33]);
        FMOD_Sound_Release(sfxSamples[34]);

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
        FMOD_Sound_Release(musicSamples[25]);
        FMOD_Sound_Release(musicSamples[26]);
        FMOD_Sound_Release(musicSamples[27]);
        FMOD_Sound_Release(musicSamples[29]);

        if (musicSamples[28] != NULL)
            FMOD_Sound_Release(musicSamples[28]);
    }
    #endif

    return true;
}

static bool InitAssets()
{
    bool allright = true;

    Image tempImage;
    FMOD_RESULT result;

    fadeAlpha = 0;

    exitToTitle = false;
    nextLevel = false;
    restartLevel = false;
    fadeCircle = false;

    circleFadeTex = NULL;
    circleSpr = NULL;

    blackBorder = new RectangleShape;
    blackBorder->setFillColor(Color::Black);

    cameraPos = Vector2f(320, 240);

    levelCamera = new View(Vector2f(320, 240), Vector2f(640, 480));
    mapCamera = new View(Vector2f(320, 240), Vector2f(640, 480));
    levelCamera_shake = 0;

    effectCloudPos[0] = 0;
    effectCloudPos[1] = 0;
    effectLavafallPos = 0;
    effectWeatherPos = 0;

    autoscroll_node = 0;
    autoscroll_nodesaved = 0;

    game_powerup = 0;
    game_coins = 0;
    game_score = 0;

    #ifdef PUBLISHER
    if (target_lives == 0)
        game_lives = 4;
    else
        game_lives = target_lives;
    #else
        game_lives = 4;
    #endif

    worldLightTex = new Texture;

    if (!worldLightTex->loadFromFile("Data/Gfx/Worldmap/Worldmap_LightHalo.png"))
        allright = false;

    worldBackTex[0] = new Texture;

    if (!worldBackTex[0]->loadFromFile("Data/Gfx/Worldmap/Worldmap_Background.jpg"))
        allright = false;

    worldBackTex[0]->setRepeated(true);

    worldBackTex[1] = NULL;

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_Cross.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[0] = new Texture;
    entTex[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_Progress.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[1] = new Texture;
    entTex[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_CornerSheet.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[2] = new Texture;
    entTex[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_Title.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[3] = new Texture;
    entTex[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_Completed.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[4] = new Texture;
    entTex[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_Start.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[5] = new Texture;
    entTex[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Worldmap/Worldmap_Mario.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    entTex[6] = new Texture;
    entTex[6]->loadFromImage(tempImage);

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

    markSpr = new Sprite(*entTex[0]);

    worldLightSpr[0] = new Sprite(*worldLightTex);
    worldLightSpr[0]->setOrigin(64, 64);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Lands.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[0] = new Texture;
    backgroundTxt[0]->loadFromImage(tempImage);
    backgroundTxt[0]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Autumn.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[1] = new Texture;
    backgroundTxt[1]->loadFromImage(tempImage);
    backgroundTxt[1]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Desert.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[2] = new Texture;
    backgroundTxt[2]->loadFromImage(tempImage);
    backgroundTxt[2]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Blue.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[14] = new Texture;
    backgroundTxt[14]->loadFromImage(tempImage);
    backgroundTxt[14]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Snow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[15] = new Texture;
    backgroundTxt[15]->loadFromImage(tempImage);
    backgroundTxt[15]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Night.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[3] = new Texture;
    backgroundTxt[3]->loadFromImage(tempImage);
    backgroundTxt[3]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Underwater.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[4] = new Texture;
    backgroundTxt[4]->loadFromImage(tempImage);
    backgroundTxt[4]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Rocky.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[5] = new Texture;
    backgroundTxt[5]->loadFromImage(tempImage);
    backgroundTxt[5]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_Clouds.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[6] = new Texture;
    backgroundTxt[6]->loadFromImage(tempImage);
    backgroundTxt[6]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Backgrounds/Background_DarkClouds.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    backgroundTxt[7] = new Texture;
    backgroundTxt[7]->loadFromImage(tempImage);
    backgroundTxt[7]->setRepeated(true);

    backgroundTxt[8] = new Texture;

    if (!backgroundTxt[8]->loadFromFile("Data/Gfx/Backgrounds/Background_Castle.png"))
        allright = false;

    backgroundTxt[8]->setRepeated(true);

    backgroundTxt[9] = new Texture;

    if (!backgroundTxt[9]->loadFromFile("Data/Gfx/Backgrounds/Background_Underground.png"))
        allright = false;

    backgroundTxt[9]->setRepeated(true);

    backgroundTxt[10] = new Texture;

    if (!backgroundTxt[10]->loadFromFile("Data/Gfx/Backgrounds/Background_DarkCave.png"))
        allright = false;

    backgroundTxt[10]->setRepeated(true);

    backgroundTxt[11] = new Texture;

    if (!backgroundTxt[11]->loadFromFile("Data/Gfx/Backgrounds/Background_LavaCave.png"))
        allright = false;

    backgroundTxt[11]->setRepeated(true);

    backgroundTxt[12] = NULL;
    backgroundTxt[13] = NULL;

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_GreenBush.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[0] = new Texture;
    sceneriesTxt[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_GreyBush.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[1] = new Texture;
    sceneriesTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_YellowBush.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[2] = new Texture;
    sceneriesTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_RedBush.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[3] = new Texture;
    sceneriesTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_BlueBush.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[4] = new Texture;
    sceneriesTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_Cloud.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[5] = new Texture;
    sceneriesTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_DarkCloud.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[6] = new Texture;
    sceneriesTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_BowserPaint.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[7] = new Texture;
    sceneriesTxt[7]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_SmallCastle.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[8] = new Texture;
    sceneriesTxt[8]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_BigCastle.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[9] = new Texture;
    sceneriesTxt[9]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_Waterfall.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[10] = new Texture;
    sceneriesTxt[10]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_Lavafall.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[11] = new Texture;
    sceneriesTxt[11]->loadFromImage(tempImage);
    sceneriesTxt[11]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_SmallCastleSnow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[12] = new Texture;
    sceneriesTxt[12]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Sceneries/Scenery_BigCastleSnow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    sceneriesTxt[13] = new Texture;
    sceneriesTxt[13]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_DayClouds.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[0] = new Texture;
    effectTxt[0]->loadFromImage(tempImage);
    effectTxt[0]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_DarkClouds.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[1] = new Texture;
    effectTxt[1]->loadFromImage(tempImage);
    effectTxt[1]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_SandClouds.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[2] = new Texture;
    effectTxt[2]->loadFromImage(tempImage);
    effectTxt[2]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_Rain.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[3] = new Texture;
    effectTxt[3]->loadFromImage(tempImage);
    effectTxt[3]->setRepeated(true);

    effectTxt[4] = new Texture;

    if (!effectTxt[4]->loadFromFile("Data/Gfx/Effects/Effect_Lightning.jpg"))
        allright = false;

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_Snow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[5] = new Texture;
    effectTxt[5]->loadFromImage(tempImage);
    effectTxt[5]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_LevelEndBar.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[6] = new Texture;
    effectTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_Water.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[7] = new Texture;
    effectTxt[7]->loadFromImage(tempImage);
    effectTxt[7]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_BlockCoin.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[8] = new Texture;
    effectTxt[8]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_FireExplosion.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[9] = new Texture;
    effectTxt[9]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_WaterSplash.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[10] = new Texture;
    effectTxt[10]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_LavaSplash.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[11] = new Texture;
    effectTxt[11]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_CoinSparks.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[12] = new Texture;
    effectTxt[12]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_Points.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[13] = new Texture;
    effectTxt[13]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Effects/Effect_Shard.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    effectTxt[14] = new Texture;
    effectTxt[14]->loadFromImage(tempImage);

    effectTxt[15] = new Texture;

    if (!effectTxt[15]->loadFromFile("Data/Gfx/Effects/Effect_Invincibility.png"))
        allright = false;

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Block.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[0] = new Texture;
    itemsTxt[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Brick.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[1] = new Texture;
    itemsTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Coin.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[2] = new Texture;
    itemsTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Mushroom.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[3] = new Texture;
    itemsTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_FireFlower.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[4] = new Texture;
    itemsTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Beetroot.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[5] = new Texture;
    itemsTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Lui.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[6] = new Texture;
    itemsTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_Star.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[7] = new Texture;
    itemsTxt[7]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Item_1up.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[8] = new Texture;
    itemsTxt[8]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Projectiles/Item_Fireball.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[9] = new Texture;
    itemsTxt[9]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Items/Projectiles/Item_Beet.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    itemsTxt[10] = new Texture;
    itemsTxt[10]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_GoombaBrown.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[0] = new Texture;
    enemiesTxt[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_GoombaGrey.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[1] = new Texture;
    enemiesTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_Spiny.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[2] = new Texture;
    enemiesTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_GreySpiny.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[3] = new Texture;
    enemiesTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_BuzzyBeetle.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[4] = new Texture;
    enemiesTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_KoopaGreen.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[5] = new Texture;
    enemiesTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_KoopaRed.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[6] = new Texture;
    enemiesTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_KoopaBlue.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[7] = new Texture;
    enemiesTxt[7]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_KoopaYellow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[8] = new Texture;
    enemiesTxt[8]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ParatroopaGreen.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[9] = new Texture;
    enemiesTxt[9]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ParatroopaRed.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[10] = new Texture;
    enemiesTxt[10]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ParatroopaBlue.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[11] = new Texture;
    enemiesTxt[11]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ParatroopaYellow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[12] = new Texture;
    enemiesTxt[12]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ShellBuzzy.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[13] = new Texture;
    enemiesTxt[13]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ShellGreen.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[14] = new Texture;
    enemiesTxt[14]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ShellRed.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[15] = new Texture;
    enemiesTxt[15]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ShellBlue.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[16] = new Texture;
    enemiesTxt[16]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_ShellYellow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[17] = new Texture;
    enemiesTxt[17]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_PlantGreen.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[18] = new Texture;
    enemiesTxt[18]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_PlantPink.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[19] = new Texture;
    enemiesTxt[19]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_PlantGrey.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[20] = new Texture;
    enemiesTxt[20]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_FishRed.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[21] = new Texture;
    enemiesTxt[21]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_FishGreen.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[22] = new Texture;
    enemiesTxt[22]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_FishBlue.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[23] = new Texture;
    enemiesTxt[23]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_FishYellow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[24] = new Texture;
    enemiesTxt[24]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_HammerBro.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[25] = new Texture;
    enemiesTxt[25]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_SilverBro.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[26] = new Texture;
    enemiesTxt[26]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_FireBro.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[27] = new Texture;
    enemiesTxt[27]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_BeetBro.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[28] = new Texture;
    enemiesTxt[28]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_SpikeBro.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[29] = new Texture;
    enemiesTxt[29]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_Hammer.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[30] = new Texture;
    enemiesTxt[30]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_SilverHammer.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[31] = new Texture;
    enemiesTxt[31]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_Lakitu.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[32] = new Texture;
    enemiesTxt[32]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_LakituDark.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[33] = new Texture;
    enemiesTxt[33]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_LakituGold.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[34] = new Texture;
    enemiesTxt[34]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_SpinyEgg.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[35] = new Texture;
    enemiesTxt[35]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_DarkSpinyEgg.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[36] = new Texture;
    enemiesTxt[36]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_PoisonMushroom.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[37] = new Texture;
    enemiesTxt[37]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_Thwomp.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[38] = new Texture;
    enemiesTxt[38]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_Bowser.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[39] = new Texture;
    enemiesTxt[39]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_BowserFlame.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[40] = new Texture;
    enemiesTxt[40]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Enemies/Enemy_Plant.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    enemiesTxt[41] = new Texture;
    enemiesTxt[41]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_Lava.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[0] = new Texture;
    hazardsTxt[0]->loadFromImage(tempImage);
    hazardsTxt[0]->setRepeated(true);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_Podoboo.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[1] = new Texture;
    hazardsTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_Cannon.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[2] = new Texture;
    hazardsTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_BulletBill.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[3] = new Texture;
    hazardsTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_Rotodisc.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[4] = new Texture;
    hazardsTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_LavaThrower.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[5] = new Texture;
    hazardsTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_DiagonalLavaThrower.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[6] = new Texture;
    hazardsTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_LavaThrow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[7] = new Texture;
    hazardsTxt[7]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_SpikeballLauncher.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[8] = new Texture;
    hazardsTxt[8]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_SpikeBall.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[9] = new Texture;
    hazardsTxt[9]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hazards/Hazard_CentipedePlatform.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hazardsTxt[10] = new Texture;
    hazardsTxt[10]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Platforms/Platform_Red.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[0] = new Texture;
    marksTxt[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Platforms/Platform_Short.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[1] = new Texture;
    marksTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Platforms/Platform_Cloud.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[2] = new Texture;
    marksTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Platforms/Platform_Stone.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[3] = new Texture;
    marksTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Mark_SpringRed.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[4] = new Texture;
    marksTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Mark_SpringGreen.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[5] = new Texture;
    marksTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Mark_SpringBlue.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[6] = new Texture;
    marksTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Mark_CheckPoint.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[7] = new Texture;
    marksTxt[7]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Mark_LevelEnd.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[8] = new Texture;
    marksTxt[8]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Mark_LevelEndArrow.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[9] = new Texture;
    marksTxt[9]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Switches/Switch_Activator.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[10] = new Texture;
    marksTxt[10]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Marks/Switches/Switch_Blocks.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marksTxt[11] = new Texture;
    marksTxt[11]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Mario/Mario_Small.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marioTxt[0] = new Texture;
    marioTxt[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Mario/Mario_Big.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marioTxt[1] = new Texture;
    marioTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Mario/Mario_Fire.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marioTxt[2] = new Texture;
    marioTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Mario/Mario_Beet.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marioTxt[3] = new Texture;
    marioTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Mario/Mario_Lui.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marioTxt[4] = new Texture;
    marioTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Mario/Mario_Dead.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    marioTxt[5] = new Texture;
    marioTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_Lives.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[0] = new Texture;
    hudTxt[0]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_Coins.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[1] = new Texture;
    hudTxt[1]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_World.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[2] = new Texture;
    hudTxt[2]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_Time.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[3] = new Texture;
    hudTxt[3]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_Font.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[4] = new Texture;
    hudTxt[4]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_BowserHealth.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[5] = new Texture;
    hudTxt[5]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Hud/Hud_GameOver.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    hudTxt[6] = new Texture;
    hudTxt[6]->loadFromImage(tempImage);

    if (!tempImage.loadFromFile("Data/Gfx/Tilesets/Tileset_Default.png"))
        allright = false;

    tempImage.createMaskFromColor(Color::Magenta);

    mainTileset = new Texture;
    mainTileset->loadFromImage(tempImage);

    sheetSpr = new Sprite(*entTex[2]);
    titleSpr = new Sprite(*entTex[3]);
    startSpr = new Sprite(*entTex[5]);
    completedSpr = new Sprite(*entTex[4]);
    completedSpr->setTextureRect(IntRect(0, 0, 204, 31));
    marioSpr = new Sprite(*entTex[6], IntRect(0, 0, 16, 28));
    marioSpr_speed = Vector2f(-64, -64);
    marioSpr_validation = 0;
    marioSpr_velocity = WORLDMAP_MARIOSPEED;

    mapTrace = new VertexArray(Quads);
    mapTrace_timer = 0;
    mapTrace_delimiter = WORLDMAP_PROGRESSINTERVAL;

    backgroundSpr = NULL;
    effectCloudsSpr[0] = NULL;
    effectCloudsSpr[1] = NULL;
    effectWeatherSpr = NULL;

    backgroundSprb = NULL;
    effectCloudsSprb[0] = NULL;
    effectCloudsSprb[1] = NULL;
    effectWeatherSprb = NULL;

    effectLightningSpr = new Sprite(*effectTxt[4]);
    effectLightningSpr->setColor(Color(255, 255, 255, 0));

    watera = NULL;
    waterb = NULL;

    hudSpr[0] = new Sprite(*hudTxt[0]);
    hudSpr[1] = new Sprite(*hudTxt[1]);
    hudSpr[2] = new Sprite(*hudTxt[2]);
    hudSpr[3] = new Sprite(*hudTxt[3]);
    hudSpr[3]->setOrigin(0, 8);

    hudBowser = new VertexArray(Quads);
    hudBowser_descend = -308;

    scoreFont = new SpriteFont(Vector2f(153, 36), true, "0");
    coinsFont = new SpriteFont(Vector2f(285, 36), false, "0");

    fadeRect = new RectangleShape(Vector2f(640, 480));
    fadeRect->setFillColor(Color::Black);

    #ifndef NOMUSIC
    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Jump.wav", FMOD_DEFAULT, NULL, &sfxSamples[0]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Coin.wav", FMOD_DEFAULT, NULL, &sfxSamples[1]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Bump.wav", FMOD_DEFAULT, NULL, &sfxSamples[2]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Break.wav", FMOD_DEFAULT, NULL, &sfxSamples[3]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Sprout.wav", FMOD_DEFAULT, NULL, &sfxSamples[4]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/PowerUp.wav", FMOD_DEFAULT, NULL, &sfxSamples[5]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/PowerDown.wav", FMOD_DEFAULT, NULL, &sfxSamples[6]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/1Up.wav", FMOD_DEFAULT, NULL, &sfxSamples[7]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Fireball.wav", FMOD_DEFAULT, NULL, &sfxSamples[8]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Beet.wav", FMOD_DEFAULT, NULL, &sfxSamples[9]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Hammer.wav", FMOD_DEFAULT, NULL, &sfxSamples[10]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Stomp.wav", FMOD_DEFAULT, NULL, &sfxSamples[11]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Kick.wav", FMOD_DEFAULT, NULL, &sfxSamples[12]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Spring.wav", FMOD_DEFAULT, NULL, &sfxSamples[13]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Lakitu1.wav", FMOD_DEFAULT, NULL, &sfxSamples[14]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Lakitu2.wav", FMOD_DEFAULT, NULL, &sfxSamples[15]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Lakitu3.wav", FMOD_DEFAULT, NULL, &sfxSamples[16]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Swim.wav", FMOD_DEFAULT, NULL, &sfxSamples[17]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/RaisingWater.wav", FMOD_DEFAULT, NULL, &sfxSamples[18]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Centipede.wav", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &sfxSamples[19]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/CannonNormal.wav", FMOD_DEFAULT, NULL, &sfxSamples[20]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/CannonHoming.wav", FMOD_DEFAULT, NULL, &sfxSamples[21]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/CannonSpike.wav", FMOD_DEFAULT, NULL, &sfxSamples[22]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/CannonLava.wav", FMOD_DEFAULT, NULL, &sfxSamples[23]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/BowserFire.wav", FMOD_DEFAULT, NULL, &sfxSamples[24]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/BowserDammaged.wav", FMOD_DEFAULT, NULL, &sfxSamples[25]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/BowserLastDammage.wav", FMOD_DEFAULT, NULL, &sfxSamples[26]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/BowserFall.wav", FMOD_DEFAULT, NULL, &sfxSamples[27]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/EnterLevel.wav", FMOD_DEFAULT, NULL, &sfxSamples[28]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/TimeWarning.wav", FMOD_DEFAULT, NULL, &sfxSamples[29]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/TimeCount.wav", FMOD_DEFAULT, NULL, &sfxSamples[30]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Death.wav", FMOD_DEFAULT, NULL, &sfxSamples[31]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/GameOver.wav", FMOD_DEFAULT, NULL, &sfxSamples[32]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Storm.wav", FMOD_DEFAULT, NULL, &sfxSamples[33]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateSound(soundSystem, "Data/Sfx/Switch.wav", FMOD_DEFAULT, NULL, &sfxSamples[34]);

    if (result != FMOD_OK)
        allright = false;

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

    result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Victory.mp3", FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[25]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Starman.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[26]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/Battle.mp3", FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[27]);

    if (result != FMOD_OK)
        allright = false;

    result = FMOD_System_CreateStream(soundSystem, "Data/Sfx/Musics/MapCompleted.mp3", FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[29]);

    if (result != FMOD_OK)
        allright = false;
    #endif

    musicSamples[22] = NULL;
    musicSamples[23] = NULL;
    musicSamples[24] = NULL;
    musicSamples[28] = NULL;

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

    #ifdef DEBUGMODE
    enableShaders = false;
    #endif // DEBUGMODE

    if (enableShaders && loadingType == 4)
    {
        if (!useShaders)
        {
            #ifndef PUBLISHER
            StrCpy(trayIcon.szInfoTitle, "Shaders unavailable");
            StrCpy(trayIcon.szInfo, "Your current Graphics Driver doesn't support Shader Model 2.0 !");

            Shell_NotifyIcon(NIM_MODIFY, &trayIcon);
            #endif // PUBLISHER

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

        renderPassSpr[0] = NULL;
        renderPassSpr[1] = NULL;

        glowShaderH = NULL;
        glowShaderV = NULL;
    }

    if (loadingType < 4)
    {
        if (!Level_Load())
            allright = false;
    }
    else
    {
        if (!World_Load())
            allright = false;
    }

    return allright;
}

static bool Level_Load()
{
    vector <Placeable*> listMiddle;
    vector <Placeable*> listAfter;

    vector <Placeable*> listMiddleb;
    vector <Placeable*> listAfterb;

    Vector2f startPos(-64, -64);
    char startZone;
    unsigned char gravity;

    Uint8 alphaBack;
    Uint8 alphaFront;

    Uint8 alphaBackb;
    Uint8 alphaFrontb;

    levelCamera_shake = 0;

    effectCloudPos[0] = 0;
    effectCloudPos[1] = 0;
    effectLavafallPos = 0;
    effectWeatherPos = 0;
    effectLightning = (rand() % 350) + 100;

    autoscroll_node = 0;
    autoscroll_nodesaved = 0;

    koopaEngaged = 0;

    frame_Hud = 0;
    frame_Water = 0;
    frame_Lava = 0;
    frame_Flame = 0;
    frame_Podoboo = 0;
    frame_Clouds = 0;
    frame_Flower = 0;
    frame_Rotodisc = 0;

    game_toggleSolidity[0] = 0;
    game_toggleSolidity[1] = 0;
    game_toggleSolidity[2] = 0;
    game_toggleSolidity[3] = 0;

    game_toggleSoliditySave[0] = 0;
    game_toggleSoliditySave[1] = 0;
    game_toggleSoliditySave[2] = 0;
    game_toggleSoliditySave[3] = 0;

    player = NULL;

    blockhitter = FloatRect(-64, -64, 28, 8);
    spritehitter = FloatRect(-64, -64, 0, 0);
    enemystomper = FloatRect(-64, -64, 34, 12);
    enemykiller = FloatRect(-64, -64, 8, 8);
    blockkiller = FloatRect(-64, -64, 28, 33);
    enemystomper_pt = NULL;
    enemykiller_fire = 0;
    enemykiller_create = false;
    blockkiller_create = true;
    spritehitter_create = false;

    killcount = 0;

    timerAlert = 1.0f;
    timerAlertPhase = true;

    if (loadingType == 2 && !scenario_file.is_open())
    {
        scenario_file.open(scenarioToLoad.c_str(), ios::binary);

        if (!scenario_file.good())
        {
            MessageBox(NULL, "Failed to open the Scenario file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
            return false;
        }

        scenario_file.seekg(5, ios::beg);

        scenario_file.read(reinterpret_cast<char*>(&game_lives), 1);
    }

    if (loadingType < 2 && !nextLevel)
    {
        if (loadingType == 0)
            level_file.open(fileToLoad.c_str(), ios::binary);
        else
            level_file.open(scenarioToLoad.c_str(), ios::binary);

        if (!level_file.good())
        {
            if (loadingType != 1)
                MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
            else
                MessageBox(NULL, "Failed to open the Scenario file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);

            return false;
        }
    }

    if (loadingType == 0)
        level_file.seekg(4, ios::beg);
    else if (loadingType == 1)
    {
        if (nextLevel)
        {
            char c;

            level_file.seekg(1, ios::cur);
            level_file.read(&c, 1);

            #ifdef DEBUGMODE
            cout << "LoAd " << c << endl;
            #endif // DEBUGMODE

            if (c != 'C')
                return false;

            level_file.seekg(3, ios::cur);
        }
        else
        {
            level_file.seekg(5, ios::beg);

            level_file.read(reinterpret_cast<char*>(&game_lives), 1);

            level_file.seekg(4, ios::cur);
        }
    }
    else if (loadingType == 2)
    {
        TCHAR path[MAX_PATH];
        TCHAR temp[MAX_PATH];
        TCHAR c;
        register short i = 0;

        do
        {
            scenario_file.read(&c, 1);
            path[i] = c;

            if (scenario_file.eof())
                return false;

            i++;
        } while (c != '\0');

        // Set the current directory :
        StrCpy(temp, scenarioToLoad.c_str());

        PathRemoveFileSpec(temp);
        SetCurrentDirectory(temp);

        level_file.open(path, ios::binary);

        if (!level_file.good())
        {
            MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
            return false;
        }

        fileToLoad = path;

        // Reset the current directory :
        GetModuleFileName(NULL, temp, MAX_PATH);
        PathRemoveFileSpec(temp);
        SetCurrentDirectory(temp);
    }
    else
    {
        TCHAR temp[MAX_PATH];

        // Set the current directory :
        StrCpy(temp, scenarioToLoad.c_str());

        PathRemoveFileSpec(temp);
        SetCurrentDirectory(temp);

        level_file.open(fileToLoad, ios::binary);

        if (!level_file.good())
        {
            MessageBox(NULL, "Failed to open the Level file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
            return false;
        }

        // Reset the current directory :
        GetModuleFileName(NULL, temp, MAX_PATH);
        PathRemoveFileSpec(temp);
        SetCurrentDirectory(temp);
    }

    {
        char lives[3]("");
        sprintf(lives, "%u", game_lives);

        livesFont = new SpriteFont(Vector2f(138, 16), false, lives);
    }

    {
        TCHAR filePath[MAX_PATH];
        TCHAR getString[MAX_PATH];

        if (loadingType == 1)
            strcpy(filePath, scenarioToLoad.c_str());
        else
            strcpy(filePath, fileToLoad.c_str());

        if (loadingType >= 2)
        {
            strcpy(getString, scenarioToLoad.c_str());

            PathRemoveFileSpec(getString);
            SetCurrentDirectory(getString);

            // Offset the cursor if needed :
            level_file.seekg(4, ios::cur);
        }

        PathRemoveFileSpec(filePath);
        SetCurrentDirectory(filePath);

        #ifdef DEBUGMODE
        cout << "Dir : " << filePath << endl;
        #endif // DEBUGMODE

        for (register unsigned int i = 0; i < 5; i++)
        {
            level_file.read(getString, 1);

            if (getString[0] != '\0')
            {
                level_file.seekg(-1, ios::cur);

                for (register unsigned int j = 0; true; j++)
                {
                    level_file.read(&getString[j], 1);

                    if (getString[j] == '\0')
                        break;
                }

                #ifdef DEBUGMODE
                cout << "Dir : " << getString << endl;
                #endif // DEBUGMODE

                switch (i)
                {
                    case 0 :
                        FMOD_System_CreateStream(soundSystem, getString, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[22]);

                        break;
                    case 1 :
                        backgroundTxt[12] = new Texture;
                        backgroundTxt[12]->loadFromFile(getString);
                        backgroundTxt[12]->setRepeated(true);

                        break;
                    case 2 :
                        FMOD_System_CreateStream(soundSystem, getString, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[23]);

                        break;
                    case 3 :
                        backgroundTxt[13] = new Texture;
                        backgroundTxt[13]->loadFromFile(getString);
                        backgroundTxt[13]->setRepeated(true);

                        break;
                    case 4 :
                        FMOD_System_CreateStream(soundSystem, getString, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[24]);

                        break;
                }
            }
        }

        // Reset the current directory :
        GetModuleFileName(NULL, filePath, MAX_PATH);
        PathRemoveFileSpec(filePath);
        SetCurrentDirectory(filePath);
    }

    level_file.read(reinterpret_cast<char*>(&levelScale.x), 4);
    level_file.read(reinterpret_cast<char*>(&levelScale.y), 4);
    level_file.read(reinterpret_cast<char*>(&levelScaleb.x), 4);
    level_file.read(reinterpret_cast<char*>(&levelScaleb.y), 4);

    collisionMatrix = new Matrix(levelScale.x * 20, levelScale.y * 15);
    collisionMatrixb = new Matrix(levelScaleb.x * 20, levelScaleb.y * 15);

    {
        char value;

        level_file.read(&value, 1);
        musicZonea = &musicSamples[value];

        level_file.read(&value, 1);
        musicZoneb = &musicSamples[value];
    }

    {
        char value;

        level_file.read(&value, 1);

        switch (value)
        {
            case 0 : break;
            case 1 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 274));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 274);
                backgroundSpr->setTexture(backgroundTxt[0]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 274));
                break;
            case 2 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 274));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 274);
                backgroundSpr->setTexture(backgroundTxt[1]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 274));
                break;
            case 3 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 274));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 274);
                backgroundSpr->setTexture(backgroundTxt[2]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 274));
                break;
            case 4 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 178));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 178);
                backgroundSpr->setTexture(backgroundTxt[3]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 178));
                break;
            case 5 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 165));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 165);
                backgroundSpr->setTexture(backgroundTxt[4]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 165));
                break;
            case 6 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 101));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 101);
                backgroundSpr->setTexture(backgroundTxt[5]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 101));
                break;
            case 7 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 320));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 320);
                backgroundSpr->setTexture(backgroundTxt[6]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 320));
                break;
            case 8 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 320));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 320);
                backgroundSpr->setTexture(backgroundTxt[7]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 320));
                break;
            case 9 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                backgroundSpr->setTexture(backgroundTxt[8]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 10 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                backgroundSpr->setTexture(backgroundTxt[9]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 11 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                backgroundSpr->setTexture(backgroundTxt[10]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 12 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                backgroundSpr->setTexture(backgroundTxt[11]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 13 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                backgroundSpr->setTexture(backgroundTxt[12]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 14 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 274));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 274);
                backgroundSpr->setTexture(backgroundTxt[14]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 274));
                break;
            case 15 :
                backgroundSpr = new RectangleShape(Vector2f(levelScale.x * 640, 274));
                backgroundSpr->setPosition(0, (levelScale.y * 480) - 274);
                backgroundSpr->setTexture(backgroundTxt[15]);
                backgroundSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, 274));
                break;
        }
    }

    {
        char value;

        level_file.read(&value, 1);

        switch (value)
        {
            case 0 : break;
            case 1 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 274));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 274);
                backgroundSprb->setTexture(backgroundTxt[0]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 274));
                break;
            case 2 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 274));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 274);
                backgroundSprb->setTexture(backgroundTxt[1]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 274));
                break;
            case 3 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 274));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 274);
                backgroundSprb->setTexture(backgroundTxt[2]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 274));
                break;
            case 4 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 178));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 178);
                backgroundSprb->setTexture(backgroundTxt[3]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 178));
                break;
            case 5 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 165));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 165);
                backgroundSprb->setTexture(backgroundTxt[4]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 165));
                break;
            case 6 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 101));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 101);
                backgroundSprb->setTexture(backgroundTxt[5]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 101));
                break;
            case 7 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 320));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 320);
                backgroundSprb->setTexture(backgroundTxt[6]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 320));
                break;
            case 8 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 320));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 320);
                backgroundSprb->setTexture(backgroundTxt[7]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 320));
                break;
            case 9 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                backgroundSprb->setTexture(backgroundTxt[8]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 10 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                backgroundSprb->setTexture(backgroundTxt[9]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 11 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                backgroundSprb->setTexture(backgroundTxt[10]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 12 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                backgroundSprb->setTexture(backgroundTxt[11]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 13 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                backgroundSprb->setTexture(backgroundTxt[13]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 14 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 274));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 274);
                backgroundSprb->setTexture(backgroundTxt[14]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 274));
                break;
            case 15 :
                backgroundSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, 274));
                backgroundSprb->setPosition(0, (levelScaleb.y * 480) - 274);
                backgroundSprb->setTexture(backgroundTxt[15]);
                backgroundSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 274));
                break;
        }
    }

    {
        Color topcolor;
        Color bottomcolor;

        char value;

        level_file.read(reinterpret_cast<char*>(&topcolor.r), 1);
        level_file.read(reinterpret_cast<char*>(&topcolor.g), 1);
        level_file.read(reinterpret_cast<char*>(&topcolor.b), 1);
        level_file.read(reinterpret_cast<char*>(&bottomcolor.r), 1);
        level_file.read(reinterpret_cast<char*>(&bottomcolor.g), 1);
        level_file.read(reinterpret_cast<char*>(&bottomcolor.b), 1);

        level_file.read(&value, 1);

        switch (value)
        {
            case 0 :
                gradientRectangle[0].position = Vector2f(0, 0);
                gradientRectangle[1].position = Vector2f(levelScale.x * 640, 0);
                gradientRectangle[2].position = Vector2f(levelScale.x * 640, levelScale.y * 480);
                gradientRectangle[3].position = Vector2f(0, levelScale.y * 480);
                clearColora = false;
                break;
            case 1 :
                gradientRectangle[0].position = Vector2f(0, 0);
                gradientRectangle[1].position = Vector2f(levelScale.x * 640, 0);
                gradientRectangle[2].position = Vector2f(levelScale.x * 640, 480);
                gradientRectangle[3].position = Vector2f(0, 480);
                levelBackColor = bottomcolor;
                clearColora = true;
                break;
            case 2 :
                gradientRectangle[0].position = Vector2f(0, (levelScale.y * 480) - 480);
                gradientRectangle[1].position = Vector2f(levelScale.x * 640, (levelScale.y * 480) - 480);
                gradientRectangle[2].position = Vector2f(levelScale.x * 640, levelScale.y * 480);
                gradientRectangle[3].position = Vector2f(0, levelScale.y * 480);
                levelBackColor = topcolor;
                clearColora = true;
                break;
        }

        gradientRectangle[0].color = topcolor;
        gradientRectangle[1].color = topcolor;
        gradientRectangle[2].color = bottomcolor;
        gradientRectangle[3].color = bottomcolor;
    }

    {
        Color topcolor;
        Color bottomcolor;

        char value;

        level_file.read(reinterpret_cast<char*>(&topcolor.r), 1);
        level_file.read(reinterpret_cast<char*>(&topcolor.g), 1);
        level_file.read(reinterpret_cast<char*>(&topcolor.b), 1);
        level_file.read(reinterpret_cast<char*>(&bottomcolor.r), 1);
        level_file.read(reinterpret_cast<char*>(&bottomcolor.g), 1);
        level_file.read(reinterpret_cast<char*>(&bottomcolor.b), 1);

        level_file.read(&value, 1);

        switch (value)
        {
            case 0 :
                gradientRectangleb[0].position = Vector2f(0, 0);
                gradientRectangleb[1].position = Vector2f(levelScaleb.x * 640, 0);
                gradientRectangleb[2].position = Vector2f(levelScaleb.x * 640, levelScaleb.y * 480);
                gradientRectangleb[3].position = Vector2f(0, levelScaleb.y * 480);
                clearColorb = false;
                break;
            case 1 :
                gradientRectangleb[0].position = Vector2f(0, 0);
                gradientRectangleb[1].position = Vector2f(levelScaleb.x * 640, 0);
                gradientRectangleb[2].position = Vector2f(levelScaleb.x * 640, 480);
                gradientRectangleb[3].position = Vector2f(0, 480);
                levelBackColorb = bottomcolor;
                clearColorb = true;
                break;
            case 2 :
                gradientRectangleb[0].position = Vector2f(0, (levelScaleb.y * 480) - 480);
                gradientRectangleb[1].position = Vector2f(levelScaleb.x * 640, (levelScaleb.y * 480) - 480);
                gradientRectangleb[2].position = Vector2f(levelScaleb.x * 640, levelScaleb.y * 480);
                gradientRectangleb[3].position = Vector2f(0, levelScaleb.y * 480);
                levelBackColorb = topcolor;
                clearColorb = true;
                break;
        }

        gradientRectangleb[0].color = topcolor;
        gradientRectangleb[1].color = topcolor;
        gradientRectangleb[2].color = bottomcolor;
        gradientRectangleb[3].color = bottomcolor;
    }

    {
        char valueb;

        level_file.read(&effectWeatherType, 1);
        level_file.read(&valueb, 1);

        switch (effectWeatherType)
        {
            case 0 : break;
            case 1 :
                effectWeatherSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                effectWeatherSpr->setFillColor(Color(255, 255, 255, 128));
                effectWeatherSpr->setTexture(effectTxt[3]);
                effectWeatherSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 2 :
                effectWeatherSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                effectWeatherSpr->setFillColor(Color(255, 255, 255, 128));
                effectWeatherSpr->setTexture(effectTxt[3]);
                effectWeatherSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
            case 3 :
                effectWeatherSpr = new RectangleShape(Vector2f(levelScale.x * 640, levelScale.y * 480));
                effectWeatherSpr->setTexture(effectTxt[5]);
                effectWeatherSpr->setTextureRect(IntRect(0, 0, levelScale.x * 640, levelScale.y * 480));
                break;
        }

        switch (valueb)
        {
            case 0 : break;
            case 1 :
                effectCloudsSpr[0] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[0]->setTexture(effectTxt[0]);
                effectCloudsSpr[0]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                break;
            case 2 :
                effectCloudsSpr[0] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[0]->setTexture(effectTxt[2]);
                effectCloudsSpr[0]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                break;
            case 3 :
                effectCloudsSpr[0] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[0]->setTexture(effectTxt[1]);
                effectCloudsSpr[0]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                break;
            case 4 :
                effectCloudsSpr[0] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[0]->setTexture(effectTxt[0]);
                effectCloudsSpr[0]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                effectCloudsSpr[1] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[1]->setPosition(0, 24);
                effectCloudsSpr[1]->setTexture(effectTxt[0]);
                effectCloudsSpr[1]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                break;
            case 5 :
                effectCloudsSpr[0] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[0]->setTexture(effectTxt[2]);
                effectCloudsSpr[0]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                effectCloudsSpr[1] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[1]->setPosition(0, 24);
                effectCloudsSpr[1]->setTexture(effectTxt[2]);
                effectCloudsSpr[1]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                break;
            case 6 :
                effectCloudsSpr[0] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[0]->setTexture(effectTxt[1]);
                effectCloudsSpr[0]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                effectCloudsSpr[1] = new RectangleShape(Vector2f(levelScale.x * 640, 63));
                effectCloudsSpr[1]->setPosition(0, 24);
                effectCloudsSpr[1]->setTexture(effectTxt[1]);
                effectCloudsSpr[1]->setTextureRect(IntRect(0, 0, levelScale.x * 640, 63));
                break;
        }
    }

    {
        char valueb;

        level_file.read(&effectWeatherTypeb, 1);
        level_file.read(&valueb, 1);

        switch (effectWeatherTypeb)
        {
            case 0 : break;
            case 1 :
                effectWeatherSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                effectWeatherSprb->setFillColor(Color(255, 255, 255, 128));
                effectWeatherSprb->setTexture(effectTxt[3]);
                effectWeatherSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 2 :
                effectWeatherSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                effectWeatherSprb->setFillColor(Color(255, 255, 255, 128));
                effectWeatherSprb->setTexture(effectTxt[3]);
                effectWeatherSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
            case 3 :
                effectWeatherSprb = new RectangleShape(Vector2f(levelScaleb.x * 640, levelScaleb.y * 480));
                effectWeatherSprb->setTexture(effectTxt[5]);
                effectWeatherSprb->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, levelScaleb.y * 480));
                break;
        }

        switch (valueb)
        {
            case 0 : break;
            case 1 :
                effectCloudsSprb[0] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[0]->setTexture(effectTxt[0]);
                effectCloudsSprb[0]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                break;
            case 2 :
                effectCloudsSprb[0] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[0]->setTexture(effectTxt[2]);
                effectCloudsSprb[0]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                break;
            case 3 :
                effectCloudsSprb[0] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[0]->setTexture(effectTxt[1]);
                effectCloudsSprb[0]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                break;
            case 4 :
                effectCloudsSprb[0] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[0]->setTexture(effectTxt[0]);
                effectCloudsSprb[0]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                effectCloudsSprb[1] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[1]->setPosition(0, 24);
                effectCloudsSprb[1]->setTexture(effectTxt[0]);
                effectCloudsSprb[1]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                break;
            case 5 :
                effectCloudsSprb[0] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[0]->setTexture(effectTxt[2]);
                effectCloudsSprb[0]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                effectCloudsSprb[1] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[1]->setPosition(0, 24);
                effectCloudsSprb[1]->setTexture(effectTxt[2]);
                effectCloudsSprb[1]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                break;
            case 6 :
                effectCloudsSprb[0] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[0]->setTexture(effectTxt[1]);
                effectCloudsSprb[0]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                effectCloudsSprb[1] = new RectangleShape(Vector2f(levelScaleb.x * 640, 63));
                effectCloudsSprb[1]->setPosition(0, 24);
                effectCloudsSprb[1]->setTexture(effectTxt[1]);
                effectCloudsSprb[1]->setTextureRect(IntRect(0, 0, levelScaleb.x * 640, 63));
                break;
        }
    }

    level_file.read(reinterpret_cast<char*>(&startZone), 1);
    level_file.read(reinterpret_cast<char*>(&gravity), 1);

    {
        unsigned short tmp;
        level_file.read(reinterpret_cast<char*>(&tmp), 2);

        timer = static_cast<unsigned int>(tmp) * 25;
        levelTimer = timer;
    }

    level_file.read(reinterpret_cast<char*>(&autoscroll), 1);

    {
        float speed;

        level_file.read(reinterpret_cast<char*>(&speed), 4);

        autoscrollSpeed = fma((speed - 1.0f), 0.8f, 1.0f);
    }

    mario_gravity = gravity / 7.0f;
    sprite_gravity = gravity / 14.0f;
    star_gravity = gravity / 28.0f;

    {
        unsigned char nodeSize;

        level_file.read(reinterpret_cast<char*>(&nodeSize), 1);

        if (nodeSize != 0)
        {
            for (register unsigned char i = 0; i < nodeSize; i++)
            {
                Vector2f currentNode;

                level_file.read(reinterpret_cast<char*>(&currentNode.x), 4);
                level_file.read(reinterpret_cast<char*>(&currentNode.y), 4);

                autoscrollPath.emplace_back(Vector2f(currentNode.x + 16, currentNode.y + 16));
            }
        }
    }

    level_file.read(reinterpret_cast<char*>(&alphaBack), 1);
    level_file.read(reinterpret_cast<char*>(&alphaFront), 1);

    level_file.read(reinterpret_cast<char*>(&alphaBackb), 1);
    level_file.read(reinterpret_cast<char*>(&alphaFrontb), 1);

    {
        char type;
        float height;

        level_file.read(reinterpret_cast<char*>(&type), 1);
        level_file.read(reinterpret_cast<char*>(&height), 4);

        liquidStartHeight = height;
        liquidTargetHeight = height;

        if (type == 1)
            watera = new Mark_WaterPlane(effectTxt[7], height, levelScale);
        else if (type == 2)
            watera = new Mark_LavaPlane(hazardsTxt[0], height, levelScale);
    }

    {
        unsigned char spotSize;

        level_file.read(reinterpret_cast<char*>(&spotSize), 1);

        if (spotSize != 0)
        {
            for (register unsigned char i = 0; i < spotSize; i++)
            {
                TriggerData* currentData = new TriggerData;

                level_file.read(reinterpret_cast<char*>(currentData), sizeof(TriggerData));

                liquidSpots.emplace_back(currentData);
            }
        }
    }

    {
        char type;
        float height;

        level_file.read(reinterpret_cast<char*>(&type), 1);
        level_file.read(reinterpret_cast<char*>(&height), 4);

        liquidStartHeightb = height;
        liquidTargetHeightb = height;

        if (type == 1)
            waterb = new Mark_WaterPlane(effectTxt[7], height, levelScaleb);
        else if (type == 2)
            waterb = new Mark_LavaPlane(hazardsTxt[0], height, levelScaleb);
    }

    {
        unsigned char spotSize;

        level_file.read(reinterpret_cast<char*>(&spotSize), 1);

        if (spotSize != 0)
        {
            for (register unsigned char i = 0; i < spotSize; i++)
            {
                TriggerData* currentData = new TriggerData;

                level_file.read(reinterpret_cast<char*>(currentData), sizeof(TriggerData));

                liquidSpotsb.emplace_back(currentData);
            }
        }
    }

    level_file.read(reinterpret_cast<char*>(&liquidSpeed), 4);

    {
        unsigned char temp[6];

        level_file.read(reinterpret_cast<char*>(&temp[0]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[1]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[2]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[3]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[4]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[5]), 1);

        for (register unsigned char i = 0; i < 6; i++)
        {
            if (temp[i] == 0)
                koopaAttacks[i] = false;
            else
                koopaAttacks[i] = true;
        }

        level_file.read(reinterpret_cast<char*>(&temp[0]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[1]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[2]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[3]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[4]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[5]), 1);

        koopaFrequences[0] = 225 - (temp[0] - 5) * 14;
        koopaFrequences[1] = 85 - (temp[1] - 5) * 2;
        koopaFrequences[2] = 250 - (temp[2] - 10);
        koopaFrequences[3] = 350 - (temp[3] - 10) * 2;
        koopaFrequences[4] = 305 - (temp[4] - 10) * 2;
        koopaFrequences[5] = 200 - (temp[5] - 10) * 2;

        level_file.read(reinterpret_cast<char*>(&temp[0]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[1]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[2]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[3]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[4]), 1);
        level_file.read(reinterpret_cast<char*>(&temp[5]), 1);

        koopaSpeeds[0] = fma((temp[0] - 5.0f), 0.25f, 6.0f);
        koopaSpeeds[1] = fma((temp[1] - 1.0f), (4.0f) / (9.0f), 2.0f);
        koopaSpeeds[2] = fma((temp[2] - 5.0f), (5.5f) / (15.0f), 2.5f);
        koopaSpeeds[3] = fma((temp[3] - 5.0f), (5.5f) / (15.0f), 2.5f);
        koopaSpeeds[4] = fma((temp[4] - 5.0f), (4.0f) / (15.0f), 2.0f);
        koopaSpeeds[5] = fma((temp[5] - 5.0f), (5.5f) / (15.0f), 2.5f);

        level_file.read(reinterpret_cast<char*>(&temp[0]), 1);

        koopaTrail = 2 + (20 - temp[0]);
    }

    level_file.read(reinterpret_cast<char*>(&koopaHealth), 1);
    level_file.read(reinterpret_cast<char*>(&koopaMusic), 1);

    koopaHealth_saved = koopaHealth;

    #ifdef DEBUGMODE
    cout << "Bowser music : " << static_cast<unsigned int>(koopaMusic) << endl;
    #endif // DEBUGMODE

    {
        TCHAR getString[MAX_PATH];

        level_file.read(getString, 1);

        if (getString[0] != '\0')
        {
            level_file.seekg(-1, ios::cur);

            for (register unsigned int j = 0; true; j++)
            {
                level_file.read(&getString[j], 1);

                if (getString[j] == '\0')
                    break;
            }

            #ifdef DEBUGMODE
            cout << getString << endl;
            #endif // DEBUGMODE

            worldFont = new SpriteFont(Vector2f(0, 0), false, getString);
        }
        else
        {
            worldFont = new SpriteFont(Vector2f(0, 0), false, "");
            worldNamePos = Vector2f(-1, -1);
        }
    }

    {
        TCHAR getString[MAX_PATH];

        level_file.read(getString, 1);

        if (getString[0] != '\0')
        {
            level_file.seekg(-1, ios::cur);

            for (register unsigned int j = 0; true; j++)
            {
                level_file.read(&getString[j], 1);

                if (getString[j] == '\0')
                    break;
            }
        }
    }

    {
        unsigned int listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 4);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TileData currentData;

                level_file.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                tilemapLayer1.emplace_back(Vertex(Vector2f(currentData.x, currentData.y), Color(255, 255, 255, alphaBack), Vector2f(currentData.tilex * 32, currentData.tiley * 32)));
                tilemapLayer1.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y), Color(255, 255, 255, alphaBack), Vector2f((currentData.tilex + 1) * 32, currentData.tiley * 32)));
                tilemapLayer1.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y + 32), Color(255, 255, 255, alphaBack), Vector2f((currentData.tilex + 1) * 32, (currentData.tiley + 1) * 32)));
                tilemapLayer1.emplace_back(Vertex(Vector2f(currentData.x, currentData.y + 32), Color(255, 255, 255, alphaBack), Vector2f(currentData.tilex * 32, (currentData.tiley + 1) * 32)));
            }
        }
    }

    {
        unsigned int listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 4);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TileData currentData;

                level_file.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                tilemapLayer2.emplace_back(Vertex(Vector2f(currentData.x, currentData.y), Color::White, Vector2f(currentData.tilex * 32, currentData.tiley * 32)));
                tilemapLayer2.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y), Color::White, Vector2f((currentData.tilex + 1) * 32, currentData.tiley * 32)));
                tilemapLayer2.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y + 32), Color::White, Vector2f((currentData.tilex + 1) * 32, (currentData.tiley + 1) * 32)));
                tilemapLayer2.emplace_back(Vertex(Vector2f(currentData.x, currentData.y + 32), Color::White, Vector2f(currentData.tilex * 32, (currentData.tiley + 1) * 32)));

                collisionMatrix->setValue(currentData.x / 32, currentData.y / 32, 1);
            }
        }
    }

    {
        unsigned int listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 4);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TileData currentData;

                level_file.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                tilemapLayer3.emplace_back(Vertex(Vector2f(currentData.x, currentData.y), Color(255, 255, 255, alphaFront), Vector2f(currentData.tilex * 32, currentData.tiley * 32)));
                tilemapLayer3.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y), Color(255, 255, 255, alphaFront), Vector2f((currentData.tilex + 1) * 32, currentData.tiley * 32)));
                tilemapLayer3.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y + 32), Color(255, 255, 255, alphaFront), Vector2f((currentData.tilex + 1) * 32, (currentData.tiley + 1) * 32)));
                tilemapLayer3.emplace_back(Vertex(Vector2f(currentData.x, currentData.y + 32), Color(255, 255, 255, alphaFront), Vector2f(currentData.tilex * 32, (currentData.tiley + 1) * 32)));
            }
        }
    }

    {
        unsigned int listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 4);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TileData currentData;

                level_file.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                tilemapLayer1b.emplace_back(Vertex(Vector2f(currentData.x, currentData.y), Color(255, 255, 255, alphaBackb), Vector2f(currentData.tilex * 32, currentData.tiley * 32)));
                tilemapLayer1b.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y), Color(255, 255, 255, alphaBackb), Vector2f((currentData.tilex + 1) * 32, currentData.tiley * 32)));
                tilemapLayer1b.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y + 32), Color(255, 255, 255, alphaBackb), Vector2f((currentData.tilex + 1) * 32, (currentData.tiley + 1) * 32)));
                tilemapLayer1b.emplace_back(Vertex(Vector2f(currentData.x, currentData.y + 32), Color(255, 255, 255, alphaBackb), Vector2f(currentData.tilex * 32, (currentData.tiley + 1) * 32)));
            }
        }
    }

    {
        unsigned int listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 4);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TileData currentData;

                level_file.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                tilemapLayer2b.emplace_back(Vertex(Vector2f(currentData.x, currentData.y), Color::White, Vector2f(currentData.tilex * 32, currentData.tiley * 32)));
                tilemapLayer2b.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y), Color::White, Vector2f((currentData.tilex + 1) * 32, currentData.tiley * 32)));
                tilemapLayer2b.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y + 32), Color::White, Vector2f((currentData.tilex + 1) * 32, (currentData.tiley + 1) * 32)));
                tilemapLayer2b.emplace_back(Vertex(Vector2f(currentData.x, currentData.y + 32), Color::White, Vector2f(currentData.tilex * 32, (currentData.tiley + 1) * 32)));

                collisionMatrixb->setValue(currentData.x / 32, currentData.y / 32, 1);
            }
        }
    }

    {
        unsigned int listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 4);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                TileData currentData;

                level_file.read(reinterpret_cast<char*>(&currentData), sizeof(TileData));

                tilemapLayer3b.emplace_back(Vertex(Vector2f(currentData.x, currentData.y), Color(255, 255, 255, alphaFrontb), Vector2f(currentData.tilex * 32, currentData.tiley * 32)));
                tilemapLayer3b.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y), Color(255, 255, 255, alphaFrontb), Vector2f((currentData.tilex + 1) * 32, currentData.tiley * 32)));
                tilemapLayer3b.emplace_back(Vertex(Vector2f(currentData.x + 32, currentData.y + 32), Color(255, 255, 255, alphaFrontb), Vector2f((currentData.tilex + 1) * 32, (currentData.tiley + 1) * 32)));
                tilemapLayer3b.emplace_back(Vertex(Vector2f(currentData.x, currentData.y + 32), Color(255, 255, 255, alphaFrontb), Vector2f(currentData.tilex * 32, (currentData.tiley + 1) * 32)));
            }
        }
    }

    spriteDefStart = level_file.tellg();

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 92 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_TREE)); originPos = Vector2f(-1, 32); break;
                    case 93 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 94 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[0])); originPos = Vector2f(16, -2); break;
                    case 95 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_TREE)); originPos = Vector2f(-1, 32); break;
                    case 96 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 97 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[1])); originPos = Vector2f(16, -2); break;
                    case 98 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_TREE)); originPos = Vector2f(-1, 32); break;
                    case 99 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 100 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[2])); originPos = Vector2f(16, -2); break;
                    case 101 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_TREE)); originPos = Vector2f(-1, 32); break;
                    case 102 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 103 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[3])); originPos = Vector2f(16, -2); break;
                    case 104 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_TREE)); originPos = Vector2f(-1, 32); break;
                    case 105 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 106 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[4])); originPos = Vector2f(16, -2); break;
                    case 107 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_FENCE)); originPos = Vector2f(64, 0); break;
                    case 108 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_SNOWFENCE)); originPos = Vector2f(64, 0); break;
                    case 109 : listPlaceables.emplace_back(new Scenery_Cloud(*sceneriesTxt[5])); originPos = Vector2f(15, 8); break;
                    case 110 : listPlaceables.emplace_back(new Scenery_Cloud(*sceneriesTxt[6])); originPos = Vector2f(15, 8); break;
                    case 111 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[7], IntRect(0, 0, 127, 160))); originPos = Vector2f(47, 128); break;
                    case 112 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[8], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 113 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[9], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                    case 229 : listPlaceables.emplace_back(new Scenery_Waterfall(*sceneriesTxt[10])); originPos = Vector2f(31, -5); break;
                    case 230 : listPlaceables.emplace_back(new Scenery_Scroll(*sceneriesTxt[11])); originPos = Vector2f(11, 0); break;
                    case 231 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[12], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 232 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[13], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                }

                listPlaceables.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                vector<float> entData;

                unsigned char create = 0;

                float currentData[3];

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 0 : startPos = Vector2f(entPos.x, entPos.y); break;
                    case 1 : listMiddle.emplace_back(new Mark_CheckPoint(*marksTxt[7], false)); originPos = Vector2f(28, 79); create = 2; break;
                    case 2 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listPlaceables.emplace_back(new Mark_LevelEnd(*marksTxt[8], *marksTxt[9], *effectTxt[6], currentData[0]));
                        originPos = Vector2f(-2, 256);
                        create = 1;
                        break;
                    case 3 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedMoveH(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        #ifdef DEBUGMODE
                        cout << entPos.x << " : " << entPos.y << endl;
                        #endif // DEBUGMODE

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 4 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedTouchH(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 5 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedMoveV(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 6 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedTouchV(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 7 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedFall(*marksTxt[0], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 8 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortMoveH(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 9 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortTouchH(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 10 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortMoveV(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 11 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortTouchV(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 12 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortFall(*marksTxt[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 13 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudMoveH(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 14 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudTouchH(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 15 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudMoveV(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 16 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudTouchV(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 17 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudFall(*marksTxt[2], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 18 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneMoveH(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 19 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneTouchH(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 20 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneMoveV(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 21 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneTouchV(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 22 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneFall(*marksTxt[3], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 23 : listMiddle.emplace_back(new Mark_Spring(*marksTxt[4], 20)); originPos = Vector2f(0, 32); create = 2; break;
                    case 24 : listMiddle.emplace_back(new Mark_Spring(*marksTxt[5], 28)); originPos = Vector2f(0, 32); create = 2; break;
                    case 25 : listMiddle.emplace_back(new Mark_Spring(*marksTxt[6], 36)); originPos = Vector2f(0, 32); create = 2; break;
                    case 114 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_Goomba(*enemiesTxt[0], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 115 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_GoombaGrey(*enemiesTxt[1], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 116 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_Spiny(*enemiesTxt[2], listCollider.back())); originPos = Vector2f(-17, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 117 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_SpinyGrey(*enemiesTxt[3], listCollider.back())); originPos = Vector2f(-17, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 118 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_BuzzyBeetle(*enemiesTxt[4], enemiesTxt[13], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 119 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 120 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 121 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 122 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 123 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 124 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 125 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 126 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 127 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaGreenH(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 128 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaRedH(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 129 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaBlueH(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 130 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaGreenV(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 131 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaRedV(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 132 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaBlueV(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 133 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaGreenJ(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 134 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaRedJ(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 135 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaBlueJ(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 136 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaYellow(*enemiesTxt[12], enemiesTxt[8], enemiesTxt[17], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 137 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenT(enemiesTxt[18], listCollider.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 138 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenL(enemiesTxt[18], listCollider.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 139 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenR(enemiesTxt[18], listCollider.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 140 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenB(enemiesTxt[18], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 141 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireT(enemiesTxt[19], listCollider.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 142 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireL(enemiesTxt[19], listCollider.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 143 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireR(enemiesTxt[19], listCollider.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 144 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireB(enemiesTxt[19], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 145 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyT(enemiesTxt[20], listCollider.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 146 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyL(enemiesTxt[20], listCollider.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 147 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyR(enemiesTxt[20], listCollider.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 148 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyB(enemiesTxt[20], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 149 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepRed(*enemiesTxt[21], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 150 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepGreen(*enemiesTxt[22], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 151 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepBlue(*enemiesTxt[23], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 152 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepYellow(*enemiesTxt[24], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 153 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddle.emplace_back(new Sprite_HammerBro(*enemiesTxt[25], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 154 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddle.emplace_back(new Sprite_SilverBro(*enemiesTxt[26], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 155 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddle.emplace_back(new Sprite_FireBro(*enemiesTxt[27], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 156 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddle.emplace_back(new Sprite_BeetBro(*enemiesTxt[28], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 157 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddle.emplace_back(new Sprite_SpikeBro(*enemiesTxt[29], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 158 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_LakituWhite(*enemiesTxt[32], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 159 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_LakituDark(*enemiesTxt[33], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 160 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_LakituGold(*enemiesTxt[34], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 161 : listMiddle.emplace_back(new Sprite_Poison(*enemiesTxt[37])); originPos = Vector2f(0, 0); create = 2; break;
                    case 162 :
                        listPlaceables.emplace_back(new Bonus_PoisonBlock(itemsTxt[0])); originPos = Vector2f(0, 0); create = 1;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 163 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_PoisonBlockInv(itemsTxt[0], listCollider.back())); originPos = Vector2f(0, 0); create = 1;
                        break;
                    case 164 : listMiddle.emplace_back(new Sprite_ThwompB(*enemiesTxt[38])); originPos = Vector2f(10, 4); create = 2; break;
                    case 165 : listMiddle.emplace_back(new Sprite_ThwompT(*enemiesTxt[38])); originPos = Vector2f(10, -36); create = 2; break;
                    case 166 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 42, entPos.y - 23), Vector2f(52, 55)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_Bowser(*enemiesTxt[39], listCollider.back())); originPos = Vector2f(16, 40); create = 2;
                        break;
                    case 167 : listPlaceables.emplace_back(new Hazard_LavaT(*hazardsTxt[0])); originPos = Vector2f(0, 2); create = 1; break;
                    case 168 : listPlaceables.emplace_back(new Hazard_LavaB(*hazardsTxt[0])); originPos = Vector2f(0, -34); create = 1; break;
                    case 169 : listPlaceables.emplace_back(new Hazard_LavaL(*hazardsTxt[0])); originPos = Vector2f(2, 0); create = 1; break;
                    case 170 : listPlaceables.emplace_back(new Hazard_LavaR(*hazardsTxt[0])); originPos = Vector2f(-34, 0); create = 1; break;
                    case 171 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(928, 96)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(960, 96)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(960, 128)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(928, 128)));
                        create = 0;
                        break;
                    case 172 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooT(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 173 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooB(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 174 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooL(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 175 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooR(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 176 : listMiddle.emplace_back(new Hazard_PodobooFountainT); originPos = Vector2f(0, 0); create = 2; break;
                    case 177 : listMiddle.emplace_back(new Hazard_PodobooFountainB); originPos = Vector2f(0, 0); create = 2; break;
                    case 178 : listMiddle.emplace_back(new Hazard_PodobooFountainL); originPos = Vector2f(0, 0); create = 2; break;
                    case 179 : listMiddle.emplace_back(new Hazard_PodobooFountainR); originPos = Vector2f(0, 0); create = 2; break;
                    case 180 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 96))); originPos = Vector2f(0, 0); create = 1; break;
                    case 181 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 192))); originPos = Vector2f(0, 0); create = 1; break;
                    case 182 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 128))); originPos = Vector2f(0, 0); create = 1; break;
                    case 183 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 160))); originPos = Vector2f(0, 0); create = 1; break;
                    case 184 :
                        listMiddle.emplace_back(new Hazard_CannonHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 185 :
                        listMiddle.emplace_back(new Hazard_CannonHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 186 :
                        listMiddle.emplace_back(new Hazard_CannonVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 187 :
                        listMiddle.emplace_back(new Hazard_CannonVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 188 :
                        listMiddle.emplace_back(new Hazard_CannonHomingHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 189 :
                        listMiddle.emplace_back(new Hazard_CannonHomingHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 190 :
                        listMiddle.emplace_back(new Hazard_CannonHomingVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 191 :
                        listMiddle.emplace_back(new Hazard_CannonHomingVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2;
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 192 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(608, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(640, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(640, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(608, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 193 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(576, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(608, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(608, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(576, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 194 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(640, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(672, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(672, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(640, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 195 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(704, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(736, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(736, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(704, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 196 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(672, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(704, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(704, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(672, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 197 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(768, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(800, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(800, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(768, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 198 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(736, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(768, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(768, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(736, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 199 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(800, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(832, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(832, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(800, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 200 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(864, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(896, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(896, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(864, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 201 :
                        tilemapLayer2.emplace_back(Vertex(entPos, Color::White, Vector2f(832, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(864, 224)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(864, 256)));
                        tilemapLayer2.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(832, 256)));
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 202 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 203 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 204 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscRCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 205 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscRC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 206 :
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrix->setValue((entPos.x/32) + 1, entPos.y/32, 1);
                        listMiddle.emplace_back(new Hazard_LavalauncherL(*hazardsTxt[5])); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 207 :
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrix->setValue((entPos.x/32) - 1, entPos.y/32, 1);
                        listMiddle.emplace_back(new Hazard_LavalauncherR(*hazardsTxt[5])); originPos = Vector2f(-32, 0); create = 2;
                        break;
                    case 208 :
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrix->setValue(entPos.x/32, (entPos.y/32)-1, 1);
                        listMiddle.emplace_back(new Hazard_LavalauncherVL(*hazardsTxt[6])); originPos = Vector2f(2, 37); create = 2;
                        break;
                    case 209 :
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrix->setValue(entPos.x/32, (entPos.y/32)-1, 1);
                        listMiddle.emplace_back(new Hazard_LavalauncherVR(*hazardsTxt[6])); originPos = Vector2f(-30, 37); create = 2;
                        break;
                    case 210 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y - 6), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddle.emplace_back(new Hazard_SpikelauncherT(*hazardsTxt[8])); originPos = Vector2f(12, 8); create = 2;
                        break;
                    case 211 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y + 2), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddle.emplace_back(new Hazard_SpikelauncherB(*hazardsTxt[8])); originPos = Vector2f(12, -40); create = 2;
                        break;
                    case 212 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 64, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 96, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 128, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 160, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});

                        // Read all nodes :
                        {
                            vector<Vector2f> path;
                            float nodeSize, x, y;

                            level_file.read(reinterpret_cast<char*>(&nodeSize), 4);

                            short t = static_cast<short>(nodeSize);
                            for (short i = 0; i < t; i++)
                            {
                                level_file.read(reinterpret_cast<char*>(&x), 4);
                                level_file.read(reinterpret_cast<char*>(&y), 4);

                                path.emplace_back(Vector2f(x, y));
                            }

                            listAfter.emplace_back(new Hazard_Centipede(&path.front(), t, listCollider.end()));
                        }
                        originPos = Vector2f(0, 0);
                        create = 3;

                        break;
                    case 225 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 0)); originPos = Vector2f(-1, -1); create = 1; break;
                    case 226 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 270)); originPos = Vector2f(-1, -32); create = 1; break;
                    case 227 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 90)); originPos = Vector2f(-31, -1); create = 1; break;
                    case 228 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 180)); originPos = Vector2f(-31, -31); create = 1; break;
                }

                if (create == 1)
                    listPlaceables.back()->setPosition(entPos - originPos);
                else if (create == 2)
                    listMiddle.back()->setPosition(entPos - originPos);
                else if (create == 3)
                    listAfter.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                bool toMiddle = false;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 26 :
                        listPlaceables.emplace_back(new Bonus_MushroomBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 27 :
                        listPlaceables.emplace_back(new Bonus_FlowerBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 28 :
                        listPlaceables.emplace_back(new Bonus_BeetBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 29 :
                        listPlaceables.emplace_back(new Bonus_LuiBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 30 :
                        listPlaceables.emplace_back(new Bonus_StarBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 31 :
                        listPlaceables.emplace_back(new Bonus_LifeBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 32 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 33 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 34 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 35 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 36 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 37 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 38 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listCollider.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 39 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listCollider.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 40 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listCollider.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 41 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listCollider.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 42 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listCollider.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 43 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listCollider.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 44 : listMiddle.emplace_back(new Sprite_Mushroom(*itemsTxt[3])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 45 : listMiddle.emplace_back(new Sprite_Flower(*itemsTxt[4])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 46 : listMiddle.emplace_back(new Sprite_Beet(*itemsTxt[5])); originPos = Vector2f(-2, 2); toMiddle = true; break;
                    case 47 : listMiddle.emplace_back(new Sprite_Lui(*itemsTxt[6])); originPos = Vector2f(-1, 1); toMiddle = true; break;
                    case 48 : listMiddle.emplace_back(new Sprite_Star(*itemsTxt[7])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 49 : listMiddle.emplace_back(new Sprite_Life(*itemsTxt[8])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 50 :
                        listPlaceables.emplace_back(new Bonus_CoinBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 51 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 52 : listPlaceables.emplace_back(new Bonus_Coin(*itemsTxt[2])); originPos = Vector2f(-6, -2); break;
                    case 53 :
                        listPlaceables.emplace_back(new Bonus_Brick(itemsTxt[1])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 54 :
                        listPlaceables.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 10)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 55 :
                        listPlaceables.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 15)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 213 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 214 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, true);

                        toggle->attributeSolid(collisionMatrix->getReference(entPos.x/32, entPos.y/32));

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 215 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 0)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 216 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 217 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, true);

                        toggle->attributeSolid(collisionMatrix->getReference(entPos.x/32, entPos.y/32));

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 218 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 1)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 219 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 220 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, true);

                        toggle->attributeSolid(collisionMatrix->getReference(entPos.x/32, entPos.y/32));

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 221 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 2)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 222 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 223 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, true);

                        toggle->attributeSolid(collisionMatrix->getReference(entPos.x/32, entPos.y/32));

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 224 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 3)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                }

                if (toMiddle)
                    listMiddle.back()->setPosition(entPos - originPos);
                else
                    listPlaceables.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 92 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_TREE)); originPos = Vector2f(-1, 32); break;
                    case 93 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 94 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[0])); originPos = Vector2f(16, -2); break;
                    case 95 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_TREE)); originPos = Vector2f(-1, 32); break;
                    case 96 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 97 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[1])); originPos = Vector2f(16, -2); break;
                    case 98 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_TREE)); originPos = Vector2f(-1, 32); break;
                    case 99 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 100 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[2])); originPos = Vector2f(16, -2); break;
                    case 101 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_TREE)); originPos = Vector2f(-1, 32); break;
                    case 102 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 103 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[3])); originPos = Vector2f(16, -2); break;
                    case 104 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_TREE)); originPos = Vector2f(-1, 32); break;
                    case 105 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 106 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[4])); originPos = Vector2f(16, -2); break;
                    case 107 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_FENCE)); originPos = Vector2f(64, 0); break;
                    case 108 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_SNOWFENCE)); originPos = Vector2f(64, 0); break;
                    case 109 : listPlaceablesb.emplace_back(new Scenery_Cloud(*sceneriesTxt[5])); originPos = Vector2f(15, 8); break;
                    case 110 : listPlaceablesb.emplace_back(new Scenery_Cloud(*sceneriesTxt[6])); originPos = Vector2f(15, 8); break;
                    case 111 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[7], IntRect(0, 0, 127, 160))); originPos = Vector2f(47, 128); break;
                    case 112 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[8], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 113 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[9], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                    case 229 : listPlaceablesb.emplace_back(new Scenery_Waterfall(*sceneriesTxt[10])); originPos = Vector2f(31, -5); break;
                    case 230 : listPlaceablesb.emplace_back(new Scenery_Scroll(*sceneriesTxt[11])); originPos = Vector2f(11, 0); break;
                    case 231 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[12], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 232 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[13], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                }

                listPlaceablesb.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                vector<float> entData;

                unsigned char create = 0;

                float currentData[3];

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 0 : startPos = Vector2f(entPos.x, entPos.y); break;
                    case 1 : listMiddleb.emplace_back(new Mark_CheckPoint(*marksTxt[7], true)); originPos = Vector2f(28, 79); create = 2; break;
                    case 2 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listPlaceablesb.emplace_back(new Mark_LevelEnd(*marksTxt[8], *marksTxt[9], *effectTxt[6], currentData[0]));
                        originPos = Vector2f(-2, 256);
                        create = 1;
                        break;
                    case 3 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedMoveH(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        #ifdef DEBUGMODE
                        cout << entPos.x << " : " << entPos.y << endl;
                        #endif // DEBUGMODE

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 4 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedTouchH(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 5 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedMoveV(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 6 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedTouchV(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 7 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedFall(*marksTxt[0], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 8 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortMoveH(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 9 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortTouchH(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 10 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortMoveV(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 11 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortTouchV(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 12 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortFall(*marksTxt[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 13 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudMoveH(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 14 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudTouchH(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 15 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudMoveV(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 16 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudTouchV(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 17 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudFall(*marksTxt[2], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 18 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneMoveH(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 19 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneTouchH(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 20 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneMoveV(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 21 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneTouchV(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 22 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneFall(*marksTxt[3], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 23 : listMiddleb.emplace_back(new Mark_Spring(*marksTxt[4], 20)); originPos = Vector2f(0, 32); create = 2; break;
                    case 24 : listMiddleb.emplace_back(new Mark_Spring(*marksTxt[5], 28)); originPos = Vector2f(0, 32); create = 2; break;
                    case 25 : listMiddleb.emplace_back(new Mark_Spring(*marksTxt[6], 36)); originPos = Vector2f(0, 32); create = 2; break;
                    case 114 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_Goomba(*enemiesTxt[0], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 115 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_GoombaGrey(*enemiesTxt[1], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 116 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_Spiny(*enemiesTxt[2], listColliderb.back())); originPos = Vector2f(-17, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 117 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_SpinyGrey(*enemiesTxt[3], listColliderb.back())); originPos = Vector2f(-17, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 118 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_BuzzyBeetle(*enemiesTxt[4], enemiesTxt[13], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 119 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 120 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 121 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 122 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 123 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 124 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 125 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 126 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 127 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaGreenH(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 128 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaRedH(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 129 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaBlueH(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 130 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaGreenV(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 131 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaRedV(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 132 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaBlueV(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 133 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaGreenJ(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 134 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaRedJ(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 135 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaBlueJ(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 136 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaYellow(*enemiesTxt[12], enemiesTxt[8], enemiesTxt[17], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 137 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenT(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 138 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenL(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 139 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenR(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 140 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenB(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 141 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireT(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 142 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireL(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 143 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireR(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 144 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireB(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 145 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyT(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 146 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyL(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 147 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyR(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 148 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyB(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 149 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepRed(*enemiesTxt[21], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 150 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepGreen(*enemiesTxt[22], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 151 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepBlue(*enemiesTxt[23], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 152 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepYellow(*enemiesTxt[24], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 153 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddleb.emplace_back(new Sprite_HammerBro(*enemiesTxt[25], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 154 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddleb.emplace_back(new Sprite_SilverBro(*enemiesTxt[26], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 155 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddleb.emplace_back(new Sprite_FireBro(*enemiesTxt[27], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 156 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddleb.emplace_back(new Sprite_BeetBro(*enemiesTxt[28], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 157 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, reinterpret_cast<Placeable*>(1)});
                        listMiddleb.emplace_back(new Sprite_SpikeBro(*enemiesTxt[29], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 158 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_LakituWhite(*enemiesTxt[32], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 159 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_LakituDark(*enemiesTxt[33], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 160 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_LakituGold(*enemiesTxt[34], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 161 : listMiddleb.emplace_back(new Sprite_Poison(*enemiesTxt[37])); originPos = Vector2f(0, 0); create = 2; break;
                    case 162 :
                        listPlaceablesb.emplace_back(new Bonus_PoisonBlock(itemsTxt[0])); originPos = Vector2f(0, 0); create = 1;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 163 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_PoisonBlockInv(itemsTxt[0], listColliderb.back())); originPos = Vector2f(0, 0); create = 1;
                        break;
                    case 164 : listMiddleb.emplace_back(new Sprite_ThwompB(*enemiesTxt[38])); originPos = Vector2f(10, 4); create = 2; break;
                    case 165 : listMiddleb.emplace_back(new Sprite_ThwompT(*enemiesTxt[38])); originPos = Vector2f(10, -36); create = 2; break;
                    case 166 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 42, entPos.y - 23), Vector2f(52, 55)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_Bowser(*enemiesTxt[39], listColliderb.back())); originPos = Vector2f(16, 40); create = 2;
                        break;
                    case 167 : listPlaceablesb.emplace_back(new Hazard_LavaT(*hazardsTxt[0])); originPos = Vector2f(0, 2); create = 1; break;
                    case 168 : listPlaceablesb.emplace_back(new Hazard_LavaB(*hazardsTxt[0])); originPos = Vector2f(0, -34); create = 1; break;
                    case 169 : listPlaceablesb.emplace_back(new Hazard_LavaL(*hazardsTxt[0])); originPos = Vector2f(2, 0); create = 1; break;
                    case 170 : listPlaceablesb.emplace_back(new Hazard_LavaR(*hazardsTxt[0])); originPos = Vector2f(-34, 0); create = 1; break;
                    case 171 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(928, 96)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(960, 96)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(960, 128)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(928, 128)));
                        create = 0;
                        break;
                    case 172 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooT(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 173 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooB(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 174 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooL(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 175 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooR(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 176 : listMiddleb.emplace_back(new Hazard_PodobooFountainT); originPos = Vector2f(0, 0); create = 2; break;
                    case 177 : listMiddleb.emplace_back(new Hazard_PodobooFountainB); originPos = Vector2f(0, 0); create = 2; break;
                    case 178 : listMiddleb.emplace_back(new Hazard_PodobooFountainL); originPos = Vector2f(0, 0); create = 2; break;
                    case 179 : listMiddleb.emplace_back(new Hazard_PodobooFountainR); originPos = Vector2f(0, 0); create = 2; break;
                    case 180 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 96))); originPos = Vector2f(0, 0); create = 1; break;
                    case 181 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 192))); originPos = Vector2f(0, 0); create = 1; break;
                    case 182 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 128))); originPos = Vector2f(0, 0); create = 1; break;
                    case 183 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 160))); originPos = Vector2f(0, 0); create = 1; break;
                    case 184 :
                        listMiddleb.emplace_back(new Hazard_CannonHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 185 :
                        listMiddleb.emplace_back(new Hazard_CannonHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 186 :
                        listMiddleb.emplace_back(new Hazard_CannonVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 187 :
                        listMiddleb.emplace_back(new Hazard_CannonVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 188 :
                        listMiddleb.emplace_back(new Hazard_CannonHomingHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 189 :
                        listMiddleb.emplace_back(new Hazard_CannonHomingHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 190 :
                        listMiddleb.emplace_back(new Hazard_CannonHomingVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 191 :
                        listMiddleb.emplace_back(new Hazard_CannonHomingVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2;
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 192 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(608, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(640, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(640, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(608, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 193 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(576, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(608, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(608, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(576, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 194 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(640, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(672, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(672, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(640, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 195 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(704, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(736, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(736, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(704, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 196 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(672, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(704, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(704, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(672, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 197 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(768, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(800, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(800, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(768, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 198 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(736, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(768, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(768, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(736, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 199 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(800, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(832, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(832, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(800, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 200 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(864, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(896, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(896, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(864, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 201 :
                        tilemapLayer2b.emplace_back(Vertex(entPos, Color::White, Vector2f(832, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y), Color::White, Vector2f(864, 224)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x + 32, entPos.y + 32), Color::White, Vector2f(864, 256)));
                        tilemapLayer2b.emplace_back(Vertex(Vector2f(entPos.x, entPos.y + 32), Color::White, Vector2f(832, 256)));
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        create = 0;
                        break;
                    case 202 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 203 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 204 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscRCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 205 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscRC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 206 :
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrixb->setValue((entPos.x/32) + 1, entPos.y/32, 1);
                        listMiddleb.emplace_back(new Hazard_LavalauncherL(*hazardsTxt[5])); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 207 :
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrixb->setValue((entPos.x/32) - 1, entPos.y/32, 1);
                        listMiddleb.emplace_back(new Hazard_LavalauncherR(*hazardsTxt[5])); originPos = Vector2f(-32, 0); create = 2;
                        break;
                    case 208 :
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrixb->setValue(entPos.x/32, (entPos.y/32)-1, 1);
                        listMiddleb.emplace_back(new Hazard_LavalauncherVL(*hazardsTxt[6])); originPos = Vector2f(2, 37); create = 2;
                        break;
                    case 209 :
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        collisionMatrixb->setValue(entPos.x/32, (entPos.y/32)-1, 1);
                        listMiddleb.emplace_back(new Hazard_LavalauncherVR(*hazardsTxt[6])); originPos = Vector2f(-30, 37); create = 2;
                        break;
                    case 210 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y - 6), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddleb.emplace_back(new Hazard_SpikelauncherT(*hazardsTxt[8])); originPos = Vector2f(12, 8); create = 2;
                        break;
                    case 211 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y + 2), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddleb.emplace_back(new Hazard_SpikelauncherB(*hazardsTxt[8])); originPos = Vector2f(12, -40); create = 2;
                        break;
                    case 212 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 64, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 96, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 128, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 160, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});

                        // Read all nodes :
                        {
                            vector<Vector2f> path;
                            float nodeSize, x, y;

                            level_file.read(reinterpret_cast<char*>(&nodeSize), 4);

                            short t = static_cast<short>(nodeSize);
                            for (short i = 0; i < t; i++)
                            {
                                level_file.read(reinterpret_cast<char*>(&x), 4);
                                level_file.read(reinterpret_cast<char*>(&y), 4);

                                path.emplace_back(Vector2f(x, y));
                            }

                            listAfterb.emplace_back(new Hazard_Centipede(&path.front(), t, listColliderb.end()));
                        }

                        originPos = Vector2f(0, 0);
                        create = 3;

                        break;
                    case 225 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 0)); originPos = Vector2f(-1, -1); create = 1; break;
                    case 226 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 270)); originPos = Vector2f(-1, -32); create = 1; break;
                    case 227 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 90)); originPos = Vector2f(-31, -1); create = 1; break;
                    case 228 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 180)); originPos = Vector2f(-31, -31); create = 1; break;
                }

                if (create == 1)
                    listPlaceablesb.back()->setPosition(entPos - originPos);
                else if (create == 2)
                    listMiddleb.back()->setPosition(entPos - originPos);
                else if (create == 3)
                    listAfterb.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                bool toMiddle = false;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 26 :
                        listPlaceablesb.emplace_back(new Bonus_MushroomBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 27 :
                        listPlaceablesb.emplace_back(new Bonus_FlowerBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 28 :
                        listPlaceablesb.emplace_back(new Bonus_BeetBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 29 :
                        listPlaceablesb.emplace_back(new Bonus_LuiBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 30 :
                        listPlaceablesb.emplace_back(new Bonus_StarBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 31 :
                        listPlaceablesb.emplace_back(new Bonus_LifeBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 32 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 33 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 34 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 35 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 36 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 37 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 38 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listColliderb.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 39 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listColliderb.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 40 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listColliderb.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 41 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listColliderb.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 42 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listColliderb.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 43 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listColliderb.back(), 1)); originPos = Vector2f(0, 0);
                        break;
                    case 44 : listMiddleb.emplace_back(new Sprite_Mushroom(*itemsTxt[3])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 45 : listMiddleb.emplace_back(new Sprite_Flower(*itemsTxt[4])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 46 : listMiddleb.emplace_back(new Sprite_Beet(*itemsTxt[5])); originPos = Vector2f(-2, 2); toMiddle = true; break;
                    case 47 : listMiddleb.emplace_back(new Sprite_Lui(*itemsTxt[6])); originPos = Vector2f(-1, 1); toMiddle = true; break;
                    case 48 : listMiddleb.emplace_back(new Sprite_Star(*itemsTxt[7])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 49 : listMiddleb.emplace_back(new Sprite_Life(*itemsTxt[8])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 50 :
                        listPlaceablesb.emplace_back(new Bonus_CoinBlock(itemsTxt[0])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 51 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 52 : listPlaceablesb.emplace_back(new Bonus_Coin(*itemsTxt[2])); originPos = Vector2f(-6, -2); break;
                    case 53 :
                        listPlaceablesb.emplace_back(new Bonus_Brick(itemsTxt[1])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 54 :
                        listPlaceablesb.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 10)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 55 :
                        listPlaceablesb.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 15)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 213 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 214 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, true);

                        toggle->attributeSolid(collisionMatrixb->getReference(entPos.x/32, entPos.y/32));

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 215 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 0)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 216 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 217 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, true);

                        toggle->attributeSolid(collisionMatrixb->getReference(entPos.x/32, entPos.y/32));

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 218 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 1)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 219 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 220 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, true);

                        toggle->attributeSolid(collisionMatrixb->getReference(entPos.x/32, entPos.y/32));

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 221 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 2)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 222 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 223 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, true);

                        toggle->attributeSolid(collisionMatrixb->getReference(entPos.x/32, entPos.y/32));

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 224 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 3)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                }

                if (toMiddle)
                    listMiddleb.back()->setPosition(entPos - originPos);
                else
                    listPlaceablesb.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned char listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 1);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                WarpsData* currentData = new WarpsData;

                level_file.read(reinterpret_cast<char*>(currentData), sizeof(WarpsData));

                listPassages.emplace_back(currentData);
            }
        }
    }

    if (!listMiddle.empty())
    {
        for (vector<Placeable*>::iterator it = listMiddle.begin(); it != listMiddle.end(); it++)
            listPlaceables.emplace_back(*it);
    }

    spriteIterator = listPlaceables.end();

    if (!listAfter.empty())
    {
        for (vector<Placeable*>::iterator it = listAfter.begin(); it != listAfter.end(); it++)
            listPlaceables.emplace_back(*it);
    }

    if (!listMiddleb.empty())
    {
        for (vector<Placeable*>::iterator it = listMiddleb.begin(); it != listMiddleb.end(); it++)
            listPlaceablesb.emplace_back(*it);
    }

    spriteIteratorb = listPlaceablesb.end();

    if (!listAfterb.empty())
    {
        for (vector<Placeable*>::iterator it = listAfterb.begin(); it != listAfterb.end(); it++)
            listPlaceablesb.emplace_back(*it);
    }

    listMiddle.clear();
    listAfter.clear();

    listMiddleb.clear();
    listAfterb.clear();

    if (startZone == 2)
    {
        zoneb = true;
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZoneb, 0, &musicChannel);
    }
    else
    {
        zoneb = false;
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZonea, 0, &musicChannel);
    }

    if (startPos.x > -64)
    {
        player = new Mark_Mario(marioTxt, effectTxt[15]);
        player->setPosition(startPos + Vector2f(16, 0));
    }

    {
        Vector2f playerPos = startPos - Vector2f(5, 32);

        if (playerPos.x < 298)
            playerPos.x = 298;

        if (playerPos.y < 208)
            playerPos.y = 208;

        if (zoneb)
        {
            if (playerPos.x > (levelScaleb.x * 640) - 341)
                playerPos.x = (levelScaleb.x * 640) - 341;

            if (playerPos.y > (levelScaleb.y * 480) - 272)
                playerPos.y = (levelScaleb.y * 480) - 272;
        }
        else
        {
            if (playerPos.x > (levelScale.x * 640) - 341)
                playerPos.x = (levelScale.x * 640) - 341;

            if (playerPos.y > (levelScale.y * 480) - 272)
                playerPos.y = (levelScale.y * 480) - 272;
        }

        cameraPos = Vector2f(playerPos.x + 21, playerPos.y + 32);

        if (!autoscrollPath.empty() && autoscroll == zoneb + 1)
        {
            Vector2f firstnode(autoscrollPath.front());

            if (firstnode.x > cameraPos.x - 300 && firstnode.x < cameraPos.x + 300 && firstnode.y > cameraPos.y - 220 && firstnode.y < cameraPos.y + 220)
            {
                cameraPos = firstnode;
                autoscroll_node = 1;
            }
        }
    }

    {
        char buffer[6]("");

        sprintf(buffer, "%d", timer/25);

        timerFont = new SpriteFont(Vector2f(577, 36), true, buffer);
    }

    if (worldNamePos.x > -1)
        worldNamePos = Vector2f(roundf(worldFont->getHorizontalSize() / 2), -204);

    nextLevel = false;

    return true;
}

static void Level_Reload()
{
    vector <Placeable*> listMiddle;
    vector <Placeable*> listAfter;

    vector <Placeable*> listMiddleb;
    vector <Placeable*> listAfterb;

    vector <Vector2f> invisiblesOnce;
    vector <Vector2f> invisiblesOnceb;

    Vector2f startPos(-64, -64);
    char startZone = 0;

    Mark_CheckPoint* currentCheckpoint = NULL;

    if (player->m_checkpoint != NULL)
    {
        Mark_CheckPoint* checkPoint = player->m_checkpoint;

        startPos = Vector2f(checkPoint->m_collision.left-13, checkPoint->m_collision.top+79);
        startZone = checkPoint->m_zone + 1;
    }

    for (list<Placeable*>::iterator it = listPlaceables.begin(); it != listPlaceables.end(); it++)
    {
        if ((*it)->getID() == ID_INVISIBLE)
        {
            Bonus_BlockInv* inv = reinterpret_cast<Bonus_BlockInv*>(*it);

            if ((inv->m_once == 1 && inv->m_hit > 0) || inv->m_once == 2)
                invisiblesOnce.emplace_back(inv->m_sprite.getPosition());

            Vector2f pos((*it)->getPosition());
            collisionMatrix->setValue(pos.x/32, pos.y/32, 0);
        }
        delete *it;
    }

    for (list<Placeable*>::iterator it = listPlaceablesb.begin(); it != listPlaceablesb.end(); it++)
    {
        if ((*it)->getID() == ID_INVISIBLE)
        {
            Bonus_BlockInv* inv = reinterpret_cast<Bonus_BlockInv*>(*it);

            if ((inv->m_once == 1 && inv->m_hit > 0) || inv->m_once == 2)
                invisiblesOnceb.emplace_back(inv->m_sprite.getPosition());

            Vector2f pos((*it)->getPosition());
            collisionMatrixb->setValue(pos.x/32, pos.y/32, 0);
        }
        delete *it;
    }

    delete player;

    for (list<Collider*>::iterator it = listCollider.begin(); it != listCollider.end(); it++)
        delete *it;

    for (list<Collider*>::iterator it = listColliderb.begin(); it != listColliderb.end(); it++)
        delete *it;

    for (list<Effect*>::iterator it = listEffect.begin(); it != listEffect.end(); it++)
        delete *it;

    listPlaceables.clear();
    listPlaceablesb.clear();

    listCollider.clear();
    listColliderb.clear();

    listEffect.clear();

    frame_Hud = 0;
    frame_Water = 0;
    frame_Lava = 0;
    frame_Flame = 0;
    frame_Podoboo = 0;
    frame_Clouds = 0;
    frame_Flower = 0;
    frame_Rotodisc = 0;

    game_powerup = 0;

    game_toggleSolidity[0] = game_toggleSoliditySave[0];
    game_toggleSolidity[1] = game_toggleSoliditySave[1];
    game_toggleSolidity[2] = game_toggleSoliditySave[2];
    game_toggleSolidity[3] = game_toggleSoliditySave[3];

    blockhitter = FloatRect(-64, -64, 28, 8);
    spritehitter = FloatRect(-64, -64, 0, 0);
    enemystomper = FloatRect(-64, -64, 34, 12);
    enemykiller = FloatRect(-64, -64, 8, 8);
    blockkiller = FloatRect(-64, -64, 28, 33);
    enemystomper_pt = NULL;
    enemykiller_fire = 0;
    enemykiller_create = false;
    blockkiller_create = true;
    spritehitter_create = false;

    killcount = 0;

    timerAlert = 1.0f;
    timerAlertPhase = true;

    levelCamera_shake = 0;

    effectCloudPos[0] = 0;
    effectCloudPos[1] = 0;
    effectLavafallPos = 0;
    effectWeatherPos = 0;
    effectLightning = (rand() % 350) + 100;

    timer = levelTimer;

    koopaEngaged = 0;
    koopaHealth = koopaHealth_saved;

    hudBowser->clear();
    hudBowser_descend = -308;

    autoscroll_node = autoscroll_nodesaved;

    player = NULL;

    hudSpr[3]->setScale(1, 1);

    if (watera != NULL)
    {
        liquidTargetHeight = liquidStartHeight;

        watera->setHeight(liquidStartHeight, levelScale);
    }

    if (waterb != NULL)
    {
        liquidTargetHeightb = liquidStartHeightb;

        waterb->setHeight(liquidStartHeightb, levelScaleb);
    }

    level_file.clear(ios::eofbit);
    level_file.seekg(spriteDefStart);

    restartLevel = false;

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 92 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_TREE)); originPos = Vector2f(-1, 32); break;
                    case 93 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 94 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[0])); originPos = Vector2f(16, -2); break;
                    case 95 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_TREE)); originPos = Vector2f(-1, 32); break;
                    case 96 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 97 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[1])); originPos = Vector2f(16, -2); break;
                    case 98 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_TREE)); originPos = Vector2f(-1, 32); break;
                    case 99 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 100 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[2])); originPos = Vector2f(16, -2); break;
                    case 101 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_TREE)); originPos = Vector2f(-1, 32); break;
                    case 102 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 103 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[3])); originPos = Vector2f(16, -2); break;
                    case 104 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_TREE)); originPos = Vector2f(-1, 32); break;
                    case 105 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 106 : listPlaceables.emplace_back(new Scenery_Bush(*sceneriesTxt[4])); originPos = Vector2f(16, -2); break;
                    case 107 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_FENCE)); originPos = Vector2f(80, 0); break;
                    case 108 : listPlaceables.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_SNOWFENCE)); originPos = Vector2f(80, 0); break;
                    case 109 : listPlaceables.emplace_back(new Scenery_Cloud(*sceneriesTxt[5])); originPos = Vector2f(15, 8); break;
                    case 110 : listPlaceables.emplace_back(new Scenery_Cloud(*sceneriesTxt[6])); originPos = Vector2f(15, 8); break;
                    case 111 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[7], IntRect(0, 0, 127, 160))); originPos = Vector2f(47, 128); break;
                    case 112 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[8], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 113 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[9], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                    case 229 : listPlaceables.emplace_back(new Scenery_Waterfall(*sceneriesTxt[10])); originPos = Vector2f(31, -5); break;
                    case 230 : listPlaceables.emplace_back(new Scenery_Scroll(*sceneriesTxt[11])); originPos = Vector2f(11, 0); break;
                    case 231 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[12], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 232 : listPlaceables.emplace_back(new Scenery_Static(*sceneriesTxt[13], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                }

                listPlaceables.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                vector<float> entData;

                unsigned char create = 0;

                float currentData[3];

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 0 :
                        if (startPos.x <= -64)
                            startPos = Vector2f(entPos.x, entPos.y);

                        startZone = 0;
                        break;
                    case 1 :
                    {
                        Mark_CheckPoint* checkPoint = new Mark_CheckPoint(*marksTxt[7], false);

                        if (startPos.x > -64 && currentCheckpoint == NULL)
                        {
                            #ifdef DEBUGMODE
                            cout << "Ent Pos : " << entPos.x << " : " << entPos.y << endl;
                            cout << "Player Pos : " << startPos.x << " : " << startPos.y << endl;
                            #endif // DEBUGMODE
                            if (entPos.x == startPos.x && entPos.y == startPos.y)
                            {
                                checkPoint->silentCheck();
                                currentCheckpoint = checkPoint;
                            }
                        }

                        listMiddle.emplace_back(checkPoint); originPos = Vector2f(28, 79); create = 2;
                        break;
                    }
                    case 2 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listPlaceables.emplace_back(new Mark_LevelEnd(*marksTxt[8], *marksTxt[9], *effectTxt[6], currentData[0]));
                        originPos = Vector2f(-2, 256);
                        create = 1;
                        break;
                    case 3 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedMoveH(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 4 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedTouchH(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 5 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedMoveV(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 6 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedTouchV(*marksTxt[0], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 7 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformRedFall(*marksTxt[0], listCollider.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 8 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortMoveH(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 9 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortTouchH(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 10 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortMoveV(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 11 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortTouchV(*marksTxt[1], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 12 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformShortFall(*marksTxt[1], listCollider.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 13 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudMoveH(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 14 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudTouchH(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 15 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudMoveV(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 16 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudTouchV(*marksTxt[2], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 17 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformCloudFall(*marksTxt[2], listCollider.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 18 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneMoveH(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 19 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneTouchH(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 20 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneMoveV(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 21 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneTouchV(*marksTxt[3], currentData[0], currentData[1], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 22 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfter.emplace_back(new Mark_PlatformStoneFall(*marksTxt[3], listCollider.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 23 : listMiddle.emplace_back(new Mark_Spring(*marksTxt[4], 20)); originPos = Vector2f(0, 32); create = 2; break;
                    case 24 : listMiddle.emplace_back(new Mark_Spring(*marksTxt[5], 28)); originPos = Vector2f(0, 32); create = 2; break;
                    case 25 : listMiddle.emplace_back(new Mark_Spring(*marksTxt[6], 36)); originPos = Vector2f(0, 32); create = 2; break;
                    case 114 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_Goomba(*enemiesTxt[0], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 115 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_GoombaGrey(*enemiesTxt[1], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 116 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_Spiny(*enemiesTxt[2], listCollider.back())); originPos = Vector2f(-17, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 117 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_SpinyGrey(*enemiesTxt[3], listCollider.back())); originPos = Vector2f(-17, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 118 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_BuzzyBeetle(*enemiesTxt[4], enemiesTxt[13], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 119 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 120 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 121 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 122 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listCollider.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 123 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 124 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 125 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 126 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listCollider.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 127 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaGreenH(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 128 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaRedH(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 129 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaBlueH(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 130 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaGreenV(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 131 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaRedV(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 132 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaBlueV(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listCollider.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 133 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaGreenJ(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 134 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaRedJ(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 135 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaBlueJ(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 136 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddle.emplace_back(new Sprite_ParatroopaYellow(*enemiesTxt[12], enemiesTxt[8], enemiesTxt[17], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        listCollider.back()->object = listMiddle.back();
                        break;
                    case 137 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenT(enemiesTxt[18], listCollider.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 138 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenL(enemiesTxt[18], listCollider.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 139 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenR(enemiesTxt[18], listCollider.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 140 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreenB(enemiesTxt[18], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 141 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireT(enemiesTxt[19], listCollider.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 142 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireL(enemiesTxt[19], listCollider.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 143 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireR(enemiesTxt[19], listCollider.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 144 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantFireB(enemiesTxt[19], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 145 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyT(enemiesTxt[20], listCollider.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 146 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyL(enemiesTxt[20], listCollider.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 147 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyR(enemiesTxt[20], listCollider.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 148 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_PlantGreyB(enemiesTxt[20], listCollider.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 149 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepRed(*enemiesTxt[21], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 150 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepGreen(*enemiesTxt[22], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 151 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepBlue(*enemiesTxt[23], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 152 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_CheepCheepYellow(*enemiesTxt[24], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 153 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_HammerBro(*enemiesTxt[25], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 154 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_SilverBro(*enemiesTxt[26], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 155 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_FireBro(*enemiesTxt[27], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 156 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_BeetBro(*enemiesTxt[28], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 157 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_SpikeBro(*enemiesTxt[29], listCollider.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 158 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_LakituWhite(*enemiesTxt[32], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 159 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_LakituDark(*enemiesTxt[33], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 160 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_LakituGold(*enemiesTxt[34], listCollider.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 161 : listMiddle.emplace_back(new Sprite_Poison(*enemiesTxt[37])); originPos = Vector2f(0, 0); create = 2; break;
                    case 162 : listPlaceables.emplace_back(new Bonus_PoisonBlock(itemsTxt[0])); originPos = Vector2f(0, 0); create = 1; break;
                    case 163 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_PoisonBlockInv(itemsTxt[0], listCollider.back())); originPos = Vector2f(0, 0); create = 1;
                        break;
                    case 164 : listMiddle.emplace_back(new Sprite_ThwompB(*enemiesTxt[38])); originPos = Vector2f(10, 4); create = 2; break;
                    case 165 : listMiddle.emplace_back(new Sprite_ThwompT(*enemiesTxt[38])); originPos = Vector2f(10, -36); create = 2; break;
                    case 166 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 42, entPos.y - 23), Vector2f(52, 55)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddle.emplace_back(new Sprite_Bowser(*enemiesTxt[39], listCollider.back())); originPos = Vector2f(16, 40); create = 2;
                        break;
                    case 167 : listPlaceables.emplace_back(new Hazard_LavaT(*hazardsTxt[0])); originPos = Vector2f(0, 2); create = 1; break;
                    case 168 : listPlaceables.emplace_back(new Hazard_LavaB(*hazardsTxt[0])); originPos = Vector2f(0, -34); create = 1; break;
                    case 169 : listPlaceables.emplace_back(new Hazard_LavaL(*hazardsTxt[0])); originPos = Vector2f(2, 0); create = 1; break;
                    case 170 : listPlaceables.emplace_back(new Hazard_LavaR(*hazardsTxt[0])); originPos = Vector2f(-34, 0); create = 1; break;
                    case 172 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooT(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 173 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooB(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 174 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooL(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 175 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddle.emplace_back(new Hazard_PodobooR(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 176 : listMiddle.emplace_back(new Hazard_PodobooFountainT); originPos = Vector2f(0, 0); create = 2; break;
                    case 177 : listMiddle.emplace_back(new Hazard_PodobooFountainB); originPos = Vector2f(0, 0); create = 2; break;
                    case 178 : listMiddle.emplace_back(new Hazard_PodobooFountainL); originPos = Vector2f(0, 0); create = 2; break;
                    case 179 : listMiddle.emplace_back(new Hazard_PodobooFountainR); originPos = Vector2f(0, 0); create = 2; break;
                    case 180 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 96))); originPos = Vector2f(0, 0); create = 1; break;
                    case 181 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 192))); originPos = Vector2f(0, 0); create = 1; break;
                    case 182 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 128))); originPos = Vector2f(0, 0); create = 1; break;
                    case 183 : listPlaceables.emplace_back(new Hazard_Spike(Vector2i(864, 160))); originPos = Vector2f(0, 0); create = 1; break;
                    case 184 : listMiddle.emplace_back(new Hazard_CannonHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2; break;
                    case 185 : listMiddle.emplace_back(new Hazard_CannonHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2; break;
                    case 186 : listMiddle.emplace_back(new Hazard_CannonVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2; break;
                    case 187 : listMiddle.emplace_back(new Hazard_CannonVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2; break;
                    case 188 : listMiddle.emplace_back(new Hazard_CannonHomingHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2; break;
                    case 189 : listMiddle.emplace_back(new Hazard_CannonHomingHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2; break;
                    case 190 : listMiddle.emplace_back(new Hazard_CannonHomingVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2; break;
                    case 191 : listMiddle.emplace_back(new Hazard_CannonHomingVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2; break;
                    case 202 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 203 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 204 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscRCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 205 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfter.emplace_back(new Hazard_RotodiscRC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 206 : listMiddle.emplace_back(new Hazard_LavalauncherL(*hazardsTxt[5])); originPos = Vector2f(0, 0); create = 2; break;
                    case 207 : listMiddle.emplace_back(new Hazard_LavalauncherR(*hazardsTxt[5])); originPos = Vector2f(-32, 0); create = 2; break;
                    case 208 : listMiddle.emplace_back(new Hazard_LavalauncherVL(*hazardsTxt[6])); originPos = Vector2f(2, 37); create = 2; break;
                    case 209 : listMiddle.emplace_back(new Hazard_LavalauncherVR(*hazardsTxt[6])); originPos = Vector2f(-30, 37); create = 2; break;
                    case 210 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y - 6), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddle.emplace_back(new Hazard_SpikelauncherT(*hazardsTxt[8])); originPos = Vector2f(12, 8); create = 2;
                        break;
                    case 211 :
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y + 2), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddle.emplace_back(new Hazard_SpikelauncherB(*hazardsTxt[8])); originPos = Vector2f(12, -40); create = 2;
                        break;
                    case 212 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 64, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 96, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 128, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listCollider.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 160, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});

                        // Read all nodes :
                        {
                            vector<Vector2f> path;
                            float nodeSize, x, y;

                            level_file.read(reinterpret_cast<char*>(&nodeSize), 4);

                            short t = static_cast<short>(nodeSize);
                            for (short i = 0; i < t; i++)
                            {
                                level_file.read(reinterpret_cast<char*>(&x), 4);
                                level_file.read(reinterpret_cast<char*>(&y), 4);

                                path.emplace_back(Vector2f(x, y));
                            }

                            listAfter.emplace_back(new Hazard_Centipede(&path.front(), t, listCollider.end()));
                        }
                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 225 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 0)); originPos = Vector2f(-1, -1); create = 1; break;
                    case 226 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 270)); originPos = Vector2f(-1, -32); create = 1; break;
                    case 227 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 90)); originPos = Vector2f(-31, -1); create = 1; break;
                    case 228 : listPlaceables.emplace_back(new Sprite_StaticPlant(entPos, 180)); originPos = Vector2f(-31, -31); create = 1; break;
                }

                if (create == 1)
                    listPlaceables.back()->setPosition(entPos - originPos);
                else if (create == 2)
                    listMiddle.back()->setPosition(entPos - originPos);
                else if (create == 3)
                    listAfter.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                bool toMiddle = false;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 26 : listPlaceables.emplace_back(new Bonus_MushroomBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 27 : listPlaceables.emplace_back(new Bonus_FlowerBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 28 : listPlaceables.emplace_back(new Bonus_BeetBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 29 : listPlaceables.emplace_back(new Bonus_LuiBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 30 : listPlaceables.emplace_back(new Bonus_StarBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 31 : listPlaceables.emplace_back(new Bonus_LifeBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 32 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 33 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 34 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 35 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 36 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 37 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 38 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnce.begin(); it != invisiblesOnce.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 2));
                            else
                                listPlaceables.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listCollider.back(), 1));
                        }
                        break;
                    case 39 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnce.begin(); it != invisiblesOnce.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 2));
                            else
                                listPlaceables.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listCollider.back(), 1));
                        }
                        break;
                    case 40 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnce.begin(); it != invisiblesOnce.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 2));
                            else
                                listPlaceables.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listCollider.back(), 1));
                        }
                        break;
                    case 41 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnce.begin(); it != invisiblesOnce.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 2));
                            else
                                listPlaceables.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listCollider.back(), 1));
                        }
                        break;
                    case 42 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnce.begin(); it != invisiblesOnce.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 2));
                            else
                                listPlaceables.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listCollider.back(), 1));
                        }
                        break;
                    case 43 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnce.begin(); it != invisiblesOnce.end(); it++)
                            {
                                #ifdef DEBUGMODE
                                cout << "Entpos.x = " << entPos.x << " ; Registered.x = " << (*it).x << endl;
                                cout << "Entpos.y = " << entPos.y << " ; Registered.y = " << (*it).y << endl;
                                #endif // DEBUGMODE
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 2));
                            else
                                listPlaceables.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listCollider.back(), 1));
                        }
                        break;
                    case 44 : listMiddle.emplace_back(new Sprite_Mushroom(*itemsTxt[3])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 45 : listMiddle.emplace_back(new Sprite_Flower(*itemsTxt[4])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 46 : listMiddle.emplace_back(new Sprite_Beet(*itemsTxt[5])); originPos = Vector2f(-2, 2); toMiddle = true; break;
                    case 47 : listMiddle.emplace_back(new Sprite_Lui(*itemsTxt[6])); originPos = Vector2f(-1, 1); toMiddle = true; break;
                    case 48 : listMiddle.emplace_back(new Sprite_Star(*itemsTxt[7])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 49 : listMiddle.emplace_back(new Sprite_Life(*itemsTxt[8])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 50 : listPlaceables.emplace_back(new Bonus_CoinBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 51 :
                        listCollider.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceables.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listCollider.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 52 : listPlaceables.emplace_back(new Bonus_Coin(*itemsTxt[2])); originPos = Vector2f(-6, -2); break;
                    case 53 :
                        listPlaceables.emplace_back(new Bonus_Brick(itemsTxt[1])); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 54 : listPlaceables.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 10)); originPos = Vector2f(0, 0); break;
                    case 55 : listPlaceables.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 15)); originPos = Vector2f(0, 0); break;
                    case 213 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 214 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, true);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 215 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 0)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 216 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 217 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, true);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 218 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 1)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 219 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 220 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, true);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 221 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 2)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 222 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, false);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 223 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, true);
                        unsigned char* c = collisionMatrix->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceables.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 224 :
                        listPlaceables.emplace_back(new Bonus_Switch(marksTxt[10], 3)); originPos = Vector2f(0, 0);
                        collisionMatrix->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                }

                if (toMiddle)
                    listMiddle.back()->setPosition(entPos - originPos);
                else
                    listPlaceables.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 92 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_TREE)); originPos = Vector2f(-1, 32); break;
                    case 93 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GREEN_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 94 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[0])); originPos = Vector2f(16, -2); break;
                    case 95 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_TREE)); originPos = Vector2f(-1, 32); break;
                    case 96 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_GRAY_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 97 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[1])); originPos = Vector2f(16, -2); break;
                    case 98 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_TREE)); originPos = Vector2f(-1, 32); break;
                    case 99 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_YELLOW_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 100 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[2])); originPos = Vector2f(16, -2); break;
                    case 101 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_TREE)); originPos = Vector2f(-1, 32); break;
                    case 102 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_RED_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 103 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[3])); originPos = Vector2f(16, -2); break;
                    case 104 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_TREE)); originPos = Vector2f(-1, 32); break;
                    case 105 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_BLUE_BIGTREE)); originPos = Vector2f(0, 64); break;
                    case 106 : listPlaceablesb.emplace_back(new Scenery_Bush(*sceneriesTxt[4])); originPos = Vector2f(16, -2); break;
                    case 107 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_FENCE)); originPos = Vector2f(80, 0); break;
                    case 108 : listPlaceablesb.emplace_back(new Scenery_Static(*mainTileset, TILE_SCENERY_SNOWFENCE)); originPos = Vector2f(80, 0); break;
                    case 109 : listPlaceablesb.emplace_back(new Scenery_Cloud(*sceneriesTxt[5])); originPos = Vector2f(15, 8); break;
                    case 110 : listPlaceablesb.emplace_back(new Scenery_Cloud(*sceneriesTxt[6])); originPos = Vector2f(15, 8); break;
                    case 111 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[7], IntRect(0, 0, 127, 160))); originPos = Vector2f(47, 128); break;
                    case 112 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[8], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 113 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[9], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                    case 229 : listPlaceablesb.emplace_back(new Scenery_Waterfall(*sceneriesTxt[10])); originPos = Vector2f(31, -5); break;
                    case 230 : listPlaceablesb.emplace_back(new Scenery_Scroll(*sceneriesTxt[11])); originPos = Vector2f(11, 0); break;
                    case 231 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[12], IntRect(0, 0, 187, 160))); originPos = Vector2f(77, 128); break;
                    case 232 : listPlaceablesb.emplace_back(new Scenery_Static(*sceneriesTxt[13], IntRect(0, 0, 315, 192))); originPos = Vector2f(141, 160); break;
                }

                listPlaceablesb.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                vector<float> entData;

                unsigned char create = 0;

                float currentData[3];

                unsigned char entType;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 0 :
                        if (startPos.x <= -64)
                            startPos = Vector2f(entPos.x, entPos.y);

                        startZone = 1;
                        break;
                    case 1 :
                        {
                            Mark_CheckPoint* checkPoint = new Mark_CheckPoint(*marksTxt[7], true);

                            if (startPos.x > -64 && currentCheckpoint == NULL)
                            {
                                if (entPos.x == startPos.x && entPos.y == startPos.y)
                                {
                                    checkPoint->silentCheck();
                                    currentCheckpoint = checkPoint;
                                }
                            }

                            listMiddleb.emplace_back(checkPoint); originPos = Vector2f(28, 79); create = 2;
                        }
                        break;
                    case 2 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listPlaceablesb.emplace_back(new Mark_LevelEnd(*marksTxt[8], *marksTxt[9], *effectTxt[6], currentData[0]));
                        originPos = Vector2f(-2, 256);
                        create = 1;
                        break;
                    case 3 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedMoveH(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 4 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedTouchH(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 5 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedMoveV(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 6 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedTouchV(*marksTxt[0], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 7 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 32, entPos.y), Vector2f(96, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformRedFall(*marksTxt[0], listColliderb.back()));

                        originPos = Vector2f(32, 0);
                        create = 3;
                        break;
                    case 8 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortMoveH(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 9 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortTouchH(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 10 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortMoveV(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 11 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortTouchV(*marksTxt[1], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 12 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformShortFall(*marksTxt[1], listColliderb.back()));

                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 13 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudMoveH(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 14 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudTouchH(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 15 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudMoveV(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 16 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudTouchV(*marksTxt[2], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 17 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 45, entPos.y), Vector2f(126, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformCloudFall(*marksTxt[2], listColliderb.back()));

                        originPos = Vector2f(45, 0);
                        create = 3;
                        break;
                    case 18 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneMoveH(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 19 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneTouchH(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 20 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneMoveV(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 21 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);

                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneTouchV(*marksTxt[3], currentData[0], currentData[1], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 22 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 43, entPos.y), Vector2f(120, 32)), Vector2f(0, 0), 1, C_PLATFORM, false, NULL});
                        listAfterb.emplace_back(new Mark_PlatformStoneFall(*marksTxt[3], listColliderb.back()));

                        originPos = Vector2f(43, 0);
                        create = 3;
                        break;
                    case 23 : listMiddleb.emplace_back(new Mark_Spring(*marksTxt[4], 20)); originPos = Vector2f(0, 32); create = 2; break;
                    case 24 : listMiddleb.emplace_back(new Mark_Spring(*marksTxt[5], 28)); originPos = Vector2f(0, 32); create = 2; break;
                    case 25 : listMiddleb.emplace_back(new Mark_Spring(*marksTxt[6], 36)); originPos = Vector2f(0, 32); create = 2; break;
                    case 114 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_Goomba(*enemiesTxt[0], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 115 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_GoombaGrey(*enemiesTxt[1], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 116 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_Spiny(*enemiesTxt[2], listColliderb.back())); originPos = Vector2f(-17, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 117 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_SpinyGrey(*enemiesTxt[3], listColliderb.back())); originPos = Vector2f(-17, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 118 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_BuzzyBeetle(*enemiesTxt[4], enemiesTxt[13], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 119 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 120 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 121 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 122 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listColliderb.back(), false)); originPos = Vector2f(-16, 15); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 123 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaGreen(*enemiesTxt[5], enemiesTxt[14], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 124 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaRed(*enemiesTxt[6], enemiesTxt[15], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 125 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaBlue(*enemiesTxt[7], enemiesTxt[16], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 126 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_KoopaYellow(*enemiesTxt[8], enemiesTxt[17], listColliderb.back(), true)); originPos = Vector2f(-16, 0); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 127 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaGreenH(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 128 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaRedH(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 129 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaBlueH(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 130 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaGreenV(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 131 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaRedV(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 132 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaBlueV(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listColliderb.back(), currentData[0])); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 133 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaGreenJ(*enemiesTxt[9], enemiesTxt[5], enemiesTxt[14], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 134 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaRedJ(*enemiesTxt[10], enemiesTxt[6], enemiesTxt[15], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 135 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaBlueJ(*enemiesTxt[11], enemiesTxt[7], enemiesTxt[16], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 136 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false});
                        listMiddleb.emplace_back(new Sprite_ParatroopaYellow(*enemiesTxt[12], enemiesTxt[8], enemiesTxt[17], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        listColliderb.back()->object = listMiddleb.back();
                        break;
                    case 137 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenT(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 138 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenL(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 139 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenR(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 140 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreenB(enemiesTxt[18], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 141 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireT(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 142 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireL(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 143 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireR(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 144 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantFireB(enemiesTxt[19], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 145 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y + 32), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyT(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(-16, 15); create = 2;
                        break;
                    case 146 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyL(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(16, 15); create = 2;
                        break;
                    case 147 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x, entPos.y - 15), Vector2f(0, 31)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyR(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(0, 15); create = 2;
                        break;
                    case 148 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 16, entPos.y), Vector2f(31, 0)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_PlantGreyB(enemiesTxt[20], listColliderb.back())); originPos = Vector2f(-16, 0); create = 2;
                        break;
                    case 149 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepRed(*enemiesTxt[21], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 150 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepGreen(*enemiesTxt[22], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 151 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepBlue(*enemiesTxt[23], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 152 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(31, 32)), Vector2f(0, 0), 0, C_UNSTOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_CheepCheepYellow(*enemiesTxt[24], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 153 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_HammerBro(*enemiesTxt[25], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 154 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_SilverBro(*enemiesTxt[26], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 155 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_FireBro(*enemiesTxt[27], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 156 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_BeetBro(*enemiesTxt[28], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 157 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_SpikeBro(*enemiesTxt[29], listColliderb.back())); originPos = Vector2f(-16, 16); create = 2;
                        break;
                    case 158 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_LakituWhite(*enemiesTxt[32], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 159 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_LakituDark(*enemiesTxt[33], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 160 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_LakituGold(*enemiesTxt[34], listColliderb.back())); originPos = Vector2f(0, 0); create = 2;
                        break;
                    case 161 : listMiddleb.emplace_back(new Sprite_Poison(*enemiesTxt[37])); originPos = Vector2f(0, 0); create = 2; break;
                    case 162 : listPlaceablesb.emplace_back(new Bonus_PoisonBlock(itemsTxt[0])); originPos = Vector2f(0, 0); create = 1; break;
                    case 163 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_PoisonBlockInv(itemsTxt[0], listColliderb.back())); originPos = Vector2f(0, 0); create = 1;
                        break;
                    case 164 : listMiddleb.emplace_back(new Sprite_ThwompB(*enemiesTxt[38])); originPos = Vector2f(10, 4); create = 2; break;
                    case 165 : listMiddleb.emplace_back(new Sprite_ThwompT(*enemiesTxt[38])); originPos = Vector2f(10, -36); create = 2; break;
                    case 166 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 42, entPos.y - 23), Vector2f(52, 55)), Vector2f(0, 0), 0, C_STOMPABLE, false, NULL});
                        listMiddleb.emplace_back(new Sprite_Bowser(*enemiesTxt[39], listColliderb.back())); originPos = Vector2f(16, 40); create = 2;
                        break;
                    case 167 : listPlaceablesb.emplace_back(new Hazard_LavaT(*hazardsTxt[0])); originPos = Vector2f(0, 2); create = 1; break;
                    case 168 : listPlaceablesb.emplace_back(new Hazard_LavaB(*hazardsTxt[0])); originPos = Vector2f(0, -34); create = 1; break;
                    case 169 : listPlaceablesb.emplace_back(new Hazard_LavaL(*hazardsTxt[0])); originPos = Vector2f(2, 0); create = 1; break;
                    case 170 : listPlaceablesb.emplace_back(new Hazard_LavaR(*hazardsTxt[0])); originPos = Vector2f(-34, 0); create = 1; break;
                    case 172 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooT(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 173 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooB(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 174 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooL(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 175 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        listMiddleb.emplace_back(new Hazard_PodobooR(*hazardsTxt[1], currentData[0])); originPos = Vector2f(-2, 0); create = 2; break;
                    case 176 : listMiddleb.emplace_back(new Hazard_PodobooFountainT); originPos = Vector2f(0, 0); create = 2; break;
                    case 177 : listMiddleb.emplace_back(new Hazard_PodobooFountainB); originPos = Vector2f(0, 0); create = 2; break;
                    case 178 : listMiddleb.emplace_back(new Hazard_PodobooFountainL); originPos = Vector2f(0, 0); create = 2; break;
                    case 179 : listMiddleb.emplace_back(new Hazard_PodobooFountainR); originPos = Vector2f(0, 0); create = 2; break;
                    case 180 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 96))); originPos = Vector2f(0, 0); create = 1; break;
                    case 181 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 192))); originPos = Vector2f(0, 0); create = 1; break;
                    case 182 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 128))); originPos = Vector2f(0, 0); create = 1; break;
                    case 183 : listPlaceablesb.emplace_back(new Hazard_Spike(Vector2i(864, 160))); originPos = Vector2f(0, 0); create = 1; break;
                    case 184 : listMiddleb.emplace_back(new Hazard_CannonHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2; break;
                    case 185 : listMiddleb.emplace_back(new Hazard_CannonHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2; break;
                    case 186 : listMiddleb.emplace_back(new Hazard_CannonVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2; break;
                    case 187 : listMiddleb.emplace_back(new Hazard_CannonVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2; break;
                    case 188 : listMiddleb.emplace_back(new Hazard_CannonHomingHT(*hazardsTxt[2])); originPos = Vector2f(0, -1); create = 2; break;
                    case 189 : listMiddleb.emplace_back(new Hazard_CannonHomingHB(*hazardsTxt[2])); originPos = Vector2f(0, -31); create = 2; break;
                    case 190 : listMiddleb.emplace_back(new Hazard_CannonHomingVL(*hazardsTxt[2])); originPos = Vector2f(-1, 0); create = 2; break;
                    case 191 : listMiddleb.emplace_back(new Hazard_CannonHomingVR(*hazardsTxt[2])); originPos = Vector2f(-31, 0); create = 2; break;
                    case 202 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 203 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 204 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscRCC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 205 :
                        level_file.read(reinterpret_cast<char*>(&currentData[0]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[1]), 4);
                        level_file.read(reinterpret_cast<char*>(&currentData[2]), 4);
                        listAfterb.emplace_back(new Hazard_RotodiscRC(*hazardsTxt[4], Vector2f(currentData[0], currentData[1]), currentData[2])); originPos = Vector2f(0, 0); create = 3;
                        break;
                    case 206 : listMiddleb.emplace_back(new Hazard_LavalauncherL(*hazardsTxt[5])); originPos = Vector2f(0, 0); create = 2; break;
                    case 207 : listMiddleb.emplace_back(new Hazard_LavalauncherR(*hazardsTxt[5])); originPos = Vector2f(-32, 0); create = 2; break;
                    case 208 : listMiddleb.emplace_back(new Hazard_LavalauncherVL(*hazardsTxt[6])); originPos = Vector2f(2, 37); create = 2; break;
                    case 209 : listMiddleb.emplace_back(new Hazard_LavalauncherVR(*hazardsTxt[6])); originPos = Vector2f(-30, 37); create = 2; break;
                    case 210 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y - 6), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddleb.emplace_back(new Hazard_SpikelauncherT(*hazardsTxt[8])); originPos = Vector2f(12, 8); create = 2;
                        break;
                    case 211 :
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x - 10, entPos.y + 2), Vector2f(54, 38)), Vector2f(0, 0), 0, C_IMMOBILE, false, NULL});
                        listMiddleb.emplace_back(new Hazard_SpikelauncherB(*hazardsTxt[8])); originPos = Vector2f(12, -40); create = 2;
                        break;
                    case 212 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 32, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 64, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 96, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 128, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});
                        listColliderb.emplace_back(new Collider{FloatRect(Vector2f(entPos.x + 160, entPos.y), Vector2f(32, 32)), Vector2f(0, 0), 0, C_CENTIPEDE, false, NULL});

                        // Read all nodes :
                        {
                            vector<Vector2f> path;
                            float nodeSize, x, y;

                            level_file.read(reinterpret_cast<char*>(&nodeSize), 4);

                            short t = static_cast<short>(nodeSize);
                            for (short i = 0; i < t; i++)
                            {
                                level_file.read(reinterpret_cast<char*>(&x), 4);
                                level_file.read(reinterpret_cast<char*>(&y), 4);

                                path.emplace_back(Vector2f(x, y));
                            }

                            listAfterb.emplace_back(new Hazard_Centipede(&path.front(), t, listColliderb.end()));
                        }
                        originPos = Vector2f(0, 0);
                        create = 3;
                        break;
                    case 225 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 0)); originPos = Vector2f(-1, -1); create = 1; break;
                    case 226 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 270)); originPos = Vector2f(-1, -32); create = 1; break;
                    case 227 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 90)); originPos = Vector2f(-31, -1); create = 1; break;
                    case 228 : listPlaceablesb.emplace_back(new Sprite_StaticPlant(entPos, 180)); originPos = Vector2f(-31, -31); create = 1; break;
                }

                if (create == 1)
                    listPlaceablesb.back()->setPosition(entPos - originPos);
                else if (create == 2)
                    listMiddleb.back()->setPosition(entPos - originPos);
                else if (create == 3)
                    listAfterb.back()->setPosition(entPos - originPos);
            }
        }
    }

    {
        unsigned short listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
            {
                Vector2f entPos;
                Vector2f originPos;

                unsigned char entType;

                bool toMiddle = false;

                level_file.read(reinterpret_cast<char*>(&entType), 1);

                level_file.read(reinterpret_cast<char*>(&entPos.x), 4);
                level_file.read(reinterpret_cast<char*>(&entPos.y), 4);

                switch (entType)
                {
                    case 26 : listPlaceablesb.emplace_back(new Bonus_MushroomBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 27 : listPlaceablesb.emplace_back(new Bonus_FlowerBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 28 : listPlaceablesb.emplace_back(new Bonus_BeetBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 29 : listPlaceablesb.emplace_back(new Bonus_LuiBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 30 : listPlaceablesb.emplace_back(new Bonus_StarBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 31 : listPlaceablesb.emplace_back(new Bonus_LifeBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 32 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 33 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 34 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 35 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 36 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 37 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 38 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnceb.begin(); it != invisiblesOnceb.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 2));
                            else
                                listPlaceablesb.emplace_back(new Bonus_MushroomBlockInv(itemsTxt[0], listColliderb.back(), 1));
                        }
                        break;
                    case 39 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnceb.begin(); it != invisiblesOnceb.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 2));
                            else
                                listPlaceablesb.emplace_back(new Bonus_FlowerBlockInv(itemsTxt[0], listColliderb.back(), 1));
                        }
                        break;
                    case 40 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnceb.begin(); it != invisiblesOnceb.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 2));
                            else
                                listPlaceablesb.emplace_back(new Bonus_BeetBlockInv(itemsTxt[0], listColliderb.back(), 1));
                        }
                        break;
                    case 41 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnceb.begin(); it != invisiblesOnceb.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 2));
                            else
                                listPlaceablesb.emplace_back(new Bonus_LuiBlockInv(itemsTxt[0], listColliderb.back(), 1));
                        }
                        break;
                    case 42 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnceb.begin(); it != invisiblesOnceb.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 2));
                            else
                                listPlaceablesb.emplace_back(new Bonus_StarBlockInv(itemsTxt[0], listColliderb.back(), 1));
                        }
                        break;
                    case 43 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        originPos = Vector2f(0, 0);

                        {
                            bool coin = false;
                            for (vector<Vector2f>::iterator it = invisiblesOnceb.begin(); it != invisiblesOnceb.end(); it++)
                            {
                                if (entPos == (*it))
                                {
                                    coin = true;
                                    break;
                                }
                            }

                            if (coin)
                                listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 2));
                            else
                                listPlaceablesb.emplace_back(new Bonus_LifeBlockInv(itemsTxt[0], listColliderb.back(), 1));
                        }
                        break;
                    case 44 : listMiddleb.emplace_back(new Sprite_Mushroom(*itemsTxt[3])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 45 : listMiddleb.emplace_back(new Sprite_Flower(*itemsTxt[4])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 46 : listMiddleb.emplace_back(new Sprite_Beet(*itemsTxt[5])); originPos = Vector2f(-2, 2); toMiddle = true; break;
                    case 47 : listMiddleb.emplace_back(new Sprite_Lui(*itemsTxt[6])); originPos = Vector2f(-1, 1); toMiddle = true; break;
                    case 48 : listMiddleb.emplace_back(new Sprite_Star(*itemsTxt[7])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 49 : listMiddleb.emplace_back(new Sprite_Life(*itemsTxt[8])); originPos = Vector2f(0, 0); toMiddle = true; break;
                    case 50 : listPlaceablesb.emplace_back(new Bonus_CoinBlock(itemsTxt[0])); originPos = Vector2f(0, 0); break;
                    case 51 :
                        listColliderb.emplace_back(new Collider{FloatRect(entPos, Vector2f(32, 32)), Vector2f(0, 0), 2, C_IMMOBILE, false, NULL});
                        listPlaceablesb.emplace_back(new Bonus_CoinBlockInv(itemsTxt[0], listColliderb.back(), 0)); originPos = Vector2f(0, 0);
                        break;
                    case 52 : listPlaceablesb.emplace_back(new Bonus_Coin(*itemsTxt[2])); originPos = Vector2f(-6, -2); break;
                    case 53 :
                        listPlaceablesb.emplace_back(new Bonus_Brick(itemsTxt[1])); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 54 : listPlaceablesb.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 10)); originPos = Vector2f(0, 0); break;
                    case 55 : listPlaceablesb.emplace_back(new Bonus_CoinBrick(itemsTxt[1], 15)); originPos = Vector2f(0, 0); break;
                    case 213 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 214 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 0, true);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 215 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 0)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 216 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 217 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 1, true);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 218 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 1)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 219 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 220 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 2, true);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 221 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 2)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                    case 222 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, false);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 1;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 223 :
                    {
                        Bonus_Toggle* toggle = new Bonus_Toggle(marksTxt[11], 3, true);
                        unsigned char* c = collisionMatrixb->getReference(entPos.x/32, entPos.y/32);

                        toggle->attributeSolid(c);

                        (*c) = 0;

                        listPlaceablesb.emplace_back(toggle);

                        originPos = Vector2f(0, 0);
                        break;
                    }
                    case 224 :
                        listPlaceablesb.emplace_back(new Bonus_Switch(marksTxt[10], 3)); originPos = Vector2f(0, 0);
                        collisionMatrixb->setValue(entPos.x/32, entPos.y/32, 1);
                        break;
                }

                if (toMiddle)
                    listMiddleb.back()->setPosition(entPos - originPos);
                else
                    listPlaceablesb.back()->setPosition(entPos - originPos);
            }
        }
    }

    // Necessary to not perturbate the cursor position in the case of a compiled scenario :
    if (loadingType == 1)
    {
        unsigned char listSize;

        level_file.read(reinterpret_cast<char*>(&listSize), 1);

        if (listSize != 0)
        {
            for (register unsigned short i = 0; i < listSize; i++)
                level_file.seekg(sizeof(WarpsData), ios_base::cur);
        }
    }

    if (!listMiddle.empty())
    {
        for (vector<Placeable*>::iterator it = listMiddle.begin(); it != listMiddle.end(); it++)
            listPlaceables.emplace_back(*it);
    }

    spriteIterator = listPlaceables.end();

    if (!listAfter.empty())
    {
        for (vector<Placeable*>::iterator it = listAfter.begin(); it != listAfter.end(); it++)
            listPlaceables.emplace_back(*it);
    }

    if (!listMiddleb.empty())
    {
        for (vector<Placeable*>::iterator it = listMiddleb.begin(); it != listMiddleb.end(); it++)
            listPlaceablesb.emplace_back(*it);
    }

    spriteIteratorb = listPlaceablesb.end();

    if (!listAfterb.empty())
    {
        for (vector<Placeable*>::iterator it = listAfterb.begin(); it != listAfterb.end(); it++)
            listPlaceablesb.emplace_back(*it);
    }

    listMiddle.clear();
    listAfter.clear();

    listMiddleb.clear();
    listAfterb.clear();

    if (startZone == 1)
    {
        zoneb = true;
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZoneb, 0, &musicChannel);
    }
    else
    {
        zoneb = false;
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicZonea, 0, &musicChannel);
    }

    player = new Mark_Mario(marioTxt, effectTxt[15]);
    player->setPosition(startPos + Vector2f(16, 0));

    if (currentCheckpoint != NULL)
        player->m_checkpoint = currentCheckpoint;

    {
        Vector2f playerPos = startPos - Vector2f(5, 32);

        if (playerPos.x < 298)
            playerPos.x = 298;

        if (playerPos.y < 208)
            playerPos.y = 208;

        if (zoneb)
        {
            if (playerPos.x > (levelScaleb.x * 640) - 341)
                playerPos.x = (levelScaleb.x * 640) - 341;

            if (playerPos.y > (levelScaleb.y * 480) - 272)
                playerPos.y = (levelScaleb.y * 480) - 272;
        }
        else
        {
            if (playerPos.x > (levelScale.x * 640) - 341)
                playerPos.x = (levelScale.x * 640) - 341;

            if (playerPos.y > (levelScale.y * 480) - 272)
                playerPos.y = (levelScale.y * 480) - 272;
        }

        cameraPos = Vector2f(playerPos.x + 21, playerPos.y + 32);

        if (!autoscrollPath.empty())
        {
            Vector2f firstnode(autoscrollPath.front());

            if (firstnode.x > cameraPos.x - 300 && firstnode.x < cameraPos.x + 300 && firstnode.y > cameraPos.y - 220 && firstnode.y < cameraPos.y + 220)
            {
                cameraPos = firstnode;
                autoscroll_node = 1;
            }
        }
    }

    {
        char buffer[6]("");

        sprintf(buffer, "%d", timer/25);

        timerFont->setText(buffer, 6, Vector2f(577, 36), true);
    }
}

static void Level_Clear()
{
    for (list<Placeable*>::iterator it = listPlaceables.begin(); it != listPlaceables.end(); it++)
        delete *it;

    for (list<Collider*>::iterator it = listCollider.begin(); it != listCollider.end(); it++)
        delete *it;

    for (list<Placeable*>::iterator it = listPlaceablesb.begin(); it != listPlaceablesb.end(); it++)
        delete *it;

    for (list<Collider*>::iterator it = listColliderb.begin(); it != listColliderb.end(); it++)
        delete *it;

    for (list<WarpsData*>::iterator it = listPassages.begin(); it != listPassages.end(); it++)
        delete *it;

    for (vector<TriggerData*>::iterator it = liquidSpots.begin(); it != liquidSpots.end(); it++)
        delete *it;

    for (vector<TriggerData*>::iterator it = liquidSpotsb.begin(); it != liquidSpotsb.end(); it++)
        delete *it;

    for (list<Effect*>::iterator it = listEffect.begin(); it != listEffect.end(); it++)
        delete *it;

    tilemapLayer1.clear();
    tilemapLayer2.clear();
    tilemapLayer3.clear();

    listPlaceables.clear();
    listCollider.clear();

    tilemapLayer1b.clear();
    tilemapLayer2b.clear();
    tilemapLayer3b.clear();

    listPlaceablesb.clear();
    listColliderb.clear();

    listPassages.clear();

    listEffect.clear();

    liquidSpots.clear();
    liquidSpotsb.clear();

    autoscrollPath.clear();

    if (loadingType != 1)
    {
        if (level_file.is_open())
            level_file.close();
    }

    if (backgroundTxt[12] != NULL)
    {
        delete backgroundTxt[12];
        backgroundTxt[12] = NULL;
    }

    if (backgroundTxt[13] != NULL)
    {
        delete backgroundTxt[13];
        backgroundTxt[13] = NULL;
    }

    delete livesFont;
    delete timerFont;
    delete worldFont;

    delete player;

    if (backgroundSpr != NULL)
    {
        delete backgroundSpr;
        backgroundSpr = NULL;
    }

    if (effectCloudsSpr[0] != NULL)
    {
        delete effectCloudsSpr[0];
        effectCloudsSpr[0] = NULL;
    }

    if (effectCloudsSpr[1] != NULL)
    {
        delete effectCloudsSpr[1];
        effectCloudsSpr[1] = NULL;
    }

    if (effectWeatherSpr != NULL)
    {
        delete effectWeatherSpr;
        effectWeatherSpr = NULL;
    }

    if (backgroundSprb != NULL)
    {
        delete backgroundSprb;
        backgroundSprb = NULL;
    }

    if (effectCloudsSprb[0] != NULL)
    {
        delete effectCloudsSprb[0];
        effectCloudsSprb[0] = NULL;
    }

    if (effectCloudsSprb[1] != NULL)
    {
        delete effectCloudsSprb[1];
        effectCloudsSprb[1] = NULL;
    }

    if (effectWeatherSprb != NULL)
    {
        delete effectWeatherSprb;
        effectWeatherSprb = NULL;
    }

    if (watera != NULL)
    {
        delete watera;
        watera = NULL;
    }

    if (waterb != NULL)
    {
        delete waterb;
        waterb = NULL;
    }

    delete collisionMatrix;
    delete collisionMatrixb;

    {
        FMOD_BOOL isPlaying;

        FMOD_Channel_IsPlaying(musicChannel, &isPlaying);

        if (isPlaying)
            FMOD_Channel_Stop(musicChannel);

        if (musicSamples[22] != NULL)
        {
            FMOD_Sound_Release(musicSamples[22]);
            musicSamples[22] = NULL;
        }

        if (musicSamples[23] != NULL)
        {
            FMOD_Sound_Release(musicSamples[23]);
            musicSamples[23] = NULL;
        }

        if (musicSamples[24] != NULL)
        {
            FMOD_Sound_Release(musicSamples[24]);
            musicSamples[24] = NULL;
        }
    }
}

static bool World_Load()
{
    Vector2f startPos(320, 240);

    frame_Mario = 0;
    frame_Progress = 0;
    frame_Start = 0;

    titlePos = -360;
    titleSpd = 0;

    levelsFinished = 0;
    marksDelta = 0;
    marksDelta_phase = true;

    map_completed = 0;

    scenario_file.open(scenarioToLoad, ios::binary);

    if (!scenario_file.good())
    {
        MessageBox(NULL, "Failed to open the World file !", "Error !", MB_OK | MB_TASKMODAL | MB_ICONERROR);
        return false;
    }

    scenario_file.seekg(4, ios::beg);

    {
        TCHAR filePath[MAX_PATH];
        TCHAR getString[MAX_PATH];

        strcpy(filePath, scenarioToLoad.c_str());

        PathRemoveFileSpec(filePath);
        SetCurrentDirectory(filePath);

        for (register unsigned int i = 0; i < 2; i++)
        {
            scenario_file.read(getString, 1);

            if (getString[0] != '\0')
            {
                scenario_file.seekg(-1, ios::cur);

                for (register unsigned int j = 0; true; j++)
                {
                    scenario_file.read(&getString[j], 1);

                    if (getString[j] == '\0')
                        break;
                }

                switch (i)
                {
                    case 0 :
                        FMOD_System_CreateStream(soundSystem, getString, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_2D, NULL, &musicSamples[28]);
                        break;

                    case 1 :
                        worldBackTex[1] = new Texture;
                        worldBackTex[1]->loadFromFile(getString);
                        worldBackTex[1]->setRepeated(true);
                        break;
                }
            }
        }

        // Reset the current directory :
        GetModuleFileName(NULL, filePath, MAX_PATH);
        PathRemoveFileSpec(filePath);
        SetCurrentDirectory(filePath);
    }

    scenario_file.read(reinterpret_cast<char*>(&mpScale.x), 4);
    scenario_file.read(reinterpret_cast<char*>(&mpScale.y), 4);

    {
        unsigned char bck;

        scenario_file.read(reinterpret_cast<char*>(&bck), 1);

        mapBack = new RectangleShape(Vector2f(mpScale.x * 32, mpScale.y * 32));
        mapBack->setTexture(worldBackTex[bck]);
        mapBack->setTextureRect(IntRect(0, 0, mpScale.x * 32, mpScale.y * 32));
    }

    {
        unsigned char mus;

        scenario_file.read(reinterpret_cast<char*>(&mus), 1);

        if (mus < 22)
            musicWorld = &musicSamples[mus];
        else
            musicWorld = &musicSamples[28];
    }

    #ifdef PUBLISHER
    {
        unsigned char lives;

        scenario_file.read(reinterpret_cast<char*>(&lives), 1);

        if (target_lives == 0)
            game_lives = lives;
    }
    #else
        scenario_file.read(reinterpret_cast<char*>(&game_lives), 1);
    #endif

    {
        unsigned short listSize;

        scenario_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                unsigned int id;

                Vector2f pos;
                Vector2f size;

                scenario_file.read(reinterpret_cast<char*>(&id), 4);
                scenario_file.read(reinterpret_cast<char*>(&pos.x), 4);
                scenario_file.read(reinterpret_cast<char*>(&pos.y), 4);
                scenario_file.read(reinterpret_cast<char*>(&size.x), 4);
                scenario_file.read(reinterpret_cast<char*>(&size.y), 4);

                RectangleShape* tex = new RectangleShape(size);

                tex->setPosition(pos);
                tex->setTexture(worldTextures[id-1]);
                tex->setTextureRect(IntRect(0, 0, size.x, size.y));

                listTex.emplace_back(tex);
            }
        }
    }

    {
        unsigned short listSize;

        scenario_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                unsigned int id;

                Vector2f pos;

                scenario_file.read(reinterpret_cast<char*>(&id), 4);
                scenario_file.read(reinterpret_cast<char*>(&pos.x), 4);
                scenario_file.read(reinterpret_cast<char*>(&pos.y), 4);

                Sprite* spr = new Sprite(*worldEyecandies[id]);

                spr->setPosition(pos);

                listEye.emplace_back(spr);
            }
        }
    }

    {
        unsigned short listSize;

        scenario_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            for (register unsigned int i = 0; i < listSize; i++)
            {
                LightData currentData;
                Lamp* light;

                scenario_file.read(reinterpret_cast<char*>(&currentData), sizeof(LightData));

                light = new Lamp;

                light->color = currentData.lightColor;
                light->litTextures = currentData.litTextures;
                light->litEyecandies = currentData.litEyecandies;
                light->x = currentData.x;
                light->y = currentData.y;

                float deltax = currentData.x - currentData.moveablex;
                float deltay = currentData.y - currentData.moveabley;
                light->radius = sqrtf((deltax * deltax) + (deltay * deltay));

                listLamps.emplace_back(light);
            }
        }
    }

    {
        unsigned short listSize;

        scenario_file.read(reinterpret_cast<char*>(&listSize), 2);

        if (listSize != 0)
        {
            MarkerData* currentData;
            float lastx, lasty;

            for (register unsigned int i = 0; i < listSize; i++)
            {
                currentData = new MarkerData;
                Vector2f pos;

                scenario_file.read(reinterpret_cast<char*>(currentData), sizeof(MarkerData));

                if (i == 0)
                {
                    startPos = Vector2f(currentData->x, currentData->y);

                    if (currentData->isLevel)
                    {
                        marioSpr_speed = Vector2f(0, 0);
                        fileToLoad = currentData->levelURL;
                    }
                }
                else if (i == 1 && marioSpr_speed != Vector2f(0, 0))
                {
                    float angle = atan2(startPos.y - currentData->y, startPos.x - currentData->x);

                    marioSpr_speed = Vector2f(-WORLDMAP_MARIOSPEED * cos(angle), -WORLDMAP_MARIOSPEED * sin(angle));
                }

                currentData->x += 2;
                currentData->y += 4;

                listMark.emplace_back(currentData);
            }

            // Append a last marker at the same position :
            lastx = currentData->x;
            lasty = currentData->y;

            currentData = new MarkerData;

            currentData->x = lastx;
            currentData->y = lasty;
            currentData->isLevel = false;

            listMark.emplace_back(currentData);
        }
    }

    levelsPassed = listMark.begin();

    if (listMark.size() > 1)
        levelsPassed++;

    marioSpr->setPosition(startPos.x + 2, startPos.y - 4);

    if (startPos.x < 320)
        startPos.x = 320;

    if (startPos.y < 240)
        startPos.y = 240;

    if (startPos.x > (mpScale.x * 32) - 320)
        startPos.x = (mpScale.x * 32) - 320;

    if (startPos.y > (mpScale.y * 32) - 240)
        startPos.y = (mpScale.y * 32) - 240;

    mapPos = startPos;

    sheetSpr->setPosition(startPos.x + 213, startPos.y + 163);

    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), *musicWorld, 0, &musicChannel);

    return true;
}

void addCoin()
{
    char coins[3]("");

    if (game_coins < 99)
        game_coins++;
    else
    {
        game_coins = 0;
        addLife();
    }

    sprintf(coins, "%u", game_coins);

    coinsFont->setText(coins, 3, Vector2f(cameraPos.x - 35, cameraPos.y - 204), false);
}

void addPoints(unsigned int points)
{
    char score[7]("");

    game_score += points;

    if (game_score > 999999)
        game_score = 999999;

    sprintf(score, "%u", game_score);

    scoreFont->setText(score, 7, Vector2f(cameraPos.x - 167, cameraPos.y - 204), true);
}

void addLife()
{
    // Test in editor :
    if (game_lives == 255)
        return;

    char lives[3]("");

    if (game_lives < 99)
        game_lives++;

    sprintf(lives, "%u", game_lives);

    livesFont->setText(lives, 3, Vector2f(cameraPos.x - 182, cameraPos.y - 224), false);
}

void subLife()
{
    // Test in editor :
    if (game_lives == 255)
        return;

    char lives[3]("");

    if (game_lives > 0)
        game_lives--;

    sprintf(lives, "%u", game_lives);

    livesFont->setText(lives, 3, Vector2f(cameraPos.x - 182, cameraPos.y - 224), false);
}
