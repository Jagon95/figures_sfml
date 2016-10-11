#include <SFML/Graphics.hpp>
#include <iostream>
#include "Dot.h"
#include "Line.h"
#include "Figures.h"

using namespace std;

const double COORD_X_MAX = 400;
const double COORD_X_MIN = -400;
const double COORD_Y_MAX = 400;
const double COORD_Y_MIN = -400;

class Factory {
	static Factory * top;
	Figure * data;
	char * name;
	Factory * next;
	static void draw_ (sf::RenderWindow & window, float scale, const Dot & moveTo) {

		Factory * elem = Factory :: top;
		while (elem) {
			elem -> data -> draw (window, scale, moveTo);

			elem = elem -> next;
		}
	}
	static void areaUpdate (Figure * n) {
		Dot tmpDot = n -> getHighest ();
		if (tmpDot.getY () > area [0])
			area [0] = tmpDot.getY ();
		tmpDot = n -> getLowest ();
		if (tmpDot.getY () < area [2])
			area [2] = tmpDot.getY ();

		tmpDot = n -> getRighest ();
		if (tmpDot.getX () > area [1])
			area [1] = tmpDot.getX ();
		tmpDot = n -> getLeftest ();
		if (tmpDot.getX () < area [3])
			area [3] = tmpDot.getX ();
	}
	static void areaRecounter () {
		area [0] = DBL_MIN;
		area [1] = DBL_MIN;
		area [2] = DBL_MAX;
		area [3] = DBL_MAX;
		Factory * elem = Factory :: top;
		while (elem) {
			areaUpdate (elem -> data);
			elem = elem -> next;
		}
	}
public:
	static double area [];	//↑ → ↓ ←
	const char * getName () {
		return name;
	}
	~Factory () {
		delete  data;
		delete [] name;
	}
	Factory (Figure * figure, char _name []) {
		if (!figure -> canExist ())
			throw ("Figure can't exist!");

		if (Factory :: cloneExist (figure)) 
			throw ("Clone exist.\nElement can't be created!");

		if (Factory :: find (_name)) 
			throw ("ID exist already\nElement can't be created!");

		if (figure -> getHighest ().getY () > COORD_Y_MAX ||
			figure -> getLowest ().getY () < COORD_Y_MIN ||
			figure -> getLeftest ().getX () < COORD_X_MIN ||
			figure -> getRighest ().getX () > COORD_X_MAX)
			throw ("Figure beyond the boundary of the window\nElement can't be created!");

		this -> next = Factory :: top;
		Factory :: top = this;
		this -> data = figure;
		this -> name = new char [strlen (_name) + 1];
		strcpy (this -> name, _name);

		areaUpdate (figure);
	}
	static bool removeItem (char _name []) {
		Factory * elem = Factory :: top;

		if (!elem)
			return false;

		if (!strcmp (elem -> name, _name)) {		//Factory :: top;
			Factory :: top = elem -> next;
			delete elem;
			areaRecounter ();
			return true;
		}

		while (elem -> next && strcmp (elem -> next -> name, _name))		//для всех непервых и непоследних элементов
			elem = elem -> next;

		if (elem -> next && !strcmp (elem -> next -> name, _name)) {
			Factory * tmp = elem -> next -> next;
			delete elem -> next;
			elem -> next = tmp;
			areaRecounter ();
			return true;
		}
		return false;
	}
	bool move (double x, double y) {
		this -> data -> move (x, y);
		if (Factory :: cloneExist (this -> data)) 		{
			this -> data -> move (-x, -y);
			return false;
		}
		areaUpdate (data);
		return true;
	}
	void printItem () {
		cout << name << ":" << endl;
		this -> data -> print ();
	}
	static void printList () {
		if (!Factory :: top) {
			cout << "List is emply!" << endl;
			return;
		}

		Factory * elem = Factory :: top;
		while (elem) {
			elem -> printItem ();
			elem = elem -> next;
		}
	}
	static bool isEmpty () {
		return ! (bool) top;
	}
	static bool cloneExist (Figure * elem) {
		if (!Factory :: top)
			return false;


		Factory * tmp = Factory :: top;
		while (tmp) {
			if ((tmp -> data != elem) && (*tmp -> data == *elem))
				return true;
			tmp = tmp -> next;
		}

		/*			//В итоге все разрулено полиморфными операторами проверки равенства
		if (Circle * circle = dynamic_cast <Circle *> (elem))					//Если elem - объект типа Circle, то приведение
			while (tmp) {														//вернет указатель на Circle, иначе nullptr
				if ((circle != tmp -> data) && (*circle == *tmp -> data))		//важно, чтобы circle при сравнении был слева!
					return true;												//иначе будет вызвано сравнение для Figure
				tmp = tmp -> next;												//которое не знает о кругах...
			}
		else																	//если объект - не Circle
			while (tmp) {
				if ((tmp -> data != elem) && (*tmp -> data == *elem))
					return true;
				tmp = tmp -> next;
			}
		*/
		return false;
	}
	static Factory * find (char str []) {
		Factory * tmp = Factory :: top;
		while (tmp)	{
			if (!strcmp (tmp -> name, str))
				return tmp;
			tmp = tmp -> next;
		}
		return nullptr;
	}
	static bool findIntersect (Factory * elem) {
		Factory * tmp = Factory :: top;
		bool flag = false;
		while (tmp)	{
			if ((elem -> data != tmp -> data) && (tmp -> data -> isIntersect (elem -> data))) {
				tmp -> printItem ();
				flag = true;
			}
			tmp = tmp -> next;
		}
		return flag;
	}
	static vector <Polygon> getIntersect (Factory * elem, Factory * elem2) {
		//M_shape.getIntersects (&rectangle);
		//elem -> data -> getIntersects (elem2 -> data)
		Polygon * p1 = dynamic_cast <Polygon *> (elem -> data);
		Polygon * p2 = dynamic_cast <Polygon *> (elem2 -> data);
		if (!p1 || !p2) {
			cout << "Sorry, only polygons supported ^_^" << endl;
			return vector <Polygon> ();
		}

		return p1 -> getIntersects (p2);
		
	}
	static void findAssets (Factory * elem) {
		Factory * tmp = Factory :: top;

		if (Circle * circle = dynamic_cast <Circle *> (elem -> data)) {
			while (tmp)	{
				if ((circle != tmp -> data) && (circle -> isInclude (tmp -> data)))
					tmp -> printItem ();
				tmp = tmp -> next;
			}
		}
		else {
			while (tmp)	{
				if ((elem -> data != tmp -> data) && (elem -> data -> isInclude (tmp -> data)))
					tmp -> printItem ();
				tmp = tmp -> next;
			}
		}
	}
	static void showCenters () {
		Factory * elem = Factory :: top;
		if (!elem) {
			cout << "List is emply!" << endl;
			return;
		}

		while (elem) {
			cout << elem -> name << ": ";
			elem -> data -> getCenter ().print ();
			cout << endl;
			elem = elem -> next;
		}
	}
	static double centerDistance (Factory * elem1, Factory * elem2) {
		if (elem1 && elem2)
			return elem1 -> data -> getCenter ().dist (elem2 -> data -> getCenter ());
		else return 0;
	}
	static void showSquares () {
		Factory * elem = Factory :: top;
		if (!elem) {
			cout << "List is emply!" << endl;
			return;
		}

		while (elem) {
			cout << elem -> name << ": " << elem -> data -> getSquare () << endl;
			elem = elem -> next;
		}
	}
	static void showPerimeters () {
		Factory * elem = Factory :: top;
		if (!elem) {
			cout << "List is emply!" << endl;
			return;
		}

		while (elem) {
			cout << elem -> name << ": " << elem -> data -> getPerimeter () << endl;
			elem = elem -> next;
		}
	}
	static void draw () {
		if (Factory :: isEmpty ()) {
			cout << "List is emply!" << endl;
			return;
		}

		const double HEIGHT = COORD_Y_MAX - COORD_Y_MIN;
		const double WIDTH = COORD_X_MAX - COORD_X_MIN;
		const double padding = 20;

		sf::RenderWindow window (sf::VideoMode (HEIGHT, WIDTH), "All Figures");
		window.setFramerateLimit (5);
		double scale = (abs (area [0] - area [2]) > abs (area [1] - area [3])) ?
			(HEIGHT - 2 * padding) / abs (area [0] - area [2]) :
			(WIDTH - 2 * padding) / abs (area [1] - area [3]);
		Dot moveTo (- area [3] + padding / scale, - area [2] + padding / scale);

		while (window.isOpen ()) {

			sf::Event event;

			while (window.pollEvent (event))
				if (event.type == sf::Event::Closed)
					window.close ();

			window.clear ();

			Factory :: draw_ (window, scale, moveTo);

			window.display ();
		}
	}
};

