/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Bitmap.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	grd(wnd),
	gameoverSound(L"test.mp3"),
	undertaleSound(L"Undertale.mp3",0.00f,381.00f),
	homeSound(L"Home.mp3",0.00f,123.00f)
{
	std::ifstream inputFile("users\\userInfo.data");
	if (inputFile.is_open()) {
		getline(inputFile, username);
		inputFile.close();
	}
	grd.curLoc = { 3,3 };
	grd.selLoc = { 7,7 };
	allCellrng(grd.speed, 1, 4);
	Initial(grd);
	grd.preLoc = { 8,8 };
	Load("images\\achievement.bmp", 384, 36, achievementsur, &achievementbmp, transB);
	Load("images\\score.bmp", 140, 44, scoresur, &scorebmp, transB);
	Load("images\\pikachu.bmp", 92, 72, &grd.surface[92 * 72], &tempbmp, transB);
	Load("images\\bullbasaur.bmp", 92, 72, &grd.surface[92 * 72 * 2], &tempbmp, transB);
	Load("images\\charmander.bmp", 92, 72, &grd.surface[92 * 72 * 3], &tempbmp, transB);
	Load("images\\squirtle.bmp", 92, 72, &grd.surface[92 * 72 * 4], &tempbmp, transB);
	Load("images\\grid.bmp", 644, 504, backgroundgrid, &backgroundbmp, transW);
	Load("images\\gameover.bmp", 400, 267, surface3, &gameoverscreen, transB);
	Load("images\\startscreen.bmp", 960, 720, surface4, &startscreen, transB);
	Load("images\\legend\\ice.bmp", 96, 96, &legendsur[0], &legend[0], transB);
	Load("images\\legend\\thunder.bmp", 96, 96, &legendsur[96 * 96], &legend[1], transB);
	Load("images\\legend\\fire.bmp", 96, 96, &legendsur[96 * 96 * 2], &legend[2], transB);
	Load("images\\legend\\lugia.bmp", 96, 96, &legendsur[96 * 96 * 3], &legend[3], transB);
	Load("images\\leveltext.bmp", 132, 28, levelsur, &levelbmp, transB);
	Load("images\\hp.bmp", 52, 24, hpsur, &hpbmp, transB);
	Load("images\\total.bmp", 136, 28, totalsur, &totalbmp, transB);
	Load("images\\pokeball.bmp", 28, 28, pokeballsur, &pokeballbmp, transB);
	Load("images\\gamerule.bmp", 960, 720, gamerulesur, &gamerulebmp, transB);
	LoadAnimation(&magikarptemp, magikarpbmp, magikarpsur, "images\\magikarp\\magikarp", 100, 132, 8, 5, transB);
	Animating(&magikarptemp, &magikarp, 750, 100);
	LoadAnimation(&runningpikachutemp, runningpikachubmp, surface1, "images\\runningpikachu\\runningpikachu", 152, 108, 4, 5, transB);
	Animating(&runningpikachutemp, &runningpikachu, 400, 300);
	for (int i = 0; i < 10; i++) {
		char temp[64];
		sprintf(temp, "images\\number\\%d.bmp", i);
		Load(temp, 24, 32, &numbersur[24 * 32 * i], &numberbmp[i], transB);
	}
}

