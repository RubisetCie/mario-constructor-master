/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <SFML/Graphics.hpp>

extern "C"
{
    #include <fmod.h>
    #ifndef LINUX
    #include <windows.h>
    #endif
}

#include <string>
#include <list>

#include "Core/scene.hpp"
#include "Core/effect.hpp"
#include "Graphics/entwarps.hpp"
#include "Graphics/spritefont.hpp"
#include "Graphics/trigger.hpp"
#include "Marks/mark_mario.hpp"
#include "Marks/mark_liquidplane.hpp"

#define lerp(a, b, t) fma(t, b, fma(-t, a, a))

#define EDITOR_VERSION 2

#ifdef LINUX
#define MAX_PATH 1024
#endif

#define TILE_PIPE_GREEN_VTOP        IntRect(576, 0, 64, 32)
#define TILE_PIPE_GREEN_VMID        IntRect(576, 32, 64, 32)
#define TILE_PIPE_GREEN_VBOT        IntRect(576, 64, 64, 32)
#define TILE_PIPE_GREEN_HLEF        IntRect(576, 96, 32, 64)
#define TILE_PIPE_GREEN_HMID        IntRect(608, 96, 32, 64)
#define TILE_PIPE_GREEN_HRIG        IntRect(640, 96, 32, 64)
#define TILE_PIPE_BLUE_VTOP         IntRect(640, 0, 64, 32)
#define TILE_PIPE_BLUE_VMID         IntRect(640, 32, 64, 32)
#define TILE_PIPE_BLUE_VBOT         IntRect(640, 64, 64, 32)
#define TILE_PIPE_BLUE_HLEF         IntRect(672, 96, 32, 64)
#define TILE_PIPE_BLUE_HMID         IntRect(704, 96, 32, 64)
#define TILE_PIPE_BLUE_HRIG         IntRect(736, 96, 32, 64)
#define TILE_PIPE_RED_VTOP          IntRect(704, 0, 64, 32)
#define TILE_PIPE_RED_VMID          IntRect(704, 32, 64, 32)
#define TILE_PIPE_RED_VBOT          IntRect(704, 64, 64, 32)
#define TILE_PIPE_RED_HLEF          IntRect(768, 96, 32, 64)
#define TILE_PIPE_RED_HMID          IntRect(800, 96, 32, 64)
#define TILE_PIPE_RED_HRIG          IntRect(832, 96, 32, 64)
#define TILE_PIPE_YELLOW_VTOP       IntRect(768, 0, 64, 32)
#define TILE_PIPE_YELLOW_VMID       IntRect(768, 32, 64, 32)
#define TILE_PIPE_YELLOW_VBOT       IntRect(768, 64, 64, 32)
#define TILE_PIPE_YELLOW_HLEF       IntRect(576, 160, 32, 64)
#define TILE_PIPE_YELLOW_HMID       IntRect(608, 160, 32, 64)
#define TILE_PIPE_YELLOW_HRIG       IntRect(640, 160, 32, 64)
#define TILE_PIPE_GRAY_VTOP         IntRect(832, 0, 64, 32)
#define TILE_PIPE_GRAY_VMID         IntRect(832, 32, 64, 32)
#define TILE_PIPE_GRAY_VBOT         IntRect(832, 64, 64, 32)
#define TILE_PIPE_GRAY_HLEF         IntRect(672, 160, 32, 64)
#define TILE_PIPE_GRAY_HMID         IntRect(704, 160, 32, 64)
#define TILE_PIPE_GRAY_HRIG         IntRect(736, 160, 32, 64)
#define TILE_PIPE_DARK_VTOP         IntRect(896, 0, 64, 32)
#define TILE_PIPE_DARK_VMID         IntRect(896, 32, 64, 32)
#define TILE_PIPE_DARK_VBOT         IntRect(896, 64, 64, 32)
#define TILE_PIPE_DARK_HLEF         IntRect(768, 160, 32, 64)
#define TILE_PIPE_DARK_HMID         IntRect(800, 160, 32, 64)
#define TILE_PIPE_DARK_HRIG         IntRect(832, 160, 32, 64)

