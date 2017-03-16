#ifndef POINT_H
#define POINT_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Point
{
public:
	// Constructeurs
	Point(int indice, double x, double y, double z, bool lock);

	// Autres méthodes
	void afficher() const;
	void verrouille();
	bool aPourVoisin(Point p);
	int nbVoisin();
	void ajouterVoisin(Point p);
	void afficherVoisins();
	bool equals(Point p);

public:
	double x, y, z;
	int indice;
	bool lock;
	vector<Point> voisins;
};

#endif