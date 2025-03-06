#include "Enemy.h"
#include "Player.h"
#include "./Stage.h"
#include "globals.h"
#include <map>
#include <queue>

namespace
{
    Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };
}

Enemy::Enemy()
    :pos_({ 0,0 }), isAlive_(true), nextPos_({ 0,0 })
{
    int rx = 0;
    int ry = 0;
    while (rx % 2 == 0 || ry % 2 == 0)
    {
		rx = GetRand(STAGE_WIDTH - 1);
		ry = GetRand(STAGE_HEIGHT - 1);
	}
    pos_ = { rx * CHA_WIDTH, ry * CHA_HEIGHT };
    forward_ = RIGHT;

    dist = vector(STAGE_HEIGHT, vector<int>(STAGE_WIDTH, INT_MAX));
    pre = vector(STAGE_HEIGHT, vector<Point>(STAGE_WIDTH, { -1,-1 }));
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    static bool stop = false;

    if (!stop)
    {
        Point op = pos_;
        Point move = { nDir[forward_].x, nDir[forward_].y };
        Rect eRect = { pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };
        Stage* stage = (Stage*)FindGameObject<Stage>();
        pos_ = { pos_.x + move.x, pos_.y + move.y };
        for (auto& obj : stage->GetStageRects())
        {
            if (CheckHit(eRect, obj))
            {
                Rect tmpRectX = { op.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };
                Rect tmpRectY = { pos_.x, op.y, CHA_WIDTH, CHA_HEIGHT };
                if (!CheckHit(tmpRectX, obj))
                {
                    pos_.x = op.x;
                }
                else if (!CheckHit(tmpRectY, obj))
                {
                    pos_.y = op.y;
                }
                else
                {
                    pos_ = op;
                }
                forward_ = (DIR)GetRand(3);
                RightHandMove();
                break;
            }
        }
    }
    int prgssx = pos_.x % (CHA_WIDTH);
    int prgssy = pos_.y % (CHA_HEIGHT);
    int cx = (pos_.x / (CHA_WIDTH)) % 2;
    int cy = (pos_.y / (CHA_HEIGHT)) % 2;
    if (prgssx == 0 && prgssy == 0 && cx && cy)
    {
		//forward_ = (DIR)GetRand(3);
        //YCloseMove();
        RightHandMove();
	}

    //ÉvÉåÉCÉÑÅ[Ç∆ÇÃìñÇΩÇËîªíË
    Player* player = (Player*)FindGameObject<Player>();
    Rect playerRect = { player->GetPos().x, player->GetPos().y, CHA_WIDTH, CHA_HEIGHT };
    Rect enemyRect = { pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };
    if (CheckHit(playerRect, enemyRect))
    {
		stop = true;
	}
    else
    {
		stop = false;
	}
}

