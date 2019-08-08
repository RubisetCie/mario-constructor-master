/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_checkpoint.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Mark_CheckPoint::Mark_CheckPoint(const Texture& texture, bool zone) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 107, 111));

    m_collision.width = 8;
    m_collision.height = 111;

    m_checked = false;
    m_zone = zone;
}

void Mark_CheckPoint::setPosition(Vector2f pos)
{
    m_sprite.setPosition(pos);

    m_collision.left = pos.x + 41;
    m_collision.top = pos.y;
}

void Mark_CheckPoint::silentCheck()
{
    m_checked = true;
    m_sprite.setTextureRect(IntRect(107, 0, 107, 111));
}

void Mark_CheckPoint::unCheck()
{
    m_checked = false;
    m_sprite.setTextureRect(IntRect(0, 0, 107, 111));
}

void Mark_CheckPoint::update()
{
    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (!m_checked)
    {
        FloatRect playerbbox = player->m_aabb;

        if (playerbbox.intersects(m_collision))
        {
            if (player->m_checkpoint != NULL)
            {
                if (playerbbox.intersects(player->m_checkpoint->m_collision))
                    return;
            }

            bool done = false;

            for (list<Placeable*>::iterator it = listPlaceables.begin(); it != listPlaceables.end(); it++)
            {
                if ((*it)->getID() == ID_CHECKPOINT)
                {
                    if (dynamic_cast<Mark_CheckPoint*>(*it)->m_checked)
                    {
                        dynamic_cast<Mark_CheckPoint*>(*it)->unCheck();
                        done = true;
                        break;
                    }
                }
            }

            if (!done)
            {
                for (list<Placeable*>::iterator it = listPlaceablesb.begin(); it != listPlaceablesb.end(); it++)
                {
                    if ((*it)->getID() == ID_CHECKPOINT)
                    {
                        if (dynamic_cast<Mark_CheckPoint*>(*it)->m_checked)
                        {
                            dynamic_cast<Mark_CheckPoint*>(*it)->unCheck();
                            done = true;
                            break;
                        }
                    }
                }
            }

            m_checked = true;
            m_sprite.setTextureRect(IntRect(107, 0, 107, 111));

            player->m_checkpoint = this;

            if (autoscroll == zoneb + 1)
                autoscroll_nodesaved = autoscroll_node;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(5), sfxSamples[4], 0, NULL);
        }
    }
}

void Mark_CheckPoint::secureUpdate()
{
}

void Mark_CheckPoint::afterUpdate()
{
}

Vector2f Mark_CheckPoint::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Mark_CheckPoint::getSize() const
{
    return Vector2f(107, 111);
}

ID Mark_CheckPoint::getID() const
{
    return ID_CHECKPOINT;
}

void Mark_CheckPoint::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
