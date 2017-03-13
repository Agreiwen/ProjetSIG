// main.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "tgaimage.h"
#include "point.h"
#include <iostream>
#include <vector>
#include <limits>
#include <sstream>
#include <cmath>
#include <algorithm>


using namespace std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

vector<Point> lectureSommets(string nomFichier) {
	cout << "Lecture des sommets... ";
	ifstream fichier(nomFichier.c_str(), ios::in);
	vector<Point> sommets;
	if (fichier)
	{
		string ligne;
		int nbSommets = 0;
		while (getline(fichier, ligne, '\n'))
		{
			if (ligne[0] == 'v' && ligne[1] == ' ')
			{
				string aux;
				for (unsigned int i = 2; i < ligne.size(); ++i)
				{
					aux.push_back(ligne[i]);
				}
				stringstream ss;
				ss.str(aux);
				double partie1, partie2, partie3;
				ss >> partie1 >> partie2 >> partie3;
				Point p = Point(nbSommets+1 , partie1, partie2, partie3, false);
				sommets.push_back(p);
				nbSommets++;
			}
		}
		cout << "(" << nbSommets << " sommets) ";
		cout << "Succes" << endl;
		return sommets;
	}
	else
		cerr << "Impossible d'ouvrir le fichier !" << endl;
}

