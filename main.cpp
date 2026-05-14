#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

using namespace std;

const int BOARD_SIZE = 48;
const int MAX_TURNS = 2000;
const int NUM_PIECES = 4;

struct QuanCo {
    int viTri;
    bool veDich;

    QuanCo() : viTri(-1), veDich(false) {}
};

class NguoiChoi {
public:
    string ten;
    int chienThuat;
    QuanCo quan[NUM_PIECES];
    int soLanThang;
    int tongLuotChoi;

    NguoiChoi(string t, int ct)
        : ten(t), chienThuat(ct), soLanThang(0), tongLuotChoi(0) {
        for (int i = 0; i < NUM_PIECES; i++) {
            quan[i] = QuanCo();
        }
    }

    void reset() {
        for (int i = 0; i < NUM_PIECES; i++) {
            quan[i].viTri = -1;
            quan[i].veDich = false;
        }
    }

    int soQuanONha() const {
        int count = 0;
        for (int i = 0; i < NUM_PIECES; i++) {
            if (quan[i].viTri == -1) count++;
        }
        return count;
    }

    int soQuanVeDich() const {
        int count = 0;
        for (int i = 0; i < NUM_PIECES; i++) {
            if (quan[i].veDich) count++;
        }
        return count;
    }

    int soQuanTrenBan() const {
        int count = 0;
        for (int i = 0; i < NUM_PIECES; i++) {
            if (quan[i].viTri >= 0 && quan[i].viTri < BOARD_SIZE) count++;
        }
        return count;
    }


    int chonQuan(int diem, const int banCo[], int myTurn) {
        // 1. Ưu tiên xuất chuồng nếu đổ được 6
        if (diem == 6) {
            for (int i = 0; i < NUM_PIECES; i++) {
                if (quan[i].viTri == -1) {
                    int startPos = (myTurn * 12) % BOARD_SIZE;

                    if (banCo[startPos] != myTurn) {
                        return i;
                    }
                }
            }
        }


        vector<int> coTheDi;
        for (int i = 0; i < NUM_PIECES; i++) {
            if (quan[i].viTri >= 0 && quan[i].viTri < BOARD_SIZE && !quan[i].veDich) {
                int nextViTri = quan[i].viTri + diem;

                //
                if (nextViTri > BOARD_SIZE) {
                    nextViTri = 2 * BOARD_SIZE - nextViTri;
                }

                if (nextViTri < BOARD_SIZE) {
                    int absPos = (myTurn * 12 + nextViTri) % BOARD_SIZE;
                    if (banCo[absPos] != myTurn) {
                        coTheDi.push_back(i);
                    }
                }
                else if (nextViTri == BOARD_SIZE) {
                    coTheDi.push_back(i);
                }
            }
        }

        if (coTheDi.empty()) return -1;

        int chosen = coTheDi[0];

        if (chienThuat == 0) {
            for (int i : coTheDi) {
                int nextViTri = quan[i].viTri + diem;
                if (nextViTri > BOARD_SIZE) nextViTri = 2 * BOARD_SIZE - nextViTri;

                if (nextViTri < BOARD_SIZE) {
int target = (myTurn * 12 + nextViTri) % BOARD_SIZE;
                    if (banCo[target] != -1 && banCo[target] != myTurn) {
                        return i;
                    }
                }
            }
        }
        else if (chienThuat == 1) {
            for (int i : coTheDi) {
                if (quan[i].viTri > quan[chosen].viTri) {
                    chosen = i;
                }
            }
        }
        else if (chienThuat == 2) {
            for (int i : coTheDi) {
                if (quan[i].viTri < quan[chosen].viTri) {
                    chosen = i;
                }
            }
        }
        else if (chienThuat == 3) {
            for (int i : coTheDi) {
                int nextViTri = quan[i].viTri + diem;
                if (nextViTri > BOARD_SIZE) nextViTri = 2 * BOARD_SIZE - nextViTri;

                if (nextViTri < BOARD_SIZE) {
                    int newPos = (myTurn * 12 + nextViTri) % BOARD_SIZE;
                    if (banCo[newPos] == -1) {
                        chosen = i;
                        break;
                    }
                }
            }
        }

        return chosen;
    }
};

