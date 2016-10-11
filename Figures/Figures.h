#pragma once
#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Dot.h"
#include "Line.h"
#include <vector>
#include <iostream>
#include <deque>
#include <stdarg.h>
#include <algorithm>

class Circle;
class Ellipse;

class Figure {
public:
	virtual ~Figure () = 0;
	virtual Dot getHighest () const = 0;
	virtual Dot getLowest () const = 0;
	virtual Dot getLeftest () const = 0;
	virtual Dot getRighest () const = 0;
	virtual double getSquare () const = 0;
	virtual double getPerimeter () const = 0;
	virtual Dot getCenter () const = 0;
	virtual void draw (sf::RenderWindow &, float, const Dot &) const = 0;
	virtual bool isHaveInside (const Dot &) const = 0;

	virtual bool operator == (const Figure &) const = 0;
	virtual bool isIntersect (const Figure *) const = 0;
	virtual bool isIntersect (const Line &) const = 0;
	virtual vector <Dot> getIntersects (const Line& line) const = 0;
	virtual bool isInclude (const Figure *) const = 0;
	virtual void print () const = 0;
	virtual void move (double, double) = 0;
	virtual void move (Dot &) = 0;
	virtual bool canExist () const = 0;
};

class Polygon: public Figure {
protected:
	Dot * dots;
	int n;
	//Polygon (int);
	Polygon () {}
public:
	void rotate (double);
	virtual ~Polygon () override;
	Polygon (unsigned int, ...);
	Polygon (unsigned int, Dot *);
	Polygon (vector <Dot>);
	Polygon (const Polygon&);
	virtual int getAngle () const;
	virtual Dot getHighest () const override;
	virtual Dot getLowest () const override;
	virtual Dot getLeftest () const override;
	virtual Dot getRighest () const override;
	virtual double getSquare () const override;
	virtual double getPerimeter () const override;
	virtual Dot getCenter () const override;
	virtual void draw (sf::RenderWindow &, float, const Dot &) const override;
	vector <Line> getLines () const;
	vector <Dot> getDots () const;
	virtual bool isHaveInside (const Dot &) const override;
	virtual bool operator == (const Figure &) const override { return false; }
	virtual bool operator == (const Polygon &) const;
	virtual bool isIntersect (const Figure *) const override;
	virtual bool isIntersect (const Polygon *) const;
	virtual bool isIntersect (const Line &) const override;
	virtual bool isInclude (const Figure *) const override;
	virtual vector <Dot> getIntersects (const Line& line) const override;
	//template <class T>
	virtual vector <Polygon> getIntersects (const Figure *) const;
	virtual void print () const override;
	virtual void move (double, double) override;
	virtual void move (Dot &) override;
	virtual bool canExist () const override;
};

class Triangle: public Polygon {
	//virtual int getAngle () const override;
public:
	static const int angleNumber = 3;
	Triangle () : Polygon (0, new Dot [0]) {}
	Triangle (Dot *);
	//Triangle (Polygon &);
	Triangle (Dot &, Dot &, Dot &);
	Triangle (const Triangle &);
	virtual double getSquare () const override;
	virtual bool canExist () const override;
};

class Trapeze: public Polygon {
protected:
	Trapeze () {}
public:
	static const int angleNumber = 4;
	Trapeze (Dot&, double, double, double, double);
};

class Parallelogram: public Trapeze {
protected:
	Parallelogram () {}
public:
	Parallelogram (Dot&, double, double, double, double);
};

class Rectangle: public Parallelogram {
protected:
	Rectangle () {}
public:
	Rectangle (Dot&, double, double, double);
	virtual double getSquare () const override;
	virtual bool canExist () const override;
};

class Rhombus: public Parallelogram {
protected:
	Rhombus () {}
public:
	Rhombus (Dot&, double, double, double);
	virtual double getSquare () const override;
	virtual double getPerimeter () const override;
};

class Quadrate: public Rhombus {
public:
	Quadrate (Dot& centre, double side, double degree);
	virtual double getSquare () const override;
};


class Pentagon: public Polygon {
public:
	Pentagon () : Polygon (0, new Dot [0]) {}
	static const int angleNumber = 5;
	Pentagon (Dot&, Dot&, Dot&, Dot&, Dot&);
	Pentagon (Dot*);
};

class Circle: public Figure {
	static const int focusNumber = 1;
protected:
	Dot *focuses;
	double radius;
	int n;
	Circle (int, double);
public:
	virtual ~Circle () override;
	virtual vector <Dot> getIntersects (const Line&) const;
	Circle (Dot, double);
	Circle (const Circle&);
	virtual int getFocuses () const;
	virtual Dot getHighest () const override;
	virtual Dot getLowest () const override;
	virtual Dot getLeftest () const override;
	virtual Dot getRighest () const override;
	virtual double getSquare () const override;
	virtual double getPerimeter () const override;
	virtual Dot getCenter () const override;
	virtual bool operator == (const Circle &) const;
	virtual bool operator == (const Figure &) const override { return false; };
	virtual void draw (sf::RenderWindow &, float, const Dot &) const override; 
	virtual bool isIntersect (const Polygon *) const;
	virtual bool isIntersect (const Figure *) const override;
	virtual bool isIntersect (const Line &) const override;
	virtual bool isIntersect (const Circle *) const;
	virtual bool isIntersect (const Ellipse *) const;
	virtual bool isInclude (const Figure *) const override;
	virtual bool isInclude (const Polygon *) const;
	virtual bool isInclude (const Circle *) const;
	virtual bool isHaveInside (const Dot &) const override;
	virtual void print () const override;
	virtual void move (double, double) override;
	virtual void move (Dot &) override;
	virtual bool canExist () const override;
};

class Ellipse: public Circle {
	static const int focusNumber = 2;
public:
	virtual vector <Dot> getIntersects (const Line&) const;
	double getOtherRadius () const;
	Ellipse (Dot, Dot, double);
	virtual Dot getHighest () const override;
	virtual Dot getLowest () const override;
	virtual Dot getLeftest () const override;
	virtual Dot getRighest () const override;
	virtual double getSquare () const override;
	virtual double getPerimeter () const override;
	virtual Dot getCenter () const override;
	virtual bool isIntersect (const Circle *) const;
	virtual bool isIntersect (const Line &) const override;
	virtual bool isHaveInside (const Dot &) const override;
	virtual void print () const override;
	virtual void draw (sf::RenderWindow &, float, const Dot &) const override;
};
#endif