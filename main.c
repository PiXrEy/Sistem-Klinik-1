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

struct Reservasi {
    char id[10];
    char dokter[50];
    char tanggal[20];
    char waktu[10];
};

struct Reservasi r[MAX];
int totalR = 0;

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

void muatdata() {
    FILE *file = fopen("pasien.txt", "r");
    if (!file) return;

    while (fscanf(file, "%[^,],%[^,],%d,%[^,],%[^\n]\n",
                  p[jumlah].id, p[jumlah].nama,
                  &p[jumlah].umur, p[jumlah].telp, p[jumlah].dokter) == 5) {
        jumlah++;
    }
    fclose(file);
}

void simpanReservasi() {
    FILE *file = fopen("reservasi.txt", "w");
    if (!file) return;

    for (int i = 0; i < totalR; i++) {
        fprintf(file, "%s,%s,%s,%s\n",
                r[i].id, r[i].dokter, r[i].tanggal, r[i].waktu);
    }
    fclose(file);
}

void muatReservasi() {
    FILE *file = fopen("reservasi.txt", "r");
    if (!file) return;

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n",
                  r[totalR].id, r[totalR].dokter,
                  r[totalR].tanggal, r[totalR].waktu) == 4) {
        totalR++;
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

void updatedata() {
    system("cls");
    char cariID[10];
    int found = -1;

    printf("\nMasukkan ID pasien yang ingin diupdate: ");
    scanf("%s", cariID);

    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].id, cariID) == 0) {
            found = i; break;
        }
    }

    if (found == -1) {
        printf("Pasien tidak ditemukan!\n");
        return;
    }

    printf("\n=== Data Lama ===\n");
    printf("Nama   : %s\n", p[found].nama);
    printf("Umur   : %d\n", p[found].umur);
    printf("Telp   : %s\n", p[found].telp);

    printf("\n=== Masukkan Data Baru ===\n");
    printf("Nama Baru   : ");
    scanf(" %[^\n]", p[found].nama);
    printf("Umur Baru   : ");
    scanf("%d", &p[found].umur);
    printf("Telp Baru   : ");
    scanf("%s", p[found].telp);

    simpanData();
    printf("Data berhasil diupdate!\n");
}

void lihatJadwalDokter() {
    system("cls");
    printf("\n=== JADWAL DOKTER ===\n");
    printf("1. dr. Anya (Umum)  : Senin - Jumat, 08.00 - 15.00\n");
    printf("2. dr. Gilang (Gigi): Senin - Sabtu, 09.00 - 17.00\n");
    printf("3. dr. Keiko (Anak) : Selasa - Jumat, 10.00 - 14.00\n");
    printf("4. dr. Diva (Kulit) : Rabu - Sabtu, 13.00 - 18.00\n");
}

void buatJanji() {
    system("cls");

    printf("\nMasukkan ID Pasien: ");
    scanf("%s", r[totalR].id);

    printf("Nama Dokter: ");
    scanf(" %[^\n]", r[totalR].dokter);

    printf("Tanggal (DD-MM-YYYY): ");
    scanf("%s", r[totalR].tanggal);

    printf("Waktu (HH:MM): ");
    scanf("%s", r[totalR].waktu);

    totalR++;
    simpanReservasi();
    printf("Janji berhasil dibuat!\n");
}

void ubahJanji() {
    system("cls");
    char id[10];
    int found = -1;

    printf("\nMasukkan ID pasien: ");
    scanf("%s", id);

    for (int i = 0; i < totalR; i++) {
        if (strcmp(r[i].id, id) == 0) {
            found = i; break;
        }
    }

    if (found == -1) {
        printf("Janji tidak ditemukan!\n");
        return;
    }

    printf("Masukkan Dokter Baru: ");
    scanf(" %[^\n]", r[found].dokter);

    printf("Tanggal Baru: ");
    scanf("%s", r[found].tanggal);

    printf("Waktu Baru: ");
    scanf("%s", r[found].waktu);

    simpanReservasi();
    printf("Janji berhasil diubah!\n");
}

void batalkanJanji() {
    system("cls");
    char id[10];
    int found = -1;

    printf("Masukkan ID pasien: ");
    scanf("%s", id);

    for (int i = 0; i < totalR; i++) {
        if (strcmp(r[i].id, id) == 0) {
            found = i; break;
        }
    }

    if (found == -1) {
        printf("Janji tidak ditemukan!\n");
        return;
    }

    for (int i = found; i < totalR - 1; i++) {
        r[i] = r[i + 1];
    }

    totalR--;
    simpanReservasi();
    printf("Janji berhasil dibatalkan!\n");
}

void antrianDigital() {
    system("cls");
    printf("\n=== ANTRIAN DIGITAL ===\n");

    if (totalR == 0) {
        printf("Belum ada antrian.\n");
        return;
    }

    for (int i = 0; i < totalR; i++) {
        printf("%d. ID: %s | Dokter: %s | %s %s\n",
               i + 1, r[i].id, r[i].dokter, r[i].tanggal, r[i].waktu);
    }
}

void jadwalreservasi() {
    int pilih;

    do {
        system("cls");
        printf("\n=== MENU RESERVASI ===\n");
        printf("1. Lihat Jadwal Dokter\n");
        printf("2. Buat Janji\n");
        printf("3. Ubah Janji\n");
        printf("4. Batalkan Janji\n");
        printf("5. Antrian Digital\n");
        printf("6. Kembali\n");
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: lihatJadwalDokter(); break;
            case 2: buatJanji(); break;
            case 3: ubahJanji(); break;
            case 4: batalkanJanji(); break;
            case 5: antrianDigital(); break;
        }

        if (pilih != 6) {
            printf("\nTekan Enter...");
            getchar();
            getchar();
        }
    } while (pilih != 6);
}


int main() {
    int menu;

    muatData();
    muatReservasi();

    do {
        system("cls");
        printf("\n===== SISTEM KLINIK KELOMPOK 4 =====\n");
        printf("1. Daftar Pasien\n");
        printf("2. Lihat Pasien\n");
        printf("3. Update Data Pasien\n");
        printf("4. Hapus Data Pasien\n");
        printf("5. Pilih Dokter untuk Pasien\n");
        printf("6. Menu Reservasi & Antrian\n");
        printf("7. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1: tambahPasien(); break;
            case 2: lihatPasien(); break;
            case 3: updatedata(); break;
            case 4: hapusPasien(); break;
            case 5: jadwalDokter(); break;
            case 6: jadwalreservasi(); break;
            case 7: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid!\n");
        }

        if (menu != 7) {
            printf("\nTekan Enter untuk kembali ke menu...");
            getchar();
            getchar();
        }

    } while (menu != 7);

    return 0;
}
