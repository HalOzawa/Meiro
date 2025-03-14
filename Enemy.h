#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"
#include <vector>
#include <queue>
#include <stack>

using std::vector;
using std::queue;
using std::stack;

class Enemy :
    public GameObject
{
    Point pos_;
	Point PlayerPos_;
    bool isAlive_;
    float speed_;
    Point nextPos_;
    DIR forward_;

    vector<vector<int>> dist;
    vector<vector<Point>> pre;

	std::queue<Point> path;
public:
    Enemy();
    ~Enemy();

    void Update() override;
    void Draw() override;
    bool CheckHit(const Rect& me, const Rect& other);
    void YCloserMove();
    void XCloserMove();
    void XYCloserMove();
    void XYCloserMoveRandom();
    void RightHandMove();
	void Dijkstra(Point sp, Point gp); // ダイクストラ法
	void BFS(Point sp, Point gp); // 幅優先探索

    //int GetX() { return pos_.x; }
    //int GetY() { return pos_.y; }
};

