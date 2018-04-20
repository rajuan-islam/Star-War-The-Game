/*	Project Name:		STAR WARS
	Type:				Space War Game
	Platform:			C++ and iGraphics
	Used Softwares:		Microsoft Visual C++ 2006, Sublime Text 2

	Group name: BFS
	Members:	1. Md. Rajuan Islam				ID: 13.02.04.060
				2. Md. Fahim Mohiuddin			ID: 13.02.04.062
				3. Ebne Faiz Al-Amin			ID: 13.02.04.063

				Year:		1st
				Semester:	2nd
				Department of Computer Science and Engineering
				Ahsanullah University of Science and Technology

	Objective:
		To develop a 2D game for the course mentioned below.
		Course title:	CSE 1200
		Course name:	Software Development - 1

	" bismillahir rahmanir rahim "
*/

// C headers
# include "iGraphics.h"

// C++ STL Classes
# include <iostream>
# include <vector>
using namespace std;

// timer count
int sec;
int player_down;
int intro_sec;
int load_sec;

// timer indices
int back_timer, fire_timer, sec_timer, enemy_timer;

// variables
int score;
int life;
bool game_pause;
int high_score;
char high_name[100];
char name[100];

// Macro List
# define GAME_X 800
# define GAME_Y 700
# define MAIN_X 1000
# define MAIN_Y 700

// Application levels
# define INTRODUCTION 1
# define MAIN_MENU 2
# define LOAD_GAME 3
# define GAME_PLAY 4
# define NEW_HIGH_SCORE 5
# define HIGH_SCORE 6
# define CREDIT 7
# define HELP 8
int LEVEL;

// function prototypes which are needed
void initializeGame();

// common structures -------------------------------------------------------------------------------------------------------
struct bullet
{
    int x,y;
    bullet( int u, int v )
    {
        x = u, y = v;
    }
};
vector <bullet> plf,pmf,prf; // bullets for the player
vector <bullet> ef;			 // bullets for the enemies

// foods --------------------------------------------------------------
struct food
{
    int x, y, value;

    food( int u, int v, int w )
    {
        x = u;
        y = v;
        value = w;
    }

    void draw()
    {
        if( value == 1 )
        {
            iShowBMP( x-20, y-20, "img/life40.bmp" );
        }
        else if( value == 2 )
        {
            iShowBMP( x-20, y-20, "img/pow40.bmp" );
        }
    }

    bool destroy( int bx, int by )
    {
        int x1,x2,y1,y2;

        x1 = x-20;
        y1 = y-20;
        x2 = x+20;
        y2 = y+20;

        if( bx>=x1 && bx<=x2 && by>=y1 && by<=y2 ) return true;
        return false;
    }
} ;
vector <food> foods;

// demo variables ----------------------------------------------------------------------------------------------------------
int point_x = 0, point_y = 0;
int point_x_2 = 0, point_y_2 = 0;
void point_show()
{
    char str[100];
    iSetColor(255,255,255);
    sprintf( str, "%d", point_x );
    iText( 820, 350, str, GLUT_BITMAP_HELVETICA_18 );
    sprintf( str, "%d", point_y );
    iText( 900, 350, str, GLUT_BITMAP_HELVETICA_18 );

    sprintf( str, "%d", point_x_2 );
    iText( 820, 300, str, GLUT_BITMAP_HELVETICA_18 );
    sprintf( str, "%d", point_y_2 );
    iText( 900, 300, str, GLUT_BITMAP_HELVETICA_18 );
}

//--------------------------------------------------------------------------------------------------------------------------

// Background construction
bool first_time_background;
int point_add_count;

struct point
{
    int x, y;

    point( int u, int v )
    {
        x = u, y = v;
    }

    void draw()
    {
        iSetColor( 255, 255, 255 );
        iPoint( x, y );
    }
};
vector <point> points;

void changeBackGround()
{
    // variables
    int i;

    // if it is for the first time
    if( first_time_background )
    {
        first_time_background = false;
        for( i=0; i<30; i++ )
        {
            int x = rand()%801;
            int y = rand()%701;
            points.push_back( point( x, y ) );
        }
        return;
    }

    // changing point coordinates
    for( i=0; i<points.size(); )
    {
        points[i].y -= 1;
        if( points[i].y < 0 ) points.erase( points.begin()+i );
        else i++;
    }

    // adding new point after time gap
    point_add_count++;
    if( point_add_count == 25 )
    {
        point_add_count = 0;
        int x = rand()%801;
        int y = rand()%101 + 700;
        points.push_back( point( x, y ) );
    }
}

void backGround()
{
    // variables
    int i;

    // background point
    for( i=0; i<points.size(); i++ )
    {
        points[i].draw();
    }

    // demo purpose margin line on the right side
    //iLine( GAME_X, 0, GAME_X, GAME_Y );
    // demo purpose time count on the right side
    //char str[100]; sprintf( str, "%d\n", sec );
    //iText( GAME_X+10, 10, str, GLUT_BITMAP_HELVETICA_18 );

    // demo purpose point click showing
}

