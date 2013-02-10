// Polygon.cpp

#include "Polygon.h"

const double pi = 3.141592653589793;

Polygon::Polygon( double x, double y, int n, double A, int t, int id, vector< double > parentNormal )
{

	this->plot = Plot::CreateInstance();
	this->parentNormal = parentNormal;

	this->t = t;
	this->id = id;
	this->A = A;

	if( this->id > 2147483647 ) printf("Index %ld too large!\n", this->id);

	this->hidden = false;

	setCenter( x, y );
	setSides( n );

}

Polygon::~Polygon()
{

	this->hide();

}

void Polygon::setCenter( double x, double y )
{

	this->x = x;
	this->y = y;

}

vector< double > Polygon::getCenter()
{

	vector<double> position;
	position.resize(2);
	position[0] = this->x;
	position[1] = this->y;

	return position;

}

void Polygon::setSides( int n )
{

	this->n = n;

	char command[128];
	sprintf(command, "set object %ld polygon from \\", id);
	plot->write(command);

	this->vertices.resize(n);

	for( int i = 0; i < n; i++ )
	{

		this->vertices[i].resize(2);
		double phi = 2*pi/n;

		if( i == 0)
		{

			double x0 = 0;
			double y0 = this->getOuterRadius();

			double theta = phi/2;

			this->vertices[0][0] = x0*cos(theta) - y0*sin(theta);
			this->vertices[0][1] = x0*sin(theta) + y0*cos(theta);

		}
		else
		{

			double vx = this->vertices[i-1][0];
			double vy = this->vertices[i-1][1];

			this->vertices[i][0] = vx*cos(phi) - vy*sin(phi);
			this->vertices[i][1] = vx*sin(phi) + vy*cos(phi);

		}

	}

	if( this->id == 1 )
	{

		int index = 0;
		for( int i = 0; i < vertices.size(); i++ )
			if( vertices[i][0] < vertices[index][0] )
				index = i;

		double dx = plot->getXMin() - vertices[index][0];
		double dy = plot->getYMax() - vertices[0][1];

		this->setCenter(this->getCenter()[0]+dx,this->getCenter()[1]+dy);

	}

	for( int i = 0; i < n; i++ )
	{

		if( t % 2 ) vertices[i][1] = -this->vertices[i][1];
		else vertices[i][0] = -this->vertices[i][0];

		sprintf(command, "%f,%f to \\", this->x+this->vertices[i][0], this->y+this->vertices[i][1]);
		plot->write( command );

	}

	sprintf(command, "%f,%f\n", this->x+this->vertices[0][0], this->y+this->vertices[0][1]);
	plot->write( command );

	int red   = sin(2*pi*t/5 + 0.0/3.0*pi) * 127 + 128;
	int green = sin(2*pi*t/5 + 2.0/3.0*pi) * 127 + 128;
	int blue  = sin(2*pi*t/5 + 4.0/3.0*pi) * 127 + 128;

	sprintf(command, "set label %ld '%ld' at %f,%f center enhanced\n", this->id, this->id, this->x, this->y);
	plot->write( command );

	sprintf(command, "set object %ld fc rgb '#%02X%02X%02X' fillstyle solid border lc rgb '#000000'", this->id, red, green, blue);
	plot->write( command );

	setNormals();

}

void Polygon::setNormals()
{

	normals.resize(n);

	for( int i = 0; i < n; i++ )
	{

		normals[i].resize(2);

		normals[i][0] = this->vertices[i][0]+this->vertices[(i==n-1 ? 0 : i+1)][0];
		normals[i][1] = this->vertices[i][1]+this->vertices[(i==n-1 ? 0 : i+1)][1];

	}

	int index = 0;
	for( int i = 0; i < normals.size(); i++ )
		if( fabs(normals[i][0] + parentNormal[0]) < 0.000001 )
			if( fabs(normals[i][1] + parentNormal[1]) < 0.000001 )
				index = i;

	vector< vector< double > > temp;
	temp.resize(normals.size());
	for( int i = 0; i < normals.size(); i++ )
	{

		temp[i].resize(2);
		temp[i][0] = normals[(index+i > n-1)?index+i-n:index+i][0];
		temp[i][1] = normals[(index+i > n-1)?index+i-n:index+i][1];

	}
	normals = temp;

}

vector< double > Polygon::getNormal( int i )
{

	return this->normals[i];

}

vector< double > Polygon::getVertex( int i )
{

	return this->vertices[i];

}

double Polygon::getOuterRadius()
{

	return sqrt(2*A/(n*sin(2*pi/n)));

}

double Polygon::getInnerRadius()
{

	return sqrt(A/(n*tan(pi/n)));

}

bool Polygon::isHidden()
{

	return this->hidden;

}

void Polygon::hide()
{

	char command[128];
	sprintf(command,"unset object %ld\n", this->id);
	plot->write(command);
	sprintf(command,"unset label %ld\n", this->id);
	plot->write(command);
	this->hidden = true;

}

bool Polygon::collidesWith( Polygon* polygon )
{

	// Already hidden?
	if( polygon->isHidden() ) return false;

	vector<double> center1 = polygon->getCenter();
	vector<double> center2 = this->getCenter();

	double radius1 = polygon->getInnerRadius();
	double radius2 = this->getInnerRadius();

	// Blatantly off the plot?
	if( center2[0]-radius2 < plot->getXMin() - 0.000001 ) return true;
	if( center2[0]+radius2 > plot->getXMax() + 0.000001 ) return true;
	if( center2[1]-radius2 < plot->getYMin() - 0.000001 ) return true;
	if( center2[1]+radius2 > plot->getYMax() + 0.000001 ) return true;

	// Inner circles overlap?
	vector<double> displacement;
	displacement.resize(2);
	displacement[0] = center2[0] - center1[0];
	displacement[1] = center2[1] - center1[1];
	double distance = displacement[0]*displacement[0] + displacement[1]*displacement[1];
	bool collide = ((radius1 + radius2)*(radius1 + radius2) - distance) > 0.000001;
	if( collide ) return true;

	// Any vertex off the plot?
	for( int i = 0; i < vertices.size(); i++ )
	{

		if( center2[0]+vertices[i][0] < plot->getXMin() - 0.000001 ) return true;
		if( center2[0]+vertices[i][0] > plot->getXMax() + 0.000001 ) return true;
		if( center2[1]+vertices[i][1] < plot->getYMin() - 0.000001 ) return true;
		if( center2[1]+vertices[i][1] > plot->getYMax() + 0.000001 ) return true;

	}

	return false;			

}
