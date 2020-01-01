/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITECHEEPCHEEPBLUE_INCLUDED
#define SPRITECHEEPCHEEPBLUE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "gotcollision.hpp"

class Sprite_CheepCheepBlue : public GotCollision, public Pawn
{
    public :

        Sprite_CheepCheepBlue(const sf::Texture& texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        sf::Vector2f m_movedistance;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        bool m_killed, m_swimming, m_active;
};

#endif
