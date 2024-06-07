#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

typedef struct materias_alumno {
    int codigo_materia;
    struct materias_alumno *siguiente;
} materias_alumno;

typedef struct materias_aprobadas {
    int codigo_materia;
    int nota_materia;
    struct materias_aprobadas *siguiente;
} materias_aprobadas;

typedef struct materias_correlativas {
    int codigo_materia;
    struct materias_correlativas *siguiente;
} materias_correlativas;

typedef struct estudiante {
    char nombre[50];
    char apellido[50];
    int edad;
    materias_alumno *materias_alumno;
    materias_aprobadas *materias_aprobadas;
    struct estudiante *siguiente;
} estudiante;

typedef struct materia {
    int codigo;
    char nombre[50];
    int cupo;
    materias_correlativas *materias_correlativas;
    struct materia *siguiente;
} materia;

typedef struct sistema {
    estudiante *estudiantes;
    materia *materias;
} sistema;

sistema *crear_sistema(void) {
    sistema *sistema = malloc(sizeof(sistema));
    sistema->estudiantes = NULL;
    sistema->materias = NULL;
    return sistema;
}

void agregar_estudiante(sistema *sistema, char *nombre,char *apellido, int edad) {
    estudiante *nuevo = malloc(sizeof(estudiante));
    strcpy(nuevo->nombre, nombre);
    strcpy(nuevo->apellido, apellido);
    nuevo->edad = edad;
    nuevo->materias_alumno = NULL;
    nuevo->materias_aprobadas = NULL;
    nuevo->siguiente = sistema->estudiantes;
    sistema->estudiantes = nuevo;
}

void modificar_estudiante(sistema *sistema, char *nombre, char *apellido, char *nuevo_nombre, char *nuevo_apellido, int edad) {
    estudiante *actual = sistema->estudiantes;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0 && strcasecmp(actual->apellido, apellido) == 0) {
            strcpy(actual->nombre, nuevo_nombre);
            strcpy(actual->apellido, nuevo_apellido);
            actual->edad = edad;
            printf("Modificacion exitosa: El Estudiante %s %s ahora se llama %s %s y tiene %d anios.\n", nombre, apellido, nuevo_nombre, nuevo_apellido, edad);
            return;
        }
        actual = actual->siguiente;
    }
    printf("Modificacion fallida: El Estudiante %s %s no fue encontrado.\n", nombre, apellido);
}

// Tengo que revisar porque acá tengo que poner un puntero para que funcione (No lo estoy entendiendo)
materia *materia_por_codigo (sistema *sistema, int codigo_materia){
    materia *actual = sistema->materias;
    while (actual != NULL){
        if (actual->codigo == codigo_materia){
            return actual;
        }
        actual = actual->siguiente;
    }
    return actual;
}

estudiante *estudiante_por_nombre_apellido(sistema *sistema, char *nombre, char *apellido) {
    estudiante *actual = sistema->estudiantes;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0 && strcasecmp(actual->apellido, apellido) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return actual;
}

