#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include "FonctionsMenu.h"
using namespace std;
extern vector <string> TableauContact;
extern vector <string> TableauFichier;
extern vector <string> TableauSauvegarde;
extern string adresse, adresseP;
//Fonction Menu
bool Menu()
{
    int choix(0),continuer(1),k(0);
    string indice(" "),ligne(" ");
    bool bien(true);
    cout << "########### MENU ###########\n\n1. Afficher tous mes contacts\n2. Ajouter Contact\n"
    "3. Rechercher Contact\n4. Modifier Contact\n5. Supprimer Contact\n6. Options avancées\n7. Quitter" << endl;
    cin >> choix;
    choix = GerErreur(choix);
    switch (choix)
    {
        case 1:
        {
            bien = AfficherTousMesContacts();
            if(!bien)
                cerr << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
            break;
        }
        case 2:
        {
            int ajout(1);
            do
            {
                bien = AjouterContact();
                if(!bien)
                    cerr << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
                else
                {
                    cout << "####### NOUVEAU CONTACT ENREGISTRE #######" << endl << "Voulez vous enregistrer un nouveau contact ?\n1. OUI\t2. NON\t";
                    cin >> ajout;
                    ajout = GerErreur(ajout);
                }
            }while (ajout == 1);
            ParOrdreAlphabetique();
            break;
        }
        case 3:
        {
            do
            {
                cout << "Indice de recherche (Nom, Prenom ou Numero):\t";
                getline(cin,indice);
                bien = RechercherContact(indice);
                if (TableauContact.empty() && bien)
                    cout << "CONTACT INEXISTANT !" << endl;
                else if (!bien)
                    cerr << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
                else 
                {
                    cout << "\n####### CONTACT(s) RETROUVE(s): #######" << endl;
                    for (int i = 0; i < TableauContact.size(); i++)
                        cout << i+1 << ". " << TableauContact[i] << endl;
                }
                cout << "\nRechercher un autre contact?\n1. OUI\t2. NON\t";
                cin >> continuer;
                continuer = GerErreur(continuer);
            }while (continuer == 1);
            break;
        }
        case 4:
        {
            do
            {
                cout << "Indice de recherche du contact à modifier (Nom, Prenom ou Numero):\t";
                getline(cin,indice);
                bien = ModifierContact(indice);
                if (bien)
                    cout << "\n####### MODIFICATION REUSSIE ! #######\n" << endl;
                cout << "\nModifier un autre contact?\n1. OUI\t2. NON\t";
                cin >> continuer;
                continuer = GerErreur(continuer);
            }while (continuer == 1);
            break;
        }
        case 5:
        {
            do
            {
                cout << "Indice de recherche du contact à supprimer (Nom, Prenom ou Numero):\t";
                getline(cin,indice);
                bien = SupprimerContact(indice);
                if(bien)
                    cout << "\n####### SUPPRESSION REUSSIE ! #######\n" << endl;
                cout << "\nSupprimer un autre contact?\n1. OUI\t2. NON\t";
                cin >> continuer;
                continuer = GerErreur(continuer);
            }while (continuer == 1);
            break;
        }
        case 6:
        {
            bien = OptionsAvancees();
            break;
        }
        case 7:
        {
            int sortir(0);
            cout << "Voulez vous quitter le répertoire?\n1. OUI\t2. NON\t";
            cin >> sortir;
            sortir = GerErreur(sortir);
            if (sortir == 1)
            {
                cout << "\n####### Sortie du répertoire..! #######\n" << endl;
                exit(0);
            }
            break;
        }
        default:
        {
            cerr << "Entrée invalide!!!" << endl;
            break;
        }
    }
    return bien;
}

