#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#define BUFFER_SIZE 256
#define IP_SIZE 30

// Funcion de lectura
int lectura(char* cad, int n) {
	int i, c;
	c = getchar();

	if (c == EOF) {
		cad[0] = '\0';
		return 0;
	}
	if (c == '\n')
		i = 0;
	else {
		cad[0] = c;
		i = 1;
	}
	for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++)
		cad[i] = c;
	cad[i] = '\0';
	if (c != '\n' && c != EOF)
		while ((c = getchar()) != '\n' && c != EOF);
	return 1;
};


// Funcion para mostrar el contenido del archivo
void escritura(FILE* input) {
	char ip[IP_SIZE] = "";

	while ((fgets(ip, IP_SIZE, input) != NULL)) {
		printf("%s", ip);
	}
	printf("\n");
}

// Funcion para encontrar el DNS de un adaptador red
void DNSobtener(FILE* archivo, char* nombreDNS, char* dns) {
	char ch[BUFFER_SIZE] = "";
	int numeroDeLineasVacias = 0;
	int encontrado = 0;

	while (((fgets(ch, BUFFER_SIZE, archivo)) != NULL) && numeroDeLineasVacias != 2) {
		if ((strstr(ch, nombreDNS) != NULL) || (encontrado == 1)) {
			encontrado = 1;
			if (strcmp(ch, "\n") == 0) {
				numeroDeLineasVacias++;
			}

			if (strstr(ch, "DNS Servers") != NULL) {
				sscanf(ch, "%*[^:]: %[^\n]", dns);
			}
		}
	}
	if ((encontrado == 0) || (strlen(dns) == 0)) {
		puts("No se ha encontrado el adaptador indicado, comprueba que el nombre introducido sea el correcto.");
		exit(1);
	}
}

// Funcion para el lanzamiento de Pings, si el archivo de salida no existe este es creado de forma automática
void hazPING(FILE** input) {
	int ping = 0;
	if (input != NULL) {
		char comandoPing[80] = "ping ";
		char ip[30] = "";
		while ((fgets(ip, sizeof(ip), input)) != NULL) {
			ip[strcspn(ip, "\n")] = 0;
			if (ping < 1) {
				strcat(strcat(comandoPing, ip), " > \"pingsip.txt\"");
				system(comandoPing);
				ping++;
			}
			else {
				strcat(strcat(strcat(strcpy(comandoPing, ""), "ping "), ip), "  >> \"pingsip.txt\"");
				system(comandoPing);
			}
		}
	}
}

// Funcion para comprobar qué IPs de las anteriores tienen conexión
void testConexion(FILE** archivo, char* dns) {
	FILE* ipsConConexion = NULL;
	char ch[BUFFER_SIZE] = "";
	char ip[30] = "";
	int paquetesRecibidos = 5, encontrado = 0, escaneado = 0;
	ipsConConexion = fopen("ipsConConexion.txt", "w+");
	if (ipsConConexion == NULL) {
		puts("Se ha producido un error y es necesario reiniciar.");
		return -1;
	}
	while ((fgets(ch, sizeof(ch), archivo)) != NULL) {
		if (strstr(ch, "Ping") && (encontrado == 0)) {
			sscanf(ch, "%*[^0-9]%s", &ip);
			encontrado = 1;
		}
		if ((strstr(ch, "Received") || strstr(ch, "Recibido"))) {
			sscanf(ch, "%*[^=]=%*[^=]=%d", &paquetesRecibidos);
			escaneado = 1;
		}
		if (escaneado == 1) {
			if (paquetesRecibidos > 0) {
				printf("La ip %s tiene conexion.\n", ip);
				fputs(strcat(ip, "\n"), ipsConConexion);
			}
			else {
				printf("La ip %s no tiene conexion.\n", ip);
			}
			escaneado = 0;
			encontrado = 0;
			strcpy(ip, "");

		}
	}
	fclose(ipsConConexion);
}

// Funcion para obtener la ip con la conexión media más rápida de las analizadas
void testVelocidad(FILE** archivo, char* masRapido) {
	char ch[BUFFER_SIZE] = "";
	char ip[30] = "";
	char dnsMasRapido[30] = "";
	int velocidadMedia = 0, ipMasRapida = 500, encontrado = 0, escaneado = 0;
	while ((fgets(ch, sizeof(ch), archivo)) != NULL) {
		if (strstr(ch, "Ping") && (encontrado == 0)) {
			sscanf(ch, "%*[^0-9]%s", &ip);
			encontrado = 1;
		}
		if ((strstr(ch, "Average") || strstr(ch, "Media"))) {
			sscanf(ch, "%*[^=]=%*[^=]=%*[^=]=%d", &velocidadMedia);
			escaneado = 1;
		}
		if (escaneado == 1) {
			printf("El DNS %s tiene una velocidad media de %d ms.\n", ip, velocidadMedia);
			if (ipMasRapida > velocidadMedia) {
				strcpy(dnsMasRapido, ip);
				ipMasRapida = velocidadMedia;
				printf("El DNS %s es el mas rapido con una velocidad media de %d ms.\n", dnsMasRapido, ipMasRapida);
			}
			else {
				printf("El DNS %s es el mas rapido con una velocidad media de %d ms.\n", dnsMasRapido, ipMasRapida);

			}
			escaneado = 0;
			encontrado = 0;
			strcpy(ip, "");
		}
	}
	strcpy(masRapido, dnsMasRapido);
}