Factory * Factory :: top = nullptr;
double Factory :: area [] = {-DBL_MAX, -DBL_MAX, DBL_MAX, DBL_MAX};	//↑ → ↓ ←		//DBL_*** - константы. Максимум и минимум для double

class Menu {
	Menu * next;
	char * name;
	void (__cdecl * func)(void);
public:
	Menu () : next (nullptr), name (nullptr), func (nullptr) {}
	Menu (char _name [], void (__cdecl * _func)(void)): func (_func), next (nullptr) {
		name = new char [strlen (_name) + 1];
		strcpy (name, _name);
	}
	~Menu () {
		delete [] name;
		if (next)
			next -> ~Menu ();
	}
	void add (char _name [], void (__cdecl * _func)(void)) {
		if (!name) {
			name = new char [strlen (_name) + 1];
			strcpy (name, _name);
			func = _func;
		}
		else if (!next)
			next = new Menu (_name, _func);
		else
			next -> add (_name, _func);
	}
	void show (int n = 0) {
		cout << setw (3) << n << ' ' << name << endl;
		if (next)
			next -> show (n + 1);
	}
	void run (int n) {
		if (n < 0)
			return;
		else if (n == 0)
			func ();
		else if (next)
			next -> run (n - 1);
		else
			return;
	}
};


