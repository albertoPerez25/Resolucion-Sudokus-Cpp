#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
using namespace std;

string getRuta(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "NOTA: Puede pasarse la ruta como parametro: python3 main.py <ruta/al/sudoku>\nSe usará el sudoku predefinido\n";
        return "/home/calberto/Documents/Uni/6toCuatri/DAlgoritmos/Practicas/P4/Resolucion-Sudokus-Cpp/sudokus/test.txt";
    }
    return argv[1];
}

void getSudoku(string ruta, ifstream *pArchivo)
{
    // ifstream archivo(ruta);
    pArchivo->open(ruta);

    if (!pArchivo->is_open())
    {
        cerr << "No se ha podido leer el sudoku en " << ruta << endl;
        exit(1);
    }

    return;
}

/*
Obtiene el cuadrante para un índice del array del sudoku
*/
int getCuadrante(int n)
{
    // Calcular las coordenadas fila y columna
    int fila = n / 9;
    int columna = n % 9;

    // Determinar el cuadrante usando la fila y columna
    int cuadranteFila = fila / 3;
    int cuadranteColumna = columna / 3;

    // El cuadrante final es la combinación de ambos
    int cuadrante = cuadranteFila * 3 + cuadranteColumna;

    return cuadrante;
}

/*
Devuelve una lista de listas con el sudoku[fila][columna]
*/
void getArraySudoku(string ruta, char *pSudoku, unordered_set<char> *pFilas, unordered_set<char> *pColumnas, unordered_set<char> *pCuadrantes)
{
    int i = 0;
    char caracter;
    ifstream archivo;
    getSudoku(ruta, &archivo);
    while (caracter != -1)
    {
        caracter = archivo.get();
        if (caracter != '\n' and caracter != -1)
        {
            pSudoku[i] = caracter;
            if (caracter != ' ')
            {
                pFilas[i / 9].insert(caracter);
                pColumnas[i % 9].insert(caracter);
                pCuadrantes[getCuadrante(i)].insert(caracter);
            }
            i++;
        }
        else if ((i) % 9 != 0)
        {
            cerr << "ERROR: El sudoku dado no tiene el formato adecuado (9x9)\n";
            exit(2);
        }
    }
    archivo.close();
}

void printSudoku(char sudoku[])
{
    cout << "\n-----------------------------------\n";
    for (int i = 0; i<81; i++)
    {   
        cout << sudoku[i];
        if ((i+1) % 3 == 0)
            cout << " | ";
        else
            cout << " . ";
        if ((i+1) % 9 == 0)
        {
            cout << endl;
            if ((i+1) % (9*3) == 0)
                cout << "-----------------------------------\n";
        }
    }
}

int getSolucionSudoku(char *pSudoku, int posicion, unordered_set<char> *pFilas, unordered_set<char> *pColumnas, unordered_set<char> *pCuadrantes)
{
    if (pSudoku[posicion] != ' ')
    {
        if (posicion != 80)
            return getSolucionSudoku(pSudoku,posicion+1,pFilas,pColumnas,pCuadrantes);
        return 1;
    }
    else
    {
        char numeros[] = {'1','2','3','4','5','6','7','8','9'};
        int cuadrante_index;
        int sol;
        int i;

        for (i = 0; i<9; i++)
        {
            if ((pFilas[posicion / 9].count(numeros[i]) + pColumnas[posicion % 9].count(numeros[i])) == 0) // Comprobamos que no exista en la fila y columna
            {
                cuadrante_index = getCuadrante(posicion);
                if (pCuadrantes[cuadrante_index].count(numeros[i]) == 0) // Comprobamos cuadrante
                {
                    pSudoku[posicion] = numeros[i]; // Actualizamos el sudoku

                    // Actualizamos los conjuntos
                    pFilas[posicion / 9].insert(numeros[i]);
                    pColumnas[posicion % 9].insert(numeros[i]);
                    pCuadrantes[cuadrante_index].insert(numeros[i]);

                    if (posicion == 80)
                        return 1;
                    
                    sol = getSolucionSudoku(pSudoku,posicion+1,pFilas,pColumnas,pCuadrantes);
                    if (sol != 0)
                        return sol;

                    pSudoku[posicion] = ' ';

                    pFilas[posicion / 9].erase(numeros[i]);
                    pColumnas[posicion % 9].erase(numeros[i]);
                    pCuadrantes[cuadrante_index].erase(numeros[i]);
                }
            }
        }
        return 0;
    }
}

int main(int argc, char *argv[])
{
    printf("\n### Resolvedor de sudoku en C++ ###\n");
    printf("Hecho por:\n -Alberto Pérez Álvarez\n -Diego García Díaz\n\n");
    char sudoku[81];
    unordered_set<char> filas[9];
    unordered_set<char> columnas[9];
    unordered_set<char> cuadrantes[9];
    string ruta = getRuta(argc, argv);
    getArraySudoku(ruta, sudoku, filas, columnas, cuadrantes);
    printSudoku(sudoku);

    getSolucionSudoku(sudoku,0,filas,columnas,cuadrantes);
 
    printSudoku(sudoku);
    return 0;
}

void prueba()
{
    int x;
    float a = 2.34;
    cout << "Hello World!";
    cin >> x;
    std::cout << "\nHello World " << a << " !! \n"
              << x << "\n";
    return;
}

/*
0   1   2   3   4   5   6   7   8
9   10  11  12  13  14  15  16  17
18  19  20  21  22  23  24  25  26
27  28  29  30  31  32  33  34  35
36  37  38  39  40  41  42  43  44
45  46  47  48  49  50  51  52  53
54  55  56  57  58  59  60  61  62
63  64  65  66  67  68  69  70  71
72  73  74  75  76  77  78  79  80
*/