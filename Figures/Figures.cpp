#include "Figures.h"

using namespace std;

Figure :: ~Figure () {}

void Polygon :: rotate (double angle) {
	Dot center (getCenter ());
	for (int i = 0; i < getAngle (); i++)
		dots [i].rotatePi (angle, center);
}
//Polygon :: Polygon (int _n): n (_n) {}
Polygon :: Polygon (unsigned int angle, ...) : n (angle) {
	va_list args;
	va_start (args, angle);
	this -> dots = new Dot [angle];
	for (int i = 0; i < angle; i++)
		dots [i] = va_arg (args, Dot);
	va_end (args);
}
Polygon :: Polygon (unsigned int angle, Dot * _dots): n (angle) {
	this -> dots = new Dot [angle];
	for (int i = 0; i < angle; i++)
		dots [i] = _dots [i];
}
Polygon :: Polygon (vector <Dot> _dots) : n (_dots.size ()) {
	dots = new Dot [n];
	for (int i = 0; i < n; i++)
		dots [i] = _dots [i];
}
Polygon :: Polygon (const Polygon& _polygon) : n (_polygon.n){
	dots = new Dot [n];
	for (int i = 0; i < n; i++)
		dots [i] = _polygon.dots [i];

}
Dot Polygon :: getHighest () const {
	auto highest = dots [0];
	for (int i = 1; i < getAngle (); ++i)
		if (highest.getY () < dots [i].getY ())
			highest = dots [i];

	return highest;
}
Dot Polygon :: getLowest () const {
	auto lowest = dots [0];
	for (int i = 1; i < getAngle (); ++i)
		if (lowest.getY () > dots [i].getY ())
			lowest = dots [i];

	return lowest;
}
Dot Polygon :: getRighest () const {
	auto righest = dots [0];
	for (int i = 1; i < getAngle (); ++i)
		if (righest.getX () < dots [i].getX ())
			righest = dots [i];

	return righest;
}
Dot Polygon :: getLeftest () const {
	auto leftest = dots [0];
	for (int i = 1; i < getAngle (); ++i)
		if (leftest.getX () > dots [i].getX ())
			leftest = dots [i];

	return leftest;
}
int Polygon :: getAngle () const {
	return n;
}
Polygon :: ~Polygon () {
	delete [] dots;
}
bool Polygon :: isHaveInside (const Dot & dot) const {

	vector <Line> lines = getLines ();
	for (auto line : lines)						//проверка, принадлежит ли точка стороне фигуры
		if (line.isContains (dot))
			return true;
	Line beam;	//луч от точки, за пределы фигуры
	Dot end;

	/*--------------Подбираем такой луч, который не пересекался бы ни с одной точкой фигуры----------------*/
	bool flag;		//признак прохождения проверки
	double highest = getHighest ().getY ();
	double lowest = getLowest ().getY ();
	double leftest = getLeftest ().getX ();
	double righest = getRighest ().getX ();
	const unsigned int angle = getAngle ();

	do {
		switch (rand () % 4) {
		case 0: end = Dot (rand () % int (righest - leftest) + leftest, highest + 1); break;
		case 1: end = Dot (rand () % int (righest - leftest) + leftest, lowest - 1); break;
		case 2: end = Dot (leftest - 1, rand () % int (highest - lowest) + lowest); break;
		case 3: end = Dot (righest + 1, rand () % int (highest - lowest) + lowest); break;
		}
		beam = Line (dot, end);
		flag = true;
		for (int i = 0; i < angle; ++i)
			if (beam.isContains (dots [i]))
				flag = false;
	} while (!flag);



	//int intersects = ln.isIntersect(beam);		//(int) => 0 or 1
	bool intersect = false;							//признак четности пересечений
	/*-------------Сравниваю со всеми остальными линиями на пересечение--------------*/
	for (int i = 0; i < angle; i++)
		intersect ^= beam.isIntersect (Line (dots [i], dots [(i + 1) % angle]));	//меняем бит четности

	return intersect;
}
bool Polygon :: operator == (const Polygon & other) const {
	/*
	Следующая проверка кажется избыточно сложной на вид. Но иначе нельзя! Ведь если, например,
	существуют два многоугольника с равным набором точек, это еще не значит, что они равны.
	Возможна ситуация, когда те же точки соединены в другой последовательности.
	*/

	bool flag;
	Line L1, L2;
	int thisAngle = getAngle ();
	int otherAngle = other.getAngle ();

	if (thisAngle != otherAngle)
		return false;

	for (int i = 0; i < thisAngle; i++) {		//Для каждой линии в фигуре
		L1 = Line (this -> dots [i], this -> dots [(i + 1) % thisAngle]);
		for (int j = 0; j < otherAngle; j++) {
			flag = false;
			L2 = Line (other.dots [j], other.dots [(j + 1) % otherAngle]);
			if (L2 == L1) {						//проверяем равенство с линией в другой
				flag = true;
				break;
			}
		}
		if (!flag)
			return false;
	}
	return true;
}
bool Polygon :: isIntersect (const Figure * figure) const  {
	//const Figure * ptr = &figure;
	const Circle * circle = dynamic_cast <const Circle *> (figure);
	if (circle)
		return circle -> isIntersect (this);

	const Polygon * polygon = dynamic_cast <const Polygon *> (figure);
	if (polygon)
		return isIntersect (polygon);
}
bool Polygon :: isIntersect (const Polygon * polygon) const  {
	int polygonAngle = polygon -> getAngle ();
	Line line;

	for (int i = 0; i < polygonAngle; i++) {
		line = Line (polygon -> dots [i], polygon -> dots [(i + 1) % polygonAngle]);
		if (isIntersect (line))
			return true;
	}
	return false;
}
bool Polygon :: isIntersect (const Line & line) const  {
	Line edge;
	int thisAngle = this -> getAngle ();

	for (int i = 0; i < thisAngle; i++) {
		edge = Line (dots [i], dots [(i + 1) % thisAngle]);
		if (line.isIntersect (edge))
			return true;
	}
	return false;
}
vector <Dot> Polygon :: getIntersects (const Line& line) const {
	vector <Dot> intersects;
	Line edge;
	int thisAngle = this -> getAngle ();
	//if (!isIntersect (line))
	//	return intersects;
	for (int i = 0; i < thisAngle; i++) {
		edge = Line (dots [i], dots [(i + 1) % thisAngle]);
		if (line.isIntersect (edge))
			intersects.push_back (line.projection (edge));
	}
	return intersects;
}
vector <Polygon> Polygon :: getIntersects (const Figure * figure) const {
	vector <Polygon> intersects;
	if (!isIntersect (figure))
		return intersects;


	if (dynamic_cast <const Circle *> (figure))
		return intersects;

	const Polygon * polygon = dynamic_cast <const Polygon *> (figure);
	if (!polygon)
		return intersects;

	deque <Dot> hotspots;
	vector <Line> thisLines = getLines ();
	vector <Line> otherLines = polygon -> getLines ();
	vector <Dot> intersectsDots;


	for (auto dot : getDots ())
		if (polygon -> isHaveInside (dot))
			hotspots.push_back (dot);

	for (auto dot : polygon -> getDots ())
		if (isHaveInside (dot))
			hotspots.push_back (dot);

	for (auto line : thisLines) {
		intersectsDots = polygon -> getIntersects (line);
		hotspots.insert (hotspots.end (), intersectsDots.begin (), intersectsDots.end ());
	}


	struct {
		bool operator () (const Dot& d1, const Dot& d2) const {
			if (abs (d1.getX () - d2.getX ()) > .01)
				if (d1.getX () < d2.getX ())
					return true;
				else
					return false;

			if (abs (d1.getY () - d2.getY ()) > .01)
				if (d1.getY () < d2.getY ())
					return true;
				else
					return false;

			return false;
		}
	} comp;

	//иногда, довольно редко, в hotspots появляются одинаковые точки. Например, если у двух фигур общая вершина или сторона, всякое бывает
	sort (hotspots.begin (), hotspots.end (), comp);
	hotspots.erase (unique (hotspots.begin (), hotspots.end ()), hotspots.end ());

	if (hotspots.size () < 2)
		return intersects;
	//для всех узловых точек нужно построить прямые, соединяющие их
	vector <Line> allLines (thisLines);
	allLines.insert (allLines.end (), otherLines.begin (), otherLines.end ());

	bool dotsOver;
	intersectsDots.clear ();
	intersectsDots.push_back (hotspots.front ());
	hotspots.pop_front ();
	while (hotspots.size () > 0) {
		dotsOver = true;
		for (auto line : allLines) {	//ищем, какие еще линии содержат эту точку
			vector <deque <Dot> :: iterator> posssible;
			if (line.isContains (intersectsDots.back ())) {		//если есть такая линия
				deque <Dot> :: iterator startNode = hotspots.end ();// = line.getBegin ();
				deque <Dot> :: iterator endNode = hotspots.end ();// = line.getEnd ();
				const Dot positiveDirection = line.getBegin () - intersectsDots.back ();
				for (auto dot = hotspots.begin (); dot != hotspots.end (); ++dot) {		//то смотрим, какие еще точки на ней находятся
					if (line.isContains (*dot)) {
						Dot vect = *dot - intersectsDots.back ();
						if ((vect.getX () * positiveDirection.getX () > 0) || (vect.getY () * positiveDirection.getY () > 0)) {		//векторы сонаправлены
							if (endNode == hotspots.end () || vect.length () < endNode -> dist (intersectsDots.back ()))
								endNode = dot;
						}
						else  //векторы противоположнонаправлены
							if (startNode == hotspots.end () || vect.length () < startNode -> dist (intersectsDots.back ()))
								startNode = dot;
					}
				}
				//из всех возможных точек нужно выбрать одну. (максимум подходящих - две)
				Dot lineCenter;
				if (startNode != hotspots.end () && polygon -> isHaveInside (lineCenter = Line (intersectsDots.back (), *startNode).getCenter ()) && this -> isHaveInside (lineCenter) && (intersectsDots.size () < 2 || !(Line (intersectsDots.back (), *startNode).isContains (intersectsDots [intersectsDots.size () - 2])))) {		//Если линия существует и находится внутри обеих фигур
					intersectsDots.push_back (*startNode);
					hotspots.erase (startNode);
					dotsOver = false;
				}
				else if (endNode != hotspots.end () && polygon -> isHaveInside (lineCenter = Line (intersectsDots.back (), *endNode).getCenter ()) && this -> isHaveInside (lineCenter) && (intersectsDots.size () < 2 || !(Line (intersectsDots.back (), *endNode).isContains (intersectsDots [intersectsDots.size () - 2])))) {
					intersectsDots.push_back (*endNode);
					hotspots.erase (endNode);
					dotsOver = false;
				}
			}
		}
		if (dotsOver || hotspots.size () == 0) {
			intersects.push_back (Polygon (intersectsDots));
			intersectsDots.clear ();
			if (hotspots.size () > 0) {
				intersectsDots.push_back (hotspots.front ());
				hotspots.pop_front ();
			}
		}
	}
	return intersects;
}
bool Polygon :: isInclude (const Figure * figure) const {		//Нужно переписать. Использовать идею проверки на точку
	//внутри фигуры (построением луча). Если нет пересечений,
	//то можно проверить лишь одну любую точку
	if (isIntersect (figure))
		return false;
	/*
	//			↓ this					other ↓
	if (getHighest ().getY () < figure.getHighest ().getY () ||		//довольно очевидная предварительная проверка
	getLowest ().getY () > figure.getLowest ().getY () ||		//если самая нижняя точка жтой фигуры выше
	getLeftest ().getX () > figure.getLeftest ().getX () ||		//то очевидно, что другая фигура в нее не включена
	getRighest ().getX () < figure.getRighest ().getX ())
	return false;*/

	const Polygon * polygon = dynamic_cast <const Polygon *> (figure);
	if (polygon)
		return isHaveInside (polygon -> dots [0]);

	const Circle * circle = dynamic_cast <const Circle *> (figure);
	if (circle)
		return isHaveInside (circle -> getCenter ());

	/*int thisAngle = getAngle ();
	bool flag1 = false;
	bool flag2 = false;

	for (int i = 0; i < thisAngle; i++) {
	if ((dots [(i + 1) % thisAngle].getX () - dots [i].getX ())*(figure.dots [0].getY () - dots [i].getY ()) - (dots [(i + 1) % thisAngle].getY () - dots [i].getY ())*(figure.dots [0].getX () - dots [i].getX ()) < 0)
	flag1 = true;

	if ((dots [(i + 1) % thisAngle].getX () - dots [i].getX ())*(figure.dots [0].getY () - dots [i].getY ()) - (dots [(i + 1) % thisAngle].getY () - dots [i].getY ())*(figure.dots [0].getX () - dots [i].getX ()) > 0)
	flag2 = true;

	if (flag1 & flag2)
	return false;
	}

	return true;*/
}
void Polygon :: print () const  {
	for (int i = 0; i < this -> getAngle (); i++) {
		dots [i].print ();
		cout << "   ";
	}
	cout << endl;
}
void Polygon :: move (double _x, double _y) {
	for (int i = 0; i < this -> getAngle (); i++)
		dots [i].move (_x, _y);
}
void Polygon :: move (Dot & _d) {
	for (int i = 0; i < this -> getAngle (); i++)
		dots [i].move (_d);
}
vector <Line> Polygon :: getLines () const {
	int angles = getAngle ();
	vector <Line> lines;
	for (int i = 0; i < angles; ++i)
		lines.push_back (Line (dots [i], dots [(i + 1) % angles]));
	return lines;
}
double Polygon :: getPerimeter () const {
	vector <Line> lines = getLines ();
	double perimetr = 0;
	for (auto line : lines)
		perimetr += line.getLen ();
	return perimetr;
}
Dot Polygon :: getCenter () const {
	const unsigned int angle = getAngle ();
	Dot center;
	for (Dot dot : getDots ())
		center += dot / angle;
	return center;
}
double Polygon :: getSquare () const {
	//подробности тут: http://algolist.manual.ru/maths/geom/polygon/area.php
	double square = 0;
	const unsigned int angle = getAngle ();
	for (int i = 0; i < angle; i++)
		square += (dots [i].getX () + dots [(i + 1) % angle].getX ()) * (dots [i].getY () - dots [(i + 1) % angle].getY ()) / 2;

	return abs (square);
}
void Polygon :: draw (sf::RenderWindow & window, float scale, const Dot & moveTo) const {
	for (Line line : getLines ())
		line.draw (window, scale, moveTo);
}
vector <Dot> Polygon :: getDots () const {
	vector <Dot> Vdots (getAngle ());// (dots, dots + n);
	Vdots.assign (dots, dots + getAngle ());
	return Vdots;
}
bool Polygon :: canExist () const {
	if (!dots)
		return false;

	for (int i = 0; i < n; i++)			//Проверяет, есть ли в фигуре совпадающие точки
		for (int j = i + 1; j < n; j++)
			if (dots [i] == dots [j])
				return false;

	Line _1, _2;
	for (int i = 0; i < n - 2; i++)		//Проверяет, есть ли самопересечения
		for (int j = i + 2; (j <= (i + n - 2)) && (j < n); j++)
			if (_1.setDots (dots [i], dots [i + 1]).isIntersect (_2.setDots (dots [j], dots [(j + 1) % n])))
				return false;

	for (int i = 0; i < n; i++)			//Проверяет, образуют ли три точки прямую
		if (Dot :: isLine (dots [i], dots [i], dots [(i + 1) % n]))
			return false;

	return true;
}

