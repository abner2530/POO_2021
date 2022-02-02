#include <iostream>
#include <list>
#include <memory>
#include <map>

using namespace std;

class Conta {
protected:
    float saldo;
    string clienteId;
    int id;
    string type;

public:
    Conta(int id, string clienteId) : id{id}, clienteId{clienteId} {}

    virtual void depositando(float value) {
        if (value > 0) {
            this->saldo += value;
        } else {
            cout << "Valor invalido!\n";
        }
    }

    virtual void atualizaMes() = 0;

    void transferindo(shared_ptr<Conta> destino, float value) {
        if (this->saldo >= value && value > 0) {
            this->saldo -= value;
            destino->saldo += value;

            cout << "Transferencia de R$" << value << " de " << this->clienteId
            << " para " << destino->clienteId << "\n";
            return;
        }

        cout << "Valor excede o seu saldo!\n";
    }

    virtual void retirando(float value) {
        if (this->saldo > value && value > 0) {
            this->saldo -= value;
            cout << "Retirada de R$" << value << " feita por " << this->clienteId << "\n";
            return;
        }

        cout << "Valor > Saldo\n";
    }

    float getSaldo() const {
        return this->saldo;
    }

    string getNomeCliente() const {
        return this->clienteId;
    }

    int getId() const {
        return this->id;
    }

    string getTipoConta() const {
        return this->type;
    }

    friend ostream& operator<<(ostream& os, const Conta& cont) {
        os << "Id " << cont.id << ": Nome " << cont.clienteId << ": R$ " << cont.saldo << ": Tipo " << cont.type;
        return os;
    }
};

class Corrente : public Conta {
public:
    Corrente();

    Corrente(int id, string clienteId) : Conta(id, clienteId) {
        this->type = "CC";
    }

    virtual void atualizaMes() override {
        this->saldo -= 20;
    }
};

class Poupanca : public Conta {
public:
    Poupanca(int id, string clienteId) : Conta(id, clienteId) {
        this->type = "CP";
    }

    virtual void atualizaMes() override {
        this->saldo += this->saldo * 0.01;
    }
};

class Cliente {
private:
    list<shared_ptr<Conta>> contas;
    string Client_id;

public:
    Cliente(string Client_id) : Client_id{Client_id} {}
    
    void adicionarConta(shared_ptr<Conta> cont) {
        contas.push_back(cont);
    }

    list<shared_ptr<Conta>> getContas() const {
        return this->contas;
    }

    string getNome() const {
        return this->Client_id;
    }

    void setContas(list<shared_ptr<Conta>> novas) {
        this->contas = novas;
    }

    void setNome(string str) {
        this->Client_id = str;
    }

    friend ostream& operator<<(ostream& os, const Cliente& cont) {
        os << cont.Client_id;
        return os;
    }
};

class Agencia {
private:
    int autoId;
    map<int, shared_ptr<Conta>> contas;
    map<string, shared_ptr<Cliente>> clientes;

    bool validateId(int id) {
        auto it = this->contas.find(id);
        if(it == this->contas.end()){
            return false;
        }
        return true;
    }

    shared_ptr<Conta> get_conta(int id){ 
        auto it = this->contas.find(id);

        if (it == this->contas.end()){
            cout << "conta inexistente" << endl;
        }

        return it->second;
    }

public:
    Agencia() : autoId{0} {}

    void adicionarCliente(string nome) {
        auto it = clientes.find(nome);
        if (it != clientes.end()) {
            cout << "Cliente " << nome << " ja esta cadastrado(a).\n";
            return;
        }

        clientes[nome] = make_shared<Cliente>(nome);
        contas[autoId] = make_shared<Corrente>(++autoId, nome);
        contas[autoId] = make_shared<Poupanca>(++autoId, nome);
        cout << "Cliente " << nome << " foi cadastrado(a).\n";        
    }

    void depositar(int id_conta, float value) {
        auto it = contas.find(id_conta);
        if (validateId(id_conta)) {
            it->second->depositando(value);
            cout << "Depositados R$" << value << " para " << it->second->getNomeCliente() << '\n';
            return;
        }

        cout << "ID conta invalido!\n";
    }

    void sacar(int id_conta, float value) {
        if (validateId(id_conta) && value > 0) {
            this->get_conta(id_conta)->retirando(value);
            return;
        } else {
            std::cout << "ID invalido.\n";
        }

    }

    void transferencia(int id_conta_envia, int id_conta_recebe, float value) {
        if (validateId(id_conta_envia) && validateId(id_conta_recebe)) {
            this->get_conta(id_conta_envia)->transferindo(this->get_conta(id_conta_recebe), value);
            return;
        }
        std::cout << "ID de uma das contas invalido! \n";
    }

    void atualizaMes() {
        for (auto it = this->contas.begin(); it != this->contas.end(); ++it) {
            it->second->atualizaMes();
        }
        cout << " Mes Atualizado!\n";
    }

    friend ostream& operator<<(ostream& os, const Agencia& agencia) {        
        os << "\nContas: \n";
        for (auto conta : agencia.contas) {
            os << *conta.second << '\n';
        }

        return os;
    }
};

int main() {
    Agencia bradesco;

    bradesco.adicionarCliente("Abner");
    bradesco.adicionarCliente("Fulano");
    bradesco.adicionarCliente("Sicrano");
    bradesco.adicionarCliente("Fulano");

    cout << bradesco << '\n';

    bradesco.depositar(1, 100);
    bradesco.depositar(2, 200);
    bradesco.depositar(3, 300);
    bradesco.depositar(4, 400);

    bradesco.sacar(4, 150);
    bradesco.sacar(1, 170);
    bradesco.sacar(2, 201);

    cout << bradesco << '\n'; 

    bradesco.transferencia(1, 5, 40);
    bradesco.transferencia(4, 6, 125);
    bradesco.transferencia(25, 2, 30);
    bradesco.transferencia(3, 25, 10);
    cout << bradesco << '\n';

    bradesco.atualizaMes();
    cout << bradesco << '\n';

    return 0;
}