materias_alumno *materias_cursada(estudiante *estudiantes, int codigo_materia) {
    materias_alumno *actual = estudiantes->materias_alumno;
    while (actual != NULL) {
        if (actual->codigo_materia == codigo_materia) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return actual;
}

materias_aprobadas *materias_aprobada(estudiante *estudiantes, int codigo_materia) {
    materias_aprobadas *actual = estudiantes->materias_aprobadas;
    while (actual != NULL) {
        if (actual->codigo_materia == codigo_materia) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return actual;
}

void listar_materias_cursadas(estudiante *estudiante, sistema *sistema) {
    materias_alumno *actual = estudiante->materias_alumno;
    while (actual != NULL) {
        materia *materia_actual = materia_por_codigo (sistema, actual->codigo_materia);
        printf("%s %s está cursando la materia %s con el código %d\n", estudiante->nombre, estudiante->apellido, materia_actual->nombre, actual->codigo_materia);
        actual = actual->siguiente;
    }
}

void listar_materias_aprobadas(estudiante *estudiante, sistema *sistema) {
    materias_aprobadas *actual = estudiante->materias_aprobadas;
    while (actual != NULL) {
        materia *materia_actual = materia_por_codigo (sistema, actual->codigo_materia);
        printf("%s %s tiene aprobada la materia %s con una nota de %d\n", estudiante->nombre, estudiante->apellido, materia_actual->nombre, actual->nota_materia);
        actual = actual->siguiente;
    }
}

void calcular_promedio_estudiante(estudiante *estudiante) {
    materias_aprobadas *actual = estudiante->materias_aprobadas;
    float promedio = 0;
    int cantidad_materias = 0;
    while (actual != NULL) {
        promedio += actual->nota_materia;
        cantidad_materias += 1;
        actual = actual->siguiente;
    }
    promedio = promedio / cantidad_materias;
    printf("El estudiante %s %s tiene un promedio de %0.1f\n", estudiante->nombre, estudiante->apellido, promedio);
}

bool verificar_cumplir_correlativas (sistema *sistema, estudiante *estudiante, materia *materia){
    materias_correlativas *actual = materia->materias_correlativas;
    while (actual != NULL) {
        if (materias_aprobada(estudiante, actual->codigo_materia) == NULL) {
            printf("La materia correlativa %s no se encuentra aprobada para poder cursar %s.\n", materia_por_codigo(sistema, actual->codigo_materia)->nombre,materia->nombre);
            return false;
        }
        actual = actual->siguiente;
    }
    return true;
}

void cursar_materia(sistema *sistema, char *nombre, char *apellido, int codigo_materia) {
    estudiante *actual = sistema->estudiantes;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0 && strcasecmp(actual->apellido, apellido) == 0) {
            if(materia_por_codigo(sistema, codigo_materia) == NULL){
                printf("La materia con el código %d no existe.\n", codigo_materia);
                return;
            }
            materias_alumno *materia_alumno_a_cursar = materias_cursada(actual, codigo_materia);
            materias_aprobadas *materia_alumno_a_cursar_aprobada = materias_aprobada(actual, codigo_materia);
            materia *materia_a_cursar = materia_por_codigo (sistema, codigo_materia);
            if (materia_alumno_a_cursar != NULL){
                printf("El estudiante %s %s ya se encuentra cursando la materia %s con el código %d\n", actual->nombre, actual->apellido, materia_a_cursar->nombre, materia_alumno_a_cursar->codigo_materia);
                return;
            }
            if (materia_alumno_a_cursar_aprobada != NULL){
                printf("El estudiante %s %s ya tiene la materia %s aprobada con una nota de %d\n", actual->nombre, actual->apellido, materia_a_cursar->nombre, materia_alumno_a_cursar_aprobada->nota_materia);
                return;
            }
            if (!verificar_cumplir_correlativas (sistema, actual, materia_a_cursar)){
                return;
            }
            if (materia_a_cursar->cupo == 0){
                printf("La materia con %s no tiene cupos disponibles.\n", materia_a_cursar->nombre);
                return;
            }
            materias_alumno *nueva = malloc(sizeof(materias_alumno));
            nueva->codigo_materia = codigo_materia;
            nueva->siguiente = sistema->estudiantes->materias_alumno;
            sistema->estudiantes->materias_alumno = nueva;
            materia_a_cursar->cupo -= 1;
            printf("La materia %s quedo con un cupo de %d\n", materia_a_cursar->nombre, materia_a_cursar->cupo);
            printf("El estudiante %s %s se ha anotado correctamente a la materia %s con el código %d\n", actual->nombre, actual->apellido, materia_a_cursar->nombre, nueva->codigo_materia);
            return;
        }
        actual = actual->siguiente;
    }
}

