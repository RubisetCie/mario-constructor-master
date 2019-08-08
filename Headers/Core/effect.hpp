/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECT_INCLUDED
#define EFFECT_INCLUDED

#include <SFML/Graphics.hpp>

class Effect : public sf::Drawable
{
    public :

        Effect(sf::Vector2f position);

        virtual void update() = 0;

        bool m_destroyed;

    protected :

        void draw(sf::RenderTarget& target, sf::RenderStates) const = 0;

        sf::Sprite m_sprite;
};

#endif