void scoring()
{
    // score table background
    /*iSetColor(0,0,0);
    iFilledRectangle( GAME_X, 0, 200, 700 );
    iSetColor(255,255,255);
    iLine( GAME_X, 0, GAME_X, GAME_Y );*/
	iShowBMP( GAME_X, 0, "img/score_board.bmp" );

    char str[100];

    // score print
    iSetColor( 255, 255, 255 );
    iText( 830, 650, "SCORE", GLUT_BITMAP_TIMES_ROMAN_24 );
    sprintf( str, "%d", score );
    iText( 830, 600, str, GLUT_BITMAP_TIMES_ROMAN_24 );

    // health print
    iText( 830, 550, "HEALTH", GLUT_BITMAP_TIMES_ROMAN_24 );
    sprintf( str, "%d", life );
    iText( 830, 500, str, GLUT_BITMAP_TIMES_ROMAN_24 );
}

// Enemy Aircrafts --------------------------------------------------------------------------------------------------------
// The basic enemy ---------------------------------------------------------|
struct basic
{
    int x, y;

    basic( int u, int v )
    {
        x = u;
        y = v;
    }

    void draw()
    {
        // base circle
        iSetColor( 49, 89, 134 );
        iFilledCircle( x, y, 56 );

        // body line circle
        iSetColor( 0, 0, 0 );
        iCircle( x, y, 37 );
        iCircle( x, y, 32 );

        // backline straight line
        iLine( x, y+37, x, y+56 );

        // front circles
        iFilledCircle( x, y-45, 5 );
        iFilledCircle( x+19, y-43, 5 );
        iFilledCircle( x-19, y-43, 5 );

        // side extensions
        iSetColor( 151, 196, 217 );
        iFilledRectangle( x-43, y-42, 3, 42 );
        iFilledRectangle( x+40, y-42, 3, 42 );

        // front panel window
        iSetColor( 100, 100, 100 );
        iFilledRectangle( x-24, y-18, 48, 10 );
        iSetColor( 0, 0, 0 );
        iRectangle( x-24, y-18, 48, 10 );
    }

    bool destroy( int bx, int by )
    {
        if( sqrt( pow( x-bx, 2.0 ) + pow( y-by, 2.0 ) ) < 56 ) return true;
        return false;
    }

    void deadDraw()
    {
        iShowBMP( x-40, y-40, "img/basic_enemy_dead.bmp" );
    }
} ;
vector <basic> basics;

// the middle enemy ---------------------------------------------------
struct middle
{
    int x, y;

    middle( int u, int v )
    {
        x = u;
        y = v;
    }

    bool destroy( int mx, int my )
    {
        int x1=x-46, x2=x+46, y1=y-25, y2=y+27;
        if( mx>=x1 && mx<=x2 && my>=y1 && my<=y2 ) return true;
        return false;
    }

    void draw()
    {
        // drawing front circle
        iSetColor( 175, 97, 50 );
        iFilledCircle( x, y, 11 );

        // drawing middle body
        iSetColor( 115, 15, 15 );
        iFilledRectangle( x-9, y, 18, 14 );

        // wing tips
        iSetColor( 255, 255, 255 );
        iLine( x-9-10, y-4, x-9-10, y-4-10 );
        iLine( x+9+10, y-4, x+9+10, y-4-10 );

        // wings
        {
            iSetColor( 193, 32, 32 );

            // drawing left wing
            {
                double wing_x[] = { x-9-36, x-9-36, x-9, x-9 };
                double wing_y[] = { y-4, y-4+10, y+18, y-4 };
                iFilledPolygon( wing_x, wing_y, 4 );
            }

            // drawing right wing
            {
                double wing_x[] = { x+9+36, x+9+36, x+9, x+9 };
                double wing_y[] = { y-4, y-4+10, y+18, y-4 };
                iFilledPolygon( wing_x, wing_y, 4 );
            }
        }

        // extension
        {
            iSetColor( 115, 15, 15 );

            // right extension
            {
                // lower
                {
                    double ex_x[] = { x+9+23, x+9+23+6, x+9+36+6, x+9+36 };
                    double ex_y[] = { y-4-11-10, y-4-11-10, y-4, y-4 };
                    iFilledPolygon( ex_x, ex_y, 4 );
                }
                // middle
                {
                    double ex_x[] = { x+9+36, x+9+36+6, x+9+36+6, x+9+36 };
                    double ex_y[] = { y-4, y-4, y-4+10, y-4+10 };
                    iFilledPolygon( ex_x, ex_y, 4 );
                }
                // top
                {
                    double ex_x[] = { x+9+36, x+9+36+6, x+9+23+6, x+9+23 };
                    double ex_y[] = { y-4+10, y-4+10, y-4+10+11+10, y-4+10+11+10 };
                    iFilledPolygon( ex_x, ex_y, 4 );
                }
            }

            // left extension
            {
                // lower
                {
                    double ex_x[] = { x-9-23, x-9-23-6, x-9-36-6, x-9-36 };
                    double ex_y[] = { y-4-11-10, y-4-11-10, y-4, y-4 };
                    iFilledPolygon( ex_x, ex_y, 4 );
                }
                // middle
                {
                    double ex_x[] = { x-9-36, x-9-36-6, x-9-36-6, x-9-36 };
                    double ex_y[] = { y-4, y-4, y-4+10, y-4+10 };
                    iFilledPolygon( ex_x, ex_y, 4 );
                }
                // top
                {
                    double ex_x[] = { x-9-36, x-9-36-6, x-9-23-6, x-9-23 };
                    double ex_y[] = { y-4+10, y-4+10, y-4+10+11+10, y-4+10+11+10 };
                    iFilledPolygon( ex_x, ex_y, 4 );
                }
            }
        }
    }

