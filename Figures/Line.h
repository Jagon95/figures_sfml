#pragma once
#ifndef LINE_H
#define LINE_H

#include <SFML/Graphics.hpp>Circle ::
#include <math.h>
#include "Dot.h"


//const double PI = 3.1415926535;

class Line {
	Dot begin, end;
public:
	/*enum DotApplies {	LEFT,	RIGHT,	BEYOND,		BEHIND, BETWEEN,	ORIGIN, DESTINATION };
					//	СЛЕВА,	СПРАВА,	ВПЕРЕДИ,	ПОЗАДИ,	МЕЖДУ,		НАЧАЛО,	КОНЕЦ*/
	Line (Dot _begin, Dot _end): begin (_begin), end (_end) {};
	Line () {}
	/*DotApplies contact (const Dot & d) const {
		if (d == begin)
			return ORIGIN;
		else if (d == end)
			return DESTINATION;

		Dot a = begin - end;
		Dot b = d - end;
		double sa = a.getX () * b.getY () - b.getX () * a.getY ();

		if (sa > .05)
			return LEFT;
		else if (sa < -.05)
			return RIGHT;
		else if ((a.getX () * b.getX () < 1e-2) || (a.getY () * b.getY () < 1e-2))
			return BEHIND;
		else if (a.length () < b.length ())
			return BEYOND;

		return BETWEEN;
	}*/
	bool isContains (const Dot & dot) const {
		if (dot == begin || dot == end)
			return true;

		double A = getA ();
		double B = getB ();
		double C = getC ();
		double x = dot.getX ();
		double y = dot.getY ();

		if (abs (A*x + B*y + C) > .01)
			return false;

		double maxX = begin.getX ();
		double minX = end.getX ();
		if (abs (maxX - minX) > .01) {
			if (maxX < minX)
				swap (maxX, minX);
			if (x > maxX || x < minX)
				return false;
		}
		double maxY = begin.getY ();
		double minY = end.getY ();
		if (abs (maxY - minY) > .01) {
			if (maxY < minY)
				swap (maxY, minY);
			if (y < minY || y > maxY)
				return false;
		}
		return true;
	}
	bool isIntersect (const Line & other) const {
		return isIntersect (*this, other);
	}
	static bool isIntersect (const Line &line1, const Line &line2) {

		bool b0, b1, b2, b3;			//сложная математика. Читая код, почти невозможно понять, что происходит
										//подробности тут: e-maxx.ru/algo/segments_intersection_checking
		b0 = ((line2.end.getX () - line2.begin.getX ())*(line1.begin.getY () - line2.begin.getY ()) - (line2.end.getY () - line2.begin.getY ())*(line1.begin.getX () - line2.begin.getX ())) > 0;
		b1 = ((line2.end.getX () - line2.begin.getX ())*(line1.end.getY () - line2.begin.getY ()) - (line2.end.getY () - line2.begin.getY ())*(line1.end.getX () - line2.begin.getX ())) > 0;
		b2 = ((line1.end.getX () - line1.begin.getX ())*(line2.begin.getY () - line1.begin.getY ()) - (line1.end.getY () - line1.begin.getY ())*(line2.begin.getX () - line1.begin.getX ())) > 0;
		b3 = ((line1.end.getX () - line1.begin.getX ())*(line2.end.getY () - line1.begin.getY ()) - (line1.end.getY () - line1.begin.getY ())*(line2.end.getX () - line1.begin.getX ())) > 0;

		return ((b0 ^ b1) && (b2 ^ b3));
	}
	Dot getCenter () const {
		return (begin + end) / 2;
	}
	Line & setDots (Dot _begin, Dot _end) 	{
		this -> begin = _begin;
		this -> end = _end;
		return *this;
	}
	bool operator == (const Line & other) const {
		if (this -> begin == other.begin && this -> end == other.end)
			return true;

		if (this -> begin == other.end && this -> end == other.begin)
			return true;

		return false;
	}
	double getLen () const {
		return begin.dist (end);
	}
	const Dot getBegin () const {
		return begin;
	}
	const Dot getEnd () const {
		return end;
	}
	void move (const Dot & dot) {
		begin.move (dot);
		end.move (dot);
	}
	double getPiAngle () const {
		double angle = 0;
		if (abs (begin.getX () - end.getX ()) > 1e-2)
			angle = atan ((begin.getY () - end.getY ()) / (begin.getX () - end.getX ()));

		return angle;
	}
	Dot projection (const Dot & dot) const {
		double A = getA ();
		double B = getB ();
		double C = getC ();
		double Cd = A * dot.getY () - B * dot.getX ();
		double y = (B*C / A - Cd) / ( - B*B / A - A);
		double x = -(B * y + C) / A;
		return Dot (x, y);
	}
	Dot projection (const Line& line) const {
		double At = getA ();
		double Bt = getB ();
		double Ct = getC ();
		double Ao = line.getA ();
		double Bo = line.getB ();
		double Co = line.getC ();

		double x = -(Ct*Bo - Co*Bt) / (At*Bo - Ao*Bt);
		double y = -(At*Co - Ao*Ct) / (At*Bo - Ao*Bt);
		return Dot (x, y);
	}
	double dist (const Dot & dot) const {
		//хрен знает что происходит. Подробности тут: http://algolist.manual.ru/maths/geom/distance/pointline.php
		// и еще тут: http://ru.onlinemschool.com/math/library/analytic_geometry/p_line1/
		if (isContains (dot))
			return 0;

		Dot v = end - begin;
		Dot w1 = dot - begin;
		double c1 = v.getX () * (w1.getX ()) + v.getY () * (w1.getY ());
		if (c1 <= 0)
			return dot.dist (begin);

		Dot w2 = dot - end;
		double c2 = v.getX () * (w2.getX ()) + v.getY () * (w2.getY ());
		if (c2 >= 0)
			return dot.dist (end);

		double A = getA ();
		double B = getB ();
		double C = getC ();
		return abs (A * (dot.getX ()) + B * (dot.getY ()) + C) / sqrt (pow (A, 2) + pow (B, 2));
	}
	Line & rotate (double angle, Dot center = Dot ()) {
		angle *= PI / 180;
		return rotatePi (angle, center);
	}
	Line & rotatePi (double angle, Dot center = Dot ()) {
		/*begin.move (center * (-1));
		end.move (center * (-1));
		begin = Dot (begin.getX () * cos (angle) + begin.getY () * sin (angle), -1 * begin.getX () * sin (angle) + begin.getY () * cos (angle));
		end = Dot (end.getX () * cos (angle) + end.getY () * sin (angle), -1 * end.getX () * sin (angle) + end.getY () * cos (angle));
		begin.move (center);
		end.move (center);*/
		begin.rotatePi (angle, center);
		end.rotatePi (angle, center);
		return *this;
	}
	//Ax + By + C = 0
	double getA () const {
		return begin.getY () - end.getY ();
	}
	double getB () const {
		return end.getX () - begin.getX ();
	}
	double getC () const {
		return -getA () * (begin.getX ()) - getB () * (begin.getY ());
	}
	void draw (sf::RenderWindow & window, float scale, const Dot & moveTo) const {
		sf::Vertex line [] = {
			sf::Vertex (sf::Vector2f ((begin.getX () + moveTo.getX ()) * scale, (begin.getY () + moveTo.getY ()) * scale)),
			sf::Vertex (sf::Vector2f ((end.getX () + moveTo.getX ()) * scale, (end.getY () + moveTo.getY ()) * scale))
		};

		window.draw (line, 2, sf::Lines);
	}
};

#endif