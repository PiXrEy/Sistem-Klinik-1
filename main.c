#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

void formatRupiah(float angka, char *out) {
    char buf[64];
    sprintf(buf, "%.2f", angka);
    char depan[64] = {0}, belakang[8] = {0};
    char *dot = strchr(buf, '.');
    if (dot) {
        int idx = dot - buf;
        strncpy(depan, buf, idx);
        depan[idx] = '\0';
        strcpy(belakang, dot + 1);
    } else {
        strcpy(depan, buf);
        strcpy(belakang, "00");
    }
    int len = strlen(depan);
    char rev[64] = {0};
    int pos = 0, cnt = 0;
    for (int i = len - 1; i >= 0; --i) {
        rev[pos++] = depan[i];
        cnt++;
        if (cnt == 3 && i != 0) {
            rev[pos++] = '.';
            cnt = 0;
        }
    }
    rev[pos] = '\0';
    int L = strlen(rev);
    char hasil[64] = {0};
    for (int i = 0; i < L; ++i) hasil[i] = rev[L - i - 1];
    hasil[L] = '\0';
    sprintf(out, "Rp %s,%s", hasil, belakang);
}

void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t L = strlen(buf);
    if (L > 0 && buf[L-1] == '\n') buf[L-1] = '\0';
    if (buf[0] == '\0') {
        if (fgets(buf, size, stdin) == NULL) { buf[0] = '\0'; return; }
        L = strlen(buf);
        if (L > 0 && buf[L-1] == '\n') buf[L-1] = '\0';
    }
}

int isLeapYear(int y) {
    return ( (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0) );
}

int isValidDate(int d, int m, int y) {
    if (y < 1900 || y > 3000) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1) return 0;
    int mdays = 31;
    if (m==4||m==6||m==9||m==11) mdays = 30;
    else if (m==2) mdays = isLeapYear(y) ? 29 : 28;
    if (d > mdays) return 0;
    return 1;
}

int isValidTime(int hh, int mm) {
    if (hh < 0 || hh > 23) return 0;
    if (mm < 0 || mm > 59) return 0;
    return 1;
}

struct Pasien {
    char id[10];
    char nama[50];
    int umur;
    char telp[20];
    char dokter[50];
} p[MAX];
int jumlah = 0;

struct Reservasi {
    char id[10];
    char dokter[50];
    char tanggal[20];
    char waktu[10];
    char jenis[20];
} r[MAX];
int totalR = 0;

struct Obat {
    char kode[10];
    char nama[50];
    int stok;
    float harga;
    char expired[15];
} obat[MAX];
int jumlahObat = 0;

struct TransaksiObat {
    char pasien[50];
    char namaObat[50];
    int jumlahBeli;
    float totalHarga;
} logTrans[MAX];
int jumlahTrans = 0;

struct Invoice {
    char pasien[50];
    float totalBiaya;
    char metode[20];
    char status[20];
} riwayatInv[MAX];
int jumlahInvoice = 0;

struct Tarif {
    float konsultasi;
    float tindakan;
} tarif = {50000.0f, 100000.0f};

void simpanData() {
    FILE *file = fopen("pasien.txt", "w");
    if (!file) return;
    for (int i = 0; i < jumlah; i++) {
        fprintf(file, "%s,%s,%d,%s,%s\n",
                p[i].id, p[i].nama, p[i].umur, p[i].telp, p[i].dokter);
    }
    fclose(file);
}

void muatData() {
    FILE *file = fopen("pasien.txt", "r");
    if (!file) return;
    jumlah = 0;
    while (fscanf(file, "%[^,],%[^,],%d,%[^,],%[^\n]\n",
                  p[jumlah].id,
                  p[jumlah].nama,
                  &p[jumlah].umur,
                  p[jumlah].telp,
                  p[jumlah].dokter) == 5) {
        jumlah++;
        if (jumlah >= MAX) break;
    }
    fclose(file);
}

void simpanReservasi() {
    FILE *file = fopen("reservasi.txt", "w");
    if (!file) return;
    for (int i = 0; i < totalR; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n",
                r[i].id, r[i].dokter, r[i].tanggal, r[i].waktu, r[i].jenis);
    }
    fclose(file);
}

void muatReservasi() {
    FILE *file = fopen("reservasi.txt", "r");
    if (!file) return;
    totalR = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                  r[totalR].id, r[totalR].dokter,
                  r[totalR].tanggal, r[totalR].waktu, r[totalR].jenis) == 5) {
        totalR++;
        if (totalR >= MAX) break;
    }
    fclose(file);
}

