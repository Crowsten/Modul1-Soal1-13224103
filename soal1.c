/** 
 * EL2008 Praktikum Pemecahan Masalah dengan Pemrograman 2024/2025 
 * Modul : DATA MANIPULATION, EXTERNAL FILES, STRUCTURES AND DYNAMIC ARRAYS 
 * Hari dan Tanggal : Rabu 04 November 2026 
 * Nama (NIM) : Jonathan Chandra (13224103) 
 * Nama File : Soal1.c 
 * Deskripsi : Penjelajah Planet 
 * */ 

// Referensi
// https://www.w3schools.com/c/c_files_read.php
// https://www.geeksforgeeks.org/c/c-program-to-read-contents-of-whole-file/
// https://stackoverflow.com/questions/12048576/how-to-find-eof-through-fscanf
// https://www.geeksforgeeks.org/c/c-bubble-sort/
// https://stackoverflow.com/questions/75986182/i-want-to-use-bubblesort-on-a-text-file-to-sort-a-set-of-randomised-numbers
// https://www.geeksforgeeks.org/c/basics-file-handling-c/
// https://www.geeksforgeeks.org/c/strcmp-in-c/

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

void bersihkan_baris(char *baris) {
    int baca = 0;
    int tulis = 0;

    while (baris[baca] != '\0') {
        if (baris[baca] != '[' && baris[baca] != ']') {
            baris[tulis++] = baris[baca];
        }
        baca++;
    }
    baris[tulis] = '\0';

    while (tulis > 0 && (baris[tulis - 1] == '\n' ||
                         baris[tulis - 1] == '\r' ||
                         baris[tulis - 1] == ' ' ||
                         baris[tulis - 1] == '\t')) {
        baris[--tulis] = '\0';
    }
}

void tukar(Planet *a, Planet *b) {
    Planet sementara = *a;
    *a = *b;
    *b = sementara;
}

void sort(const char *nama_file, const char *zona_target) {
    FILE *file;
    buka(nama_file, &file);

    Planet *daftar_planet = NULL;
    int count = 0;
    int capacity = 0;
    char baris[256];

    while (fgets(baris, sizeof(baris), file) != NULL) {
        Planet sensor;
        bersihkan_baris(baris);

        if (sscanf(baris, " %19[^,],%19[^,],%f",
                   sensor.id, sensor.lokasi, &sensor.suhu) == 3 &&
            strcmp(sensor.lokasi, zona_target) == 0) {
            if (count == capacity) {
                int kapasitas_baru = capacity == 0 ? 10 : capacity * 2;
                Planet *daftar_baru = realloc(
                    daftar_planet, kapasitas_baru * sizeof(Planet));
                if (daftar_baru == NULL) {
                    free(daftar_planet);
                    fclose(file);
                    exit(1);
                }
                daftar_planet = daftar_baru;
                capacity = kapasitas_baru;
            }
            daftar_planet[count++] = sensor;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("999\n");
        free(daftar_planet);
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (daftar_planet[j].suhu < daftar_planet[j + 1].suhu ||
                (daftar_planet[j].suhu == daftar_planet[j + 1].suhu &&
                 strcmp(daftar_planet[j].id, daftar_planet[j + 1].id) > 0)) {
                tukar(&daftar_planet[j], &daftar_planet[j + 1]);
            }
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s %.2f\n", daftar_planet[i].id, daftar_planet[i].suhu);
    }

    free(daftar_planet);
}

void rata_rata(const char *nama_file, const char *zona_target) {
    FILE *file;
    buka(nama_file, &file);

    char baris[256];
    int count = 0;
    float total = 0.0;

    while (fgets(baris, sizeof(baris), file) != NULL) {
        Planet sensor;
        bersihkan_baris(baris);

        if (sscanf(baris, " %19[^,],%19[^,],%f",
                   sensor.id, sensor.lokasi, &sensor.suhu) == 3 &&
            strcmp(sensor.lokasi, zona_target) == 0) {
            total += sensor.suhu;
            count++;
        }
    }

    if (count > 0) {
        float avg = total / count;
        printf("AVERAGE %.2f\n", avg);
    }
    fclose(file);
}

int main(void) {
    char nama_file[100];
    char zona_target[100];

    if (scanf("%s %s", nama_file, zona_target) != 2) {
        return 1;
    }
    sort(nama_file, zona_target);
    rata_rata(nama_file, zona_target);

    return 0;
}
