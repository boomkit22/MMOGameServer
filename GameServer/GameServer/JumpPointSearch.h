#pragma once
#include "Type.h"
#include <list>
#include <vector>

enum CellValue {
	NONE = 0,
	OBSTACLE = 1,
	START = 2,
	END = 3,
	OPEN = 4,
	CLOSE = 5,
	ROUTE = 6,
	BRESENHAM = 10,
	CHECK = 20,
};

struct Pos {
	int y;
	int x;

	Pos& operator=(const Pos& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	Pos operator+(const Pos& other) {
		Pos ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	Pos operator-(const Pos& other)
	{
		Pos ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	bool operator<(const Pos& other) const {
		if (y < other.y) return true;
		if (y > other.y) return false;
		// y가 같을 경우 x를 비교
		return x < other.x;
	}

	bool operator==(const Pos& other) const {
		return y == other.y && x == other.x;
	}

	bool operator!=(const Pos& other) const {
		return y != other.y || x != other.x;
	}
};

struct Node {
	Pos _pos;
	int _f;
	int _g;
	int _h;
	Node* _parent;

	int directionArr[8] = { 0, };

	Node() {
		_pos = Pos{ 0,0 };
		_f = 0;
		_g = 0;
		_h = 0;
		_parent = nullptr;
	}

	Node(Pos pos, int f, int g, int h, Node* p)
	{
		_pos = pos;
		_f = f;
		_g = g;
		_h = h;
		_parent = p;
	}

	void AddDirection(int dir)
	{
		directionArr[dir] = 1;
	}
};

struct NodeComparator {
	bool operator()(const Node* a, const Node* b) const {
		// 우선순위 큐에서 더 작은 값을 가지는 Node를 우선시하도록 비교
		return a->_f < b->_f;
	}
};

class JumpPointSearch
{
	//TODO: 생성자로 맵 받고
public:
	JumpPointSearch(uint8** map, int32 mapYSize, int32 mapXSize);
	~JumpPointSearch();

public:
	//std::vector<Pos> FindPath(Pos start, Pos end);
	void FindPath(Pos start, Pos end, std::vector<Pos>& path);


private:
	Node* CreateStartNode(Pos pos);
	int GetMapValue(Pos pos) {
		return _jpsMap[pos.y][pos.x];
	}
	void SetMap(Pos& pos, uint8 value);
	int CheckUp(Pos pos);
	int CheckRight(Pos pos);
	int CheckDown(Pos pos);
	int CheckLeft(Pos pos);
	int CheckUpRight(Pos pos);
	int CheckRightDown(Pos pos);
	int CheckLeftDown(Pos pos);
	int CheckLeftUp(Pos pos);
	bool CheckAndMakeCorner(Node* p, Pos pos, int direction);

	Node* CreateNode(Pos pos, Node* parent, int direction, int distanceFromParent);
	void ReopenNode(Node* n);
	void CheckAndChangeParent(Node* newParent, Node* exist, int distance);
	int GetH(Pos& pos, Pos& dest);

	void FindShortestPath(std::vector<Pos>& path, std::vector<Pos>& reuslt);
	bool CalculateBresenham(Pos start, Pos end);

private:
	uint8** _originMap;
	uint8** _jpsMap;

	int32 _mapYSize;
	int32 _mapXSize;
	
	std::list<Node*> openList;
	Pos _start;
	Pos _dest;
};