void simpanObat() {
    FILE *f = fopen("obat.txt", "w");
    if (!f) return;
    for (int i = 0; i < jumlahObat; i++) {
        fprintf(f, "%s,%s,%d,%.2f,%s\n",
                obat[i].kode, obat[i].nama, obat[i].stok, obat[i].harga, obat[i].expired);
    }
    fclose(f);
}

void muatObat() {
    FILE *f = fopen("obat.txt", "r");
    if (!f) {
        struct Obat defaults[] = {
            {"OB001","Paracetamol 500mg",50,5000.0f,"12-2026"},
            {"OB002","Amoxicillin 500mg",40,12000.0f,"10-2025"},
            {"OB003","Antasida Syrup",30,8000.0f,"08-2026"},
            {"OB004","Vitamin C 500mg",60,4000.0f,"01-2027"},
            {"OB005","Ibuprofen 200mg",35,9000.0f,"05-2026"}
        };
        int n = sizeof(defaults)/sizeof(defaults[0]);
        for (int i = 0; i < n && jumlahObat < MAX; i++) {
            obat[jumlahObat++] = defaults[i];
        }
        simpanObat();
        return;
    }
    jumlahObat = 0;
    while (fscanf(f, "%[^,],%[^,],%d,%f,%[^\n]\n",
                  obat[jumlahObat].kode,
                  obat[jumlahObat].nama,
                  &obat[jumlahObat].stok,
                  &obat[jumlahObat].harga,
                  obat[jumlahObat].expired) == 5) {
        jumlahObat++;
        if (jumlahObat >= MAX) break;
    }
    fclose(f);
}

void simpanTransaksi() {
    FILE *f = fopen("transaksi_obat.txt", "w");
    if (!f) return;
    for (int i = 0; i < jumlahTrans; i++) {
        fprintf(f, "%s,%s,%d,%.2f\n",
                logTrans[i].pasien, logTrans[i].namaObat, logTrans[i].jumlahBeli, logTrans[i].totalHarga);
    }
    fclose(f);
}

void muatTransaksi() {
    FILE *f = fopen("transaksi_obat.txt", "r");
    if (!f) return;
    jumlahTrans = 0;
    while (fscanf(f, "%[^,],%[^,],%d,%f\n",
                  logTrans[jumlahTrans].pasien,
                  logTrans[jumlahTrans].namaObat,
                  &logTrans[jumlahTrans].jumlahBeli,
                  &logTrans[jumlahTrans].totalHarga) == 4) {
        jumlahTrans++;
        if (jumlahTrans >= MAX) break;
    }
    fclose(f);
}

void simpanInvoice() {
    FILE *f = fopen("invoice.txt", "w");
    if (!f) return;
    for (int i = 0; i < jumlahInvoice; i++) {
        fprintf(f, "%s,%.2f,%s,%s\n",
                riwayatInv[i].pasien,
                riwayatInv[i].totalBiaya,
                riwayatInv[i].metode,
                riwayatInv[i].status);
    }
    fclose(f);
}

void muatInvoice() {
    FILE *f = fopen("invoice.txt", "r");
    if (!f) return;
    jumlahInvoice = 0;
    while (fscanf(f, "%[^,],%f,%[^,],%[^\n]\n",
                  riwayatInv[jumlahInvoice].pasien,
                  &riwayatInv[jumlahInvoice].totalBiaya,
                  riwayatInv[jumlahInvoice].metode,
                  riwayatInv[jumlahInvoice].status) == 4) {
        jumlahInvoice++;
        if (jumlahInvoice >= MAX) break;
    }
    fclose(f);
}

float totalObatPasien(char *nama) {
    float total = 0.0f;
    for (int i = 0; i < jumlahTrans; i++) {
        if (strcmp(logTrans[i].pasien, nama) == 0) {
            total += logTrans[i].totalHarga;
        }
    }
    return total;
}

void tambahPasien() {
    system("cls");
    if (jumlah >= MAX) { printf("Kapasitas pasien penuh!\n"); return; }
    printf("\n=== Tambah Pasien ===\n");
    printf("Masukkan ID           : ");
    scanf("%s", p[jumlah].id);
    getchar();
    printf("Masukkan Nama Pasien   : ");
    readLine(p[jumlah].nama, sizeof(p[jumlah].nama));
    printf("Masukkan Umur          : ");
    scanf("%d", &p[jumlah].umur);
    getchar();
    printf("Masukkan No. Telepon   : ");
    scanf("%s", p[jumlah].telp);
    strcpy(p[jumlah].dokter, "Belum memilih dokter");
    jumlah++;
    simpanData();
    printf("Data pasien berhasil ditambahkan!\n");
}