//Fontion AjouterContact
bool AjouterContact()
{
    struct Contact
    {
        std::string nom;
        std::string prenom;
        unsigned int numero;
    }Contact;
    bool bien(true);
    int continuer(1);
    ofstream repertoire(adresse,ios::app);
    if (repertoire)
    {
        repertoire.seekp(0,ios::end);
        cout << "NOM:\t";
        getline(cin,Contact.nom);
        cout << "PRENOM(s):\t";
        getline(cin,Contact.prenom);
        do
        {
            cout << "NUMERO:\t";
            cin >> Contact.numero;
            Contact.numero = GerErreur(Contact.numero);
            if (Contact.numero < 0)
                cout << "Un contact ne peut pas être négatif...Réessayez !" << endl;
        } while (Contact.numero < 0);
        repertoire << Contact.nom << " " << Contact.prenom << " " << Contact.numero << endl;
    }
    else
        bien = false;
    return bien;
}

//Fontion RechercherContact
bool RechercherContact(string indice)
{
    TableauContact.clear();
    string ligne(" ");
    bool bien(true);
    int position(-1);
    ifstream repertoire(adresse,ios::in);
    if (repertoire)
        while(getline(repertoire, ligne))
        {
            position = ligne.find(indice);
            if (position != -1)
                TableauContact.push_back(ligne);
            else
            {
                for (int i = 0; i < indice.length(); i++)
                {
                    indice[i] = tolower(indice[i]);
                    position = ligne.find(indice);
                    if (position != -1)
                    {
                        TableauContact.push_back(ligne);
                        break;
                    }
                }
                for (int i = 0; i < indice.length(); i++)
                {
                    indice[i] = toupper(indice[i]);
                    position = ligne.find(indice);
                    if (position != -1)
                    {
                        TableauContact.push_back(ligne);
                        break;
                    }
                }
            }
        }
    else
        bien = false;
    return bien;  
}

//Fonction ModifierContact
bool ModifierContact(string indice)
{
    int j(0),confirmerMod(0),indiceMod(0),position(-1);
    bool bien(true);
    TableauFichier.clear();
    string contactAModifier("Aucun"),ligne(" "),nvDonnee(" ");
    RechercherContact(indice);
    if(!TableauContact.empty())
    {
        for (auto resultat:TableauContact)
        {
            cout << j+1 << ". " << resultat << endl;
            j++;
        }
        cout << "Choisir l'indice du contact à modifier:\t";
        cin >> indiceMod;
        cin.ignore();
        if (indiceMod>0  && indiceMod<=TableauContact.size())
        {
            cout << "Donnée à modifier (NOM, PRENOM, NUMERO):\t";
            getline(cin,contactAModifier);
            cout << "Nouvelle Donnée:\t";
            getline(cin,nvDonnee);
            cout << "Ce contact sera modifié. Confirmer?\n1.OUI\t2.NON\t";
            cin >> confirmerMod;
            cin.ignore();
            switch (confirmerMod)
            {
                case 1:
                {
                    ifstream repertoire(adresse,ios::in);
                    if(repertoire)
                    {
                        while (getline(repertoire,ligne))
                            TableauFichier.push_back(ligne);
                        repertoire.close();
                        for(auto& element:TableauFichier)
                            if (element.find(TableauContact[indiceMod-1]) != -1)
                            {
                                position=element.find(contactAModifier);
                                if(position != -1)
                                    element.replace(position,contactAModifier.length(),nvDonnee);
                                else
                                {
                                    for (int i = 0; i < contactAModifier.length(); i++)
                                    {
                                        contactAModifier[i] = tolower(contactAModifier[i]);
                                        position = element.find(contactAModifier);
                                        if (position != -1)
                                        {
                                            element.replace(position,contactAModifier.length(),nvDonnee);
                                            break;
                                        }
                                    }
                                    for (int i = 0; i < contactAModifier.length(); i++)
                                    {
                                        contactAModifier[i] = toupper(contactAModifier[i]);
                                        position = element.find(contactAModifier);
                                        if (position != -1)
                                        {
                                            element.replace(position,contactAModifier.length(),nvDonnee);
                                            break;
                                        }
                                    }
                                }               
                            }
                        ofstream repertoire(adresse);
                        if(repertoire)
                            for (auto element:TableauFichier)
                                repertoire << element << endl;
                        else
                        {
                            cout << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
                            bien = false;
                        }
                    }
                    else
                    {
                        cout << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
                        bien = false;
                    }
                    break;
                }
                case 2:
                {
                    cout << "MODIFICATION REVOQUEE !" << endl;
                    bien = false;
                    break;
                }
                default:
                {
                    cout << "ERREUR D'ENTREE !" << endl;
                    bien = false;
                    break;
                }
            }
        }
        else
        {
            cout << "Erreur d'entrée: indice non valide !" << endl;
            bien=false;
        }
    }
    else
    {
        cout << "Contact inexistant pour modification ou problème d'accès au répertoire !" << endl;
        bien=false;
    }
    return bien;
}

