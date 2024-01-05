#include "BronKerbosch.h"

// Tiefensuche für das Auffinden von Zusammenhangskomponenten in _erzeugeKomponenten
void BronKerbosch::_tiefensuche ( size_t u, vector< bool >& besucht, Graph const& G, vector< Ort >& orteZHK )
{
	// markiere u als besucht
	besucht[ u ] = true ;

	// füge den Ort zu _orteZHK hinzu (orteZHK ist eine Referenz auf den entsprechenden Eintrag in _orteZHK)
	orteZHK.push_back( G.ort( u ) ) ;

	vector< size_t > const& nachbarn = G.nachbarn( u ) ;

	// führe Tiefensuche auf der ZHK von u durch und markiere alle Knoten aus der ZHK von u als besucht
	for ( size_t i = 0 ;  i < nachbarn.size() ;  ++i )
	{
		size_t v = nachbarn[ i ] ;
		if ( !besucht[ v ] )
		{
			besucht[ v ] = true ;
			_tiefensuche( v, besucht, G, orteZHK ) ;
		}
	}
}

// erzeugt den Bounding-Box-Graphen und findet darin die Zusammenhangskomponenten.
// Danach sind in _orteZHK die Orte in den einzelnen Zusammenhangskomponenten.
void BronKerbosch::_erzeugeKomponenten()
{	
	Graph bboxGraph ;

	/***  Bounding-Box-Graph erzeugen  ***/

	// füge alle Orte in den Bounding-Box-Graph ein
	for ( size_t i = 0 ;  i < _alleOrte.size() ;  ++i )
		bboxGraph.neuerKnoten( _alleOrte[ i ].x(), _alleOrte[ i ].y(), _alleOrte[ i ] ) ;

	// für je zwei Orte, prüfe ob sich deren Bounding-Boxen schneiden. Wenn JA, füge eine Kante zwischen diesen Orten
	// im Bounding-Box-Graph hinzu
	for ( size_t i = 0 ;  i < _alleOrte.size() ;  ++i )
		for ( size_t j = i + 1 ;  j < _alleOrte.size() ;  ++j )
			if (  !( bboxGraph.x( i ) + _schriftgroesse * _alleOrte[ i ].laenge() <= bboxGraph.x( j ) - _schriftgroesse * _alleOrte[ j ].laenge()
				|| bboxGraph.x( i ) - _schriftgroesse * _alleOrte[ i ].laenge() >= bboxGraph.x( j ) + _schriftgroesse * _alleOrte[ j ].laenge()
				|| bboxGraph.y( i ) + _schriftgroesse <= bboxGraph.y( j ) - _schriftgroesse
				|| bboxGraph.y( i ) - _schriftgroesse >= bboxGraph.y( j ) + _schriftgroesse ) )
				{
					bboxGraph.neueUngerKante( i, j ) ;
				}

		
	/***  Zusammenhangskomponenten des Bounding-Box-Graphs mit Tiefensuche suchen  ***/
	// Der erzeugte Bounding-Box-Graph wird mittels Tiefensuche auf ZHKen durchsucht. Die Orte jeder gefundenen
	// ZHK werden in _orteZHK gespeichert.

	// gibt an, ob ein Knoten im Bounding-Box-Graph schon besucht wurde
	vector< bool > besucht( bboxGraph.anzKnoten(), false ) ;

	for ( size_t s = 0 ;  s < besucht.size() ;  ++s )
		if ( !besucht[ s ] )
		{
			// erstelle neuen leeren Vector für die Orte der ZHK, die in der folgenden
			// Tiefensuche gefunden werden
			_orteZHK.push_back( vector< Ort >() ) ;

			// markiere alle Knoten aus der ZHK vom Knoten s als besucht
			_tiefensuche( s, besucht, bboxGraph, _orteZHK[ _orteZHK.size() - 1 ] ) ;
		}

	// Platz für die Konfliktgraphen der ZHKen bereitstellen, die mit _erzeugeGraph erzeugt werden
	_graphZHK.resize( _orteZHK.size() ) ;

	// Platz für die maximalen Cliquen der ZHKen bereitstellen, die mit _bkMaxClique erzeugt werden
	_maxCliqueZHK.resize( _orteZHK.size() ) ;
}

