#include <iostream>
using namespace std;

struct Energi {
    int kode;
    char *nama;
};

struct ListNode {
    Energi data;
    ListNode *next;
};

struct TreeNode {
    Energi data;
    TreeNode *left;
    TreeNode *right;
};

int panjang(const char *s) {
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}

char *salin(const char *s) {
    int n = panjang(s);
    char *c = new char[n + 1];
    for (int i = 0; i <= n; i++) c[i] = s[i];
    return c;
}

bool spasi(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

char *trim(char *s) {
    while (*s != '\0' && spasi(*s)) s++;
    int n = panjang(s);
    while (n > 0 && spasi(s[n - 1])) n--;
    s[n] = '\0';
    return s;
}

bool bacaBaris(const char *prompt, char *buf, int ukuran) {
    cout << prompt;
    cin.getline(buf, ukuran);
    if (cin.eof()) return false;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(100000, '\n');
        buf[0] = '\0';
    }
    return true;
}

bool bacaInt(const char *prompt, int &out) {
    char buf[64];
    if (!bacaBaris(prompt, buf, 64)) return false;
    char *s = trim(buf);
    int i = 0;
    bool negatif = false;
    if (s[0] == '-' || s[0] == '+') {
        negatif = (s[0] == '-');
        i = 1;
    }
    int n = panjang(s);
    if (i >= n || n - i > 9) return false;
    long long v = 0;
    for (; i < n; i++) {
        if (s[i] < '0' || s[i] > '9') return false;
        v = v * 10 + (s[i] - '0');
    }
    out = (int)(negatif ? -v : v);
    return true;
}

bool tanyaYT(const char *prompt) {
    char buf[64];
    while (true) {
        if (!bacaBaris(prompt, buf, 64)) return false;
        char *s = trim(buf);
        if ((s[0] == 'y' || s[0] == 'Y') && s[1] == '\0') return true;
        if ((s[0] == 'n' || s[0] == 'N') && s[1] == '\0') return false;
        cout << "Error: masukkan hanya 'y' atau 'n'!\n";
    }
}

struct Antrean {
    ListNode *head = nullptr;
    ListNode *tail = nullptr;

    bool kosong() const { return head == nullptr; }

    void enqueue(int kode, const char *nama) {
        ListNode *n = new ListNode{{kode, salin(nama)}, nullptr};
        if (tail == nullptr) head = tail = n;
        else {
            tail->next = n;
            tail = n;
        }
    }

    void dequeue(Energi &out) {
        ListNode *t = head;
        out = t->data;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete t;
    }

    ~Antrean() {
        while (head != nullptr) {
            ListNode *t = head;
            head = head->next;
            delete[] t->data.nama;
            delete t;
        }
    }
};

struct Riwayat {
    ListNode *top = nullptr;

    bool kosong() const { return top == nullptr; }

    void push(int kode, const char *nama) {
        top = new ListNode{{kode, salin(nama)}, top};
    }

    void pop(Energi &out) {
        ListNode *t = top;
        out = t->data;
        top = top->next;
        delete t;
    }

    ~Riwayat() {
        while (top != nullptr) {
            ListNode *t = top;
            top = top->next;
            delete[] t->data.nama;
            delete t;
        }
    }
};

TreeNode *insertBST(TreeNode *root, int kode, const char *nama) {
    if (root == nullptr) return new TreeNode{{kode, salin(nama)}, nullptr, nullptr};
    if (kode < root->data.kode) root->left = insertBST(root->left, kode, nama);
    else if (kode > root->data.kode) root->right = insertBST(root->right, kode, nama);
    return root;
}

TreeNode *cariBST(TreeNode *root, int kode) {
    while (root != nullptr && root->data.kode != kode)
        root = (kode < root->data.kode) ? root->left : root->right;
    return root;
}

TreeNode *minNode(TreeNode *n) {
    while (n->left != nullptr) n = n->left;
    return n;
}

