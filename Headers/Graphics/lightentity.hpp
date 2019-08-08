/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef LIGHT_ENTITY_INCLUDED
#define LIGHT_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>

typedef struct LightData
{
    float x;
    float y;

    float moveablex;
    float moveabley;

    bool litTextures;
    bool litEyecandies;

    sf::Color lightColor;
} LightData;

class Light_Entity : public sf::Drawable
{
    public :

        Light_Entity(sf::Texture* lamptexture, sf::Texture* movetexture, sf::Vector2f position = sf::Vector2f(0, 0), const sf::Color& lightcolor = sf::Color::White, bool litTex = true, bool litEye = true);
        virtual ~Light_Entity();

        sf::Vector2f getPosition() const;

        void setMoveablePos(float x, float y);
        void setColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);
        void setAlpha(sf::Uint8 alpha);
        void setTexIllumination(bool state);
        void setEyeIllumination(bool state);

        float getRadius();

        void mapResized(sf::Vector2i newsize);

        void save(LightData& lightData);
        void load(LightData& lightData);

        void update();
        void drawb(sf::RenderTarget* target) const;

        sf::Color m_lightcolor;

        bool m_littextures;
        bool m_liteyecandies;

        char* identifier;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Sprite m_moveable;

        sf::CircleShape m_circleradius;

        sf::Vertex m_linelink[2];

        float m_radius;

        bool m_grabmover;
        bool m_readytoselection;


};

#endif
