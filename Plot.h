// Plot.h

#ifndef PLOT_H
#define PLOT_H

#include <fstream>
#include <string>

using namespace std;

class Plot
{

	private:
		Plot();
		Plot(Plot const&){};
		Plot& operator=(Plot const&){};
		static Plot* pInstance;
		FILE* gnuplot;
		double xmin, xmax, ymin, ymax;

	public:
		~Plot();
		static Plot* CreateInstance();
		double getArea();
		void setArea();
		double getXMin();
		double getXMax();
		double getYMin();
		double getYMax();
		void setXMin(double xmin);
		void setXMax(double xmax);
		void setYMin(double ymin);
		void setYMax(double ymax);
		void write( const char* command );
		void refresh();

};

#endif
