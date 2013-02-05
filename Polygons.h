// Polygons.h

#ifndef POLYGONS_H
#define POLYGONS_H

#include "Plot.h"
#include "Polygon.h"
#include <vector>

using namespace std;

const double pi = 3.141592653589793;

class Polygons
{

	private:
		int t, n;
		double A;
		vector< vector< Polygon* > > polygons;
		Plot* plot;

	public:
		Polygons( int n, double A );
		~Polygons();
		Polygon* getPolygon( int x, int y );
		int getNumber();
		double getArea();
		void setTiers( int t );
		void setSides( int n );

};

#endif
