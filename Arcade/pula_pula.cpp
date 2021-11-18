#include <iostream>
#include <list>
#include <memory>

using namespace std;

class Kid {
    
    string nome;
    int idade;

public:
    Kid(int idade = 0, string nome = "") :
        nome{nome}, idade{idade} {    
    }


    friend ostream& operator<<(ostream& os, const Kid& kid) {
        os << "Nome: " << kid.nome << "; "; 
        os << kid.idade << " anos\n";
        return os;
    }  

    int getIdade(){
        return this->idade;
    }

    string getNome(){
        return this->nome;
    }

    void setIdade(int idade){
        this->idade;
    }
}; 

class Pula_Pula {
    list<shared_ptr<Kid>> pulando;
    list<shared_ptr<Kid>> espera; 
    

public:
    Pula_Pula(){}

    void entra(const shared_ptr<Kid>& kid) {
        this->espera.push_back(kid);
        cout << kid->getNome() << " entrou na fila de espera" << '\n';
    }

    void entrar_brinqdo() {
        if (espera.size() == 0) {
            cout << "Lista de espera vazia" << '\n';
        }
        else{
            this->pulando.push_back(this->espera.front());
            this->espera.pop_front();
            cout << "Entrou no pula pula" << '\n';
        }
    }

    void sair_brinqdo() {
        if (pulando.size() == 0) {
            cout << "Nao tem ninguem brincando" << '\n';
        }else {
            this->espera.push_back(this->pulando.front());
            this->pulando.pop_front();
            cout << "Saiu do pula pula" << '\n';
        }
    }

    bool papaiChegou(string name) {
        for (auto kid : this->espera) {
            if(kid->getNome() == name){
                this->espera.remove(kid);
                cout << "Papai de " << kid->getNome() <<" o levou para casa" << '\n';
                return true;
            }
        }

        for (auto kid : this->pulando) {
            if(kid->getNome() == name){
                this->pulando.remove(kid);
                cout << "Papai de " << kid->getNome() <<" o levou para casa" << '\n';
                return true;
            }
        }
        return false;
    }

    friend ostream& operator<<(ostream& os, const Pula_Pula& t) {
        os << "\nEspera: ";
        for (auto kid : t.espera) {
            os << *kid << " ";
        }

        os << "\nBrincando: ";
        for (auto kid: t.pulando) {
            os << *kid << "";
        }

        return os;
    }

};

int main()
{
    Pula_Pula pula;
    while(true) {
        string cmd;
        cin >> cmd;
        if (cmd == "exit") {
            break;
        } else if (cmd == "show") {
            cout << pula << endl;
        } else if (cmd == "espera") {
            int idade{};
            string nome{};
            cin >> nome >> idade;
            pula.entra(make_shared<Kid>(idade, nome));
        } else if (cmd == "entrar") {
            pula.entrar_brinqdo();
        } else if(cmd == "sair") {
            pula.sair_brinqdo();
        } else if(cmd == "papai") {
            string name{};
            cin >> name;
            pula.papaiChegou(name);
        }
    }
}