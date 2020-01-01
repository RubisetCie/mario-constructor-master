/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEBUZZYBEETLE_INCLUDED
#define SPRITEBUZZYBEETLE_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"
#include "gotcollision.hpp"

class Sprite_BuzzyBeetle : public Walkable, public GotCollision
{
    public :

        Sprite_BuzzyBeetle(const sf::Texture& sprite_texture, sf::Texture* const shell_texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Texture* m_shelltexture;

        unsigned char m_state;
        unsigned char m_frame;
};

#endif
