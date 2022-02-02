#include <iostream>
#include <algorithm>

struct Pessoa {
    std::string nome;
    int idade;

    Pessoa(std::string nome = "", int idade = 0) : 
        nome{nome}, idade{idade} {
    }

};

struct Moto {
    Pessoa* pessoa = nullptr;
    int power {0};
    int time {0};

    Moto(int power = 1) : 
        power{power} {
    }

    void Desce() {
        if(this->pessoa != nullptr) {
            std::cout << this->pessoa->nome << " desceu da moto!\n";
            this->pessoa = nullptr;
        }
    } 

    void Imprimir() {
        std::cout << "\nPotencia : " << this->power << '\n';
        std::cout << "Tempo restante: " << this->time << '\n';
        std::cout << "Tem pessoa? ";
        if(this->pessoa != nullptr) 
            std::cout << "Sim\n"; 
        else 
            std::cout << "Nao\n";
    }

    void Subir(Pessoa* pessoa) {
        if(this->pessoa != nullptr) {
            Desce();
        }
        this->pessoa = pessoa;
        std::cout << this->pessoa->nome << " subiu na moto!\n";
    } 

    void Buytime(int time) {
        this->time += time;
    }

    void Dirigir(int time) {
        if (this->pessoa != nullptr) {
            if (this->pessoa->idade <= 10) {
                int dirigiu {0};
                for(;this->time > 0 && time > 0; dirigiu++) {
                    this->time--;
                    time--;
                }
                std::cout << this->pessoa->nome <<" dirigiu por " << dirigiu << " minutos";
                std::cout << " e acabou o tempo!\n";

            } else {
                std::cout << "Pessoa Inapropriada para brincar\n";
            }
        } else {
            std::cout << "Necessario alguem para dirigir\n";
        }
    }

    void Buzinar() {
        if (this->pessoa != nullptr) {
            std::cout << "P";
            for (int i = 1; i <= this->power; i++) {
                std::cout << "e";
            }
            std::cout << "m!\n";
        }
    }

};

int main() {
    Pessoa p1 {"Abner", 6};
    Pessoa p2 {"Zezim", 12};

    Moto m1 {5};
    m1.Subir(&p2);
    m1.Subir(&p1);
    m1.Buytime(15);
    m1.Dirigir(20);
    m1.Buzinar();
    m1.Imprimir();

    return 0;
}
