#include "Ui.hpp"

#include <cstdio>
#include <cstdlib>
#include <boost/format.hpp>

using namespace std;
using namespace boost;

namespace Bastet{

  void voidendwin(){
    endwin();
  }

  void PrepareUiGetch(){ ///gets ready for a getch() in the UI, i.e. empties the char buffer, sets blocking IO
    nodelay(stdscr,TRUE);
    while(getch()!=ERR);
    nodelay(stdscr,FALSE);
  }
  
  BorderedWindow::BorderedWindow(int height, int width, int y, int x){
    if(y==-1 || x==-1){
      int screen_h, screen_w;
      getmaxyx(stdscr,screen_h, screen_w);
      if(y==-1)
	y=(screen_h-height-2)/2-1;
      if(x==-1)
	x=(screen_w-width-2)/2-1;
    }
    _border=newwin(height+2,width+2,y,x);
    _window=derwin(_border,height,width,1,1); 
    //    wattrset(_border,COLOR_PAIR(21));
    RedrawBorder();
  }

  BorderedWindow::~BorderedWindow(){
    delwin(_window);
    delwin(_border);
  }
  
  BorderedWindow::operator WINDOW*(){
    return _window;
  }

  void BorderedWindow::RedrawBorder(){
    box(_border,0,0);
    wrefresh(_border);
  }
 
  int BorderedWindow::GetMinX(){
    int y,x;
    getbegyx(_border,y,x);
    return x;
  }

  int BorderedWindow::GetMinY(){
    int y,x;
    getbegyx(_border,y,x);
    return y;
  }

  int BorderedWindow::GetMaxX(){
    int y,x;
    getmaxyx(_border,y,x);
    return GetMinX()+x;
  }

  int BorderedWindow::GetMaxY(){
    int y,x;
    getmaxyx(_border,y,x);
    return GetMinY()+y;
  }

  Curses::Curses(){
    if(initscr()==NULL){
      fprintf(stderr,"bastet: error while initializing graphics (ncurses library).\n");
      exit(1);
    }
    if(!has_colors()){
      endwin();
      fprintf(stderr,"bastet: no color support, sorry. Ask the author for a black and white version.");
      exit(1);
    }
    
    /* Turn off cursor. */
    curs_set(0);
    atexit(voidendwin); /*make sure curses are properly stopped*/
    
    /* Setup keyboard. We'd like to get each and every character, but
       not to display them on the terminal. */
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    nonl();
    noecho();
    cbreak();
    
    start_color();
    /* 1 - 16 is for blocks */
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_BLUE);
    
    /* 17 - ? is for other things */
    init_pair(17, COLOR_RED, COLOR_BLACK); //points
    init_pair(18, COLOR_YELLOW, COLOR_BLACK); //number of lines
    init_pair(19, COLOR_GREEN, COLOR_BLACK); //level
    init_pair(20, COLOR_YELLOW, COLOR_BLACK); //messages
    init_pair(21, COLOR_WHITE, COLOR_BLACK); //window borders

    /* Set random seed. */
    srandom(time(NULL)+37);
  }

  Ui::Ui(int argc, char **argv):_width(10),_height(20),
				_wellWin(_height,2*_width),
				_nextWin(5,15,_wellWin.GetMinY(),_wellWin.GetMaxX()+1),
				_scoreWin(7,15,_nextWin.GetMaxY(),_nextWin.GetMinX())
  {
  }

  Dot BoundingRect(const std::string &message){ //returns x and y of the minimal rectangle containing the given string
    size_t height=0;
    size_t width=0;
    size_t start=0;
    bool terminate=false;
    while(!terminate){
      size_t next=message.find('\n',start);
      if(next==string::npos){
	next=message.size();
	terminate=true;
      }
      width=max(width,next-start);
      start=next+1;
      height++;
    }
    return Dot(width,height);
  }

  void Ui::MessageDialog(const std::string &message){
    RedrawStatic();

    Dot d=BoundingRect(message);

    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
  }

  int Ui::ChooseLevel(){
    int level=0;
    int ch='0';
    format fmt("    Bastet\n"
	       "\n"
	       "Starting level = %1% \n"
	       "0-9 to change\n"
	       "<SPACE> to start\n");
    string msg;
    while(ch!=' '){
      msg=str(fmt % level);
      PrepareUiGetch();
      MessageDialog(msg);
      ch=getch();
      switch(ch){
      case '0'...'9':
	level=ch-'0';
      }
    }
    return level;
  }

  void Ui::RedrawStatic(){
    _wellWin.RedrawBorder();
    _nextWin.RedrawBorder();
    _scoreWin.RedrawBorder();

    wattrset((WINDOW*)_nextWin,COLOR_PAIR(17));
    mvwprintw(_nextWin,0,0,"Next block:");
    wrefresh(_nextWin);

    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(17));
    mvwprintw(_scoreWin,1,0,"Score:");
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(18));
    mvwprintw(_scoreWin,3,0,"Lines:");
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(19));
    mvwprintw(_scoreWin,5,0,"Level:");
    wrefresh(_scoreWin);

  }

  int Ui::Play(int level){
    int points=0;
    int lines=0;
#if 0
    try{
      while(1){
	FallingBlock b=Well.Insert();
	
	
	
      }    
    } catch(GameOver &go){

    }
#endif
  }

}