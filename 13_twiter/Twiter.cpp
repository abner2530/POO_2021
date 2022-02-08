#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <set>

using namespace std;

class TweetException : public std::exception {
private:
    std::string msg;

public:
    TweetException(const std::string& msg):
        msg(msg){
    }

    const char* what() const noexcept override {
        return msg.c_str();
    }
};

class Mensagem {
private:
    int id;
    string remetente;
    string msg;
    set<string> likes;
    Mensagem* retweet {nullptr};
    bool deletada {false};

public:
    Mensagem(int id, string remetente, string msg) : id{id}, remetente{remetente}, msg{msg}{}

    int getId() const {
        return this->id;
    }

    string getRemetente() const {
        return this->remetente;
    }

    string getMsg() {
        return this->msg;
    }

    void like(string nome) {
        likes.insert(nome);
    }

    set<string> getLikes() const{
        return this->likes;
    }

    void setRt(Mensagem* tweet) {
        this->retweet = tweet;
    }

    void setDeleted() {
        this->deletada = true;
        this->msg = "Tweet foi deletado!";
        this->remetente = "";
        this->likes.clear();
    }

    bool isDeleted() const {
        return this->deletada;
    }

    friend ostream& operator<<(ostream& os, const Mensagem& mensagem) {
        auto* new_rt = &mensagem;
        int tab = 0;

        do {
            if (new_rt->isDeleted()) {
                os << string(tab * 2, ' ') << "Tweet Deletado.\n";
            }
            
            else {
                os << string(tab * 2, ' ') << new_rt->id << " (" << new_rt->remetente << ") - " << new_rt->msg << " [ ";           
                for (auto& user : new_rt->likes) {
                    os << user << ' ';
                }            
                os << "]\n";
            }

            new_rt = new_rt->retweet;
            tab++;

        } while (new_rt != nullptr);

        return os;
    }
};

class Inbox {
private:
    string nome;
    map<int, Mensagem*> time_line;
    map<int, Mensagem*> meusTweets;
public:
    Inbox() {}

    void guarda_timeline(Mensagem* tweet) {
        this->time_line[tweet->getId()] = tweet;
    }

    void guarda_meustweets(Mensagem* tweet) {
        this->meusTweets[tweet->getId()] = tweet;
    }

    Mensagem* getTweet(int id) {
        auto it  = time_line.find(id);
        if (it == time_line.end()) 
           throw TweetException("Mensagem nao encontrada.\n");
        return it->second;
    }

    void remMsgsDe(const string& nome) {
        vector<Mensagem*> tweetMarck;
        for (auto tweet : time_line) {
            if (tweet.second->getRemetente() == nome) 
                tweetMarck.push_back(tweet.second);
        }

        for (auto tweet : tweetMarck) {
            time_line.erase(tweet->getId());
        }
    }

    vector<Mensagem*> getTimeLine() const {
        vector<Mensagem*> tweets;
        for (auto& tweet : time_line) {
            tweets.push_back(tweet.second);
        }

        return tweets;
    }

    vector<Mensagem*> getMyTweets() const {
        vector<Mensagem*> tweets;
        for (auto& tweet : meusTweets) {
            tweets.push_back(tweet.second);
        }

        return tweets;
    }

    friend std::ostream& operator<<(std::ostream& os, Inbox& inbox) {
        os << "Ibox: " << inbox.nome << '\n';
        for (auto tweet : inbox.getTimeLine()) {
            os << *tweet << "\n";
        }
        os << "\n";
        return os;
    }

};

class Usuario {
private:
    string nome;
    Inbox inbox;
    map<string, Usuario*> seguidores;
    map<string, Usuario*> seguindo;

    bool seNaoSegue(string nome) {
        auto it = seguindo.find(nome);
        if (it == seguindo.end()) 
            throw TweetException("Voce nao segue essa pessoa.");
        return true;
    }

    bool seSegue(string nome) {
        auto it = seguindo.find(nome);
        if (it == seguindo.end()) 
            return true;
        throw TweetException("Voce ja segue essa pessoa.");
    }

public:
    Usuario(string nome) : nome{nome} {}

    void seguir(Usuario* other) {
        auto key = other->nome;
        if (seSegue(key)) {
            other->seguidores[this->nome] = this;
            this->seguindo[key] = other;
        }
        cout << this->nome << " esta seguindo " << other->nome <<"\n";
    }

    Inbox& getInbox() {
        return this->inbox;
    }
    
    void enviarTweet(Mensagem* tweet) {
        this->inbox.guarda_timeline(tweet);
        this->inbox.guarda_meustweets(tweet);

        for (auto user : seguidores) {
            user.second->inbox.guarda_timeline(tweet);
        }
    }

    void canseiDeSeguir(Usuario* other) {
        auto key = other->nome;
        if (seNaoSegue(key)) {
            this->inbox.remMsgsDe(other->nome);
            this->seguindo.erase(key);
            other->seguidores.erase(this->nome);
        }
    }

    void like(int twId) {
       auto tweet = this->inbox.getTweet(twId);
       tweet->like(this->nome);
    }

    void rmSeguidos() {
        vector<Usuario*> remSeguidos;
        for (auto user : this->seguindo) {
            remSeguidos.push_back(user.second);
        }

        for (auto user : remSeguidos) {
            this->seguindo.erase(user->nome);
            user->seguidores.erase(this->nome);
        }
    }

