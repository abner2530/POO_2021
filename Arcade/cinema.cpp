#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Client {
    
    string fone;
    string id;

public:
    Client(string fone = "", string id = "") :
        fone{fone}, id{id} {    
    }

    friend ostream& operator<<(ostream& os, const Client& client) {
        os << "Fone: " << client.fone << "; id: " << client.id << " \n";
        return os;
    }  

    string getId(){
        return this->id;
    }

    string getFone(){
        return this->fone;
    }

    void setId(string id){
        this->id;
    }

    void setIdade(string fone){
        this->fone;
    }
}; 

class Sala {
    vector<shared_ptr<Client>> cadeiras;
    
public:
    Sala(int capacit) : cadeiras(capacit, nullptr){}
        
    bool validaId(int indice) {
        if (indice < 0 || indice >= (int) this->cadeiras.size()) {
            cout << "cadeira invalida\n";
            return false;
        }
        return true;
    }

    bool validaClient(string id) {
        for (int i = 0; i < (int) this->cadeiras.size(); i++) {
            if(this->cadeiras[i] != nullptr && this->cadeiras[i]->getId() == id){
                return false;
            }
        }
        return true;
    }

    bool cancelar(string id) {
        for (int i = 0; i < (int) this->cadeiras.size(); i++) {
            if(this->cadeiras[i] != nullptr && this->cadeiras[i]->getId() == id){
                cadeiras[i] = nullptr;
                cout << "Reserva de cadeira cancelada" << '\n';
                return true;
            }
        }
        return false;
       
    }

    bool reservar(const shared_ptr<Client>& client, int indice) {
        if (!this->validaId(indice)) {
            return false;
        }

        if(this->cadeiras[indice] != nullptr) {
            cout << "Cadeira esta ocupada" << '\n';
            return false;
        }

        if(!this->validaClient(client->getId())) {
            cout << "Senhor voce nao pode reservar duas cadeiras" << '\n';
            return false;
        }
       
        this->cadeiras[indice] = client;
        cout << "Cadeira reservada com sucesso!" << '\n';
        return true;

    } 

    friend std::ostream& operator<<(std::ostream& os, const Sala& c) {
        for (int i = 0; i < (int) c.cadeiras.size(); i++) {
            auto& cadeira = c.cadeiras[i];
            os << i << ":" ;
            if (cadeira != nullptr)
                os << *cadeira;
            else
                os << "xxxx  ";
            
        }
        return os;
    } 

};

int main()
{
    Sala cinema(10);
    while(true) {
        string cmd;
        cin >> cmd;
        if (cmd == "exit") {
            break;
        } else if (cmd == "show") {
            cout << cinema << endl;
        }  else if (cmd == "cancelar") {
            string indice{};
            cin >> indice;
            cinema.cancelar(indice);
        } else if(cmd == "reservar") {
            string fone{};
            string id{};
            int indice{};
            cin >> fone >> id >> indice;
            cinema.reservar(make_shared<Client>(fone, id), indice);
        }
    }
}