void lihatPasien() {
    system("cls");
    printf("\n=== Data Pasien Terdaftar ===\n");
    if (jumlah == 0) { printf("Belum ada pasien terdaftar.\n"); return; }
    printf("\nNo | ID Pasien   | Nama Pasien          | Umur | No. Telepon      | Dokter Terpilih\n");
    printf("------------------------------------------------------------------------------------\n");
    for (int i = 0; i < jumlah; i++) {
        printf("%-3d| %-11s| %-20s | %-4d | %-16s | %-20s\n",
               i + 1, p[i].id, p[i].nama, p[i].umur, p[i].telp, p[i].dokter);
    }
    printf("------------------------------------------------------------------------------------\n");
}

void updatedata() {
    system("cls");
    char cariID[10];
    int found = -1;
    printf("\nMasukkan ID pasien yang ingin diupdate: ");
    scanf("%s", cariID);
    for (int i = 0; i < jumlah; i++) if (strcmp(p[i].id, cariID) == 0) { found = i; break; }
    if (found == -1) { printf("Pasien tidak ditemukan!\n"); return; }
    printf("\n=== Data Lama ===\nNama   : %s\nUmur   : %d\nTelp   : %s\n",
           p[found].nama, p[found].umur, p[found].telp);
    printf("\n=== Masukkan Data Baru ===\n");
    printf("Nama Baru   : "); getchar(); readLine(p[found].nama, sizeof(p[found].nama));
    printf("Umur Baru   : "); scanf("%d", &p[found].umur);
    printf("Telp Baru   : "); scanf("%s", p[found].telp);
    simpanData();
    printf("Data berhasil diupdate!\n");
}

void hapusPasien() {
    system("cls");
    char hapusID[10]; int found = -1;
    printf("\nMasukkan ID pasien yang ingin dihapus: ");
    scanf("%s", hapusID);
    for (int i = 0; i < jumlah; i++) if (strcmp(p[i].id, hapusID) == 0) { found = i; break; }
    if (found == -1) { printf("Data pasien tidak ditemukan!\n"); return; }
    for (int j = found; j < jumlah - 1; j++) p[j] = p[j+1];
    jumlah--; simpanData();
    printf("Data pasien berhasil dihapus!\n");
}

void lihatJadwalDokter() {
    system("cls");
    printf("\n=== JADWAL DOKTER ===\n");
    printf("1. dr. Anya (Umum)   : Senin - Jumat, 08.00 - 15.00\n");
    printf("2. dr. Gilang (Gigi) : Senin - Sabtu, 09.00 - 17.00\n");
    printf("3. dr. Keiko (Anak)  : Selasa - Jumat, 10.00 - 14.00\n");
    printf("4. dr. Diva (Kulit)  : Rabu - Sabtu, 13.00 - 18.00\n");
}

