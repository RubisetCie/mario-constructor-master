/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef INFOBUTTON_INCLUDED
#define INFOBUTTON_INCLUDED

#include <SFML/Graphics.hpp>

extern "C"
{
    #include <fmod.h>
}

class InfoButton : public sf::Drawable
{
    public :

        InfoButton(const sf::Texture& texture, unsigned char menuNumb);

        void setSound(FMOD_SOUND** clickSound);

        void setPosition(float x, float y);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        FMOD_SOUND** m_clicksound;

        unsigned char m_menunumb;
};

#endif