//Fontion SupprimerContact
bool SupprimerContact(string indice)
{
    TableauFichier.clear();
    int confirmerSup(0),indiceSup(0),position(-1),j(0);
    string ligne(" ");
    bool bien(true),existe(true);
    RechercherContact(indice);
    if(!TableauContact.empty())
    {
        for (auto resultat:TableauContact)
        {
            cout << j+1 << ". " << resultat << endl;
            j++;
        }
        cout << "Choisir l'indice du contact à supprimer:\t";
        cin >> indiceSup;
        indiceSup = GerErreur(indiceSup);
        if(indiceSup > 0  && indiceSup <= TableauContact.size())
        {
            cout << "Ce contact sera supprimé. Confirmer?\n1. OUI\t2. NON\t";
            cin >> confirmerSup;
            confirmerSup = GerErreur(confirmerSup);
            switch (confirmerSup)
            {
                case 1:
                {
                    ifstream repertoire(adresse,ios::in);
                    if(repertoire)
                    {
                        while (getline(repertoire,ligne))
                            TableauFichier.push_back(ligne);
                        repertoire.close();
                        for(auto& element:TableauFichier)
                        {
                            position = element.find(TableauContact[indiceSup-1]);
                            if(position != -1)
                                element.erase(0,element.length());
                        }
                        ReorganiseRepertoire();
                        ofstream repertoire(adresse);
                        if(repertoire)
                        {
                            for (auto element:TableauSauvegarde)
                                repertoire << element << endl;   
                        }
                        else
                        {
                            cout << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
                            bien = false;
                        }
                    }
                    else
                    {
                        cout << "PROBLEME D'ACCES AU REPERTOIRE !" << endl;
                        bien = false;
                    }
                    break;
                }
                case 2:
                {
                    cout << "Suppression révoquée !" << endl;
                    bien = false;
                    break;
                }
                default:
                {
                    cout << "Entrée invalide !" << endl;
                    bien = false;
                    break;
                }
            }
        }
        else
        {
            cout << "Erreur d'entrée: indice non valide !" << endl;
            bien = false;
        }
        
    }
    else
    {
        cout << "Contact inexistant pour suppression ou problème d'accès au répertoire !" << endl;
        bien = false;
    }
    return bien;
}

//Fonction AfficherTousMesContacts
bool AfficherTousMesContacts()
{
    bool bien(true);
    string ligne(" ");
    ifstream repertoire(adresse,ios::in);
    if (repertoire)
    {
        cout << "\n######## TOUS VOS CONTACTS: ########\n" << endl;
        while(getline(repertoire, ligne))
            cout << ligne << endl;
        cout << "\n####################################\n" << endl;
    }
    else
        bien = false;
    return bien;
}

//Fonction OptionsAvancees
bool OptionsAvancees()
{
    int f(0);
    bool bien(true);
    cout << "\n1. Paramétrer un mot de passe.\n2. Supprimer les doublons.\n3. Vider le Répertoire.\t" << endl;
    cin >> f;
    f = GerErreur(f);
    switch (f)
    {
        case 1:
        {
            bien = ParametrerSecurite();
            if (bien)
                cout << "####### Configuration du nouveau mot de passe réussi ! #######" << endl;
            else
                cout << "ERREUR DE CONFIGURATION !" << endl;
            break;
        }
        case 2:
        {
            bien = SupprimerLesDoublons();
            if (bien)
                cout << "####### SUPPRESSION DES DOUBLONS REUSSIE #######" << endl;
            else
                cout << "PROBLEME D'ACCES AU REPERTOIRE DE CONTACTS !" << endl;
            break;
        }
        case 3:
        {
            ViderLeRepertoire();
            break;
        }
        default :
        {
            cout << "Entrée invalide !" << endl;
            break;
        }
    }
    return true;
}

