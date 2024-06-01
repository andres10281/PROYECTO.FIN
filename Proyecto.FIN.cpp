// Oscar Andres Mosquera Hinestroza
// Ivan Alonso Hernandez

#include <iostream>
#include <malloc.h>

using namespace std;

struct NodoViaje {
    char id[15];
    float precio;
    char destino[100];
    char matricula[10];
    char nombreEmbarcacion[100];
    int dia, mes, annio;
    int capacidad;
    char **pasajeros;
    int numPasajeros;
    NodoViaje *izq;
    NodoViaje *der;
    int altura;
};

NodoViaje *raiz = NULL;

NodoViaje* crearNodo() {
    NodoViaje *nuevoNodo = (NodoViaje *)malloc(sizeof(NodoViaje));
    if (!nuevoNodo) {
        cerr << "Error al asignar memoria" << endl;
        exit(1);
    }
    
    cout << "Ingrese el identificador del viaje: ";
    cin >> nuevoNodo->id;
    cout << "Ingrese la matricula de la embarcacion: ";
    cin >> nuevoNodo->matricula;
    cout << "Ingrese el nombre de la embarcacion: ";
    cin >> nuevoNodo->nombreEmbarcacion;
    cout << "Ingrese la fecha del viaje (dia-mes-annio): ";
    cin >> nuevoNodo->dia >> nuevoNodo->mes >> nuevoNodo->annio;
    cout << "Ingrese el precio del viaje: ";
    cin >> nuevoNodo->precio;
    cout << "Ingrese el destino del viaje: ";
    cin >> nuevoNodo->destino;
    cout << "Ingrese la capacidad de la embarcacion: ";
    cin >> nuevoNodo->capacidad;

    nuevoNodo->pasajeros = (char **)malloc(nuevoNodo->capacidad * sizeof(char *));
    if (!nuevoNodo->pasajeros) {
        cerr << "Error al asignar memoria para pasajeros" << endl;
        free(nuevoNodo);
        exit(1);
    }

    nuevoNodo->numPasajeros = 0;
    nuevoNodo->izq = NULL;
    nuevoNodo->der = NULL;
    nuevoNodo->altura = 1;

    return nuevoNodo;
}

int obtenerAltura(NodoViaje *nodo) {
    return (nodo == NULL) ? 0 : nodo->altura;
}

int mayor(int a, int b) {
    return (a > b) ? a : b;
}

int obtenerBalance(NodoViaje *nodo) {
    return (nodo == NULL) ? 0 : obtenerAltura(nodo->izq) - obtenerAltura(nodo->der);
}

NodoViaje* rotarDerecha(NodoViaje *y) {
    NodoViaje *x = y->izq;
    NodoViaje *T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    return x;
}

NodoViaje* rotarIzquierda(NodoViaje *x) {
    NodoViaje *y = x->der;
    NodoViaje *T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    return y;
}

int compararCadenas(const char *cadena1, const char *cadena2) {
    while (*cadena1 && (*cadena1 == *cadena2)) {
        cadena1++;
        cadena2++;
    }
    return *(unsigned char *)cadena1 - *(unsigned char *)cadena2;
}

