#include <iostream>

using namespace std;

class Fruta {
    bool tem {true};
    std::string type;
    
public:
    Fruta(std::string type = ""): type(type) {
       // std::cout << this->type << " created" << std::endl;
    }
    ~Fruta() {
        //std::cout << this->type << " destroyed" << std::endl;
    }
    bool seTem() const {
        return tem;
    }
    void seNaoTem() {
        tem = false;
    }
    void Sim() {
        tem = true;
    }
    
    std::string getType() const {
        return type;
    }
    friend std::ostream& operator<<(std::ostream& os, const Fruta& fruta) {
        os << fruta.type << ":" << (fruta.tem ? "tem" : "nao tem");
        return os;
    }
};

class Suco : public Fruta {
    std::string name;
    int jarra = 2;
public:
    Suco(std::string name): name(name) {
        std::cout << "Suco de " << this->name << " feito" << std::endl;
    }

    ~Suco() {
        //std::cout << this->name << " Cabo suco destroyed" << std::endl;
    }
    void tomarSuco() {
        if (seTem() != false && jarra != 0){
            jarra = jarra - 1;
            std::cout << "Tomando suquinho de " << this->name << "\n";
        }
        else {
            seNaoTem();
            std::cout << "Suco de " << this->name << " acabou :(\n";
        }
    }

    void fazerSuco() {
        jarra =+ 2;
        if (seTem() == false) {
           Sim();
           cout << "Fizemos mais suco :)\n";
        }
    }

    std::string getName() {
        return name;
    }

};

int main() {
    Suco laranja("laranja");
    laranja.tomarSuco();
    laranja.tomarSuco();        // TOMANDO SUCO
    laranja.tomarSuco();

    laranja.fazerSuco();        // FAZENDO MAIS SUCO

    laranja.tomarSuco();
    laranja.tomarSuco();        // TOMANDO SUCO
    laranja.tomarSuco();
}