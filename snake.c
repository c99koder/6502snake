/*  (C) 2003 Sam Steele
    This file is part of Snake.

    Snake is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Snake is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <peekpoke.h>

#ifdef __REDPOWER__
extern void setMappedRedbusDevice(unsigned char);
extern unsigned getMappedRedbusDevice();
extern void enableRedbus();
extern void disableRedbus();

#define BLACK 0
#define WHITE 0
#define YELLOW 0
#define RED 0
#define GREEN 0

#define BOARD_X 20
#define BOARD_Y 20

#define TOP 130
#define BOT 2
#define LEFT 1
#define RIGHT 129
#define TOP_LEFT 140
#define TOP_RIGHT 155
#define BOT_LEFT 156
#define BOT_RIGHT 158
#define SNAKE '*'
#define GEM 'X'
#endif

#ifdef __C64__
#define BLACK 0
#define WHITE 1
#define YELLOW 7
#define RED 10
#define GREEN 13

#define BOARD_X 38
#define BOARD_Y 20

#define TOP 'C'
#define BOT 'C'
#define LEFT 'B'
#define RIGHT 'B'
#define TOP_LEFT 'U'
#define TOP_RIGHT 'I'
#define BOT_LEFT 'J'
#define BOT_RIGHT 'K'
#define SNAKE 'Q'
#define GEM 'Z'
#endif

char board[BOARD_X][BOARD_Y];

signed char sx,sy,sdx,sdy,dead;
unsigned char ssize,sscore,slives;

void redraw() {
  char scorebuf[10],x,y;
  clrscr();
#ifndef __REDPOWER__
  textcolor(WHITE);
#endif
#ifdef __C64__
  cputs("score: ");
#else
  cputs("Score: ");
#endif
  itoa(sscore, scorebuf, 10);
  cputs(scorebuf);
  gotoxy(BOARD_X - 6,0);
#ifndef __REDPOWER__
  if(dead==0) {
#endif
#ifdef __C64__
    cputs("lives: ");
#else
    cputs("Lives: ");
#endif
    itoa(slives, scorebuf, 10);
    cputs(scorebuf);
#ifndef __REDPOWER__
  }
#endif
#ifndef __REDPOWER__
  textcolor(YELLOW);
#endif
  cputcxy(0,1,TOP_LEFT);
  for(x=0; x<BOARD_X; x++)
    cputcxy(x+1,1,TOP);
  cputcxy(BOARD_X+1,1,TOP_RIGHT);
  for(y=0;y<BOARD_Y;y++) {
    cputcxy(0,y+2,LEFT);
    cputcxy(BOARD_X+1,y+2,RIGHT);
  }
  cputcxy(0,BOARD_Y+2,BOT_LEFT);
  for(x=0; x<BOARD_X; x++)
    cputcxy(x+1,BOARD_Y+2,BOT);
  cputcxy(BOARD_X+1,BOARD_Y+2,BOT_RIGHT);
  gotoxy(0, BOARD_Y + 3);
#ifndef __REDPOWER__
  textcolor(WHITE);
#endif
#ifdef __C64__
  cputs("snakegame v0.02\n\r(c) 2003 sam steele");
#else
  cputs("SnakeGame v0.02\n(C) 2003 Sam Steele");
#endif
}

int main() {
  char x,y;
  char go;
  char dead;
  char scorebuf[10];
  char *b1;

  dead = 0;
  sx = 3;
  sy = 3;
  sdx = 1;
  sdy = 0;
  ssize = 3;
  sscore = 0;
  slives = 3;

#ifdef __REDPOWER__
	enableRedbus();
	setMappedRedbusDevice(1);
#else
  bgcolor(BLACK);
  bordercolor(YELLOW);
#endif
  cursor(0);

#ifdef __C64__
  POKE(53272,21);
#endif

  //initialize the board
  for(x=0;x<BOARD_X;++x) {
    b1 = board[x];
    for(y=0;y<BOARD_Y;++y) {
      b1[y]=0;
    }
  }

  redraw();

  //Randomize the start
  sx=4+(rand()%(BOARD_X-8));
  sy=4+(rand()%(BOARD_Y-8));

  //place an item
  x=rand()%BOARD_X;
  y=rand()%BOARD_Y;
  board[x][y]=255;
#ifndef __REDPOWER__
  textcolor(RED);
#endif
  cputcxy(x+1, y+2, GEM);

  go = 1;

  while(go==1) {
#ifndef __REDPOWER__
    if(dead == 0) {
#endif
      //move head
      sx+=sdx;
      sy+=sdy;
#ifndef __REDPOWER__
      textcolor(GREEN);
#endif
      cputcxy(sx+1,sy+2,SNAKE);

      //check bounds
      if(sx>=BOARD_X || sx<0 || sy>=BOARD_Y || sy<0) { 
        slives--; 
        ssize=3; 
        for(x=0;x<BOARD_X;++x) {
          b1 = board[x];
          for(y=0;y<BOARD_Y;++y) {
            b1[y]=0;
          }
        }
        redraw();
        sx=4+(rand()%(BOARD_X-8));
        sy=4+(rand()%(BOARD_Y-8));
        x=rand()%BOARD_X;
        y=rand()%BOARD_Y;
        board[x][y]=255;
#ifndef __REDPOWER__
        textcolor(RED);
#endif
        cputcxy(x+1, y+2, GEM);
      }

      //check collisions
      if(board[sx][sy]>0) {
        if(board[sx][sy]==255) { //item
          ssize+=2; //increment snake size
          sscore++; //and score
          do { //place a new item in an empty spot
            x=rand()%BOARD_X; y=rand()%BOARD_Y;
          } while(board[x][y]!=0);
          board[x][y]=255;
#ifndef __REDPOWER__
          textcolor(RED);
#endif
          cputcxy(x+1, y+2, GEM);
          //make the stretch work correctly
          for(x=0;x<BOARD_X;++x) {
            b1 = board[x];
            for(y=0;y<BOARD_Y;++y) {
              if(b1[y]>0&&b1[y]!=255) b1[y]+=2;
            }
          }
        } else { //snake
          slives--; 
          ssize=3; 
          for(x=0;x<BOARD_X;++x) {
            b1 = board[x];
            for(y=0;y<BOARD_Y;++y) {
              b1[y]=0;
            }
          }
          redraw();
          sx=4+(rand()%(BOARD_X-8));
          sy=4+(rand()%(BOARD_Y-8));
          x=rand()%BOARD_X;
          y=rand()%BOARD_Y;
          board[x][y]=255;
#ifndef __REDPOWER__
          textcolor(RED);
#endif
          cputcxy(x+1, y+2, GEM);
        }
      }
#ifndef __REDPOWER__
      if(slives<=0) dead=1;
#endif
      board[sx][sy]=ssize+1;

      //decrement the array (simulates motion)
      for(x=0;x<BOARD_X;++x) {
        b1 = board[x];
        for(y=0;y<BOARD_Y;++y) {
          if(b1[y]>0&&b1[y]!=255) {
            --b1[y];
            if(b1[y] == 0)
              cputcxy(x+1,y+2,' ');
          }
        }
      }
#ifndef __REDPOWER__
    }
#endif
    //render the screen
    gotoxy(0,0);
#ifndef __REDPOWER__
    textcolor(WHITE);
#endif
#ifdef __C64__
    cputs("score: ");
#else
    cputs("Score: ");
#endif
    itoa(sscore, scorebuf, 10);
    cputs(scorebuf);
    gotoxy(BOARD_X - 6,0);
#ifndef __REDPOWER__
    if(dead==0) {
#endif
#ifdef __C64__
      cputs("lives: ");
#else
      cputs("Lives: ");
#endif
      itoa(slives, scorebuf, 10);
      cputs(scorebuf);
#ifndef __REDPOWER__
	  }
    if(dead==1) {
#ifndef __REDPOWER__
      textcolor(RED);
#endif
#ifdef __C64__
      gotoxy((BOARD_X/2) - 4,11); cputs("you died!");
      gotoxy((BOARD_X/2) - 5,12); cputs("press space");
#else
      gotoxy((BOARD_X/2) - 4,11); cputs("You Died!");
      gotoxy((BOARD_X/2) - 5,12); cputs("Press SPACE");
#endif
    }
    for(x=0;x<6;--x) { //we do this 6 times so that we can poll the keyboard 6 times more than we move the snake on screen
#endif
      if(kbhit()) {
        switch(cgetc()) {
        case 'p':
          do {
            //
          } while(!kbhit());
          break;
        case 'w':
          if(sdy==0) {
            sdx=0;
            sdy=-1;
          }
          break;
        case 's':
          if(sdy==0) {
            sdx=0;
            sdy=1;
          }
          break;
        case 'a':
          if(sdx==0) {
            sdx=-1;
            sdy=0;
          }
          break;
        case 'd':
          if(sdx==0) {
            sdx=1;
            sdy=0;
          }
          break;
        case ' ':
          go=0;
          break;
        }
      }
#ifndef __REDPOWER__
    }
#endif
  }
  return 0;
}  