TreeNode *hapusBST(TreeNode *root, int kode, bool bebasNama) {
    if (root == nullptr) return nullptr;

    if (kode < root->data.kode) {
        root->left = hapusBST(root->left, kode, bebasNama);
    } else if (kode > root->data.kode) {
        root->right = hapusBST(root->right, kode, bebasNama);
    } else {
        if (root->left == nullptr && root->right == nullptr) {
            if (bebasNama) delete[] root->data.nama;
            delete root;
            return nullptr;
        }
        if (root->left == nullptr) {
            TreeNode *r = root->right;
            if (bebasNama) delete[] root->data.nama;
            delete root;
            return r;
        }
        if (root->right == nullptr) {
            TreeNode *l = root->left;
            if (bebasNama) delete[] root->data.nama;
            delete root;
            return l;
        }
        TreeNode *succ = minNode(root->right);
        if (bebasNama) delete[] root->data.nama;
        root->data = succ->data;
        root->right = hapusBST(root->right, succ->data.kode, false);
    }
    return root;
}

void preOrder(TreeNode *n, int &no) {
    if (!n) return;
    cout << ++no << ". [" << n->data.kode << "] " << n->data.nama << "\n";
    preOrder(n->left, no);
    preOrder(n->right, no);
}

void inOrder(TreeNode *n, int &no) {
    if (!n) return;
    inOrder(n->left, no);
    cout << ++no << ". [" << n->data.kode << "] " << n->data.nama << "\n";
    inOrder(n->right, no);
}

void postOrder(TreeNode *n, int &no) {
    if (!n) return;
    postOrder(n->left, no);
    postOrder(n->right, no);
    cout << ++no << ". [" << n->data.kode << "] " << n->data.nama << "\n";
}

void bebaskanTree(TreeNode *n) {
    if (!n) return;
    bebaskanTree(n->left);
    bebaskanTree(n->right);
    delete[] n->data.nama;
    delete n;
}

bool kodeSudahAda(const Antrean &q, const Riwayat &r, TreeNode *organ, int kode) {
    for (ListNode *p = q.head; p; p = p->next)
        if (p->data.kode == kode) return true;
    for (ListNode *p = r.top; p; p = p->next)
        if (p->data.kode == kode) return true;
    return cariBST(organ, kode) != nullptr;
}

void tampilMenu() {
    cout << "\n=== Sistem Organ Inti Nirwana ===\n\n"
         << "1. Tambah Energi\n"
         << "2. Lihat Energi Dalam Antrean\n"
         << "3. Proses Energi\n"
         << "4. Lihat Energi Yang Sudah Terserap\n"
         << "5. Hapus Riwayat Energi Teratas\n"
         << "6. Cari Energi Di Organ\n"
         << "7. Lihat Isi Organ\n"
         << "8. Gunakan Energi\n"
         << "9. Keluar\n\n";
}

void tambahEnergi(Antrean &q, Riwayat &r, TreeNode *organ) {
    do {
        int kode;
        if (!bacaInt("Masukkan kode energi: ", kode)) {
            if (cin.eof()) return;
            cout << "Error: kode energi harus berupa angka!\n";
            continue;
        }
        if (kode <= 0) {
            cout << "Error: kode energi harus lebih dari 0!\n";
            continue;
        }
        if (kodeSudahAda(q, r, organ, kode)) {
            cout << "Error: kode energi " << kode << " sudah tercatat!\n";
            continue;
        }

        char buf[256];
        if (!bacaBaris("Masukkan nama energi: ", buf, 256)) return;
        char *nama = trim(buf);
        if (nama[0] == '\0') {
            cout << "Error: nama energi tidak boleh kosong dan maksimal 255 karakter!\n";
            continue;
        }

        q.enqueue(kode, nama);
        cout << "Berhasil memasukkan energi!\n\n";
    } while (tanyaYT("Apakah anda ingin menambahkan energi lagi? (y/n): "));
}

void lihatAntrean(const Antrean &q) {
    if (q.kosong()) {
        cout << "Error: tidak ada energi dalam antrean!\n";
        return;
    }
    cout << "=== Energi Dalam Antrean ===\n\n";
    int no = 0;
    for (ListNode *p = q.head; p; p = p->next)
        cout << ++no << ". [" << p->data.kode << "] " << p->data.nama << "\n";
}

