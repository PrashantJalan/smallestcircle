/* This program finds the smallest circle enclosing given n points
 * in a plane. It implements it using the trivial/brute force approach.
 * 
 * Terminology - 
 * True circle - A circle which encloses all the given n points.
 * 
 * Author - 
 * Prashant Jalan
 * */

#include <LEDA/geo/rat_circle.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/numbers/rational.h>
#include <LEDA/geo/random_rat_point.h>
#include <LEDA/core/list.h>
#include <LEDA/graphics/window.h>
#include <iostream>
#include <limits>
#include <fstream>
#include <LEDA/system/timer.h>
#define LIMIT 10000		//Limit/range of the point coordinates

using namespace std;
using namespace leda;

list<rat_point> L;		//List of points


void draw(rat_circle C)
{
	rat_point cntr;
	cntr = C.center();
	double x = cntr.xcoordD();
	double y = cntr.ycoordD();
	double r = sqrt(C.sqr_radius().to_float());
	rat_point p;
	double a,b;
	
	window W("Graphical Representation");
	W.init(x-r-LIMIT, x+r+LIMIT, y-r-LIMIT);
	W.open(); 
	W.set_node_width(2); 
	W.set_line_width(2);

	list_item item;
	item = L.first();
	for(int i=0;i<L.length();i++)
	{
		p = L.contents(item);
		a = cntr.xcoordD();
		b = cntr.ycoordD();
		W.draw_filled_node(a,b, blue);
		item = L.succ(item);
	}
	
	W.draw_circle(x,y,r,orange);
	W.display();
	W.read_mouse();
}	     


int true_circle (rat_circle C)
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

rat_circle func3 (rat_point p1, rat_point p2, rat_point p3, rat_circle min_C)
{
	/* This function takes three points and find a circle in O(1) time.
	 * If the points however are collinear then it rejects it. If not,
	 * then it checks if it is true or not in O(n) time. If true it 
	 * compares its radius with the minimum circle and then returns the 
	 * one having the minimum radius */
	 
/*	cout << "Reaching" << endl;
	cout << p1 << endl;
	cout << p2 << endl;
	cout << p3 << endl;
*/
	rat_circle C(p1,p2,p3);
	
	bool check = C.is_degenerate();		//Checking collinearity

	if (check==1)
		return min_C;

	if (true_circle(C)==0)		//Checking if a true circle
		return min_C;
		
	if (C.sqr_radius() < min_C.sqr_radius())
		return C;
	else
		return min_C;
}

rat_circle func2 (rat_point p1, rat_point p2, rat_circle min_C)
{
	/* This function takes two points and find a circle in O(1) time.
	 * If the points however are collinear then it rejects it. If not,
	 * then it checks if it is true or not in O(n) time. If true it 
	 * compares its radius with the minimum circle and then returns the 
	 * one having the minimum radius */
	 
	rat_point cen;
	cen = midpoint(p1,p2);
	
/*	cout << "Reaching" << endl;
	cout << p1 << endl;
	cout << p2 << endl;
*/
	rat_circle C(cen,p1);
	
	if (true_circle(C)==0)		//Checking if a true circle
		return min_C;
	
	if (C.sqr_radius() < min_C.sqr_radius())
		return C;
	else
		return min_C;
}

int main() {

	int i,j,k;		//To run loops, ofcourse!
	int N;			//No. of points
	list_item item1;
	list_item item2;
	list_item item3;
	timer f_timer;

	cout << "Enter the no. of points for which you want to test the program:-" << endl;
	cin >> N;
	
	if (N<=2)	//Error check
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
	
	
	//Processing
	f_timer.start();	//Started timer
	
	rat_circle min_C(L.contents(L.get_item(0)),L.contents(L.get_item(1)),L.contents(L.get_item(2)));
	
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
	
	f_timer.stop();		//Timer stopped
	
	//Random error check
	if (!true_circle(min_C))
	{
		cout << "Some error occured" << endl;
		cout << "Radius of circle: " << sqrt(min_C.sqr_radius().to_float()) << endl;
		draw(min_C);
	}
	else
	{
		cout << "Program ran successfully!" << endl;
		cout << "Radius of circle: " << sqrt(min_C.sqr_radius().to_float()) << endl;
		cout << "Execution time: " << f_timer.elapsed_time() << "seconds" << endl;
		draw(min_C);
	}
	
	return 0;
}
