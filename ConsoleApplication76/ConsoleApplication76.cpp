#include <iostream>
#include <bitset>
#include <string>

using namespace std;

// Función para imprimir un número en binario con n bits
void print(int num, int n) {
    string binario = bitset<64>(num).to_string().substr(64 - n, n);
    cout << binario << endl;
}

// Función para convertir decimal a binario con n bits
int decimal_a_binario(int decimal, int n) {
    return decimal & ((1 << n) - 1);
}

// Función para obtener el complemento a 2 de un número con n bits
int complemento_2(int num, int n) {
    return decimal_a_binario(~num + 1, n);
}

// Función para sumar dos números binarios con n bits
int adicion_binaria(int num1, int num2, int n) {
    int resultado = num1 + num2;
    return resultado & ((1 << n) - 1);
}

// Función para restar dos números binarios con n bits
int subtraccion_binaria(int minuendo, int subtraendo, int n) {
    subtraendo = complemento_2(subtraendo, n);
    return adicion_binaria(minuendo, subtraendo, n);
}

// Función para realizar corrimiento aritmético a la derecha
void corrimiento_aritmetico(int& A, int& Q, int& Qn_1, int n) {
    int nuevo_Qn_1 = Q & 1;
    Q = (Q >> 1) | ((A & 1) << (n - 1));
    A = (A >> 1) | ((A & (1 << (n - 1))) ? (1 << (n - 1)) : 0);
    Qn_1 = nuevo_Qn_1;
}

// Función para multiplicar dos enteros con el algoritmo de Booth
int multiplicacion(int multiplicando, int multiplicador, int n) {
    int A = 0;
    int Q = decimal_a_binario(multiplicador, n);
    int M = decimal_a_binario(multiplicando, n);
    int Qn_1 = 0;

    cout << "Estado inicial:\n";
    cout << "A: "; print(A, n);
    cout << "Q: "; print(Q, n);
    cout << "Q-1: " << Qn_1 << "\n";
    cout << "M: "; print(M, n);
    cout << endl;

    for (int i = 0; i < n; ++i) {
        int Q0 = Q & 1;
        cout << "Iteracion " << i + 1 << ":\n";
        if (Q0 == 1 && Qn_1 == 0) {
            cout << "Q0 y Q-1 = 10, realizar A = A - M\n";
            A = subtraccion_binaria(A, M, n);
        }
        else if (Q0 == 0 && Qn_1 == 1) {
            cout << "Q0 y Q-1 = 01, realizar A = A + M\n";
            A = adicion_binaria(A, M, n);
        }
        else {
            cout << "Q0 y Q-1 = " << Q0 << Qn_1 << ", no se realiza operacion\n";
        }
        corrimiento_aritmetico(A, Q, Qn_1, n);

        cout << "A: "; print(A, n);
        cout << "Q: "; print(Q, n);
        cout << "Q-1: " << Qn_1 << "\n";
        cout << endl;
    }

    int resultado = (A << n) | Q;
    if ((multiplicando < 0) ^ (multiplicador < 0)) {
        resultado = complemento_2(resultado, 2 * n);
    }
    return resultado;
}

int main() {
    int n;
    int multiplicando;
    int multiplicador;

    cout << "Ingrese la cantidad de bits: ";
    cin >> n;

    // Validar que la cantidad de bits sea razonable
    if (n <= 0 || n > 32) {
        cout << "Cantidad de bits no valida. Debe estar entre 1 y 32." << endl;
        return 1;
    }

    cout << "Ingrese el multiplicando: ";
    cin >> multiplicando;
    cout << "Ingrese el multiplicador: ";
    cin >> multiplicador;

    // Validar que los números ingresados no excedan los n bits
    if (multiplicando >= (1 << (n - 1)) || multiplicando < -(1 << (n - 1)) ||
        multiplicador >= (1 << (n - 1)) || multiplicador < -(1 << (n - 1))) {
        cout << "Los numeros ingresados exceden el rango permitido para " << n << " bits." << endl;
        return 1;
    }

    int resultado = multiplicacion(multiplicando, multiplicador, n);

    cout << "El resultado en decimal es: " << resultado << endl;
    cout << "El resultado en binario es: ";
    print(resultado, 2 * n);

    return 0;
}