Triangle :: Triangle (Dot &one, Dot &two, Dot &three): Polygon (Triangle :: angleNumber, one, two, three) {}
Triangle :: Triangle (Dot * _dots) : Polygon (Triangle :: angleNumber, _dots) {}
Triangle :: Triangle (const Triangle & obj) : Polygon (Triangle :: angleNumber, obj.dots) {}
double Triangle :: getSquare () const  {
	double p = 0;
	for (int i = 0; i < 3; i++)
		p += Line (this -> dots [i], this -> dots [(i + 1) % 3]).getLen ();
	p /= 2;

	double res = p;

	for (int i = 0; i < 3; i++)
		res *= p - Line (this -> dots [i], this -> dots [(i + 1) % 3]).getLen ();
	res = sqrt (res);
	return res;
}
bool Triangle :: canExist () const {
	if (this -> dots [0] == this -> dots [1] ||
		this -> dots [1] == this -> dots [2] ||
		this -> dots [2] == this -> dots [0])
		return false;

	return !(Dot :: isLine (this -> dots [0], this -> dots [1], this -> dots [2]));
}


Trapeze :: Trapeze (Dot& center, double width0, double width1, double height, double angle) {
	n = angleNumber;
	dots = new Dot [n];
	dots [0] = Dot (center).move (-width1 / 2, height / 2).rotate (angle, center);
	dots [1] = Dot (center).move (width1 / 2, height / 2).rotate (angle, center);
	dots [2] = Dot (center).move (width0 / 2, -height / 2).rotate (angle, center);
	dots [3] = Dot (center).move (-width0 / 2, -height / 2).rotate (angle, center);
}

