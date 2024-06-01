/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef ELEMENTBUTTON_INCLUDED
#define ELEMENTBUTTON_INCLUDED

#include <SFML/Graphics.hpp>

extern "C"
{
    #include <fmod.h>
}

class ElementButton : public sf::Drawable
{
    public :

        ElementButton(const sf::Texture& texture, const sf::IntRect& rectangle, unsigned int elementnumb, void (*ptrFunction) (unsigned int elementNumb));

        void setSound(FMOD_SOUND** clickSound);

        void setPosition(float x, float y);

        void Unselected();

        void update();

        void freeze();

        unsigned int m_elementnumb;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        FMOD_SOUND** m_clicksound;

        bool m_selected;

        void (*m_callfunction) (unsigned int elementNumb);
};

#endif
