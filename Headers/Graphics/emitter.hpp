/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef EMITTER_INCLUDED
#define EMITTER_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

class Emitter : public sf::Drawable
{
    public :

        Emitter(unsigned int m_radiusx, unsigned int m_radiusy);
        ~Emitter();

        void setRadius(unsigned int m_radiusx, unsigned int m_radiusy);

        void setPosition(float x, float y);

        void emit(sf::Texture* texture);
        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        std::vector<sf::Vertex> m_particles;

        float m_x, m_y;

        unsigned int m_radiusx;
        unsigned int m_radiusy;
};

#endif
