#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <sstream>

using namespace std;

class Client{
    string fone;
    string id;

public:
    Client(string fone = "", string id = "") : fone{fone} , id{id} {}

    string getFone(){
        return this->fone;
    }

    string getId(){
        return this->id;
    }

    void setId(string id){
        this->id;
    }

    void setFone(string fone){
        this->fone;
    }

    friend std::ostream& operator<<(std::ostream& os, Client client) {
        os << "Fone:(" << client.fone << ") Id:" << client.id;
        return os;
    }
};

class Sala {
    map<int, shared_ptr<Client>> cadeiras;

    bool validaClient(string id) {
        for (auto it = this->cadeiras.begin(); it != this->cadeiras.end(); ++it) {
            if (it->second->getId() == id){
                return true;
            }
        }
        return false;
    }
    
public:
    Sala(){};

    bool reservar(const shared_ptr<Client>& client, int idc) {
        auto it = cadeiras.find(idc);

        if (it != cadeiras.end()) {
            cout << "Cadeira ja esta ocupada" << '\n';
            return false;
        }

        if (validaClient(client->getId())) {
            cout << "Nao e possivel reservar 2 cadeiras" << '\n';
            return false;
        }

        cadeiras[idc] = client;
        cout << "Cadeira reservada" << '\n';
        return true;
    } 

    bool cancelar(string id) {
      auto i = this->cadeiras.begin();
      auto end = this->cadeiras.end();

       for ( i; i != end; ++i) {
            if (i->second->getId() == id){
                cadeiras.erase(i);
                cout << id << " foi removido da cadeira" << '\n';
                return true;
            }
        }
        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const Sala& sala) {
        os << "[";
        for (auto cadeira : sala.cadeiras) {
            if (cadeira.second == nullptr)
                os << " - ";
            else 
                os << "Cadeira" <<  ": " << *cadeira.second << "| ";
        }
        os << "]";

        return os;
    }
}; 

int main()
{
    Sala cinema;

    cout << cinema << "\n"; //Cinema vazio
    cinema.reservar(make_shared<Client>("4400", "Abner"), 1); // Add 1 pessoa na sala
    cout << cinema << "\n\n";
  
    cinema.reservar(make_shared<Client>("0011", "Fulano"), 1);  //Teste reserva na mesma cadeira
    cinema.reservar(make_shared<Client>("1010", "Fulano"), 2);
    cout << cinema << "\n\n";

    cinema.cancelar("Fulano"); //Cancela reserva
    cout << cinema << "\n";
}