#pragma once
#ifndef DOT_H
#define DOT_H

#include <iostream>
#include <iomanip>
#include <math.h>

const double PI = 3.1415926535;

using namespace std;

class Dot {
	double x;
	double y;
public:
	Dot (double _x, double _y): x (_x), y (_y) {}		//округление до сотых
	Dot () : x (0), y (0) {}
	double getX () const {
		return this -> x;
	}
	double getY () const {
		return this -> y;
	}
	Dot & move (double _x, double _y) {
		this -> x += _x;
		this -> y += _y;
		return *this;
	}
	Dot & move (Dot _d) {
		return this -> move (_d.x, _d.y);
	}
	static bool isLine (Dot & p1, Dot & p2, Dot & p3) {			//проверка, образуют ли три точки линию
		double b, k;
		k = (p1.y - p2.y) / (p1.x - p2.x);
		b = p1.y - (k * (p1.x));

		if (abs (p3.y - (k * p3.x + b)) < 1E-2)
			return true;

		return false;
	}
	bool operator == (const Dot & other) const  {
		return (abs (this -> x - other.x) < 1E-2 && abs (this -> y - other.y) < 1E-2);
	}
	void setCoord (Dot _x) {
		this->x = _x.getX ();
		this->y = _x.getY ();
	};
	Dot operator + (const Dot & other) const  {
		return Dot (this -> getX () + other.getX (), this -> getY () + other.getY ());
	}
	Dot & operator += (const Dot & other) {
		x += other.getX ();
		y += other.getY ();
		return * this;
	}
	Dot operator / (const double divider) const {
		return Dot (getX () / divider, getY () / divider);
	}
	Dot operator - (const Dot & other) const  {
		return Dot (this -> getX () - other.getX (), this -> getY () - other.getY ());
	}
	Dot operator * (const double mult) {
		return Dot (this -> getX () * mult, this -> getY () * mult);
	}
	double length () const {
		return sqrt (x*x + y*y);
	}
	Dot & rotatePi (double angle, Dot center = Dot ()) {
		move (center * (-1));
		//begin = Dot (begin.getX () * cos (angle) + begin.getY () * sin (angle), -1 * begin.getX () * sin (angle) + begin.getY () * cos (angle));
		
		double x_ = x * cos (angle) + y * sin (angle);
		double y_ = y * cos (angle) - x * sin (angle);
		x = x_;
		y = y_;
		move (center);
		return *this;
	}
	Dot & rotate (double angle, Dot center = Dot ()) {
		return rotatePi (angle * PI / 180, center);
	}
	void print () const {
		cout << fixed;		//форматирование вывода. Для вывод лишь одного символа после точки (4.2, например)
		cout << setprecision (1) << '(' << getX () << "; " << setprecision (1) << getY () << ')';
	}
	double dist (const Dot & other) const {
		return sqrt (pow (getX () - other.getX (), 2) + pow (getY () - other.getY (), 2));
	}
};

#endif