Parallelogram :: Parallelogram (Dot& center, double height, double width, double angleSide, double angleRotate) {
	n = angleNumber;
	double piAngleSide = angleSide * PI / 180;
	double d = 0;
	if (abs (piAngleSide) > .01)
		d = width / 2 + cos (piAngleSide) * sqrt (pow (height / sin (piAngleSide), 2) + height*height);
	dots = new Dot [n];
	dots [0] = Dot (center).move (-d, height / 2).rotate (angleRotate, center);
	dots [1] = Dot (center).move (width - d, height / 2).rotate (angleRotate, center);
	dots [2] = Dot (center).move (d, -height / 2).rotate (angleRotate, center);
	dots [3] = Dot (center).move (-(width - d), -height / 2).rotate (angleRotate, center);
}

Rectangle :: Rectangle (Dot& center, double height, double width, double angle) {
	n = angleNumber;
	dots = new Dot [n];
	dots [0] = Dot (center).move (-width / 2, height / 2).rotate (angle, center);
	dots [1] = Dot (center).move (width / 2, height / 2).rotate (angle, center);
	dots [2] = Dot (center).move (width / 2, -height / 2).rotate (angle, center);
	dots [3] = Dot (center).move (-width / 2, -height / 2).rotate (angle, center);
}
double Rectangle :: getSquare () const {
	return dots [0].dist (dots [1]) * dots [0].dist (dots [3]);
}
bool Rectangle :: canExist () const {
	return true;
}

