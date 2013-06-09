#include <cstdlib>
#include <cstring>
#include <sstream>
#include <GL/gl.h>
#include <GL/freeglut.h>

using namespace std;

template <class xType,class yType>
class graphPoint
{
public:
    xType x;
    yType y;
    double scaledX;
    double scaledY;
    float R,G,B;

    graphPoint(xType x,yType y,float R, float G, float B)
    {
        (*this).R = R;
        (*this).G = G;
        (*this).B = B;
        (*this).x = x;
        (*this).y = y;
    }

    graphPoint(xType x,yType y)
    {
        *this = *new graphPoint(x,y,0.0,0.1,1.0);

    }

    void draw(xType xMin,xType xMax,yType yMin, yType yMax)
    {
        glColor3f(R,G,B);
        scaledX = 2*(((float)x - (float)xMin)/((float)xMax - (float)xMin)) - (float)1;
        scaledY = 2*(((float)y - (float)yMin)/((float)yMax - (float)yMin)) - (float)1;

        glBegin(GL_POINTS);
        glVertex2f(scaledX,scaledY);
        glEnd();
    }

};
template <class xType,class yType>
class graph
{
private:
    graphPoint<xType,yType> * pointArray;
    xType xMin, xMax;
    yType yMin, yMax;
    int numPoints;
    bool scaleUpdated;

    void renderString(float x, float y, void *font, const char* string)
    {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(x, y);
        glutBitmapString(font, (unsigned char *)string);
    }

    void drawAxis()
    {

        ostringstream lables[2][2];
        ostringstream origin;

        glColor3f(0.0,0.0,0.0);
        glLineWidth(3);

        glBegin(GL_LINES);
        glVertex2f(-1.00,0.00);
        glVertex2f( 1.00,0.00);

        glVertex2f( 0.00,-1.00);
        glVertex2f( 0.00, 1.00);

        glVertex2f(-1.00, 0.05);
        glVertex2f(-1.00,-0.05);

        glVertex2f( 1.00, 0.05);
        glVertex2f( 1.00,-0.05);

        glVertex2f(-0.05,-1.00);
        glVertex2f( 0.05,-1.00);

        glVertex2f(-0.05, 1.00);
        glVertex2f( 0.05, 1.00);
        glEnd();

        lables[0][0] << xMin;
        lables[0][1] << xMax;
        lables[1][0] << yMin;
        lables[1][1] << yMax;

        origin << '(' << 0.5*(xMax + xMin) << ',' << 0.5*(yMax + yMin) << ')';

        renderString(-1.0                , 0.05, GLUT_BITMAP_TIMES_ROMAN_24, lables[0][0].str().c_str());
        renderString(0.98-0.005*log(xMax), 0.05, GLUT_BITMAP_TIMES_ROMAN_24, lables[0][1].str().c_str());
        renderString(0.0                 , -1.0, GLUT_BITMAP_TIMES_ROMAN_24, lables[1][0].str().c_str());
        renderString(0.0                 , 0.95, GLUT_BITMAP_TIMES_ROMAN_24, lables[1][1].str().c_str());

        renderString(0.0                 , 0.05, GLUT_BITMAP_TIMES_ROMAN_24, origin.str().c_str());

    }

public:

    graph()
    {
        *this = *new graph(800,800);
    }

    graph(int xSize,int ySize)
    {
        char *myargv [1];
        int myargc=1;
        myargv [0]=strdup ("Graph");
        glutInit(&myargc, myargv);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(xSize,ySize);
        glutCreateWindow("Graph");

        glMatrixMode ( GL_PROJECTION );
        glClearColor ( 1.0, 1.0, 1.0, 0.0 );
        glPointSize(3.0);

        scaleUpdated = false;

        (*this).xMin = 0;
        (*this).xMax = 0;

        (*this).yMin = 0;
        (*this).yMax = 0;

        numPoints = 0;

        pointArray = (graphPoint<xType,yType> *)malloc(0);
    }

    void addPoint(graphPoint<xType,yType> point)
    {
        ++numPoints;
        pointArray = (graphPoint<xType,yType> *)realloc(pointArray,numPoints * sizeof(graphPoint<xType,yType>));
        pointArray[numPoints - 1] = point;

        if(point.x < xMin)
        {
            xMin = 1.2*point.x;
            scaleUpdated = true;
        }
        else if(point.x > xMax)
        {
            xMax = 1.2*point.x;
            scaleUpdated = true;
        }

        if(point.y < yMin)
        {
            yMin = 1.2*point.y;
            scaleUpdated = true;
        }
        else if(point.y > yMax)
        {
            yMax = 1.2*point.y;
            scaleUpdated = true;
        }
    }


    void display()
    {
        if(scaleUpdated)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            drawAxis();
            for(int i = 0; i<numPoints; ++i)
            {
                pointArray[i].draw(xMin,xMax,yMin,yMax);
            }
        }
        else
        {
            pointArray[numPoints -1].draw(xMin,xMax,yMin,yMax);
        }

        glFlush();
        glutSwapBuffers();
    }


    void plot(xType xMin,xType xMax,xType h, yType (*f)(xType) )
    {
        (*this).xMin = xMin;
        (*this).xMax = xMax;

        (*this).yMin = 0;
        (*this).yMax = 0;

        for(xType i = xMin; i<xMax; i = i + h)
        {
            addPoint(graphPoint<xType,yType>(i,f(i)));
        }
        scaleUpdated = true;
    }

    void animate()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        for(int i = 0; i<numPoints; ++i)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            drawAxis();
            for(int j = 0; j<i; ++j)
            {
                pointArray[j].draw(xMin,xMax,yMin,yMax);
                glFlush();
            }
            glutSwapBuffers();
        }

       // glutSwapBuffers();

    }
};