// erzeugt den Konfliktgraphen der Zusammenhangskomponente mit dem Index zhk
void BronKerbosch::_erzeugeGraph( size_t zhk )
{
	Graph& G = _graphZHK[ zhk ] ;				  // hier wird der Konfliktgraph gespeichert
	vector< Ort > const& orte = _orteZHK[ zhk ] ; // die bereits mit _erzeugeKomponenten berechneten Orte in der ZHK mit dem Index zhk

	// Knoten zum Konfliktgraphen für alle Orte hinzufügen (für jeden Ort werden die Kästchen drum herum geschoben
	// und für jede Kästchen-Position ein Knoten hinzugefügt)
	for ( size_t i = 0 ;  i < orte.size() ;  ++i )
	{
		double x = orte[ i ].x() ;
		for ( size_t j = 0 ;  j < orte[ i ].laenge() + 1 ;  ++j )
		{
			G.neuerKnoten( x, orte[ i ].y(), orte[ i ] ) ;
			G.neuerKnoten( x, orte[ i ].y() - _schriftgroesse, orte[ i ] ) ;
			x -= _schriftgroesse ;
		}
	}

	// Kanten zum Konfliktgraphen hinzufügen
	for ( size_t i = 0 ;  i < orte.size() ;  ++i )
	{
		vector< size_t > const& knotenOrt1 = G.knoten( orte[ i ] ) ; // Knotenmenge, die zum ersten Ort gehört
		for ( size_t j = i + 1 ;  j < orte.size() ;  ++j )
		{
			vector< size_t > const& knotenOrt2 = G.knoten( orte[ j ] ) ; // Knotenmenge, die zum zweiten Ort gehört

			// wenn die Kästchen von zwei Knoten der Knotenmengen der beiden Orte sich NICHT schneiden,
			// füge Kante zum Konfliktgraphen hinzu
			for ( vector<size_t>::const_iterator k = knotenOrt1.begin() ;  k != knotenOrt1.end() ;  ++k )
				for ( vector<size_t>::const_iterator l = knotenOrt2.begin() ;  l != knotenOrt2.end() ;  ++l )
					if ( G.x( *k ) + _schriftgroesse * orte[ i ].laenge() <= G.x( *l )
						|| G.x( *k ) >= G.x( *l ) + _schriftgroesse * orte[ j ].laenge()
						|| G.y( *k ) + _schriftgroesse <= G.y( *l )
						|| G.y( *k ) >= G.y( *l ) + _schriftgroesse )
						{
							G.neueUngerKante( *k, *l ) ;
						}			
		}
	}
}

// Hilfsfunktion, welche die Anzahl der VERSCHIEDENEN Orte in C berechnet. Wird in _bkExtend verwendet.
size_t BronKerbosch::_anzVerschiedeneOrte( Menge const& C, Graph const& G )
{
	if ( C.empty() )
		return 0 ;

	size_t anzGefundenerOrte = 1 ;
	size_t aktIndex = G.ort( C[ 0 ] ).index() ;

	// Die Knoten in C, welche zum selben Ort gehören, stehen immer nebeneinander, weil bei der Erzeugung der Konfliktgraphen
	// die Knoten desselben Ortes hintereinander eingefügt werden, d.h. vom Knotenindex her aufsteigend und C auch eine aufsteigend sortierte
	// Menge ist. Daher kann man die Anzahl der verschiedenen Orte in C wie folgt berechnen.
	for ( size_t i = 1 ;  i < C.size() ;  ++i )
		if ( G.ort( C[ i ] ).index() != aktIndex )
		{
			aktIndex = G.ort( C[ i ] ).index() ;
			++anzGefundenerOrte ;
		}

	return anzGefundenerOrte ;
}