void buatJanji() {
    system("cls");
    if (totalR >= MAX) { printf("Kapasitas reservasi penuh!\n"); return; }
    char idPasien[10];
    printf("\n=== Buat Janji Terstruktur ===\n");
    printf("Masukkan ID Pasien (terdaftar): ");
    scanf("%s", idPasien);
    int idxPas = -1;
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].id, idPasien) == 0) { idxPas = i; break; }
    }
    if (idxPas == -1) {
        printf("Pasien dengan ID '%s' tidak ditemukan! Buatlah data pasien terlebih dahulu.\n", idPasien);
        return;
    }
    int pilihan;
    printf("\nPilih dokter untuk janji:\n");
    printf("1. dr. Anya (Umum)\n");
    printf("2. dr. Gilang (Gigi)\n");
    printf("3. dr. Keiko (Anak)\n");
    printf("4. dr. Diva (Kulit)\n");
    printf("Pilih (1-4): ");
    scanf("%d", &pilihan);
    getchar();
    char dokterTerpilih[50];
    switch (pilihan) {
        case 1: strcpy(dokterTerpilih, "dr. Anya (Umum)"); break;
        case 2: strcpy(dokterTerpilih, "dr. Gilang (Gigi)"); break;
        case 3: strcpy(dokterTerpilih, "dr. Keiko (Anak)"); break;
        case 4: strcpy(dokterTerpilih, "dr. Diva (Kulit)"); break;
        default:
            printf("Pilihan dokter tidak valid.\n");
            return;
    }
    int pilihanJenis;
    char jenisJanji[20];
    printf("\nPilih jenis janji:\n");
    printf("1. Konsultasi\n");
    printf("2. Tindakan\n");
    printf("Pilih (1-2): ");
    scanf("%d", &pilihanJenis);
    getchar();
    switch (pilihanJenis) {
        case 1: strcpy(jenisJanji, "Konsultasi"); break;
        case 2: strcpy(jenisJanji, "Tindakan"); break;
        default:
            printf("Pilihan jenis tidak valid. Batal.\n");
            return;
    }
    char tanggalStr[32];
    int d, m, y;
    int attempts = 0;
    while (1) {
        printf("Tanggal kunjungan (DD-MM-YYYY) : ");
        readLine(tanggalStr, sizeof(tanggalStr));
        if (sscanf(tanggalStr, "%d-%d-%d", &d, &m, &y) == 3) {
            if (isValidDate(d,m,y)) {
                sprintf(tanggalStr, "%02d-%02d-%04d", d, m, y);
                break;
            }
        }
        attempts++;
        printf("Format tanggal salah atau tidak valid. Contoh: 05-09-2025\n");
        if (attempts >= 3) { printf("Gagal memasukkan tanggal valid. Batal.\n"); return; }
    }
    char waktuStr[16];
    int hh, mm;
    attempts = 0;
    while (1) {
        printf("Waktu janji (HH:MM, 24-jam)      : ");
        readLine(waktuStr, sizeof(waktuStr));
        if (sscanf(waktuStr, "%d:%d", &hh, &mm) == 2) {
            if (isValidTime(hh, mm)) {
                sprintf(waktuStr, "%02d:%02d", hh, mm);
                break;
            }
        }
        attempts++;
        printf("Format waktu salah. Contoh: 09:30 atau 15:05\n");
        if (attempts >= 3) { printf("Gagal memasukkan waktu valid. Batal.\n"); return; }
    }
    strcpy(r[totalR].id, idPasien);
    strcpy(r[totalR].dokter, dokterTerpilih);
    strcpy(r[totalR].tanggal, tanggalStr);
    strcpy(r[totalR].waktu, waktuStr);
    strcpy(r[totalR].jenis, jenisJanji);
    totalR++;
    simpanReservasi();
    strcpy(p[idxPas].dokter, dokterTerpilih);
    simpanData();
    lihatPasien();
    printf("\n====================================================================================");
    printf("\nJanji berhasil dibuat untuk pasien %s (ID: %s)\n", p[idxPas].nama, idPasien);
    printf("Dokter  : %s\n", dokterTerpilih);
    printf("Tanggal : %s\n", tanggalStr);
    printf("Waktu   : %s\n", waktuStr);
    printf("Jenis   : %s\n", jenisJanji);
    printf("Data dokter pada tabel di atas telah diperbarui.\n");
    printf("====================================================================================\n");
}

