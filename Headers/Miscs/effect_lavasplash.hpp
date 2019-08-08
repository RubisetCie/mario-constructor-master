/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECTLAVASPLASH_INCLUDED
#define EFFECTLAVASPLASH_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/effect.hpp"

class Effect_LavaSplash : public Effect
{
    public :

        Effect_LavaSplash(sf::Texture* texture, sf::Vector2f position, sf::Vector2f size, float angle);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        unsigned int m_frame;
};

#endif
