#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Stoppuhr.h"
#include "Menge.h"
#include "Graph.h"

using namespace std ;

/*
	Hauptklasse des Beschriftungsproblems:
	Enthält einen an das Beschriftungsproblem angepassten Bron-Kerbosch-Algorithmus.
	Im Konstruktor wird der Dateipfad zur Input-Datei angegeben, die Datei geladen und der Algorithmus auf den geladenen Daten ausgeführt.
	Mit der Methode speichernUnter() kann das Ergebnis in einer Datei abgespeichert werden.
*/

class BronKerbosch
{
	double					_schriftgroesse ; // Breite und Höhe jedes Buchstaben
	vector< Ort >			_alleOrte ;		  // alle aus den Daten eingelesene Orte

	// für die jeweils unabhängigen Zusammenhangskomponenten ("Ballungszentren")
	vector< Graph >			_graphZHK ;		// Graph der ZHK
	vector< vector< Ort > > _orteZHK ;		// Orte in der ZHK
	vector< Menge >			_maxCliqueZHK ;	// vom BK-Algorithmus berechnete maximum Clique in der ZHK

	// für den Bron-Kerbosch-Algorithmus (_bkMaxClique)
	size_t _bkZHK ;				// Index der ZHK, auf welcher aktuell der Bron-Kerbosch-Algorithmus ausgeführt wird


/***  Erzeugung der Zusammenhangskomponenten  ***/
/*
	Nachdem die Daten eingelesen wurden, wird durch _erzeugeKomponenten der Bounding-Box-Graph erzeugt, d.h.
	ein Graph, der für jeden Ort einen Knoten enthält und in dem es genau dann eine Kante zwischen zwei Knoten gibt,
	wenn die zugehörigen Ortsnamen sich schneiden KÖNNTEN, d.h. wenn die Bounding-Boxen der Orte sich schneiden.

	Mit _tiefensuche werden dann die Zusammenhangskomponenten in dem Bounding-Box-Graphen gefunden und in
	_graphZHK gespeichert.
*/

	// Tiefensuche für das Auffinden von Zusammenhangskomponenten in _erzeugeKomponenten
	void _tiefensuche ( size_t u, vector< bool >& besucht, Graph const& G, vector< Ort >& orteZHK ) ;

	// erzeugt den Bounding-Box-Graphen und findet darin die Zusammenhangskomponenten.
	// Danach sind in _orteZHK die Orte in den einzelnen Zusammenhangskomponenten.
	void _erzeugeKomponenten() ;

/***  Erzeugung der Konfliktgraphen  ***/
/*
	Nachdem durch _erzeugeKomponenten herausgefunden wurde, welche Orte in welcher ZHK liegen, werden die
	eigentlichen Konfliktgraphen, auf denen der Bron-Kerbosch-Algorithmus ausgeführt wird, mit
	_erzeugeGraph erzeugt und in _graphZHK gespeichert.
*/

	// erzeugt den Konfliktgraphen der Zusammenhangskomponente mit dem Index zhk
	void _erzeugeGraph( size_t zhk ) ;

/***  Bron-Kerbosch-Algorithmus  ***/
/*
	Wenn die Konfliktgraphen der ZHKen erzeugt wurden, kann mit _bkMaxClique der Bron-Kerbosch-Algorithmus auf
	einer ZHK ausgeführt werden.
*/

	// Hilfsfunktion, welche die Anzahl der VERSCHIEDENEN Orte in C berechnet. Wird in _bkExtend verwendet.
	size_t _anzVerschiedeneOrte( Menge const& C, Graph const& G ) ;


	// führt den Bron-Kerbosch-Algorithmus auf der ZHK mit dem Index zhk aus, speichert das Ergebnis in _maxCliqueZHK
	void _bkMaxClique( size_t zhk ) ;

	bool _bkExtend( Menge const& Q, Menge F, Menge C ) ;

/***  Laden der Eingabedaten  ***/

	// lädt Orte aus der Datei mit dem Dateinamen dateiname
	void _datenLaden( string const& dateiname ) ;

public:
/***  Konstruktoren  ***/

	// lädt die Ortsbeschriftungen, sowie X- und Y-Koordinaten der Ortspunkte aus der Datei dateiname und führt den Bron
	BronKerbosch( string const& dateiname ) ;

	// führt den Algorithmus auf den geladenen Daten aus
	void start() ;

	// speichert die Ergebnisse in der Datei mit dateiname
	void speichernUnter( string const& dateiname ) ;
} ;