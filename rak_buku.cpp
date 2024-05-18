#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Buku
{
  string judul;
  string penulis;
  string penerbit;
  int tahun;
  string kategori; // Tambahkan kategori
  Buku *left;
  Buku *right;
};

struct Peminjam
{
  string nama;
  string noHandphone;
  string alamat;
  string judulBukuDipinjam;
  Peminjam *next;
};

const int TABLE_SIZE = 10;
Peminjam *hashTable[TABLE_SIZE];
unordered_map<string, bool> statusPeminjaman;

struct Rak
{
  vector<Buku *> sains;
  vector<Buku *> teknologi;
  vector<Buku *> sejarah;
  vector<Buku *> filsafat;
  vector<Buku *> agama;
  vector<Buku *> sosial;
  vector<Buku *> bahasa;
};

int hashFunction(const string &key)
{
  int hash = 0;
  for (char c : key)
  {
    hash = (hash * 31 + c) % TABLE_SIZE;
  }
  return hash;
}

void initializeHashTable()
{
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    hashTable[i] = nullptr;
  }
}

string toLowerCase(string &str)
{
  for (char &c : str)
  {
    c = tolower(c);
  }
  return str;
}

Buku *createNode(const string &judul, const string &penulis, const string &penerbit, int tahun, const string &kategori)
{
  Buku *newNode = new Buku();
  newNode->judul = judul;
  newNode->penulis = penulis;
  newNode->penerbit = penerbit;
  newNode->tahun = tahun;
  newNode->kategori = kategori; // Set kategori
  newNode->left = nullptr;
  newNode->right = nullptr;
  statusPeminjaman[judul] = false; // Semua buku awalnya tersedia
  return newNode;
}

Buku *tambahBuku(Buku *root, const string &judul, const string &penulis, const string &penerbit, int tahun, const string &kategori, Rak &rak)
{
  if (root == nullptr)
  {
    Buku *newNode = createNode(judul, penulis, penerbit, tahun, kategori);
    if (kategori == "sains")
      rak.sains.push_back(newNode);
    else if (kategori == "teknologi")
      rak.teknologi.push_back(newNode);
    else if (kategori == "sejarah")
      rak.sejarah.push_back(newNode);
    else if (kategori == "filsafat")
      rak.filsafat.push_back(newNode);
    else if (kategori == "agama")
      rak.agama.push_back(newNode);
    else if (kategori == "sosial")
      rak.sosial.push_back(newNode);
    else if (kategori == "bahasa")
      rak.bahasa.push_back(newNode);
    return newNode;
  }
  if (judul < root->judul)
  {
    root->left = tambahBuku(root->left, judul, penulis, penerbit, tahun, kategori, rak);
  }
  else if (judul > root->judul)
  {
    root->right = tambahBuku(root->right, judul, penulis, penerbit, tahun, kategori, rak);
  }
  else
  {
    cout << "Buku dengan judul \"" << judul << "\" sudah ada di tree." << endl;
  }
  return root;
}

void cetakHeaderTabel()
{
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "| No |      JUDUL      |      PENULIS      |      TAHUN      |     STATUS      |" << endl;
  cout << "--------------------------------------------------------------------------------" << endl;
}

void cetakDenganLebar(const string &str, int lebar)
{
  cout << str;
  for (int i = str.length(); i < lebar; ++i)
  {
    cout << ' ';
  }
}

void cetakBuku(Buku *buku, int nomor)
{
  cout << "| ";
  cetakDenganLebar(to_string(nomor), 2);
  cout << " | ";
  if (buku != nullptr)
  {
    cetakDenganLebar(buku->judul, 15);
    cout << " | ";
    cetakDenganLebar(buku->penulis, 17);
    cout << " | ";
    cetakDenganLebar(to_string(buku->tahun), 15);
    cout << " | ";
    string status = statusPeminjaman[buku->judul] ? "Dipinjam" : "Tersedia";
    cetakDenganLebar(status, 15);
  }
  else
  {
    cetakDenganLebar("", 15);
    cout << " | ";
    cetakDenganLebar("", 17);
    cout << " | ";
    cetakDenganLebar("", 15);
    cout << " | ";
    cetakDenganLebar("", 15);
  }
  cout << " |" << endl;
}

