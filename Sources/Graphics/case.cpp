/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/case.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Case::Case(Texture& texture, IntRect rekt, unsigned int eye_id)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(rekt);

    m_eyeid = eye_id;
}

void Case::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Case::update(RectangleShape* rekt, void (*ptrFunction)(unsigned int))
{
    FloatRect aabb(m_sprite.getGlobalBounds());

    if (aabb.contains(mpos_relative))
    {
        rekt->setPosition(m_sprite.getPosition());
        rekt->setSize(Vector2f(aabb.width, aabb.height));

        if (mousePressed == 1)
            ptrFunction(m_eyeid);
    }
}

void Case::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
