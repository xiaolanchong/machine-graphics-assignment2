#include "graphics.h"
#include "conio.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "dos.h"

const int 	Xc = 320,
			Yc = 240,//koordinate centr

			a = 100,
			b = 70,//radiusi ellipsa x y

			n = 19,//amount koordinats
			masch = 12;//maschtab

const double pi = 3.14159;

typedef double mas_kor[n];
const mas_kor 	X = {-4,5.5,5.5,4,-2.5,-3,-5, -2,0,2,4, 0,0.5, 2,2.5, -0.5,-1,0.5,2},
				Y = {0, 0,1,2,   2, 4, 4,  1,1,1,1, 0, -4,-4,  0,    2, 5.5, 5.5,2};

void DRAW(int c,int alpha)
{
	int i;
	int koef;
	mas_kor X1,Y1;
	int point0[2];
	double x = a*cos(alpha*pi/180);
	double y = -b*sin(alpha*pi/180);
	point0[0] = Xc + x;
	point0[1] = Yc + y;

	int ost = (alpha%360)*180;

	if(ost >= 180)
		koef = 1;
	else
		koef = -1;

	int clr = getcolor();
	setcolor(c);

	double k;
	if (ost == 0)
		k = pi/2;
	else
	{
		if (ost == 90)
			k = -3*pi/4;
		else
		{
			if(koef == 1)
				k = pi/2 - (pi/2 - atan(koef*b*x/(a*a*sqrt(1-x*x/a/a))));
			else
				k = -pi/2 - (pi/2 - atan(koef*b*x/(a*a*sqrt(1-x*x/a/a))));
		}
	}

	X1[0] = X[0]*masch*cos(k) + Y[0]*masch*sin(k) + point0[0];
	Y1[0] = -(Y[0]*masch*cos(k) - X[0]*masch*sin(k)) + point0[1];
	for(i = 0;i < n-1;i++)
	{
		X1[i+1] = X[i+1]*masch*cos(k) + Y[i+1]*masch*sin(k) + point0[0];
		Y1[i+1] = -(Y[i+1]*masch*cos(k) - X[i+1]*masch*sin(k)) + point0[1];
		if(i < 6)
			line( X1[i],Y1[i],X1[i+1],Y1[i+1]);
		if((i > 6)&&(i < 11))
			circle(X1[i],Y1[i],0.5*masch);
		if((i >= 11)&&(i < 14))
			line( X1[i],Y1[i],X1[i+1],Y1[i+1]);
		if((i > 14)&&(i < 19))
			line( X1[i],Y1[i],X1[i+1],Y1[i+1]);
	}
	line( X1[6],Y1[6],X1[0],Y1[0]);

	setcolor(clr);

}
int main(void)
{
	/* request auto detection */
	int gdriver = DETECT, gmode, errorcode;

	/* initialize graphics mode */
	initgraph(&gdriver, &gmode, "D:\\BORLANDC\\BGI");

	/* read result of initialization */
	errorcode = graphresult();

	if (errorcode != grOk)  /* an error occurred */
	{
	  printf("Graphics error: %s\n", grapherrormsg(errorcode));
	  printf("Press any key to halt:");
	  getch();
	  exit(1);             /* return with error code */
	}

	int beta = 500;
    setcolor(2);
	while (beta != 0)
	{
		DRAW(getcolor(), beta);
		delay(50);
		DRAW(getbkcolor(), beta);
		beta -= 2;
	}

//	getch();
	closegraph();

	return 0;
}