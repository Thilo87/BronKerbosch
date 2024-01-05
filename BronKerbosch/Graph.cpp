#include "Graph.h"

// gibt die Anzahl der Kanten im Graphen zurück
size_t Graph::anzKanten() const
{
	size_t s = 0 ;
	for ( size_t i = 0 ;  i < _nachbarn.size() ;  ++i )
		s += _nachbarn[ i ].size() ;

	return s / 2 ;
}

// gibt alle Knoten zurück, die dem Ort ort zugeordnet sind
Menge Graph::knoten( Ort const& ort ) const
{
	Menge knoten ;
	for ( size_t i = 0 ;  i < anzKnoten() ;  ++i )
		if ( _ort[ i ] == ort )
			knoten.insert( _knoten[ i ] ) ;

	return knoten ;
}

// fügt einen neuen Knoten zum Graphen hinzu
void Graph::neuerKnoten( double x, double y, Ort const& ort )
{
	_knoten.insert( anzKnoten() ) ;
	_x.push_back( x ) ;
	_y.push_back( y ) ;
	_nachbarn.push_back( Menge() ) ;
	_ort.push_back( ort ) ;
}

// fügt eine neue ungerichtete Kante vom Knoten knoten1 zum Knoten knoten2 zum Graphen hinzu
void Graph::neueUngerKante( size_t knoten1, size_t knoten2 )
{
	_nachbarn.at( knoten1 ).insert( knoten2 ) ;
	_nachbarn.at( knoten2 ).insert( knoten1 ) ;
}

// löscht Kanten und Knoten des Graphen
void Graph::clear()
{
	_knoten.clear() ;
	_x.clear() ;
	_y.clear() ;
	_ort.clear() ;
	_nachbarn.clear() ;
}