Quadrate :: Quadrate (Dot& centre, double side, double degree) {
	n = angleNumber;
	double rad = side * sqrt (2.) / 2;
	this -> dots = new Dot [n];
	this -> dots [0] = Dot (centre.getX () + rad * cos ((degree + 45) * PI / 180), centre.getY () + rad * sin ((degree + 45) * PI / 180));
	this -> dots [1] = Dot (centre.getX () - rad * sin ((degree + 45) * PI / 180), centre.getY () + rad * cos ((degree + 45) * PI / 180));
	this -> dots [2] = Dot (centre.getX () * 2 - this -> dots [0].getX (), centre.getY () * 2 - this -> dots [0].getY ());
	this -> dots [3] = Dot (centre.getX () * 2 - this -> dots [1].getX (), centre.getY () * 2 - this -> dots [1].getY ());
}
double Quadrate :: getSquare () const  {
	return pow (dots [0].dist (dots [1]), 2);
}

Rhombus :: Rhombus (Dot& center, double height, double width, double angle) {
	n = angleNumber;
	dots = new Dot [n];
	dots [0] = Dot (center).move (0, height / 2).rotate (angle, center);
	dots [1] = Dot (center).move (width / 2, 0).rotate (angle, center);
	dots [2] = Dot (center).move (0, -height / 2).rotate (angle, center);
	dots [3] = Dot (center).move (-width / 2, 0).rotate (angle, center);
}
double Rhombus :: getPerimeter () const {
	return dots [0].dist (dots [1]) * 4;
}
double Rhombus :: getSquare () const  {
	return dots [0].dist (dots [2]) * dots [1].dist (dots [3]) / 2;
}

