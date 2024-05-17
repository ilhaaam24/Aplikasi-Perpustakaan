#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

const int MAX_BUKU = 10;
const int MAX_PEMINJAM = 15;

struct Peminjam
{
  string nama;
  string noHandphone;
  Peminjam *next;
};

Peminjam *tablePeminjam[MAX_PEMINJAM] = {nullptr};

struct Buku
{
  string judul;
  string penulis;
  int tahun;
  Buku *next;
  Buku *prev;
};

Buku *head = nullptr;
Buku *tail = nullptr;
Buku *newNode = nullptr;
Buku *del = nullptr;

struct RAK_BUKU
{
  Buku *rakSains[MAX_BUKU] = {nullptr};
  Buku *rakTeknologi[MAX_BUKU] = {nullptr};
  Buku *rakSeni[MAX_BUKU] = {nullptr};
  Buku *rakNovel[MAX_BUKU] = {nullptr};
} rakBuku;

unordered_map<string, bool> statusPeminjaman;
unordered_map<string, Peminjam> peminjamBuku;

string toLowerCase(const string &str)
{
  string result = str;
  for (char &c : result)
  {
    c = tolower(c);
  }
  return result;
}

int hashFunction(const string &str, int tableSize)
{
  int hash = 0;
  for (char c : str)
  {
    hash = (hash * 31 + c) % tableSize;
  }
  return hash;
}

void tambahBuku(const string &judul, const string &penulis, int tahun, const string &kategori)
{
  newNode = new Buku();
  newNode->judul = judul;
  newNode->penulis = penulis;
  newNode->tahun = tahun;
  newNode->next = nullptr;
  newNode->prev = nullptr;

  // Menentukan rak berdasarkan kategori
  Buku **rak;
  string kategoriLower = toLowerCase(kategori);
  if (kategoriLower == "sains")
  {
    rak = rakBuku.rakSains;
  }
  else if (kategoriLower == "teknologi")
  {
    rak = rakBuku.rakTeknologi;
  }
  else if (kategoriLower == "seni")
  {
    rak = rakBuku.rakSeni;
  }
  else if (kategoriLower == "novel")
  {
    rak = rakBuku.rakNovel;
  }
  else
  {
    cout << "Kategori tidak valid." << endl;
    return;
  }

  // Mencari indeks kosong berikutnya di rak
  int index = 0;
  while (index < MAX_BUKU && rak[index] != nullptr)
  {
    ++index;
  }

  // Menambahkan buku baru ke rak pada indeks yang telah ditemukan
  if (index < MAX_BUKU)
  {
    rak[index] = newNode;
  }
  else
  {
    cout << "Rak penuh. Buku tidak dapat ditambahkan ke dalam rak." << endl;
    delete newNode;
    return;
  }

  statusPeminjaman[judul] = false; // Buku baru dianggap tersedia
}

void cetakDenganLebar(const string &str, int lebar)
{
  cout << str;
  for (int i = str.length(); i < lebar; ++i)
  {
    cout << ' ';
  }
}

void cetakHeaderTabel()
{
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "| No |      JUDUL      |      PENULIS      |      TAHUN      |     STATUS      |" << endl;
  cout << "--------------------------------------------------------------------------------" << endl;
}

void cetakRakBuku(Buku *rak[])
{
  cetakHeaderTabel();
  int nomor = 1;
  for (int i = 0; i < MAX_BUKU; ++i)
  {
    if (rak[i] != nullptr)
    {
      cout << "| ";
      cetakDenganLebar(to_string(nomor++), 2);
      cout << " | ";
      cetakDenganLebar(rak[i]->judul, 15);
      cout << " | ";
      cetakDenganLebar(rak[i]->penulis, 17);
      cout << " | ";
      cout << rak[i]->tahun;
      int tahunPanjang = to_string(rak[i]->tahun).length();
      for (int j = tahunPanjang; j < 15; ++j)
      {
        cout << ' ';
      }
      cout << " | ";
      string status = statusPeminjaman[rak[i]->judul] ? "Dipinjam" : "Tersedia";
      cout << status;
      for (int j = status.length(); j < 15; ++j)
      {
        cout << ' ';
      }
      cout << " |" << endl;
    }
    else
    {
      cout << "| ";
      cetakDenganLebar(to_string(nomor++), 2);
      cout << " | ";
      cetakDenganLebar("", 15); // Cetak judul kosong
      cout << " | ";
      cetakDenganLebar("", 17); // Cetak penulis kosong
      cout << " | ";
      cetakDenganLebar("", 15); // Cetak tahun kosong
      cout << " | ";
      cetakDenganLebar("", 15); // Cetak status kosong
      cout << " |" << endl;
    }
  }
  cout << "--------------------------------------------------------------------------------" << endl;
}