#define TILE_SCENERY_GREEN_TREE     IntRect(576, 320, 32, 64)
#define TILE_SCENERY_GREEN_BIGTREE  IntRect(607, 288, 32, 96)
#define TILE_SCENERY_GRAY_TREE      IntRect(640, 320, 32, 64)
#define TILE_SCENERY_GRAY_BIGTREE   IntRect(671, 288, 32, 96)
#define TILE_SCENERY_YELLOW_TREE    IntRect(704, 320, 32, 64)
#define TILE_SCENERY_YELLOW_BIGTREE IntRect(735, 288, 32, 96)
#define TILE_SCENERY_RED_TREE       IntRect(768, 320, 32, 64)
#define TILE_SCENERY_RED_BIGTREE    IntRect(799, 288, 32, 96)
#define TILE_SCENERY_BLUE_TREE      IntRect(832, 320, 32, 64)
#define TILE_SCENERY_BLUE_BIGTREE   IntRect(863, 288, 32, 96)
#define TILE_SCENERY_FENCE          IntRect(576, 256, 160, 32)
#define TILE_SCENERY_SNOWFENCE      IntRect(735, 256, 160, 32)

#define TILE_HAZARD_LAVABOTTOM      IntRect(928, 96, 32, 32)
#define TILE_HAZARD_SPIKETOP        IntRect(864, 96, 32, 32)
#define TILE_HAZARD_SPIKEBOTTOM     IntRect(864, 192, 32, 32)
#define TILE_HAZARD_SPIKELEFT       IntRect(864, 128, 32, 32)
#define TILE_HAZARD_SPIKERIGHT      IntRect(864, 160, 32, 32)
#define TILE_HAZARD_BBILLTOP        IntRect(608, 224, 32, 32)
#define TILE_HAZARD_BBILLTOPB       IntRect(576, 224, 32, 32)
#define TILE_HAZARD_BBILLMIDV       IntRect(640, 224, 32, 32)
#define TILE_HAZARD_BBILLBOT        IntRect(704, 224, 32, 32)
#define TILE_HAZARD_BBILLBOTB       IntRect(672, 224, 32, 32)
#define TILE_HAZARD_BBILLLEF        IntRect(768, 224, 32, 32)
#define TILE_HAZARD_BBILLLEFB       IntRect(736, 224, 32, 32)
#define TILE_HAZARD_BBILLMIDH       IntRect(800, 224, 32, 32)
#define TILE_HAZARD_BBILLRIG        IntRect(864, 224, 32, 32)
#define TILE_HAZARD_BBILLRIGB       IntRect(832, 224, 32, 32)

extern sf::RenderTexture mainTexture;
extern sf::Sprite textureRender;

extern sf::RenderWindow* mainWindow;
extern sf::Sprite* loading;
extern sf::Sprite* cursor;

extern sf::Vector2f wpos;
extern sf::Vector2f mpos_relative;
extern sf::Vector2i mpos_absolute;

extern FMOD_SYSTEM* soundSystem;
extern FMOD_SOUND* music;
extern FMOD_CHANNEL* musicChannel;

#ifndef LINUX
extern HINSTANCE* mainInstance;
extern NOTIFYICONDATA trayIcon;

extern COLORREF colorArray[16];
#endif

extern Scenes targetScene;

extern std::string fileToLoad;
extern std::string scenarioToLoad;
extern char loadingType;

extern unsigned char killcount;
extern unsigned char game_powerup;
extern unsigned char game_coins;
extern unsigned char game_lives;
extern unsigned int game_score;

extern unsigned char game_toggleSolidity[4];
extern unsigned char game_toggleSoliditySave[4];

extern float windowScale;
extern bool restartLevel;
extern bool editorMusic;
extern bool editorMouseScrolling;
extern bool enableShaders;

extern char mousePressed;
extern char rightPressed;
extern char middlPressed;

// Ingame globals :
extern sf::Color levelBackColor;
extern sf::Color levelBackColorb;

extern sf::Vector2u levelScale;
extern sf::Vector2u levelScaleb;

extern sf::Vector2f cameraPos;
extern float levelCamera_shake;

extern sf::Vector2f worldNamePos;

extern Mark_Mario* player;

extern std::vector<sf::Vertex> tilemapLayer1;
extern std::vector<sf::Vertex> tilemapLayer2;
extern std::vector<sf::Vertex> tilemapLayer3;

extern std::vector<sf::Vertex> tilemapLayer1b;
extern std::vector<sf::Vertex> tilemapLayer2b;
extern std::vector<sf::Vertex> tilemapLayer3b;

extern std::list<Collider*> listCollider;
extern std::list<Collider*> listColliderb;

extern std::list<Placeable*> listPlaceables;
extern std::list<Placeable*> listPlaceablesb;

extern std::list<WarpsData*> listPassages;

extern std::list<Effect*> listEffect;

extern std::list<Placeable*>::iterator spriteIterator;
extern std::list<Placeable*>::iterator spriteIteratorb;

