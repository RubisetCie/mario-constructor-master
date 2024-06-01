/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef CHECKBUTTON_INCLUDED
#define CHECKBUTTON_INCLUDED

#include <SFML/Graphics.hpp>

extern "C"
{
    #include <fmod.h>
}

class CheckButton : public sf::Drawable
{
    public :

        CheckButton(void (*ptrFunction) (), const sf::Texture& checkTexture, std::string helptext = "");

        void setSound(FMOD_SOUND** clickSound);

        void setPosition(float x, float y);

        sf::Vector2f getPosition() const;

        void setChecked(bool checked);

        bool getChecked();

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::string m_helptext;

        sf::Sprite m_sprite;
        sf::Sprite m_mark;

        FMOD_SOUND** m_clicksound;

        bool isCheck;

        void (*m_callfunction) ();
};

#endif
