#include "Board.h"

int Grid::GetWidth() const
{
	return width;
}

int Grid::GetHeight() const
{
	return height;
}

int Grid::GetgridX() const
{
	return gridX;
}

int Grid::GetgridY() const
{
	return gridY;
}

int Grid::rng(int x, int y)
{
		static std::random_device seed;
		static std::mt19937 random(seed());
		std::uniform_int_distribution<int> rng(x, y);
		return rng(random);
}

void Grid::MoveUp(MainWindow & wnd, Grid & grd, bool& nore)
{
		if (wnd.kbd.KeyIsPressed(VK_UP) && nore)
		{
			nore = false;
			if (grd.curLoc.y != 0)
				grd.curLoc.y = grd.curLoc.y -1;
			else grd.curLoc.y = 6;
		}
		if (!(wnd.kbd.KeyIsPressed(VK_UP))) nore = true;
}

void Grid::MoveDown(MainWindow & wnd, Grid & grd, bool& nore)
{
	if (wnd.kbd.KeyIsPressed(VK_DOWN) && nore)
	{
		nore = false;
		if (grd.curLoc.y != 6)
			grd.curLoc.y = grd.curLoc.y +1;
		else grd.curLoc.y = 0;
	}
	if (!(wnd.kbd.KeyIsPressed(VK_DOWN))) nore = true;
}

void Grid::MoveLeft(MainWindow & wnd, Grid & grd, bool& nore)
{
	if (wnd.kbd.KeyIsPressed(VK_LEFT) && nore)
	{
		nore = false;
		if (grd.curLoc.x != 0)
			grd.curLoc.x = grd.curLoc.x - 1;
		else grd.curLoc.x = 6;
	}
	if (!(wnd.kbd.KeyIsPressed(VK_LEFT))) nore = true;
}

void Grid::MoveRight(MainWindow & wnd, Grid & grd, bool& nore)
{
	if (wnd.kbd.KeyIsPressed(VK_RIGHT) && nore)
	{
		nore = false;
		if (grd.curLoc.x!= 6)
			grd.curLoc.x = grd.curLoc.x + 1;
		else grd.curLoc.x= 0;
	}
	if (!(wnd.kbd.KeyIsPressed(VK_RIGHT))) nore = true;
}


Grid::Grid(MainWindow& wnd)
	:
	gfx(gfx),
	wnd(wnd)
{
}


void Grid::CreateGrid(Grid & grd, Graphics & gfx, int move[7][7], int& step, bool& triggerscan, int& dist, int score[2], bool& scoretrigger)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (j >= step && loc[i][j].code!=0)
			{
				fillCell(loc[i][j], gfx, "Down", move);
			}
			Scan(loc[i][j], grd, triggerscan,dist, score, scoretrigger);
			//test(loc[i][j], grd, gfx, triggerscan);
		}
	}
	//Drop(grd.loc[0][0], grd.loc[0][3], grd, gfx, dist, move[0][0]);
}

void Grid::HightLight(Location& loc, Graphics& gfx)
{
	if (loc.x >= 0 && loc.y >= 0 && loc.x < 7 && loc.y < 7) {
		for (int y = 0; y < 20; y++) {
				gfx.PutPixel(xpos + loc.x*gridX + 5, ypos + loc.y*gridY + y + 5, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX + 5 + y, ypos + loc.y*gridY + 5, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX - 5 + gridX, ypos + loc.y*gridY + 5  +y, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX - 5 - y + gridX, ypos + loc.y*gridY + 5, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX + 5, ypos + loc.y*gridY -y - 5 + gridY, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX + 5 + y, ypos + loc.y*gridY - 5 + gridY, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX - 5 + gridX, ypos + loc.y*gridY - y - 5 + gridY, Color(255, 255, 255));
				gfx.PutPixel(xpos + loc.x*gridX - 5 - y + gridX, ypos + loc.y*gridY - 5 + gridY, Color(255, 255, 255));

		}
	}
}
void Grid::HightLightB(Location& loc, Graphics& gfx)
{
	if (loc.x >= 0 && loc.y >= 0 && loc.x < 7 && loc.y < 7) {
		for (int y = 0; y < gridY; y++) {
			for (int x = 0; x < gridX; x++) {
				gfx.PutPixel(xpos + loc.x*gridX + x, ypos + loc.y*gridY + y, Color(0, 0, 0));
			}
		}
	}
}

void Grid::HightLighttrans(Location & loc, Graphics & gfx, Graphics::Bmp & background)
{
	if (loc.x >= 0 && loc.y >= 0 && loc.x < 7 && loc.y < 7) {
		for (int y = 0; y < gridY; y++) {
			for (int x = 0; x < gridX; x++) {
				gfx.PutPixel(xpos + loc.x*gridX + x, ypos + loc.y*gridY + y, background.surface[x + y*background.width]);
			}
		}
	}
}