// führt den Bron-Kerbosch-Algorithmus auf der ZHK mit dem Index zhk aus, speichert das Ergebnis in _maxCliqueZHK
void BronKerbosch::_bkMaxClique( size_t zhk )
{
	_bkZHK = zhk ;
	_bkExtend( Menge(), Menge(), _graphZHK[ _bkZHK ].knoten() ) ;
}

bool BronKerbosch::_bkExtend( Menge const& Q, Menge F, Menge C )
{
	Graph const& G = _graphZHK[ _bkZHK ] ;

	// Anzahl der verschiedenen Orte in C: |C|_{Orte}
	size_t anzVerschiedeneOrte = _anzVerschiedeneOrte( C, G ) ;

	// Schranke: |Q| + |C|_{Orte} <= |Q_{max}|
	if ( Q.size() + anzVerschiedeneOrte <= _maxCliqueZHK[ _bkZHK ].size() )
		return true ;

	// maximale Clique gefunden
	if ( F.size() == 0 && C.size() == 0 )
	{
		// ist die maximale Clique größer als die bisher gefundene maximale Clique?
		if ( Q.size() > _maxCliqueZHK[ _bkZHK ].size()  )
		{
			_maxCliqueZHK[ _bkZHK ] = Q ; // Update
			cout << "Clique mit " << _maxCliqueZHK[ _bkZHK ].size() << " Orten gefunden!" << endl ;

			// Wenn alle Orte der ZHK eine Beschriftung haben, ist eine maximum Clique gefunden
			// und der Algorithmus kann abgebrochen werden
			if ( _maxCliqueZHK[ _bkZHK ].size() == _orteZHK[ _bkZHK ].size() )
				return false ; 
		}
			
		// Algorithmus weiter ausführen, es könnte noch größere maximale Cliquen geben
		return true ;
	}

	if ( C.empty() )
		return true ;

	// Im Folgenden: Komplizierte Minimierung von |C\N(v)| für v aus CuF

	size_t min = C.size() + 1 ; // argmin
	size_t u = 0 ;				// min
		
	for ( size_t i = 0 ;  i < C.size() + F.size() ;  ++i )
	{
		// damit nicht über beide Mengen C und F einzeln gelaufen werden muss (C und F sind disjunkt, daher könnte man
		// eigentlich auch über C minimieren und dann darauf nochmal über F minimieren, gibt aber viel redundanten Code)

		size_t w ; // Knoten aus CuF, der aktuell betrachtet wird
		if ( i >= C.size() )
			w = F[ i - C.size() ] ;
		else
			w = C[ i ] ;

		Menge const& Nw = G.nachbarn( w ) ; // Nachbarmenge N(w) des aktuell betrachteten Knotens w aus CuF
		size_t anzNachbarnInC = 0 ;			// Anzahl der Nachbarn von w in C
		
		// nun wird die Nachbarmenge N(w) durchlaufen. Wenn C einen dieser Knoten enthält, wird |C\N(w)| um 1 kleiner
		// und die Anzahl der Nachbarn von w in C um 1 größer. Ist |C| - anzNachbarnInC < min, ist bereits ein Minimum
		// gefunden worden und die Schleife kann abgebrochen werden.
		for ( size_t j = 0 ;  j < Nw.size() ;  ++j )
			if ( C.contains( Nw[ j ] ) )
			{
				++anzNachbarnInC ;
				if ( C.size() - anzNachbarnInC < min )
				{
					u = w ; // min
					min = C.size() - anzNachbarnInC ;
					break ;
				}
			}

		// falls obige Schleife überhaupt nicht ausgeführt wird, weil Nw leer ist oder
		// die Anzahl der Nachbarn von w in C gleich 0 ist. In beiden Fällen ist anzNachbarnInC gleich 0.
		if ( C.size() < min )
		{
			u = w ;
			min = C.size() ;
		}

		// kleiner kann |C\N(v)| nicht werden
		if ( min == 0 )
			break ;
	}


	// bilde C\N(u), laufe über diese Menge und erweitere ggf. Q
	Menge R = C.differenz( G.nachbarn( u ) ) ;

	for ( size_t i = 0 ;  i < R.size() ;  ++i )
	{
		// _maxCliqueZHK[ _bkZHK ] ist hier lokal nicht konstant (_maxCliqueZHK ist global und kann sich bei einem Aufruf von _bkExtend ändern).
		// Es kann mit _bkExtend maximal eine Clique mit der Kardinalität |Q| + |C|_{Orte} gefunden werden.
		// Wurde im weiteren Abstieg des Suchbaumes solch eine Clique gefunden, sind hier keine weiteren Aufrufe von _bkExtend mehr
		// nötig. Zwar würde _bkExtend sowieso durch die Schranke vorzeitig abgebrochen werden, man spart sich aber die Schnittmengen und die 
		// erneute Berechnung der Anzahl der verschiedenen Orte in C, wenn man direkt hier abbricht.
		if ( Q.size() + anzVerschiedeneOrte > _maxCliqueZHK[ _bkZHK ].size() )
		{
			// Erweiterung von Q
			if ( !_bkExtend( Q + R[ i ],
					            F.schnitt( G.nachbarn( R[ i ] ) ),
								C.schnitt( G.nachbarn( R[ i ] ) )
								) )
				return false ; // Abbruch des gesamten Algorithmus
			
			F += R[ i ] ;
			C -= R[ i ] ;
		}
	}
		
	return true ;
}

