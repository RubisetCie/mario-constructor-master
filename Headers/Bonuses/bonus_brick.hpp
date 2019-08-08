/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKBRICK_INCLUDED
#define BLOCKBRICK_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Bonus_Brick : public Placeable
{
    public :

        Bonus_Brick(sf::Texture* brickTexture);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        char m_hit;

        float m_startheight;
        float m_hitspeed;
};

#endif
