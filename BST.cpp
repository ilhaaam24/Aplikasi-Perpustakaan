Searching
Sorting





#include <iostream>
#include <unordered_map>
using namespace std;

struct Buku
{
  string judul;
  string penulis;
  string penerbit;
  int tahun;
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

string toLowerCase(string &str){
  for (char &c : str)
  {
    c = tolower(c);
  }
  return str;
}

Buku *createNode(const string &judul, const string &penulis, const string &penerbit, int tahun)
{
  Buku *newNode = new Buku();
  newNode->judul = judul;
  newNode->penulis = penulis;
  newNode->penerbit = penerbit;
  newNode->tahun = tahun;
  newNode->left = nullptr;
  newNode->right = nullptr;
  statusPeminjaman[judul] = false; // Semua buku awalnya tersedia
  return newNode;
}

Buku *tambahBuku(Buku *root, const string &judul, const string &penulis, const string &penerbit, int tahun)
{
  if (root == nullptr)
  {
    return createNode(judul, penulis, penerbit, tahun);
  }
  if (judul < root->judul)
  {
    root->left = tambahBuku(root->left, judul, penulis, penerbit, tahun);
  }
  else if (judul > root->judul)
  {
    root->right = tambahBuku(root->right, judul, penulis, penerbit, tahun);
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
    cout << "Buku \"" << judul << "\" berhasil dipinjam." << endl;
  }
  else
  {
    cout << "Buku \"" << judul << "\" tidak tersedia atau sudah dipinjam." << endl;
  }
}

int main()
{
  Buku *root = nullptr;

  root = tambahBuku(root, "Java", "James Gosling", "Informatika", 1995);
  root = tambahBuku(root, "Python", "Guido van Rossum", "Informatika", 1991);
  root = tambahBuku(root, "C++", "Bjarne Stroustrup", "Informatika", 1979);

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
        cout << "       MASUK SEBAGAI ADMIN      " << endl;
        cout << "================================" << endl;
        cout << "1. Tambah Buku" << endl;
        cout << "2. Edit Buku" << endl;
        cout << "3. Hapus Buku" << endl;
        cout << "4. Tampilkan Buku" << endl;
        cout << "5. Cari Buku" << endl;
        cout << "6. Tampilkan Peminjam" << endl;
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuAdmin;
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          string judul, penulis, penerbit;
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
          cin.ignore(); // Mengabaikan newline character

          root = tambahBuku(root, judul, penulis, penerbit, tahun);
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
          string judul; // Pindahkan deklarasi judul ke sini
          cout << "Masukkan judul buku yang ingin dicari: ";
          cin.ignore();
          getline(cin, judul);
          Buku *buku = cariBuku(root, judul);
          tampilkanDetailBuku(buku);
          break;
        }
        case 6 :

          tampilkanPeminjam();
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
          // Implementasi Kembalikan Buku di sini
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