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
#include "matrice.h"
#include <iomanip>


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
				Point p = Point(nbSommets + 1, partie1, partie2, partie3, false);
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
				vectPoints[j].verrouille();
			}
		}
	}
}

void voisinagePoint(vector<Point> &vectPoints, vector<vector<int> > &vectTriangles) {
	cout << "Recuperation des voisins... ";
	for (int i = 0; i < vectTriangles.size(); i++) {
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

vector<Point> triPoints(vector<Point> &vectPoints) {
	cout << "On trie les points verrouilles et les libres... ";
	vector<Point> vectRes;
	// Points libres en premier
	int indice = 0;
	for (int i = 0; i < vectPoints.size(); i++) {
		if (vectPoints[i].lock == false) {
			Point p = vectPoints[i];
			vectRes.push_back(p);
			indice++;
		}
	}
	for (int i = 0; i < vectPoints.size(); i++) {
		if (vectPoints[i].lock == true) {
			Point p = vectPoints[i];
			vectRes.push_back(p);
			indice++;
		}
	}
	cout << "Succes" << endl;
	return vectRes;
}

Point barycentreVoisins(Point p) {
	vector<Point> voisins = p.voisins;
	double x = 0;
	double y = 0;
	double z = 0;
	for (int i = 0; i < voisins.size(); i++) {
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
	for (int i = 0; i < vectPoints.size(); i++) {
		if (vectPoints[i].lock == false) {
			vectRes.push_back(vectPoints[i]);
		}
		else {
			vectRes.push_back(barycentreVoisins(vectPoints[i]));
		}

	}
	for (int i = 0; i < vectPoints.size(); i++) {
		if (vectPoints[i].lock == true) {
			vectRes.push_back(vectPoints[i]);
		}
		else {
			vectRes.push_back(barycentreVoisins(vectPoints[i]));
		}

	}
	return vectRes;
}

Matrice remplissageMatriceUnique(vector<Point> vectPoints) {
	cout << "Remplissage de la matrices A... ";
	int nbPointLibre = 0;
	int nbPoints = vectPoints.size();
	while (vectPoints[nbPointLibre].lock == false) {
		nbPointLibre++;
	}
	int nbPointVerrouille = nbPoints - nbPointLibre;
	//cout << "nbPointLibre : " << nbPointLibre << endl;
	//cout << "nbPointVerrouille : " << nbPointVerrouille << endl;
	Matrice res = Matrice(nbPoints, nbPoints);
	for (int l = 0; l < nbPoints; l++) {
		for (int c = 0; c < nbPoints; c++) {
			if (l == c) {
				res(l, c) = -1.0;
			}
			else if (vectPoints[l].aPourVoisin(vectPoints[c])) {
				double rapport = 1.0 / vectPoints[l].nbVoisin();
				res(l, c) = rapport;
			}
			else {
				res(l, c) = 0.0;
			}
		}
	}
	cout << "Succes" << endl;
	return res;
}

void divisionMatrice(vector<Point> vectPoints, Matrice &A, Matrice &Af, Matrice &Al) {
	int limiteFreeLock = 0;
	int nbPoint = vectPoints.size();
	while (vectPoints[limiteFreeLock].lock == false) {
		limiteFreeLock++;
	}
	// Division de A en deux : Af et Al
	// Division de X en deux : Xf et Xl
	Af = Matrice(nbPoint, limiteFreeLock);
	Al = Matrice(nbPoint, nbPoint - limiteFreeLock);
	//Xf = Matrice(limiteFreeLock, 1);
	//Xl = Matrice(nbPoint - limiteFreeLock, 1);
	//cout << limiteFreeLock << endl;
	for (int c = 0; c < A.nb_colones(); c++) {
		for (int l = 0; l < A.nb_lignes(); l++) {
			if (c < limiteFreeLock) {
				Af(l, c) = A(l, c);
				//Xf(c, 0) = X(c, 0);
			}
			else {
				Al(l, c - limiteFreeLock) = A(l, c);
				//Xl(c - limiteFreeLock, 0) = X(c, 0);
			}
		}
	}

}

void affichageMatrice(Matrice &m) {
	for (int l = 0; l < m.nb_lignes(); l++) {
		for (int c = 0; c < m.nb_colones(); c++) {
			cout << setprecision(3) << m(l, c) << "\t";
			//cout << m(l, c) << ",";
		}
		cout << endl;
	}
}

Matrice preparationGauss(Matrice &matrice, Matrice &vecteurConnu) {
	int nbLigne = matrice.nb_lignes();
	int nbColonne = matrice.nb_colones() + vecteurConnu.nb_colones();
	int limite = matrice.nb_colones();
	Matrice res = Matrice(nbLigne, nbColonne);
	for (int c = 0; c < nbColonne; c++){
		for (int l = 0; l < nbLigne; l++){
			if (c < limite) {
				res(l, c) = matrice(l, c);
			}
			else {
				res(l, c) = -vecteurConnu(l, 0);
			}
		}
	}
	return res;
}

void gaussJordan(Matrice &A) {
	Matrice indice = Matrice(A.nb_lignes(), 1);
	for (int l = 0; l < A.nb_lignes(); l++){
		indice(l, 0) = l;
	}
	int n = A.nb_lignes();
	int m = A.nb_colones();
	int r = -1; // r est l'indice de ligne du dernier pivot trouve
	for (int j = 0; j < m-1; j++) // j décrit tous les indices de colonnes (on ne doit pas prendre la derniere)
	{
		double max = A(0, j);
		int k = 0;
		for (int i = 0; i < n; i++){
			if (abs(A(i, j)) > max) {
				max = A(i, j);
				k = i;
			}
		}
		double pivot = A(k, j);// A[k, j] est le pivot
		if (A(k, j) != 0) {
			r = r + 1;// r designe l'indice de la future ligne servant de pivot
			for (int c = 0; c < m; c++) {
				A(k, c) = A(k, c) / pivot;// On normalise la ligne de pivot de façon que le pivot prenne la valeur 1
			}
			if (r != k) {//On place la ligne du pivot en position r
				for (int c = 0; c < m; c++){
					double aux1 = A(r, c);
					A(r, c) = A(k, c);
					A(k, c) = aux1;
				}
				int aux2 = indice(r, 0);
				indice(r, 0) = indice(k, 0);
				indice(k, 0) = aux2;
			}
			for (int i = 0; i < n; i++){
				if (i != r) {
					for (int c = 0; c < m; c++){
						A(i, c) = A(i, c) - (A(r, c)*A(i, j));
					}
				}
			}
		}
	}
	/*affichageMatrice(indice);
	cout << endl;*/
}

void remplissageX(vector<Point> vectPoints, Matrice &Xf, Matrice &Xl) {
	int limiteFreeLock = 0;
	int nbPoint = vectPoints.size();
	while (vectPoints[limiteFreeLock].lock == false) {
		limiteFreeLock++;
	}
	Xf = Matrice(limiteFreeLock, 1);
	Xl = Matrice(nbPoint - limiteFreeLock, 1);
	for (int i = 0; i < vectPoints.size(); i++){
		if (i < limiteFreeLock) {
			Xf(i, 0) = vectPoints[i].x;
		}
		else {
			Xl(i - limiteFreeLock, 0) = vectPoints[i].x;
		}
	}
}

void remplissageY(vector<Point> vectPoints, Matrice &Yf, Matrice &Yl) {
	int limiteFreeLock = 0;
	int nbPoint = vectPoints.size();
	while (vectPoints[limiteFreeLock].lock == false) {
		limiteFreeLock++;
	}
	Yf = Matrice(limiteFreeLock, 1);
	Yl = Matrice(nbPoint - limiteFreeLock, 1);
	for (int i = 0; i < vectPoints.size(); i++) {
		if (i < limiteFreeLock) {
			Yf(i, 0) = vectPoints[i].y;
		}
		else {
			Yl(i - limiteFreeLock, 0) = vectPoints[i].y;
		}
	}
}

void remplissageZ(vector<Point> vectPoints, Matrice &Zf, Matrice &Zl) {
	int limiteFreeLock = 0;
	int nbPoint = vectPoints.size();
	while (vectPoints[limiteFreeLock].lock == false) {
		limiteFreeLock++;
	}
	Zf = Matrice(limiteFreeLock, 1);
	Zl = Matrice(nbPoint - limiteFreeLock, 1);
	for (int i = 0; i < vectPoints.size(); i++) {
		if (i < limiteFreeLock) {
			Zf(i, 0) = vectPoints[i].z;
		}
		else {
			Zl(i - limiteFreeLock, 0) = vectPoints[i].z;
		}
	}
}

Matrice extractionResultat(Matrice &gauss) {
	Matrice res = Matrice(gauss.nb_lignes(), 1);
	for (int l = 0; l < gauss.nb_lignes(); l++){
		res(l, 0) = gauss(l, gauss.nb_colones() - 1);
	}
	return res;
}

Matrice lissageX(vector<Point> vectPoints, Matrice &Af, Matrice &Al, Matrice &matrice) {
	Matrice Xf, Xl;
	remplissageX(vectPoints, Xf, Xl);
	Matrice vecteurConnu = Af.t()*Al*Xl;
	/*affichageMatrice(vecteurConnu);
	cout << endl;*/
	Matrice gauss = preparationGauss(matrice, vecteurConnu);
	//affichageMatrice(gauss);
	gaussJordan(gauss);
	Matrice vecteurInconnu = extractionResultat(gauss);
	return vecteurInconnu;
}

Matrice lissageY(vector<Point> vectPoints, Matrice &Af, Matrice &Al, Matrice &matrice) {
	Matrice Yf, Yl;
	remplissageY(vectPoints, Yf, Yl);
	Matrice vecteurConnu = Af.t()*Al*Yl;
	Matrice gauss = preparationGauss(matrice, vecteurConnu);
	gaussJordan(gauss);
	Matrice vecteurInconnu = extractionResultat(gauss);
	return vecteurInconnu;
}

Matrice lissageZ(vector<Point> vectPoints, Matrice &Af, Matrice &Al, Matrice &matrice) {
	Matrice Zf, Zl;
	remplissageZ(vectPoints, Zf, Zl);
	Matrice vecteurConnu = Af.t()*Al*Zl;
	Matrice gauss = preparationGauss(matrice, vecteurConnu);
	gaussJordan(gauss);
	Matrice vecteurInconnu = extractionResultat(gauss);
	return vecteurInconnu;
}

void lissage(vector<Point> &vectPoints, Matrice &vecteurX, Matrice &vecteurY, Matrice &vecteurZ) {
	for (int i = 0; i < vecteurX.nb_lignes(); i++){
		Point p = Point(vectPoints[i].indice, vecteurX(i,0), vecteurY(i,0), vecteurZ(i,0), vectPoints[i].lock);
		vectPoints[i] = p;
	}
}

int main()
{
	/*ofstream cout("output.txt");   
	ios_base::sync_with_stdio(false);
	std::cout.rdbuf(cout.rdbuf());*/

	cout << "Projet SIG - M2 Informatique\n" << endl;
	/* Parse du fichier obj */
	vector<Point> vectPoints = lectureSommets("test.obj");
	vector<int> vectLock = lectureVerrouille("test.obj");
	verouillage(vectPoints, vectLock);
	vector<vector<int> > vectTriangles = lectureTriangles("test.obj");

	/* Ajout des voisins à chaque point */
	voisinagePoint(vectPoints, vectTriangles);

	/* Tri des points pour avoir une numerotation free puis lock */
	vectPoints = triPoints(vectPoints);

	/* Remplissage de la matrice A */
	Matrice A = remplissageMatriceUnique(vectPoints);
	affichageMatrice(A);
	cout << endl;

	/* Separation de A pour avoir Af et Al */
	Matrice Af, Al;
	divisionMatrice(vectPoints, A, Af, Al);
	/*affichageMatrice(Af);
	cout << endl;
	affichageMatrice(Al);
	cout << endl;*/

	/* Creation de la matrice matrice */
	Matrice matrice = Af.t()*Af;
	/*affichageMatrice(matrice);
	cout << endl;*/

	/* Resolution du systeme lineaire pour la dimension x */
	Matrice vecteurX = lissageX(vectPoints, Af, Al, matrice);

	/* Resolution du systeme lineaire pour la dimension y */
	Matrice vecteurY = lissageY(vectPoints, Af, Al, matrice);

	/* Resolution du systeme lineaire pour la dimension z */
	Matrice vecteurZ = lissageZ(vectPoints, Af, Al, matrice);

	lissage(vectPoints, vecteurX, vecteurY, vecteurZ);

	dessinePoint(vectPoints);
	dessineFilDeFer(vectPoints, vectTriangles);

	system("PAUSE");
	return 0;
}

