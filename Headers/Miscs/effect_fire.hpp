/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECTFIRE_INCLUDED
#define EFFECTFIRE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/effect.hpp"

class Effect_Fire : public Effect
{
    public :

        Effect_Fire(sf::Texture* texture, sf::Vector2f position);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        unsigned int m_frame;
};

#endif
