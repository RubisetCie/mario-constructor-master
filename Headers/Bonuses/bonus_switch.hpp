/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKSWITCH_INCLUDED
#define BLOCKSWITCH_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Bonus_Switch : public Placeable
{
    public :

        Bonus_Switch(sf::Texture* blockTexture, unsigned char slot);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        float m_startheight;
        float m_hitspeed;

        char m_hit;

        unsigned char m_slot;
};

#endif
