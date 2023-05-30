#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <chrono>
#include <cstdlib>

using namespace std;

struct User {
    string username;
    string password;
};

bool isUsernameExists(const string& username) {
    ifstream file("user_database.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername;

        getline(ss, storedUsername, ',');

        if (storedUsername == username) {
            file.close();
            return true;  // Nama pengguna sudah ada dalam database
        }
    }

    file.close();
    return false;  // Nama pengguna tidak ada dalam database
}

void registerUser(const string& username, const string& password) {
    if (isUsernameExists(username)) {
        cout << "Nama pengguna sudah digunakan. Silakan pilih nama pengguna lain." << endl;
        return;
    }

    User newUser;
    newUser.username = username;
    newUser.password = password;

    ofstream file("user_database.csv", ios::app);

    if (file.is_open()) {
        file << newUser.username << "," << newUser.password << endl;
        file.close();
        cout << "Pendaftaran pengguna berhasil!" << endl;
    } else {
        cout << "Gagal membuka file database." << endl;
    }
}

bool loginUser(const string& username, const string& password) {
    ifstream file("user_database.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername, storedPassword;

        getline(ss, storedUsername, ',');
        getline(ss, storedPassword);

        if (storedUsername == username && storedPassword == password) {
            file.close();
            return true;  // Login berhasil
        }
    }

    file.close();
    return false;  // Login gagal
}

void displayShopMenu(const string& username) {
    cout << "=== Selamat Datang di Ayam Store, " << username << "! ===" << endl;
    cout << "Menu Pilihan Online Shop:" << endl;
    cout << "1. Beli Ayam" << endl;
    cout << "2. Lihat Keranjang Belanja" << endl;
    cout << "3. Kasir" << endl;
    cout << "4. Logout" << endl;
}

void kosongkanKeranjang(map<string, int>& keranjang) {
    keranjang.clear();
}

void logUser(const string& username, const string& action) {
    ofstream file("userlog.txt", ios::app);

    if (file.is_open()) {
        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        string timestamp = ctime(&now);
        timestamp.erase(timestamp.length() - 1);

        file << username << ", " << timestamp << ", " << action << endl;

        file.close();
    } else {
        cout << "Gagal membuka file log." << endl;
    }
}

void shopMenu(const string& username) {
    int choice;
    bool loggedIn = true;

    map<string, int> keranjang;
    map<string, int> hargaAyam = {
        {"Ayam Bakar", 15000},
        {"Ayam Goreng", 12000},
        {"Ayam Gulai", 15000},
        {"Ayam Geprek", 12500},
        {"Ayam Kecap", 13000},
        {"Ayam Tepung", 11000}
    };

    int saldoAwal = 0;
    int saldoAkhir = 0;

    do {
        displayShopMenu(username);
        cout << "Pilihan Anda: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "=== Menu Ayam ===" << endl;
                cout << "1. Ayam Bakar    - Harga: Rp15000" << endl;
                cout << "2. Ayam Goreng   - Harga: Rp12000" << endl;
                cout << "3. Ayam Gulai    - Harga: Rp15000" << endl;
                cout << "4. Ayam Geprek   - Harga: Rp12500" << endl;
                cout << "5. Ayam Kecap    - Harga: Rp13000" << endl;
                cout << "6. Ayam Tepung   - Harga: Rp11000" << endl;
                cout << "Pilihan Ayam (1-6): ";
                int ayamChoice;
                cin >> ayamChoice;

                if (ayamChoice >= 1 && ayamChoice <= 6) {
                    string ayamName;
                    int ayamPrice;

                    int count = 1;
                    for (const auto& ayam : hargaAyam) {
                        if (count == ayamChoice) {
                            ayamName = ayam.first;
                            ayamPrice = ayam.second;
                            break;
                        }
                        count++;
                    }

                    if (keranjang.find(ayamName) == keranjang.end()) {
                        keranjang[ayamName] = 1;
                    } else {
                        keranjang[ayamName]++;
                    }

                    cout << "Anda telah menambahkan " << ayamName << " ke dalam keranjang." << endl;

                    // Mencatat entri log
                    string action = "Menambahkan " + ayamName + " ke dalam keranjang";
                    logUser(username, action);
                } else {
                    cout << "Pilihan ayam tidak valid." << endl;
                }
                break;
            }
            case 2: {
                cout << "=== Keranjang Belanja ===" << endl;
                if (keranjang.empty()) {
                    cout << "Keranjang belanja kosong." << endl;
                } else {
                    int totalHarga = 0;
                    for (const auto& item : keranjang) {
                        string ayamName = item.first;
                        int ayamQty = item.second;
                        int ayamPrice = hargaAyam[ayamName];

                        cout << ayamName << " - Harga: Rp" << ayamPrice << " - Qty: " << ayamQty << endl;
                        totalHarga += ayamPrice * ayamQty;
                    }
                    cout << "Total Harga: Rp" << totalHarga << endl;
                }
                break;
            }
            case 3: {
                cout << "=== Kasir ===" << endl;
                if (keranjang.empty()) {
                    cout << "Keranjang belanja kosong." << endl;
                } else {
                    int totalHarga = 0;
                    for (const auto& item : keranjang) {
                        string ayamName = item.first;
                        int ayamQty = item.second;
                        int ayamPrice = hargaAyam[ayamName];

                        totalHarga += ayamPrice * ayamQty;
                    }

                    cout << "Total Harga: Rp" << totalHarga << endl;

                    int uangPembayaran;
                    cout << "Masukkan jumlah uang pembayaran: Rp";
                    cin >> uangPembayaran;

                    if (uangPembayaran < totalHarga) {
                        cout << "Jumlah uang yang dimasukkan kurang dari total harga." << endl;
                    } else {
                        saldoAwal += totalHarga;
                        saldoAkhir = uangPembayaran - totalHarga;

                        cout << "Kembalian: Rp" << saldoAkhir << endl;

                        // Mencatat entri log
                        string action = "Melakukan pembayaran dengan total harga Rp" + to_string(totalHarga);
                        logUser(username, action);

                        // Mengosongkan keranjang belanja
                        kosongkanKeranjang(keranjang);
                    }
                }
                break;
            }
            case 4: {
                // Mencatat entri log
                string action = "Logout";
                logUser(username, action);

                loggedIn = false;
                cout << "Anda telah berhasil logout." << endl;
                break;
            }
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }

        cout << endl;

    } while (loggedIn);
}

int main() {
    system("cls");
    string username, password;
    int loginChoice;

    cout << "=== Selamat Datang di Ayam Store ===" << endl;

    do {
        cout << "1. Login" << endl;
        cout << "2. Daftar" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihan Anda: ";
        cin >> loginChoice;

        switch (loginChoice) {
            case 1:
                cout << "Masukkan username: ";
                cin >> username;
                cout << "Masukkan password: ";
                cin >> password;

                if (loginUser(username, password)) {
                    cout << "Login berhasil!" << endl;

                    // Mencatat entri log
                    string action = "Login";
                    logUser(username, action);

                    shopMenu(username);
                } else {
                    cout << "Username atau password salah." << endl;
                }
                break;
            case 2:
                cout << "Masukkan username: ";
                cin >> username;
                cout << "Masukkan password: ";
                cin >> password;

                registerUser(username, password);
                break;
            case 3:
                cout << "Terima kasih telah menggunakan Ayam Store." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }

        cout << endl;

    } while (loginChoice != 3);

    return 0;
}
