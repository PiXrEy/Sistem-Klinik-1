#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

struct Pasien {
    char id[10];
    char nama[50];
    int umur;
    char telp[20];
    char dokter[50];
};

struct Pasien p[MAX];
int jumlah = 0;



void simpanData() {
    FILE *file = fopen("pasien.txt", "w");
    if (file == NULL) {
        printf("Gagal menyimpan data!\n");
        return;
    }

    for (int i = 0; i < jumlah; i++) {
        fprintf(file, "%s,%s,%d,%s,%s\n",
                p[i].id, p[i].nama, p[i].umur, p[i].telp, p[i].dokter);
    }

    fclose(file);
}

void muatData() {
    FILE *file = fopen("pasien.txt", "r");
    if (file == NULL) {
        return; // Tidak ada file = belum ada data
    }

    while (fscanf(file, "%[^,],%[^,],%d,%[^,],%[^\n]\n",
                  p[jumlah].id,
                  p[jumlah].nama,
                  &p[jumlah].umur,
                  p[jumlah].telp,
                  p[jumlah].dokter) == 5) {
        jumlah++;
    }

    fclose(file);
}


void tambahPasien() {
    system("cls");
    printf("\n=== Tambah Pasien ===\n");
    printf("Masukkan ID            : ");
    scanf("%s", p[jumlah].id);

    printf("Masukkan Nama Pasien   : ");
    scanf(" %[^\n]", p[jumlah].nama);

    printf("Masukkan Umur          : ");
    scanf("%d", &p[jumlah].umur);

    printf("Masukkan No. Telepon   : ");
    scanf("%s", p[jumlah].telp);

    strcpy(p[jumlah].dokter, "Belum memilih dokter");

    jumlah++;
    simpanData(); // Simpan otomatis setelah tambah pasien
    printf("Data pasien berhasil ditambahkan!\n");
}

void lihatPasien() {
    system("cls");
    printf("\n=== Data Pasien Terdaftar ===\n");

    if (jumlah == 0) {
        printf("Belum ada pasien terdaftar.\n");
        return;
    }

    printf("\nNo | ID Pasien  | Nama Pasien          | Umur | No. Telepon      | Dokter Terpilih\n");
    printf("------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlah; i++) {
        printf("%-3d| %-11s| %-20s | %-4d | %-16s | %-20s\n",
               i + 1,
               p[i].id,
               p[i].nama,
               p[i].umur,
               p[i].telp,
               p[i].dokter);
    }

    printf("------------------------------------------------------------------------------------\n");
}

void jadwalDokter() {
    system("cls");
    char cariID[10];
    int found = -1;
    int pilihan;

    printf("\nMasukkan ID pasien: ");
    scanf("%s", cariID);

    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].id, cariID) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Pasien tidak ditemukan!\n");
        return;
    }

    printf("\n=== Pilihan Dokter ===\n");
    printf("1. dr. Anya (Umum)\n");
    printf("2. dr. Gilang (Gigi)\n");
    printf("3. dr. Keiko (Anak)\n");
    printf("4. dr. Diva (Kulit)\n");

    printf("\nPilih dokter: ");
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1: strcpy(p[found].dokter, "dr. Anya (Umum)"); break;
        case 2: strcpy(p[found].dokter, "dr. Gilang (Gigi)"); break;
        case 3: strcpy(p[found].dokter, "dr. Keiko (Anak)"); break;
        case 4: strcpy(p[found].dokter, "dr. Diva (Kulit)"); break;
        default: strcpy(p[found].dokter, "Tidak valid");
    }

    simpanData();
    printf("Dokter berhasil dipilih!\n");
}

void hapusPasien() {
    system("cls");
    char hapusID[10];
    int found = -1;

    printf("\nMasukkan ID pasien yang ingin dihapus: ");
    scanf("%s", hapusID);

    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].id, hapusID) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Data pasien tidak ditemukan!\n");
        return;
    }

    for (int j = found; j < jumlah - 1; j++) {
        p[j] = p[j + 1];
    }

    jumlah--;
    simpanData();
    printf("Data pasien berhasil dihapus!\n");
}



int main() {
    int menu;

    muatData();

    do {
        system("cls");
        printf("\n===== SISTEM KLINIK KELOMPOK 4 =====\n");
        printf("1. Daftar Pasien\n");
        printf("2. Lihat Pasien\n");
        printf("3. Pilih Dokter untuk Pasien\n");
        printf("4. Hapus Data Pasien\n");
        printf("5. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1: tambahPasien(); break;
            case 2: lihatPasien(); break;
            case 3: jadwalDokter(); break;
            case 4: hapusPasien(); break;
            case 5: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid!\n");
        }

        if (menu != 5) {
            printf("\nTekan Enter untuk kembali ke menu...");
            getchar();
            getchar();
        }

    } while (menu != 5);

    return 0;
}