// Ingame resources :
extern FMOD_SOUND* musicSamples[30];
extern FMOD_SOUND* sfxSamples[35];

extern sf::Vertex gradientRectangle[4];
extern sf::Vertex gradientRectangleb[4];

extern sf::Shader* glowShaderH;
extern sf::Shader* glowShaderV;

extern sf::RenderTexture* renderPasses[6];
extern sf::Sprite* renderPassSpr[2];

extern bool clearColora;
extern bool clearColorb;

extern bool exitToTitle;
extern bool nextLevel;
extern bool timerAlertPhase;
extern bool restartLevel;
extern bool zoneb;
extern bool useShaders;

extern sf::Texture* backgroundTxt[16];
extern sf::Texture* sceneriesTxt[14];
extern sf::Texture* effectTxt[16];
extern sf::Texture* itemsTxt[11];
extern sf::Texture* enemiesTxt[42];
extern sf::Texture* hazardsTxt[11];
extern sf::Texture* marksTxt[12];
extern sf::Texture* marioTxt[6];

extern sf::Texture* worldLightTex;
extern sf::Texture* entTex[7];
extern sf::Texture* worldBackTex[2];
extern sf::Texture* worldTextures[166];
extern sf::Texture* worldEyecandies[81];
extern sf::Sprite* worldLightSpr[5];

extern sf::Texture* hudTxt[7];

extern sf::Texture* mainTileset;

extern sf::Sprite* hudSpr[4];
extern sf::VertexArray* hudBowser;
extern float hudBowser_descend;

extern SpriteFont* scoreFont;
extern SpriteFont* livesFont;
extern SpriteFont* coinsFont;
extern SpriteFont* worldFont;
extern SpriteFont* timerFont;

extern sf::RectangleShape* backgroundSpr;
extern sf::RectangleShape* backgroundSprb;

extern sf::RectangleShape* effectCloudsSpr[2];
extern sf::RectangleShape* effectCloudsSprb[2];

extern sf::RectangleShape* effectWeatherSpr;
extern sf::RectangleShape* effectWeatherSprb;
extern sf::Sprite* effectLightningSpr;

extern unsigned short effectLightning;

extern sf::FloatRect blockhitter;
extern sf::FloatRect spritehitter;
extern sf::FloatRect enemystomper;
extern sf::FloatRect enemykiller;
extern sf::FloatRect blockkiller;
extern Collider* enemystomper_pt;
extern char enemykiller_fire;
extern bool enemykiller_create;
extern bool blockkiller_create;
extern bool spritehitter_create;

extern Matrix* collisionMatrix;
extern Matrix* collisionMatrixb;

extern Mark_LiquidPlane* watera;
extern Mark_LiquidPlane* waterb;

extern unsigned int effectCloudPos[2];
extern unsigned int effectLavafallPos;
extern unsigned int effectWeatherPos;

extern unsigned int frame_Hud;
extern unsigned int frame_Water;
extern unsigned int frame_Lava;
extern unsigned int frame_Flame;
extern unsigned int frame_Podoboo;
extern unsigned int frame_Clouds;
extern unsigned int frame_Flower;
extern unsigned int frame_Rotodisc;

extern int keybindings[6];

extern float mario_gravity;
extern float sprite_gravity;
extern float star_gravity;

extern sf::View* levelCamera;

extern FMOD_SOUND** musicZonea;
extern FMOD_SOUND** musicZoneb;

extern unsigned char gravity;

extern unsigned char autoscroll;
extern unsigned char autoscroll_node;
extern unsigned char autoscroll_nodesaved;
extern float autoscrollSpeed;

extern int timer;
extern int levelTimer;

extern std::vector<sf::Vector2f> autoscrollPath;

extern float liquidStartHeight;
extern float liquidStartHeightb;
extern float liquidTargetHeight;
extern float liquidTargetHeightb;
extern float liquidSpeed;

extern float timerAlert;

extern std::vector<TriggerData*> liquidSpots;
extern std::vector<TriggerData*> liquidSpotsb;

extern bool koopaAttacks[6];
extern unsigned short koopaFrequences[6];
extern float koopaSpeeds[6];
extern unsigned char koopaTrail;
extern unsigned char koopaHealth;
extern unsigned char koopaMusic;
extern unsigned char koopaEngaged;

void addCoin();
void addPoints(unsigned int points);
void addLife();
void subLife();

bool checkLoadResources(std::ifstream& levelFile, const char* filename);

#endif
