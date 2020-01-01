/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPARATROOPA_INCLUDED
#define SPRITEPARATROOPA_INCLUDED

#include <SFML/Graphics.hpp>
#include "gotspeed.hpp"
#include "gotcollision.hpp"
#include "../Core/pawn.hpp"

class Sprite_Paratroopa : public Pawn, public GotCollision, public GotSpeed
{
    public :

        Sprite_Paratroopa(const sf::Texture& sprite_texture, sf::Texture* const turtle_texture, sf::Texture* const shell_texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        virtual void flyUpdate() = 0;
        void walkUpdate();

        virtual void toTurtle() = 0;

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    protected :

        sf::Sprite m_sprite;

        sf::Texture* m_shelltexture;
        sf::Texture* m_turtletexture;

        unsigned char m_state;
        unsigned char m_frame;

        bool m_onfloor, m_active;
};

#endif
