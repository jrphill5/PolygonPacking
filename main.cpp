// main.cpp

#include "Plot.h"
#include "Polygons.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

double x,y,p=0.5;
int tiers = 1;
int sides = 4;
double area = 0.75*0.75;
double height = 1;
void createModel();

Plot* plot;
Polygons* polygons;
vector< vector< double > > results;
double minimum, limit1, limit2;
int index, imax;

int main()
{

	plot  = Plot::CreateInstance();
	srand ( (unsigned)time(NULL) );
	createModel();

	while ( true )
	{

		int select = -1;

		printf("Make a selection:\n");
		printf(" [ 0] Exit Simulation\n");
		printf(" [ 1] Change Value: Number of Sides [%d]\n", sides);
		printf(" [ 2] Change Value: Dimensions      [%.2fx%.2f]\n", plot->getXMax()-plot->getXMin(),plot->getYMax()-plot->getYMin());
		printf(" [ 3] Display Statistics\n");
		printf(" [ 4] Optimize Shape\n");
		printf(" [ 5] Display Coverage\n");
		printf(" [99] Take Screenshot\n");

		printf(">> ");
		scanf("%d",&select);

		int value;
		switch ( select )
		{

			case 1:
				printf("Number of Sides: ");
				scanf("%d",&sides);
				if( sides <  3 ) sides =  3;
				createModel();
				break;

			case 2:
				printf("Width (x): ");
				scanf("%lf",&x);
				printf("Depth (y): ");
				scanf("%lf",&y);
				plot->setXMin(-fabs(0.5*x));
				plot->setXMax( fabs(0.5*x));
				plot->setYMin(-fabs(0.5*y));
				plot->setYMax( fabs(0.5*y));
				createModel();
				break;

			case 3:
				printf("Number of Pans:        %d\n", polygons->getNumber());
				printf("Planar Area of Pans:   %f\n", polygons->getArea());
				printf("Surface Area of Pans:  %f\n", polygons->getArea()+polygons->getNumber()*height*2*sqrt(area*tan(pi/sides)/sides));
				printf("Area of Oven:          %f\n", plot->getArea());
				printf("Covered Ratio:         %f\n", polygons->getArea()/plot->getArea());
				break;

			case 4:
				results.clear();
				for( sides = 3; sides <= 16; sides++ )
				{

					if( sides == 16 ) sides = 50;

					createModel();

					vector<double> temp;
					temp.resize(8);
					temp[0] = sides;
					temp[1] = polygons->getNumber();
					temp[2] = polygons->getArea();
					temp[3] = polygons->getArea()+polygons->getNumber()*height*2*sqrt(area*tan(pi/sides)/sides);
					temp[4] = plot->getArea();
					temp[5] = polygons->getArea()/plot->getArea();
					temp[6] = temp[4]-temp[2];
					temp[7] = temp[3];

					results.push_back(temp);

				}

				imax = 100;
				for( int i = 0; i <= imax; i++ )
				{

					p = (double)i/(double)imax;

					limit1 = 0;
					limit2 = 0;
					for( int i = 0; i < results.size(); i++ )
					{

						limit1 += results[i][6];
						limit2 += results[i][7];

					}

					index = 0;
					minimum = pow(2,15)-1;
					for( int i = results.size() - 1; i >= 0; i-- )
					{

						double weight = p*results[i][6]/limit1 + (1-p)*results[i][7]/limit2;

						if( weight < minimum )
						{

							minimum = weight;
							index = i;

						}

					}
					
					printf("%1.2f %2d %2.2f\n", p, (int)results[index][0],results[index][5]);


				}
				break;

			case 5:
				results.clear();
				for( sides = 3; sides <= 16; sides++ )
				{

					if( sides == 16 ) sides = 50;

					createModel();

					vector<double> temp;
					temp.resize(8);
					temp[0] = sides;
					temp[1] = polygons->getNumber();
					temp[2] = polygons->getArea();
					temp[3] = polygons->getArea()+polygons->getNumber()*height*2*sqrt(area*tan(pi/sides)/sides);
					temp[4] = plot->getArea();
					temp[5] = polygons->getArea()/plot->getArea();
					temp[6] = temp[4]-temp[2];
					temp[7] = temp[3];

					results.push_back(temp);

				}

				for( int i = 0; i < results.size(); i++ )
					printf("%d %0.2f\n",(int)results[i][0],results[i][5]);

				break;

			case 99:
				//plot->write("set term postscript eps enhanced color");
				plot->write("set terminal svg enhanced size 800 800");
				plot->write("set object 2147483647 rect from screen 0, 0, 0 to screen 1, 1, 0 behind");
				plot->write("set object 2147483647 rect fc rgb 'white' fillstyle solid 1.0");
				//plot->write("set term png enhanced color");
				char savefile[128]; sprintf(savefile, "./screenshots/oven-%Lu.svg", (unsigned long long)time( NULL ));
				char command[128];  sprintf(command,  "set out \"%s\"",					savefile);
				char status[128];   sprintf(status,   "Screenshot saved to \"%s\".",	savefile);
				plot->write( command );
				plot->refresh();
				printf("%s\n", status);
				plot->write( "set term x11" );
				break;

			case 1337:
				unsigned long long timestamp;
				timestamp = time( NULL );

				for( double i = 1; i <= 5; i+=0.5 )
				{

					plot->setXMax( i);
					plot->setXMin(-i);

					for( double j = 1; j <= 5; j+=0.5 )
					{

						plot->setYMax( j);
						plot->setYMin(-j);

						plot->setArea();

						sprintf(command, "mkdir -p ./screenshots/oven-%Lu/%02.0fx%02.0f/", timestamp, 2*i, 2*j);
						system( command );

						for( sides = 3; sides <= 10; sides++ )
						{
					
							createModel();

							plot->write("set terminal svg enhanced size 800 800");
							plot->write("set object 2147483647 rect from screen 0, 0, 0 to screen 1, 1, 0 behind");
							plot->write("set object 2147483647 rect fc rgb 'white' fillstyle solid 1.0");
							sprintf(savefile, "./screenshots/oven-%Lu/%02.0fx%02.0f/Polygon-%02dSides.svg", timestamp, 2*i, 2*j, sides);
							sprintf(command,  "set out \"%s\"",					savefile);
							sprintf(status,   "Screenshot saved to \"%s\".",	savefile);
							plot->write( command );
							plot->refresh();
							printf("%s\n", status);
							plot->write( "set term x11" );

						}

					}

				}

				break;

			default:
			case 0:
				delete plot;
				return 0;

		}

	}

	return 0;

}

void createModel()
{

	delete polygons;
	polygons = new Polygons(sides,area);

}
