/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPARATROOPAYELLOW_INCLUDED
#define SPRITEPARATROOPAYELLOW_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "gotspeed.hpp"
#include "gotcollision.hpp"

class Sprite_ParatroopaYellow : public Pawn, public GotCollision, public GotSpeed
{
    public :

        Sprite_ParatroopaYellow(const sf::Texture& sprite_texture, sf::Texture* const turtle_texture, sf::Texture* const shell_texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        void flyUpdate();
        void walkUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        sf::Texture* m_shelltexture;
        sf::Texture* m_turtletexture;

        unsigned char m_state;
        unsigned char m_frame;

        bool m_onfloor, m_active;
};

#endif
