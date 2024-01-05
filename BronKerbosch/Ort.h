#ifndef ORT_H
#define ORT_H

#include <iostream>
using namespace std ;

/*
	Klasse, welche die wesentlichen Eigenschaften eines Ortes speichert.
*/

class Ort
{
	double _x ;		// x-Koordinate
	double _y ;		// y-Koordinate

	string _name ;	// Name des Ortes (Beschriftung)
	size_t _index ; // jeder Ort bekommt einen eindeutigen Index (ID), damit spätere Vergleiche weniger aufwändig sind

public:
/***  Konstruktoren  ***/

	// Initialisierungskonstruktor
	Ort ( string const& name, double x, double y, size_t index )
		: _name( name ),
		  _x( x ),
		  _y( y ),
		  _index( index )
	{ }

/***  get-Methoden  ***/

	// Name (Beschriftung) des Objekts
	string const& name() const
	{
		return _name ;
	}

	// x-Koordinate des Objekts
	double x() const
	{
		return _x ;
	}

	// y-Koordinate des Objekts
	double y() const
	{
		return _y ;
	}

	// Anzahl Buchstaben der Beschriftung des Objekts
	size_t laenge() const
	{
		return _name.size() ;
	}

	// Index (ID) des Objekts
	size_t index() const
	{
		return _index ;
	}

/***  Vergleichsmethoden  ***/
	bool operator == ( Ort const& ort ) const
	{
		return _index == ort._index ;
	}

/***  Ausgabemethoden  ***/
	friend ostream& operator << ( ostream& ostr, Ort const& ort )
	{
		return ostr << ort._name.c_str() << " (" << ort.laenge() << ") " << " / ( " << ort._x << ", " << ort._y << " )" ;
	}
} ;

#endif 