    void deadDraw()
    {
        iShowBMP( x-40, y-40, "img/basic_enemy_dead.bmp" );
    }
};
vector <middle> middles;

// the final enemy ----------------------------------------------------
struct final
{
    int x, y;
    bool fire;

    final( int u, int v )
    {
        x = u;
        y = v;
        fire = true;
    }

    bool destroy( int bx, int by )
    {
        int x1=x-52;
        int x2=x+50;
        int y1=y-80;
        int y2=y+20;

        if( bx>=x1 && bx<=x2 && by>=y1 && by<=y2 ) return true;
        return false;
    }

    void deadDraw()
    {
        iShowBMP( x-40, y-40, "img/basic_enemy_dead.bmp" );
    }

    void draw()
    {
        //skeleton
        iSetColor( 140, 140, 140 );
        iLine( x, y-15, x-48, y-30 );
        iLine( x, y-15, x+48, y-30 );
        iLine( x-50, y, x-25, y-25 );
        iLine( x+50, y, x+25, y-25 );
        iLine( x-15, y, x-25, y-25 );
        iLine( x+15, y, x+25, y-25 );

        // wings
        iSetColor( 46, 165, 105 );
        iFilledRectangle( x-53, y-80, 6, 103 );
        iSetColor(0,0,0);
        iRectangle( x-53, y-80, 6, 103 );
        iSetColor( 46, 165, 105 );
        iFilledRectangle( x+47, y-80, 6, 103 );
        iSetColor(0,0,0);
        iRectangle( x+47, y-80, 6, 103 );
        // second part
        iSetColor( 46, 165, 105 );
        iFilledRectangle( x-53, y-80, 25, 10 );
        iFilledRectangle( x+28, y-80, 25, 10 );
        iSetColor(0,0,0);
        iRectangle( x-53, y-80, 25, 10 );
        iRectangle( x+28, y-80, 25, 10 );


        // middle body
        iSetColor( 0, 98, 49 );
        iFilledRectangle( x-50, y, 100, 20 );
        iSetColor( 0, 0, 0 );
        iRectangle( x-50, y, 100, 20 );
        iSetColor( 64, 128, 115 );
        iFilledCircle( x, y, 20 );
        iSetColor(0,0,0);
        iCircle( x, y, 20 );
        iCircle( x, y, 17 );
        iSetColor( 80, 80, 80 );
        iFilledCircle( x, y-10, 5 );
        iSetColor(0,0,0);
        iCircle( x, y-10, 5 );
    }
};
vector <final> finals;

// food----------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------
// Player Aircraft
struct player
{
    // variables
    int x, y;
    bool boost, fire;
    int fireLevel;

    // functions
    void initialize()
    {
        x=200, y=200;
        fire=boost=false;
        fireLevel = 0;

        plf.clear();
        pmf.clear();
        pmf.clear();
    }

    void fireBlastDraw()
    {
        iSetColor( 255, 255, 0 );
        iFilledCircle( x, y+30+15, 10 );
    }

    void fireDraw()
    {
        int i;
        iSetColor( 255, 0, 0 );
        // left
        for( i=0; i<plf.size(); i++ ) iFilledCircle( plf[i].x, plf[i].y, 3 );
        // middle
        for( i=0; i<pmf.size(); i++ ) iFilledCircle( pmf[i].x, pmf[i].y, 3 );
        // right
        for( i=0; i<prf.size(); i++ ) iFilledCircle( prf[i].x, prf[i].y, 3 );
    }

    void boostDraw()
    {
        iSetColor( 255, 0, 0 );//color: red
        iFilledEllipse( x, y-15, 7, 30 );

        iSetColor( 255, 255, 0 ); // color: yellow
        iFilledEllipse( x, y-15, 3, 20 );
    }

