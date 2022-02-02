#include <iostream>
#include <list>
#include <sstream>

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
    
    friend ostream& operator<<(ostream& os, const Fone& fone) {
        os << fone.id  << ":(" << fone.number << ")";
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

    void setNumber(string number) {
        this->number = number;
    }

private:
    string id;
    string number;

};

class Contact{
protected:
    string prefix = "-";

public:
    Contact(string name = "") : name{name} {};

    void addFone(Fone fone) {
        string num = fone.getNumber();
        if(fone.validate(num)) {
            fones.push_back(fone);
            cout << "Fone adicionado\n";
        }
    }

    void rmFone(int ind) {
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
};

int main() {

    Contact Agenda{"Abner"};

    while(true) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        if (cmd == "exit") {
            break;
        } else if (cmd == "show") {
            cout << Agenda << endl;
        } else if (cmd == "add") {
            string id {};
            string number {};
            ss >> id >> number;
            Agenda.addFone(Fone(id, number));
        } else if (cmd == "rm") {
            int index{};
            ss >> index;
            Agenda.rmFone(index);
        }
    }
}