/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEKOOPAYELLOW_INCLUDED
#define SPRITEKOOPAYELLOW_INCLUDED

#include <SFML/Graphics.hpp>
#include "walkable.hpp"

class Sprite_KoopaYellow : public Walkable
{
    public :

        Sprite_KoopaYellow(const sf::Texture& sprite_texture, sf::Texture* const shell_texture, Collider* collision, bool shell);

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

        Collider* m_collider;

        unsigned char m_state;
        unsigned char m_frame;
};

#endif
