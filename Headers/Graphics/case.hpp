/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef CASE_INCLUDED
#define CASE_INCLUDED

#include <SFML/Graphics.hpp>

class Case : public sf::Drawable
{
    public :

        Case(sf::Texture& texture, sf::IntRect rekt, unsigned int eye_id);

        void setPosition(sf::Vector2f pos);

        void update(sf::RectangleShape* rekt, void (*ptrFunction)(unsigned int));

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        unsigned int m_eyeid;
};

#endif
