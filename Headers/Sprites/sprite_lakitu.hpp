/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITELAKITU_INCLUDED
#define SPRITELAKITU_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/pawn.hpp"
#include "../Core/collider.hpp"

class Sprite_Lakitu : public Pawn
{
    public :

        Sprite_Lakitu(const sf::Texture& texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        virtual void thruw() = 0;

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        sf::Vector2f m_movedistance;

        unsigned char m_enemies;

    protected :

        sf::VertexArray m_array;

        sf::Texture const* m_texture;

        Collider* m_collider;

        short m_alarm[3];
        float m_targetspeed;

        bool m_killed, m_active;
};

#endif
