/** 
 * EL2008 Praktikum Pemecahan Masalah dengan Pemrograman 2024/2025 
 * Modul : DATA MANIPULATION, EXTERNAL FILES, STRUCTURES AND DYNAMIC ARRAYS 
 * Hari dan Tanggal : Rabu 04 November 2026 
 * Nama (NIM) : Jonathan Chandra (13224103) 
 * Nama File : Soal1.c 
 * Deskripsi : Penjelajah Planet 
 * */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char id[20];
    char lokasi[20];
    float suhu;   
}Planet;

void buka(const char *nama_file, FILE **file) {
    *file = fopen(nama_file, "r");
    if (!*file) {
        exit(1);
    }
}

void sort(const char *nama_file, const char *zona_target) {
    FILE *file;
    buka(nama_file, &file);

    Planet *daftar_planet = NULL;
    int count = 0;
    int capacity = 10;
    daftar_planet = (Planet *)malloc(capacity * sizeof(Planet));

    char ch;
    while ((ch = fgetc(file)) != EOF && ch != '[');

    char id[20];
    char lokasi[50];
    float suhu;

    while (fscanf(file, " %[^,],%[^,],%f", id, lokasi, &suhu) == 3) {
        if (strcmp(lokasi, zona_target) == 0) {
            if (count >= capacity) {
                capacity *= 2;
                daftar_planet = (Planet *)realloc(daftar_planet, capacity * sizeof(Planet));
            }
            strcpy(daftar_planet[count].id, id);
            strcpy(daftar_planet[count].lokasi, lokasi);
            daftar_planet[count].suhu = suhu;
            count++;
        }
        
        ch = fgetc(file);
        if (ch == ']') {
            break;
        }
    }
    fclose(file);

    if (count == 0) {
        free(daftar_planet);
        return;
    }


    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (daftar_planet[j].suhu < daftar_planet[j + 1].suhu) {
                Planet temp = daftar_planet[j];
                daftar_planet[j] = daftar_planet[j + 1];
                daftar_planet[j + 1] = temp;
            }
        }
    }

    float total = 0.0;
    for (int i = 0; i < count; i++) {
        printf("%s %.2f\n", daftar_planet[i].id, daftar_planet[i].suhu);
        total += daftar_planet[i].suhu;
    }

    float avg = total / count;
    printf("AVERAGE %.2f\n", avg);
    free(daftar_planet);
}

void rata_rata(const char *nama_file, const char *zona_target) {
    FILE *file;
    buka(nama_file, &file);

    char id[20];
    char lokasi[50];
    float suhu;
    
    int count = 0;
    float total = 0.0;

    while (fscanf(file, " %[^,],%[^,],%f", id, lokasi, &suhu) == 3) {
        if (strcmp(lokasi, zona_target) == 0) {
            total += suhu;
            count++;
        }
    }

    if (count > 0) {
        float avg = total / count;
        printf("AVERAGE %s: %.2f\n", zona_target, avg);
    }
    fclose(file);
}

int main() {
    char nama_file[100];
    char zona_target[100];

    scanf("%s", nama_file);
    scanf("%s", zona_target);
    sort(nama_file, zona_target);
    rata_rata(nama_file, zona_target);

    return 0;
}