// lädt Orte aus der Datei mit dem Dateinamen dateiname
void BronKerbosch::_datenLaden( string const& dateiname )
{
	fstream fin ;
	fin.open( dateiname, ios::in ) ;

	string s ;
	if ( fin )
	{
		size_t anzahlOrte ;
		fin >> anzahlOrte ;

		getline( fin, s ) ;
		fin >> _schriftgroesse ;
			
		getline( fin, s ) ;
		getline( fin, s ) ;
		getline( fin, s ) ;
		getline( fin, s ) ;
			
		double x ;
		double y ;
		string name ;

		for ( size_t i = 0 ;  i < anzahlOrte ;  ++i )
		{
			fin >> s ;
			fin >> x ;
			fin >> y ;
			fin >> name ;
			_alleOrte.push_back( Ort( name, x, y, _alleOrte.size() ) ) ;
		}

		fin.close() ;
	}
	else
		throw "void BronKerbosch::_datenLaden(): Eingabedatei konnte nicht geoffnet werden." ;
}

// lädt die Ortsbeschriftungen, sowie X- und Y-Koordinaten der Ortspunkte aus der Datei dateiname und führt den Bron
BronKerbosch::BronKerbosch( string const& dateiname )
{
	Stoppuhr uhr ;
		
	cout << endl ;
	cout << "*************** Eingabedaten ***************" << endl ;

	uhr.start() ;
	_datenLaden( dateiname ) ;
	cout << "Eingabedaten gelesen in " << uhr.stopp() << " s:" << endl << endl ;

	cout << "Schriftgroesse " << _schriftgroesse << endl << endl ;

	cout << _alleOrte.size() << " Orte:" << endl << endl ;
	cout << "Name (Laenge) / (x, y)" << endl ;
	for ( size_t i = 0 ;  i < _alleOrte.size() ;  ++i )
		cout << _alleOrte[ i ] << endl ;

	cout << endl ;
}