//Fonction ParametrerSecurite
bool ParametrerSecurite()
{
    bool bien(true),bien1(false);
    int indicateur(1),choix(1),essai(0),essai1(0);
    string mot0("***"), mot1("***"), mot2("***"), ligne("ligne");
    vector <string> TableauPrive;
    cout << "########### CONFIGURATION DE SECURITE ###########\n\nRenseigner les champs." << endl;
    ifstream repertoireP(adresseP,ios::in);
    if (repertoireP)
    {
        while(getline(repertoireP, ligne))
            TableauPrive.push_back(ligne);
        repertoireP.close();
        if (TableauPrive[1] == "Connecté")
        {
            cout << "Entrer ancien mot de passe:\t";
            getline(cin,mot0);
            while (mot0 != TableauPrive[3])
            {
                cout << "Mot de passe incorrect !\n" << "\n1. Réessayer.\n2. Abandonner configuration.\n3. Ancien Mot de passe oublié.\t" << endl;
                cin >> choix;
                choix = GerErreur(choix);
                switch (choix)
                {
                    case 1:
                    {
                        essai+=1;
                        if (essai == 4)
                            return false;
                        cout << "Réessayer (ancien mot de passe):\t";
                        getline (cin,mot0);
                        break;
                    }
                    case 2:
                    {
                        cout << "Opération révoquée !" << endl;
                        return false;
                        break;
                    }
                    case 3:
                    {
                        bien1 = VerifierIdentite();
                        if (bien1)
                        {
                            bien1 = ParametrerSecurite();
                            if (bien1)
                                return true;
                            else
                                return false;
                        }
                        else
                        {
                            cout << "Echec de la reconnaissance !" << endl;
                            return false;
                        }
                    }
                    default:
                    {
                        cout << "Entrée invalide !" << endl;
                        return false;
                        break;
                    }
                }
            }
        }
    }
    do
    {
        cout << "####### Mot De Passe #######" << endl;
        cout << "Entrer Nouveau Mot De Passe:\t";
        getline(cin,mot1);
        cout << "Confirmer Mot De Passe:\t";
        getline(cin,mot2);
        while (mot1 != mot2)
        {
            cout << "Mot de passe non conforme au premier.\n" << "\n1. Réessayer.\n2. Abandonner configuration.\t" << endl;
            cin >> indicateur;
            indicateur = GerErreur(indicateur);
            switch (indicateur)
            {
                case 1:
                {
                    essai1+=1;
                    if (essai1 == 4)
                        return false;
                    cout << "Réessayer (confirmer nouveau mot de passe):\t";
                    getline (cin,mot2);
                    break;
                }
                case 2:
                {
                    cout << "Opération révoquée !" << endl;
                    return false;
                    break;
                }
                default:
                {
                    cout << "Entrée invalide !" << endl;
                    return false;
                    break;
                }
            }
        }
        if(mot1 == mot2)
        {
            string question1("Nom de votre chien ?"), question2("Votre ville d'origine ?"), question3("La couleur de votre telephone ?"), reponse1(""), reponse2(""), reponse3("");
            cout << "\n####### Questions de sécurité en cas de mot de passe oublié #######" << endl;
            cout << question1 << "\t";
            getline(cin,reponse1);
            cout << question2 << "\t";
            getline(cin,reponse2);
            cout << question3 << "\t";
            getline(cin,reponse3);
            ofstream repertoireP(adresseP);
            if(repertoireP)
            {
                repertoireP << "Etat\nConnecté\nMot De Passe:\n" << mot1 << "\nQuestion 1:\n" << question1 << endl << reponse1 << "\nQuestion 2:\n" << question2 << endl << reponse2 << "\nQuestion 3:\n" << question3 << endl << reponse3 << endl;
                return true;
            }
            else
                return false;
        }
    }while (indicateur == 1);
    return true;
}

