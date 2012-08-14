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
#endif

#define BOARD_X 20
#define BOARD_Y 20

char board[BOARD_X][BOARD_Y];

signed char sx,sy,sdx,sdy,dead;
unsigned char ssize,sscore,slives;

void redraw() {
  char scorebuf[10],x,y;
  clrscr();
  //color(1,0);
  cputs("Score: ");
  itoa(sscore, scorebuf, 10);
  cputs(scorebuf);
  gotoxy(BOARD_X - 6,0);
  if(dead==0) {
    cputs("Lives: ");
    itoa(slives, scorebuf, 10);
    cputs(scorebuf);
  }
  //color(7,0); //yellow on black
  cputcxy(0,1,'+');
  for(x=0; x<BOARD_X; x++)
    cputcxy(x+1,1,'-');
  cputcxy(BOARD_X+1,1,'+');
  for(y=0;y<BOARD_Y;y++) {
    //color(7,0); //yellow on black
    cputcxy(0,y+2,'|');
    cputcxy(BOARD_X+1,y+2,'|');
  }
  cputcxy(0,BOARD_Y+2,'+');
  for(x=0; x<BOARD_X; x++)
    cputcxy(x+1,BOARD_Y+2,'-');
  cputcxy(BOARD_X+1,BOARD_Y+2,'+');
  gotoxy(0, BOARD_Y + 3);
  cputs("SnakeGame v0.02\n\r(C) 2003 Sam Steele");
  
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
#endif
  cursor(0);

  //initialize the board
  for(x=0;x<BOARD_X;x++) {
    for(y=0;y<BOARD_Y;y++) {
      board[x][y]=0;
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
  cputcxy(x+1, y+2, 'X');

  go = 1;

  while(go==1) {
    if(dead == 0) {
      //move head
      sx+=sdx;
      sy+=sdy;
      cputcxy(sx+1,sy+2,'*');

      //check bounds
      if(sx>=BOARD_X || sx<0 || sy>=BOARD_Y || sy<0) { 
        slives--; 
        ssize=3; 
        for(x=0;x<BOARD_X;++x) {
          for(y=0;y<BOARD_Y;++y) {
            board[x][y]=0;
          }
        }
        redraw();
        sx=4+(rand()%(BOARD_X-8));
        sy=4+(rand()%(BOARD_Y-8));
        x=rand()%BOARD_X;
        y=rand()%BOARD_Y;
        board[x][y]=255;
        cputcxy(x+1, y+2, 'X');
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
          cputcxy(x+1, y+2, 'X');
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
            for(y=0;y<BOARD_Y;++y) {
              board[x][y]=0;
            }
          }
          redraw();
          sx=4+(rand()%(BOARD_X-8));
          sy=4+(rand()%(BOARD_Y-8));
          x=rand()%BOARD_X;
          y=rand()%BOARD_Y;
          board[x][y]=255;
          cputcxy(x+1, y+2, 'X');
        }
      }
      if(slives<=0) dead=1;
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
    }
    //render the screen
    gotoxy(0,0);
    //color(1,0);
    cputs("Score: ");
    itoa(sscore, scorebuf, 10);
    cputs(scorebuf);
    cputs("\n");
    gotoxy(BOARD_X - 6,0);
    if(dead==0) {
      cputs("Lives: ");
      itoa(slives, scorebuf, 10);
      cputs(scorebuf);
	  }
    if(dead==1) {
      //color(2,0);
      gotoxy((BOARD_X/2) - 4,12); cputs("You Died!");
      gotoxy((BOARD_X/2) - 4,13); cputs("Press ESC");
    }
    for(x=0;x<6;--x) { //we do this 6 times so that we can poll the keyboard 6 times more than we move the snake on screen
      if(kbhit()) {
        switch(cgetc()) {
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
        case 27:
          go=0;
          break;
        }
      }
    }
  }
  return 0;
}  