void inorderTraversal(Buku *root, Buku *arr[], int &index, int maxBuku)
{
  if (root != nullptr && index < maxBuku)
  {
    inorderTraversal(root->left, arr, index, maxBuku);
    if (index < maxBuku)
    {
      arr[index++] = root;
    }
    inorderTraversal(root->right, arr, index, maxBuku);
  }
}

void tampilkanBuku(Buku *root, int maxBuku)
{
  cetakHeaderTabel();
  Buku *arr[maxBuku] = {nullptr};
  int index = 0;
  inorderTraversal(root, arr, index, maxBuku);

  for (int i = 0; i < maxBuku; ++i)
  {
    cetakBuku(arr[i], i + 1);
  }

  cout << "--------------------------------------------------------------------------------" << endl;
}

Buku *cariBuku(Buku *root, const string &judul)
{
  if (root == nullptr || root->judul == judul)
  {
    return root;
  }

  if (judul < root->judul)
  {
    return cariBuku(root->left, judul);
  }

  return cariBuku(root->right, judul);
}

void tampilkanDetailBuku(Buku *buku)
{
  if (buku != nullptr)
  {
    cetakHeaderTabel();
    cetakBuku(buku, 1);
    cout << "--------------------------------------------------------------------------------" << endl;
  }
  else
  {
    cout << "Buku tidak ditemukan." << endl;
  }
}

void insertPeminjam(const string &nama, const string &noHandphone, const string &alamat, const string &judulBukuDipinjam)
{
  Peminjam *newPeminjam = new Peminjam();
  newPeminjam->nama = nama;
  newPeminjam->noHandphone = noHandphone;
  newPeminjam->alamat = alamat;
  newPeminjam->judulBukuDipinjam = judulBukuDipinjam;

  int hashIndex = hashFunction(nama);
  while (hashTable[hashIndex] != nullptr)
  {
    hashIndex = (hashIndex + 1) % TABLE_SIZE;
  }
  hashTable[hashIndex] = newPeminjam;
}

Peminjam *searchPeminjam(const string &nama)
{
  int hashIndex = hashFunction(nama);
  int originalIndex = hashIndex;
  while (hashTable[hashIndex] != nullptr)
  {
    if (hashTable[hashIndex]->nama == nama)
    {
      return hashTable[hashIndex];
    }
    hashIndex = (hashIndex + 1) % TABLE_SIZE;
    if (hashIndex == originalIndex)
    {
      break;
    }
  }
  return nullptr;
}

void hapusPeminjam(const string &nama)
{
  int hashIndex = hashFunction(nama);
  int originalIndex = hashIndex;
  while (hashTable[hashIndex] != nullptr)
  {
    if (hashTable[hashIndex]->nama == nama)
    {
      delete hashTable[hashIndex];
      hashTable[hashIndex] = nullptr;
      return;
    }
    hashIndex = (hashIndex + 1) % TABLE_SIZE;
    if (hashIndex == originalIndex)
    {
      break;
    }
  }
}

void cetakHeaderTabelPeminjam()
{
  cout << "------------------------------------------------------------------------------------------------------------" << endl;
  cout << "| No |         NAMA         |    NO HANDPHONE   |          ALAMAT          |    JUDUL BUKU DIPINJAM   |" << endl;
  cout << "------------------------------------------------------------------------------------------------------------" << endl;
}

void cetakDenganLebarPeminjam(const string &str, int lebar)
{
  cout << str;
  for (int i = str.length(); i < lebar; ++i)
  {
    cout << ' ';
  }
}