void cetakSemuaRak()
{
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "                                RAK SAINS                                       " << endl;
  cetakRakBuku(rakBuku.rakSains);

  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "                              RAK TEKNOLOGI                                     " << endl;
  cetakRakBuku(rakBuku.rakTeknologi);

  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "                                RAK SENI                                        " << endl;
  cetakRakBuku(rakBuku.rakSeni);

  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "                                RAK NOVEL                                       " << endl;
  cetakRakBuku(rakBuku.rakNovel);
}

void cetakRakTertentu(int pilihanRak)
{
  switch (pilihanRak)
  {
  case 1:
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                RAK SAINS                                       " << endl;
    cetakRakBuku(rakBuku.rakSains);
    break;
  case 2:
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                              RAK TEKNOLOGI                                     " << endl;
    cetakRakBuku(rakBuku.rakTeknologi);
    break;
  case 3:
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                RAK SENI                                        " << endl;
    cetakRakBuku(rakBuku.rakSeni);
    break;
  case 4:
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                RAK NOVEL                                       " << endl;
    cetakRakBuku(rakBuku.rakNovel);
    break;
  default:
    cout << "Pilihan rak tidak valid." << endl;
    break;
  }
}
void bubbleSort(Buku *rak[])
{
  for (int i = 0; i < MAX_BUKU - 1; ++i)
  {
    for (int j = 0; j < MAX_BUKU - i - 1; ++j)
    {
      if (rak[j] != nullptr && rak[j + 1] != nullptr && rak[j]->judul > rak[j + 1]->judul)
      {
        // Tukar posisi buku jika judul buku saat ini lebih besar dari judul buku berikutnya
        Buku *temp = rak[j];
        rak[j] = rak[j + 1];
        rak[j + 1] = temp;
      }
    }
  }
}

Buku *cariBuku(const string &judul)
{
  for (int i = 0; i < MAX_BUKU; ++i)
  {
    if (rakBuku.rakSains[i] != nullptr && toLowerCase(rakBuku.rakSains[i]->judul) == toLowerCase(judul))
    {
      return rakBuku.rakSains[i];
    }
  }
  for (int i = 0; i < MAX_BUKU; ++i)
  {
    if (rakBuku.rakTeknologi[i] != nullptr && toLowerCase(rakBuku.rakTeknologi[i]->judul) == toLowerCase(judul))
    {
      return rakBuku.rakTeknologi[i];
    }
  }
  for (int i = 0; i < MAX_BUKU; ++i)
  {
    if (rakBuku.rakSeni[i] != nullptr && toLowerCase(rakBuku.rakSeni[i]->judul) == toLowerCase(judul))
    {
      return rakBuku.rakSeni[i];
    }
  }
  for (int i = 0; i < MAX_BUKU; ++i)
  {
    if (rakBuku.rakNovel[i] != nullptr && toLowerCase(rakBuku.rakNovel[i]->judul) == toLowerCase(judul))
    {
      return rakBuku.rakNovel[i];
    }
  }
  return nullptr;
}

void tampilkanStatusBuku(Buku *buku)
{
  if (buku != nullptr)
  {
    cout << "Judul: " << buku->judul << endl;
    cout << "Penulis: " << buku->penulis << endl;
    cout << "Tahun: " << buku->tahun << endl;
    string status = statusPeminjaman[buku->judul] ? "Tidak Tersedia (Dipinjam)" : "Tersedia";
    cout << "Status: " << status << endl;
  }
  else
  {
    cout << "Buku tidak ditemukan." << endl;
  }
}

bool linearProbingInsertPeminjam(Peminjam *table[], Peminjam *newPeminjam)
{
  int index = hashFunction(newPeminjam->noHandphone, MAX_PEMINJAM);
  for (int i = 0; i < MAX_PEMINJAM; ++i)
  {
    int probingIndex = (index + i) % MAX_PEMINJAM;
    if (table[probingIndex] == nullptr)
    {
      table[probingIndex] = newPeminjam;
      return true;
    }
  }
  return false;
}

Peminjam *linearProbingSearchPeminjam(Peminjam *table[], const string &noHandphone)
{
  int index = hashFunction(noHandphone, MAX_PEMINJAM);
  for (int i = 0; i < MAX_PEMINJAM; ++i)
  {
    int probingIndex = (index + i) % MAX_PEMINJAM;
    if (table[probingIndex] == nullptr)
    {
      return nullptr;
    }
    if (table[probingIndex]->noHandphone == noHandphone)
    {
      return table[probingIndex];
    }
  }
  return nullptr;
}

