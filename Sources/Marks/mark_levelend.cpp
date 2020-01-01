/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/globals.hpp"
#include "../../Headers/Marks/mark_levelend.hpp"
#include "../../Headers/Miscs/effect_score.hpp"

#define LEVELEND_BARSPEED 2
#define LEVELEND_BARROTATE -18
#define LEVELEND_BARMAXHEIGHT 210

#define LEVELEND_ENDSPEED 2

using namespace sf;
using namespace std;

Mark_LevelEnd::Mark_LevelEnd(const Texture& texture, const Texture& limitTexture, const Texture& barTexture, float limit) : Placeable()
{
    m_sprite.setTexture(texture);
    m_limit.setTexture(limitTexture);
    m_end.setTexture(barTexture);
    m_end.setOrigin(22, 8);

    m_endspeed = Vector2f(0, 0);

    m_limitdist = limit;
    m_endpos = 0;
    m_enddir = 1;

    m_cleared = 0;
}

void Mark_LevelEnd::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
    m_limit.setPosition(m_limitdist + 1, pos.y + 256);
    m_end.setPosition(pos.x + 40, pos.y + 248);
}

void Mark_LevelEnd::update()
{
    if (m_cleared > 0)
        return;

    if (m_cleared == 0)
    {
        Vector2f pos(m_sprite.getPosition());

        if (player->m_aabb.left >= m_limitdist && player->m_aabb.top + player->m_aabb.height == pos.y + 288)
        {
            m_cleared = 1;

            listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(pos.x+26, pos.y-4)));
            addPoints(100);
        }

        if (player->m_aabb.intersects(m_end.getGlobalBounds()))
        {
            Vector2f endpos(m_end.getPosition());

            m_endspeed = Vector2f(-3, -5);
            m_cleared = 2;

            if (m_endpos <= 30)
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 0, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(100);
            }
            else if (m_endpos <= 60)
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 1, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(200);
            }
            else if (m_endpos <= 90)
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 2, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(500);
            }
            else if (m_endpos <= 120)
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 3, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(1000);
            }
            else if (m_endpos <= 150)
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 4, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(2000);
            }
            else if (m_endpos <= 180)
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 5, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(5000);
            }
            else
            {
                listEffect.emplace_back(new Effect_Score(effectTxt[13], 6, Vector2f(endpos.x-8, endpos.y-14)));
                addPoints(10000);
            }
        }
    }

    if (m_cleared > 0)
        player->clearCourse();
}

void Mark_LevelEnd::secureUpdate()
{
    Vector2f endPos(m_sprite.getPosition());

    if (m_cleared > 0)
    {
        player->moveHorizontal(LEVELEND_ENDSPEED);

        if (m_cleared == 2)
        {
            m_endspeed.y += 0.15;

            m_end.rotate(LEVELEND_BARROTATE);
            m_end.move(m_endspeed);
        }

        return;
    }

    if (m_enddir)
    {
        m_endpos += LEVELEND_BARSPEED;

        if (m_endpos > LEVELEND_BARMAXHEIGHT)
        {
            m_endpos = LEVELEND_BARMAXHEIGHT;
            m_enddir = false;
        }
    }
    else
    {
        m_endpos -= LEVELEND_BARSPEED;

        if (m_endpos < 0)
        {
            m_endpos = 0;
            m_enddir = true;
        }
    }

    m_end.setPosition(endPos.x + 40, endPos.y - m_endpos + 248);
}

void Mark_LevelEnd::afterUpdate()
{
}

Vector2f Mark_LevelEnd::getPosition() const
{
    return Vector2f(m_limitdist, m_sprite.getPosition().y);
}

Vector2f Mark_LevelEnd::getSize() const
{
    return Vector2f(m_sprite.getPosition().x - m_limitdist + 107, 288);
}

ID Mark_LevelEnd::getID() const
{
    return ID_USELESS;
}

void Mark_LevelEnd::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
    target.draw(m_limit);
    target.draw(m_end);
}
