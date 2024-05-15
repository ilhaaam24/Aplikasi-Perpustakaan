#include <iostream>
#include <string>
using namespace std;

struct RAK_BUKU{
  struct Sains
  {
    string namaRak = "Rak Buku Sains";
    const int SAINS_SIZE = 10;
  };
  struct Teknologi{
    string namaRak = "Rak Buku Teknologi";
    const int TEKNOLOGI_SIZE = 10;
  }
  struct Seni{
    string namaRak = "Rak Buku Seni";
    const int SENI_SIZE = 10;
  }
  struct Novel{
    string namaRak = "Rak Buku Novel";
    const int NOVEL_SIZE = 10;
  }
}
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

void tambahBuku(const string &judul, const string &penulis, int tahun)
{
  newNode = new Buku();
  newNode->judul = judul;
  newNode->penulis = penulis;
  newNode->tahun = tahun;
  newNode->next = nullptr;
  newNode->prev = nullptr;

  if (head == nullptr)
  {
    head = newNode;
    tail = newNode;
  }
  else
  {
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
  }
}


void cetakDenganLebar(const string &str, int lebar)
{
  cout << str;
  for (int i = str.length(); i < lebar; ++i)
  {
    cout << ' ';
  }
}

void cetakSemuaBuku()
{
  Buku *current = head;
  cout << "=========================================================" << endl;
  cout << "                       DAFTAR BUKU                        " << endl;
  cout << "=========================================================" << endl;
  cout << "|      JUDUL      |      PENULIS      |      TAHUN      |" << endl;
  cout << "=========================================================" << endl;

  while (current != nullptr)
  {
    cout << "| ";
    cetakDenganLebar(current->judul, 15);
    cout << " | ";
    cetakDenganLebar(current->penulis, 17);
    cout << " | ";
    cout << current->tahun;
    int tahunPanjang = to_string(current->tahun).length();
    for (int i = tahunPanjang; i < 15; ++i)
    {
      cout << ' ';
    }
    cout << " |" << endl;
    current = current->next;
  }

  cout << "=========================================================\n\n" << endl;
}

int main()
{
  Buku buku;
  int pilihMenuUtama;
  do
  {
    cout << "================================" << endl;
    cout << " SELAMAT DATANG DI PERPUSTAKAAN " << endl;
    cout << "================================" << endl;
    cout << "1. Masuk Sebagai Admin" << endl;
    cout << "2. Masuk Sebagai User" << endl;
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
        cout << "0. Keluar" << endl;
        cout << ">";
        cin >> pilihMenuAdmin;
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          cout << "Masukan Judul Buku : ";
          cin.ignore();
          getline(cin, buku.judul);
          cout << "Masukan Penulis Buku : ";
          getline(cin, buku.penulis);
          cout << "Masukan Tahun Buku : ";
          cin >> buku.tahun;
          tambahBuku(buku.judul, buku.penulis, buku.tahun);
          break;
        }
        case 2:
          // Implementasi Edit Buku di sini
          cout << RAK_BUKU << endl; 
          break;
        case 3:
          // Implementasi Hapus Buku di sini
          break;
        case 4:
          cetakSemuaBuku();
          break;
        case 5:
          // Implementasi Cari Buku di sini
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
          cetakSemuaBuku();
          break;
        case 2:
          // Implementasi Cari Buku di sini
          break;
        default:
          cout << "Pilihan Tidak Tersedia" << endl;
          break;
        }
      } while (pilihMenuUser != 0);
      break;
    }
    default:
      cout << "Pilihan Tidak Tersedia" << endl;
      break;
    }
  } while (pilihMenuUtama != 0);
  return 0;
}
