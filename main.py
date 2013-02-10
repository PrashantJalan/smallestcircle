#!/usr/bin/python

# Copyright (c) 2012 Prashant Jalan

# A program showing the implementation of finding the smallest
# circle enclosing given n points. The worst case time complexity
# is O(n^3) and average case is O(n). 
# It is assumed that in the input no two points are the same.

import sys
import numpy as np
import matplotlib.pyplot as plt
import math
import scipy.misc
import itertools
from sympy.geometry import Point
import random
import time

def check (test,circle):
	#test are the test points, circle contains the center and the radius of the circle
	
	x=test[0]
	y=test[1]
	h=circle[0]
	k=circle[1]
	r=circle[2]

	temp = (x-h)*(x-h) + (y-k)*(y-k) - r*r


	if temp>0.000001:	return 1	#Outside the circle
	elif temp<-0.000001:	return -1 	#On the circle
	else:	return 0	#Inside the circle


def make_plot (circle, point):
	
	temp1 = []
	temp2 = []
	
	for item in point:
		temp1.append(item[0])
		temp2.append(item[1])
	
	x = circle[0]
	y = circle[1]
	r = circle[2]
	
	fig = plt.figure()
	ax = fig.add_subplot(111,aspect='equal')  

	circ = plt.Circle((x,y),radius=r, fill=False)

	ax.plot()
	ax.add_patch(circ)
	
	plt.plot(temp1, temp2, 'o')
	
	plt.xlim([x-r-10,x+r+10])
	plt.ylim([y-r-10,y+r+10])
	plt.title('Graphical Representation')
	plt.xlabel('x-axis')
	plt.ylabel('y-axis')
	plt.savefig('graphical.png')
	plt.show()


def make_circle_2 (a, b):
	#The function returns a circle passing through points a and b
	
	d1 = -(a[0]*a[0]+a[1]*a[1])
	d2 = -(b[0]*b[0]+b[1]*b[1])
	d3 = b[1]*(b[0]-a[0])-b[0]*(b[1]-a[1])

	a = np.array([[2*a[0],2*a[1],1], [2*b[0],2*b[1],1], [b[1]-a[1],a[0]-b[0],0]])
	b = np.array([d1,d2,d3])
	x = np.linalg.solve(a, b)
	
	radius = math.sqrt(x[0]*x[0] + x[1]*x[1] -x[2]) 
	
	return ([-x[0],-x[1], radius])

def make_circle_3 (a,b,c):
	#The function returns a circle passing through points a,b,c
	
	d1 = -(a[0]*a[0]+a[1]*a[1])
	d2 = -(b[0]*b[0]+b[1]*b[1])
	d3 = -(c[0]*c[0]+c[1]*c[1])
	
	a = np.array([[2*a[0],2*a[1],1], [2*b[0],2*b[1],1], [2*c[0],2*c[1],1]])
	b = np.array([d1,d2,d3])
	x = np.linalg.solve(a, b)
	
	radius = math.sqrt(x[0]*x[0] + x[1]*x[1] -x[2]) 
	
	return ([-x[0],-x[1], radius])

 
def true_circle(circle, point):
	#True circle is a circle which encloses all the given points
	
	flag=0
	for i in point:
		if check(i,circle)==1:
			flag=1

	if flag==1:		return 0
	else:	return 1	#The circle is true



def find_angle (x,y,z):
	#Returns the angle of point x
	a = math.hypot(y[1]-z[1],y[0]-z[0])
	b = math.hypot(y[1]-x[1],y[0]-x[0])
	c = math.hypot(x[1]-z[1],x[0]-z[0])
	
	temp = (b*b+c*c-a*a)/(2*b*c)
	ans = math.acos(round(temp, 15))
	return math.degrees(ans)


def opp_side(a,b,x,y):
	x1=x[0]
	x2=y[0]
	y1=x[1]
	y2=y[1]
	
	temp1 = (a[1]-y1)*(x2-x1)-(y2-y1)*(a[0]-x1)
	temp2 = (b[1]-y1)*(x2-x1)-(y2-y1)*(b[0]-x1)


	if temp1*temp2<=0:
		return True
	else:
		return False


def true_circle2(circle, point,end):
	#True circle is a circle which encloses all the given points
	
	flag=0
	i=0
	while i<end:
		if check(point[i],circle)==1:
			flag=1
		i=i+1

	if flag==1:		return 0
	else:	return 1	#The circle is true


 
