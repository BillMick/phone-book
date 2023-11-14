#ifndef FONCTIONSMENU_H_INCLUDED
#define FONCTIONSMENU_H_INCLUDED
#include <string>
bool Menu();//Fonction menu
bool AfficherTousMesContacts();//Fonction AfficherTousMesContacts
bool AjouterContact();//Fontion AjouterContact
bool RechercherContact(std::string indice);//Fonction RechercherContact
bool ModifierContact(std::string indice);//Fonction ModifierContact
bool SupprimerContact(std::string indice);//Fontion SupprimerContact
void ViderLeRepertoire();//Fonction ViderLeRepertoire
//bool VerifierIndice(int);//
void ReorganiseRepertoire();//Fonction ReorganiseRepertoire
void ParOrdreAlphabetique();//Fonction ParOrdreAlphabetique
int GerErreur(int d);//Fonction GerErreur
bool MotDePasse(std::string motDePasse);//Fonction MotDePasse
bool ParametrerSecurite();
bool OptionsAvancees();
bool SupprimerLesDoublons();
bool VerifierIdentite();
/* class Strowberry
{
    private:
    std::string adresseP("C:/Users/bito/Desktop/privé.pdf");
    public:
    std::string getAdresse() const;
    bool MotDePasse(std::string);
    bool ParametrerSecurite();
}; */
#endif//FONCTIONSMENU_H_INCLUDED