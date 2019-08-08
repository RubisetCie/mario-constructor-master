/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef REGULATOR_ENTITY_INCLUDED
#define REGULATOR_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>

class Regulator : public sf::Drawable
{
    public :

        Regulator(sf::Texture* slider, std::string helptext, void (*ptrFunction) (float sliderPos));

        void setPosition(float position);

        void setParams(float x, float y, float minx, float maxx);

        void update();

        float getPosition() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::string m_helptext;

        sf::Sprite m_slider;

        float m_minx;
        float m_maxx;

        bool m_grabslider;

        void (*m_callfunction) (float sliderPos);
};

#endif
