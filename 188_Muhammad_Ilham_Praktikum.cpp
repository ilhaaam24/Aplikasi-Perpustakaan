
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

struct Buku
{
  string judul;
  string penulis;
  string penerbit;
  int tahun;
  string kategori; 
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
unordered_map<string, vector<pair<string, int>>> graph;
const int INT_MAX = 100;

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
  newNode->kategori = kategori; 
  newNode->left = nullptr;
  newNode->right = nullptr;
  statusPeminjaman[judul] = false; // Semua buku awalnya tersedia
  return newNode;
}
Buku *cariPengganti(Buku *node)
{
  Buku *current = node;
  while (current && current->left != nullptr)
  {
    current = current->left;
  }
  return current;
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

void bacaFileBuku(Buku *&root, Rak &rak, const string &filename)
{
  ifstream file(filename);
  if (!file)
  {
    cerr << "Tidak bisa membuka file: " << filename << endl;
    return;
  }

  string line;
  while (getline(file, line))
  {
    stringstream ss(line);
    string judul, penulis, penerbit, tahunStr, kategori;
    getline(ss, judul, '|');
    getline(ss, penulis, '|');
    getline(ss, penerbit, '|');
    getline(ss, tahunStr, '|');
    getline(ss, kategori, '|');

    int tahun = stoi(tahunStr);
    root = tambahBuku(root, judul, penulis, penerbit, tahun, kategori, rak);
  }
  file.close();
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

Peminjam *searchPeminjam(const string &judul)
{
  for (int i = 0; i < TABLE_SIZE; ++i)
  {
    if (hashTable[i] != nullptr && hashTable[i]->judulBukuDipinjam == judul)
    {
      return hashTable[i];
    }
  }
  return nullptr;
}

void hapusPeminjam(const string &nama)
{
  int hashIndex = hashFunction(nama);
  int indexAwal = hashIndex;
  while (hashTable[hashIndex] != nullptr)
  {
    if (hashTable[hashIndex]->nama == nama)
    {
      delete hashTable[hashIndex];
      hashTable[hashIndex] = nullptr;
      return;
    }
    hashIndex = (hashIndex + 1) % TABLE_SIZE;
    if (hashIndex == indexAwal)
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
void cetakPeminjam(Peminjam *peminjam, int nomor)
{
  cout << "| ";
  cetakDenganLebar(to_string(nomor), 2);
  cout << " | ";
  if (peminjam != nullptr)
  {
    cetakDenganLebar(peminjam->nama, 20);
    cout << " | ";
    cetakDenganLebar(peminjam->noHandphone, 16);
    cout << " | ";
    cetakDenganLebar(peminjam->alamat, 25);
    cout << " | ";
    cetakDenganLebar(peminjam->judulBukuDipinjam, 22);
  }
  else
  {
    cetakDenganLebar("", 20);
    cout << " | ";
    cetakDenganLebar("", 16);
    cout << " | ";
    cetakDenganLebar("", 25);
    cout << " | ";
    cetakDenganLebar("", 22);
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
    cout << "Buku " << judul << " berhasil dipinjam >_<" << endl;
  }
  else
  {
    cout << "Buku " << judul << " tidak tersedia atau sudah dipinjam " << endl;
  }
}

void kembalikanBuku(const string &judul)
{
  if (statusPeminjaman[judul])
  {
    statusPeminjaman[judul] = false;

    // Cari peminjam berdasarkan judul buku yang dipinjam
    Peminjam *peminjam = searchPeminjam(judul);

    if (peminjam != nullptr)
    {
      hapusPeminjam(peminjam->nama); // Hapus peminjam dari daftar peminjam
      cout << "Buku " << judul << " berhasil dikembalikan >_<" << endl;
    }
    else
    {
      cout << "Buku " << judul << " tidak dalam daftar buku yang sedang dipinjam." << endl;
    }
  }
  else
  {
    cout << "Buku " << judul << " tidak sedang dipinjam." << endl;
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
  switch (pilihan){
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
  }if (kategoriDipilih){
    cetakHeaderTabel();
    int nomor = 1;
    for (Buku *buku : *kategoriDipilih){
      cetakBuku(buku, nomor++);
    }
    cout << "--------------------------------------------------------------------------------" << endl;
  }
}

Buku *hapusNode(Buku *root, const string &judul)
{
  if (root == nullptr)
  {
    return nullptr;
  }

  if (judul < root->judul)
  {
    root->left = hapusNode(root->left, judul);
  }
  else if (judul > root->judul)
  {
    root->right = hapusNode(root->right, judul);
  }
  else
  {
    // Kasus ketika ditemukan buku yang akan dihapus
    if (root->left == nullptr)
    {
      Buku *temp = root->right;
      delete root;
      return temp;
    }
    else if (root->right == nullptr)
    {
      Buku *temp = root->left;
      delete root;
      return temp;
    }

    // Jika buku memiliki dua anak, cari pengganti dari sub-pohon kanan
    Buku *temp = cariPengganti(root->right);

    // Salin nilai pengganti ke node yang akan dihapus
    root->judul = temp->judul;

    // Hapus pengganti dari sub-pohon kanan
    root->right = hapusNode(root->right, temp->judul);
  }
  return root;
}
void hapusDariRak(vector<Buku *> &kategori, const string &judul)
{
  for (auto it = kategori.begin(); it != kategori.end(); ++it)
  {
    if ((*it)->judul == judul)
    {
      delete *it;
      kategori.erase(it);
      break;
    }
  }
}
void hapusBuku(Buku *&root, const string &judul, Rak &rak)
{
  Buku *buku = cariBuku(root, judul);
  if (buku != nullptr)
  {
    root = hapusNode(root, judul);

    if (buku->kategori == "sains")
    {
      hapusDariRak(rak.sains, judul);
    }
    else if (buku->kategori == "teknologi")
    {
      hapusDariRak(rak.teknologi, judul);
    }
    else if (buku->kategori == "sejarah")
    {
      hapusDariRak(rak.sejarah, judul);
    }
    cout << "Buku dengan judul \"" << judul << "\" berhasil dihapus." << endl;
  }
  else
  {
    cout << "Buku dengan judul \"" << judul << "\" tidak ditemukan." << endl;
  }
}
void denahPerpustakaan()
{
  cout << "================================================================================" << endl;
  cout << "                            DENAH PERPUSTAKAAN                                  " << endl;
  cout << "================================================================================" << endl;
  cout << "|                               PINTU MASUK                                    |" << endl;
  cout << "|                                    |                                         |" << endl;
  cout << "|                                    |                                         |" << endl;
  cout << "|               SAINS----------TEKNOLOGI-------SEJARAH-----FILSAFAT            |" << endl;
  cout << "|                 |                  |                        /                |" << endl;
  cout << "|                 |                  |                       /                 |" << endl;
  cout << "|                 |                  |                      /                  |" << endl;
  cout << "|                 |                  |                     /                   |" << endl;
  cout << "|              BAHASA-------------SOSIAL-------------AGAMA                     |" << endl;
  cout << "================================================================================" << endl;
}
void tambahEdge(const string &rak1, const string &rak2, int bobot)
{
  graph[rak1].push_back(make_pair(rak2, bobot));
  graph[rak2].push_back(make_pair(rak1, bobot));
}

void inisialisasiGraph()
{

  tambahEdge("Pintu_Masuk", "Teknologi", 1);
  tambahEdge("Teknologi", "Sains", 1);
  tambahEdge("Teknologi", "Sejarah", 1);
  tambahEdge("Teknologi", "Sosial", 2);
  tambahEdge("Sejarah", "Filsafat", 1);
  tambahEdge("Sains", "Bahasa", 2);
  tambahEdge("Bahasa", "Sosial", 2);
  tambahEdge("Sosial", "Agama", 2);
  tambahEdge("Agama", "Filsafat", 3);
}

void reverseVector(vector<string> &vec)
{
  int n = vec.size();
  for (int i = 0; i < n / 2; ++i)
  {
    string temp = vec[i];
    vec[i] = vec[n - i - 1];
    vec[n - i - 1] = temp;
  }
}
vector<string> dijkstra(const string &awal, const string &tujuan)
{
  unordered_map<string, int> jarak;
  unordered_map<string, string> sebelumnya;
  priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

  // Inisialisasi jarak awal ke semua simpul menjadi tak terhingga
  for (const auto &simpul : graph)
  {
    jarak[simpul.first] = INT_MAX;
  }

  jarak[awal] = 0;
  pq.push(make_pair(0, awal));

  while (!pq.empty())
  {
    string sekarang = pq.top().second;
    int jarakSekarang = pq.top().first;
    pq.pop();

    if (sekarang == tujuan)
      break;

    for (const auto &neighbor : graph[sekarang])
    {
      string berikutnya = neighbor.first;
      int berat = neighbor.second;
      int jarakBaru = jarak[sekarang] + berat;

      // Perbarui jarak jika ditemukan jarak yang lebih pendek
      if (jarakBaru < jarak[berikutnya])
      {
        jarak[berikutnya] = jarakBaru;
        sebelumnya[berikutnya] = sekarang;
        pq.push(make_pair(jarakBaru, berikutnya));
      }
    }
  }

    // Rekonstruksi rute terpendek dari simpul tujuan ke simpul awal
  vector<string> path;
  string at = tujuan;
  while (at != "")
  {
    path.push_back(at);
    at = sebelumnya[at];
  }
  reverseVector(path);

  return path;
}

int main()
{
  Buku *root = nullptr;
  Rak rak;

  bacaFileBuku(root, rak, "Database_Buku.txt");

  int pilihMenuUtama;
  do
  {
    cout << "\n======================================" << endl;
    cout << "    SELAMAT DATANG DI PERPUSTAKAAN    " << endl;
    cout << "             CAHAYA ILMU              " << endl;
    cout << "======================================" << endl;
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
        cout << "\n================================" << endl;
        cout << "           MENU ADMIN           " << endl;
        cout << "================================" << endl;
        cout << "1. Input Buku" << endl;
        cout << "2. Hapus Buku" << endl;
        cout << "3. Tampilkan Buku Berdasarkan Kategori" << endl;
        cout << "4. Cari Buku" << endl;
        cout << "5. Tampilkan Peminjam" << endl;
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
        {
          string judul;
          cout << "Masukkan judul buku yang ingin dihapus: ";
          cin.ignore();
          getline(cin, judul);
          hapusBuku(root, judul, rak);
          break;
        }
        case 3:
          pilihKategori(rak);
          break;
        case 4:
        {
          string judul;
          cout << "Masukkan judul buku yang ingin dicari: ";
          cin.ignore();
          getline(cin, judul);
          Buku *buku = cariBuku(root, judul);
          tampilkanDetailBuku(buku);
          break;
        }
        case 5:
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
        cout << "\n================================" << endl;
        cout << "           MENU USER            " << endl;
        cout << "================================" << endl;
        cout << "1. Tampilkan Kategory Buku" << endl;
        cout << "2. Cari Buku" << endl;
        cout << "3. Pinjam Buku" << endl;
        cout << "4. Kembalikan Buku" << endl;
        cout << "5. Cari Lokasi Rak Buku" << endl;
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuUser;
        switch (pilihMenuUser)
        {
        case 1:
          pilihKategori(rak);
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
        case 5 :
        {
          string start, dest;
          start = "Pintu_Masuk";
          cout << "Lokasi buku yang ingin dicari: ";
          cin.ignore();
          getline(cin, dest);
          inisialisasiGraph();
          vector<string> rute = dijkstra(start, dest);
          denahPerpustakaan();
          cout << "Rute terpendek dari " << start << " ke " << dest << ": ";
          for (const auto &rak : rute)
          {
            cout << " -> " << rak ;
          }
          cout << endl;
          break;
        }
        default:
          cout << "Pilihan Tidak Tersedia" << endl;
          break;
        }
      } while (pilihMenuUser != 0);
    }
    }
  } while (pilihMenuUtama != 0);
}