void eliminar_materia_cursada(sistema *sistema, estudiante *estudiante, int codigo_materia) {
    materias_alumno *actual = estudiante->materias_alumno;
    materias_alumno *anterior = NULL;
    while (actual != NULL) {
        if (actual->codigo_materia == codigo_materia) {
            materia *materia = materia_por_codigo (sistema, codigo_materia);
            if (anterior == NULL) {
                estudiante->materias_alumno = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            materia->cupo += 1;
            free(actual);
            printf("Eliminacion exitosa: El alumno %s %s dejo de cursar la materia %s.\n", estudiante->nombre, estudiante->apellido, materia->nombre);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Eliminacion fallida: El alumno %s %s no se encontraba cursando la materia %s.\n", estudiante->nombre, estudiante->apellido, materia_por_codigo (sistema, codigo_materia)->nombre);
}

void aprobar_materia_cursada_estudiante(sistema *sistema, char *nombre, char *apellido, int codigo_materia, int nota_materia){
    estudiante *estudiante_actual = sistema->estudiantes;
    while (estudiante_actual != NULL) {
        if (strcasecmp(estudiante_actual->nombre, nombre) == 0 && strcasecmp(estudiante_actual->apellido, apellido) == 0) {
            if(materia_por_codigo(sistema, codigo_materia) == NULL){
                printf("La materia con el código %d no existe.\n", codigo_materia);
                return;
            }

            materias_alumno *materia_alumno_a_cursar = materias_cursada(estudiante_actual, codigo_materia);
            materia *materia = materia_por_codigo (sistema, codigo_materia);
            if (materia_alumno_a_cursar == NULL){
                printf("El estudiante %s %s no se encuentra anotado en la materia %s con el código %d\n", estudiante_actual->nombre, estudiante_actual->apellido, materia->nombre, materia->codigo);
                return;
            }

            eliminar_materia_cursada(sistema, estudiante_actual, codigo_materia);
            if (nota_materia < 4){
                printf("El estudiante %s %s ha desaprobado la materia %s.\n", estudiante_actual->nombre, estudiante_actual->apellido, materia->nombre);
                return;
            }
            materias_aprobadas *nueva = malloc(sizeof(materias_aprobadas));
            nueva->codigo_materia = codigo_materia;
            nueva->nota_materia = nota_materia;
            nueva->siguiente = sistema->estudiantes->materias_aprobadas;
            sistema->estudiantes->materias_aprobadas = nueva;
            printf("El estudiante %s %s ha aprobado la materia %s con una nota de %d\n", estudiante_actual->nombre, estudiante_actual->apellido, materia->nombre, nueva->nota_materia);
            return;

        }
        estudiante_actual = estudiante_actual->siguiente;
    }
}

void eliminar_estudiante(sistema *sistema, char *nombre, char *apellido) {
    estudiante *actual = sistema->estudiantes;
    estudiante *anterior = NULL;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0 && strcasecmp(actual->apellido, apellido) == 0) {
            if (anterior == NULL) {
                sistema->estudiantes = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            free(actual);
            printf("Eliminacion exitosa: El Estudiante %s %s fue eliminado.\n", nombre, apellido);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Eliminacion fallida: El Estudiante %s %s no fue encontrado.\n", nombre, apellido);
}


void listar_estudiantes(sistema *sistema) {
    estudiante *actual = sistema->estudiantes;
    while (actual != NULL) {
        printf("%s,%s,%d\n", actual->nombre,actual->apellido, actual->edad,"\n");
        actual = actual->siguiente;
    }
}

void listar_estudiantes_por_nombre(estudiante *estudiantes) {
    estudiante *actual = estudiantes;
    while (actual != NULL) {
        printf("%s,%s,%d\n", actual->nombre,actual->apellido, actual->edad);
        actual = actual->siguiente;
    }
}

estudiante estudiante_por_nombre(sistema *sistema, char *nombre){
    estudiante *actual = sistema->estudiantes;
    estudiante *lista = NULL;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0) {
            estudiante *nuevo = malloc(sizeof(estudiante));
            strcpy(nuevo->nombre, actual->nombre);
            strcpy(nuevo->apellido, actual->apellido);
            nuevo->edad = actual->edad;
            nuevo->materias_alumno = actual->materias_alumno;
            nuevo->siguiente = lista;
            lista = nuevo;
        }
        actual = actual->siguiente;
    }
    listar_estudiantes_por_nombre(lista);
    return *lista;
}




void agregar_materia(sistema *sistema, char *nombre, int codigo, int cupo) {
    materia *nueva = malloc(sizeof(materia));
    strcpy(nueva->nombre, nombre);
    nueva->codigo = codigo;
    nueva->cupo = cupo;
    nueva->materias_correlativas = NULL;
    nueva->siguiente = sistema->materias;
    sistema->materias = nueva;
}

/*materia obtener_materia_por_codigo(sistema *sistema) {
    materia *actual = sistema->materias;
    while (actual != NULL) {
        printf("%s,%d,%d\n", actual->nombre, actual->codigo, actual->cupo);
        actual = actual->siguiente;
    }
}*/

void agregar_materia_correlativa(sistema *sistema, int codigo_materia_actual, int codigo_materia_correlativa) {
    materia *materia_actual = materia_por_codigo (sistema, codigo_materia_actual);
    if (materia_actual == NULL){
        printf("La materia con el código %d no exite.\n", codigo_materia_actual);
        return;
    }
    materia *materia_correlativa = materia_por_codigo (sistema, codigo_materia_correlativa);
    if (materia_correlativa == NULL){
        printf("La materia correlativa con el código %d no exite.\n", codigo_materia_correlativa);
        return;
    }
    materias_correlativas *nueva = malloc(sizeof(materias_correlativas));
    nueva->codigo_materia = codigo_materia_correlativa;
    nueva->siguiente = sistema->materias->materias_correlativas;
    sistema->materias->materias_correlativas = nueva;
    printf("Se ha agregado a la materia %s como correlativa de %s.\n", materia_correlativa->nombre, materia_actual->nombre);
}

void listar_materias(sistema *sistema) {
    materia *actual = sistema->materias;
    while (actual != NULL) {
        printf("%s,%d,%d\n", actual->nombre, actual->codigo, actual->cupo);
        actual = actual->siguiente;
    }
}

void modificar_materia(sistema *sistema, char *nombre, char *nuevo_nombre, int nuevo_codigo, int nuevo_cupo) {
    materia *actual = sistema->materias;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0) {
            strcpy(actual->nombre, nuevo_nombre);
            actual->codigo = nuevo_codigo;
            actual->cupo = nuevo_cupo;
            printf("Modificacion exitosa: La materia %s ahora se llama %s, tiene el codigo %d y el cupo %d.\n", nombre, nuevo_nombre, nuevo_codigo, nuevo_cupo);
            return;
        }
        actual = actual->siguiente;
    }
    printf("Modificacion fallida: La materia %s no fue encontrada.\n", nombre);
}

