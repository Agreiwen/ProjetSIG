#include "stdafx.h"

#include "matrice.h"

void
Matrice::echange(double &a, double &b) const
{
	a += b;
	b = a - b;
	a -= b;
}

Matrice::Matrice()
{
	n = 0;
	m = 0;
	lignes = NULL;
}

Matrice::Matrice(unsigned int nl, unsigned int nc)
{
	n = nl;
	m = nc;
	lignes = new ligne[n];  // allocations
	for (unsigned int i = 0; i<n; i++)
	{
		lignes[i] = new double[m];
		for (unsigned int j = 0; j<m; j++)
			lignes[i][j] = 0;  //initialisation à '0'
	}
}

Matrice::Matrice(const Matrice &source)
{
	n = source.n;
	m = source.m;

	lignes = new ligne[n];
	for (unsigned int i = 0; i<n; i++)
	{
		lignes[i] = new double[m];
		for (unsigned int j = 0; j<m; j++)
			lignes[i][j] = source.lignes[i][j]; //recopie
	}

}

Matrice::Matrice(unsigned int nl, unsigned int nc, double valeur)
{
	n = nl;
	m = nc;

	lignes = new ligne[n];
	for (unsigned int i = 0; i<n; i++)
	{
		lignes[i] = new double[m];
		for (unsigned int j = 0; j<m; j++)
			lignes[i][j] = valeur; //initialisation à 'valeur'
	}
}

Matrice::~Matrice()
{
	for (unsigned int i = 0; i<n; i++) //destruction
		delete[] lignes[i];

	delete[] lignes;
}

Matrice &
Matrice::operator=(const Matrice &mat)
{
	if (this != &mat) //assignation M = M impossible
	{
		if (mat.n != n || mat.m != m) // si tailles pas compatibles
		{
			for (unsigned int i = 0; i<n; i++) // on détruit...
				delete[] lignes[i];
			delete[] lignes;

			n = mat.n;
			m = mat.m,

				lignes = new ligne[n];   // on réalloue
			for (unsigned int i = 0; i<n; i++)
				lignes[i] = new double[m];
		}

		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<m; j++)
				lignes[i][j] = mat.lignes[i][j]; // et on copie...
	}
	return *this;
}

double &
Matrice::operator()(unsigned int i, unsigned int j)
{
	if (i> n || j > m) // si out of range exception
	{
		Erreur a(Erreur::notinmat);
		throw (a);
	}
	return lignes[i][j];
}

Matrice
Matrice::operator+(const Matrice &mat) const
{
	Matrice tmp(n, m);
	for (unsigned int i = 0; i<n; i++) // Double boucle.
		for (unsigned int j = 0; j<m; j++)
			tmp.lignes[i][j] = lignes[i][j] + mat.lignes[i][j]; //addition

	return tmp;
}

Matrice
Matrice::operator-(const Matrice &mat) const
{
	Matrice tmp(n, m);
	for (unsigned int i = 0; i<n; i++) // Double boucle.
		for (unsigned int j = 0; j<m; j++)
			tmp.lignes[i][j] = lignes[i][j] - mat.lignes[i][j]; //soustraction

	return tmp;
}

Matrice
Matrice::operator*(const double nb) const
{
	Matrice tmp(n, m);
	for (unsigned int i = 0; i<n; i++) // Double boucle.
		for (unsigned int j = 0; j<m; j++)
			tmp.lignes[i][j] = lignes[i][j] * nb; // produit

	return tmp;
}

Matrice
Matrice::operator*(const Matrice &mat) const
{
	Matrice tmp(n, mat.m, 0);
	if (m == mat.n) // si nb_colone == nb_ligne de l'autre matrice...
	{
		for (unsigned int i = 0; i<n; i++) //pour chq ligne...
			for (unsigned int j = 0; j<mat.m; j++) // pour chaque colone
			{
				for (unsigned int k = 0; k<m; k++) // produit scalaire
					tmp.lignes[i][j] += lignes[i][k] * mat.lignes[k][j];
			}
	}
	else //multiplication impossible, exception
	{
		Erreur err(Erreur::multiplication);
		throw(err);
	}
	return tmp;
}

Matrice
operator*(const double nb, const Matrice &mat)
{
	return mat*nb;
}

Matrice &
Matrice::operator*=(const double nb)
{
	Matrice tmp(n, m, 0);
	for (unsigned int i = 0; i<n; i++) // Double boucle.
		for (unsigned int j = 0; j<m; j++)
			tmp.lignes[i][j] = lignes[i][j] * nb; //produit

	*this = tmp;

	return *this;
}

Matrice &
Matrice::operator*=(const Matrice &mat)
{
	Matrice tmp(n, mat.m, 0);
	if (m == mat.n) //si multiplication possible....
	{
		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<mat.m; j++)
			{
				for (unsigned int k = 0; k<m; k++)
					tmp.lignes[i][j] += lignes[i][k] * mat.lignes[k][j];
			}
		*this = tmp;
	}
	else //...sinon exception
	{
		Erreur err(Erreur::multiplication);
		throw(err);
	}

	return *this;
}

