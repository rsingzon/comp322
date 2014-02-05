/**
 * Singzon, Ryan
 * 260397455
 * 
 * COMP 322 - Introduction to C++
 * Winter 2014
 */

#ifndef EDGE_H
#define EDGE_H

struct Edge{
	int startVertex;
 	int endVertex;

 	int weight;
 	Edge* nextEdge;	
} ; 

#endif