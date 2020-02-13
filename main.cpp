#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

int multipli = 5;

int dim1 = 30*multipli;
//une différence de dim1 pixels est traduite par une différence de 1, en partie imaginaire comme réelle
// avec dim1 = 30, le pixel (0,0) vaut 0, (30,0) vaut 1, (0,30) vaut i, (30,30) vaut 1+i, etc.
int nbPixelsX = 750*multipli; //largeur en pixels de l'image
int nbPixelsY = 800*multipli; //hauteur en pixels de l'image

bool couleursInit = true; //laisser à true pour avoir des couleurs plus jolies

//pour le moment, l'image est forcément centrée sur 0, je vais essayer de permettre de choisir le centre de l'image

std::complex<double> cosComplexe(std::complex<double> nb) {
    return (std::exp(std::complex<double>(0,1) * nb) + std::exp(std::complex<double>(0,-1) * nb)) / 2.;
}

std::complex<double> sinComplexe(std::complex<double> nb) {
    return (std::exp(std::complex<double>(0,1) * nb) - std::exp(std::complex<double>(0,-1) * nb)) / std::complex<double>(0,2);
}

std::complex<double> fonction(std::complex<double>& nb) {
    //fonction proposée sur Wikipedia pour illustrer la coloration de régions, à modifier à la guise de l'utilisateur !
    //std::complex<double> image = (std::pow(nb, 2) - 1.0) * std::pow(nb - std::complex<double>(2, 1), 2) / (std::pow(nb, 2) + std::complex<double>(2, 2));
    //std::complex<double> image = std::exp(nb) - std::exp(nb - 1.0) * std::pow(std::complex<double>(0, 1), nb);
    //std::complex<double> image = (nb != 0.0) ? std::pow(std::sin(nb), nb) : 0;
    /*std::complex<double> image = 0; //fonction de Weierstrass
    for (int n = 0; n < 50; n++) {
        std::complex<double> interne = pow(2 + 3 * M_PI, n) * M_PI * nb;
        std::complex<double> cosCalc = cosComplexe(interne / norm(interne));

        image += pow(0.5, n) * cosCalc / norm(cosCalc);
    }*/
    std::complex<double> image = std::exp(cos(nb));
    /*for (int n = 1; n <= 50; n++) { //fonction zeta de Riemann
        image += 1. / std::pow(n, nb);
    }*/
    //std::complex<double> image = std::pow(sinComplexe(nb * nb), std::complex<double>(0,1)); //TRES TRES SYMPA

    //std::complex<double> image = std::pow(sinComplexe(nb *nb), std::complex<double>(0,1));
    //std::complex<double> image = log(nb);

    if (std::isnan(real(image))) {
        image = 0;
    }

    return image;
}

std::string couleurStr(std::complex<double>& nb) {
    //double norme = norm(nb); //On laisse tomber la norme, pourquoi pas l'intégrer d'une façon ou d'une autre à l'avenir...
    double argument = arg(nb);
    int r, g, b;

    if (couleursInit) {
        r = round(127.5 * (cos(argument) + 1));
        g = round(127.5 * (sin(argument) + 1));
        b = 255 - r;
    } else {
        float distR = std::pow(std::pow(cos(argument) - cos(0), 2) + std::pow(sin(argument) - sin(0), 2), 0.5);
        float distJ = std::pow(std::pow(cos(argument) - cos(- 1 * M_PI / 3), 2) + std::pow(sin(argument) - sin(- 1 * M_PI / 3), 2), 0.5);
        float distG = std::pow(std::pow(cos(argument) - cos(- 2 * M_PI / 3), 2) + std::pow(sin(argument) - sin(- 2 * M_PI / 3), 2), 0.5);
        float distC = std::pow(std::pow(cos(argument) - cos(- 3 * M_PI / 3), 2) + std::pow(sin(argument) - sin(- 3 * M_PI / 3), 2), 0.5);
        float distB = std::pow(std::pow(cos(argument) - cos(- 4 * M_PI / 3), 2) + std::pow(sin(argument) - sin(- 4 * M_PI / 3), 2), 0.5);
        float distM = std::pow(std::pow(cos(argument) - cos(- 5 * M_PI / 3), 2) + std::pow(sin(argument) - sin(- 5 * M_PI / 3), 2), 0.5);

        r = round(255 - 255*std::min(1.f, distR) + 255 - 255*std::min(1.f, distJ) + 255 - 255*std::min(1.f, distM));
        g = round(255 - 255*std::min(1.f, distG) + 255 - 255*std::min(1.f, distJ) + 255 - 255*std::min(1.f, distC));
        b = round(255 - 255*std::min(1.f, distB) + 255 - 255*std::min(1.f, distM) + 255 - 255*std::min(1.f, distC));
    }

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
            affi += couleurStr(nb) + "\n";
        affi += "\n";
    }

    fichier << affi;
    fichier.close();
}

int main() {
    std::vector<std::vector<std::complex<double> > > matrice = std::vector<std::vector<std::complex<double> > >(nbPixelsY, std::vector<std::complex<double> >(nbPixelsX, std::complex<double>(0, 0)));
    std::vector<int> centre = {nbPixelsX / 2, nbPixelsY / 2};
    //centre = {nbPixelsX / 2, nbPixelsY / 2};

    for (double im = 0; im < nbPixelsY; im++)
        for (double re = 0; re < nbPixelsX; re++) {
            std::complex<double> nb = std::complex<double>((re - centre[0]) / dim1, (im - centre[1]) / dim1);
            matrice[im][re] = fonction(nb);
        }

    ecritPGM(matrice);
}
