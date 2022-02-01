#include <iostream>
#include <list>

using namespace std;

class Fone {
public: 
    Fone(string id = "", string number = "") : id{id}, number{number}{};

    bool validate(string number) {
        for (char c : number) {
            if ((int)c > 57 || (int)c < 48) {
                return false;
            }
        }
        if (number.size() > 11 || number.size() <= 0) {
            cout << "Numero invalido\n";
            return false;
            }
        cout << "Fone valido" << '\n';
        return true;
    }
    
    friend ostream& operator<<(ostream& os, Fone fone) {
        os << fone.id << ":(" << fone.number << ")";
        return os;
    }

    string getId() {
        return this->id;
    }

    string getNumber() {
        return this->number;
    }

    void setId(string id) {
           this->id = id;
    }

    void setNumero(string str) {
        if (validate(str)) {
            this->number = str;
        }
    }

private:
    string id;
    string number;

};

class Contact{
protected:
    string prefix = "-";

public:
    Contact(string name, list<Fone> fones) : name{name}, fones{fones} {}
    Contact(string name) : name{name}, fones{} {}

    void add(Fone fone) {
        if (fone.getNumber() != "" && !Repetidos(fone)) {
            fones.push_back(fone);
        }
    }

    void rm(int ind) {
        if (isValid(ind)) {
            fones.erase(next(fones.begin(), ind));
            cout << "Fone removido\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, Contact& contact) {
        if (contact.fones.empty()) {
            os << contact.prefix << " " << contact.name;
            os << " []";
        } else {
            int i = 0;
            os << contact.prefix << " " << contact.name;
            for (auto fone: contact.fones) {
                os << " [" << i << ":" << fone << "]";
                i++;
            }
        }
        return os;
    }

    string getNames() {
        return this->name;
    }

    list<Fone> getFones() {
        return this->fones;
    }


    bool isValid(int indice) {
        if (indice < 0 || indice >= (int) this->fones.size()) {
            cout << "Fone inexistente\n";
            return false;
        }
        return true;
    }

private:
    string name;
    list<Fone> fones;

    bool Repetidos(Fone fone) {
        for (auto phone : fones) {
            if (fone.getId() == phone.getId() && fone.getNumber() == phone.getNumber()) {
                return true;
            }
        }

        return false;
    }
};

class Agenda {
public:
    Agenda() {}

    void addCont(list<Contact>& cts) {
        for (auto cont : cts) {
            addFone(cont);
        }
    }

    void addFone(Contact& cnt) {
        auto it = existe(cnt.getNames());
        if (it != contacts.end()) {
            cout << "Contato existente.\n";
            addNum(it, cnt);
            return;
        }

        cout << "Contato adicionado.\n";
        contacts.push_back(cnt);
    }

    void rmFone(string name) {
        auto it = existe(name);
        if (it == contacts.end()) {
            cout << "Contato inexistente.\n";
            return;
        }

        cout << name << " removido(a) da agenda.\n";
        contacts.erase(it);
    }

    void Search(string str) {
        for (auto cont : contacts) {
            size_t found = cont.getNames().find(str);
            if (found != string::npos) {
                cout << cont << '\n';
            }
        }
        cout << '\n';
    }

    friend ostream& operator<<(ostream& os, Agenda& agenda) {
        os << '\n';
        for (auto cont : agenda.contacts) {
            os << cont << '\n';
        }

        return os;
    }

private:
    list<Contact> contacts;

    list<Contact>::iterator existe(string name) {
        for (auto i = contacts.begin(); i != contacts.end(); ++i) {
            if (name == (*i).getNames()) {
                return i;
            }
        }

        return contacts.end();
    }

    void addNum(list<Contact>::iterator it, Contact cont) {
        for (auto num : cont.getFones()) {
            (*it).add(num);
        }
    }
};

int main() {    
    Agenda agenda;

    Contact a1("Joao"), a2("Nogueira"), a3 ("Carlos"), a4("Nogueira");
    a1.add(Fone("Tim", "20002012"));         
    a1.add(Fone("Vivo", "19448632"));
    a2.add(Fone("Oi", "34000412"));
    a3.add(Fone("Tim", "19017612"));
    a3.add(Fone("Claro","81238912"));
    a4.add(Fone("Oi","25303540"));     //Repetição Teste

    list<Contact> contacts {a1,a2,a3,a4};   
    agenda.addCont(contacts);
    cout << agenda << '\n';

    agenda.rmFone("Carlos");                       //Remove Contato da Agenda
    cout << agenda << '\n';

    agenda.Search("N");                            //Contatos com a Letra "N"

    return 0;
}

