/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPARATROOPAREDH_INCLUDED
#define SPRITEPARATROOPAREDH_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_paratroopa.hpp"

class Sprite_ParatroopaRedH : public Sprite_Paratroopa
{
    public :

        Sprite_ParatroopaRedH(const sf::Texture& sprite_texture, sf::Texture* const turtle_texture, sf::Texture* const shell_texture, Collider* collision, float target);

        void setPosition(const sf::Vector2f& pos);

        void flyUpdate();

        void toTurtle();

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        float m_target, m_startpos;
        bool m_forward;
};

#endif
