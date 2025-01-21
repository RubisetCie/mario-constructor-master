/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Marks/mark_waterplane.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_WaterPlane::Mark_WaterPlane(Texture* texture, float height, Vector2u dimensions)
{
    m_water.setTexture(texture);
    m_water.setTextureRect(IntRect(0, 0, 640, 16));
    m_water.setFillColor(Color(255, 255, 255, 120));
    m_water.setPosition(0, height - 16);
    m_water.setSize(Vector2f(640, 16));

    m_waterdepth.setFillColor(Color(66, 115, 206, 120));
    m_waterdepth.setSize(Vector2f(dimensions.x * 640, (dimensions.y * 480) - height));
    m_waterdepth.setPosition(0, (dimensions.y * 480) - height);
}

float Mark_WaterPlane::getHeight()
{
    return m_water.getPosition().y + 16;
}

bool Mark_WaterPlane::getType() const
{
    return false;
}

void Mark_WaterPlane::setHeight(float height, Vector2u dimensions)
{
    m_water.setPosition(cameraPos.x - 320, height - 16);
    m_waterdepth.setPosition(cameraPos.x - 320, height);
    m_waterdepth.setSize(Vector2f(dimensions.x * 640, (dimensions.y * 480) - height));
}

void Mark_WaterPlane::update()
{
    m_water.setPosition(cameraPos.x - 320, m_water.getPosition().y);

    if (frame_Water % 6 == 0)
        m_water.setTextureRect(IntRect(0, (frame_Water / 6) * 16, 640, 16));
}

void Mark_WaterPlane::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_water);
    target.draw(m_waterdepth);
}