void ubahJanji() {
    system("cls");
    char id[10]; int found = -1;
    printf("\nMasukkan ID pasien: "); scanf("%s", id);
    for (int i = 0; i < totalR; i++) if (strcmp(r[i].id, id) == 0) { found = i; break; }
    if (found == -1) { printf("Janji tidak ditemukan!\n"); return; }
    char tanggalLama[20], waktuLama[10];
    strcpy(tanggalLama, r[found].tanggal);
    strcpy(waktuLama, r[found].waktu);
    int pilihan;
    printf("\nPilih dokter baru:\n1. dr. Anya (Umum)\n2. dr. Gilang (Gigi)\n3. dr. Keiko (Anak)\n4. dr. Diva (Kulit)\n");
    printf("Pilih (1-4): "); scanf("%d", &pilihan);
    switch (pilihan) {
        case 1: strcpy(r[found].dokter, "dr. Anya (Umum)"); break;
        case 2: strcpy(r[found].dokter, "dr. Gilang (Gigi)"); break;
        case 3: strcpy(r[found].dokter, "dr. Keiko (Anak)"); break;
        case 4: strcpy(r[found].dokter, "dr. Diva (Kulit)"); break;
        default: printf("Pilihan dokter tidak valid.\n"); return;
    }
    int pilihanJenis;
    printf("\nPilih jenis janji baru:\n1. Konsultasi\n2. Tindakan\n");
    printf("Pilih (1-2): ");
    scanf("%d", &pilihanJenis);
    getchar();
    switch (pilihanJenis) {
        case 1: strcpy(r[found].jenis, "Konsultasi"); break;
        case 2: strcpy(r[found].jenis, "Tindakan"); break;
        default: printf("Pilihan jenis tidak valid, set ke Konsultasi.\n");
                 strcpy(r[found].jenis, "Konsultasi");
                 break;
    }
    char tanggalStr[32];
    int d,m,y;
    printf("Tanggal Baru (DD-MM-YYYY): "); readLine(tanggalStr, sizeof(tanggalStr));
    if (sscanf(tanggalStr, "%d-%d-%d", &d, &m, &y) == 3 && isValidDate(d,m,y)) {
        sprintf(r[found].tanggal, "%02d-%02d-%04d", d, m, y);
    } else {
        printf("Format tanggal tidak valid, mempertahankan tanggal lama.\n");
        strcpy(r[found].tanggal, tanggalLama);
    }
    char waktuStr[16];
    int hh,mm;
    printf("Waktu Baru (HH:MM): "); readLine(waktuStr, sizeof(waktuStr));
    if (sscanf(waktuStr, "%d:%d", &hh, &mm) == 2 && isValidTime(hh,mm)) {
        sprintf(r[found].waktu, "%02d:%02d", hh, mm);
    } else {
        printf("Format waktu tidak valid, mempertahankan waktu lama.\n");
        strcpy(r[found].waktu, waktuLama);
    }
    simpanReservasi();
    printf("Janji berhasil diubah!\n");
    printf("Info Janji Baru: Dokter: %s, Jenis: %s\n", r[found].dokter, r[found].jenis);
    int idxPas = -1;
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].id, r[found].id) == 0) {
            idxPas = i;
            break;
        }
    }
    if (idxPas != -1) {
        strcpy(p[idxPas].dokter, r[found].dokter);
        simpanData();
        printf("Data dokter di Menu 2 (Lihat Pasien) juga telah diupdate.\n");
    }
}

void batalkanJanji() {
    system("cls");
    char id[10]; int found = -1;
    printf("Masukkan ID pasien: "); scanf("%s", id);
    for (int i = 0; i < totalR; i++) if (strcmp(r[i].id, id) == 0) { found = i; break; }
    if (found == -1) { printf("Janji tidak ditemukan!\n"); return; }
    int idxPas = -1;
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].id, r[found].id) == 0) {
            idxPas = i;
            break;
        }
    }
    if (idxPas != -1) {
        strcpy(p[idxPas].dokter, "Belum memilih dokter");
        simpanData();
        printf("Status dokter di Menu 2 (Lihat Pasien) telah di-reset.\n");
    }
    for (int i = found; i < totalR - 1; i++) r[i] = r[i+1];
    totalR--;
    simpanReservasi();
    printf("Janji berhasil dibatalkan!\n");
}

void antrianDigital() {
    system("cls");
    printf("\n=== ANTRIAN DIGITAL ===\n");
    if (totalR == 0) { printf("Belum ada antrian.\n"); return; }
    for (int i = 0; i < totalR; i++) {
        printf("%d. ID: %s | Dokter: %s | %s %s | (%s)\n",
               i + 1, r[i].id, r[i].dokter, r[i].tanggal, r[i].waktu, r[i].jenis);
    }
}

void jadwalreservasi() {
    int pilih;
    do {
        system("cls");
        printf("\n=== MENU RESERVASI ===\n");
        printf("1. Lihat Jadwal Dokter\n2. Buat Janji\n3. Ubah Janji\n4. Batalkan Janji\n5. Antrian Digital\n6. Kembali\nPilih: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: lihatJadwalDokter(); break;
            case 2: buatJanji(); break;
            case 3: ubahJanji(); break;
            case 4: batalkanJanji(); break;
            case 5: antrianDigital(); break;
        }
        if (pilih != 6) { printf("\nTekan Enter..."); getchar(); getchar(); }
    } while (pilih != 6);
}

