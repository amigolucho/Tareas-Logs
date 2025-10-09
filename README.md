# README - Implementación de Árboles B y B+

## Descripción del Proyecto

Este proyecto implementa las estructuras de datos **Árbol B** y **Árbol B+** para almacenamiento y consulta eficiente de pares clave-valor en memoria externa. Los árboles están diseñados para manejar grandes volúmenes de datos de temperatura provenientes de la Estación Quinta Normal, Santiago, entre enero de 2019 y julio de 2025.

## Estructura del Proyecto

```
├── bin/    
│   ├── btree.cpp      
│   └── main.cpp  
├── include/
│   ├── btree.hpp       
│   └── bptree.hpp              
```

## Requisitos del Sistema

- **Sistema Operativo**: Linux, Windows o macOS
- **Compilador**: g++ (GCC) 9.0 o superior
- **Memoria RAM**: Mínimo 4GB recomendado
- **Espacio en disco**: Suficiente para archivos binarios de datos

## Dependencias

- **Standard Library de C++17**: Incluye `<vector>`, `<iostream>`, `<fstream>`, `<utility>`, `<array>`

No se requieren librerías externas adicionales.

## Compilación y Ejecución

### Compilación

```bash
g++ -std=c++17 -O2 -o btree_experiment main.cpp
```

### Ejecución

```bash
./btree_experiment
```

## Archivos de Datos

### Entrada
- **datos.bin**: Archivo binario que contiene pares (timestamp, temperatura) desordenados
  - Formato: `int` (4 bytes) + `float` (4 bytes) por cada par
  - Rango de timestamps: [1546300800, 1754006400] (enero 2019 - julio 2025)

### Salida
- **tree.bin**: Árbol serializado en disco
- **resultado.txt**: Métricas de experimentación (I/Os, tiempos, tamaños)

## Parámetros de Configuración

- **Orden del árbol (b)**: 340 (tamaño de nodo = 4096 bytes = 1 bloque de disco)
- **Tamaños de N probados**: \(2^{15}, 2^{16}, \ldots, 2^{26}\)
- **Rango de búsqueda**: 604,800 segundos (1 semana)

## Funcionalidades Implementadas

### Árbol B
- Inserción de pares clave-valor
- Split de nodos internos y hojas
- Búsqueda por rango recursiva
- Serialización en disco

### Árbol B+ (en desarrollo)
- Inserción con punteros entre hojas
- Búsqueda secuencial eficiente
- Split especial para hojas

### Utilidades
- Lectura/escritura de nodos en disco
- Creación de raíz inicial
- Inserción ordenada en nodos
- Conteo de operaciones I/O

## Experimentación Automática

El programa realiza automáticamente:

1. **Construcción de árboles** para diferentes tamaños de N
2. **Inserción** de N pares desde `datos.bin`
3. **Métrica de I/Os** (lecturas/escrituras simuladas)
4. **Búsquedas por rango** con rangos aleatorios de 1 semana
5. **Generación de resultados** en `resultado.txt`

## Estructuras de Datos

### BTreeNode
```cpp
struct BTreeNode {
    int es_interno;     // 1: interno, 0: hoja
    int k;              // Cantidad de pares actuales
    std::array<std::pair<int, float>, 340> llaves_valores;
    int hijos[341];     // Índices de hijos
    int siguiente;      // Puntero a siguiente hoja (B+)
};
```

### BTree
- Gestión completa de operaciones B-Tree
- Simulación de I/Os en memoria RAM
- Serialización final a disco

```

## Autores

Proyecto desarrollado para el curso CC4102 - Diseño y Análisis de Algoritmos, Departamento de Ciencias de la Computación, Universidad de Chile.

## Licencia

Proyecto académico - Uso educativo.