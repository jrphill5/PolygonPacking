// Polygon.h

#ifndef TREE_H
#define TREE_H

#include "Plot.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class Polygon
{

	private:
		int t, n;
		unsigned long int id;
		double x, y, A;
		bool hidden;
		Plot* plot;
		vector< vector< double > > vertices;
		vector< vector< double > > normals;
		vector< double > parentNormal;

	public:
		Polygon( double x, double y, int n, double A, int t, int id, vector< double > parentNormal );
		~Polygon();
		void setCenter( double x, double y );
		void setSides( int n );
		void setNormals();
		vector< double > getCenter();
		vector< double > getNormal( int i );
		vector< double > getVertex( int i );
		double getOuterRadius();
		double getInnerRadius();
		bool collidesWith( Polygon* polygon );
		bool isHidden();
		void hide();

};

#endif
