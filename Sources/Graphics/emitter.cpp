/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <vector>

#include "../../Headers/Graphics/emitter.hpp"

using namespace sf;
using namespace std;

Emitter::Emitter(unsigned int radiusx, unsigned int radiusy)
{
    /**The following code do : Initialize the private attributes.*/

    m_x = 0;
    m_y = 0;

    m_radiusx = radiusx;
    m_radiusy = radiusy;
}

Emitter::~Emitter()
{
    m_particles.clear();
}

void Emitter::setRadius(unsigned int radiusx, unsigned int radiusy)
{
    m_radiusx = radiusx;
    m_radiusy = radiusy;
}

void Emitter::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Emitter::emit(sf::Texture* texture)
{
    float posx = rand() % m_radiusx + m_x;
    float posy = rand() % m_radiusy + m_y;

    unsigned int texw = texture->getSize().x;
    unsigned int texh = texture->getSize().y;

    m_particles.emplace_back(Vertex(Vector2f(posx - (texw/2), posy - (texh/2)), Color::White, Vector2f(0, 0)));
    m_particles.emplace_back(Vertex(Vector2f(posx + (texw/2), posy - (texh/2)), Color::White, Vector2f(texw, 0)));
    m_particles.emplace_back(Vertex(Vector2f(posx + (texw/2), posy + (texh/2)), Color::White, Vector2f(texw, texh)));
    m_particles.emplace_back(Vertex(Vector2f(posx - (texw/2), posy + (texh/2)), Color::White, Vector2f(0, texh)));
}

void Emitter::update()
{
    for (register unsigned int i = 0; i < m_particles.size() / 4; i++)
    {
        Vertex* particle = const_cast<Vertex*>(&m_particles[i * 4]);

        Uint8 alpha = particle->color.a;

        if (alpha > 6)
        {
            alpha -= 6;

            particle[0].color.a = alpha;
            particle[1].color.a = alpha;
            particle[2].color.a = alpha;
            particle[3].color.a = alpha;
        }
        else
            m_particles.erase(m_particles.begin() + (i * 4), m_particles.begin() + 4 + (i * 4));
    }
}

void Emitter::draw(RenderTarget& target, RenderStates states) const
{
    states.blendMode = BlendAdd;

    target.draw(&m_particles.front(), m_particles.size(), Quads, states);
}