Pentagon :: Pentagon (Dot& one, Dot& two, Dot& three, Dot& four, Dot& fife): Polygon (Pentagon :: angleNumber, one, two, three, four, fife) {}
Pentagon :: Pentagon (Dot * _dots) : Polygon (Pentagon :: angleNumber, _dots) {}


vector <Dot> Circle :: getIntersects (const Line& _line) const {
	Line line = _line;
	line.move (getCenter () * -1);
	double angle = Line (focuses [0], focuses [1]).getPiAngle ();
	line.rotatePi (angle);

	double Al = line.getA ();
	double Bl = line.getB ();
	double Cl = line.getC ();

	double R = radius;

	//Квадратное уравнение
	double Ad = 1 + (Al*Al) / (Bl*Bl);
	double Bd = Al*Cl / (Bl*Bl);
	double Cd = (Cl*Cl) / (Bl*Bl) - R*R;

	double D = 4 * (Bd * Bd - Ad * Cd);
	//cout << D << endl;
	if (D < 0)
		return vector <Dot> ();

	//Случай, когда D = 0 - это что-то совсем необычное. Можно об этом не думать
	vector <Dot> intersects;
	Dot intersect;
	for (int i = 0; i <= 1; i++) {
		double x = (-2 * Bd + (1 - 2 * i) * sqrt (D)) / (2 * Ad);
		double y = -(Al * x + Cl) / Bl;
		intersect = Dot (x, y).rotatePi (-angle).move (getCenter ());
		intersects.push_back (intersect);
	}
	return intersects;
}
int Circle :: getFocuses () const {
	return n;
}
Circle :: Circle (Dot center, double _radius): radius (abs (_radius)), n (Circle :: focusNumber) {
	focuses = new Dot [n];
	focuses [0] = center;
}
Circle :: Circle (int _n, double _radius) : n (_n), radius (_radius) {}
Circle :: Circle (const Circle & obj) : n (obj.n), radius (obj.radius), focuses (new Dot [n]){
	for (int i = 0; i < n; i++)
		focuses [i] = obj.focuses [i];
}
Circle :: ~Circle () {
	delete [] focuses;
}
Dot Circle :: getHighest () const {
	return Dot (focuses [0]).move (0, radius);
}
Dot Circle :: getLowest () const {
	return Dot (focuses [0]).move (0, -radius);
}
Dot Circle :: getLeftest () const {
	return Dot (focuses [0]).move (-radius, 0);
}
Dot Circle :: getRighest () const {
	return Dot (focuses [0]).move (radius, 0);
}
double Circle :: getSquare () const  {
	return PI * radius*radius;
}
double Circle :: getPerimeter () const {
	return 2 * PI * radius;
}
Dot Circle :: getCenter () const {
	return focuses [0];
}
void Circle :: draw (sf::RenderWindow & window, float scale, const Dot & moveTo) const {
	sf::CircleShape shape;

	shape.setRadius (radius * scale);
	shape.setFillColor (sf::Color::Transparent);
	shape.setOutlineThickness (1);
	shape.setOutlineColor (sf::Color::White);
	shape.setOrigin (radius * scale, radius * scale);
	shape.setPosition ((focuses [0].getX () + moveTo.getX ()) * scale, (focuses [0].getY () + moveTo.getY ()) * scale);
	window.draw (shape);
}
bool Circle :: isIntersect (const Figure * figure) const {
	const Polygon * polygon = dynamic_cast <const Polygon *> (figure);
	if (polygon)
		return isIntersect (polygon);

	const Circle * circle = dynamic_cast <const Circle *> (figure);
	if (circle)
		return isIntersect (circle);

}
bool Circle :: isIntersect (const Polygon * polygon) const {
	for (Line line : polygon -> getLines ())
		if (isIntersect (line))
			return true;

	return false;
}
bool Circle :: isIntersect (const Circle * circle) const {
	const Ellipse * ellipse = dynamic_cast <const Ellipse *> (circle);
	if (ellipse)
		return isIntersect (ellipse);

	double distance = focuses [0].dist (circle -> focuses [0]);
	return (distance < (radius + circle -> radius)) && (distance > abs (radius - circle -> radius));
}
bool Circle :: isIntersect (const Ellipse * ellipse) const {
	return ellipse -> isIntersect (this);
}
bool Circle :: isIntersect (const Line & line) const {
	return (line.dist (focuses [0]) < radius) && ((!isHaveInside (line.getBegin ())) || (!isHaveInside (line.getEnd ())));
}
bool Circle :: isInclude (const Figure * figure) const {
	if (isIntersect (figure))
		return false;
	const Polygon * polygon = dynamic_cast <const Polygon *> (figure);
	if (polygon)
		return isInclude (polygon);

	const Circle * circle = dynamic_cast <const Circle *> (figure);
	if (circle)
		return isInclude (circle);
}
bool Circle :: isInclude (const Polygon * polygon) const {
	for (Dot dot : polygon -> getDots ())
		if (dot.dist (focuses [0]) > radius)
			return false;

	return true;
}
bool Circle :: isInclude (const Circle * circle) const {
	if (radius < circle -> radius)
		return false;

	if (isIntersect (circle))
		return false;

	if (abs (radius - circle -> radius) < focuses [0].dist (circle -> focuses [0]))
		return false;

	return true;
}
bool Circle :: isHaveInside (const Dot & dot) const {
	return focuses [0].dist (dot) < radius;
}
void Circle :: print () const {
	cout << "Center: ";
	focuses [0].print ();
	cout << "     radius: " << radius << endl;
}
void Circle :: move (double x, double y) {			//полиморфный алгоритм, юзается как для круга, так и для эллипса
	for (int i = 0; i < getFocuses (); i++)
		focuses [i].move (x, y);
}
void Circle :: move (Dot & dot) {					//полиморфный алгоритм, юзается как для круга, так и для эллипса
	for (int i = 0; i < getFocuses (); i++)
		focuses [i].move (dot);
}
bool Circle :: operator == (const Circle & circle) const {//полиморфный алгоритм, юзается как для круга, так и для эллипса
	if (getFocuses () != circle.getFocuses () || radius != circle.radius)
		return false;

	bool flag = true;
	for (int i = 0; i < getFocuses (); i++)
		if (!(focuses [i] == circle.focuses [i]))
			flag = false;
	return flag;
}
bool Circle :: canExist () const  {
	return true;
}

