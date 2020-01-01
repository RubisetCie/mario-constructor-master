/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEFONT_INCLUDED
#define SPRITEFONT_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class SpriteFont : public sf::Drawable
{
    public :

        SpriteFont(const sf::Vector2f& position, bool right, const std::string& startText);

        void setText(const char* text, unsigned int size, const sf::Vector2f& position, bool right);
        void setPosition(const sf::Vector2f& position);

        float getHorizontalSize();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::vector<sf::Vertex> m_text;
};

#endif
