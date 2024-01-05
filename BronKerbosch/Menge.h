#ifndef MENGE_H
#define MENGE_H

#include <iostream>
#include <vector>

using namespace std ;

class Menge
	: public vector< size_t >
{
public:
/***  Konstruktoren  ***/

	// Standardkonstruktor, init. leere Menge
	Menge ()
		: vector< size_t >()
	{ }

	// Initialisierungskonstruktor mit einem Element
	Menge ( size_t const& elem )
		: vector< size_t >()
	{
		(*this).push_back( elem ) ;
	}

	// Initialisierungskonstruktor mit einer aufsteigend sortierten Menge elem
	Menge ( vector< size_t > const& elem )
		: vector< size_t >( elem )
	{ }

/***  Rechenmethoden  ***/

	// ist elem Element des Objekts?
	bool contains ( size_t const& elem ) const
	{
		return find( elem ) != -1 ;
	}

	// füge neuen Eintrag in das Objekt ein
	bool insert ( size_t const& obj ) ;

	// entferne Eintrag aus dem Objekt
	bool remove ( size_t const& obj ) ;

	// füge elem ins Objekt ein, wenn noch nicht vorhanden;
	// gib zurück, ob es neu eingefügt wurde
	bool operator += ( size_t const& elem )
	{
		return insert( elem ) ;
	}

	// füge elem ins Objekt ein, wenn noch nicht vorhanden;
	// gib zurück, ob es neu eingefügt wurde
	bool operator -= ( size_t const& elem )
	{
		return remove( elem ) ;
	}

	// gib Objekt mit hinzugefügtem elem als neues Objekt aus
	Menge operator + ( size_t const& elem ) const
	{
		Menge neu = *this ;
		neu += elem ;
		return neu ;
	}

	// gibt den Index von obj zurück, falls gefunden und sonst -1
	int find( size_t const& elem ) const ;

	// gib Differenzmenge des Objekts mit menge als neues Objekt aus
	Menge differenz ( Menge const& menge ) const ;

	// gib Schnittmenge des Objekts mit menge als neues Objekt aus
	Menge schnitt ( Menge const& menge ) const ;

	// gib Vereinigungsmenge des Objekts mit menge als neues Objekt aus
	Menge vereinigung ( Menge const& menge ) const ;

/***  Ausgabemethoden  ***/

	friend ostream& operator << ( ostream& ostr, Menge const& menge )
	{
		cout << "[" ;
		for ( size_t i = 0 ;  i < menge.size() ;  ++i )
			ostr << " " << menge[ i ] ;

		return ostr << " ]" ;
	}	
} ;

#endif