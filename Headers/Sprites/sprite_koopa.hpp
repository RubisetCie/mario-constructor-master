/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEKOOPA_INCLUDED
#define SPRITEKOOPA_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"
#include "gotcollision.hpp"

class Sprite_Koopa : public Walkable, public GotCollision
{
    public :

        Sprite_Koopa(const sf::Texture& sprite_texture, sf::Texture* const shell_texture, Collider* collision, bool shell);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    protected :

        sf::Texture* m_shelltexture;

        unsigned char m_state;
        unsigned char m_frame;
};

#endif