// führt den Algorithmus auf den geladenen Daten aus
void BronKerbosch::start() {
	Stoppuhr uhr ;

	cout << "*************** Zusammenhangskomponenten ***************" << endl ;

	uhr.start() ;
	_erzeugeKomponenten() ;

	cout << _orteZHK.size() << " ZHKen in " << uhr.stopp() << " s gefunden:" << endl << endl ;

	cout << "Name (Laenge) / (x, y)" << endl ;
	for ( size_t i = 0 ;  i < _orteZHK.size() ;  ++i )
	{
		cout << (i+1) << ". ZHK (" << _orteZHK[ i ].size() << " Orte):" << endl ;
		for ( size_t j = 0 ;  j < _orteZHK[ i ].size() ;  ++j )
			cout << _orteZHK[ i ][ j ] << endl ;
		cout << endl ;
	}

	cout << "*************** Konfliktgraphen ***************" << endl ;

	for ( size_t i = 0 ;  i < _orteZHK.size() ;  ++i )
	{
		uhr.start() ;
		_erzeugeGraph( i ) ;
		cout << "Konfliktgraph fuer " << (i+1) << ". ZHK in " << uhr.stopp() << " s mit "
				<< _graphZHK[ i ].anzKnoten() << " Knoten und " << _graphZHK[ i ].anzKanten() << " Kanten erzeugt." << endl ;
	}

	cout << endl ;
	cout << "*************** Algorithmus ***************" << endl ;

	double dauerGesamt = 0 ;
	for ( size_t i = 0 ;  i < _orteZHK.size() ;  ++i )
	{
		cout << "Starte Bron-Kerbosch-Algorithmus fuer " << (i+1) << ". ZHK..." << endl ;
		uhr.start() ;
		_bkMaxClique( i ) ;
			
		double dauer = uhr.stopp() ;
		cout << "Algorithmus fuer " << (i+1) << ". ZHK beendet in " << dauer << " s." << endl << endl ;
		dauerGesamt += dauer ;
	}

	cout << "*************** Ergebnis ***************" << endl ;
	size_t s = 0 ;
	for ( size_t i = 0 ;  i < _maxCliqueZHK.size() ;  ++i )
		s += _maxCliqueZHK[ i ].size() ;

	cout << "Insgesamt Clique fuer " << s << " von " << _alleOrte.size() << " Orten in " << dauerGesamt << " s gefunden:" << endl << endl ;

	cout << "Name --> obere linke Ecke (x, y)" << endl ;
	for ( size_t i = 0 ;  i < _maxCliqueZHK.size() ;  ++i )
	{
		for ( Menge::iterator j = _maxCliqueZHK[ i ].begin() ;  j != _maxCliqueZHK[ i ].end() ;  ++j )
			cout << _graphZHK[ i ].ort( *j ).name() << " --> ( " << _graphZHK[ i ].x( *j ) << ", " << _graphZHK[ i ].y( *j ) << " )" << endl ;
	}
	cout << endl ;
}

// speichert die Ergebnisse in der Datei mit dateiname
void BronKerbosch::speichernUnter( string const& dateiname )
{
	if ( _maxCliqueZHK.size() != 0 )
	{
		ofstream fout( dateiname ) ;

		if ( fout )
		{
			fout << _schriftgroesse << " Schriftgroesse\n" ;
			fout << "\n" ;
			fout << "Name   x-Koord   y-Koord   x-Urspr   y-Urspr\n" ;
			for ( size_t i = 0 ;  i < _maxCliqueZHK.size() ;  ++i )
			for ( Menge::iterator it = _maxCliqueZHK[ i ].begin() ;  it != _maxCliqueZHK[ i ].end() ;  ++it )
			{
				Ort knotenort = _graphZHK[ i ].ort( *it ) ;
				fout << knotenort.name() << " " << _graphZHK[ i ].x( *it ) << " " << _graphZHK[ i ].y( *it ) << " "
						<< knotenort.x() << " " << knotenort.y() << endl ;
			}

			fout.close() ;
		}
		else
			throw "void BronKerbosch::speichernUnter(): Ausgabedatei konnte nicht geoeffnet oder erzeugt werden." ;
	}

	cout << "Ergebnis gespeichert unter " << dateiname << "." << endl ;
}