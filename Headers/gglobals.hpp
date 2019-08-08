/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <SFML/Graphics.hpp>

extern "C"
{
    #include <FMODEx/fmod.h>
    #include <windows.h>
}

#include <string>
#include <list>

#include "Core/gscene.hpp"
#include "Core/effect.hpp"
#include "Graphics/entwarps.hpp"
#include "Graphics/spritefont.hpp"
#include "Graphics/trigger.hpp"
#include "Marks/mark_mario.hpp"
#include "Marks/mark_liquidplane.hpp"

#define lerp(a, b, t) fma(t, b, fma(-t, a, a))

#define EDITOR_VERSION 1

extern sf::RenderTexture mainTexture;
extern sf::Sprite textureRender;

extern sf::RenderWindow* mainWindow;
extern sf::Sprite* loading;

extern sf::Vector2f wpos;

extern FMOD_SYSTEM* soundSystem;
extern FMOD_SOUND* music;
extern FMOD_CHANNEL* musicChannel;

extern HINSTANCE* mainInstance;
extern NOTIFYICONDATA trayIcon;

extern Scenes targetScene;

extern std::string fileToLoad;
extern std::string scenarioToLoad;
extern char loadingType;

extern unsigned char killcount;
extern unsigned char game_powerup;
extern unsigned char game_coins;
extern unsigned char game_lives;
extern unsigned int game_score;

extern float windowScale;
extern bool restartLevel;
extern bool enableShaders;

// Ingame globals :
extern sf::Color levelBackColor;
extern sf::Color levelBackColorb;

extern sf::Vector2i levelScale;
extern sf::Vector2i levelScaleb;

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
extern FMOD_SOUND* sfxSamples[34];

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

extern sf::Texture* backgroundTxt[14];
extern sf::Texture* sceneriesTxt[10];
extern sf::Texture* effectTxt[16];
extern sf::Texture* itemsTxt[11];
extern sf::Texture* enemiesTxt[41];
extern sf::Texture* hazardsTxt[11];
extern sf::Texture* marksTxt[10];
extern sf::Texture* marioTxt[6];

extern sf::Texture* worldLightTex;
extern sf::Texture* entTex[7];
extern sf::Texture* worldBackTex[2];
extern sf::Texture* worldTextures[166];
extern sf::Texture* worldEyecandies[73];
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

extern sf::RectangleShape* effectCloudsSpr;
extern sf::RectangleShape* effectCloudsSprb;

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

extern unsigned int effectCloudPos;
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

bool checkLoadResources(std::ifstream& levelFile, LPCSTR filename);

#endif
