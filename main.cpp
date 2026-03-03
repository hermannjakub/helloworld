#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
class Transakcja {
private:
    double kwota;
    std::string kategoria;
    std::string data;
    bool czy_wydatek;
public:
    Transakcja(const std::string &k, const double &kw, const std::string &d,  const bool &czyw)
        :kwota(kw),kategoria(k),data(d),czy_wydatek(czyw) {}
    [[nodiscard]]double getKwota() const {
        return kwota;
    }
    [[nodiscard]]std::string getKategoria() const {
        return kategoria;
    }
    [[nodiscard]]std::string getData() const {
        return data;
    }
    [[nodiscard]]bool getCzyWydatek() const {
        return czy_wydatek;
    }
    [[nodiscard]]std::string do_string() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        std::string wydatek;
        if (czy_wydatek) {
            wydatek = "WYDATEK";
        }else {
            wydatek = "PRZYCHOD";
        }
        ss << wydatek<<';';
        ss << data << ';';
        ss << kwota << ';';
        ss << kategoria;
        return ss.str();
    }
};

class ManagerBudzetu {
private:
    std::vector<Transakcja> transakcje;
    double saldo;
public:
    ManagerBudzetu(const double &saldowstepne,  const std::vector<Transakcja> &transakcjones)
        :transakcje(transakcjones), saldo(saldowstepne) {}


    void dodaj_transakcje() {
        std::string data, kategoria;
        double kwota;
        bool czy_wydatek,flag = false;
        std::string wybor;
        do {
            std::cout << "Czy był to wydatek? (Tak/Nie)" << std::endl;
            std::cin >> wybor;
            if (wybor == "Tak") {
                czy_wydatek = true;
                flag = true;
            }
            if (wybor == "Nie") {
                czy_wydatek = false;
                flag = true;
            }
            if (flag == false) {
                std::cout << "Sprobuj ponownie" << std::endl;
            }
        }while (flag == false);

        do {
            std::cout << "Podaj kwote :" << std::endl;
            std::cin >> kwota;
            if (kwota < 0) {
                std::cout << "Kwota nie moze byc ujemna" << std::endl;
            }else {
                flag = false;
            }
        }while (flag == true);

        std::cout << "Podaj date: (D-M-R)" << std::endl;
        std::cin >> data;

        std::cout << "Podaj kategorie wydatku: " << std::endl;
        std::cin >> kategoria;

        Transakcja t1(kategoria,kwota,data,czy_wydatek);
        transakcje.emplace_back(t1);
        if (czy_wydatek) {
            saldo -= kwota;
        }else {
            saldo += kwota;
        }
    }
    void wyswietlWszystko() {
        std::cout << "------------------------------" << std::endl;
        for (auto &el: transakcje) {
            std::cout << "| Data: " << el.getData() << std::endl;
            std::cout << "| Kategoria: " << el.getKategoria() << std::endl;
            std::cout << "| Kwota: " << el.getKwota() << "PLN" << std::endl;
            if (el.getCzyWydatek()) {
                std::cout << "| WYDATEK" << std::endl;
            }else{
                std::cout << "| PRZYCHOD" << std::endl;
            }
            std::cout << "------------------------------" << std::endl;
        }
        std::cout << "| Saldo: " << saldo << std::endl;
    }
    void filtruj_po_kategorii(const std::string &kategoria) {
        double saldo_dla_transakcji = 0.0;
        for (auto &el: transakcje) {
            if (el.getKategoria() == kategoria) {
                std::cout << "------------------------------" << std::endl;
                std::cout << "| Data: " << el.getData();
                std::cout << "| Kategoria: " << el.getKategoria() << std::endl;
                std::cout << "| Kwota: " << el.getKwota() << "PLN" << std::endl;
                if (el.getCzyWydatek()) {
                    std::cout << "| WYDATEK" << std::endl;
                    saldo_dla_transakcji -= el.getKwota();
                }else{
                    std::cout << "| PRZYCHOD" << std::endl;
                    saldo_dla_transakcji += el.getKwota();
                }
                std::cout << "------------------------------" << std::endl;
            }
        }
        std::cout << "| Kwota transakcji: " << saldo_dla_transakcji << std::endl;
    }
    void zapisz_do_pliku() {
        std::fstream file("wydatki.txt",std::ios::out);
        file << saldo << std::endl;
        for (auto &el:transakcje) {
            file << el.do_string() << std::endl;
        }
        file.close();
    }
    void wczytaj_z_pliku() {
        std::fstream file("wydatki.txt",std::ios::in);
        std::string kwotastr,kategoriastr,czywydatekstr,datastr,saldostr;
        if (file.is_open()) {
            std::getline(file,saldostr);
            saldo = std::stod(saldostr);
            while (std::getline(file,czywydatekstr,';')) {
                if (std::getline(file,datastr,';')) {
                    if (std::getline(file,kwotastr,';')) {
                        if (std::getline(file,kategoriastr)) {
                            bool czywydatekb;
                            if (czywydatekstr == "WYDATEK") {
                                czywydatekb = true;
                            }else {
                                czywydatekb = false;
                            }
                            Transakcja t1(kategoriastr,std::stod(kwotastr),datastr,czywydatekb);
                            transakcje.emplace_back(t1);
                        }
                    }
                }
            }
        }
        file.close();
    }
};

int main() {
    std::vector<Transakcja> transakcje;
    ManagerBudzetu Manager(0.0,transakcje);
    bool flag = false;

    do {
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "| Wybierz opcje: " << std::endl;
        int wybor;
        std::cout << "| 1. Dodaj transakcje" << std::endl;
        std::cout << "| 2. Wyswietl historie" << std::endl;
        std::cout << "| 3. Wyswietl historie po kategorii" << std::endl;
        std::cout << "| 4. Zapisz do pliku żrodlowego" << std::endl;
        std::cout << "| 5. Wczytaj z pliku źrodlowego" << std::endl;
        std::cout << "| 0. Wyjscie z programu" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cin >> wybor;
        switch (wybor) {
            case 1: {
                Manager.dodaj_transakcje();
                break;
            }
            case 2: {
                Manager.wyswietlWszystko();
                break;
            }
            case 3: {
                std::cout << "| Jaka kategoria cie interesuje?" << std::endl;
                std::string kategoria;
                std::cin >> kategoria;
                Manager.filtruj_po_kategorii(kategoria);
                break;
            }
            case 4: {
                Manager.zapisz_do_pliku();
                std::cout << "| Pomyślnie zapisano do pliku dane" << std::endl;
                break;
            }
            case 5: {
                Manager.wczytaj_z_pliku();
                std::cout << "| Pomyslnie wczytano z pliku dane" << std::endl;
                break;
            }
            case 0: {
                std::cout << "| Zamykanie programu" << std::endl;
                flag = true;
                break;
            }
                default: {
                std::cout << "Nie ma takiej komendy - sprobuj ponownie!" << std::endl;
            }
        }
    }while (flag == false);
}