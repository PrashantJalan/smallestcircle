/* This program finds the smallest circle enclosing given n points
 * in a plane. It implements it using the both the trivial
 * and the optimised algorithm.
 * 
 * Terminology - 
 * True circle - A circle which encloses all the given n points.
 * 
 * Author - 
 * Prashant Jalan
 * */
 
 
#include <LEDA/geo/circle.h>
#include <LEDA/geo/random_point.h>
#include <LEDA/graphics/window.h>
#include <LEDA/core/list.h>
#include <iostream>
#include <limits>
#include <LEDA/system/stream.h>
#include <LEDA/system/timer.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define LIMIT 10000		//Limit/range of the point coordinates

using namespace std;
using namespace leda;


list<point> L;		//List of points
double min_angle1, min_angle2;	//The min angles subtended
point min_p1;	//The point subtending the min angle
point min_p2;
point zero;
double pi = M_PI;
int flag1, flag2;
double inf = numeric_limits<double>::infinity();
point p_temp(0,0);
circle inf_C(p_temp , inf);


void draw (circle C)
{
	point cntr;
	cntr = C.center();
	double x = cntr.xcoord();
	double y = cntr.ycoord();
	double r = C.radius();
	
	window W("Graphical Representation");
	W.init(x-r-LIMIT, x+r+LIMIT, y-r-LIMIT);
	W.open(); 
	W.set_node_width(2); 
	W.set_line_width(2);

	point p;
	forall(p,L) W.draw_filled_node(p.to_point(), blue);
	
	W.draw_circle(C,orange);
	W.screenshot("Graphical Representation");
	W.display();
	W.read_mouse();
}	     

void draw1 (circle C)
{
	point cntr;
	cntr = C.center();
	double x = cntr.xcoord();
	double y = cntr.ycoord();
	double r = C.radius();
	
	window W("Graphical Representation");
	W.init(x-r-LIMIT, x+r+LIMIT, y-r-LIMIT);
	W.open(); 
	W.set_node_width(2); 
	W.set_line_width(2);

	point p;
	forall(p,L) W.draw_filled_node(p.to_point(), blue);
	
	W.draw_circle(C,orange);
	W.screenshot("Optimised");
}	     


void draw2 (circle C)
{
	point cntr;
	cntr = C.center();
	double x = cntr.xcoord();
	double y = cntr.ycoord();
	double r = C.radius();
	
	window W("Graphical Representation");
	W.init(x-r-LIMIT, x+r+LIMIT, y-r-LIMIT);
	W.open(); 
	W.set_node_width(2); 
	W.set_line_width(2);

	point p;
	forall(p,L) W.draw_filled_node(p.to_point(), blue);
	
	W.draw_circle(C,orange);
	W.screenshot("Trivial");
}	     


int possible_circle (point p1, point p2, int end)
{
	min_angle1 = pi;
	min_angle2 = pi;
	int i;
	double ang;
	list_item item;
	point p3;
	flag1 = 0; flag2 = 0;
	
	item = L.first();
	for (i=0; i<=end; i++)
	{
		p3 = L.contents(item);

		if (p3!=p1 && p3!=p2)	//The two points musn't be same
		{	
			ang = p3.angle(p1,p2);
			if (ang>pi)
			{
				ang = 2*pi-ang;
			}
			if (p3.orientation(p1, p2) == 0)		//Collinear points
			{
				if (ang == 0)
				{
					return 0;
				}
			}
			else if (p3.orientation(p1, p2) == 1)
			{
				if (ang<min_angle1)
				{
					min_angle1 = ang;
					min_p1 = p3;
					flag1 = 1;	//Denotes that min_p1 changed
				}
			}
			else
			{
				if (ang<min_angle2)
				{
					min_angle2 = ang;
					min_p2 = p3;
					flag2 = 1;	//Denotes that min_p2 changed
				}
			}
		}
		item = L.succ(item);
	}
	
	if ((min_angle1+min_angle2)>=pi)
	{
		return 1;
	}
	else
	{ 
		return 0;
	}
}


circle find_circle (point p1, point p2, int end)
{
	if (flag1 == 1 && flag2 == 1)
	{
		if (min_angle1 >= 90 && min_angle2>=90)
		{
			point cen;
			cen = midpoint(p1, p2);
			circle C(cen, p1);
			return C; 
		}
		else
		{
			circle C1 (p1, p2, min_p1);
			circle C2 (p1, p2, min_p2);
			if (C1.radius() < C2.radius())
				return C1;
			else
				return C2;
		}
	}
	else if (flag1 == 1 && flag2 == 0)
	{
		circle C(p1, p2, min_p1);
		return C;
	}
	else if (flag1 == 0 && flag2 == 1)
	{
		circle C(p1, p2, min_p2);
		return C;
	}
	else
	{
		cout << "There are some ghosts!!" << endl;
	}
	
}


