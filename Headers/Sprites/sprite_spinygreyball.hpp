/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITESPINYGREYBALL_INCLUDED
#define SPRITESPINYGREYBALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "sprite_lakitu.hpp"
#include "gotspeed.hpp"
#include "gotcollision.hpp"

class Sprite_SpinyGreyBall : public Pawn, public GotSpeed, public GotCollision
{
    public :

        Sprite_SpinyGreyBall(const sf::Texture& texture, const sf::Texture& walktexture, Collider* collision, Sprite_Lakitu* parent);

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
        const sf::Texture* m_texture;

        Sprite_Lakitu* m_parent;

        unsigned char m_state;

        bool m_onfloor;
};

#endif
