// +
//   fitur tambah buku/input Buku
//   fitur edit buku
//   fitur delete buku
//   fungsi mengurutkan buku berdasarkan judul
//   fungsi cari buku 
      // membuat  tree metadata buku
      // membuat graph tempat buku yang berhubungan
//   membuat hashing ketika meminjam buku
// +
#include <iostream>
using namespace std;

struct Buku
{
  int ID;
  string judul;
  string penulis;
  int tahun;
  Buku *next;
  Buku *prev;
};

Buku *head, *tail, *newNode, *del;

void tambahBuku(string judul, string penulis, int tahun){

  Buku buku;

}

struct Tree{
  Buku data;
  Tree *next;
  Tree *prev;
};






int main(){
  int pilihMenu;
  Buku buku;
  Tree *root = NULL;
  do
  {

    cout << "================================" << endl;
    cout << " SELAMAT DATANG DI PERPUSTAKAAN " << endl;
    cout << "================================" << endl;
    cout << "1.Input Data Buku" << endl;
    cout << "2.Lihat Data Buku" << endl;
    cout << "3.Edit Data Buku" << endl;
    cout << "4.Peminjaman Buku" << endl;
    cout << "5.Pengembalian Buku" << endl;
    cout << "6. Cari Buku" << endl;
    cout << ">";
    cin >> pilihMenu;
    switch (pilihMenu)
    {
    case 1:
      
      break;
    case 2:

      break;

    case 3:

      break;

    case 4:

      break;

    case 5:

      break;

    case 6:

      break;

    default:
    cout << "Pilihan Tidak Ada" << endl;
      break;
    }
  } while (pilihMenu != 0);
  
}