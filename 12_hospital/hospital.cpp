#include <iostream>
#include <map>
#include <memory>

using namespace std;

class IMedico;
class IPaciente {
public:
    virtual string getNome() const = 0;
    virtual void addMedico(IMedico* medico) = 0;
    virtual void rmMedico(string nome_medico) = 0;
    virtual map<string, IMedico*>& getMedicos() = 0;
    virtual string getDiagnostico() const = 0;
};

class IMedico {
public:
    virtual string getNome() const = 0;
    virtual void addPaciente(IPaciente* paciente) = 0;
    virtual void rmPaciente(string nome_paciente) = 0;
    virtual map<string, IPaciente*>& getPacientes() = 0; 
    virtual string getEspecialidade() const = 0;   
};

class Medico : public IMedico {
private:
    string nome;
    string esp;
    map<string, IPaciente*> pacientes;

public:
    Medico(string nome, string esp) : nome{nome}, esp{esp} {}

    virtual void addPaciente(IPaciente* paciente) override{
        auto key = paciente->getNome();
        if (this->pacientes.find(key) == this->pacientes.end()) {
            this->pacientes[key] = paciente;
            paciente->addMedico(this);
            return;
        }
    }

    string getEspecialidade() const override {
        return this->esp;
    }

    string getNome() const override {
        return this->nome;
    }

    map<string, IPaciente*>& getPacientes() override {
        return this->pacientes;
    }

    virtual void rmPaciente(string idPaciente) override{
        auto it = this->pacientes.find(idPaciente);
        if (it != this->pacientes.end()) {
            auto paciente = it->second;
            this->pacientes.erase(it);
            paciente->rmMedico(this->nome);
        }

    }

    friend ostream& operator<<(ostream& os, IMedico* med) {
        string inicio = "Med: " + med->getNome() + ":" + med->getEspecialidade();
        os << inicio;
        
        os << "\t" << "Pacs: [ ";
        for (auto pacs : med->getPacientes()) {
            os << pacs.second->getNome() << ' ';
        }
        
        os << "]";
        return os;
    } 
};

class Paciente : public IPaciente {
private:
    string nome;
    string diag;
    map<string, IMedico*> medicos;

public:
    Paciente(string nome, string diag) : nome{nome}, diag{diag} {}

    virtual void addMedico(IMedico* medico) override{
        auto key = medico->getNome();
        if (this->medicos.find(key) == this->medicos.end()) {
            this->medicos[key] = medico;
            medico->addPaciente(this);
        }
    }

    virtual string getDiagnostico() const override {
        return this->diag;
    }

    virtual string getNome() const override {
        return this->nome;
    }

    virtual map<string, IMedico*>& getMedicos() override {
        return this->medicos;
    }
    
    virtual void rmMedico(string name) override {
        auto it = this->medicos.find(name);
        if (it != this->medicos.end()) {
            auto medico = it->second;
            this->medicos.erase(it);
            medico->rmPaciente(this->nome);
        }
    }

    friend ostream& operator<<(ostream& os, IPaciente* pac) {
        string inicio = "Pac: " + pac->getNome() + ":" + pac->getDiagnostico();
        os << inicio;

        os << "\t" << "Meds: [ ";
        for (auto meds : pac->getMedicos()) {
            os << static_cast<Medico*>(meds.second)->getNome() << " ";
        }

        os << "]";
        return os;
    }
};

class Hospital {
private:
    map<string, shared_ptr<IMedico>> medicos;
    map<string, shared_ptr<IPaciente>> pacientes;

public:
    Hospital() {}

    void addMedico(shared_ptr<IMedico> medico) {
        auto key = medico->getNome();
        if (this->medicos.find(key) == this->medicos.end()) {
            this->medicos[key] = medico;
        } else {
            throw "Esse medico ja exite!";
        }
        cout << "Medico " << key << " cadastrado!\n";
    }

    void addPaciente(shared_ptr<IPaciente> paciente) {
        auto key = paciente->getNome();
        if (this->pacientes.find(key) == this->pacientes.end()) {
            this->pacientes[key] = paciente;
        } else {
            throw "Esse paciente ja existe!";
        }
        cout << "Paciente " << key << " cadastrado!\n";
    }

    void vincular(string name_med, string name_pac) {
        auto it_med = this->medicos.find(name_med);
        auto it_pac = this->pacientes.find(name_pac);

        if (it_med == this->medicos.end() || it_pac == this->pacientes.end()) {
            throw "erro: medico e paciente nao cadastrados";
            return;
        }

        auto lista_medicos = it_pac->second->getMedicos();
        for (auto med : lista_medicos) {
            if (med.second->getEspecialidade() == it_med->second->getEspecialidade()) {
                throw "Error: Apenas um medico especializado com seu paciente!";
                return;
            }
        }

        it_pac->second.get()->addMedico(it_med->second.get());
        it_med->second.get()->addPaciente(it_pac->second.get());
        cout << "Medico " << name_med << " e paciente " << name_pac << " foram vinculados\n";
    }

    void removerPaciente(string nome) {
        auto it = this->pacientes.find(nome);
        if (it == this->pacientes.end()) {
            throw "Error: Paciente nao cadastrado!";
            return;
        }

        for (auto med : it->second->getMedicos()) {
            med.second->getPacientes().erase(nome);
        }
        
        this->pacientes.erase(it);
        cout << "Paciente " << nome << "foi removido do hospital.\n";
    }

    void removerMedico(string nome) {
        auto it = this->medicos.find(nome);
        if (it == this->medicos.end()) {
            throw "Erro: medico nao cadastrado!";
            return;
        }

        for (auto pac : it->second->getPacientes()) {
            pac.second->getMedicos().erase(nome);
        }

        this->medicos.erase(it);
        cout << "Medico " << nome << "foi removido do hospital.\n";
    }

    friend ostream& operator<<(ostream& os, Hospital& hosp) {
        os << "HOSPITAL: \n";
        for (auto paciente : hosp.pacientes) {
            os << static_cast<Paciente*>(paciente.second.get()) << '\n';
        }

        for (auto medico : hosp.medicos) {
            os << static_cast<Medico*>(medico.second.get()) << '\n';
        }

        return os;     
    }
};

int main() {
    Hospital hosp;
    
    while(true) {
        string cmd {};
        cin >> cmd;
        try {
            if (cmd == "exit") {
                break;
            } else if (cmd == "addmed") {
                string nome {}, esp {};
                cin >> nome >> esp;
                hosp.addMedico(make_shared<Medico>(nome, esp));

            } else if (cmd == "addpac") {
                string nome {}, diag {};
                cin >> nome >> diag;
                hosp.addPaciente(make_shared<Paciente>(nome, diag));

            }  else if (cmd == "show") {
                cout << hosp << '\n';

            } else if (cmd == "rmmed") {
                string nome {};
                cin >> nome;
                hosp.removerMedico(nome);

            } else if (cmd == "rmpac") {
                string nome {};
                cin >> nome;
                hosp.removerPaciente(nome);

            } else if (cmd == "vinc") {
                string nome_pac {}, nome_med {};
                cin >> nome_pac >> nome_med;
                hosp.vincular(nome_pac, nome_med);

            } else {
                cout << "Erro: comando invalido" << '\n';
            }
        } catch(const char* e) {
            cout << "Error: " << e << '\n';
        }
    }
}