    void rmSeguidores() {
        vector<Usuario*> remSeguidores;
        for (auto user : this->seguidores) {
            remSeguidores.push_back(user.second);
        }

        for (auto user : remSeguidores) {
            this->seguidores.erase(user->nome);
            user->seguindo.erase(this->nome);
        }
    }

    void deletar() {
        auto time_line = this->inbox.getTimeLine();
        for (auto msg : time_line) {
            if (msg->getRemetente() == this->nome) {
                msg->setDeleted();
            }
        }  
    }

    friend std::ostream& operator<<(std::ostream& os, const Usuario& user) {
        int a {0}, b {0};
        os << user.nome << "\n";
        os << "  Seguidos: [";
        for (auto& seguidos : user.seguindo) {
            if (a == 0) 
                os << seguidos.second->nome;
            else 
                os << ", " << seguidos.second->nome;
            a++;
        }
        os << "]\n";
        os << "  Seguidores: [";
        for (auto& seguidores : user.seguidores) {
            if (b == 0) 
                os << seguidores.second->nome;
            else 
                os << ", " << seguidores.second->nome;
            b++;
        }
        os << "]\n\n";
        return os;
    }

};

class Controlador {
private:
    int nextTwId;
    map<string, shared_ptr<Usuario>> usuarios;
    map<int, shared_ptr<Mensagem>> tweets;

    Mensagem* createTweet(string nome, string msg) {
        auto tweet = make_shared<Mensagem>(this->nextTwId, nome, msg);
        tweets[nextTwId] = tweet;
        this->nextTwId++;
        return tweet.get();
    }
    
public:
    Controlador() : nextTwId{0} {}

    void addUsuario(string nome) {
        auto it = usuarios.find(nome);
        if (it != usuarios.end()) {
            throw TweetException("Erro: usuario(a) ja cadastrado");
        }

        cout << nome << " foi cadastrado(a).\n";
        auto user = make_shared<Usuario>(nome);
        this->usuarios[nome] = user;
    }

    Usuario* getUser(string username) {
        auto it = usuarios.find(username);
        if(it != usuarios.end()) {
            return it->second.get();
        } else {
            throw TweetException("Usuario nao encontrado!");
        }    
    }

    void enviarTweet(string nome, string msg) {
        auto user = getUser(nome);
        auto novoTweet = this->createTweet(nome, msg);
        user->enviarTweet(novoTweet);
    }

    void enviarRt(string nome, int twId, string msg) {
        auto user = getUser(nome);
        auto tweetRt = user->getInbox().getTweet(twId);
        auto novoTweet = this->createTweet(nome, msg);
        novoTweet->setRt(tweetRt);
        user->enviarTweet(novoTweet);
    }

    void removerUsuario(string nome) {
        auto it = usuarios.find(nome);
        if (it != usuarios.end()) {
            it->second->deletar();
            it->second->rmSeguidos();
            it->second->rmSeguidores();
            this->usuarios.erase(nome);
                        
            cout << nome << " foi removido do sistema.\n";
            return;
        }

        throw TweetException("erro: usuario nao cadastrado");
    }

    friend std::ostream& operator<<(std::ostream& os, const Controlador& controle) {
        for (auto user : controle.usuarios) {
            os << *user.second;
        }
        return os;
    }
};

int main() {
    Controlador sistema; 

    while(true) {
        try {
            string cmd {};
            cin >> cmd;

            if (cmd == "exit") {
                break;
            } 
            
            else if (cmd == "add") { 
                string nome {};
                cin >> nome;
                sistema.addUsuario(nome);
            } 
            
            else if (cmd == "rm") {
                string nome {};
                cin >> nome;
                sistema.removerUsuario(nome);
            }  
            
            else if (cmd == "show") { 
                std::cout << sistema << '\n';
            } 
            
            else if (cmd == "follow") {
                string autor {}, receptor {};
                cin >> autor >> receptor;
                sistema.getUser(autor)->seguir(sistema.getUser(receptor));
            } 
            
            else if (cmd == "unfollow") { 
                string autor {}, receptor {};
                cin >> autor >> receptor;
                sistema.getUser(autor)->canseiDeSeguir(sistema.getUser(receptor));
            } 
            
            else if (cmd == "tw") { 
                string nome {}, mensagem {};
                cin >> nome;
                getline(cin, mensagem);
                sistema.enviarTweet(nome, mensagem);
            } 
            
            else if (cmd == "like") {
                string nome {};
                int id;
                cin >> nome >> id;
                sistema.getUser(nome)->like(id);
            }
            
            else if (cmd == "timeline") { 
                string nome {};
                cin >> nome;
                cout << sistema.getUser(nome)->getInbox();
            } 
            
            else if (cmd == "rt") { 
                string nome {}, mensagem {};
                int id {};
                cin >> nome >> id;
                getline(cin, mensagem);
                sistema.enviarRt(nome, id, mensagem);            
            }

            else {
                std::cout << "Error: comando invalido" << '\n';
            }

        } catch(TweetException& e) {
            cout << e.what() << endl;
        }
        cout << '\n';
    }
    return 0;
}