void tampilkanDataPeminjam(const string &noHandphone)
{
  Peminjam *peminjamDitemukan = linearProbingSearchPeminjam(tablePeminjam, noHandphone);
  if (peminjamDitemukan != nullptr)
  {
    cout << "Nama: " << peminjamDitemukan->nama << endl;
    cout << "No Handphone: " << peminjamDitemukan->noHandphone << endl;
  }
  else
  {
    cout << "Peminjam tidak ditemukan." << endl;
  }
}
void tambahPeminjam(const string &nama, const string &noHandphone)
{
  Peminjam *peminjam = new Peminjam();
  peminjam->nama = nama;
  peminjam->noHandphone = noHandphone;
  peminjam->next = nullptr;

  if (!linearProbingInsertPeminjam(tablePeminjam, peminjam))
  {
    cout << "Tabel peminjam penuh. Peminjam tidak dapat ditambahkan." << endl;
    delete peminjam;
  }
}

void cetakSemuaPeminjam()
{
  cout << "----------------------------------------------------------" << endl;
  cout << "|                   TABEL PEMINJAM                        |" << endl;
  cout << "----------------------------------------------------------" << endl;
  cout << "| No | Nama                   | No Handphone             |" << endl;
  cout << "----------------------------------------------------------" << endl;
  for (int i = 0; i < MAX_PEMINJAM; ++i)
  {
    if (tablePeminjam[i] != nullptr)
    {
      cout << "| " << (i + 1) << " | ";
      cetakDenganLebar(tablePeminjam[i]->nama, 22);
      cout << " | ";
      cetakDenganLebar(tablePeminjam[i]->noHandphone, 24);
      cout << " |" << endl;
    }
  }
  cout << "----------------------------------------------------------" << endl;
}
void pinjamBuku()
{
  string judulBuku;
  cout << "Masukkan Judul Buku yang ingin dipinjam: ";
  cin.ignore();
  getline(cin, judulBuku);
  Buku *bukuDitemukan = cariBuku(judulBuku);
  if (bukuDitemukan != nullptr && !statusPeminjaman[judulBuku])
  {
    string nama, noHandphone;
    cout << "Masukkan Nama Peminjam: ";
    getline(cin, nama);
    cout << "Masukkan Nomor Handphone Peminjam: ";
    getline(cin, noHandphone);

    tambahPeminjam(nama, noHandphone);

    statusPeminjaman[judulBuku] = true;
    peminjamBuku[judulBuku] = {nama, noHandphone};

    cout << "Buku berhasil dipinjam." << endl;
  }
  else if (bukuDitemukan == nullptr)
  {
    cout << "Buku tidak ditemukan." << endl;
  }
  else
  {
    cout << "Buku sedang dipinjam." << endl;
  }
}

