#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Inventario de la tienda
struct Producto {
    string nombre;
    double precio;
    int cantidad; // Esto es el stock disponible
};

// Para guardar lo que el cliente lleva en su carrito
struct ItemCarrito {
    string nombre;
    double precioUnitario;
    int cantidad;
    double subtotalOriginal;
    double subtotalConDescuento;
};

// Función para descuento del 20% por llevar una decena (10 o más)
double aplicarDescuentoPorDecena(double precioOriginal, int cantidadComprada) {
    if (cantidadComprada >= 10) {
        return precioOriginal * 0.80; // Se cobra solo el 80% (20% de descuento)
    }
    return precioOriginal; // Si son menos de 10, no hay descuento aquí
}

// Función para generar el ticket con el nombre y los descuentos
void generarTicket(string nombreCliente, const vector<ItemCarrito>& carrito, double sumaSubtotales) {
    ofstream archivoSalida("ticket_estudiante.txt");
    
    if (!archivoSalida.is_open()) {
        cout << "Error al crear el ticket." << endl;
        return;
    }

    // Descuento de 15% extra si el total es mayor de 100 soles
    double descuentoGeneral = 0;
    double totalFinal = sumaSubtotales;
    
    if (sumaSubtotales > 100.0) {
        descuentoGeneral = sumaSubtotales * 0.15;
        totalFinal = sumaSubtotales - descuentoGeneral;
    }

    // Diseño del ticket
    archivoSalida << "_______________________________________\n";
    archivoSalida << "            TICKET DE COMPRA    \n";
    archivoSalida << "_______________________________________\n";
    archivoSalida << "Cliente: " << nombreCliente << "\n";
    archivoSalida << "---------------------------------------\n";
    
    // Detalle de cada producto comprado
    for (const auto& item : carrito) {
        archivoSalida << item.nombre << " x" << item.cantidad << "\n";
        archivoSalida << "  Precio c/u: S/ " << item.precioUnitario << "\n";
        
        // Si el subtotal cambió, entonces se aplicó el descuento de la decena
        if (item.subtotalConDescuento < item.subtotalOriginal) {
            archivoSalida << "  Subtotal original: S/ " << item.subtotalOriginal << "\n";
            archivoSalida << "  Subtotal (Aplica -20% por decena): S/ " << item.subtotalConDescuento << "\n";
        } else {
            archivoSalida << "  Subtotal: S/ " << item.subtotalConDescuento << "\n";
        }
        
        // espacio para que no este muy junto
        archivoSalida << "\n"; 
    }
    
    archivoSalida << "---------------------------------------\n";
    archivoSalida << "SUBTOTAL BRUTO: S/ " << sumaSubtotales << "\n";
    
    // Se muestra descuento si se gastó más de 100 soles
    if (descuentoGeneral > 0) {
        archivoSalida << "DESCUENTO EXTRA (-15% por > S/ 100): -S/ " << descuentoGeneral << "\n";
    }
    
    archivoSalida << "TOTAL A PAGAR: S/ " << totalFinal << "\n";
    archivoSalida << "=======================================\n";
    archivoSalida << "¡Gracias por su compra:3 Que tenga buen día!\n";
    
    cout << "\nCompra finalizada. Su ticket se guardo en ticket_estudiante.txt .\n";
    
    archivoSalida.close();
}

int main() {
    ifstream archivoEntrada("productos.txt");
    
    if (!archivoEntrada.is_open()) {
        cout << "Error: Uy, No se encontro productos.txt." << endl;
        return 1;
    }
    
    vector<Producto> inventario;
    Producto p;
    
    while (archivoEntrada >> p.nombre >> p.precio >> p.cantidad) {
        inventario.push_back(p);
    }
    archivoEntrada.close();
    
  
    // INTERACCIÓN CON EL COMPRADOR
    
    string nombreUsuario;
    cout << "=======================================\n";
    cout << "         LIBRERIA AESTHETIC        \n";
    cout << "=======================================\n";
    cout << "Ingrese su primer nombre para la boleta: ";
    cin >> nombreUsuario; // Leemos el nombre de nuestro cliente
    
    int opcion;
    int cantidadDeseada;
    vector<ItemCarrito> carrito; 
    double sumaSubtotales = 0;
    
    cout << "\nHolaa " << nombreUsuario << ", estos son nuestros productos disponibles;3 :\n";
    
    for (size_t i = 0; i < inventario.size(); i++) {
        cout << i + 1 << ". " << inventario[i].nombre 
             << " (S/ " << inventario[i].precio << ") - Stock: " 
             << inventario[i].cantidad << "\n";
    }
    
    while (true) {
        cout << "\nIngresa el numero del producto (0 para terminar): ";
        cin >> opcion;
        
        if (opcion == 0) break;
        
        if (opcion > 0 && opcion <= inventario.size()) {
            cout << "Cuantas unidades de " << inventario[opcion-1].nombre << " deseas?: ";
            cin >> cantidadDeseada;
            
            // Verificamos que la tienda tenga stock suficiente
            if (cantidadDeseada <= inventario[opcion-1].cantidad) {
                
                // Cálculos matemáticos del producto
                double precioBase = inventario[opcion-1].precio;
                double subtotalOriginal = precioBase * cantidadDeseada;
                
                // Aplicamos la regla de descuento por la decena
                double precioConDescuento = aplicarDescuentoPorDecena(precioBase, cantidadDeseada);
                double subtotalConDescuento = precioConDescuento * cantidadDeseada;
                
                // Empaquetamos todo 
                ItemCarrito item;
                item.nombre = inventario[opcion-1].nombre;
                item.precioUnitario = precioBase;
                item.cantidad = cantidadDeseada;
                item.subtotalOriginal = subtotalOriginal;
                item.subtotalConDescuento = subtotalConDescuento;
                
                // Guardamos en el carrito y sumamos al acumulador
                carrito.push_back(item);
                sumaSubtotales += subtotalConDescuento;
                
                // Restamos el stock del inventario
                inventario[opcion-1].cantidad -= cantidadDeseada;
                
                cout << " [Agregado] " << cantidadDeseada << " x " << item.nombre << " al carrito.\n";
            } else {
                cout << "Lamentamos mucho: No hay suficiente stock. Solo quedan " << inventario[opcion-1].cantidad << ".\n";
            }
        } else {
            cout << "Opcion no valida.\n";
        }
    }
    
    // Generamos el ticket enviando el nombre del comprador y su total
    if (!carrito.empty()) {
        generarTicket(nombreUsuario, carrito, sumaSubtotales);
    } else {
        cout << "No se realizo ninguna comprita.\n";
    }
    
    return 0;
}