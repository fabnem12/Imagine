#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

int dim1 = 10;
int nbPixelsX = round(2 * 8 * dim1);
int nbPixelsY = round(2 * 8 * dim1);

std::complex<double> fonction(std::complex<double>& nb) {
    //fonction proposée sur Wikipedia pour illustrer la coloration de régions, à modifier à la guise de l'utilisateur !
    std::complex<double> image = (std::pow(nb, 2) - 1.0) * std::pow(nb - std::complex<double>(2, 1), 2) / (std::pow(nb, 2) + std::complex<double>(2, 2));

    return image;
}

std::string coul(std::complex<double>& nb) {
    //double norme = norm(nb); //On laisse tomber la norme, pourquoi pas l'intégrer d'une façon ou d'une autre à l'avenir...
    double argument = arg(nb);

    int r = round(127.5 * (cos(argument) + 1));
    int g = round(127.5 * (sin(argument) + 1));
    int b = 255 - r;

    return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
}

void ecritPGM(std::vector<std::vector<std::complex<double> > >& matrice) {
    std::ofstream fichier;
    fichier.open("output.pgm");

    //initialisation du fichier .pgm
    std::string affi = "P3\n" + std::to_string(nbPixelsX) + " " + std::to_string(nbPixelsY) + "\n";
    affi += "255\n";

    for (std::vector<std::complex<double> >& ligne : matrice) {
        for (std::complex<double>& nb : ligne)
            affi += coul(nb) + "\n";
        affi += "\n";
    }

    fichier << affi;
    fichier.close();
}

int main() {
    std::vector<std::vector<std::complex<double> > > matrice = std::vector<std::vector<std::complex<double> > >(nbPixelsY, std::vector<std::complex<double> >(nbPixelsX, std::complex<double>(0, 0)));

    for (double im = 0; im < nbPixelsY; im++)
        for (double re = 0; re < nbPixelsX; re++) {
            std::complex<double> nb = std::complex<double>((re - nbPixelsX/2) / dim1, (im - nbPixelsY/2) / dim1);
            matrice[im][re] = fonction(nb);
        }

    ecritPGM(matrice);
}