void tambahObat() {
    system("cls");
    if (jumlahObat >= MAX) { printf("Kapasitas obat penuh!\n"); return; }
    printf("\n=== Tambah Obat ===\n");
    printf("Kode obat (mis. OB006): "); scanf("%s", obat[jumlahObat].kode); getchar();
    printf("Nama obat      : "); readLine(obat[jumlahObat].nama, sizeof(obat[jumlahObat].nama));
    printf("Stok           : "); scanf("%d", &obat[jumlahObat].stok);
    printf("Harga (Rp)     : "); scanf("%f", &obat[jumlahObat].harga); getchar();
    printf("Expired (MM-YYYY): "); readLine(obat[jumlahObat].expired, sizeof(obat[jumlahObat].expired));
    jumlahObat++; simpanObat(); printf("Obat berhasil ditambahkan!\n");
}

void tampilkanObat() {
    system("cls");
    printf("\n=== DAFTAR OBAT ===\n");
    if (jumlahObat == 0) { printf("Belum ada data obat.\n"); return; }
    printf("\nKode   | Nama Obat                  | Stok | Harga           | Exp\n");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < jumlahObat; i++) {
        char h[64];
        formatRupiah(obat[i].harga, h);
        printf("%-6s | %-23s | %-4d | %-15s | %s\n",
               obat[i].kode, obat[i].nama, obat[i].stok, h, obat[i].expired);
        if (obat[i].stok <= 5) printf("   PERINGATAN: Stok menipis!\n");
    }
}

void transaksiObat() {
    system("cls");
    char kode[10];
    int qty;
    printf("\n=== Transaksi Obat ===\n");
    printf("Masukkan kode obat: "); scanf("%s", kode);
    int idx = -1;
    for (int i = 0; i < jumlahObat; i++) if (strcmp(obat[i].kode, kode) == 0) { idx = i; break; }
    if (idx == -1) { printf("Obat tidak ditemukan!\n"); return; }
    char hargaRp[64];
    formatRupiah(obat[idx].harga, hargaRp);
    printf("Nama: %s | Stok: %d | Harga: %s\n", obat[idx].nama, obat[idx].stok, hargaRp);
    printf("Jumlah beli: "); scanf("%d", &qty);
    if (qty <= 0) { printf("Jumlah tidak valid.\n"); return; }
    if (obat[idx].stok < qty) { printf("Stok tidak cukup! Stok tersedia: %d\n", obat[idx].stok); return; }
    char namaPasien[50];
    getchar();
    printf("Nama pasien pembeli: ");
    readLine(namaPasien, sizeof(namaPasien));
    obat[idx].stok -= qty;
    float total = obat[idx].harga * qty;
    strcpy(logTrans[jumlahTrans].pasien, namaPasien);
    strcpy(logTrans[jumlahTrans].namaObat, obat[idx].nama);
    logTrans[jumlahTrans].jumlahBeli = qty;
    logTrans[jumlahTrans].totalHarga = total;
    jumlahTrans++;
    simpanObat(); simpanTransaksi();
    char totalRp[64];
    formatRupiah(total, totalRp);
    printf("Transaksi berhasil! Total = %s\n", totalRp);
    if (jumlahInvoice >= MAX) {
        printf("Kapasitas invoice penuh, tidak bisa mencatat ke billing.\n");
        return;
    }
    int pilihMetode;
    printf("Masukkan metode bayar [1.Tunai / 2.Transfer]: ");
    scanf("%d", &pilihMetode);
    strcpy(riwayatInv[jumlahInvoice].pasien, namaPasien);
    riwayatInv[jumlahInvoice].totalBiaya = total;
    strcpy(riwayatInv[jumlahInvoice].metode, (pilihMetode == 1) ? "Tunai" : "Transfer");
    strcpy(riwayatInv[jumlahInvoice].status, "Lunas");
    jumlahInvoice++;
    simpanInvoice();
    printf("Transaksi obat ini juga telah dicatat di Riwayat Pembayaran (Billing).\n");
}

void laporanPenjualanObat() {
    system("cls");
    printf("\n=== LAPORAN PENJUALAN OBAT ===\n");
    if (jumlahTrans == 0) { printf("Belum ada transaksi.\n"); return; }
    float totalKeseluruhan = 0;
    for (int i = 0; i < jumlahTrans; i++) {
        char itemTotalRp[64];
        formatRupiah(logTrans[i].totalHarga, itemTotalRp);
        printf("%d. Pasien: %s | Obat: %s | Jumlah: %d | Total: %s\n",
               i + 1, logTrans[i].pasien, logTrans[i].namaObat, logTrans[i].jumlahBeli, itemTotalRp);
        totalKeseluruhan += logTrans[i].totalHarga;
    }
    char totalRp[64];
    formatRupiah(totalKeseluruhan, totalRp);
    printf("TOTAL PEMASUKAN OBAT: %s\n", totalRp);
}

