#include <iostream>
#include <vector>
#include <memory>
#include <map>

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
        os << fone.id << ":" << fone.number;
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

    void setNumero(string num) {
        if (validate(num)) {
            this->number = num;
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
    Contact(string name, vector<Fone> fones) : name{name}{
        add(fones);
    }

    Contact(string name) : name{name} {}

    Contact() : name{""} {}

    void add(vector<Fone> fones) {
        for (auto fone : fones) {
            add(fone);
        }
    }

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

    string getNames() const {
        return this->name;
    }

    vector<Fone> getFones() const {
        return this->fones;
    }


    bool isValid(int id) {
        if (id < 0 || id >= (int) this->fones.size()) {
            cout << "Fone inexistente\n";
            return false;
        }
        return true;
    }

private:
    string name;
    vector<Fone> fones;

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
private:
    map<string, Contact> fones;

public:
    Agenda() {}

    void addFone(const Contact& phone) {
        auto it = fones.find(phone.getNames());

        if (it == fones.end()) {
            this->fones[phone.getNames()] = phone;
        } else {
            it->second.add(phone.getFones());
        }
    }

    void addCont(const vector<Contact>& contacts) {
        for (auto contact : contacts) {
            addFone(contact);
        }
    }

    void rmFone(string name) {
        auto it = fones.find(name);

        if (it != fones.end()) {
            fones.erase(it);
        } else {
            std::cout << "Contact nao existe.\n";
        }
    }

    void Search(string str) {
        for (auto phone : fones) {
            size_t found = phone.second.getNames().find(str);
            if (found != string::npos) {
                cout << phone.second << '\n';
            }
        }
        cout << '\n';
    }

    friend ostream& operator<<(ostream& os, Agenda& agenda) {
        os << '\n';

        for (auto phone : agenda.fones) {
            os << phone.second << '\n';
        }

        return os;
    }
};

int main() {    
    Agenda agenda;

    Contact c1("Tiao");
    c1.add(vector<Fone>{{"Tim","1122"}, {"Oi", "2233"}});

    Contact c2("Fulano");
    c2.add(vector<Fone>{{"Oi","1513"}, {"Tim", "1144"}});

    Contact c3("Sicrano");
    c3.add(vector<Fone>{{"Claro","1213"}, {"Oi", "1414"}});

    agenda.addCont(vector<Contact>({c1, c2, c3}));

    cout << agenda << "\n";

    agenda.rmFone("Sicrano");
    cout << agenda << "\n";

    agenda.Search("T");

    return 0;
}