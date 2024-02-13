
// Compila utilizando g++ main.cpp -o main -ljsoncpp
#include <fstream>
#include <iostream>
#include <json/json.h>

using namespace std;

int main(int argc, char **argv) {
    // Cogememos las rutas de los argumentos de entrada
    if (argc < 3) {
        cerr << "Usage: " << argv[0]
             << " <json path (input)> <csv path (output)>" << endl;
        return 1;
    }
    string in_path = argv[1];
    string out_path = argv[2];

    // Leemos el archivo de entrada
    ifstream in_file(in_path);
    if (!in_file.is_open()) {
        cerr << "Error: could not open file " << in_path << endl;
        return 1;
    }
    Json::Value root;
    in_file >> root;
    in_file.close();

    // Abrimos el archivo de salida
    ofstream out_file(out_path);
    if (!out_file.is_open()) {
        cerr << "Error: could not open file " << out_path << endl;
        return 1;
    }

    string f_name;
    string f_long;
    string f_lat;
    // string f_desc;
    Json::Value fountain;

    out_file << "WKT,nombre" << endl;

    // Recorremos los elementos 
    for (Json::Value::ArrayIndex i = 0; i < root.size(); i++) {

        /* Para no superar el limite de elementos
            crearemos otro fichero de salida cada 1000 fuentes*/
        if (i % 1000 == 0 && i != 0) { // might be useful to be able to toggle this using args
            out_file.close();
            auto dot_pos = out_path.find_last_of(".");
            if (i != 1000) {
                out_path = out_path.substr(0, dot_pos - 1) +
                           to_string(i / 1000 + 1) +
                           out_path.substr(dot_pos);
            } else
                out_path = out_path.substr(0, dot_pos) + "_2" +
                           out_path.substr(dot_pos);
            out_file.open(out_path);
            out_file << "WKT,nombre" << endl;
        }

        fountain = root[i];
        f_name = fountain["ID"].asString() + " " +
                 fountain["DIRECCION_AUX"].asString();
        f_long = fountain["LONGITUD"].asString();
        f_lat = fountain["LATITUD"].asString();
        
        // Escribimos en el fichero

        if (fountain["ESTADO"].asString() != "NO_OPERATIVO"|| fountain["ESTADO"].asString() != "CERRADA_TEMPORALMENT")
            out_file << "\"POINT(" << f_long << " " << f_lat << ")\""
                     << "," << f_name << endl
                     << endl;
    }

    out_file.close();

    //No fumen
    cout << "Done!" << endl;

    return 0;
}