void copiarCadena(char *destino, const char *origen) {
    while (*origen) {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = '\0';
}

int longitudCadena(const char *cadena) {
    int longitud = 0;
    while (*cadena) {
        longitud++;
        cadena++;
    }
    return longitud;
}

NodoViaje* insertar(NodoViaje* nodo, NodoViaje* nuevoNodo) {
    if (nodo == NULL) {
        return nuevoNodo;
    }

    if (compararCadenas(nuevoNodo->id, nodo->id) < 0) {
        nodo->izq = insertar(nodo->izq, nuevoNodo);
    } else if (compararCadenas(nuevoNodo->id, nodo->id) > 0) {
        nodo->der = insertar(nodo->der, nuevoNodo);
    } else {
        return nodo;
    }

    nodo->altura = 1 + mayor(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && compararCadenas(nuevoNodo->id, nodo->izq->id) < 0) {
        return rotarDerecha(nodo);
    }

    if (balance < -1 && compararCadenas(nuevoNodo->id, nodo->der->id) > 0) {
        return rotarIzquierda(nodo);
    }

    if (balance > 1 && compararCadenas(nuevoNodo->id, nodo->izq->id) > 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && compararCadenas(nuevoNodo->id, nodo->der->id) < 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

NodoViaje* minValueNode(NodoViaje* nodo) {
    NodoViaje* current = nodo;

    while (current->izq != NULL)
        current = current->izq;

    return current;
}

NodoViaje* eliminar(NodoViaje* nodo, const char* id) {
    if (nodo == NULL)
        return nodo;

    if (compararCadenas(id, nodo->id) < 0)
        nodo->izq = eliminar(nodo->izq, id);
    else if (compararCadenas(id, nodo->id) > 0)
        nodo->der = eliminar(nodo->der, id);
    else {
        if ((nodo->izq == NULL) || (nodo->der == NULL)) {
            NodoViaje *temp = nodo->izq ? nodo->izq : nodo->der;

            if (temp == NULL) {
                temp = nodo;
                nodo = NULL;
            } else
                *nodo = *temp;
            free(temp);
        }

        if (nodo == NULL)
            return nodo;

        nodo->altura = 1 + mayor(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

        int balance = obtenerBalance(nodo);

        if (balance > 1 && obtenerBalance(nodo->izq) >= 0)
            return rotarDerecha(nodo);

        if (balance > 1 && obtenerBalance(nodo->izq) < 0) {
            nodo->izq = rotarIzquierda(nodo->izq);
            return rotarDerecha(nodo);
        }

        if (balance < -1 && obtenerBalance(nodo->der) <= 0)
            return rotarIzquierda(nodo);

        if (balance < -1 && obtenerBalance(nodo->der) > 0) {
            nodo->der = rotarDerecha(nodo->der);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    return nodo;
}

NodoViaje* buscar(NodoViaje* nodo, const char* id) {
    if (nodo == NULL || compararCadenas(nodo->id, id) == 0)
        return nodo;

    if (compararCadenas(nodo->id, id) < 0)
        return buscar(nodo->der, id);

    return buscar(nodo->izq, id);
}

int inOrden(NodoViaje *nodo) {
    if (nodo != NULL) {
        inOrden(nodo->izq);
        cout << nodo->id << " - " << nodo->destino << endl;
        inOrden(nodo->der);
    }
    return 0;
}

int registrarPasajero(NodoViaje* nodo, const char* id, const char* pasajero) {
    NodoViaje* nodoViaje = buscar(nodo, id);
    if (nodoViaje != NULL) {
        if (nodoViaje->numPasajeros < nodoViaje->capacidad) {
            nodoViaje->pasajeros[nodoViaje->numPasajeros] = new char[longitudCadena(pasajero) + 1];
            copiarCadena(nodoViaje->pasajeros[nodoViaje->numPasajeros], pasajero);
            nodoViaje->numPasajeros++;
            cout << "Pasajero registrado correctamente." << endl;
        } else {
            cout << "Capacidad del viaje alcanzada." << endl;
        }
    } else {
        cout << "Viaje no encontrado." << endl;
    }
    return 0;
}

int listarPasajeros(NodoViaje* nodo, const char* id) {
    NodoViaje* nodoViaje = buscar(nodo, id);
    if (nodoViaje != NULL) {
        cout << "Pasajeros del viaje " << id << ":" << endl;
        for (int i = 0; i < nodoViaje->numPasajeros; i++) {
            cout << nodoViaje->pasajeros[i] << endl;
        }
    } else {
        cout << "Viaje no encontrado." << endl;
    }
    return 0;
}

void liberarNodo(NodoViaje* nodo) {
    if (nodo == NULL) return;
    liberarNodo(nodo->izq);
    liberarNodo(nodo->der);
    delete[] nodo->pasajeros;
    delete nodo;
}

int main() {
    int opc = 0;
    do {
        cout << "Menu de opciones" << endl;
        cout << "1. Registrar un nuevo viaje" << endl;
        cout << "2. Buscar un viaje por identificador" << endl;
        cout << "3. Listar todos los viajes" << endl;
        cout << "4. Eliminar un viaje por identificador" << endl;
        cout << "5. Registrar un pasajero en un viaje" << endl;
        cout << "6. Listar todos los pasajeros de un viaje" << endl;
        cout << "7. Salir" << endl;
        cin >> opc;
        char id[15];
        char pasajero[100];
        NodoViaje *nuevoNodo;
        switch (opc) {
            case 1:
                nuevoNodo = crearNodo();
                raiz = insertar(raiz, nuevoNodo);
                break;
            case 2:
                cout << "Ingrese el identificador del viaje: ";
                cin >> id;
                nuevoNodo = buscar(raiz, id);
                if (nuevoNodo != NULL) {
                    cout << "Viaje encontrado: " << nuevoNodo->nombreEmbarcacion << " a " << nuevoNodo->destino << " el " << nuevoNodo->dia << "/" << nuevoNodo->mes << "/" << nuevoNodo->annio << endl;
                } else {
                    cout << "Viaje no encontrado." << endl;
                }
                break;
            case 3:
                cout << "Listado de viajes:" << endl;
                inOrden(raiz);
                break;
            case 4:
                cout << "Ingrese el identificador del viaje a eliminar: ";
                cin >> id;
                raiz = eliminar(raiz, id);
                cout << "Viaje eliminado correctamente." << endl;
                break;
            case 5:
                cout << "Ingrese el identificador del viaje: ";
                cin >> id;
                cout << "Ingrese el nombre del pasajero: ";
                cin >> pasajero;
                registrarPasajero(raiz, id, pasajero);
                break;
            case 6:
                cout << "Ingrese el identificador del viaje: ";
                cin >> id;
                listarPasajeros(raiz, id);
                break;
        }
    } while (opc != 7);

    return 0;
}

