// Polygons.cpp

#include "Polygons.h"

Polygons::Polygons( int n, double A )
{

	this->A = A;

	setSides( n );

	plot = Plot::CreateInstance();

	polygons.resize(1);
	polygons[0].resize(1);

	char command[128];
	polygons[0][0] = new Polygon(0,0,n,A,0,1, vector<double>(2,0) );
	sprintf(command, "set title \"%d Polygons (%d sides, %d tiers, %.2f (%.2fx%.2f) area, %.2f%% covered)\"", this->getNumber(), n, 1, plot->getArea(), plot->getXMax()-plot->getXMin(), plot->getYMax()-plot->getYMin(), 100*this->getArea()/plot->getArea());
	plot->write( command );
	plot->refresh();

	int i = 1;
	int index = 1;
	int added = 1;
	while( added )
	{

		polygons.resize(polygons.size()+1);

		added = 0;
		int maxsize = polygons[i-1].size();
		for( int j = 0; j < maxsize; j++ )
		{

			Polygon* parent = getPolygon(i-1,j);

			for( int k = 0; k < n; k++ )
			{

				if( parent == NULL ) continue;
				if( parent->isHidden() ) continue;

				index++;

				vector<double> center = parent->getCenter();
				vector<double> normal = parent->getNormal(k);
				vector<double> vertex = parent->getVertex(k);

				Polygon* child = new Polygon( center[0] + normal[0], center[1] + normal[1], n, A, i, index, normal );

				bool collides = false;
				for( int u = 0; u < polygons.size(); u++ )
				{

					for( int v = 0; v < polygons[u].size(); v++ )
					{

						Polygon* collide = getPolygon(u,v);
						if( collide == NULL ) continue;

						if( child->collidesWith(collide) ) collides = true;
						
						if( collides ) break;

					}
					if( collides ) break;

				}

				if( collides )
				{

					index--;
					delete child;

				}
				else
				{

					polygons[i].push_back( child );
					added++;

				}

			}

		}

		sprintf(command, "set title \"%d Polygons (%d sides, %d tiers, %.2f (%.2fx%.2f) area, %.2f%% covered)\"", this->getNumber(), n, (added?i+1:i), plot->getArea(), plot->getXMax()-plot->getXMin(), plot->getYMax()-plot->getYMin(), 100*this->getArea()/plot->getArea());
		plot->write( command );
		plot->refresh();

		i++;

	}

}

Polygons::~Polygons()
{

	plot->write("unset arrow");
	plot->write("unset label");
	plot->write("unset object");
	plot->refresh();

	for( int i = 0; i < polygons.size(); i++ )
		for( int j = 0; j < polygons[i].size(); i++ )
			delete getPolygon(i,j);

}

int Polygons::getNumber()
{

	int count = 0;

	for( int i = 0; i < polygons.size(); i++ )
		for( int j = 0; j < polygons[i].size(); j++ )
		{

			Polygon* polygon = getPolygon(i,j);
			if( polygon == NULL ) continue;

			if( !polygon->isHidden() ) count++;

		}

	return count;

}

double Polygons::getArea()
{

	double area = 0;

	for( int i = 0; i < polygons.size(); i++ )
		for( int j = 0; j < polygons[i].size(); j++ )
		{

			Polygon* polygon = getPolygon(i,j);
			if( polygon == NULL ) continue;

			if( !polygon->isHidden() )
			{
				double R = polygon->getOuterRadius();
				area += 0.5*n*R*R*sin(2*pi/n);

			}

		}

	return area;

}

Polygon* Polygons::getPolygon( int x, int y )
{

	if( x > polygons.size() - 1 || y > polygons[x].size() - 1 )
		return NULL;

	return polygons[x][y];

}

void Polygons::setSides( int n )
{

	this->n = n;

}