struct Actions {
private:
	Actions ();
	template <class T>
	static void protectInput (T & val, const T min, const T max, const char msg [] = "") {
		while (true) {
			cin.clear ();
			cin.sync ();
			cout << msg << endl;
			cin >> val;

			if (cin.fail ())
				cout << "Input type error" << endl;
			else if (val < min)
				cout << "Too small value" << endl;
			else if (val > max)
				cout << "Too big value" << endl;
			else break;
		}
	}
	static Factory * chooseElement () {
		cout << "Select item" << endl;
		Factory :: printList ();
		if (Factory :: isEmpty ())
			return nullptr;

		char name [32];
		Factory * factory;
		do {
			cin >> name;
			factory = Factory :: find (name);
			if (!factory)
				cout << "Element not found!" << endl;
		} while (! factory);
		return factory;
	}
	static Dot dotInput () {
		double x, y;
		protectInput (x, COORD_X_MIN, COORD_X_MAX, "Enter the X coordinate");
		protectInput (y, COORD_Y_MIN, COORD_Y_MAX, "Enter the Y coordinate");
		return Dot (x, y);
	}
	static Polygon * polygonConstructor (const int angle) {
		Dot * dots;
		dots = new Dot [angle];
		for (int i = 0; i < angle; i++) {
			cout << "Dot#" << i << ':' << endl;
			dots [i] = dotInput ();
		}
		
		Polygon * polygon = new Polygon (angle, dots);
		delete [] dots;
		return polygon;
	}
	template <class T>
	static Polygon * polygonConstructor () {
		/*Polygon * polygon = polygonConstructor (T :: angleNumber);
		return new T (*polygon);*/
		//Dot * dots;
		Dot * dots = new Dot [T :: angleNumber];
		for (int i = 0; i < T :: angleNumber; i++) {
			cout << "Dot#" << i << ':' << endl;
			dots [i] = dotInput ();
		}
		//Polygon * polygon = new Polygon (T :: angleNumber, dots);
		Polygon * polygon = new T (dots);
		delete [] dots;
		return polygon;
		//return new Polygon (5, Dot (1, 0), Dot (2, 1.7), Dot (3, 0), Dot (3, 2), Dot (1, 2));
	}
public:
	static void addFigure () {
		vector <char *> typelist;
		typelist.push_back ("Polygon");			//0
		typelist.push_back ("Triangle");		//1
		typelist.push_back ("Trapeze");			//2
		typelist.push_back ("Parallelorgam");	//3
		typelist.push_back ("Rectangle");		//4
		typelist.push_back ("Rhombus");			//5
		typelist.push_back ("Quadrate");		//6
		typelist.push_back ("Pentagon");		//7
		typelist.push_back ("Circle");			//8
		typelist.push_back ("Ellipse");			//9

		cout << "Type list:" << endl;
		for (int i = 0; i < typelist.size (); i++)
			cout << setw (3) << i << ' ' << typelist [i] << endl;

		int type;
		protectInput (type, 0, int (typelist.size () - 1), "Choose figure type");

		char name [32];
		cout << "Input Figure's name" << endl;
		cin >> name;
		if (Factory :: find (name)) {
			cout << "This name is already taken" << endl;
			return;
		}

		Figure * figure;
		switch (type) {
			case 0:
				protectInput (type, 3, 100, "How much angle you want?");
				figure = polygonConstructor (type);
				break;
			case 1:	{			//Triangle
				figure = polygonConstructor <Triangle> ();
				break;
			}
			case 2: {			//Trapeze
				cout << "Enter the coordinates of the center" << endl;
				Dot center = dotInput ();
				double widthTop, widthBottom, height, rotate;
				protectInput (widthTop, 0.1, 100., "Enter top width");
				protectInput (widthBottom, 0.1, 100., "Enter bottom width");
				protectInput (height, 0.1, 100., "Enter height");
				protectInput (rotate, 0., 360., "Enter rotate angle");
				figure = new Trapeze (center, widthTop, widthBottom, height, rotate);
				break;
			}
			case 3: {			//Parallelogram
				cout << "Enter the coordinates of the center" << endl;
				Dot center = dotInput ();
				double width, height, rotateSide, rotateAngle;
				protectInput (width, 0.1, 100., "Enter width");
				protectInput (height, 0.1, 100., "Enter height");
				protectInput (rotateSide, 0., 360., "Enter angle between sides");
				protectInput (rotateAngle, 0., 360., "Enter rotate angle");
				figure = new Parallelogram (center, width, height, rotateSide, rotateAngle);
				break;
			}
			case 4: {			//Rectangle
				cout << "Enter the coordinates of the center" << endl;
				Dot center = dotInput ();
				double width, height, rotateAngle;
				protectInput (width, 0.1, 100., "Enter width");
				protectInput (height, 0.1, 100., "Enter height");
				protectInput (rotateAngle, 0., 360., "Enter rotate angle");
				figure = new Rectangle (center, width, height, rotateAngle);
				break;
			}
			case 5: {			//Rhombus
				//Dot& center, double height, double width, double angle)
				cout << "Enter the coordinates of the center" << endl;
				Dot center = dotInput ();
				double width, height, rotateAngle;
				protectInput (width, 0.1, 100., "Enter width");
				protectInput (height, 0.1, 100., "Enter height");
				protectInput (rotateAngle, 0., 360., "Enter rotate angle");
				figure = new Rhombus (center, height, width, rotateAngle);

				break;
			}
			case 6:	{			//Quadrate
				cout << "Enter the coordinates of the center" << endl;
				Dot center = dotInput ();
				double side;
				int degree;
				protectInput (side, 0.1, 100., "Enter the length of the side");
				protectInput (degree, -180, 180, "Enter the angle of rotation");
				figure = new Quadrate (center, side, degree);
				break;
			}
			case 7:	{			//Pentagon
				figure = polygonConstructor <Pentagon> ();
				break;
			}
			case 8:	{			//Circle
				cout << "Enter the coordinates of the center" << endl;
				Dot center = dotInput ();
				double radius;
				protectInput (radius, .1, 100., "Enter the radius of the circle");
				figure = new Circle (center, radius);
				break;
			}
			case 9: {
				Dot * dots = new Dot [2];
				for (int i = 0; i < 2; i++) {
					cout << "Enter the coordinates of focus #" << i << endl;
					dots [i] = dotInput ();
				}
				double radius;
				protectInput (radius, .1, 100., "Enter the radius of the ellipse");
				figure = new Ellipse (dots [0], dots [1], radius);
				delete [] dots;
				break;
			}
		}
		try {
			new Factory (figure, name);
		}
		catch (char err []) {
			cout << err << endl;
			delete figure;
		}

	}
	