void eliminar_materia(sistema *sistema, char *nombre) {
    materia *actual = sistema->materias;
    materia *anterior = NULL;
    while (actual != NULL) {
        if (strcasecmp(actual->nombre, nombre) == 0) {
            if (anterior == NULL) {
                sistema->materias = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            free(actual);
            printf("Eliminacion exitosa: La Materia %s fue eliminada.\n", nombre);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Eliminacion fallida: La Materia %s no fue encontrada.\n", nombre);
}

void buscar_estudiantes_por_rango_edad(sistema *sistema, int edad_min, int edad_max) {
    estudiante *actual = sistema->estudiantes;
    printf("Estudiantes en el rango de edad %d - %d:\n", edad_min, edad_max);
    while (actual != NULL) {
        if (actual->edad >= edad_min && actual->edad <= edad_max) {
            printf("%s, %s, %d\n", actual->nombre, actual->apellido, actual->edad);
        }
        actual = actual->siguiente;
    }
}

void liberar_memoria_estudiantes(estudiante *estudiantes) {
    while (estudiantes != NULL) {
        estudiante *temp = estudiantes;
        estudiantes = estudiantes->siguiente;
        while (temp->materias_alumno != NULL) {
            materias_alumno *temp_mat = temp->materias_alumno;
            temp->materias_alumno = temp->materias_alumno->siguiente;
            free(temp_mat);
        }
        free(temp);
    }
}

void liberar_memoria_materias(materia *materias) {
    while (materias != NULL) {
        materia *temp = materias;
        materias = materias->siguiente;
        free(temp);
    }
}

void calcular_estadisticas_materias_cursadas_actualmente(sistema *sistema) {
    materia *actual_materia = sistema->materias;
    int total_materias = 0;
    int total_estudiantes = 0;
    int *materias_cursadas = NULL;

    //Contamos el número total de materias y estudiantes
    while (actual_materia != NULL) {
        total_materias++;
        actual_materia = actual_materia->siguiente;
    }

    estudiante *actual_estudiante = sistema->estudiantes;
    while (actual_estudiante != NULL) {
        total_estudiantes++;
        actual_estudiante = actual_estudiante->siguiente;
    }

    //Reservamos memoria para el contador de materias cursadas
    materias_cursadas = malloc(total_materias * sizeof(int));
    for (int i = 0; i < total_materias; i++) {
        materias_cursadas[i] = 0;
    }

    //Contamos cuántos estudiantes cursan cada materia
    actual_estudiante = sistema->estudiantes;
    while (actual_estudiante != NULL) {
        materias_alumno *materia_alumno = actual_estudiante->materias_alumno;
        while (materia_alumno != NULL) {
            int codigo = materia_alumno->codigo_materia;
            materia *materia_actual = sistema->materias;
            int index = 0;
            while (materia_actual != NULL) {
                if (materia_actual->codigo == codigo) {
                    materias_cursadas[index]++;
                    break;
                }
                materia_actual = materia_actual->siguiente;
                index++;
            }
            materia_alumno = materia_alumno->siguiente;
        }
        actual_estudiante = actual_estudiante->siguiente;
    }

    //Imprimimos las estadísticas
    actual_materia = sistema->materias;
    int index = 0;
    while (actual_materia != NULL) {
        float porcentaje = ((float) materias_cursadas[index] / total_estudiantes) * 100;
        printf("Materia: %s (Código: %d) es cursada por %d estudiantes actualmente(%0.2f%%)\n", actual_materia->nombre, actual_materia->codigo, materias_cursadas[index], porcentaje);
        actual_materia = actual_materia->siguiente;
        index++;
    }

    free(materias_cursadas);
}

void calcular_estadisticas_materias_aprobadas(sistema *sistema) {
    materia *actual_materia = sistema->materias;
    int total_materias = 0;
    int total_estudiantes = 0;
    int *materias_aprobadass = NULL;

    //Contamos el número total de materias y estudiantes
    while (actual_materia != NULL) {
        total_materias++;
        actual_materia = actual_materia->siguiente;
    }

    estudiante *actual_estudiante = sistema->estudiantes;
    while (actual_estudiante != NULL) {
        total_estudiantes++;
        actual_estudiante = actual_estudiante->siguiente;
    }

    //Reservamos memoria para el contador de materias aprobadas
    materias_aprobadass = malloc(total_materias * sizeof(int));
    for (int i = 0; i < total_materias; i++) {
        materias_aprobadass[i] = 0;
    }

    //Contamos cuántos estudiantes han aprobado cada materia
    actual_estudiante = sistema->estudiantes;
    while (actual_estudiante != NULL) {
        materias_aprobadas *materia_aprobada_alumno = actual_estudiante->materias_aprobadas;
        while (materia_aprobada_alumno != NULL) {
            int codigo = materia_aprobada_alumno->codigo_materia;
            materia *materia_actual = sistema->materias;
            int index = 0;
            while (materia_actual != NULL) {
                if (materia_actual->codigo == codigo) {
                    materias_aprobadass[index]++;
                    break;
                }
                materia_actual = materia_actual->siguiente;
                index++;
            }
            materia_aprobada_alumno = materia_aprobada_alumno->siguiente;
        }
        actual_estudiante = actual_estudiante->siguiente;
    }

    //Imprimimos las estadísticas
    actual_materia = sistema->materias;
    int index = 0;
    while (actual_materia != NULL) {
        float porcentaje = ((float) materias_aprobadass[index] / total_estudiantes) * 100;
        printf("Materia: %s (Código: %d) ha sido aprobada por %d estudiantes (%0.2f%%)\n", actual_materia->nombre, actual_materia->codigo, materias_aprobadass[index], porcentaje);
        actual_materia = actual_materia->siguiente;
        index++;
    }

    free(materias_aprobadass);
}

int main() {
    sistema *sistema = crear_sistema();
 

    int opcion;
    do
    {
        printf("\n--- Sistema de Gestion Academica ---\n");
        printf("1. Agregar Estudiante\n");
        printf("2. Modificar Estudiante\n");
        printf("3. Eliminar Estudiante\n");
        printf("4. Listar Estudiantes\n");
        printf("5. Agregar Materia\n");
        printf("6. Agregar correlatividad\n");
        printf("7. Modificar Materia\n");
        printf("8. Eliminar Materia\n");
        printf("9. Listar Materias\n");
        printf("10. Inscribir Estudiante en Materia\n");
        printf("11. Buscar Estudiantes por Rango de Edad\n");
        printf("12. Consultar de materias en curso\n");
        printf("13. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        char nombre[50], apellido[50], nuevo_nombre[50], nuevo_apellido[50];
        int edad, codigo, cupo, edad_min, edad_max, nuevo_cupo, nuevo_codigo, codigo_correlativa;
        estudiante alumno;
    
        switch (opcion)

        {
       case 1://AGREGAR ESTUDIANTE
                printf("Ingrese el nombre del estudiante: ");
                scanf("%s", nombre);
                printf("Ingrese el apellido: ");
                scanf("%s", apellido);
                printf("Ingrese edad ");
                scanf("%d", &edad);
                agregar_estudiante(sistema, nombre, apellido, edad);
                break;

            case 2://MODIFICAR ESTUDIANTE
                printf("Ingrese el nombre del estudiante: ");
                scanf("%s", nombre);
                printf("Ingrese el apellido: ");
                scanf("%s", apellido);
                printf("Ingrese el nuevo nombre: ");
                scanf("%s", nuevo_nombre);
                printf("Ingrese el nuevo apellido: ");
                scanf("%s", nuevo_apellido);
                printf("Ingrese la nueva edad: ");
                scanf("%d", &edad);
                modificar_estudiante(sistema,nombre,apellido,&nuevo_nombre,&nuevo_apellido, &edad);
                break;  

            case 3://ELIMINAR ESTUDIANTE
                printf("Ingrese el nombre del estudiante a eliminar: ");
                scanf("%s", nombre);
                printf("Ingrese el apellido: ");
                scanf("%s", apellido);
                eliminar_estudiante(sistema, nombre, apellido);
                break;
            case 4://LISTAR ESTUDIANTES
                listar_estudiantes(sistema);
                break;
            case 5://CREAR MATERIA
                printf("Ingrese el nombre de la materia: ");
                scanf("%s", nombre);
                printf("Ingrese el codigo: ");
                scanf("%d", &codigo);
                printf("Ingrese el cupo del durso: ");
                scanf("%d", &cupo);          
                agregar_materia(sistema, nombre, codigo, cupo);
                break;

            case 6:// AGREGA CORRELATIVA
                printf("Ingrese el codigo de la materia base: \n");
                scanf("%s", &codigo);
                printf("Ingrese el codigo de la materia correlativa: \n");
                scanf("%s", &codigo_correlativa);
                agregar_materia_correlativa(sistema, codigo, codigo_correlativa);

            case 7://MODIFICAR MATERIA
                printf("Ingrese el nombre de la materia: ");
                scanf("%s", nombre);
                printf("Ingrese el nuevo nombre: ");
                scanf("%s", nuevo_nombre);
                printf("Ingrese el nuevo codigo: ");
                scanf("%d", &nuevo_codigo);
                printf("Ingrese el nuevo cupo del curso: ");
                scanf("%d", &nuevo_cupo);
                modificar_materia(sistema, nombre, nuevo_nombre, nuevo_codigo, nuevo_cupo);
                break;
            case 8://ELIMINAR MATERIA
                printf("Ingrese el nombre de la materia: ");
                scanf("%s", nombre);
                eliminar_materia(sistema, nombre);
                break;
            case 9://LISTAR MATERIAS    
                listar_materias(sistema);
                break;
            case 10://INSCRIPCION A MATERIAS
                printf("Ingrese el nombre del estudiante: ");
                scanf("%s", nombre);
                printf("Ingrese el apellido: ");
                scanf("%s", apellido);
                printf("Ingrese el codigo de la materia: ");
                scanf("%d", &codigo);
                cursar_materia(sistema, nombre, apellido, codigo);
                break;
            case 11://ESTUDIASTES POR RANGO DE EDAD
                printf("Ingrese la edad minima: ");
                scanf("%d", &edad_min);
                printf("Ingrese la edad maxima: ");
                scanf("%d", &edad_max);
                buscar_estudiantes_por_rango_edad(sistema, edad_min, edad_max);
                break;
            case 12://MATERIAS CURSADAS POR EL ALUMNO
                printf("Ingrese el nombre del alumno: ");
                scanf("%s", &nombre);
                alumno = estudiante_por_nombre(sistema, nombre);
                printf("El alumno %s %s cursa las siguientes materias: \n", nombre, apellido);

            case 13://CALCULO ESTADISTICAS DE MATERIAS APROBADAS
                printf("Calculando estadisticas... \n");
                calcular_estadisticas_materias_aprobadas(&sistema);
                break;
        
            case 14:
                free(sistema);
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }  

    } while (opcion !=14);

    return 0;
}