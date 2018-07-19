//Autores: Omar Olivares Urrutia - Rut: 17.603.163-8 ------- Luis Sepulveda Martinez - Rut: 19871495-k
//UCMZIP UPGRADE
//La version anterior del arbol se encuentra en https://github.com/owllxz/UCMZIP-v1
//La forma de como funciona esta codigo es de la misma manera que el anterior, dicha explicacion se encuentra en el archivo anterior subido al LMS o en el vinculo a Github
//Nota: El almacenado ahora es mediante numeros y no caracteres de esta manera trabajamos directamente con el codigo ASCII
//Nota 2: Se cambiaron la mayoria de las funciones por funciones mas eficientes y efectivas, y otras fueron reutilizadas
//Nota 3: Ahora las funciones se llaman desde cada proceso desde codificado o decodificado en cada caso y no en cadena como en la version anterior, eso permite hacer modificaciones directamente a la funcion sin afectar al resto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.c"
#include "arbol.c"

//PreDeclaracion de funciones
void verificarExistencia(char nameFile[]);
//Funciones del codificado
void codificar(char nameFile[]);
int longitudArchivo(char nameFile[]);
void almacenadoArchivo(char nameFile[], char arregloArchivo[], int largoArchivo);
void almacenadoLista(lista **caracterFrecuencia,char arregloArchivo[], int largoArchivo);
int frecuenciaCaracter(char arregloArchivo[], char caracter, int largoArchivo);
void almacenadoArbol(arbol **arbolHuffman,lista *caracterFrecuencia, int largoArchivo);
void obtenerCodigo(arbol *arbolHuffman, lista *caracterFrecuencia, char arregloArchivo[], char nameFile[]);
void nombreSalida(char nameFile[],char nameSalida[]);
//Funciones del decodificado
void decodificar(char nameFile[]);
void obtenerInformacion(char nameFile[], lista **informacionArbol);
void obtenerBinario(char cadenaBinaria[], char nameFile[]);
void recrearArchivo(arbol *arbolHuffmanDec, char cadenaBinaria[], char nameFile[]);

