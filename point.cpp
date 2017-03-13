#include "stdafx.h"

#include "Point.h"
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

Point::Point(int indice, double x, double y, double z, bool lock) : x(x), y(y), z(z), indice(indice), lock(lock)
{}

void Point::bloque()
{
	lock = true;
}

void Point::afficher() const
{
	cout << "Indice : " << this-> indice << " (" << this->x << "," << this->y << "," << this->z << ")" << " lock : " << this->lock << endl;
}

void Point::ajouterVoisin(Point p)
{
	//p.afficher();
	bool trouve = false;
	for each (Point point in this->voisins){
		trouve += (point.x == p.x && point.y == p.y && point.z == p.z);
	}
	if (!trouve) {
		this->voisins.push_back(p);
	}
}

void Point::afficherVoisins()
{
	cout << "Voisins : ";
	for (int i = 0; i < voisins.size(); i++){
		cout << voisins[i].indice << ", ";
	}
	cout << endl;
}

bool Point::equals(Point p)
{
	return (this->x == p.x && this->y == p.y && this->z == p.z);
}
