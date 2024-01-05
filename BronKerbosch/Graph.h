#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include "Ort.h"
#include "Menge.h"
using namespace std ;

/*
	Graph-Klasse:
	Repräsentiert einen Graphen, in dem sich Knoten befinden, die Index, x-Koordinate, y-Koordinate und einen Ort als Attribute haben.
	Ein Knoten repräsentiert ein Kästchen der Breite und Höhe wie in der Hauptklasse (Schriftgröße) mit OBERER LINKER ECKE an den Koordinaten
	(_x, _y). Die Knoten werden anhand ihres eindeutigen Index identifiziert. Der Ort gibt an, zu welchem Ort (z.B. Berlin) der jeweilige Knoten
	gehört, da für jeden einzelnen Buchstaben ein Knoten erzeugt wird.
	Die Nachbarschaft eines Knotens wird aus der Hauptklasse BronKerbosch heraus erzeugt und enthält alle Knoten (bzw. deren Indizes
	mit denen sich die Kästchen des Knotens NICHT schneiden.
*/

class Graph
{
	Menge						_knoten ;	// Knoten des Graphen: _knoten[ knotenindex ]
	vector< double >			_x ;		// _x[ knotenindex ]: x-Koordinate des Knotens mit knotenindex
	vector< double >			_y ;		// _y[ knotenindex ]: y-Koordinate des Knotens mit knotenindex
	vector< Ort >				_ort ;		// _ort[ knotenindex ]: Ort, dem der Knoten mit knotenindex zugeordnet ist

	vector< Menge >	_nachbarn ;				// _nachbarn[ knotenindex ] enthält die Knotenindizes der Nachbarn vom Knoten mit knotenindex

public:
/***  Konstruktoren  ***/

	// Standardkonstruktor
	Graph () { } ;

/***  get-Methoden  ***/

	// gibt die Anzahl der Knoten im Graphen zurück
	size_t anzKnoten() const
	{
		return _knoten.size() ;
	}

	// gibt die Anzahl der Kanten im Graphen zurück
	size_t anzKanten() const ;

	// gibt die Nachbarmenge zum Knotenindex knotenindex zurück
	Menge const& nachbarn( size_t knotenindex ) const
	{
		return _nachbarn.at( knotenindex ) ;
	}


	// gibt alle Knoten des Graphen zurück
	Menge const& knoten() const
	{
		return _knoten ;
	}

	// gibt die x-Koordinate des Knotens mit knotenindex zurück
	double x( size_t knotenindex ) const
	{
		return _x.at( knotenindex ) ;
	}

	// gibt die y-Koordinate des Knotens mit knotenindex zurück
	double y( size_t knotenindex ) const
	{
		return _y.at( knotenindex ) ;
	}

	// gibt den Ort des Knotens mit knotenindex zurück
	Ort const& ort( size_t knotenindex ) const
	{
		return _ort.at( knotenindex ) ;
	}

	// gibt alle Knoten zurück, die dem Ort ort zugeordnet sind
	Menge knoten( Ort const& ort ) const ;

/***  set-Methoden  ***/

	// fügt einen neuen Knoten zum Graphen hinzu
	void neuerKnoten( double x, double y, Ort const& ort ) ;

	// fügt eine neue ungerichtete Kante vom Knoten knoten1 zum Knoten knoten2 zum Graphen hinzu
	void neueUngerKante( size_t knoten1, size_t knoten2 ) ;

	// löscht Kanten und Knoten des Graphen
	void clear() ;

/***  Ausgabemethoden  ***/
	friend ostream& operator << ( ostream& ostr, Graph const& G )
	{
		ostr << "Anzahl Knoten: " << G.anzKnoten() << endl ;
		ostr << "Anzahl Kanten: " << G.anzKanten() << endl << endl ;

		ostr << "Ort / Knotenindex / (X, Y) / Nachbarn" << endl ;
		for ( size_t i = 0 ;  i < G._knoten.size() ;  ++i )
		{
			ostr << G._ort[ i ].name().c_str() << " / " << G._knoten[ i ] << " / (" << G._x[ i ] << ", " << G._y[ i ] << ") / " ;
			for ( size_t j = 0 ;  j < G._nachbarn[ i ].size() ;  ++j )
				ostr << G._nachbarn[ i ][ j ] << " " ;
			ostr << endl ;
		}

		return ostr ;
	}
} ;
#endif // GRAPH_H