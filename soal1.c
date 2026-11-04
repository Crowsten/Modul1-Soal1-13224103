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

typedef struct {
    char id[20];
    char zona[50];
    float suhu;
} Sensor;

void tukar(Sensor *a, Sensor *b) {
    Sensor sementara = *a;
    *a = *b;
    *b = sementara;
}

void urutkan(Sensor data[], size_t jumlah) {
    for (size_t i = 0; i < jumlah; i++) {
        for (size_t j = i + 1; j < jumlah; j++) {
            if (data[i].suhu < data[j].suhu ||
                (data[i].suhu == data[j].suhu &&
                 strcmp(data[i].id, data[j].id) > 0)) {
                tukar(&data[i], &data[j]);
            }
        }
    }
}

int spasi(char karakter) {
    return karakter == ' ' || karakter == '\t' || karakter == '\n' ||
           karakter == '\r' || karakter == '\v' || karakter == '\f';
}

char *trim(char *teks) {
    while (spasi(*teks)) {
        teks++;
    }

    char *akhir = teks + strlen(teks);
    while (akhir > teks && spasi(akhir[-1])) {
        *--akhir = '\0';
    }
    return teks;
}

int main(void) {
    char nama_file[256];
    char zona_target[50];
    if (scanf("%s %s", nama_file, zona_target) != 2) {
        return 1;
    }

    FILE *file = fopen(nama_file, "r");
    if (file == NULL) {
        return 1;
    }

    Sensor *data = NULL;
    size_t jumlah = 0;
    size_t kapasitas = 0;
    char baris[256];

    while (fgets(baris, sizeof(baris), file) != NULL) {
        char *isi = trim(baris);
        if (*isi == '[') {
            isi = trim(isi + 1);
        }

        size_t panjang = strlen(isi);
        if (panjang > 0 && isi[panjang - 1] == ']') {
            isi[--panjang] = '\0';
            isi = trim(isi);
        }
        if (*isi == '\0') {
            continue;
        }

        Sensor sensor;
        char sisa;
        if (sscanf(isi, " %19[^,],%49[^,],%f %c",
            sensor.id, sensor.zona, &sensor.suhu, &sisa) != 3) {
            free(data);
            fclose(file);
            return 1;
        }

        char *id = trim(sensor.id);
        char *zona = trim(sensor.zona);
        if (id != sensor.id) {
            memmove(sensor.id, id, strlen(id) + 1);
        }
        if (zona != sensor.zona) {
            memmove(sensor.zona, zona, strlen(zona) + 1);
        }

        if (strcmp(sensor.zona, zona_target) == 0) {
            if (jumlah == kapasitas) {
                size_t kapasitas_baru = kapasitas == 0 ? 8 : kapasitas * 2;
                Sensor *data_baru = realloc(data, kapasitas_baru * sizeof(*data));
                if (data_baru == NULL) {
                    free(data);
                    fclose(file);
                    return 1;
                }
                data = data_baru;
                kapasitas = kapasitas_baru;
            }
            data[jumlah++] = sensor;
        }
    }

    if (ferror(file)) {
        free(data);
        fclose(file);
        return 1;
    }
    fclose(file);

    if (jumlah == 0) {
        puts("999");
        free(data);
        return 0;
    }

    urutkan(data, jumlah);
    double total = 0.0;
    for (size_t i = 0; i < jumlah; i++) {
        printf("%s %.2f\n", data[i].id, data[i].suhu);
        total += data[i].suhu;
    }
    printf("AVERAGE %.2f\n", total / jumlah);

    free(data);
    return 0;
}