void cetakPeminjam(Peminjam *peminjam, int nomor)
{
  cout << "| ";
  cetakDenganLebarPeminjam(to_string(nomor), 2);
  cout << " | ";
  if (peminjam != nullptr)
  {
    cetakDenganLebarPeminjam(peminjam->nama, 20);
    cout << " | ";
    cetakDenganLebarPeminjam(peminjam->noHandphone, 16);
    cout << " | ";
    cetakDenganLebarPeminjam(peminjam->alamat, 25);
    cout << " | ";
    cetakDenganLebarPeminjam(peminjam->judulBukuDipinjam, 22);
  }
  else
  {
    cetakDenganLebarPeminjam("", 20);
    cout << " | ";
    cetakDenganLebarPeminjam("", 16);
    cout << " | ";
    cetakDenganLebarPeminjam("", 25);
    cout << " | ";
    cetakDenganLebarPeminjam("", 22);
  }
  cout << " |" << endl;
}

void tampilkanPeminjam()
{
  cetakHeaderTabelPeminjam();
  int nomor = 1;
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    if (hashTable[i] != nullptr)
    {
      cetakPeminjam(hashTable[i], nomor++);
    }
  }
  cout << "------------------------------------------------------------------------------------------------------------" << endl;
}

void pinjamBuku(Buku *root, const string &judul, const string &nama, const string &noHandphone, const string &alamat)
{
  Buku *buku = cariBuku(root, judul);
  if (buku != nullptr && !statusPeminjaman[judul])
  {
    statusPeminjaman[judul] = true;
    insertPeminjam(nama, noHandphone, alamat, judul);
    cout << "Buku " << judul << " berhasil dipinjam." << endl;
  }
  else
  {
    cout << "Buku "<< judul <<  " tidak tersedia atau sudah dipinjam." << endl;
  }
}

void kembalikanBuku(const string &judul)
{
  if (statusPeminjaman[judul])
  {
    statusPeminjaman[judul] = false; // Ubah status peminjaman menjadi tersedia
    Peminjam *peminjam = searchPeminjam(judul);
    if (peminjam != nullptr)
    {
      hapusPeminjam(peminjam->nama); // Hapus data peminjam dari hashTable
    }
    cout << "Buku " << judul << " berhasil dikembalikan."<< endl;
  }
  else
  {
    cout << "Buku " << judul <<"tidak dalam daftar buku yang sedang dipinjam."<< endl;
  }
}

void tampilkanKategori(Rak &rak)
{
  cout << "Kategori yang tersedia: " << endl;
  cout << "1. Sains" << endl;
  cout << "2. Teknologi" << endl;
  cout << "3. Sejarah" << endl;
  cout << "4. Filsafat" << endl;
  cout << "5. Agama" << endl;
  cout << "6. Sosial" << endl;
  cout << "7. Bahasa" << endl;
}

void pilihKategori(Rak &rak)
{
  int pilihan;
  tampilkanKategori(rak);
  cout << "Pilih kategori (masukkan nomor): ";
  cin >> pilihan;

  vector<Buku *> *kategoriDipilih = nullptr;
  switch (pilihan)
  {
  case 1:
    kategoriDipilih = &rak.sains;
    break;
  case 2:
    kategoriDipilih = &rak.teknologi;
    break;
  case 3:
    kategoriDipilih = &rak.sejarah;
    break;
  case 4:
    kategoriDipilih = &rak.filsafat;
    break;
  case 5:
    kategoriDipilih = &rak.agama;
    break;
  case 6:
    kategoriDipilih = &rak.sosial;
    break;
  case 7:
    kategoriDipilih = &rak.bahasa;
    break;
  default:
    cout << "Pilihan kategori tidak valid." << endl;
    return;
  }

  if (kategoriDipilih)
  {
    cetakHeaderTabel();
    int nomor = 1;
    for (Buku *buku : *kategoriDipilih)
    {
      cetakBuku(buku, nomor++);
    }
    cout << "--------------------------------------------------------------------------------" << endl;
  }
}

