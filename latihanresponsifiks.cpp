#include <iostream>
#include <string>
#include <stack>

using namespace std;

struct perpus {
    string judul;
    string pengarang;
    int tahun;

    perpus *kiri;
    perpus *kanan;

    perpus(string ju, string peng, int thn) {
        judul = ju;
        pengarang = peng;
        tahun = thn;
        kiri = nullptr;
        kanan = nullptr;
    }
};

struct aksi {
    string aksi;
    perpus *data;
};

stack<aksi> undostack;

void tambahdata(perpus *&akar, string judul, string pengarang, int tahun, bool rekamaksi = true) {
    if (akar == nullptr) {
        akar = new perpus(judul, pengarang, tahun);
        if (rekamaksi)
            undostack.push({"tambah", new perpus(judul, pengarang, tahun)});
        return;
    }

    perpus *bantu = akar;
    while (true) {
        if (judul < bantu->judul) {
            if (bantu->kiri == nullptr) {
                bantu->kiri = new perpus(judul, pengarang, tahun);
                if (rekamaksi)
                    undostack.push({"tambah", new perpus(judul, pengarang, tahun)});
                return;
            }
            bantu = bantu->kiri;
        } else if (judul > bantu->judul) {
            if (bantu->kanan == nullptr) {
                bantu->kanan = new perpus(judul, pengarang, tahun);
                if (rekamaksi)
                    undostack.push({"tambah", new perpus(judul, pengarang, tahun)});
                return;
            }
            bantu = bantu->kanan;
        } else {
            cout << "Buku dengan judul " << judul << " sudah terdaftar\n";
            return;
        }
    }
}

void tampildata(perpus *akar) {
    if (akar == nullptr) return;
    tampildata(akar->kanan);
    cout << akar->judul << " - " << akar->pengarang << " - " << akar->tahun << endl;
    tampildata(akar->kiri);
}

void hapusdata(perpus *&akar, string ygdihapus, bool rekamaksi = true) {
    if (akar == nullptr) {
        cout << "Buku dengan judul " << ygdihapus << " tidak ditemukan" << endl;
        return;
    }

    if (ygdihapus < akar->judul) {
        hapusdata(akar->kiri, ygdihapus, rekamaksi);
    } else if (ygdihapus > akar->judul) {
        hapusdata(akar->kanan , ygdihapus, rekamaksi);
    } else {
        perpus *bantu = akar;
        if (rekamaksi)
            undostack.push({"hapus", new perpus(bantu->judul, bantu->pengarang, bantu->tahun)});

        if (akar->kiri == nullptr){
            akar = akar->kanan;
        } else if (akar->kanan == nullptr){
            akar = akar->kiri;
        } else {
            perpus *ganti = akar->kanan;
            while (ganti->kiri != nullptr) {
                ganti = ganti->kiri;
            }
            akar->judul = ganti->judul;
            akar->pengarang = ganti->pengarang;
            akar->tahun = ganti->tahun;
            hapusdata(akar->kanan, ganti->judul, false);
            return;
        }
        delete bantu;
    }
}

void undo(perpus *&akar) {
    if (undostack.empty()) {
        cout << "Tidak ada aksi untuk di-undo.\n";
        return;
    }

    aksi aksiterakhir = undostack.top();
    undostack.pop();

    if (aksiterakhir.aksi == "tambah") {
        hapusdata(akar, aksiterakhir.data->judul, false);
        cout << "Undo: Tambah dibatalkan, buku dihapus.\n";
    } else if (aksiterakhir.aksi == "hapus") {
        tambahdata(akar, aksiterakhir.data->judul, aksiterakhir.data->pengarang, aksiterakhir.data->tahun, false);
        cout << "Undo: Hapus dibatalkan, buku dikembalikan.\n";
    }

    delete aksiterakhir.data;
}

int main() {
    int pil;
    perpus *akar = nullptr;
    string judul, pengarang, ygdihapus;
    int tahun;

    do {
        system("cls");
        cout << "Menu:\n";
        cout << "1. Tambah data buku\n";
        cout << "2. Tampilkan semua data buku\n";
        cout << "3. Hapus Buku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "0. Keluar\n";
        cout << "Masukan pilihan menu: ";
        cin >> pil;
        cin.ignore();

        switch (pil)
        {
        case 1:
            cout << "Masukan judul buku: ";
            getline(cin, judul);
            cout << "Masukan nama pengarang buku: ";
            getline(cin, pengarang);
            cout << "Masukan tahun terbit buku: ";
            cin >> tahun;
            tambahdata(akar, judul, pengarang, tahun);
            cout << "Data berhasil ditambahkan\n";
            system("pause");
            break;

        case 2:
            cout << "Data Buku:\n";
            tampildata(akar);
            system("pause");
            break;

        case 3:
            cout << "Masukan judul buku yang ingin dihapus: ";
            getline(cin, ygdihapus);
            hapusdata(akar, ygdihapus);
            system("pause");
            break;

        case 4:
            undo(akar);
            system("pause");
            break;

        case 0:
            cout << "Keluar program\n";
            break;

        default:
            cout << "Pilihan tidak valid\n";
            system("pause");
            break;
        }
    } while (pil != 0);

    return 0;
}