void Enemy::Draw()
{
    DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(0, 0, 0), TRUE);
    Point tp[4][3] =
    {
        {{pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
        {{pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}, {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
        {{pos_.x            , pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}},
        {{pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}}
    };

    DrawTriangle(tp[forward_][0].x, tp[forward_][0].y, tp[forward_][1].x, tp[forward_][1].y,
                 tp[forward_][2].x, tp[forward_][2].y, GetColor(255, 255, 255), TRUE);
}

bool Enemy::CheckHit(const Rect& me, const Rect& other)
{
    if (me.x < other.x + other.w &&
        me.x + me.w > other.x &&
        me.y < other.y + other.h &&
        me.y + me.h > other.y)
    {
        return true;
    }

    return false;
}

void Enemy::YCloserMove()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (pos_.y > player->GetPos().y)
    {
        forward_ = UP;
    }
    else if (pos_.y < player->GetPos().y)
    {
        forward_ = DOWN;
    }
}

void Enemy::XCloserMove()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (pos_.x > player->GetPos().x)
    {
	    forward_ = LEFT;
	}
    else if (pos_.x < player->GetPos().x)
    {
		forward_ = RIGHT;
	}
}

void Enemy::XYCloserMove()
{
    Player* player = (Player*)FindGameObject<Player>();
    int xdis = abs(pos_.x - player->GetPos().x);
    int ydis = abs(pos_.y - player->GetPos().y);
    if (xdis > ydis)
    {
        if (pos_.x > player->GetPos().x)
        {
			forward_ = LEFT;
		}
        else if (pos_.x < player->GetPos().x)
        {
			forward_ = RIGHT;
		}
	}
    else
    {
        if (pos_.y > player->GetPos().y)
        {
			forward_ = UP;
		}
        else if (pos_.y < player->GetPos().y)
        {
			forward_ = DOWN;
		}
	}
}

void Enemy::XYCloserMoveRandom()
{
    Player* player = (Player*)FindGameObject<Player>();
    int xdis = abs(pos_.x - player->GetPos().x);
    int ydis = abs(pos_.y - player->GetPos().y);
    int rnum = GetRand(2);
    if (rnum == 0)
    {
        XYCloserMove();
    }
    else if (rnum == 1)
    {
        forward_ = (DIR)GetRand(3);
    }
}

void Enemy::RightHandMove()
{
    DIR myRight[4] = { RIGHT, LEFT, UP, DOWN };
    DIR myLeft[4] = { LEFT, RIGHT, DOWN, UP };
    Point nposF = { pos_.x + nDir[forward_].x, pos_.y + nDir[forward_].y };
    Point nposR = { pos_.x + nDir[myRight[forward_]].x, pos_.y + nDir[myRight[forward_]].y };
    Rect myRectF = { nposF.x, nposF.y, CHA_WIDTH, CHA_HEIGHT };
    Rect myRectR = { nposR.x, nposR.y, CHA_WIDTH, CHA_HEIGHT };
    Stage* stage = (Stage*)FindGameObject<Stage>();
    bool isRightOpen = true;
    bool isForwardOpen = true;
    for (auto& obj : stage->GetStageRects())
    {
        if (CheckHit(myRectF, obj))
        {
            isForwardOpen = false;
        }
        if (CheckHit(myRectR, obj))
        {
			isRightOpen = false;
		}
    }
    if (isRightOpen)
    {
        forward_ = myRight[forward_];
    }
    else if (isRightOpen == false && isForwardOpen == false)
    {
        forward_ = myLeft[forward_];
    }
}

void Enemy::Dijkstra(Point sp, Point gp)
{
    using Mdat = std::pair<int, Point>;

    dist[sp.y][sp.x] = 0;
    std::priority_queue<Mdat, std::vector<Mdat>, std::greater<Mdat>> pq;
    pq.push(Mdat(0, { sp.x, sp.y }));
    vector<vector<StageObj>> stageData = ((Stage*)FindGameObject<Stage>())->GetStageGrid();

    while (!pq.empty())
    {
        Mdat p = pq.top();
        pq.pop();

        //Rect{ (int)p.second.x * STAGE_WIDTH, (int)p.second.y * BLOCK_SIZE.y, BLOCK_SIZE }.draw(Palette::Red);
        //getchar();
        int c = p.first;
        Point v = p.second;

        for (int i = 0; i < 4; i++)
        {
            Point np = { v.x + (int)nDir[i].x, v.y + (int)nDir[i].y };
            if(np.x < 0 || np.y < 0 || np.x >= STAGE_WIDTH || np.y >= STAGE_HEIGHT) continue;
            if (stageData[np.y][np.x].obj == STAGE_OBJ::WALL) continue;
            if(dist[np.y][np.x] <= stageData[np.y][np.x].weight + c) continue;
            dist[np.y][np.x] = stageData[np.y][np.x].weight + c;
            pre[np.y][np.x] = Point({ v.x, v.y });
            pq.push(Mdat(dist[np.y][np.x], np));
        }
    }
}