vector<vector<int> > lectureTriangles(string nomFichier) {
	cout << "Lecture des triangles... ";
	ifstream fichier(nomFichier.c_str(), ios::in);
	if (fichier)
	{
		string ligne;
		vector<vector<int> > vect;
		vector<int> vectLigne;
		int nbTriangle = 0;
		while (getline(fichier, ligne, '\n'))
		{
			if (ligne[0] == 'f' && ligne[1] == ' ')
			{
				string aux;
				for (unsigned int i = 2; i < ligne.size(); ++i)
				{
					aux.push_back(ligne[i]);
				}
				int posEspace1 = aux.find(' ', 0);
				int posEspace2 = aux.find(' ', posEspace1 + 1);
				string texte1 = aux.substr(0, posEspace1);
				string texte2 = aux.substr(posEspace1 + 1, posEspace2 - (posEspace1 + 1));
				string texte3 = aux.substr(posEspace2 + 1, aux.size() - (posEspace2 + 1));
				int posSlashTexte1 = texte1.find('/', 0);
				int posSlashTexte2 = texte2.find('/', 0);
				int posSlashTexte3 = texte3.find('/', 0);
				int partie1 = atof(texte1.substr(0, posSlashTexte1).c_str());
				int partie2 = atof(texte2.substr(0, posSlashTexte2).c_str());
				int partie3 = atof(texte3.substr(0, posSlashTexte3).c_str());
				vectLigne.push_back(partie1);
				vectLigne.push_back(partie2);
				vectLigne.push_back(partie3);
				vect.push_back(vectLigne);
				nbTriangle++;
			}
			vectLigne.clear();
		}
		cout << "(" << nbTriangle << " triangles) ";
		cout << "Succes" << endl;
		return vect;
	}
	else
		cerr << "Impossible d'ouvrir le fichier !" << endl;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1)) {
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void dessinePoint(vector<Point> vect) {
	cout << "Creation de l'image nuage de point ... ";
	int tailleImage = 500;
	int tailleImageSurDeux = tailleImage / 2;
	TGAImage image(tailleImage, tailleImage, TGAImage::RGB);
	for (unsigned int i = 0; i < vect.size(); ++i) {
		double x = vect[i].x * 100 + tailleImageSurDeux;
		double y = vect[i].y * 100 + tailleImageSurDeux;
		bool lock = vect[i].lock;
		if (lock == true) {
			image.set(x, y, red);
		}
		else {
			image.set(x, y, white);
		}
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("nuage_de_point.tga");
	cout << "Succes" << endl;
}

void dessineFilDeFer(vector<Point> vectPoints, vector<vector<int> > vectTriangles) {
	cout << "Creation de l'image fil de fer ... ";
	int tailleImage = 500;
	int tailleImageSurDeux = tailleImage / 2;
	TGAImage image(tailleImage, tailleImage, TGAImage::RGB);
	for (unsigned int i = 0; i < vectTriangles.size(); ++i) {
		int indice1 = vectTriangles[i][0];
		int x1 = vectPoints[indice1 - 1].x * 100 + tailleImageSurDeux;
		int y1 = vectPoints[indice1 - 1].y * 100 + tailleImageSurDeux;

		int indice2 = vectTriangles[i][1];
		int x2 = vectPoints[indice2 - 1].x * 100 + tailleImageSurDeux;
		int y2 = vectPoints[indice2 - 1].y * 100 + tailleImageSurDeux;

		int indice3 = vectTriangles[i][2];
		int x3 = vectPoints[indice3 - 1].x * 100 + tailleImageSurDeux;
		int y3 = vectPoints[indice3 - 1].y * 100 + tailleImageSurDeux;

		line(x1, y1, x2, y2, image, white);
		line(x1, y1, x3, y3, image, white);
		line(x2, y2, x3, y3, image, white);
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("fil_de_fer.tga");
	cout << "Succes" << endl;
}

vector<int> lectureVerrouille(string nomFichier) {
	cout << "Lecture des sommets verouilles... ";
	ifstream fichier(nomFichier.c_str(), ios::in);
	if (fichier)
	{
		string ligne;
		vector<int> vect;
		int nbPointLock = 0;
		while (getline(fichier, ligne, '\n'))
		{
			if (ligne[0] == '#' && ligne[1] == ' ')
			{
				string aux;
				for (unsigned int i = 9; i < ligne.size(); ++i)
				{
					aux.push_back(ligne[i]);
				}
				int indice = atoi(aux.c_str());
				vect.push_back(indice);
				nbPointLock++;
			}
		}
		cout << "(" << nbPointLock << " sommets verouilles) ";
		cout << "Succes" << endl;
		return vect;
	}
	else
		cerr << "Impossible d'ouvrir le fichier !" << endl;
}

void verouillage(vector<Point> &vectPoints, vector<int> &vectLock) {
	for (int i = 0; i < vectLock.size(); i++)
	{
		for (int j = 0; j < vectPoints.size(); j++)
		{
			if (vectLock[i] == vectPoints[j].indice) {
				vectPoints[j].bloque();
			}
		}
	}
}

void voisinagePoint(vector<Point> &vectPoints, vector<vector<int> > &vectTriangles) {
	cout << "Recuperation des voisins... ";
	for (int i = 0; i < vectTriangles.size(); i++){
		int indice1 = vectTriangles[i][0];
		Point p1 = vectPoints[indice1 - 1];
		int indice2 = vectTriangles[i][1];
		Point p2 = vectPoints[indice2 - 1];
		int indice3 = vectTriangles[i][2];
		Point p3 = vectPoints[indice3 - 1];

		vectPoints[indice1 - 1].ajouterVoisin(p2);
		vectPoints[indice1 - 1].ajouterVoisin(p3);
		vectPoints[indice2 - 1].ajouterVoisin(p1);
		vectPoints[indice2 - 1].ajouterVoisin(p3);
		vectPoints[indice3 - 1].ajouterVoisin(p1);
		vectPoints[indice3 - 1].ajouterVoisin(p2);
	}
	cout << "Succes" << endl;
}

Point barycentreVoisins(Point p) {
	vector<Point> voisins = p.voisins;
	double x = 0;
	double y = 0;
	double z = 0;
	for (int i = 0; i < voisins.size(); i++){
		x += voisins[i].x;
		y += voisins[i].y;
		z += voisins[i].z;
	}
	x = x / voisins.size();
	y = y / voisins.size();
	z = z / voisins.size();
	Point res = Point(p.indice, x, y, z, p.lock);
	return res;
}

vector<Point> lissage(vector<Point> &vectPoints) {
	vector<Point> vectRes;
	for (int i = 0; i < vectPoints.size(); i++){
		if (vectPoints[i].lock == true) {
			vectRes.push_back(vectPoints[i]);
		}
		else {
			vectRes.push_back(barycentreVoisins(vectPoints[i]));
		}
		
	}
	return vectRes;
}
	
int main()
{
	cout << "Projet SIG - M2 Informatique\n" << endl;
	/* Parse du fichier obj */
	vector<Point> vectPoints = lectureSommets("icosa_split3.obj");
	vector<int> vectLock = lectureVerrouille("icosa_split3.obj");
	verouillage(vectPoints, vectLock);
	vector<vector<int> > vectTriangles = lectureTriangles("icosa_split3.obj");
	//dessinePoint(vectPoints);
	//dessineFilDeFer(vectPoints, vectTriangles);
	voisinagePoint(vectPoints, vectTriangles);
	vector<Point> resLissage;
	resLissage = lissage(vectPoints);
	dessinePoint(resLissage);
	dessineFilDeFer(resLissage, vectTriangles);
	system("PAUSE");
	return 0;
}