vector <Dot> Ellipse :: getIntersects (const Line& _line) const {
	Line line = _line;
	line.move (getCenter () * -1);
	double angle = Line (focuses [0], focuses [1]).getPiAngle ();
	line.rotatePi (angle);

	double Al = line.getA ();
	double Bl = line.getB ();
	double Cl = line.getC ();

	//Уравнение эллипса
	double Ae = radius;
	double Be = getOtherRadius ();

	//Квадратное уравнение
	double Ad = Ae * Ae + pow (Al * Be / Bl, 2);
	double Bd = Al * Cl * Be * Be / (Bl * Bl);			//Тут не совсем b, а вдвое меньше. Так удобнее 
	double Cd = pow (Cl * Be / Bl, 2) - pow (Ae * Be, 2);

	double D = 4 * (Bd * Bd - Ad * Cd);
	//cout << D << endl;
	if (D < 0)
		return vector <Dot> ();

	//Случай, когда D = 0 - это что-то совсем необычное. Можно об этом не домать
	vector <Dot> intersects;
	Dot intersect;
	for (int i = 0; i <= 1; i++) {		//типа цикл сначала дла +√D, потом для -√D
		double x = (-2 * Bd + (1 - 2 * i) * sqrt (D)) / (2 * Ad);
		double y = -(Al * x + Cl) / Bl;
		intersect = Dot (x, y).rotatePi (-angle).move (getCenter ());
		intersects.push_back (intersect);
	}
	return intersects;
}
Ellipse :: Ellipse (Dot dot0, Dot dot1, double _radius): Circle (Ellipse :: focusNumber, abs (_radius)) {
	focuses = new Dot [n];
	focuses [0] = dot0;
	focuses [1] = dot1;
}
Dot Ellipse :: getCenter () const {
	return (focuses [0] + focuses [1]) / 2;
}
Dot Ellipse :: getHighest () const {
	Dot highest = focuses [0].getY () > focuses [1].getY () ? focuses [0] : focuses [1];
	return highest.move (0, radius);
}
Dot Ellipse :: getLowest () const {
	Dot lowest = focuses [0].getY () < focuses [1].getY () ? focuses [0] : focuses [1];
	return lowest.move (0, -radius);
}
Dot Ellipse :: getLeftest () const {
	Dot leftest = focuses [0].getX () < focuses [1].getX () ? focuses [0] : focuses [1];
	return leftest.move (-radius, 0);
}
Dot Ellipse :: getRighest () const {
	Dot righest = focuses [0].getX () > focuses [1].getX () ? focuses [0] : focuses [1];
	return righest.move (radius, 0);
}
double Ellipse :: getOtherRadius () const {
	return sqrt (pow (focuses [0].dist (focuses [1]) / 2, 2) + radius * radius);
}
double Ellipse :: getSquare () const {
	return PI * radius * getOtherRadius ();
}
double Ellipse :: getPerimeter () const {
	double otherRadius = getOtherRadius ();
	return 4 * (PI * radius * otherRadius + pow (otherRadius - radius, 2)) / (radius + otherRadius);
}
bool Ellipse :: isIntersect (const Circle * other) const {
	double area [4]; //top y, right x, bottom y, left x // ↑→↓←

	//!!!!!!!!РЕАЛИЗОВАТЬ СЛЕДУЮЩУЮ ИДЕЮ:
	//генерировать 2 прямоугольника по координатам верхних/нижних/левых/правых точек
	//Проверять, пересекаются ли эти прямоугольники (если нет - return false)
	//Создать область, в которой будет выполняться поиск...
	area [0] = (getHighest ().getY () > other -> getHighest ().getY ()) ? getHighest ().getY () : other -> getHighest ().getY ();
	area [1] = (getRighest ().getX () < other -> getRighest ().getX ()) ? getRighest ().getX () : other -> getRighest ().getX ();
	area [2] = (getLowest ().getY () < other -> getLowest ().getY ()) ? getLowest ().getY () : other -> getLowest ().getY ();
	area [3] = (getLeftest ().getX () > other -> getLeftest ().getX ()) ? getLeftest ().getX () : other -> getLeftest ().getX ();



	double width = area [1] - area [3];
	double eps = .05;
	Line scanning;
	vector <Dot> intersectsThis;
	vector <Dot> intersectsOther;
	Line ___0;
	Line ___1;

	for (double step = (area [1] - area [3]) / 2; abs (step) > eps; step /= 2) {
		for (int i = 1; i * abs (step) <= width; i += 2) {
			scanning = Line (Dot (area [3] + i * step, area [0]), Dot (area [3] + i * step, area [2]));
			intersectsThis = getIntersects (scanning);
			intersectsOther = other -> getIntersects (scanning);

			if (intersectsThis.size () > 0 && intersectsOther.size () > 0) {
				___0 = Line (intersectsThis [0], intersectsThis [1]);
				___1 = Line (intersectsOther [0], intersectsOther [1]);
				if ((___0.isContains (intersectsOther [0]) || ___0.isContains (intersectsOther [1])) && (___1.isContains (intersectsThis [0]) || ___1.isContains (intersectsThis [1])))
					return true;
			}
		}
	}
	return false;
}

