#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct masina{
    char *brand;
    char *numar;
    char *combustibil;
    float consum;
    int km;
};

struct aux{
    char *brand;
    float consum;
    float pret;
    char *numar;
};


void a(int N, struct masina *mas, int *r){
    for(int i = 0; i < N; i++){
        if(strcmp(mas[i].combustibil, "benzina") == 0)
            r[0]++;
        else if(strcmp(mas[i].combustibil, "motorina") == 0)
            r[1]++;
        else if(strcmp(mas[i].combustibil, "electric") == 0)
            r[2]++;
        else if(strcmp(mas[i].combustibil, "hibrid") == 0)
            r[3]++;
    }
}

void b(int *n, struct masina *mas, struct aux *sol){
    for(int i = 0; i < *n; i++){
        sol[i].brand = mas[i].brand;
        sol[i].consum = ( mas[i].consum / 100.0f ) * (float)mas[i].km;
    }

    for(int i = 0; i < *n-1; i++){
        for(int j = i+1; j < *n; j++){
            if(strcmp(sol[i].brand, sol[j].brand) == 0){
                sol[i].consum += sol[j].consum;
                sol[i].pret += sol[j].pret;

                for(int k = j; k < *n-1; k++){
                    //swap(sol[k], sol[k+1]
                    float auxC, auxP;
                    char *baux;

                    baux = sol[k].brand;
                    auxC = sol[k].consum;
                    auxP = sol[k].pret;

                    sol[k].brand = sol[k+1].brand;
                    sol[k].consum = sol[k+1].consum;
                    sol[k].pret = sol[k+1].pret;

                    sol[k+1].brand = baux;
                    sol[k+1].consum = auxC;
                    sol[k+1].pret = auxP;
                }
                *n-=1;
            }
        }
    }

}

void c(int N, struct masina *mas, struct aux *sol, int *x){
    for(int i = 0; i < N; i++){
        bool ok2 = true;
        int j,k;
        int strEnd = strlen(mas[i].numar)-1;
        if(strEnd < 5 || strEnd > 6 ){
            sol[*x].brand = mas[i].brand;
            sol[*x].numar = mas[i].numar;
            *x += 1;
            continue;
        }
        for(j = strEnd; j > strEnd-3; j--){
            if(!(mas[i].numar[j] >= 'A' && mas[i].numar[j] <='Z')){
                ok2 = false;
                break;
            }
        }
        if(!ok2){
            sol[*x].brand = mas[i].brand;
            sol[*x].numar = mas[i].numar;
            *x += 1;
            continue;
        }

        if(mas[i].numar[0] >= 'A' && mas[i].numar[0] <='Z'){
            if(mas[i].numar[1] >='A' && mas[i].numar[1] <='Z')
                k = 2;
            else k = 1;
        }else ok2 = false;

        if(!ok2){
            sol[*x].brand = mas[i].brand;
            sol[*x].numar = mas[i].numar;
            *x += 1;
            continue;
        }

        for(k; k <= j; k++){
            if(!(mas[i].numar[k] > '0' && mas[i].numar[k] < '9')) {
                ok2 = false;
                break;
            }
        }

        if(!ok2){
            sol[*x].brand = mas[i].brand;
            sol[*x].numar = mas[i].numar;
            *x += 1;
            continue;
        }
    }
}

void citire(FILE *inputFile, int N, struct masina *mas, char *cerinta){
    char cBuf[2];
    for(int i = 0; i < N; i++){
        char buf[100]; // spatiut temporar de stocare pentru citire
        float consum; int km;

        // citeste in buffer, calculeaza memoria pe care o ocupa data citita
        // apoi alocheaza spatiu si copiaza in memorie
        fscanf(inputFile, "%20s", buf);
        mas[i].brand = (char*) malloc(strlen(buf)*sizeof(char) + 1);
        strncpy(mas[i].brand, buf, strlen(buf)*sizeof(char));
        mas[i].brand[strlen(buf)*sizeof(char)] = '\0';

        fscanf(inputFile, "%20s", buf);
        mas[i].numar = (char*) malloc(strlen(buf)*sizeof(char) + 1);
        strncpy(mas[i].numar, buf, strlen(buf)*sizeof(char));
        mas[i].numar[strlen(buf)*sizeof(char)] = '\0';

        fscanf(inputFile, "%20s", buf);
        mas[i].combustibil = (char*) malloc(strlen(buf)*sizeof(char) + 1);
        strncpy(mas[i].combustibil, buf, strlen(buf)*sizeof(char));
        mas[i].combustibil[strlen(buf)*sizeof(char)] = '\0';

        fscanf(inputFile, "%s", buf);
        consum = strtof(buf, NULL);
        memcpy(&(mas+i)->consum, &consum, sizeof(consum));

        fscanf(inputFile, "%s", buf);
        km = strtol(buf, NULL, 10);
        memcpy(&(mas+i)->km, &km, sizeof(km));
    }
    fscanf(inputFile, "%1s", cBuf);
    *cerinta = cBuf[0];
}

int main() {

    errno = 0;
    FILE *inputFile = fopen("citire.txt", "r");

    int N;
    char cerinta;
    if(inputFile == NULL) {
        printf("Could not open file! %d", errno);
        return -100;
    }else
        fscanf(inputFile, "%d", &N);


    struct masina *mas =(struct masina*) malloc((N * sizeof(struct masina)) + 1);
    citire(inputFile, N, mas, &cerinta);

    if(cerinta == 'a'){
        int *res = (int*) calloc(4, sizeof(int));
        a(N, mas, res);
        printf("benzina - %d\nmotorina - %d\nelectric - %d\nhibrid - %d\n", res[0], res[1], res[2], res[3]);
        free(res);
    }else if(cerinta == 'b'){
        struct aux *sol = (struct aux*) malloc(N*sizeof(struct aux));
        int n = N;
        b(&n, mas, sol);

        for(int i = 0; i < n; i++){
            if(strcmp(mas[i].combustibil, "benzina") == 0 || strcmp(mas[i].combustibil, "hibrid") == 0){
                sol[i].pret = sol[i].consum * 8.02f;
            }else if(strcmp(mas[i].combustibil, "motorina") == 0){
                sol[i].pret = sol[i].consum * 9.29f;
            }else sol[i].pret = 0;
            printf("%s a consumat %0.2f - %0.2f lei\n", sol[i].brand, sol[i].consum, sol[i].pret);
        }

        free(sol);
    }else if(cerinta=='c'){
        struct aux *sol = (struct aux*) malloc(N*sizeof(struct aux));
        int x = 0;
        c(N, mas, sol, &x);
        if(x == 0){
            printf("Numere corecte!");
        }else{
            for(int i = 0; i < x; i++){
                printf("%s cu numarul %s: numar invalid\n", sol[i].brand, sol[i].numar);
            }
        }
    }

    return 0;
}