    void bodyDraw()
    {
        // drawing front
        {
            // drawing the front base
            iSetColor( 192, 192, 192 ); // color: silver
            {
                double front_x[] = { x-10, x, x+10 };
                double front_y[] = { y+30, y+30+15, y+30 };
                iFilledPolygon( front_x, front_y, 3 );
            }
            // drawing the front top
            iSetColor( 0, 0, 0 ); // color: black
            {
                iFilledCircle( x, y+30, 7 );
            }
        }
        /* drawing wings*/
        {
            // wing base
            iSetColor( 192, 192, 192 ) ; // color: silver
            {
                double wing_x[] = { x-15-30, x-15, x+15, x+15+30 };
                double wing_y[] = { y, y+20, y+20, y };
                iFilledPolygon( wing_x, wing_y, 4 );
            }
            // wing top
            iSetColor( 255, 255, 0 ); // color : yellow
            {
                double wing_x[] = { x-15-15, x-15, x+15, x+15+15 };
                double wing_y[] = { y+4, y+13, y+13, y+4 };
                iFilledPolygon( wing_x, wing_y, 4 );
            }
            // wing tip
            iSetColor( 255, 255, 255 );
            iLine( x-15-20, y, x-15-20, y+20 ); // left tip
            iLine( x+15+20, y, x+15+20, y+20 ); // right tip
        }
        // drawing back wings
        {
            iSetColor( 192, 192, 192 ) ; // color: silver
            {
                double wing_x[] = { x-15-10, x-15, x+15, x+15+10 };
                double wing_y[] = { y-15, y, y, y-15 };
                iFilledPolygon( wing_x, wing_y, 4 );
            }
        }
        /* drawing middle */
        {
            // drawing the middle body
            iSetColor( 128, 128, 128 );	// color: grey
            iFilledRectangle( x-15, y-15, 30, 30+15 );

            iSetColor( 0, 0, 0 ); // color: black
            iRectangle( x-15, y-15, 30, 30+15 );
            iRectangle( x-12, y-12, 24, 30+8 );

            iSetColor( 80, 80, 80 ); // color: grey
            iFilledRectangle( x-9, y-8, 18, 8 );

            iSetColor( 0, 0, 0 ); // color: black
            iRectangle( x-9, y-8, 18, 8 );
        }
    }

    bool destroy( int bx, int by )
    {
        int x1, x2, y1, y2;
        // first
        x1 = x-15;
        y1 = y+20;
        x2 = x+15;
        y2 = y+45;
        if( bx>=x1 && bx<=x2 && by>=y1 && by<=y2 ) return true;
        // second middle
        x1 = x-15-20;
        y1 = y;
        x2 = x+15+20;
        y2 = y+20;
        if( bx>=x1 && bx<=x2 && by>=y1 && by<=y2 ) return true;
        // third
        x1 = x-15-10;
        y1 = y-15;
        x2 = x+15+10;
        y2 = y;
        if( bx>=x1 && bx<=x2 && by>=y1 && by<=y2 ) return true;

        return false;
    }

    bool collide( basic e )
    {
        int tx, ty;
        // top point
        tx = x;
        ty = y+45;
        if( e.destroy( tx, ty )) return true;

        // front left
        tx = x-15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;
        // front right
        tx = x+15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;

        // wing mid left
        tx = x-30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;
        // wing mid right
        tx = x+30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;

        // wing tip left
        tx = x - 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;
        // wing tip right
        tx = x + 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;

        // back wing tip left
        tx = x-25;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;
        // back wing tip right
        tx = x+25;
        ty = y - 15;
        if( e.destroy( tx, ty )) return true;

        // back mid
        tx = x;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;

        return false;
    }

    bool collide( middle e )
    {
        int tx, ty;
        // top point
        tx = x;
        ty = y+45;
        if( e.destroy( tx, ty )) return true;

        // front left
        tx = x-15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;
        // front right
        tx = x+15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;

        // wing mid left
        tx = x-30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;
        // wing mid right
        tx = x+30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;

        // wing tip left
        tx = x - 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;
        // wing tip right
        tx = x + 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;

        // back wing tip left
        tx = x-25;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;
        // back wing tip right
        tx = x+25;
        ty = y - 15;
        if( e.destroy( tx, ty )) return true;

        // back mid
        tx = x;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;

        return false;
    }

    bool collide( final e )
    {
        int tx, ty;
        // top point
        tx = x;
        ty = y+45;
        if( e.destroy( tx, ty )) return true;

        // front left
        tx = x-15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;
        // front right
        tx = x+15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;

        // wing mid left
        tx = x-30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;
        // wing mid right
        tx = x+30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;

        // wing tip left
        tx = x - 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;
        // wing tip right
        tx = x + 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;

        // back wing tip left
        tx = x-25;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;
        // back wing tip right
        tx = x+25;
        ty = y - 15;
        if( e.destroy( tx, ty )) return true;

        // back mid
        tx = x;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;

        return false;
    }

    bool collide( food e )
    {
        int tx, ty;
        // top point
        tx = x;
        ty = y+45;
        if( e.destroy( tx, ty )) return true;

        // front left
        tx = x-15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;
        // front right
        tx = x+15;
        ty = y+30;
        if( e.destroy( tx, ty )) return true;

        // wing mid left
        tx = x-30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;
        // wing mid right
        tx = x+30;
        ty = y+11;
        if( e.destroy( tx, ty )) return true;

        // wing tip left
        tx = x - 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;
        // wing tip right
        tx = x + 45;
        ty = y;
        if( e.destroy( tx, ty )) return true;

        // back wing tip left
        tx = x-25;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;
        // back wing tip right
        tx = x+25;
        ty = y - 15;
        if( e.destroy( tx, ty )) return true;

        // back mid
        tx = x;
        ty = y-15;
        if( e.destroy( tx, ty )) return true;

        return false;
    }

    void draw()
    {
        if( fire )
        {
            fireBlastDraw();
            fire = false;
        }

        fireDraw();

        if( boost )
        {
            boostDraw();
            boost = false;
        }
        bodyDraw();
    }

    void deadDraw()
    {
        iShowBMP( x-40, y-40, "img/basic_enemy_dead.bmp" );
    }
} plane ;

