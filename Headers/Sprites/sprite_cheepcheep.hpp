/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITECHEEPCHEEP_INCLUDED
#define SPRITECHEEPCHEEP_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "../Core/collider.hpp"

class Sprite_CheepCheep : public Pawn
{
    public :

        Sprite_CheepCheep(const sf::Texture& texture, Collider* collision);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        sf::Vector2f m_movedistance;

    protected :

        sf::Sprite m_sprite;

        Collider* m_collider;

        char m_timer;
        bool m_killed, m_phase, m_swimming, m_active;
};

#endif