void runGame(vector<NguoiChoi>& players) {
    int banCo[BOARD_SIZE];
    fill(banCo, banCo + BOARD_SIZE, -1);

    for (auto& p : players) {
        p.reset();
        p.tongLuotChoi++;
    }

    int turn = 0;
    int totalTurns = 0;
    bool done = false;

    while (!done && totalTurns < MAX_TURNS) {
        NguoiChoi& p = players[turn];
        int diem = (rand() % 6) + 1;

        int id = p.chonQuan(diem, banCo, turn);

        if (id != -1) {

            if (p.quan[id].viTri >= 0 && p.quan[id].viTri < BOARD_SIZE) {
                int oldAbs = (turn * 12 + p.quan[id].viTri) % BOARD_SIZE;
                banCo[oldAbs] = -1;
            }


            int nextViTri;
            if (p.quan[id].viTri == -1) {
                nextViTri = 0;
            }
            else {
                nextViTri = p.quan[id].viTri + diem;
                if (nextViTri > BOARD_SIZE) {
                    nextViTri = 2 * BOARD_SIZE - nextViTri;
                }
            }


            if (nextViTri == BOARD_SIZE) {
                p.quan[id].veDich = true;
                p.quan[id].viTri = BOARD_SIZE;
            }
            else {
                p.quan[id].viTri = nextViTri;
                int newAbs = (turn * 12 + nextViTri) % BOARD_SIZE;

                // Logic đá quân chuẩn xác
                if (banCo[newAbs] != -1 && banCo[newAbs] != turn) {
                    int p_bi_da = banCo[newAbs];
                    for (int j = 0; j < NUM_PIECES; j++) {
                        if (players[p_bi_da].quan[j].viTri >= 0 && players[p_bi_da].quan[j].viTri < BOARD_SIZE) {
                            int enemyAbs = (p_bi_da * 12 + players[p_bi_da].quan[j].viTri) % BOARD_SIZE;
                            if (enemyAbs == newAbs) {
players[p_bi_da].quan[j].viTri = -1;
                                break;
                            }
                        }
                    }
                }
                banCo[newAbs] = turn;
            }


            if (p.soQuanVeDich() == NUM_PIECES) {
                p.soLanThang++;
                done = true;
            }
        }

        if (diem != 6) {
            turn = (turn + 1) % 4;
        }

        totalTurns++;
    }
}

int main() {
    srand(time(0));
    int n;

    cout << "       TRO CHOI CO CA NGUA - SIMULATOR" << endl;
    cout << "\nNhap so van muon mo phong: ";

    if (!(cin >> n) || n <= 0) {
        cout << "Loi: So van phai la so duong!" << endl;
        return 1;
    }

    vector<NguoiChoi> players = {
        NguoiChoi("CHIEN THUAT LIEU", 0),
        NguoiChoi("CHIEN THUAT TOC DO", 1),
        NguoiChoi("CHIEN THUAT DAN QUAN", 2),
        NguoiChoi("CHIEN THUAT AN TOAN", 3)
    };

    for (int i = 0; i < n; i++) {
        runGame(players);
    }

    cout << "\n========================================" << endl;
    cout << "   KET QUA SAU " << n << " VAN MO PHONG" << endl;

    vector<pair<double, int>> results;
    for (int i = 0; i < 4; i++) {
        double winRate = (double)players[i].soLanThang / n * 100;
        results.push_back({ winRate, i });
    }

    sort(results.rbegin(), results.rend());

    int rank = 1;
    for (auto [winRate, idx] : results) {
        cout << rank << ". " << left << setw(25) << players[idx].ten
            << right << setw(5) << players[idx].soLanThang << " thang | "
            << fixed << setprecision(2) << setw(6) << winRate << "%" << endl;
        rank++;
    }

    cout << "----------------------------------------" << endl;
    cout << "\nChi tiet tung chien thuat:" << endl;


    for (int i = 0; i < 4; i++) {
        cout << "\n" << players[i].ten << endl;
        cout << "  Tong luot choi: " << players[i].tongLuotChoi << endl;
        cout << "  Tong thang:     " << players[i].soLanThang << endl;
        double rate = players[i].tongLuotChoi > 0 ? (double)players[i].soLanThang / players[i].tongLuotChoi * 100 : 0;
        cout << "  Ty le thang (Win/Luot): " << fixed << setprecision(2) << rate << "%" << endl;
    }

    cout << "\n========================================" << endl;
    cout << "       KET THUC MO PHONG" << endl;

    return 0;
}
