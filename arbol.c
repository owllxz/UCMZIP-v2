#include <stdio.h>
#include <stdlib.h>

typedef struct arbol{
	int numero;
	int caracter;
	struct arbol *izq;
	struct arbol *der;
}arbol;

typedef arbol Arbol;

arbol *crear_arbol(){
	arbol *p;
	p = NULL;
	return p;
}

arbol *crear_nodoa(int numero, int caracter){
	arbol *nodo;
	nodo = (arbol *) malloc(sizeof(arbol));
	nodo -> numero = numero;
	nodo -> caracter = caracter;
	nodo -> izq = NULL;
	nodo -> der = NULL;
	return nodo;
}
arbol *agregar(arbol **thee, int numero, int caracter){
	arbol *nodo_a = crear_nodoa(numero, caracter);
	if(*thee == NULL){
		*thee = nodo_a;
	}
	else{
		int valorRaiz = (*thee) -> numero;
		if(numero < valorRaiz){
			agregar(&(*thee)->izq,numero,caracter);
		}
		else{
			agregar(&(*thee)->der,numero,caracter);
		}
	}
	return *thee;
}
arbol *add_force(arbol **thee, int numero, int caracter){
	arbol *nodo_a = crear_nodoa(numero, caracter);
	if(*thee == NULL){
		*thee = nodo_a;
	}
	else{
		agregar(&(*thee)->der,numero,caracter);
	}
	return *thee;
}
arbol *agregar_derecha(arbol *thee, arbol *two){
	thee -> der = two;
	return thee;
}
void preorder(arbol *a){
	if(a !=NULL){
		printf("%d %d\n", a->numero, a->caracter);
		preorder(a->izq);
		preorder(a->der);
	}
}
void inorder(arbol *a){
	if(a !=NULL){
		preorder(a->izq);
		printf("%d %c\n", a->numero, a->caracter);
		preorder(a->der);
	}
}
void postorder(arbol *a){
	if(a !=NULL){
		preorder(a->izq);
		preorder(a->der);
		printf("%d %c\n", a->numero, a->caracter);
	}
}
int raiz_value(arbol *a){
	int raiz;
	raiz = a -> numero;
	return raiz;
}
int izq_value(arbol *a){
	int izq_v;
	a = a -> izq;
	izq_v = a -> numero;
	return izq_v;
}
int der_value(arbol *a){
	int der_v;
	a = a -> der;
	der_v = a -> numero;
	return der_v;
}
arbol *move_this(arbol *a,int num){
	if(num == 0){
		a = a ->izq;
	}
	else{
		a = a ->der;
	}
	return a;
}
void muestra_hojas(arbol *a){
	if( a != NULL){
		if((a->izq == NULL) && (a ->der == NULL)){
        	if(a -> caracter == '\n'){
				printf("%d Salto\n", a->numero);
			}
			else if(a -> caracter == '\0'){
				printf("%d Nulo\n", a->numero);	
			}
			else if(a -> caracter == ' ')
				printf("%d Espacio\n",a->numero);
			else{
				printf("%d %c\n", a->numero, a->caracter);
			}				
		}
	muestra_hojas(a->izq);
	muestra_hojas(a->der);
	}
}
int esVacio(arbol *a){
	if((a->izq == NULL) && (a ->der == NULL)){
		return 1;
	}
	return 0;
}
int nivel_arbol(arbol *a){
	int c,b;
	if((a->izq == NULL) && (a ->der == NULL)){
		return 1;
	}
	else{
		c = nivel_arbol(a->izq);
		b = nivel_arbol(a->der);
		if(c<b){
			return b+1;
		}
		else{
			return c+1;
		}
	}
}
char valorApuntado(arbol *a){
	char caracterA;
	caracterA = a ->caracter;
	return caracterA;
}
void codigo_binario(arbol *a, int caracter, char almacenar, char cadena[], int i, char cadenafix[]){
	if(a != NULL){
		if(a->caracter == caracter && a->izq == NULL & a->der ==NULL){
			cadena[i] = almacenar;
			i = i + 1;
			strcpy(cadenafix,cadena); //Guardamos en fixed solo lo que queremos guardar
		}
		else{
			if(almacenar == '0' && a -> izq != NULL){
				cadena[i] = '0';
				i = i + 1;
			}
			if(almacenar == '1' && a->der != NULL){
				cadena[i] = '1';
				i = i + 1;
			}
			codigo_binario(a->izq,caracter,'0',cadena,i,cadenafix);
			codigo_binario(a->der,caracter,'1',cadena,i,cadenafix);
		}
	}
}
/*
arbol *buscar_c(arbol *a, char caracter, int almacenar){
	if(a !=NULL){
		if(a->caracter == caracter){
			vector[0] = '1';
			vector[1] = '0';
			printf("%d\n", almacenar);
			printf("Encontrado\n");
			return vector;
		}
		if(almacenar == 0 && a -> izq != NULL){
			printf("0\n");
		}
		else if(almacenar == 1 && a->der != NULL){
			printf("1\n");
		}
		buscar_c(a->izq,caracter,0);
		buscar_c(a->der,caracter,1);
	}
}
*/

/*
void primero(arbol *a, int camino){
	lista *L;
	L = crear_lista();
	binarycode(arbol *a,int camino,list *list);
}
void binarycode(arbol *a, int camino,list *list{
	if(a != NULL){
		if((a->izq == NULL) && (a ->der == NULL)){
			printf("---------------------\n");
		}
		if(camino == 0){
			printf("0\n");
		}
		else if(camino == 1){
			printf("1\n");
		}
		binarycode(a->izq,0);
		binarycode(a->der,1);
	}
}
void amplitud(arbol *a){
	tcola *cola;
	arbol *aux;

	if(a != NULL){
		cola = crear_cola();
		enconlar(cola, a);
		while(!vacia(cola)){
			desencolar(cola,aux);
			printf("%d\n", aux ->numero);
			if(aux ->izq !=NULL) enconlar(cola,aux->izq);
			if(aux ->der !=NULL) enconlar(cola,aux->der);
		}
	}
}
*/