	static void addSomeFigures () {
		try {
			new Factory (new Quadrate (Dot (1, 1), sqrt (2.), 45), "first");
			new Factory (new Triangle (Dot (1, 1), Dot (1, 5), Dot (3.5, 3)), "tri1");
			new Factory (new Triangle (Dot (2, 4), Dot (6, 4), Dot (2, 8)), "tri2");
			new Factory (new Pentagon (Dot (4.5, .5), Dot (8, .5), Dot (8, 2.5), Dot (5.5, 6), Dot (4.5, 2.5)), "pent1");
			new Factory (new Pentagon (Dot (4.5, 7), Dot (8, 7), Dot (8, 9), Dot (5.5, 12.5), Dot (4.5, 9)), "pent2");
			new Factory (new Circle (Dot (5, 5), 2), "circ1");
			new Factory (new Circle (Dot (6, 9), 1), "circ2");
			new Factory (new Ellipse (Dot (8, 7), Dot (5, 2), 2), "Ellipse");
			new Factory (new Rectangle (Dot (6, 9), 1, 1.4, 32), "Rectangle");
			new Factory (new Rhombus (Dot (10, 4), 5, 3, 8), "Rhombus");
			new Factory (new Parallelogram (Dot (10, 6), 2, 3, 45, 0), "Parallel");
		}
		catch (char err []) {
			cout << err << endl;
		}
	}
	static void centersDistance () {
		cout << "First figure:" << endl;
		Factory * elem1 = Actions :: chooseElement ();
		system ("cls");


		cout << "Second figure:" << endl;
		Factory * elem2 = Actions :: chooseElement ();
		system ("cls");

		if (elem1 && elem2)
			cout << "Distance from centers = " << Factory :: centerDistance (elem1, elem2) << endl;
	}
	static void removeFigure () {
		cout << "Select item" << endl;
		Factory :: printList ();
		if (Factory :: isEmpty ())
			return;

		char name [32];
		cin >> name;

		if (Factory :: removeItem (name))
			cout << "Done!" << endl;
		else
			cout << "Element not found" << endl;
	}
	static void myExit () {
		exit (EXIT_SUCCESS);
	}
	static void move () {
		Factory * elem = Actions :: chooseElement ();
		if (!elem)
			return;
		double x, y;
		protectInput (x, COORD_X_MIN, COORD_X_MAX, "Enter the X coordinate");
		protectInput (y, COORD_Y_MIN, COORD_Y_MAX, "Enter the Y coordinate");
		if (elem -> move (x, y))
			cout << "Done!" << endl;
		else
			cout << "Can't move" << endl;
		
	}
	static void intersect () {
		Factory * elem = Actions :: chooseElement ();

		if (!elem)
			return;
		system ("cls");
		cout << elem -> getName () << " intersects with:" << endl;
		if (!Factory :: findIntersect (elem))
			cout << "No intersects" << endl;
	}
	static void assets () {
		Factory * elem = Actions :: chooseElement ();
		if (!elem)
			return;
		system ("cls");
		Factory :: findAssets (elem);
	}
	static void showAll () {
		Factory :: printList ();
		Factory :: draw ();
	}
	static void getIntersect () {
		cout << "First figure:" << endl;
		Factory * elem1 = Actions :: chooseElement ();
		system ("cls");


		cout << "Second figure:" << endl;
		Factory * elem2 = Actions :: chooseElement ();
		system ("cls");

		if (!elem1 || !elem2)
			return;

		vector <Polygon> intersects = Factory :: getIntersect (elem1, elem2);

		if (intersects.size () > 0) {
			const double HEIGHT = COORD_Y_MAX - COORD_Y_MIN;
			const double WIDTH = COORD_X_MAX - COORD_X_MIN;
			const double padding = 20;

			sf::RenderWindow window (sf::VideoMode (HEIGHT, WIDTH), "Intersects");
			window.setFramerateLimit (5);

			double * area = Factory :: area;
			double scale = (abs (area [0] - area [2]) > abs (area [1] - area [3])) ?
				(HEIGHT - 2 * padding) / abs (area [0] - area [2]) :
				(WIDTH - 2 * padding) / abs (area [1] - area [3]);
			Dot moveTo (-area [3] + padding / scale, -area [2] + padding / scale);

			while (window.isOpen ()) {

				sf::Event event;

				while (window.pollEvent (event))
					if (event.type == sf::Event::Closed)
						window.close ();

				window.clear ();
				system ("cls");

				for (auto polygon : intersects) {
					polygon.draw (window, scale, moveTo);
					polygon.print ();
					cout << endl;
				}

				window.display ();
			}
		}

	}