int main(int argc, char *argv[]){
	//Proceso de obtencion del nombre del archivo
	char nameFile[strlen(argv[1])];
	memset(nameFile,'\0',strlen(argv[1]));
	strcpy(nameFile,argv[1]);
	//Fin proceso de obtencion del nombre del archivo
	verificarExistencia(nameFile); //Funcion para verificar si es correcto el nombre del archivo
	return 0;
}
void verificarExistencia(char nameFile[]){
	int largoName = strlen(nameFile);
	int i;
	int j = 0;
	char extension[4];
	memset(extension,'\0',4);
	//For que nos permite obtener los ultimos tres caracteres y almacenarlos en la variable extencion
	for (i = largoName-1; i >= largoName-3; --i)
	{
		extension[j] = nameFile[i];
		j++;
	}
	//Proceso para comparar la extension del archivo y seleccionar un proceso, cabe recalcar que la funcion anterior nos obtencion a la inversa
	if(strcmp(extension,"txt") == 0){
		if(longitudArchivo(nameFile) != 0){
			printf("Proceso codificado.\n");
			codificar(nameFile);
		}
		else{
			printf("El archivo esta vacio.\n");
		}
	}
	else if(strcmp(extension,"pzu") == 0){
		if(longitudArchivo(nameFile) != 1){
			printf("Proceso decodificado.\n");
			decodificar(nameFile);
		}
		else{
			printf("El archivo esta vacio.\n");
		}
	}
	else{
		printf("Extension invalida.\n");
	}
}
void codificar(char nameFile[]){
	int largoArchivo = longitudArchivo(nameFile);
	char arregloArchivo[largoArchivo+1];
	memset(arregloArchivo,'\0',largoArchivo+1);
	almacenadoArchivo(nameFile,arregloArchivo,largoArchivo);
	lista *caracterFrecuencia;
	caracterFrecuencia = crear_lista();
	almacenadoLista(&caracterFrecuencia,arregloArchivo,largoArchivo);
	caracterFrecuencia = ordenar_lista(caracterFrecuencia);
	arbol *arbolHuffman = crear_arbol();
	almacenadoArbol(&arbolHuffman,caracterFrecuencia,largoArchivo);
	obtenerCodigo(arbolHuffman,caracterFrecuencia,arregloArchivo,nameFile);
}
void decodificar(char nameFile[]){
	int largoArchivo = longitudArchivo(nameFile);
	char cadenaBinaria[largoArchivo+1];
	memset(cadenaBinaria,'\0',largoArchivo+1);
	lista *informacionArbol;
	informacionArbol = crear_lista();
	obtenerInformacion(nameFile,&informacionArbol); 
	arbol *arbolHuffmanDec = crear_arbol();
	almacenadoArbol(&arbolHuffmanDec,informacionArbol,largoArchivo);
	obtenerBinario(cadenaBinaria,nameFile);
	recrearArchivo(arbolHuffmanDec,cadenaBinaria,nameFile);
}
int longitudArchivo(char nameFile[]){
	FILE *archivo;
	archivo = fopen(nameFile,"r");
	int largoArchivo = 0;
	//Proceso para obtener el largo del archivo de esta manera obtener la cantidad de memora asignada para el arreglo que va a tener todo el archivo
	while(1){
		char c;
		c = fgetc(archivo);
		if(feof(archivo)){
			break;
		}
		largoArchivo++;
	}
	//Fin proceso obtener largo del archivo
	fclose(archivo);
	return largoArchivo;
}
void almacenadoArchivo(char nameFile[], char arregloArchivo[], int largoArchivo){
	FILE *archivo;
	archivo = fopen(nameFile,"r");
	//Proceso para guardar todos los caracteres dentro del arreglo
	int i = 0;
	while(i<largoArchivo){
		char c;
		c = fgetc(archivo);
		arregloArchivo[i] = c;
		i++;
	}
	//Fin proceso para guardar todos los caracteres en el arreglo
	fclose(archivo);	
}
//Hacemos uso del doble puntero ya que la lista de por si ya usa puntero
void almacenadoLista(lista **caracterFrecuencia,char arregloArchivo[], int largoArchivo){
	int i = 0;
	int j;
	int primerCaso = 0;
	//Inicio de proceso de almacenar el caracter y la frecuencia
	while(i<largoArchivo){
		char caracter = arregloArchivo[i];
		if(primerCaso == 0){
			int frecuencia = frecuenciaCaracter(arregloArchivo,caracter,largoArchivo);
			*caracterFrecuencia = agregar_inicio(*caracterFrecuencia,frecuencia,caracter);
			primerCaso = 1;
		}
		//Proceso para comprobar si el caracter ya estan dentro de la lista
		else{
			for (j = 0; j < largo_lista(*caracterFrecuencia); ++j)
			{
				if(buscar(*caracterFrecuencia,caracter) == 1){
					break;
				}
			}		
			if(j == largo_lista(*caracterFrecuencia)){
				int frecuencia = frecuenciaCaracter(arregloArchivo,caracter,largoArchivo);
				*caracterFrecuencia = agregar_inicio(*caracterFrecuencia,frecuencia,caracter);				 
			}	
		}
		//Fin de proceso de comprobar si el caracter ya esta en la lista
		i++;
	}
	//Fin de proceso de almacenado del el caracter y la frecuencia
}
int frecuenciaCaracter(char arregloArchivo[], char caracter, int largoArchivo){
	int i = 0;
	int frecuencia = 0;
	//Inicio de proceso para obtener frecuencia, en este proceso comparamos el caracter con todo el arreglo del archivo y retornamos su frecuencia
	while(i<largoArchivo){
		if(arregloArchivo[i] == caracter){
			frecuencia++;
		}
		i++;
	}
	//Fin proceso de para obtener frecuencia
	return frecuencia;
}
void almacenadoArbol(arbol **arbolHuffman,lista *caracterFrecuencia, int largoArchivo){
	//Por simplicidad y por correcto funcionamiento se utilizo el mismo proceso de llenado de llenado de arbol que la version anterior del UCMZIP
	int largoLista = largo_lista(caracterFrecuencia);
	int i = 0;
	int primerCaso = 0;
	arbol *aux;
	aux = crear_arbol();
	int posicion = 0;
	//Inicio de proceso de llenado del arbol de huffman
	if(largoLista != 1){
		while( i < largoLista-1){
		if(primerCaso == 0){
			int suma_frecuencias = (numero(caracterFrecuencia,posicion) + numero(caracterFrecuencia,posicion+1));
			*arbolHuffman = agregar(&*arbolHuffman, suma_frecuencias, 0); // 0 = NULL CHARACTER
			*arbolHuffman = agregar(&*arbolHuffman, numero(caracterFrecuencia,posicion), caracter(caracterFrecuencia,posicion));
			*arbolHuffman = add_force(&*arbolHuffman, numero(caracterFrecuencia,posicion+1), caracter(caracterFrecuencia,posicion+1)); //Si es que no usamos este add_force y simplemente utilizamos el otro agregar, el segundo caracter se transforma en hijo del primero
			primerCaso = 1;
		}
		else{
			aux = *arbolHuffman;
			*arbolHuffman = crear_arbol();
			int suma_frecuencias = raiz_value(aux) + numero(caracterFrecuencia,posicion+1);
			*arbolHuffman = agregar(&*arbolHuffman,suma_frecuencias,0);
			*arbolHuffman = agregar(&*arbolHuffman,numero(caracterFrecuencia,posicion+1),caracter(caracterFrecuencia,posicion+1));
			*arbolHuffman = agregar_derecha(*arbolHuffman,aux);
		}
		posicion = posicion + 1;
		i++;
	    }	
	}
	else{
		int suma_frecuencias = (numero(caracterFrecuencia,0) + 0);
		*arbolHuffman = agregar(&*arbolHuffman, suma_frecuencias, 0);
		*arbolHuffman = agregar(&*arbolHuffman, numero(caracterFrecuencia,0), caracter(caracterFrecuencia,0));
	}	
	//Fin de proceso del arbol de huffman
}
void obtenerCodigo(arbol *arbolHuffman, lista *caracterFrecuencia, char arregloArchivo[], char nameFile[]){
	FILE *archivoSalida;
	int largoName = strlen(nameFile);
	char nameSalida[largoName-2];
	memset(nameSalida,'\0',largoName-2);
	nombreSalida(nameFile,nameSalida);
	strcat(nameSalida,"uzp");
	archivoSalida = fopen(nameSalida,"w");
	int i;
	//Este es un proceso mejorado de almacenado de informacion para recrear el arbol en un proceso posterior
	for(i = 0; i < largo_lista(caracterFrecuencia);i++){
		fprintf(archivoSalida, "%d", caracter(caracterFrecuencia,i));
		fputc(' ',archivoSalida);	
		fprintf(archivoSalida, "%d", numero(caracterFrecuencia,i));
		fputc(' ',archivoSalida);	
	}
	//Fin proceso de almacenado de informacion para posterior recreacion
	fputc('\n',archivoSalida);
	//Este es el proceso de almacenado del codigo binario
	if(largo_lista(caracterFrecuencia) != 1){
		for (i = 0; i < strlen(arregloArchivo); ++i)
		{
			int caracter = arregloArchivo[i];
			char cadena[nivel_arbol(arbolHuffman)];
			char cadenafix[nivel_arbol(arbolHuffman)];		
			memset(cadena,'\0',nivel_arbol(arbolHuffman));
			memset(cadenafix,'\0',nivel_arbol(arbolHuffman));		
			codigo_binario(arbolHuffman,caracter,'2',cadena,0,cadenafix);
			fputs(cadenafix,archivoSalida);		
		}
	}
	else if(largo_lista(caracterFrecuencia) == 1){
		fputc('1',archivoSalida);
	}
	//Fin almacenado codigo binario
	fclose(archivoSalida);
}
void nombreSalida(char nameFile[],char nameSalida[]){
	int largoName = strlen(nameFile);
	int i;
	//Este bucle nos permite tener el nombre del archivo sin su extension siempre y cuando dicha extension sea de solo 3 caracteres
	for (i = 0; i < largoName-3; ++i)
	 {
	 	nameSalida[i] = nameFile[i];
	 } 
}
//Fin de funciones de proceso codificado
void obtenerInformacion(char nameFile[], lista **informacionArbol){
	FILE *archivoIF;
	archivoIF = fopen(nameFile,"r");
	char cadenaInformacion[longitudArchivo(nameFile)+1];
	memset(cadenaInformacion,'\0',longitudArchivo(nameFile)+1);
	int valor = 0;
	char arreglo[longitudArchivo(nameFile)+1];
	memset(arreglo,'\0',longitudArchivo(nameFile)+1);
	//Proceso para obtener la informacion para recrear el arbol
	while(1){
		fscanf(archivoIF, "%s", cadenaInformacion);
		if(valor == 0){
			strcpy(arreglo,cadenaInformacion);
			valor = 1;
		}
		else{
			*informacionArbol = agregar_inicio(*informacionArbol,atoi(cadenaInformacion),atoi(arreglo));
			valor = 0;
		}
		if( feof(archivoIF)){
			break;
		}
	}
	//Fin del proceso para obtener la informacion para recrear el arbol
	//Con este proceso invertimos nuestra lista para que quede tal cual como quedo en el caso de codificado
	lista *listaAux;
	listaAux = crear_lista();
	int i;
	for (i = 0; i < largo_lista(*informacionArbol); ++i)
	{
		listaAux = agregar_inicio(listaAux,numero(*informacionArbol,i),caracter(*informacionArbol,i));
	}
	*informacionArbol = listaAux;
	//Fin proceso de invertir lista
	fclose(archivoIF);
}
void obtenerBinario(char cadenaBinaria[], char nameFile[]){
	FILE *archivoB;
	archivoB = fopen(nameFile,"r");
	int i = 0;
	int iSalto = 0;
	//Este ciclo nos permite guardar todo el codigo binario del archivo codificado en una cadena para luego posteriormente mandarlo a la funcion recrearArchivo
	while(1){
		char c = fgetc(archivoB);
		if( feof(archivoB)){
			break;
		}
		if(c == '\n' || iSalto == 1){
			cadenaBinaria[i] = c;
			iSalto = 1;
			i++;
		}
	}
	//Fin de proceso de guardar codigo binario
	fclose(archivoB);
}
void recrearArchivo(arbol *arbolHuffmanDec, char cadenaBinaria[], char nameFile[]){
	arbol *arbolHuffmanDecAux;
	arbolHuffmanDecAux = crear_arbol();
	arbolHuffmanDecAux = arbolHuffmanDec;
	int i = 0;
	FILE *archivoDecodificado;
	int largoName = strlen(nameFile);
	char nameSalida[largoName-2];
	memset(nameSalida,'\0',largoName-2);
	nombreSalida(nameFile,nameSalida);
	strcat(nameSalida,"txt");
	archivoDecodificado = fopen(nameSalida,"w");
	//Este ciclo nos permite ir en conjunto con el codigo binario y el arbol obteniendo los caracteres necesarios para recrear el archivo
	while(i < strlen(cadenaBinaria)){
		char c = cadenaBinaria[i];
		if(c == '0'){
			arbolHuffmanDec = move_this(arbolHuffmanDec,0);
		}
		else if(c == '1'){
			arbolHuffmanDec = move_this(arbolHuffmanDec,1);
		}
		if(esVacio(arbolHuffmanDec)){
			fputc(valorApuntado(arbolHuffmanDec),archivoDecodificado);
			arbolHuffmanDec = arbolHuffmanDecAux;
		}
		i++;
	}	
	//Fin de proceso de recrear archivo
	fclose(archivoDecodificado);
}