void Grid::Swap(Grid& grd, Location & start, Location & desti, Graphics & gfx, int& dist, int movetemp[5])
{
	if (!(start.x == desti.x && start.y == desti.y)) {
		if (start.x == desti.x) {
			if (abs(start.y - desti.y) == 1) {
				if (start.y > desti.y) {
					for (int i = desti.y; i < start.y; i++) {
						Move(grd.loc[start.x][i], gfx, "Y", movetemp[1]);
					}
				}
				if (start.y < desti.y) {
					for (int i = desti.y; i > start.y; i--) {
						Move(grd.loc[start.x][i], gfx, "Y", movetemp[2]);
					}
				}
				dist = (start.y - desti.y)*gridY;
				Move(start, gfx, "Y", movetemp[0]);
			}
		}
		if (start.y == desti.y) {
			if (abs(start.x - desti.x) == 1) {
				if (start.x > desti.x) {
					for (int i = desti.x; i < start.x; i++) {
						Move(grd.loc[i][start.y], gfx, "X", movetemp[3]);
					}
				}
				if (start.x < desti.x) {
					for (int i = desti.x; i > start.x; i--) {
						Move(grd.loc[i][start.y], gfx, "X", movetemp[4]);
					}
				}
				dist = (start.x - desti.x)*gridX;
				Move(start, gfx, "X", movetemp[0]);
			}
		}
	}
}

void Grid::Move(Location& loc, Graphics& gfx, char* direct, int move)
{
	for (int y = 0; y < gridY; y++) {
		for (int x = 0; x < gridX; x++) {
			if (direct == "Y") {
				if ((ypos + loc.y*gridY + y - move) >=0 && ((ypos + loc.y*gridY + y - move)<=719))
					if (surface[loc.code*gridX*gridY + x + y*gridX]!=D3DCOLOR_XRGB(0,0,0))
					gfx.PutPixel(xpos + loc.x*gridX + x, ypos + loc.y*gridY + y - move, surface[loc.code*gridX*gridY+x+y*gridX]);
			}
			if (direct == "X") {
				if ((xpos + loc.x*gridX + x - move) >= 0)
					if (surface[loc.code*gridX*gridY + x + y*gridX] != D3DCOLOR_XRGB(0, 0, 0))
					gfx.PutPixel(xpos + loc.x*gridX + x - move, ypos + loc.y*gridY + y , surface[loc.code*gridX*gridY + x + y*gridX]);
			}
		}
	}
}
void Grid::fillCell(Location& loc, Graphics& gfx, char* direct, int move[7][7])
{
	for (int y = 0; y < gridY; y++) {
		for (int x = 0; x < gridX; x++) {
			if (direct == "Down") {
				if ((ypos + loc.y*gridY - (loc.y + 1)*gridY + y + move[loc.x][loc.y]) >= 0)
					if (surface[loc.code*gridX*gridY + x + y*gridX] != D3DCOLOR_XRGB(0, 0, 0))
					gfx.PutPixel(xpos + loc.x*gridX + x, ypos + loc.y*gridY - (loc.y + 1)*gridY + y + move[loc.x][loc.y], surface[loc.code*gridX*gridY + x + y*gridX]);
			}
		}
	}
}

void Grid::Scan(Location& loc, Grid& grd, bool trigger, int& dist, int score[2], bool& scoretrigger)
{
	unsigned char x = loc.x;
	unsigned char y = loc.y;

	if (trigger) {
		while ((grd.loc[x + 1][loc.y].code == grd.loc[x][loc.y].code) && x < 6) {
			x++;
		}
		while ((grd.loc[loc.x][y + 1].code == grd.loc[loc.x][y].code) && y < 6) {
			y++;
		}
		if ((x - loc.x) >= 2) {
			score[0] = score[0] + (x - loc.x + 1) * 10;
			score[1] = score[1] + (x - loc.x + 1) * 10;
			for (int i = loc.x; i <= x; i++) {
				for (int j = loc.y; j >= 0; j--) {
					if (j!=0) grd.loc[i][j].code = grd.loc[i][j - 1].code;
					else grd.loc[i][j].code = grd.rng(1, 4);
				}
			}
		}
		if ((y - loc.y) >= 2) {
			score[0] = score[0] + (y - loc.y + 1) * 10;
			score[1] = score[1] + (y - loc.y + 1) * 10;
			for (int i = y; i >=0; i--) {
				if (i-y+loc.y-1>=0) grd.loc[loc.x][i].code = grd.loc[loc.x][i-y+loc.y-1].code;
				else grd.loc[loc.x][i].code = grd.rng(1, 4);
			}
		}
	}
}

void Grid::Drop(Location & from, Location & to, Grid & grd, Graphics & gfx, int& dist, int& move)
{
	if (to.y == from.y && to.y==0) dist = gridY;
	else dist = (from.y-to.y)*gridY;
	Move(from, gfx, "Y", move);
}