	class Test {
	public:
		static void testList () {
			Menu menu;
			menu.add ("Parallelogram input", Test :: parallel);
			menu.add ("Trapeze input", Test :: trapeze);
			menu.add ("Show intersect", Test :: getIntersects);
			menu.add ("Is intersect", Test :: isIntersect);
			int command;
			menu.show ();
			cin >> command;
			system ("cls");
			menu.run (command);
		}
		static void getIntersects () {
			Polygon M_shape = Polygon (5, Dot (1, 0), Dot (2, 1.7), Dot (3, 0), Dot (3, 2), Dot (1, 2));
			M_shape.move (0, .3);
			Polygon rectangle = Polygon (4, Dot (0, .5), Dot (4, .5), Dot (4, 1.5), Dot (0, 1.5));

			Dot move (5, 4);
			Dot walk (-.05, 0);
			sf::RenderWindow window (sf::VideoMode (800, 800), "All Figures");
			double scale = 80;
			window.setFramerateLimit (30);

			while (window.isOpen ()) {
				sf::Event event;
				while (window.pollEvent (event))
					if (event.type == sf::Event::Closed)
						window.close ();
				window.clear ();
				system ("cls");
				M_shape.rotate (0.02);
				M_shape.move (walk);
				walk.rotatePi (.1);
				for (Polygon polygon : M_shape.getIntersects (&rectangle)) {
					polygon.draw (window, scale, Dot (0, 4));
					polygon.print ();
					cout << endl;
				}

				M_shape.draw (window, scale, move);
				rectangle.draw (window, scale, move);

				window.display ();
			}
		}
		static void isIntersect () {
			Line line (Dot (10, 15), Dot (18, -5));
			Triangle tri = Triangle (Dot (1, 1), Dot (1, 5), Dot (3.5, 3));
			Pentagon pent = Pentagon (Dot (4.5, .5), Dot (8, .5), Dot (8, 2.5), Dot (5.5, 6), Dot (4.5, 2.5));
			Circle circ = Circle (Dot (5, 6), .8);
			Dot focus1 = Dot (5, 10);
			Dot focus2 = Dot (15, 25);
			Ellipse ell = Ellipse (focus1, focus2, 4);
			Line line2 = Line (Dot (1, 2), Dot (8, 8));

			sf::RenderWindow window (sf::VideoMode (800, 800), "All Figures");
			double scale = 30;
			window.setFramerateLimit (30);
			while (window.isOpen ()) {
				sf::Event event;
				while (window.pollEvent (event))
					if (event.type == sf::Event::Closed)
						window.close ();

				window.clear ();

				tri.draw (window, scale, Dot ());
				pent.draw (window, scale, Dot ());
				circ.draw (window, scale, Dot ());
				ell.draw (window, scale, Dot ());
				line.rotate (1, line.getCenter ());
				line.draw (window, scale, Dot ());
				line2.move ((line2.getCenter () - line.getBegin ()) * -1);
				line2.rotate (7, line2.getCenter ());
				Ellipse ell2 (line2.getBegin (), line2.getEnd (), 1);
				ell2.draw (window, scale, Dot ());
				ell.draw (window, scale, Dot ());
				system ("cls");
				cout << "Triangle\tPentagon\tCircle\t\tElliopse" << endl << endl;
				if (tri.isIntersect (&ell2))
					cout << "intersect\t";
				else
					cout << "\t\t";
				if (pent.isIntersect (&ell2))
					cout << "intersect\t";
				else
					cout << "\t\t";
				if (circ.isIntersect (&ell2))
					cout << "intersect\t";
				else
					cout << "\t\t";
				if (ell.isIntersect (&ell2))
					cout << "intersect\t";
				else
					cout << "\t\t";
				window.display ();
			}
		}
		static void parallel () {

			sf::RenderWindow window (sf::VideoMode (800, 800), "All Figures");
			int angleRotate = 0;
			int angleSide = 0;
			double scale = 80;
			window.setFramerateLimit (10);
			while (window.isOpen ()) {
				sf::Event event;
				while (window.pollEvent (event))
					if (event.type == sf::Event::Closed)
						window.close ();

				window.clear ();
				//++angleRotate %= 360;
				++angleSide %= 60;
				Parallelogram (Dot (3, 3), 2, 3, 90 - angleSide, angleRotate).draw (window, scale, Dot (2, 2));
				system ("cls");
				cout << "Center: (3, 3), height: 2, width: 3, angleSide: " << 90 - angleSide << ", angleRotate: " << angleRotate << endl;
				window.display ();
			}
		}
		static void trapeze () {

			sf::RenderWindow window (sf::VideoMode (800, 800), "All Figures");
			int angleRotate = 0;
			double widthTop, widthBottom;
			double val = 0;
			double scale = 80;
			window.setFramerateLimit (30);
			while (window.isOpen ()) {
				sf::Event event;
				while (window.pollEvent (event))
					if (event.type == sf::Event::Closed)
						window.close ();

				val += .05;
				widthTop = 5 + sin (val);
				widthBottom = 5 + 2 * cos (val);
				window.clear ();
				++angleRotate %= 360;

				Trapeze trapeze (Dot (3, 3), widthTop, widthBottom, 3, angleRotate);
				trapeze.draw (window, scale, Dot (2, 2));
				system ("cls");
				cout << fixed << setprecision (2);
				cout << "Center: (3, 3), widthTop: " << widthTop << ", widthBottom: " << widthBottom << ", height: 3, angleRotate: " << angleRotate << endl;
				window.display ();
			}
		}
	};
};

void main () {
	Menu menu;
	menu.add ("Add Figure", Actions :: addFigure);
	menu.add ("Add some hardcode figures", Actions :: addSomeFigures);
	menu.add ("Delete Figure", Actions :: removeFigure);
	menu.add ("Move", Actions :: move);
	menu.add ("All intersects", Actions :: intersect);
	menu.add ("Computational intersect", Actions :: getIntersect);
	menu.add ("Assets", Actions :: assets);
	menu.add ("Show all figure", Actions :: showAll);
	menu.add ("Show centers", Factory :: showCenters);
	menu.add ("Show squares", Factory :: showSquares);
	menu.add ("Show perimeters", Factory :: showPerimeters);
	menu.add ("Centers distance", Actions :: centersDistance);
	menu.add ("Tests", Actions :: Test :: testList);
	menu.add ("Exit", Actions :: myExit);
	int command;



	while (true) {
		system ("cls");
		menu.show ();
		cin >> command;
		system ("cls");
		menu.run (command);
		system ("pause");
		cin.clear ();
		cin.sync ();
	}
}