//Fonction SupprimerLesDoublons
bool SupprimerLesDoublons()
{
    bool bien(true);
    string ligne("ligne");
    ifstream repertoire(adresse,ios::in);
    TableauFichier.clear();
    if (repertoire)
    {
        while(getline(repertoire, ligne))
            TableauFichier.push_back(ligne);
        repertoire.close();
        for (int i = 0; i < TableauFichier.size(); i++)
            for (int j = i+1; j < TableauFichier.size(); j++)
                if (TableauFichier[i] == TableauFichier[j])
                    TableauFichier[j].erase(0,TableauFichier[j].length());
        ReorganiseRepertoire();
        ofstream repertoire(adresse);
        if(repertoire)
            for (auto element:TableauSauvegarde)
                repertoire << element << endl;
        else
            bien = false;
    }
    else
        bien = false;
    return bien;
}

//Fonction ViderLeRepertoire
void ViderLeRepertoire()
{
    string motDePasse("Mon mot de passe"),mot("mot"),ligne("ligne");
    int r(0),compteur(3),confirmerV;
    bool bien(true), acces(false);
    do
    {
        if (acces == false)
        {
            ofstream repertoire(adresseP,ios::app);
            if (repertoire)
            {
                repertoire.seekp(0,ios::end);
                repertoire << "\n\nFichier pour paramètres de sécurité" << endl;
            }
            ifstream repertoireP(adresseP,ios::in);
            if (repertoireP)
            {
                TableauFichier.clear();
                while (getline(repertoireP,ligne))
                    TableauFichier.push_back(ligne);
                if (TableauFichier[1] != "Connecté")
                {
                    cout << "Pour effectuer cette opération, vous aurez besoin de configurer les données de sécurité." << endl;
                    bien = ParametrerSecurite();
                    if(bien)
                        acces = true;
                    else
                        break;
                }
            }
            else
                cerr << "Problème de vérification des paramètres de sécurité." << endl;
            cout << "Nombre d'essai(s) restant(s):\t" << compteur << endl;
            compteur-=1;
            cout << "\n1. Entrer le mot de passe.\n2. Mot de passe oublié.\t";
            cin >> r;
            r = GerErreur(r);
            switch (r)
            {
                case 1:
                {
                    cout << "Mot de passe:\t";
                    getline(cin,motDePasse);
                    acces = MotDePasse(motDePasse);
                    if(acces == true)
                        compteur = 0;
                    break;
                }
                case 2:
                {
                    acces = VerifierIdentite();
                    if (acces == true)
                        acces == ParametrerSecurite();
                    else
                        cerr << "Echec de la reconnaissance !" << endl;
                }
                default:
                {
                    cerr << "Entrée invalide !" << endl;
                    break;
                }
            }
        }
    }while (compteur > 1);
    if(acces == true)
    {
        cout << "Accès Autorisé !\nLe répertoire sera vidé. Voulez vous continuer ?\n1. OUI\t2. NON\t";
        cin >> confirmerV;
        confirmerV = GerErreur(confirmerV);
        switch (confirmerV)
        {
            case 1:
            {
                ofstream repertoire(adresse);
                if(repertoire)
                {
                    repertoire << "" << endl;
                    cout << "###### REPERTOIRE VIDE #######" << endl;
                }
                else
                    cout << "PROBLEME D'ACCES AU REPERTOIRE DE CONTACTS (impossible de vider le répertoire.) !" << endl;
                break;
            }
            case 2:
            {
                cout << "Suppression révoquée !" << endl;
                break;
            }
            default:
            {
                cerr << "Entrée invalide !" << endl;
                break;
            }
        }
    }
}