int main()
{
  Buku *root = nullptr;
  Rak rak;

  root = tambahBuku(root, "Java", "James Gosling", "Informatika", 1995, "teknologi", rak);
  root = tambahBuku(root, "Python", "Guido van Rossum", "Informatika", 1991, "teknologi", rak);
  root = tambahBuku(root, "C++", "Bjarne Stroustrup", "Informatika", 1979, "teknologi", rak);
  root = tambahBuku(root, "C#", "Anders Hejlsberg", "Informatika", 2000, "teknologi", rak);

  int pilihMenuUtama;
  do
  {
    cout << "================================" << endl;
    cout << " SELAMAT DATANG DI PERPUSTAKAAN " << endl;
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
        cout << " MASUK SEBAGAI ADMIN " << endl;
        cout << "================================" << endl;
        cout << "1. Tambah Buku" << endl;
        cout << "2. Edit Buku" << endl;
        cout << "3. Hapus Buku" << endl;
        cout << "4. Tampilkan Buku" << endl;
        cout << "5. Cari Buku" << endl;
        cout << "6. Tampilkan Peminjam" << endl;
        cout << "7. Tampilkan dan Pilih Kategori" << endl;
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuAdmin;
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          string judul, penulis, penerbit, kategori;
          int tahun;
          cout << "Masukkan Judul: ";
          cin.ignore();
          getline(cin, judul);
          cout << "Masukkan Penulis: ";
          getline(cin, penulis);
          cout << "Masukkan Penerbit: ";
          getline(cin, penerbit);
          cout << "Masukkan Tahun: ";
          cin >> tahun;
          cin.ignore(); 
          cout << "Masukkan Kategori (sains, teknologi, sejarah, filsafat, agama, sosial, bahasa): ";
          getline(cin, kategori);

          root = tambahBuku(root, judul, penulis, penerbit, tahun, kategori, rak);
          break;
        }
        case 2:
          // Implementasi Edit Buku di sini
          break;
        case 3:
          // Implementasi Hapus Buku di sini
          break;
        case 4:
          tampilkanBuku(root, 10);
          break;
        case 5:
        {
          string judul;
          cout << "Masukkan judul buku yang ingin dicari: ";
          cin.ignore();
          getline(cin, judul);
          Buku *buku = cariBuku(root, judul);
          tampilkanDetailBuku(buku);
          break;
        }
        case 6:
          tampilkanPeminjam();
          break;
        case 7:
          pilihKategori(rak);
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
        cout << "       MASUK SEBAGAI USER       " << endl;
        cout << "================================" << endl;
        cout << "1. Tampilkan Buku" << endl;
        cout << "2. Cari Buku" << endl;
        cout << "3. Pinjam Buku" << endl;
        cout << "4. Kembalikan Buku" << endl;
        cout << "5. Tampilkan dan Pilih Kategori" << endl; // Menu baru
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuUser;
        switch (pilihMenuUser)
        {
        case 1:
          tampilkanBuku(root, 10);
          break;
        case 2:
        {
          string judul;
          cout << "Masukkan judul buku yang ingin dicari: ";
          cin.ignore();
          getline(cin, judul);
          Buku *buku = cariBuku(root, judul);
          tampilkanDetailBuku(buku);
          break;
        }
        case 3:
        {
          string nama, noHandphone, alamat, judul;
          cout << "Masukkan nama Anda: ";
          cin.ignore();
          getline(cin, nama);
          cout << "Masukkan nomor handphone: ";
          getline(cin, noHandphone);
          cout << "Masukkan alamat: ";
          getline(cin, alamat);
          cout << "Masukkan judul buku yang ingin dipinjam: ";
          getline(cin, judul);
          pinjamBuku(root, judul, nama, noHandphone, alamat);
          break;
        }
        case 4:
        {
          string judul;
          cout << "Masukkan judul buku yang ingin dikembalikan: ";
          cin.ignore();
          getline(cin, judul);
          kembalikanBuku(judul);
          break;
        }
        case 5:
          pilihKategori(rak);
          break;
        default:
          cout << "Pilihan Tidak Tersedia" << endl;
          break;
        }
      } while (pilihMenuUser != 0);
    }
    }
  } while (pilihMenuUtama != 0);
}