int main()
{
  // Menambahkan beberapa buku untuk pengujian
  tambahBuku("C++", "Bjarne Stroustrup", 1979, "Sains");
  tambahBuku("Java", "James Gosling", 1995, "Sains");
  tambahBuku("Python", "Guido van Rossum", 1991, "Sains");
  tambahBuku("Harry Potter", "J.K. Rowling", 1997, "Novel");
  tambahBuku("The Lord ", "J.R.R. Tolkien", 1954, "Novel");
  tambahBuku("The Hobbit", "J.R.R. Tolkien", 1937, "Novel");

  Buku buku;
  int pilihMenuUtama;
  do
  {
    cout << "================================" << endl;
    cout << "       SISTEM PERPUSTAKAAN      " << endl;
    cout << "================================" << endl;
    cout << "1. Masuk Sebagai Admin" << endl;
    cout << "2. Masuk Sebagai User" << endl;
    cout << "0. Keluar" << endl;
    cout << ">";
    cin >> pilihMenuUtama;
    switch (pilihMenuUtama)
    {
    case 1:
    {
      int pilihMenuAdmin;
      do
      {
        cout << "================================" << endl;
        cout << "       MASUK SEBAGAI ADMIN      " << endl;
        cout << "================================" << endl;
        cout << "1. Tambah Buku" << endl;
        cout << "2. Edit Buku" << endl;
        cout << "3. Hapus Buku" << endl;
        cout << "4. Tampilkan Semua Kategory" << endl;
        cout << "5. Tampilkan Buku per Kategory" << endl;
        cout << "6. List Peminjam" << endl;
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuAdmin;
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          string kategori;
          cout << "Masukan Judul Buku : ";
          cin.ignore();
          getline(cin, buku.judul);
          cout << "Masukan Penulis Buku : ";
          getline(cin, buku.penulis);
          cout << "Masukan Tahun Buku : ";
          cin >> buku.tahun;
          cout << "Masukan Kategori Buku (Sains/Teknologi/Seni/Novel) : ";
          cin.ignore();
          getline(cin, kategori);
          tambahBuku(buku.judul, buku.penulis, buku.tahun, kategori);
          break;
        }
        case 2:
          // Implementasi Edit Buku di sini
          break;
        case 3:
          // Implementasi Hapus Buku di sini
          break;
        case 4:
          cetakSemuaRak();
          break;
        
        case 5:
        {
          int pilihanRak;
          cout << "Pilih rak yang ingin ditampilkan:" << endl;
          cout << "1. Rak Sains" << endl;
          cout << "2. Rak Teknologi" << endl;
          cout << "3. Rak Seni" << endl;
          cout << "4. Rak Novel" << endl;
          cout << ">";
          cin >> pilihanRak;
          cin.ignore(); // Menghindari masalah newline character setelah input angka
          cetakRakTertentu(pilihanRak);
          // Setelah cetak rak tertentu, panggil fungsi sorting
          switch (pilihanRak)
          {
          case 1:
            bubbleSort(rakBuku.rakSains);
            break;
          case 2:
            bubbleSort(rakBuku.rakTeknologi);
            break;
          case 3:
            bubbleSort(rakBuku.rakSeni);
            break;
          case 4:
            bubbleSort(rakBuku.rakNovel);
            break;
          default:
            cout << "Pilihan rak tidak valid." << endl;
            break;
          }
          cetakRakTertentu(pilihanRak); // Cetak rak setelah diurutkan
          break;
        }
        case 6:
          cetakSemuaPeminjam();
          break;
        default:
          cout << "Pilihan Tidak Tersedia" << endl;
          break;
        }
      } while (pilihMenuAdmin != 0);
      break;
    }
    case 2:
    {
      int pilihMenuUser;
      do
      {
        cout << "================================" << endl;
        cout << "       MASUK SEBAGAI USER      " << endl;
        cout << "================================" << endl;
        cout << "1. Cari Buku" << endl;
        cout << "2. Tampilkan Kategori Buku" << endl;
        cout << "3. Pinjam Buku" << endl;
        cout << "4. Kembalikan Buku" << endl;
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuUser;
        switch (pilihMenuUser)
        {
        case 1:
        {
          string judulBuku;
          cout << "Masukkan Judul Buku yang ingin dicari: ";
          cin.ignore();
          getline(cin, judulBuku);
          Buku *bukuDitemukan = cariBuku(judulBuku);
          tampilkanStatusBuku(bukuDitemukan);
          break;
        }
        case 2:
        {
          int pilihanRak;
          cout << "Pilih Kategori yang ingin ditampilkan:" << endl;
          cout << "1. Rak Sains" << endl;
          cout << "2. Rak Teknologi" << endl;
          cout << "3. Rak Seni" << endl;
          cout << "4. Rak Novel" << endl;
          cout << ">";
          cin >> pilihanRak;
          cin.ignore(); // Menghindari masalah newline character setelah input angka
          cetakRakTertentu(pilihanRak);
          break;
        }
        case 3:
        {
          pinjamBuku();
          break;
        }
        case 4:
        {
          string judulBuku;
          cout << "Masukkan Judul Buku yang ingin dikembalikan: ";
          getline(cin, judulBuku);
          Buku *bukuDitemukan = cariBuku(judulBuku);
          if (bukuDitemukan != nullptr && statusPeminjaman[judulBuku])
          {
            statusPeminjaman[judulBuku] = false;
            cout << "Buku berhasil dikembalikan." << endl;
          }
          else if (bukuDitemukan == nullptr)
          {
            cout << "Buku tidak ditemukan." << endl;
          }
          else
          {
            cout << "Buku tidak sedang dipinjam." << endl;
          }
          break;
        }
        case 0:
          cout << "Terima kasih telah menggunakan sistem perpustakaan." << endl;
          break;
        default:
          cout << "Pilihan Tidak Tersedia" << endl;
          break;
        }
      } while (pilihMenuUser != 0);
      break;
    }
    case 0:
      cout << "Terima kasih telah menggunakan sistem perpustakaan." << endl;
      break;
    default:
      cout << "Pilihan Tidak Tersedia" << endl;
      break;
    }
  } while (pilihMenuUtama != 0);
  return 0;
}