def find_circle(a, b, point, end):
	#Find a circle given points a and b such that it encloses all
	#points in point till i. Return -1 if not possible. Takes O(n) time.
	
	#Checking possibility

	angle=[]
	new_point=[]
	i=0
	while i<end:
		if point[i]!=b and point[i]!=a:
			temp_angle = find_angle (point[i], a, b)
			if temp_angle==0:
				return False
			angle.append(temp_angle)
			new_point.append(point[i])
		i=i+1 
	
			
	#Finding minimum and maximum angle
	min_angle1 = angle[0]
	min_point1 = new_point[0]

	
	i=0
	for item in angle:
		if min_angle1>item:
			min_angle1 = item
			min_point1 = new_point[i]
		i=i+1

	min_angle2 = 180
	min_point2 = min_point1		
	
	#If all points on the other side has angle >= 180-min then possible
	i=0
	flag=0
	while i<len(angle):
		if opp_side(new_point[i],min_point1,a,b) == True:
			if min_angle2 > angle[i]:
				min_angle2 = angle[i]
				min_point2 = new_point[i]
		i=i+1
	
	if (min_angle1 + min_angle2) < 180:
		return False

	
	#Finding the circle
	circl1 = make_circle_3(a, b, min_point1)
	circl2 = make_circle_3(a, b, min_point2)
	
	if min_angle1 >= 90:
		return make_circle_2(a, b)
	elif circl1[2]<=circl2[2]:
		return circl1
	else:
		return circl2

		
		
 
def optimize_algo(point):
	
	answer = make_circle_3(point[0], point[1], point[2])	
	i=3
	while i<len(point):
		if check(point[i], answer)==1:
			#If point is not already inside/on the circle
			j=0
			circle = [0,0,float("inf")]
			while j<i:
				if point[j]!=point[i]:
					#Now we have two points point[i] and point[j]
					temp = find_circle(point[i], point[j],point,i)
					if temp != False:
						#That is it is possible to get a circle
						if circle[2]>temp[2]:
							circle = temp		
				j=j+1
			answer = circle
			del(circle)
		i=i+1
	
	return answer
 
 
 
def trivial_algo(point):

	#All possible combinations taken 3 at a time
	
	data = list(itertools.combinations(point,3))	
	data_circle=[]
	
	for item in data:
		if Point.is_collinear(Point(item[0]), Point(item[1]), Point(item[2])) == False:
			temp_circle = make_circle_3(item[0],item[1],item[2])
			if true_circle(temp_circle, point)==1:
				data_circle.append(temp_circle)

	#All possible combinations taken 2 at a time
	
	del(data)
	data = list(itertools.combinations(point,2))	
	
	for item in data:
		temp_circle = make_circle_2(item[0],item[1])
		if true_circle(temp_circle, point)==1:
			data_circle.append(temp_circle)
	
	minimum = data_circle[0][2]
	for item in data_circle:
		if minimum > item[2]:
			minimum = item[2]
			
	for item in data_circle:
		if minimum == item[2]:
			answer = item
			break;
	
	return answer
 

 
 
 
def main(argv = None):
	# Getting the given n points
	T = raw_input('Enter the no. of points for which you would like to test the program:\n')	#Test cases
	T = int(T)
	
	#Dealing base case
	if T<2:
		print "Please enter 2 or more than 2 points"
		return 0
	
	temp_point = random.sample(range(1000), 2*T)

	i=0
	point=[]
	while i<len(temp_point):
		point.append([temp_point[i],temp_point[i+1]])
		i=i+2
		
	f = open('data.txt', 'w')
	for item in point:
		f.write(str(item))
		f.write("\n")
	f.close()

	#Processing		
	start_time = time.time()
	answer1 = trivial_algo(point)
	exec_time1 = time.time() - start_time	#execution time

	start_time = time.time()
	answer2 = optimize_algo(point)
	exec_time2 = time.time() - start_time	#execution time
		
		
		
	#Printing the answer
	if round(answer1[0],5)!=round(answer2[0],5) or round(answer1[1],5)!=round(answer2[1],5) or round(answer1[2],5)!=round(answer2[2],5):		#Random Error check
		print 'Some error occured!'
		print 'Answers of both the algorithm doesn\'t match'
		print 'Answer of trivial algorithm - ' + str(answer1)
		print 'Answer of optimised algorithm - ' + str(answer2)
		print 'Answer might sometime also differ due to precision error'
		make_plot(answer1,point)
		make_plot(answer2,point)
		return 0
		
	print 'Program ran successfully'
	print 'Circle - ' + str(answer1)	
	print 'Execution time for trivial algorithm: ' + str(exec_time1) + ' seconds'
	print 'Execution time for optimised algorithm: ' + str(exec_time2) + ' seconds'
	make_plot(answer1,point)
	del(point)

	return 0
	
	
	

if __name__ == "__main__":
	main()