void tambahStokObat() {
    system("cls");
    printf("\n=== Pemasukan / Tambah Stok Obat ===\n");
    tampilkanObat();
    if (jumlahObat == 0) return;
    char kode[10];
    int idx = -1;
    int jumlahTambah;
    printf("\nMasukkan kode obat yang akan ditambah stoknya: ");
    scanf("%s", kode);
    for (int i = 0; i < jumlahObat; i++) {
        if (strcmp(obat[i].kode, kode) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Obat dengan kode '%s' tidak ditemukan!\n", kode);
        return;
    }
    printf("Obat: %s | Stok Saat Ini: %d\n", obat[idx].nama, obat[idx].stok);
    printf("Masukkan jumlah stok yang ditambahkan: ");
    scanf("%d", &jumlahTambah);
    if (jumlahTambah <= 0) {
        printf("Jumlah tambahan tidak valid.\n");
        return;
    }
    obat[idx].stok += jumlahTambah;
    simpanObat();
    printf("Stok berhasil ditambahkan.\n");
    printf("Stok baru %s sekarang: %d\n", obat[idx].nama, obat[idx].stok);
}

void hapusObat() {
    system("cls");
    printf("\n=== Hapus Obat dari Daftar ===\n");
    tampilkanObat();
    if (jumlahObat == 0) return;
    char kodeHapus[10];
    int found = -1;
    printf("\nPERINGATAN: Ini akan menghapus obat secara permanen.\n");
    printf("Masukkan kode obat yang akan dihapus: ");
    scanf("%s", kodeHapus);
    getchar();
    for (int i = 0; i < jumlahObat; i++) {
        if (strcmp(obat[i].kode, kodeHapus) == 0) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        printf("Obat dengan kode '%s' tidak ditemukan!\n", kodeHapus);
        return;
    }
    printf("Menghapus %s (Kode: %s)...\n", obat[found].nama, obat[found].kode);
    for (int j = found; j < jumlahObat - 1; j++) {
        obat[j] = obat[j+1];
    }
    jumlahObat--;
    simpanObat();
    printf("Obat berhasil dihapus.\n");
}

void buatInvoiceOtomatis() {
    system("cls");
    if (jumlahInvoice >= MAX) {
        printf("Kapasitas invoice penuh!\n");
        return;
    }
    char pasienNama[50];
    printf("\n=== Buat Invoice Otomatis ===\n");
    printf("Nama pasien (Harus sama dengan di Menu 1): ");
    readLine(pasienNama, sizeof(pasienNama));
    char idPasien[10];
    int idxPas = -1;
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(p[i].nama, pasienNama) == 0) {
            strcpy(idPasien, p[i].id);
            idxPas = i;
            break;
        }
    }
    if (idxPas == -1) {
        printf("Pasien '%s' tidak terdaftar di Menu 1.\n", pasienNama);
        return;
    }
    int idxRes = -1;
    for (int i = 0; i < totalR; i++) {
        if (strcmp(r[i].id, idPasien) == 0) {
            idxRes = i;
            break;
        }
    }
    if (idxRes == -1) {
        printf("Pasien '%s' (ID: %s) tidak memiliki janji aktif (Menu 6).\n", pasienNama, idPasien);
        return;
    }
    float biayaKonsultasi = 0.0f;
    float biayaTindakan = 0.0f;
    if (strcmp(r[idxRes].jenis, "Konsultasi") == 0) {
        biayaKonsultasi = tarif.konsultasi;
        printf("Pasien memilih: Konsultasi (Rp %.2f)\n", tarif.konsultasi);
    } else if (strcmp(r[idxRes].jenis, "Tindakan") == 0) {
        biayaTindakan = tarif.tindakan;
        printf("Pasien memilih: Tindakan (Rp %.2f)\n", tarif.tindakan);
    } else {
        printf("Jenis janji '%s' tidak dikenali, tidak ada biaya layanan.\n", r[idxRes].jenis);
    }
    float biayaObat = totalObatPasien(pasienNama);
    float total = biayaKonsultasi + biayaObat + biayaTindakan;
    int pilih;
    printf("Metode bayar [1.Tunai / 2.Transfer]: ");
    scanf("%d", &pilih);
    strcpy(riwayatInv[jumlahInvoice].pasien, pasienNama);
    riwayatInv[jumlahInvoice].totalBiaya = total;
    strcpy(riwayatInv[jumlahInvoice].metode, (pilih == 1) ? "Tunai" : "Transfer");
    strcpy(riwayatInv[jumlahInvoice].status, "Lunas");
    jumlahInvoice++;
    simpanInvoice();
    char rp1[64], rp2[64], rp3[64], rpTotal[64];
    formatRupiah(biayaKonsultasi, rp1);
    formatRupiah(biayaTindakan, rp2);
    formatRupiah(biayaObat, rp3);
    formatRupiah(total, rpTotal);
    printf("\nInvoice otomatis berhasil dibuat!\n");
    printf("Biaya Konsultasi : %s\n", rp1);
    printf("Biaya Tindakan   : %s\n", rp2);
    printf("Biaya Obat       : %s\n", rp3);
    printf("---------------------------------\n");
    printf("TOTAL BAYAR      : %s\n", rpTotal);
}

