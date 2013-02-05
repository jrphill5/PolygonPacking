// Plot.cpp

#include "Plot.h"
#include <stdio.h>

Plot* Plot::pInstance = NULL;

Plot* Plot::CreateInstance()
{

	if (!pInstance) pInstance = new Plot();
	return pInstance;

}

Plot::Plot()
{

	this->xmax = 3;
	this->ymax = 3;

	this->xmin = -this->xmax;
	this->ymin = -this->ymax;

	gnuplot = popen("gnuplot -geometry 600x600 - > /dev/null 2>&1","w");
	//write("set term postscript eps color");
	setArea();
	write("unset key");
	write("set format ''");
	write("f(x) = -50");
	write("plot f(x)");

}

Plot::~Plot()
{

	write("set terminal x11 close 0");
	write("exit");
	pclose(gnuplot);

}

double Plot::getXMin() { return this->xmin; }
double Plot::getXMax() { return this->xmax; }
double Plot::getYMin() { return this->ymin; }
double Plot::getYMax() { return this->ymax; }

void Plot::setXMin(double xmin) { this->xmin = xmin; this->setArea(); }
void Plot::setXMax(double xmax) { this->xmax = xmax; this->setArea(); }
void Plot::setYMin(double ymin) { this->ymin = ymin; this->setArea(); }
void Plot::setYMax(double ymax) { this->ymax = ymax; this->setArea(); }

double Plot::getArea() { return (xmax-xmin)*(ymax-ymin); }
void Plot::setArea()
{ 

	char command[128];
	sprintf(command, "set xrange[%f:%f]", this->xmin, this->xmax);
	write(command);
	sprintf(command, "set yrange[%f:%f]", this->ymin, this->ymax);
	write(command);
	write("set size square");
	write("set size ratio -1");
	write("set ticslevel 0");
	write("set xtics autofreq 1");
	write("set ytics autofreq 1");

}

void Plot::write( const char* command )
{

	fprintf(gnuplot, "%s\n", command);
	fflush(gnuplot);

}

void Plot::refresh()
{

	write("replot");

}