bool Ellipse :: isHaveInside (const Dot & dot) const {
	double distance = focuses [0].dist (dot) + focuses [1].dist (dot);
	return distance < (getOtherRadius () * 2);
}
bool Ellipse :: isIntersect (const Line & line) const {
	//Если один конец линии внутри эллипса, а другой снаружи, то это пересечение
	if (isHaveInside (line.getBegin ()) ^ isHaveInside (line.getEnd ()))
		return true;

	//Иначе линия может пересекаться с эллипсом посередине
	for (Dot intersect : getIntersects (line))
		if (line.isContains (intersect))
			return true;

	return false;

}
void Ellipse :: print () const {
	cout << "Focuses: ";
	focuses [0].print ();
	cout << "   ";
	focuses [1].print ();
	cout << "     radius: " << radius << endl;
}
void Ellipse :: draw (sf::RenderWindow & window, float scale, const Dot & moveTo) const {
	sf::CircleShape shape;

	shape.setFillColor (sf::Color::Transparent);
	shape.setOutlineThickness (1);
	shape.setOutlineColor (sf::Color::White);

	double dist = focuses [0].dist (focuses [1]);
	double scaleFactor = getOtherRadius () / radius;
	shape.setRadius (radius * scale);
	shape.scale (scaleFactor, 1);

	double angle = Line (focuses [0], focuses [1]).getPiAngle () * 180 / PI;

	Dot center = getCenter ();

	shape.setOrigin (radius * scale, radius * scale);
	shape.rotate (angle);
	shape.setPosition ((center.getX () + moveTo.getX ()) * scale, (center.getY () + moveTo.getY ()) * scale);

	window.draw (shape);
}