void prosesEnergi(Antrean &q, Riwayat &r, TreeNode *&organ) {
    if (q.kosong()) {
        cout << "Error: tidak ada energi dalam antrean untuk diproses!\n";
        return;
    }
    Energi e;
    q.dequeue(e);
    r.push(e.kode, e.nama);
    organ = insertBST(organ, e.kode, e.nama);
    cout << "Berhasil memproses energi " << e.nama << "!\n";
    delete[] e.nama;
}

void lihatRiwayat(const Riwayat &r) {
    if (r.kosong()) {
        cout << "Error: belum ada energi yang terserap!\n";
        return;
    }
    cout << "=== Energi Yang Sudah Terserap ===\n\n";
    int no = 0;
    for (ListNode *p = r.top; p; p = p->next)
        cout << ++no << ". [" << p->data.kode << "] " << p->data.nama << "\n";
}

void hapusRiwayat(Riwayat &r) {
    if (r.kosong()) {
        cout << "Error: riwayat energi kosong, tidak ada yang dihapus!\n";
        return;
    }
    Energi e;
    r.pop(e);
    cout << "Riwayat energi " << e.nama << " teratas telah dihapus!\n";
    delete[] e.nama;
}

void cariEnergi(TreeNode *organ) {
    if (organ == nullptr) {
        cout << "Error: organ kosong!\n";
        return;
    }
    int kode;
    if (!bacaInt("Masukkan kode energi: ", kode) || kode <= 0) {
        cout << "Error: kode energi tidak valid (harus angka lebih dari 0)!\n";
        return;
    }
    TreeNode *n = cariBST(organ, kode);
    if (n == nullptr) cout << "Error: energi dengan kode " << kode << " tidak ada di organ!\n";
    else cout << "Ditemukan: [" << n->data.kode << "] " << n->data.nama << "\n";
}

void lihatOrgan(TreeNode *organ) {
    if (organ == nullptr) {
        cout << "Error: organ kosong!\n";
        return;
    }
    cout << "=== Isi Kristal Organ ===\n\n"
         << "1. Pre-order\n2. In-order\n3. Post-order\n\n";
    int p;
    if (!bacaInt("Masukkan pilihan anda: ", p) || p < 1 || p > 3) {
        cout << "Error: pilihan tidak valid!\n";
        return;
    }
    cout << "\n";
    int no = 0;
    if (p == 1) preOrder(organ, no);
    else if (p == 2) inOrder(organ, no);
    else postOrder(organ, no);
}

void gunakanEnergi(TreeNode *&organ) {
    if (organ == nullptr) {
        cout << "Error: organ kosong!\n";
        return;
    }
    int kode;
    if (!bacaInt("Masukkan kode energi: ", kode) || kode <= 0) {
        cout << "Error: kode energi tidak valid (harus angka lebih dari 0)!\n";
        return;
    }
    TreeNode *n = cariBST(organ, kode);
    if (n == nullptr) {
        cout << "Energi dengan kode " << kode << " tidak ada di organ!\n";
        return;
    }
    cout << "Energi " << n->data.nama << " telah digunakan!\n";
    organ = hapusBST(organ, kode, true);
}

int main() {
    Antrean antrean;
    Riwayat riwayat;
    TreeNode *organ = nullptr;

    while (true) {
        tampilMenu();
        int pilihan;
        if (!bacaInt("Masukkan pilihan anda: ", pilihan)) {
            if (cin.eof()) break;
            cout << "Error: pilihan harus berupa angka 1-9!\n";
            continue;
        }

        switch (pilihan) {
            case 1: tambahEnergi(antrean, riwayat, organ); break;
            case 2: lihatAntrean(antrean); break;
            case 3: prosesEnergi(antrean, riwayat, organ); break;
            case 4: lihatRiwayat(riwayat); break;
            case 5: hapusRiwayat(riwayat); break;
            case 6: cariEnergi(organ); break;
            case 7: lihatOrgan(organ); break;
            case 8: gunakanEnergi(organ); break;
            case 9:
                bebaskanTree(organ);
                cout << "Terima kasih, sampai jumpa!\n";
                return 0;
            default:
                cout << "Error: pilihan tidak valid, pilih angka 1-9!\n";
        }
    }
    bebaskanTree(organ);
    return 0;
}