//-------------------------------------------------------------------------------------------------------------------------
void clear_all();
void iDraw()
{	
	// GAME PLAY
	if( LEVEL == GAME_PLAY )
    {
        // variables
        int i,j;

        iClear();
        //drawing the background
        backGround();
        //point_show();

        // basic enemies
        for( i=0; i<basics.size();  )
        {
            bool gone = false;

            // left bullet list
            for( j=0; j<plf.size(); )
            {
                if( basics[i].destroy( plf[j].x, plf[j].y ) )
                {
                    basics[i].deadDraw();
                    basics.erase( basics.begin()+i );
                    plf.erase( plf.begin() + j );
                    gone = true;
                    score += 5;

                    goto last;
                }
                else j++;
            }
            // middle bullet list
            for( j=0; j<pmf.size(); )
            {
                if( basics[i].destroy( pmf[j].x, pmf[j].y ) )
                {
                    basics[i].deadDraw();
                    basics.erase( basics.begin()+i );
                    pmf.erase( pmf.begin() + j );
                    gone = true;
                    score += 5;

                    goto last;
                }
                else j++;
            }
            // right bullet list
            for( j=0; j<prf.size(); )
            {
                if( basics[i].destroy( prf[j].x, prf[j].y ) )
                {
                    basics[i].deadDraw();
                    basics.erase( basics.begin()+i );
                    prf.erase( prf.begin() + j );
                    gone = true;
                    score += 5;

                    goto last;
                }
                else j++;
            }

last:
            if( !gone )
            {
                basics[i].draw();
                i++;
            }
        }
        // middle enemies
        for( i=0; i<middles.size(); )
        {
            bool gone = false;

            // left bullet list
            for( j=0; j<plf.size(); )
            {
                if( middles[i].destroy( plf[j].x, plf[j].y ) )
                {
                    middles[i].deadDraw();
                    middles.erase( middles.begin()+i );
                    plf.erase( plf.begin() + j );
                    gone = true;
                    score += 5;

                    goto second_last;
                }
                else j++;
            }
            // middle bullet list
            for( j=0; j<pmf.size(); )
            {
                if( middles[i].destroy( pmf[j].x, pmf[j].y ) )
                {
                    middles[i].deadDraw();
                    middles.erase( middles.begin()+i );
                    pmf.erase( pmf.begin() + j );
                    gone = true;
                    score += 5;

                    goto second_last;
                }
                else j++;
            }
            // right bullet list
            for( j=0; j<prf.size(); )
            {
                if( middles[i].destroy( prf[j].x, prf[j].y ) )
                {
                    middles[i].deadDraw();
                    middles.erase( middles.begin()+i );
                    prf.erase( prf.begin() + j );
                    gone = true;
                    score += 5;

                    goto second_last;
                }
                else j++;
            }

second_last:
            if( !gone )
            {
                middles[i].draw();
                i++;
            }
        }
        // final enemies
        for( i=0; i<finals.size(); )
        {
            bool gone = false;

            // left bullet list
            for( j=0; j<plf.size(); )
            {
                if( finals[i].destroy( plf[j].x, plf[j].y ) )
                {
                    finals[i].deadDraw();
                    finals.erase( finals.begin()+i );
                    plf.erase( plf.begin() + j );
                    gone = true;
                    score += 5;

                    goto third_last;
                }
                else j++;
            }
            // middle bullet list
            for( j=0; j<pmf.size(); )
            {
                if( finals[i].destroy( pmf[j].x, pmf[j].y ) )
                {
                    finals[i].deadDraw();
                    finals.erase( finals.begin()+i );
                    pmf.erase( pmf.begin() + j );
                    gone = true;
                    score += 5;

                    goto third_last;
                }
                else j++;
            }
            // right bullet list
            for( j=0; j<prf.size(); )
            {
                if( finals[i].destroy( prf[j].x, prf[j].y ) )
                {
                    finals[i].deadDraw();
                    finals.erase( finals.begin()+i );
                    prf.erase( prf.begin() + j );
                    gone = true;
                    score += 5;

                    goto third_last;
                }
                else j++;
            }

third_last:
            if( !gone )
            {
                finals[i].draw();
                i++;
            }
        }

        //iSetColor(255,255,255);
        //iText( 225, 340, "PLAYER DOWN! PLAYER DOWN!", GLUT_BITMAP_TIMES_ROMAN_24 );
        //iShowBMP( 238, 252, "player_down.bmp" );
        //iText( 225, 340, "GAME OVER!!!", GLUT_BITMAP_TIMES_ROMAN_24 );
        //iShowBMP( 225, 250, "game_over.bmp" );

        // pause time handle between lives
        if( player_down )
        {
            if( life )
            {
                iShowBMP( 238, 252, "img/player_down.bmp" );
            }
            else
            {
                iShowBMP( 238, 252, "img/game_over.bmp" );
            }

            goto end_of_line;
        }

        // testing enemy bullets and drawing them
        for( i=0; i<ef.size(); )
        {
            if( plane.destroy( ef[i].x, ef[i].y ))
            {
                plane.deadDraw();
                ef.erase( ef.begin() + i );

                // life down
                plane.fireLevel = 0;
                life--;
                clear_all();
                sec = 1;
                player_down = 1;
                goto end_of_line;
            }
            else
            {
                iSetColor( 255,255,128 );
                iFilledCircle( ef[i].x, ef[i].y, 3 );

                i++;
            }
        }
        // testing collision with enemies
        // basics
        for( i=0; i<basics.size(); i++ )
        {
            if( plane.collide( basics[i] ))
            {
                plane.deadDraw();

                // life down
                plane.fireLevel = 0;
                life--;
                clear_all();
                sec = 1;
                player_down = 1;
                goto end_of_line;
            }
        }
        // middles
        for( i=0; i<middles.size(); i++ )
        {
            if( plane.collide( middles[i] ))
            {
                plane.deadDraw();

                // life down
                life--;
                clear_all();
                sec = 1;
                player_down = 1;
                goto end_of_line;
            }
        }
        // finals
        for( i=0; i<finals.size(); i++ )
        {
            if( plane.collide( finals[i] ))
            {
                plane.deadDraw();

                // life down
                life--;
                clear_all();
                sec = 1;
                player_down = 1;
                goto end_of_line;
            }
        }

        // testing food collisions
        for( i=0; i<foods.size(); )
        {
            if( plane.collide( foods[i] ) )
            {
                if( foods[i].value == 1 ) life++;
                else if( foods[i].value == 2 ) plane.fireLevel = 1;

                foods.erase( foods.begin() + i );
            }
            else
            {
                foods[i].draw();
                i++;
            }
        }
        plane.draw();

end_of_line:
        scoring();
        // pause menu
        if(game_pause) iShowBMP( 238, 252, "img/pause_menu.bmp" );
        // demo debug assist
        //point_show();
    }
	
	// INTRODUCTION
	else if( LEVEL == INTRODUCTION )
	{
		if( intro_sec < 3 )
		{
			iShowBMP(0,0,"img/1.bmp");
		}
		else if( intro_sec < 5 )
		{
			iShowBMP(0,0,"img/2.bmp");
		}
		else if( intro_sec < 7 )
		{
			iShowBMP(0,0,"img/3.bmp");
		}
		else
		{
			iShowBMP(0,0,"img/4.bmp");
		}
	}

	// MAIN MENU
	else if( LEVEL == MAIN_MENU )
	{
		iShowBMP(0,0,"img/main_menu.bmp");
	}

	// HIGH SCORE
	else if( LEVEL == HIGH_SCORE )
	{
		//place your drawing codes here
		iClear();
		iShowBMP(0,0,"img/high_score.bmp");
	
		iSetColor(255,255,255);
		char str[100];
		if(high_score)
		{
			strcpy(str,"Name : ");
			strcat(str,high_name);
			iText( 116, 406, str, GLUT_BITMAP_TIMES_ROMAN_24 );

			sprintf(str,"Score : %d", high_score );
			iText( 116, 360, str, GLUT_BITMAP_TIMES_ROMAN_24 );
		}
		else
		{
			iText( 116, 406, "...No high score yet!...", GLUT_BITMAP_TIMES_ROMAN_24 );
		}
	}

	// HELP
	else if( LEVEL == HELP )
	{
		iClear();
		iShowBMP(0,0,"img/help.bmp");
	}

	// CREDIT
	else if( LEVEL == CREDIT )
	{
		iClear();
		iShowBMP(0,0,"img/credit.bmp");
	}

	// LOAD GAME
	else if( LEVEL == LOAD_GAME )
	{
		iClear();

		if( load_sec < 4 ) iShowBMP(0,0,"img/load_1.bmp");
		else iShowBMP(0,0,"img/load_2.bmp");

		iSetColor(255,255,255);
		iText(10, 137, "LOADING......", GLUT_BITMAP_TIMES_ROMAN_24 );

		iRectangle(10,100,980,22);
	
		iSetColor(255,0,0);
		iFilledRectangle( 14, 104, 162*load_sec , 12 );
	}

	// NEW HIGH SCORE
	else if( LEVEL == NEW_HIGH_SCORE )
	{
		iClear();

		// background
		iShowBMP(0,0,"img/new_high_score.bmp");
		iText( 123, 121, name, GLUT_BITMAP_TIMES_ROMAN_24 );

		// drawing the box
		iSetColor(255,255,255);
		iRectangle(112,110,385,40);
	}
}

