#include "Menge.h"

// füge neuen Eintrag in das Objekt ein
bool Menge::insert ( size_t const& obj )
{
	if ( contains( obj ) )
		return false ;

	(*this).push_back( 0 ) ;
		
	// Einträge um eine Position nach hinten verschieben und obj an richtiger Stelle einfügen
	for ( size_t i = size() - 1 ;  i > 0 ;  --i )
	{
		(*this)[ i ] = (*this)[ i - 1 ] ;
		if ( (*this)[ i - 1 ] < obj )
		{
			(*this)[ i ] = obj ; // Eintrag einfügen
			return true ;
		}
	}

	(*this)[ 0 ] = obj ;
	return true ;
}

// entferne Eintrag aus dem Objekt
bool Menge::remove ( size_t const& obj )
{
	int index = find( obj ) ;

	// obj ist nicht in der Menge vorhanden
	if ( index == -1 )
		return false ;

	// obj entfernen und Elemente aufschieben
	for ( size_t i = index ;  i < size() - 1 ;  ++i )
		(*this)[ index ] = (*this)[ index + 1 ] ;
		
	// letzten (leeren bzw. unbedeutenden) Eintrag löschen
	(*this).pop_back() ;

	return true ;
}

// gibt den Index von obj zurück, falls gefunden und sonst -1
int Menge::find( size_t const& elem ) const
{
	// binäre Suche

	int i = 0 ;				// linke Intervallgrenze
	int j = size() - 1 ;	// rechte Intervallgrenze

	while ( i <= j )
	{
		size_t m = (i+j)/2 ; // Intervallmitte

		if ( (*this)[ m ] == elem )
			return m ;

		// verschiebe Intervallgrenze
		if ( (*this)[ m ] < elem )
			i = m + 1 ;
		else
			j = m - 1 ;
	}		

	return -1 ;
}

// gib Differenzmenge des Objekts mit menge als neues Objekt aus
Menge Menge::differenz ( Menge const& menge ) const
{
	if ( menge.empty() )
		return (*this) ;

	if ( (*this).empty() )
		return Menge() ;

	Menge neu ;

	// i und j sind Indizes im Objekt (i) bzw. in menge (j)
	size_t i = 0 ;
	size_t j = 0 ;

	// solange die Indizes nicht beim Ende der einen oder der anderen Menge angekommen sind...
	while ( i != (*this).size() && j != menge.size() )
	{
		// wenn das i-te Element vom Objekt kleiner als das j-te Element von menge ist,
		// kann es in menge nicht mehr vorkommen (wgn. der aufsteigenden Sortierung von Menge) => es wird durch 
		// die Differenzbildung nicht entfernt, d.h. ist in der Differenzmenge auch noch enthalten
		if ( (*this)[ i ] < menge[ j ] )
		{
			neu.push_back( (*this)[ i ] ) ;
			++i ; // gehe zum nächsten Element im Objekt
		}
		// wenn das i-te Element vom Objekt größer als das j-te Element von menge ist, kann das j-te Element
		// von menge im Objekt nicht vorkommen => gehe bei menge zum nächsten Element
		else if ( (*this)[ i ] > menge[ j ] )
		{
			++j ;
		}
		// wenn das i-te Element vom Objekt gleich dem j-ten Element von menge ist, kommt dieses Element in
		// der Differenzmenge nicht vor => gehe beim Objekt und menge zum nächsten Element
		else
		{
			++i ;
			++j ;
		}
	}

	// alle restlichen Elemente in menge sind größer als das letzte Element im Objekt => können bei der Differenzbildung
	// so oder so nicht wegfallen
	if ( i == (*this).size() )
		return neu ;

	// alle restlichen Element im Objekt anfügen
	if ( j == menge.size() )
		for ( ;  i < (*this).size() ;  ++i )
			neu.push_back( (*this)[ i ] ) ;

	return neu ;
}

// gib Schnittmenge des Objekts mit menge als neues Objekt aus
Menge Menge::schnitt ( Menge const& menge ) const
{
	if ( menge.empty() || (*this).empty() )
		return Menge() ;

	Menge neu ;

	// i und j sind Indizes im Objekt (i) bzw. in menge (j)
	size_t i = 0 ;
	size_t j = 0 ;

	// solange die Indizes nicht beim Ende der einen oder der anderen Menge angekommen sind...
	while ( i != (*this).size() && j != menge.size() )
	{
		// wenn das i-te Element vom Objekt kleiner als das j-te Element von menge ist,
		// kann es im Schnitt nicht vorkommen => gehe ein Element im Objekt weiter
		if ( (*this)[ i ] < menge[ j ] )
			++i ;
		// wenn das i-te Element vom Objekt größer als das j-te Element von menge ist,
		// kann das j-te Element von menge im Schnitt nicht vorkommen => gehe ein Element in menge weiter
		else if ( (*this)[ i ] > menge[ j ] )
			++j ;
		// ist das i-te Element des Objekts gleich dem j-ten Element von menge, ist dieses Element in der Schnittmenge
		else
		{
			neu.push_back( (*this)[ i ] ) ;
			++i ;
			++j ;
		}
	}

	return neu ;
}

// gib Vereinigungsmenge des Objekts mit menge als neues Objekt aus
Menge Menge::vereinigung ( Menge const& menge ) const
{
	if ( menge.empty() )
		return (*this) ;
		
	if ( (*this).empty() )
		return menge ;

	Menge neu ;

	// i und j sind Indizes im Objekt (i) bzw. in menge (j)
	size_t i = 0 ;
	size_t j = 0 ;

	// solange die Indizes nicht beim Ende der einen oder der anderen Menge angekommen sind...
	while ( i != (*this).size() && j != menge.size() )
	{
		// wenn das i-te Element vom Objekt kleiner als das j-te Element von menge ist,
		// kommt es in der Vereinigung vor => füge es hinzu und gehe ein Element weiter
		if ( (*this)[ i ] < menge[ j ] )
		{
			neu.push_back( (*this)[ i ] ) ;
			++i ;
		// wenn das i-te Element vom Objekt größer als das j-te Element von menge ist,
		// kommt das j-te Element von menge in der Vereinigung vor => füge es hinzu und gehe in menge ein Element weiter
		} else if ( (*this)[ i ] > menge[ j ] )
		{
			neu.push_back( menge[ j ] ) ;
			++j ;
		// wenn das i-te Element vom Objekt gleich dem j-ten Elemente von Menge ist,
		// ist es (nur einmal) in der Vereinigung enthalten => füge es hinzu und gehe im Objekt und menge ein Element weiter
		} else
		{
			neu.push_back( (*this)[ i ] ) ;
			++i ;
			++j ;
		}
	}

	// ggf. Reste von menge hinzufügen
	if ( i == (*this).size() )
		for ( ;  j < menge.size() ;  ++j )
			neu.push_back( menge[ j ] ) ;

	// ggf. Reste vom Objekt hinzufügen
	if ( j == menge.size() )
		for ( ;  i < (*this).size() ;  ++i )
			neu.push_back( (*this)[ i ] ) ;

	return neu ;
}