Matrice &
Matrice::operator+=(const Matrice &mat)
{
	for (unsigned int i = 0; i<n; i++) // Double boucle.
		for (unsigned int j = 0; j<m; j++)
			lignes[i][j] += mat.lignes[i][j]; // +=

	return *this;
}

Matrice &
Matrice::operator-=(const Matrice &mat)
{
	for (unsigned int i = 0; i<n; i++) // Double boucle.
		for (unsigned int j = 0; j<m; j++)
			lignes[i][j] -= mat.lignes[i][j]; //-=

	return *this;
}

Matrice
Matrice::operator/(const double nb) const
{
	Matrice tmp = *this;
	if (nb)
	{
		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<n; j++)
				tmp.lignes[i][j] /= nb;
	}
	else //exception division par 0
	{
		Erreur a(Erreur::dbzero);
		throw(a);
	}
	return tmp;
}

Matrice &
Matrice::operator/=(const double nb)
{
	if (nb)
	{
		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<n; j++)
				lignes[i][j] /= nb;
	}
	else //exception division par 0
	{
		Erreur a(Erreur::dbzero);
		throw(a);
	}
	return *this;
}

Matrice
Matrice::t()
{
	Matrice tmp(m, n);
	for (unsigned i = 0; i<n; i++)
		for (unsigned j = 0; j<m; j++)
			tmp.lignes[j][i] = lignes[i][j];
	return tmp;
}

double
Matrice::trace() const
{
	double sum = 0;
	if (n == m) // trace => matrice carrée
	{
		for (unsigned int i = 0; i<n; i++)
			for (unsigned int j = 0; j<m; j++)
				if (i == j)
					sum += lignes[i][j];
	}
	else //pas carrée => exception
	{
		Erreur a(Erreur::ncarre);
		throw(a);
	}
	return sum;
}

Matrice
Matrice::supligne(const Matrice &mat, unsigned int lig, unsigned int col) const
{
	unsigned int dim = mat.n;
	unsigned int ld = 0, cd = 0;
	Matrice dest(mat.n - 1, mat.n - 1, 0); // -1 ligne et -1 colone
	for (unsigned int L = 0; L<dim; L++)
	{
		cd = 0; // revient a colone 0
		if (L != lig) // sauf ligne a virer
		{
			for (unsigned int c = 0; c<dim; c++)
				if (c != col) // sauf colone a virer
					dest.lignes[ld][cd++] = mat.lignes[L][c]; // recopie
			ld++;
		}
	}
	return dest;
}

float
Matrice::expo(unsigned int n) const
{
	if (!(n % 2)) { return (1); }
	return (-1);
}

double
Matrice::det(const Matrice &mat) const
{
	Matrice m2(mat.n, mat.n, 0);
	double x = 0;

	if (mat.n == 1) //final
		return mat.lignes[0][0];

	for (unsigned int i = 0; i<mat.n; i++)
	{
		m2 = supligne(mat, i, 0); //extrait la sous matrice
		x = x + (expo(i)*mat.lignes[i][0] * det(m2));
	}
	return x;
}

double
Matrice::det()const
{
	if (n != m) // si matrice pas carrée exception
	{
		Erreur a(Erreur::ncarre);
		throw(a);
	}
	return det(*this);
}

ostream &
operator<<(ostream &os, const Matrice &mat)
{
	for (unsigned int i = 0; i<mat.n; i++)
	{
		for (unsigned int j = 0; j<mat.m; j++)
			os << mat.lignes[i][j] << " ";

		os << endl;
	}

	return os;
}

Matrice
Matrice::co() const
{
	Matrice m2(n, m, 0);
	Matrice ret(n, m, 0);
	if (n != m) // si pas carré...
	{
		Erreur a(Erreur::ncarre);
		throw(a);
	}
	if (n == 1)
	{
		ret.lignes[0][0] = 1;
	}
	else
	{
		for (unsigned int i = 0;i<n;i++)
			for (unsigned int j = 0;j<n;j++)
			{
				m2 = supligne(*this, i, j);// sous_matrice
				ret.lignes[i][j] = expo(i + j)*m2.det();
			}
	}
	return ret;
}



Matrice
Matrice::inverse() const
{
	Matrice inv(n, m, 0);
	if (n != m) // si pas carrée
	{
		Erreur a(Erreur::ncarre);
		throw(a);
	}
	inv = (this->co().t()) / (this->det());

	return inv;
}

int
Matrice::dim() const
{
	if (n != m) // si pas carrée
	{
		Erreur a(Erreur::ncarre);
		throw(a);
	}
	return (n == m) ? static_cast<int>(n) : -1;
}

unsigned int
Matrice::nb_colones() const
{
	return m;
}

unsigned int
Matrice::nb_lignes() const
{
	return n;
}