void clear_all()
{
    basics.clear();
    middles.clear();
    finals.clear();
    foods.clear();

    plf.clear();
    prf.clear();
    pmf.clear();

    ef.clear();
}

// transitional functions
void after_game()
{
    if( score > high_score )
	{
		high_score = score;
		strcpy( name, "" );
		LEVEL = NEW_HIGH_SCORE;
	}
	else
	{
		LEVEL = MAIN_MENU;
	}
}

void read_data()
{
	// data reading initialization
	FILE *f;
	f = fopen("data/record.txt","r");
	fscanf( f, "%d", &high_score );
	if(high_score<0) high_score = 0;
	if(high_score)
	{
		fscanf( f, "%s", high_name );
		int i, len=strlen(high_name);
		for(i=0;i<len;i++)
		{
			if(high_name[i]=='_')
			{
				high_name[i]=' ';
			}
		}
	}
	fclose(f);
}

void write_data()
{
	// data writing
	FILE *f;
	f = fopen("data/record.txt","w");
	fprintf(f,"%d\n",high_score);
	if(high_score)
	{
		int i, len=strlen(high_name);
		for(i=0;i<len;i++)
		{
			if(high_name[i]==' ')
			{
				high_name[i]='_';
			}
		}
		fprintf(f,"%s",high_name);
	}
	fclose(f);
}