void riwayatPembayaranTampil() {
    system("cls");
    printf("\n=== RIWAYAT PEMBAYARAN PASIEN ===\n");
    if (jumlahInvoice == 0) { printf("Belum ada riwayat pembayaran.\n"); return; }
    for (int i = 0; i < jumlahInvoice; i++) {
        char rp[64];
        formatRupiah(riwayatInv[i].totalBiaya, rp);
        printf("%d. %s | Total: %s | Metode: %s | Status: %s\n",
               i + 1, riwayatInv[i].pasien, rp, riwayatInv[i].metode, riwayatInv[i].status);
    }
}

void menuApotek() {
    int pilih;
    do {
        system("cls");
        printf("\n=== MENU APOTEK & OBAT ===\n");
        printf("1. Tambah Obat Baru\n");
        printf("2. Lihat Daftar Obat\n");
        printf("3. Transaksi Obat (Jual)\n");
        printf("4. Laporan Penjualan Obat\n");
        printf("5. Tambah Stok Obat\n");
        printf("6. Hapus Obat\n");
        printf("7. Kembali\n");
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: tambahObat(); break;
            case 2: tampilkanObat(); break;
            case 3: transaksiObat(); break;
            case 4: laporanPenjualanObat(); break;
            case 5: tambahStokObat(); break;
            case 6: hapusObat(); break;
            default: break;
        }
        if (pilih != 7) { printf("\nTekan Enter..."); getchar(); getchar(); }
    } while (pilih != 7);
}

void menuBilling() {
    int pilih;
    do {
        system("cls");
        printf("\n=== MENU BILLING & PAYMENT ===\n");
        printf("1. Buat Invoice\n");
        printf("2. Lihat Riwayat Pembayaran\n");
        printf("3. Kembali\n");
        printf("Pilih: ");
        scanf("%d", &pilih);
        getchar();
        switch (pilih) {
            case 1: buatInvoiceOtomatis(); break;
            case 2: riwayatPembayaranTampil(); break;
        }
        if (pilih != 3) { printf("\nTekan Enter..."); getchar(); getchar(); }
    } while (pilih != 3);
}

int main() {
    int menu;
    muatData();
    muatReservasi();
    muatObat();
    muatTransaksi();
    muatInvoice();

    do {
        system("cls");
        printf("\n===== SISTEM KLINIK KELOMPOK 4 =====\n");
        printf("1. Daftar Pasien\n");
        printf("2. Lihat Pasien\n");
        printf("3. Update Data Pasien\n");
        printf("4. Hapus Data Pasien\n");
        printf("5. Menu Reservasi & Antrian\n");
        printf("6. Menu Apotek & Obat\n");
        printf("7. Menu Billing & Pembayaran\n");
        printf("8. Keluar\n");
        printf("Pilih menu: ");
        if (scanf("%d", &menu) != 1) { fflush(stdin); menu = 0; }
        switch (menu) {
            case 1: tambahPasien(); break;
            case 2: lihatPasien(); break;
            case 3: updatedata(); break;
            case 4: hapusPasien(); break;
            case 5: jadwalreservasi(); break;
            case 6: menuApotek(); break;
            case 7: menuBilling(); break;
            case 8: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid!\n");
        }
        if (menu != 8) { printf("\nTekan Enter untuk kembali ke menu..."); getchar(); getchar(); }
    } while (menu != 8);

    return 0;
}
