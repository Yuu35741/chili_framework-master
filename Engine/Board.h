#pragma once
#include "Graphics.h"
#include "Colors.h"
#include "MainWindow.h"
#include <random>
#include <vector>
class Grid {
public:
	Grid(MainWindow& wnd);

	int GetWidth() const;
	int GetHeight() const;
	int GetgridX() const;
	int GetgridY() const;
	int rng(int x, int y);
	void MoveUp(MainWindow& wnd, Grid& grd, bool& nore);
	void MoveDown(MainWindow& wnd, Grid& grd, bool& nore);
	void MoveLeft(MainWindow& wnd, Grid& grd, bool& nore);
	void MoveRight(MainWindow& wnd, Grid& grd, bool& nore);
public:
	struct Location
	{
		unsigned char x;
		unsigned char y;
		unsigned char code = 0;
	};
	int xpos = 30;
	int ypos = 30;
	Location curLoc;
	Location loc[7][7];
	Location selLoc, preLoc;
	D3DCOLOR* surface = (D3DCOLOR*)malloc (sizeof(D3DCOLOR)*92*72*5);
	float speed[7][7] = { 0 };
	int drop[7][7] = { 0 };
public:
	void CreateGrid(Grid& grd, Graphics& gfx, int move[7][7], int& step, bool& triggerscan, int& dist, int score[2], bool& scoretrigger);
	void HightLight(Location& loc, Graphics& gfx);
	void HightLightB(Location& loc, Graphics& gfx);
	void HightLighttrans(Location& loc, Graphics& gfx,Graphics::Bmp& background);
	void Swap(Grid& grd, Location & start, Location & desti, Graphics & gfx, int& dist, int movetemp[5]);
	void Move(Location& loc,Graphics& gfx, char* direct, int move);
	void fillCell(Location& loc, Graphics& gfx, char* direct, int move[7][7]);
	void Scan(Location& loc, Grid& grd, bool trigger, int& dist, int score[2], bool& scoretrigger);
	void Drop(Location& from, Location& to, Grid& grd, Graphics& gfx, int& dist, int& drop);
private:
	static constexpr int width  = 7;
	static constexpr int height = 7;
	static constexpr int gridX  = 92;
	static constexpr int gridY  = 72;
	Graphics& gfx;
	MainWindow& wnd;

};
