/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Marks/mark_lavaplane.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;


Mark_LavaPlane::Mark_LavaPlane(Texture* texture, float height, Vector2u dimensions)
{
    m_lava.setTexture(texture);
    m_lava.setTextureRect(IntRect(0, 0, 640, 34));
    m_lava.setPosition(0, height - 34);
    m_lava.setSize(Vector2f(640, 34));

    m_lavadepth.setFillColor(Color(123, 0, 0, 255));
    m_lavadepth.setSize(Vector2f(dimensions.x * 640, (dimensions.y * 480) - height));
    m_lavadepth.setPosition(0, (dimensions.y * 480) - height);
}

float Mark_LavaPlane::getHeight()
{
    return m_lava.getPosition().y + 34;
}

bool Mark_LavaPlane::getType() const
{
    return true;
}

void Mark_LavaPlane::setHeight(float height, Vector2u dimensions)
{
    m_lava.setPosition(cameraPos.x - 320, height - 34);
    m_lavadepth.setPosition(cameraPos.x - 320, height);
    m_lavadepth.setSize(Vector2f(dimensions.x * 640, (dimensions.y * 480) - height));
}

void Mark_LavaPlane::update()
{
    m_lava.setPosition(cameraPos.x - 320, m_lava.getPosition().y);

    if (frame_Lava % 8 == 0)
        m_lava.setTextureRect(IntRect(0, (frame_Lava/8) * 34, 640, 34));
}

void Mark_LavaPlane::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_lava);
    target.draw(m_lavadepth);
}
