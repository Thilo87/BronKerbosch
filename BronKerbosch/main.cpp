#include <iostream>
#include <stdexcept>
#include "BronKerbosch.h"
using namespace std ;

int main ( int narg, char* argv[] )
try
{
	if ( narg < 2 || narg > 3 )
	{
		cout << "Usage: " << argv[ 0 ] << "  eingabepfad  [ausgabepfad]" << endl <<
		        "        eingabepfad: Pfad zur .gra-Datei, in der Orte mit" << endl <<
                "                     ihren Beschriftungen und x-/y-Koordinaten stehen" << endl <<
                "        ausgabepfad: Pfad zu einer Datei, in der das Ergebnis der" << endl <<
				"                     Berechnung gespeichert wird" << endl <<
		        "Berechnet Positionen zu moeglichst vielen Beschriftungslabels, so dass sich" << endl <<
				"keine zwei Labels ueberschneiden." << endl ;

		return 0 ;
	}

	BronKerbosch BK( argv[ 1 ] ) ;		 // Daten laden
	BK.start() ;						 // Algorithmus ausführen

	if ( narg == 3 )
		BK.speichernUnter( argv[ 2 ] ) ; // Ergebnis abspeichern

	return 0 ;
}
catch ( exception const& e )
{
	cerr << e.what() << endl ;
	return -1 ;
}
catch ( char const* text )
{
	cerr << text << endl ;
	return -1 ;
}
catch ( ... )
{
	cerr << "Unbekannter Fehler!" << endl ;
	return -1 ;
}