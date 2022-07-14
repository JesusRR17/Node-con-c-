#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <Windows.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <ctime>
#include <chrono>

using namespace std;
//funciones para dividir strings por espacio y $
void SplitString_space(string s, vector<string> &v){
	
	string temp = "";
	for(int i=0;i<s.length();++i){
		
		if(s[i]==' '){
			v.push_back(temp);
			temp = "";
		}
		else{
			temp.push_back(s[i]);
		}
		
	}
	v.push_back(temp);
}
void SplitString_peso(string s, vector<string> &v){
	
	string temp = "";
	for(int i=0;i<s.length();++i){
		
		if(s[i]=='$'){
			v.push_back(temp);
			temp = "";
		}
		else{
			temp.push_back(s[i]);
		}
		
	}
	v.push_back(temp);
}
//Función para sacar el balance del archivo
long long int balance(){
    long long int monto_ab;
    string m;
    ifstream bal("Balance.txt");
    getline(bal,m);
    m.erase(remove(m.begin(),m.end(),'.'),m.end());
    monto_ab = stoll(m);
    cout << monto_ab << endl;
    return monto_ab;
}

//Función para escribir en el archivo de signals.txt
void WriteSignal(stringstream& ss){
    ofstream foutput; 
    ifstream finput;
    finput.open ("Signals.txt");
    foutput.open ("Signals.txt",ios::app); 

    if(finput.is_open())
        foutput<<  ss.str(); 
    finput.close();
    foutput.close(); 
}

// Función que revisa si ApprovedTXNs.txt fue modificado y otras cosas
long long int ApprovedTXNs(int cnol, long long int monto_dca){//Pasamos el numero de filas de la funcion act_ATXNs() y el monto de dca
    string line;
    stringstream sign;
    vector<string> atxns,split, mon;
    int nol = 0;
    long long int monto_t;
    ifstream atx("ApprovedTXNs.txt");
    //Volvemos a contabilizar las lineas
    while (getline(atx,line)){
        ++nol;
        atxns.push_back(line);
    }
    //comparamos si aumentó respecto a la función act_ATXNs()
    if (nol == cnol){
        //Si no hubo cambios no hace nada
    }else{
        //Si hubo algún cambio
        auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        SplitString_space(atxns.back(),split);
        sign << "Sent " << split[2] << " to " << split[0]; // Guardamos el mensaje que se imprime en consola y en el archivo Signals.txt
        cout << sign.str() << "\n"; //Imprime en consola la última fila del archivo
        sign << " at " << ctime(&datetime);
        split[2].erase(remove(split[2].begin(),split[2].end(),'.'),split[2].end());
        SplitString_peso(split[2],mon);
        monto_t = stoll(mon[1]);
        monto_dca = monto_dca + monto_t; //Sumamos el monto de la linea al monto delta con aprobación
        WriteSignal(sign); //Escribimos en el archivo signals.txt
    }
    return(monto_dca); //Regresamos monto_dca para seguir actualizandola
}

//Función que revisa cuantas lineas tiene approvedtxns.txt
int act_ATXNs(){
    string line;
    int nol = 0;
    ifstream atx("ApprovedTXNs.txt");
    while (getline(atx,line)){
        ++nol;
    }
    return nol;
}

int main(){
    long long int  monto_ab, monto_db = 0, monto_dca = 0, monto_dsa = 0;
    int ms = 5000;
    stringstream mensaje, guiones;
    while(1){
        auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        //Revisamos la cantidad de filas que contiene el archivo ApprovedTXNs.txt
        int cnol = act_ATXNs();
        //Esperamos cada 500 milisegundos
        Sleep(ms);
        // Ejecutamos la función que revisa si ApprovedTXNs.txt fue modificado y actualizamos el monto dca
        monto_dca = ApprovedTXNs(cnol, monto_dca); 
        monto_dsa = monto_db - monto_dca;
        //Imprimimos el monto del delta aprobado y delta no aprobado en la consola y en el archivo Signals.txt
        mensaje << "[Delta aprobado]: " << monto_dca << " - " << " [Delta no aprobado]: " << monto_dsa<< " at " << ctime(&datetime);
        cout <<  mensaje.str();
        WriteSignal(mensaje);
        //Regresamos el monto de delta con aprobación y delta del balance a 0
        monto_dca = 0;
        monto_db = 0;
        //Imprimimos una fila de guiones en la consola y en el archivo Signals.txt
        guiones << "-------------------------------------------------------------------------\n";
        cout << guiones.str();
        WriteSignal(guiones);
        mensaje.str("");
        guiones.str("");
    }
    return 0;
}