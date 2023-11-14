#include <iostream>
#include <string>
#include <vector>
#include "FonctionsMenu.h"
using namespace std;
vector <string> TableauContact;
vector <string> TableauFichier;
vector <string> TableauSauvegarde;
string adresse("Contacts.txt"),adresseP("personnel.pdf");
int main()
{
    bool bien(true);
    int continuer(1);
    do
    {
        bien = Menu();
        cout << "Effectuer une autre opération?" << endl << "1. OUI\n2. NON\t";
        cin >> continuer;
        continuer = GerErreur(continuer);
        if(continuer!=1)
            cout << "\n####### Sortie du répertoire..! #######\n" << endl;
    }while (continuer == 1);
    return 0;
}