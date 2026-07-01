#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Los datos de los estudiantes:
struct Estudiante {
    string nombre;
    string apellido;
    int codigo;
    string carrera;
    double nota;
};

// Ingresar datos manualmente y crear el archivo
void ingresarEstudiantes() {
    ofstream archivo("estudiantes.txt", ios::app); 
    
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo estudiantes.txt\n";
        return;
    }

    int cantidad;
    cout << "\n¿Cuántos alumnos vas a registrar ?: ";
    cin >> cantidad;

    for (int i = 0; i < cantidad; i++) {
        Estudiante e;
        cout << "\n--- Datos del Estudiante " << i + 1 << " ---\n";
        cout << "Nombre: "; cin >> e.nombre;
        cout << "Apellido: "; cin >> e.apellido;
        cout << "Código: "; cin >> e.codigo;
        cout << "Carrera (Ciberseguridad/Computacion/Software): "; cin >> e.carrera;
        cout << "Nota (0 a 20): "; cin >> e.nota;

        // Guardamos en el txt con espacios entre cada dato
        archivo << e.nombre << " " << e.apellido << " " << e.codigo << " " << e.carrera << " " << e.nota << "\n";
    }
    
    archivo.close();
    cout << "\n¡" << cantidad << " estudiantes guardados en 'estudiantes.txt' exitosamente!\n";
}

// Diseño del reporte, con lista de aprobados
void generarReporte(const vector<Estudiante>& lista, double promGeneral, double promCiber, double promComp, double promSoft) {
    ofstream archivoSalida("reporte_notas.txt");
    
    if (!archivoSalida.is_open()) {
        cout << "Error al crear el archivo de reporte." << endl;
        return;
    }

    archivoSalida << "___________________________________________________\n";
    archivoSalida << "  REPORTE COMPLETO DE LA FACULTAD DE MATEMATICA\n";
    archivoSalida << "___________________________________________________\n";
    archivoSalida << "PROMEDIO GENERAL: " << promGeneral << "\n";
    archivoSalida << "---------------------------------------------------\n";
    archivoSalida << "Promedio de la carrera de Ciberseguridad: " << promCiber << "\n";
    archivoSalida << "Promedio de la carrera de Computacion: " << promComp << "\n";
    archivoSalida << "Promedio de la carrera de Software: " << promSoft << "\n";
    archivoSalida << "___________________________________________________\n\n";
    
    // Aquí agregamos los nombres y si aprobaron o no
    archivoSalida << "DETALLE DE ALUMNOS:\n";
    archivoSalida << "---------------------------------------------------\n";
    for (size_t i = 0; i < lista.size(); i++) {
        // Determinamos si aprobó (asumiendo 11 como nota mínima)
        string estado = (lista[i].nota >= 11.0) ? "APROBADO" : "DESAPROBADO";
        
        archivoSalida << "- " << lista[i].nombre << " " << lista[i].apellido 
                      << " | Carrera: " << lista[i].carrera 
                      << " | Nota: " << lista[i].nota 
                      << " -> [" << estado << "]\n";
    }
    
    cout << "\nReporte completo (con lista de alumnos) generado en reporte_notas.txt.\n";
    
    archivoSalida.close();
}

int main() {
    // Preguntamos al usuario si quiere agregar la base de datos primero
    char opcionIngreso;
    cout << "¿Deseas registrar nuevos estudiantes antes de ver el menú? (s/n): ";
    cin >> opcionIngreso;
    
    if (opcionIngreso == 's' || opcionIngreso == 'S') {
        ingresarEstudiantes();
    }

    // Ahora el programa sigue su curso normal leyendo el archivo
    ifstream archivoEntrada("estudiantes.txt");
    
    if (!archivoEntrada.is_open()) {
        cout << "Error: No se encontró el archivo estudiantes.txt. Asegúrate de registrar alumnos primero." << endl;
        return 1; 
    }
    
    vector<Estudiante> lista;
    Estudiante e;
    
    double sumaGeneral = 0;
    int contGeneral = 0;
    double sumaCiber = 0, sumaComp = 0, sumaSoft = 0;
    int contCiber = 0, contComp = 0, contSoft = 0;
    
    // El programa lee el archivo y agrupa las notas
    while (archivoEntrada >> e.nombre >> e.apellido >> e.codigo >> e.carrera >> e.nota) {
        lista.push_back(e);
        sumaGeneral += e.nota;
        contGeneral++;
        
        if (e.carrera == "Ciberseguridad") {
            sumaCiber += e.nota;
            contCiber++;
        } 
        else if (e.carrera == "Computacion") {
            sumaComp += e.nota;
            contComp++;
        } 
        else if (e.carrera == "Software") {
            sumaSoft += e.nota;
            contSoft++;
        }
    }
    archivoEntrada.close();
    
    // Procesos para calcular los promedios:
    double promGeneral = (contGeneral > 0) ? (sumaGeneral / contGeneral) : 0;
    double promCiber = (contCiber > 0) ? (sumaCiber / contCiber) : 0;
    double promComp = (contComp > 0) ? (sumaComp / contComp) : 0;
    double promSoft = (contSoft > 0) ? (sumaSoft / contSoft) : 0;
    
    
    //      MENÚ DE PROMEDIOS DE LA FACULTAD 
    int opcion;
    
    // Para repetir el menú hasta que el usuario decida salir
    do {
        cout << "\n=================================================\n";
        cout << "   SISTEMA DE NOTAS - FACULTAD DE MATEMATICA     \n";
        cout << "==================================================\n";
        cout << "1. Ver Promedio General\n";
        cout << "2. Ver Promedio de Ciberseguridad\n";
        cout << "3. Ver Promedio de Computacion\n";
        cout << "4. Ver Promedio de Software\n";
        cout << "5. Guardar REPORTE COMPLETO en .txt y Salir\n";
        cout << "6. Salir sin guardar\n";
        cout << "===================================================\n";
        cout << "Ingresa una opcion (1-6): ";
        cin >> opcion;
        
        // El switch evalúa que numero ingresó el usuario y ejecuta solo esa parte
        switch(opcion) {
            case 1:
                cout << "\n-> El promedio general es: " << promGeneral << "\n";
                break;
            case 2:
                cout << "\n-> El promedio de la carrera de CIBERSEGURIDAD es: " << promCiber << "\n";
                break;
            case 3:
                cout << "\n-> El promedio de la carrera de COMPUTACION es: " << promComp << "\n";
                break;
            case 4:
                cout << "\n-> El promedio de la carrera de SOFTWARE es: " << promSoft << "\n";
                break;
            case 5:
                // Si elige 5, se ejecuta la función para crear el archivo 
                generarReporte(lista, promGeneral, promCiber, promComp, promSoft);
                cout << "Saliendo del sistema...\n";
                break;
            case 6:
                cout << "Saliendo del sistema sin generar reporte...\n";
                break;
            default:
                // Si ingresa un número como 8 o 9, le avisamos del error
                cout << "\nERROR: Opción no válida. Intente de nuevo por favor.\n";
                break;
        }
        
    // El menú seguira apareciendo mientras la opción no sea 5 ni 6
    } while (opcion != 5 && opcion != 6);
    
    return 0;
}
