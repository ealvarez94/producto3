#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#define BUFFER_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"


int main() {
	FILE* IPs = NULL;
	FILE* respuestaIP = NULL;
	FILE* configuracion = NULL;
	char Larchivo[100] = "";
	char nombre[150] = "";
	char DNS[30] = "";
	char velocidadDNS[30] = "";



	// Introducir la ruta del archivo a leer, en mi caso: C:\Users\enolm\source\repos\P4(P3)\DNSips.txt, y muestra el contenido por pantalla 
	// Nombre del adaptador de red Wireless LAN adapter Wi-Fi
	do {
		printf("Introduzca la direccion del archivo:\n");
		lectura(Larchivo, 100);
		IPs = fopen(Larchivo, "r");
		if (IPs != NULL) {
			printf("Contenido del archivo %s.\n", Larchivo);
			escritura(IPs);
		}
		else {
			printf("No se pudo abrir el archivo, asegurate de que la ruta sea la correcta.\n");
		}
	} while (IPs == NULL);
	fclose(IPs);

	// Solicita el nombre del Adaptador de Red
	printf("Introduce el nombre del adaptador\n");
	lectura(nombre, 150);

	// Obtiene la información de los conectores de red que están operativos
	system("ipconfig/all > \"ipconfig_all.txt\"");
	configuracion = fopen("ipconfig_all.txt", "r");

	// Comprueba que el archivo con la información del DNS esté creado correctamente, si no indica que se debe reiniciar
	if (configuracion == NULL) {
		printf("Ha surgido un error, reinicia el programa.\n");
		return -1;
	}

	// Saca por pantalla la información actual obtenida mediante el comando ipconfig
	DNSobtener(configuracion, &nombre, &DNS);
	printf("El DNS actual es %s \n", DNS);
	fclose(configuracion);

	// Comprueba la accesibilidad de las IP
	printf("Comprobando la accesibiliadd de las IPs, espera.\n");
	IPs = fopen(Larchivo, "r");
	if (IPs != NULL) {
		hazPING(IPs);
	}
	else {
		puts("No se pudo abrir el archivo. Reinicie el programa.");
		exit(1);
	}
	fclose(IPs);
	printf("IPs comprobadas.\n");
	IPs = fopen("pingsip.txt", "r");
	if (IPs == NULL) {
		printf("No se pudo abrir el archivo. Reinicie el programa.\n");
		return -1;
	}
	puts("comprobando si las ÎPs respondieron.");
	testConexion(IPs, DNS);
	puts("Terminado.");
	fclose(IPs);

	// Comprobar cual de los dns es el mas rapido en el archivo temporal
	puts("Comprobando que conexion es la mas rapida\n");

	respuestaIP = fopen("ipsConConexion.txt", "r");
	if (respuestaIP == NULL) {
		printf("No se pudo abrir el archivo. Reinicie el programa.\n");
		return -1;
	}
	hazPING(respuestaIP);
	fclose(respuestaIP);
	IPs = fopen("pingsip.txt", "r");
	if (IPs == NULL) {
		puts("Se ha producido un error. Reinicie el programa.");
		return -1;
	}
	testVelocidad(IPs, velocidadDNS);
	fclose(IPs);
	if (strcmp(DNS, velocidadDNS) != 0) {
		puts("prueba");
	}
	else {
		puts("El DNS introducido es el mas rapido, no se va a realizar ningún cambio.");
	}

	// Elimina los archivos creados para el funcionamiento del programa
	puts("Procediendo a borrar los archivos de texto generados durante la ejecucion");
	if (remove("ipconfig_all.txt") == 0) {
		printf("Archivo ipconfig_all.txt borrado.\n");
	}
	else {
		printf("El archivo ipconfig_all.txt no se ha podido borrar o no existe.\n");
	}
	if (remove("ipsConConexion.txt") == 0) {
		printf("Archivo ipsConConexion.txt borrado.\n");
	}
	else {
		printf("El archivo ipsConConexion.txt no se ha podido borrar o no existe.\n");
	}
	if (remove("pingsip.txt") == 0) {
		printf("Archivo pings.txt borrado.\n");
	}
	else {
		printf("El archivo pings.txt no se ha podido borrar o no existe.\n");
	}
	return 0;
}