void backGround(Grid& grd, Graphics& gfx) {
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{

	if (startScreen)
	{
		UpdateAnimation(&runningpikachu);
		int my = wnd.mouse.GetPosY();
		int mx = wnd.mouse.GetPosX();
		if (my >= 420 && my <= 460 && mx >= 345 && mx <= 615) Option = 0;
		if (my > 460 && my <= 520 && mx >= 345 && mx <= 615) Option = 1;
		if (my > 520 && my <= 570 && mx >= 345 && mx <= 615) Option = 2;
		if (wnd.mouse.LeftIsPressed() && Option == 0) {
			startScreen = false; GameruleScreen = false;
		}
		if (wnd.mouse.LeftIsPressed() && Option == 1) {
			GameruleScreen = true;
		}

	}

	if (GameruleScreen) {
		if (wnd.kbd.KeyIsPressed(VK_RETURN)) GameruleScreen = false;
	}

	if (startScreen == false && gameoverScreen == false) {
		digitdisect(score[0], scoredigit, nDigit[0]);
		digitdisect(score[1], leveldigit, nDigit[1]);
		if (triggerscan == true) frameattack++;
		if (frameattack == frameattackdelay) {
			if (score[0] > attackpower && score[1] > attackpower) {
				score[1] -= attackpower;
				score[0] -= attackpower;
				frameattack = 0;
			}
			else if (score[1] <= attackpower) {
				if (score[0] <= attackpower) score[0] = 0;
				if (score[0] > attackpower) score[0] -= attackpower;
				score[1] = 0;
				frameattack = 0;
			}
		}
		if (level == 1) {
			bossHP = 2000;
			frameattackdelay = 300;
			attackpower = 100;
		}
		if (level == 2) {
			bossHP = 2000;
			frameattackdelay = 180;
			attackpower = 100;
		}
		if (level == 3) {
			bossHP = 3000;
			frameattackdelay = 600;
			attackpower = 400;
		}
		if (level == 4) {
			bossHP = 1000;
			frameattackdelay = 60;
			attackpower = 50;
		}
		if (level == 5) {
			bossHP = 10000;
			frameattackdelay = 3000;
			attackpower = 5000;
		}
		if (score[1] >= 0 && score[1] < bossHP) {
			for (int i = 0; i <= 4; i++) {
				if (i == (level - 1)) achievement[i] = true;
				else achievement[i] = false;
			}
		}
		else if (score[1] >= bossHP) {
			level++;
			frameattack = 0;
			triggertext = true;
			score[1] = 0;
		}
	}

	if (triggertext) {
		if (moveachieve >= 4 && two) {
			moveachieve -= accel[0];
			accel[0] += 0.1f;
		}
		if (moveachieve <= 4 || two == false) {
			if (frameratedelay < 30) frameratedelay++;
			if (frameratedelay == 30) {
				moveachieve += accel[1];
				accel[1] += 0.3f;
				two = false;
			}
		}
		if (moveachieve > 484 && two == false)
		{
			triggertext = false;
			moveachieve = 480.0f;
			two = true;
			accel[0] = 0.0f;
			accel[1] = 0.0f;
			frameratedelay = 0;
		}
	}

	if (startScreen == false && gameoverScreen == false) {
		for (int y = grd.GetHeight() - 1; y >= step; y--) {
			for (int x = 0; x < grd.GetWidth(); x++) {
				if (move[x][y] <= ((grd.loc[x][y].y + 1)*grd.GetgridY() - 2)) {
					move[x][y] += grd.speed[x][y];
					grd.speed[x][y] += 0.5f;
				}
				else {
					move[x][y] = (grd.loc[x][y].y + 1)*grd.GetgridY();
				}
			}
		}

		if (move[1][step] >= (grd.loc[1][step].y - 2)*grd.GetgridY() && step >0) step--;

		if (move[0][2] == 3 * grd.GetgridY()) triggerscan = true;
	}






	if (startScreen == false && gameoverScreen == false)
	{

		GetMouseLoc(grd, wnd, grd.curLoc);

		if ((wnd.mouse.LeftIsPressed()) && test)
		{
			test = false;
			next = !next;
			if (firstclick == false) {
				grd.preLoc = grd.selLoc;
			}
			grd.selLoc = grd.curLoc;
			if (firstclick == false) {
				if (grd.selLoc.x >= 0 && grd.selLoc.x < 7 && grd.selLoc.y >= 0 && grd.selLoc.y < 7)
					if (grd.preLoc.x >= 0 && grd.preLoc.x < 7 && grd.preLoc.y >= 0 && grd.preLoc.y < 7 && next) {
						if ((grd.selLoc.x == grd.preLoc.x && abs(grd.selLoc.y - grd.preLoc.y) == 1) || (grd.selLoc.y == grd.preLoc.y && abs(grd.selLoc.x - grd.preLoc.x) == 1))
							swaptrigger = true;
					}
			}
		}
		if (!(wnd.mouse.LeftIsPressed())) {
			test = true;
			firstclick = false;
		}



		if (swaptrigger) {
			if (distance > 0 && movetemp[0] < distance) movetemp[0] += 10;
			if (distance < 0 && movetemp[0] > distance) movetemp[0] -= 10;
			if (movetemp[0] == (distance - distance % 10)) {
				if (grd.preLoc.y > grd.selLoc.y) {
					for (int i = grd.preLoc.y; i > grd.selLoc.y; i--) {
						grd.loc[grd.preLoc.x][i].code = grd.loc[grd.preLoc.x][i - 1].code;
					}
				}
				if (grd.preLoc.y < grd.selLoc.y) {
					for (int i = grd.preLoc.y; i < grd.selLoc.y; i++) {
						grd.loc[grd.preLoc.x][i].code = grd.loc[grd.preLoc.x][i + 1].code;
					}
				}
				if (grd.preLoc.x > grd.selLoc.x) {
					for (int i = grd.preLoc.x; i > grd.selLoc.x; i--) {
						grd.loc[i][grd.preLoc.y].code = grd.loc[i - 1][grd.preLoc.y].code;
					}
				}
				if (grd.preLoc.x < grd.selLoc.x) {
					for (int i = grd.preLoc.x; i < grd.selLoc.x; i++) {
						grd.loc[i][grd.preLoc.y].code = grd.loc[i + 1][grd.preLoc.y].code;
					}
				}
				grd.loc[grd.selLoc.x][grd.selLoc.y].code = grd.preLoc.code;
				swaptrigger = false;
				for (int i = 0; i < 5; i++) movetemp[i] = 0;
				grd.preLoc = { 8,8 };
				grd.selLoc = { 7,7 };

			}
			if (grd.selLoc.y != grd.preLoc.y) {
				if (movetemp[1] < 72) movetemp[1] -= 10 / abs(grd.selLoc.y - grd.preLoc.y);
				if (movetemp[2] < 72) movetemp[2] += 10 / abs(grd.selLoc.y - grd.preLoc.y);
			}
			if (grd.selLoc.x != grd.preLoc.x) {
				if (movetemp[3] < 92) movetemp[3] -= 10 / abs(grd.selLoc.x - grd.preLoc.x);
				if (movetemp[4] < 92) movetemp[4] += 10 / abs(grd.selLoc.x - grd.preLoc.x);
			}
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_ESCAPE) && norepeat[0])
	{
		gameoverScreen = true;
		norepeat[0] = false;
		gameoverSound.Play();
		startScreen = false;
		logScore(username, score[0], level);
	}
	if (!(wnd.kbd.KeyIsPressed(VK_ESCAPE))) norepeat[0] = true;

}

void Game::ComposeFrame()
{
	if (startScreen == false && gameoverScreen == false)
	{
		homeSound.StopAll();
		if (undertale) {
			undertaleSound.Play();
			undertale = false;
		}
		gfx.PrintBmp(50, 560, &scorebmp);
		gfx.PrintBmp(50, 630, &totalbmp);
		for (int i = 0; i < nDigit[0]; i++) {
			gfx.PrintBmp(200 + nDigit[0] * 24 - 24 - 24 * i, 625, &numberbmp[scoredigit[i]]);
		}
		for (int i = 0; i < nDigit[1]; i++) {
			gfx.PrintBmp(200 + nDigit[1] * 24 - 24 - 24 * i, 560, &numberbmp[leveldigit[i]]);
		}
		gfx.PrintBmp(650, 650, &levelbmp);
		createLevel(gfx, level, bossHP, score[1]);
		if (triggertext) {
			gfx.PrintBmp(100 - moveachieve, 680, &achievementbmp);
		}
		if (achievement[0]) gfx.PrintBmp(750, 170, &legend[0]);
		if (achievement[1]) gfx.PrintBmp(750, 170, &legend[1]);
		if (achievement[2]) gfx.PrintBmp(750, 170, &legend[2]);
		if (achievement[3]) gfx.PrintBmp(750, 170, &legend[3]);
		if (achievement[4]) {
			gfx.DrawAnimation(&magikarp);
			UpdateAnimation(&magikarp);
		}
		gfx.PrintBmp(30, 30, &backgroundbmp);
		grd.CreateGrid(grd, gfx, move, step, triggerscan, dropdist, score, scoretrigger);
		if (swaptrigger) {
			if (grd.selLoc.x == grd.preLoc.x) {
				if (grd.selLoc.y - grd.preLoc.y > 0)
				{
					for (int i = grd.preLoc.y; i <= grd.selLoc.y; i++) {
						grd.HightLighttrans(grd.loc[grd.preLoc.x][i], gfx, backgroundbmp);
					}
				}
				if (grd.selLoc.y - grd.preLoc.y < 0)
				{
					for (int i = grd.selLoc.y; i <= grd.preLoc.y; i++) {
						grd.HightLighttrans(grd.loc[grd.preLoc.x][i], gfx, backgroundbmp);
					}
				}
			}
			if (grd.selLoc.y == grd.preLoc.y) {
				if (grd.selLoc.x - grd.preLoc.x > 0)
				{
					for (int i = grd.preLoc.x; i <= grd.selLoc.x; i++) {
						grd.HightLighttrans(grd.loc[i][grd.preLoc.y], gfx, backgroundbmp);
					}
				}
				if (grd.selLoc.x - grd.preLoc.x < 0)
				{
					for (int i = grd.selLoc.x; i <= grd.preLoc.x; i++) {
						grd.HightLighttrans(grd.loc[i][grd.preLoc.y], gfx, backgroundbmp);
					}
				}
			}
			grd.Swap(grd, grd.preLoc, grd.selLoc, gfx, distance, movetemp);
		}
		else {
			grd.HightLight(grd.curLoc, gfx);
			grd.HightLight(grd.selLoc, gfx);
		}
	}
	else if (startScreen && gameoverScreen == false)
	{
		if (home) {
			homeSound.Play();
			home = false;
		}
		if (GameruleScreen == false) {
			gfx.PrintBmp(0, 0, &startscreen);
			gfx.DrawAnimation(&runningpikachu);
			gfx.PrintBmp(320, 430 + Option * 50, &pokeballbmp);
		}
		else {
			gfx.PrintBmp(0, 0, &gamerulebmp);
		}
	}
	if (startScreen == false && gameoverScreen)
	{
		undertaleSound.StopAll();
		gfx.PrintBmp(280, 226, &gameoverscreen);
	}
}

