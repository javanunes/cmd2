#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/*
 *  CMD 2 Malicioso do JavaNunes para Linux
 *  um cmd que funciona como um frontend
 *  para coisas sacaninhas especiais...
 *  Author: javanunes.com@prontonmail.com
 *
 */

using namespace std;

class Prompt {
 public:

    string resultadoExecucao(string cmd){
        FILE *pipe = nullptr;
        char buffer[1024];
        pipe = popen(cmd.c_str(), "r");
        if(pipe)
        {
           int i = 0;
           char letra;
           while((letra = fgetc(pipe)) != EOF)
           {
               buffer[i] = letra;
               i++;
           }
           buffer[i] = '\0';
           return buffer;
        }
        return "...";
    }

    string homePath(){
        const char *homedir;
        if ((homedir = getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;
        }
        return homedir;
    }

    void salvaLog(string log){
        if(!log.empty()){
         string arquivoLog = this->homePath() + "/.historico.log";
         ofstream out(arquivoLog, ios::app);
         out << __DATE__ << "," << __TIME__ << ":: " + log+"\n";
         out.close();
        }
    }

    string pegaRetornaIP(){
        string texto = this->resultadoExecucao("nslookup -querytype=TXT o-o.myaddr.l.google.com ns1.google.com");
        int position,positionAspas =0;
        string recorte1="";
        string recorteFinal="";
        position = texto.find("text = \"") + 8;
        recorte1 = texto.substr(position);
        positionAspas = recorte1.find("\"");
        recorteFinal = recorte1.substr(0, positionAspas);
        return recorteFinal;
    }

    string ipSemUltimaParte(string ip){
        int ultimoPonto = 0;
        ultimoPonto = ip.find_last_of(".");
        return ip.substr(0,ultimoPonto);
    }

    void nmap1(string porta){
        string ipCortado = this->ipSemUltimaParte(this->pegaRetornaIP());
        string ipUltimaParte =".0-255";
        string ipFinal = ipCortado+ipUltimaParte;
        string comando = "nmap "+ipFinal + " -p"+porta+" --open";
        cout << "Alvo: "+comando << endl;
        system(comando.c_str());
    }

    void banner(){
        cout << "\nMicrosoft Windows [Version 10.0.14393]\n";
        cout << "(c) 2016 Microsoft Corporation. All rights reserved.\n\n";
    }

    void ipconfig(string ip){
        cout << "\nWindows IP configuration\n"<<endl;
        cout << "Ethernet adapter Local Area Cconnection 1:\n"<<endl;
        cout << "       Connection-specific DNS suffix...: dugway.gov"<<endl;
        cout << "       IP Address.......................: "+ ip<<endl;
        cout << "       Subnet Mask......................: 0.0.0.0"<<endl;
        cout << "       Default gateway..................: 192.168.1.1\n"<<endl;
    }

    void bloqueiaIP(string ip){
        string comando1="sudo iptables -A OUTPUT -d "+ip+" -p tcp -j DROP";
        string comando2="sudo iptables -I INPUT -p tcp  -s  "+ip+" -m state --state NEW -j DROP";
        string comando3="sudo service iptables save";
        system(comando1.c_str());
        system(comando2.c_str());
        system(comando3.c_str());
        cout << ip << " bloqueado pra sempre!"<<endl;
    }


    void ppt(string volume){
        cout << volume << "\\WINDOWS\\system32>";
    }

    int interpretador(string comandos){
        int r =0;
        this->salvaLog(comandos);
        if(comandos == "..")
        {
            return 1;
        }
        if(comandos.empty()){
            return 0;
        }
        if(comandos == "exit" || comandos == "quit" || comandos == "sair"){
            exit(1);
        }
        if(comandos == "n"){
           this->nmap1("445");
           comandos="";
        }

        if(comandos == "vnc"){
           this->nmap1("5900");
           comandos="";
        }

        if(comandos == "w"){
           this->nmap1("80");
           comandos="";
        }

        if(comandos == "ipconfig"){
           this->ipconfig(this->pegaRetornaIP());
           comandos="";
        }

        if(comandos == "ftp"){
           this->nmap1("21");
           comandos="";
        }

        if(comandos == "cls"){
           r = system("clear");
           this->banner();
           comandos="";
        }

        if(comandos == "bloqueia" || comandos == "bloquear" || comandos== "block"){
           cout << "IP/Host a ser bloqueado:";
           getline(cin, comandos);
           if(!comandos.empty()){
               this->bloqueiaIP(comandos);
           }
           comandos="";
        }


        if(comandos == ""){
            return 0;
        }
        r = system(comandos.c_str());
        if (r > 0){
            cout << "command not found\n";
        }
        comandos="";
        return r;
    }
};



int main()
{
    int go = 1;
    Prompt prompt;
    string entrada;
    prompt.banner();
    while(go > 0){
       prompt.ppt("C:");
       getline(cin, entrada); // entrada;
       prompt.interpretador(entrada);
    }
    return 0;
}
