#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int siz = 0;

class Animal {
public:
    Animal(string ne, int x, int y, int a, int b)
        : name(ne), year(0), direction(a), distance(b), x_position(x), y_position(y), pass(0), meal(0) {}

    virtual ~Animal() {}

    // Чисто виртуальные методы
    virtual void move(int n, int m) = 0; // Метод для движения
    virtual void food(vector<Animal*>& animals) = 0; // Метод для питания
    virtual void aging(vector<Animal*>& animals, int i) = 0; // Метод для старения
    virtual void reproduce(vector<Animal*>& animals) = 0; // Метод для размножения
    virtual void dying(vector<Animal*>& animals, int i) = 0; // Метод для умирания


public:
    string name;
    int year;
    int direction; 
    int distance; 
    int x_position; 
    int y_position; 
    int pass;
    int meal; 
};

class Wolf : public Animal {
public:
    Wolf(int x, int y, int a, int b)
        : Animal("W", x, y, a, b) {}

    void move(int n, int m) override { // каждого двигаем
        if (direction == 0) {
            this->y_position = (y_position - 1 + m) % m;
        } else if (direction == 1) {
            this->x_position = (x_position + 1) % n;
        } else if (direction == 2) {
            this->y_position = (y_position + 1) % m;
        } else if (direction == 3) {
            this->x_position = (x_position - 1 + n) % n;
        }

        pass++;
        if(pass == distance) {
            pass = 0;
            direction = (direction + 1) % 4;
        }
    }

    void food(vector<Animal*>& animals) override {
        for (Animal* Animal : animals) {
            //cout << Animal->name << endl;
            if (Animal->name == "W") {
                //cout << "ee" << endl;
                for (int j = 0; j < animals.size(); j++) {
                    //cout << animals[j]->name << " " << Animal->x_position << " " << animals[j]->x_position << " " << Animal->y_position << " " << animals[j]->y_position << endl;
                    if ((animals[j]->name == "H") && (Animal->x_position == animals[j]->x_position) && (Animal->y_position == animals[j]->y_position)) {
                        animals[j]->dying(animals, j);
                        meal++;
                        if (meal == 2) {
                            meal = 0;
                            this->reproduce(animals);
                        }
                    }
                }
            }
        }
    }

    void aging(vector<Animal*>& animals, int i) override { // каждого стареем
        year++;
        if (year >= 15) {
            this->dying(animals, i);
        }
    }

    void reproduce(vector<Animal*>& animals) override {
        animals.push_back(new Wolf(x_position, y_position, direction, distance));
    }

    void dying(vector<Animal*>& animals, int i) override { // умирать
        for(int j = i; j < animals.size() - 1; j++) {
            animals[j] = animals[j + 1];
        }
        animals.pop_back();
        siz--;
    }

};

class Hare : public Animal {
public:
    Hare(int x, int y, int a, int b)
        : Animal("H", x, y, a, b) {}

    void move(int n, int m) override { // каждого двигаем
        if (direction == 0) {
            this->y_position = (y_position - 1 + m) % m;
        } else if (direction == 1) {
            this->x_position = (x_position + 1) % n;
        } else if (direction == 2) {
            this->y_position = (y_position + 1) % m;
        } else if (direction == 3) {
            this->x_position = (x_position - 1 + n) % n;
        }

        pass++;
        if(pass == distance) {
            pass = 0;
            direction = (direction + 1) % 4;
        }
    }

    void food(vector<Animal*>& animals) override {}

    void aging(vector<Animal*>& animals, int i) override { // каждого стареем
        year++;
        if (year == 5 || year == 10) {
            this->reproduce(animals);
        }
        if (year >= 10) {
            this->dying(animals, i);
        }
    }

    void reproduce(vector<Animal*>& animals) override {
        animals.push_back(new Hare(x_position, y_position, direction, distance));
    }

    void dying(vector<Animal*>& animals, int i) override { // умирать
        //cout << animals[i]->name << i;
        for(int j = i; j < animals.size() - 1; j++) {
            animals[j] = animals[j + 1];
        }
        animals.pop_back();
        siz--;
    }
 
};

int Print(vector<Animal*> animals, int n, int m) {
    cout << endl;
    vector<vector<string>> S;
    for (int i = 0; i < m; i++) {
        vector<string> D;
        for (int j = 0; j < n; j++) {
            D.push_back("");
        }
        S.push_back(D);
    }

    for(int i = 0; i < animals.size(); i++) {
        S[animals[i]->y_position][animals[i]->x_position] += (animals[i]->name + to_string(i));
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (S[i][j] == "") {
                cout << "**";
            } else {
                cout << S[i][j];
            }
            cout << " ";
        }
        cout << endl;
    }
    return 0;
}

int main() {
    // Создаем массив указателей на базовый класс Animal
    std::vector<Animal*> animals;

    int n, m, t, w, h = 0;
    cin >> n >> m >> t;
    cin >> h >> w;

    for (int i = 0; i < h; i++) {
        int x, y, g, d;
        cin >> x >> y >> g >> d;
        animals.push_back(new Hare(x, y, g, d));
    }

    for (int i = 0; i < w; i++) {
        int x, y, g, d;
        cin >> x >> y >> g >> d;
        animals.push_back(new Wolf(x, y, g, d));
    }

    Print(animals, n, m);

    for (int i = 0; i < t; i++) {
        siz = animals.size();
        for (int j = 0; j < siz; j++) {
            animals[j]->move(n, m);
        }    
        for (int j = 0; j < siz; j++) {
            animals[j]->food(animals);
        } 
        for (int j = 0; j < siz; j++) {
            animals[j]->aging(animals, j);
        }   
        Print(animals, n, m);
        cout << "move " << i + 1 << endl;
    }

    cout << endl << "Итог" << endl;

    vector<vector<int>> S;
    for (int i = 0; i < m; i++) {
        vector<int> D;
        for (int j = 0; j < n; j++) {
            D.push_back(0);
        }
        S.push_back(D);
    }

    for(int i = 0; i < animals.size(); i++) {
        if (animals[i]->name == "H") {
            S[animals[i]->y_position][animals[i]->x_position] += 1;
        } else {
            S[animals[i]->y_position][animals[i]->x_position] -= 1;
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (S[i][j] == 0) {
                cout << "*";
            } else {
                cout << S[i][j];
            }
            cout << " ";
        }
        cout << endl;
    }

    // Освобождаем память
    for (Animal* Animal : animals) {
        delete Animal;
    }

    return 0;
}