//Fonction MotDePasse
bool MotDePasse(string motDePasse)
{
    vector <string> TableauPrive;
    string ligne("Privé");
    int indicateur(1);
    ifstream repertoireP(adresseP,ios::in);
    if(repertoireP)
    {
        while (getline(repertoireP,ligne))
            TableauPrive.push_back(ligne);
        if(TableauPrive[1] != "Connecté")
        {
            return (ParametrerSecurite());
            //return true;
        }
        if (motDePasse == TableauPrive[3])
            return true;
        else
            return false;
    }
    else
    {
        cerr << "Problème de vérification des paramètres de sécurité." << endl;
        return false;
    }
}

//Fonction VerifierIdentite
bool VerifierIdentite()
{
    string reponse("***"), ligne("***");
    vector <string> TableauPrive;
    ifstream repertoire(adresseP,ios::in);
    if(repertoire)
    {
        while (getline(repertoire,ligne))
            TableauPrive.push_back(ligne);
        cout << "Nous allons vérifier votre identité.\nVeuillez donc répondre aux questions suivantes:" << endl;
        cout << TableauPrive[4] << ":\t" << TableauPrive[5] << endl;
        getline(cin, reponse);
        if (reponse != TableauPrive[6])
            return false;
        else
        {
            cout << TableauPrive[7] << ":\t" << TableauPrive[8] << endl;
            getline(cin, reponse);
            if (reponse != TableauPrive[9])
                return false;
            else
            {
                cout << TableauPrive[10] << ":\t" << TableauPrive[11] << endl;
                getline(cin, reponse);
                if (reponse != TableauPrive[12])
                    return false;
                else
                {
                    ifstream repertoire(adresseP,ios::in);
                    if (repertoire)
                    {
                        TableauFichier.clear();
                        while(getline(repertoire, ligne))
                            TableauFichier.push_back(ligne);
                        repertoire.close();
                        TableauFichier[1] = "Non Connecté";
                        ofstream repertoire(adresseP);
                        if(repertoire)
                            for (auto element:TableauFichier)
                                repertoire << element << endl;
                    }
                    else
                        cerr << "Problème de vérification des paramètres de sécurité." << endl;
                    return true;
                }
            }
        }
    }
    else
    {
        cout << "Problème de vérification des paramètres de sécurité." << endl;
        return false;
    }
}

//Fonction de remise en forme du répertoire
void ReorganiseRepertoire()
{
    TableauSauvegarde.clear();
    for (auto element:TableauFichier)
        if (element.length()>0)
            TableauSauvegarde.push_back(element);
}

//Fonction ParOrdreAlphabetique()
void ParOrdreAlphabetique()
{
    TableauFichier.clear();
    ifstream repertoire (adresse,ios::in);
    if(repertoire)
    {
        string element(" "),ligne(" ");
        while (getline(repertoire,ligne))
            TableauFichier.push_back(ligne);
        repertoire.close();
        for (int i = 0; i < TableauFichier.size(); i++)
            if ((TableauFichier[i])[0] == 32)
                for (int k = 0; k < TableauFichier[i].length(); k++)
                    (TableauFichier[i])[k] = (TableauFichier[i])[k+1];
        for (int i = 0; i < TableauFichier.size(); i++)
            for (int j = i+1; j < TableauFichier.size(); j++)
                if (TableauFichier[i] > TableauFichier[j])
                    {
                        element = TableauFichier[j];
                        TableauFichier[j] = TableauFichier[i];
                        TableauFichier[i] = element;
                    }
        ofstream repertoire (adresse);
        if(repertoire)
            for (auto element:TableauFichier)
                repertoire << element << endl;
    }
}

//Fonction GerErreur()
int GerErreur(int d)
{
    int indique(0);
    string recupere(" ");
    if (!cin >> d)
        cout << "\a\a\aSaisie invalide: nous attendons une valeur numérique:\t";
    while (!cin >> d)
    {
        if(indique == 7)
        {
            cerr << "Le Répertoire de contacts a cessé de fonctionner." << endl;
            exit(1);
        }
        cin.clear();
        getline(cin,recupere);
        if(indique >= 1)
            cout << "\a\a\aValeur numérique:\t";
        cin >> d;
        indique+= 1;
    }
    cin.ignore();
    return d;
}