// controlling function -------------------------------------------------------------------------------------------------------------------------

void iMouseMove(int mx, int my)
{}

void iMouse(int button, int state, int mx, int my)
{
    /*if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // demo debug assist
        point_x = point_x_2;
        point_x_2 = mx;
        point_y = point_y_2;
        point_y_2 = my;
    }*/
	if( LEVEL == MAIN_MENU )
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if( mx>=76 && mx<=198 && my>=336 && my<=375 )
			{
				load_sec = 1;
				LEVEL = LOAD_GAME;
			}
			else if( mx>=75 && mx<=312 && my>=285 && my<= 323 )
			{
				LEVEL = HIGH_SCORE;
			}
			else if( mx>=76 && mx<=197 && my>=232 && my<=269 )
			{
				LEVEL = HELP;
			}
			else if( mx>=76 && mx<=254 && my>=179 && my<= 217 )
			{
				LEVEL = CREDIT;
			}
			else if( mx>= 817 && mx<=928 && my>=71 && my<=109 )
			{
				// data writing
				write_data();

				exit(0);
			}
		}
	}
	else if( LEVEL == HIGH_SCORE )
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if( mx>=767 && mx<=888 && my>=53 && my<=92 )
			{
				LEVEL = MAIN_MENU;
			}
		}
	}
	else if( LEVEL == HELP )
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if( mx>=818 && mx<=956 && my>=135 && my<=174 )
			{
				LEVEL = MAIN_MENU;
			}
		}
	}
	else if( LEVEL == CREDIT )
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if( mx>=126 && mx<=264 && my>=44 && my<=84 )
			{
				LEVEL = MAIN_MENU;
			}
		}
	}
}	

void pause_all()
{
    game_pause = true;

    iPauseTimer( back_timer );
    iPauseTimer( fire_timer );
    iPauseTimer( sec_timer );
    iPauseTimer( enemy_timer );
}

void resume_all()
{
    game_pause = false;

    iResumeTimer( back_timer );
    iResumeTimer( fire_timer );
    iResumeTimer( sec_timer );
    iResumeTimer( enemy_timer );
}

void iKeyboard(unsigned char key)
{
    if( LEVEL == GAME_PLAY )
    {
        if( game_pause )
        {
            if( key == 'e' )
            {
                resume_all();
                return;
            }
            else if( key == 'r' )
            {
                resume_all();
				after_game();
            }
            else return;
        }

        if( key == 'e' ) pause_all();

        if( key == 'q' ) //fire
        {
            plane.fire = true;

            pmf.push_back( bullet( plane.x, plane.y+30+15 ) );

            if( plane.fireLevel )
            {
                plf.push_back( bullet( plane.x, plane.y+30+15 ) );
                prf.push_back( bullet( plane.x, plane.y+30+15 ) );
            }
        }
    }
	else if( LEVEL == NEW_HIGH_SCORE )
	{
		if(key=='\r')
		{
			strcpy( high_name, name );
			write_data();

			LEVEL = MAIN_MENU;
			return;
		}
	
		if( (key>='a'&&key<='z') || (key>='A'&&key<='Z') || (key>='0'&&key<='9') || key==' ' )
		{
			int len = strlen(name);
			if( len<30 )
			{
				name[ len ] = key;
				name[ ++len ] = '\0';
			}
		}
	}
}

