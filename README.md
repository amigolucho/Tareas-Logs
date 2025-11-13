# README - Implementación de Árboles B y B+

## Descripción del Proyecto

Este proyecto implementa la estructuras de datos **Trie** para almacenamiento y consulta eficiente de llaves (caracteres) y poder testear la función de autocompletado.

## Requisitos del Sistema

- **Sistema Operativo**: Linux, Windows o macOS
- **Compilador**: g++ (GCC) 9.0 o superior
- **Memoria RAM**: Mínimo 4GB recomendado
- **Espacio en disco**: Suficiente para archivos bde textos de datos

## Dependencias

No se requieren librerías externas adicionales que instalar.

## Compilación y Ejecución

### Compilación

```bash
g++ -std=c++17 -O2 -o trie main.cpp
```

### Ejecución

```bash
./trie
```

## Archivos de Datos

### Entrada
- **words.txt**: Archivo de texto usado para insertar caracteres en los diferentes tries.
- **random_with_distribution.txt**, **random.txt** y **wikipedia.txt**: Archivos de texto usados para testear el autocompletado.

### Salida
- **archivos.csv**: Resultados en formato csv para cada parte de la experimentación
- **graphs.ipynb**: Jupyter notebook que utiliza los archivos csv para generar los graficos y tablas necesarios para proceder con el análisis.

Corriendo el archivo main debería resultar todo correcto.

