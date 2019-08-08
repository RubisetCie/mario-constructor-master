/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EFFECTSPLASH_INCLUDED
#define EFFECTSPLASH_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/effect.hpp"

class Effect_Splash : public Effect
{
    public :

        Effect_Splash(sf::Texture* texture, sf::Vector2f position);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        unsigned int m_frame;
};

#endif