void iSpecialKeyboard(unsigned char key)
{
    if( LEVEL == GAME_PLAY )
    {
        if( game_pause ) return;

        int amount = 15;
        if( key == GLUT_KEY_DOWN )
        {
            plane.y -= amount;
            if( plane.y < 0 ) plane.y=0;
        }
        else if(  key == GLUT_KEY_UP )
        {
            plane.y += amount;
            if( plane.y > GAME_Y ) plane.y=GAME_Y;
            plane.boost = true;
        }
        else if( key == GLUT_KEY_LEFT )
        {
            plane.x -= amount;
            if( plane.x < 0 ) plane.x = 0;
        }
        else if( key == GLUT_KEY_RIGHT )
        {
            plane.x += amount;
            if( plane.x > GAME_X ) plane.x = GAME_X;
        }
    }
	else if( LEVEL == NEW_HIGH_SCORE )
	{
		if(key == GLUT_KEY_LEFT)
		{
			int len = strlen(name);
			if( len )
			{
				name[ --len ] = '\0';
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------
void basic_enemy_add()
{
    int x = rand()%601 + 100;
    int y = 800;
    basics.push_back( basic( x, y ) );
}

void middle_enemy_add()
{
    int x = rand()%601 + 100;
    int y = 800;
    middles.push_back( middle( x, y ) );
}

void final_enemy_add()
{
    int x = rand()%601 + 100;
    int y = 800;
    finals.push_back( final( x, y ) );
}

void food_add(int ind)
{
    int x = rand()%601 + 100;
    int y = 800;
    foods.push_back( food(x,y,ind) );
}

void enemy_change()
{
    if( LEVEL == GAME_PLAY )
    {
        int i;
        // changing foods
        for( i = 0; i<foods.size(); )
        {
            foods[i].y -= 2;
            if( foods[i].y+100 < 0 )
            {
                foods.erase( foods.begin() + i );
            }
            else i++;
        }
        // changing the basic enemies
        for( i = 0; i<basics.size(); )
        {
            basics[i].y -= 2;
            if( basics[i].y+100 < 0 )
            {
                basics.erase( basics.begin() + i );
            }
            else i++;
        }
        // changing the middle enemies
        for( i = 0; i<middles.size(); )
        {
            //if( middles[i].y>=650 && middles[i].y<=653 )
            if( middles[i].y >= 600 && middles[i].y <= 603 )
            {
                ef.push_back( bullet( middles[i].x, middles[i].y ));
            }
            middles[i].y -= 3;
            if( middles[i].y+100 < 0 )
            {
                middles.erase( middles.begin() + i );
            }
            else i++;
        }
        // changing the final enemies
        for( i = 0; i<finals.size(); )
        {
            if( finals[i].x != plane.x )
            {
                finals[i].fire = true;
                if( plane.x > finals[i].x )
                {
                    if( plane.x - finals[i].x >=2 ) finals[i].x += 2;
                    else finals[i].x = plane.x;
                }
                else
                {
                    if( finals[i].x - plane.x >= 2 ) finals[i].x -= 2;
                    else finals[i].x = plane.x;
                }
            }
            else if( finals[i].fire )
            {
                ef.push_back( bullet(finals[i].x, finals[i].y) );
                finals[i].fire = false;
            }

            finals[i].y -= 4;
            if( finals[i].y+100 < 0 )
            {
                finals.erase( finals.begin() + i );
            }
            else i++;
        }
    }
}

void fireChange()
{
    if( LEVEL == GAME_PLAY )
    {
        int i;
        int amount = 5;

        //plane's fire change
        //left
        for( i=0; i<plf.size(); )
        {
            plf[i].x -= amount;
            plf[i].y += amount;
            if( plf[i].x<0 || plf[i].y>GAME_Y ) plf.erase( plf.begin()+i );
            else i++;
        }
        //middle
        for( i=0; i<pmf.size(); )
        {
            pmf[i].y += amount;
            if( pmf[i].y>GAME_Y ) pmf.erase( pmf.begin()+i );
            else i++;
        }
        //right
        for( i=0; i<prf.size(); )
        {
            prf[i].x += amount;
            prf[i].y += amount;
            if( prf[i].x>GAME_X || prf[i].y>GAME_Y ) prf.erase( prf.begin()+i );
            else i++;
        }
        // enemy fire
        for( i=0; i<ef.size(); )
        {
            ef[i].y -= amount;
            if( ef[i].y>GAME_Y ) ef.erase( ef.begin()+i );
            else i++;
        }
    }
}

void secTimer()
{
    if( LEVEL == GAME_PLAY )
    {
        if( sec < 250 ) sec++;
        else sec=1;

        // managing foods
        if( sec % 120 == 0 ) food_add(1);
        if( sec % 17 == 0 ) food_add(2);
        if( plane.fireLevel ) plane.fireLevel++;
        if( plane.fireLevel == 11 ) plane.fireLevel = 0;

        // adding basic enemies
        if( sec % 6 == 0 ) basic_enemy_add();
        // adding middle enemies
        if( sec % 5 == 0 && score >= 0 ) middle_enemy_add();
        // adding final enemies
        if( sec % 7 == 0 && score >= 0 ) final_enemy_add();

        // managing player down
        if( player_down )
        {
            player_down++;
            sec = 1;
        }
        if( player_down == 5 )
        {
            if( !life ) after_game() ;
            player_down = 0;
            sec = 1;
        }
    }
	else if( LEVEL == INTRODUCTION )
	{
		intro_sec++;
		if( intro_sec == 11 )
		{
			LEVEL = MAIN_MENU;
		}
	}
	else if( LEVEL == LOAD_GAME )
	{
		load_sec++;
		if( load_sec == 7 )
		{
			initializeGame();
			LEVEL = GAME_PLAY;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------

void initializeGame()
{
    sec = 1;
    plane.initialize();
    score = 0;
    clear_all();
    player_down = 0;
    life = 2;
    game_pause = false;

    // initialize background
    first_time_background = true;
    point_add_count = 0;
}

int main()
{
    // launching introduction
	intro_sec = 1;
	LEVEL = INTRODUCTION;

	// data reading initialization
	read_data();

    //initializeGame();

    back_timer = iSetTimer( 5, changeBackGround ); // setting the background
    fire_timer = iSetTimer( 3, fireChange ); // spreading bullet fires both from player's ship and enemy ship
    sec_timer = iSetTimer( 1000, secTimer ); // general purpose 1 second timer
    enemy_timer = iSetTimer( 6, enemy_change ); // advancing enemy positions

    iInitialize(MAIN_X, MAIN_Y, "Star Wars");
    return 0;
}