circle optimise() 
{	
	int i,j;		//To run loops, ofcourse!
	int N=L.length();			//No. of points
	circle C;
	circle min_C;
	circle tmp_C;
	list_item item1;
	list_item item2;
			
	//Processing
	
	//Iniatising the initial circle
	point cen;
	cen = midpoint(L.contents(L.get_item(0)), L.contents(L.get_item(1)));
	circle C_temp(cen, L.contents(L.get_item(0)));
	C = C_temp;

	
	//Now we aim to incorporate the other points into the circle or make it bigger
	i=2;
	item1 = L.get_item(2);
	while (i<N)
	{
		if (C.outside(L.contents(item1)))
		{
			//So, we now need a new circle
			//item1 gives one point through which circle will pass
			j=0;
			item2 = L.first();
			min_C = inf_C;
			while (j<i)
			{
				if (L.contents(item1)!=L.contents(item2))
				{
					//Now, we have two points, item1 & item2
					if (possible_circle(L.contents(item1), L.contents(item2), i)==1)
					{
						tmp_C = find_circle(L.contents(item1), L.contents(item2), i);
						if (tmp_C.radius()<min_C.radius())
						{
							min_C = tmp_C;
						}
					} 
				}
				j=j+1;
				item2 = L.succ(item2);
			}
			C = min_C;
		}
		item1 = L.succ(item1);
		i=i+1;
	}
	return C;
}




int true_circle (circle C)
{
	int i;
	int flag = 1;
	list_item item;
	
	item = L.first();
	for (i=0;i<L.length();i++)
	{
		if (C.outside(L.contents(item)))
		{
			flag = 0;
			break;
		}
		item = L.succ(item);
	}
	
	return flag;	
}


circle func3 (point p1, point p2, point p3, circle min_C)
{
	/* This function takes three points and find a circle in O(1) time.
	 * If the points however are collinear then it rejects it. If not,
	 * then it checks if it is true or not in O(n) time. If true it 
	 * compares its radius with the minimum circle and then returns the 
	 * one having the minimum radius */

	circle C(p1,p2,p3);
	
	bool check = C.is_degenerate();		//Checking collinearity
	if (check==1)
		return min_C;

	if (true_circle(C)==0)		//Checking if a true circle
		return min_C;
		
	if (C.radius() < min_C.radius())
		return C;
	else
		return min_C;
}

circle func2 (point p1, point p2, circle min_C)
{
	/* This function takes two points and find a circle in O(1) time.
	 * If the points however are collinear then it rejects it. If not,
	 * then it checks if it is true or not in O(n) time. If true it 
	 * compares its radius with the minimum circle and then returns the 
	 * one having the minimum radius */
	 
	point cen;
	cen = midpoint(p1,p2);
	
	circle C(cen,p1);
	
	if (true_circle(C)==0)		//Checking if a true circle
		return min_C;
	
	if (C.radius() < min_C.radius())
		return C;
	else
		return min_C;
}

circle trivial()
{
	int i,j,k;		//To run loops, ofcourse!
	int N=L.length();			//No. of points
	circle min_C;
	min_C = inf_C;
	list_item item1;
	list_item item2;
	list_item item3;

	//Processing
	
	//Taking combinations three at a time
	item1 = L.first();
	for(i=0;i<N;i++)
	{
		item2 = L.succ(item1);
		for(j=i+1;j<N;j++)
		{
			item3 = L.succ(item2);
			for(k=j+1;k<N;k++)
			{
				min_C = func3(L.contents(item1), L.contents(item2), L.contents(item3), min_C);
				item3 = L.succ(item3);
			}
			item2 = L.succ(item2);
		}
		item1 = L.succ(item1);
	}
	
	//Taking combinations two at a time
	item1 = L.first();
	for(i=0;i<N;i++)
	{
		item2 = L.succ(item1);
		for(j=i+1;j<N;j++)
		{
			min_C = func2(L.contents(item1), L.contents(item2), min_C);
			item2 = L.succ(item2);
		}
		item1 = L.succ(item1);
	}
	
	return min_C;
}


int main() {

	int N;			//No. of points
	circle ans1;
	circle ans2;
	timer f_timer1;
	timer f_timer2;

	cout << "Enter the no. of points for which you want to test the program:-" << endl;
	cin >> N;
	
	if (N<2)	//Error check
	{
		cout << "Please enter a number greater than 2" << endl;
		return 0;
	}
	
	//Generating a random list of N point between [-LIMIT, LIMIT]
	random_points_in_square(N, LIMIT , L);	
  
	//Storing in the file for reference
	std::ofstream myfile;
	myfile.open ("data.txt");	
	L.print(myfile);
	myfile.close();
	
	
	//Processing through optimised
	f_timer1.start();	//Started timer
	ans1 = optimise();
	f_timer1.stop();		//Timer stopped

	f_timer2.start();	//Started timer
	ans2 = trivial();
	f_timer2.stop();		//Timer stopped
	
		
	//Random error check
	if (ans1.radius()!=ans2.radius())
	{
		cout << "Some error occured" << endl;
		cout << "Through optimised code:" << endl;
		cout << "Center of circle: " << ans1.center() << endl;
		cout << "Radius of circle: " << ans1.radius() << endl;
		draw1(ans1);
		cout << "Execution time for optimised: " << f_timer1.elapsed_time() << "seconds" << endl;
		
		cout << "Through trivial code:" << endl;
		cout << "Center of circle: " << ans2.center() << endl;
		cout << "Radius of circle: " << ans2.radius() << endl;
		cout << "Execution time for trivial: " << f_timer2.elapsed_time() << "seconds" << endl;
		draw2(ans2);
	}
	else
	{
		cout << "Program ran successfully!" << endl;
		cout << "Center of circle: " << ans1.center() << endl;
		cout << "Radius of circle: " << ans1.radius() << endl;
		cout << "Execution time for optimised: " << f_timer1.elapsed_time() << "seconds" << endl;
		cout << "Execution time for trivial: " << f_timer2.elapsed_time() << "seconds" << endl;
		draw(ans1);
	}
	
	return 0;
}
