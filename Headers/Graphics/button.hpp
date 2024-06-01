/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BUTTON_INCLUDED
#define BUTTON_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

extern "C"
{
    #include <fmod.h>
}

class Button : public sf::Drawable
{
    public :

        Button(sf::Texture& texture, void (*ptrFunction) (), std::string helptext, const sf::IntRect& rectangle = sf::IntRect(0, 0, 0, 0));

        void setTexture(sf::Texture& texture);
        void setTextureRect(sf::IntRect rectangle);
        void setSound(FMOD_SOUND** clickSound);

        void freeze();

        void setPosition(float x, float y);
        void setRotation(float angle);
        void setScale(float x, float y);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::string m_helptext;

        sf::Sprite m_sprite;

        FMOD_SOUND** m_clicksound;

        void (*m_callfunction) ();
};

#endif
