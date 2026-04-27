#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>

using namespace std;

//тип для типа поезда
enum TrainType { CARGO, PASSENGER, REPAIR, EXPRESS };

struct Time {
    int hours;
    int minutes;
};

struct Route {
    char name[50];       // название маршрута
    char departure[50];  // пункт отправления
    char arrival[50];    // пункт прибытия
    Time depTime;        // время отправления
    Time arrTime;        // время прибытия
    int  numCars;        // количество вагонов
    TrainType type;      // тип поезда
};

void printTime(const Time& t) {
    cout << setw(2) << setfill('0') << t.hours << ":"
         << setw(2) << setfill('0') << t.minutes;
}

void printTrainType(TrainType t) {
    switch (t) {
        case CARGO:     cout << "Грузовой";     break;
        case PASSENGER: cout << "Пассажирский"; break;
        case REPAIR:    cout << "Ремонтный";    break;
        case EXPRESS:   cout << "Экспресс";     break;
    }
}

void printRoute(const Route& r) {
    cout << "Маршрут:    " << r.name << endl;
    cout << "Отправление: " << r.departure << " в ";
    printTime(r.depTime);
    cout << endl;
    cout << "Прибытие:    " << r.arrival << " в ";
    printTime(r.arrTime);
    cout << endl;
    cout << "Вагонов: " << r.numCars << ", Тип: ";
    printTrainType(r.type);
    cout << endl;
    cout << "    " << endl;
}

//поиск и вывод конкретного маршрута по имени
void printTrainByName(const Route arr[], int size) {
    char name[50];
    cout << "Введите название маршрута: ";
    cin >> name;

    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            printRoute(arr[i]);
            return;
        }
    }
    cout << "Маршрут '" << name << "' не найден." << endl;
}
//3 самых длинных пассажирских поездов
void printTop3LongestPassenger(const Route arr[], int size) {
    Route passenger[20];
    int pCount = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i].type == PASSENGER)
            passenger[pCount++] = arr[i];
    }
    if (pCount == 0) {
        cout << "Нет пассажирских поездов." << endl;
        return;
    }
    //Сортировка пузырьком
    for (int i = 0; i < pCount - 1; i++) {
        for (int j = 0; j < pCount - i - 1; j++) {
            if (passenger[j].numCars < passenger[j + 1].numCars) {
                Route temp = passenger[j];
                passenger[j] = passenger[j + 1];
                passenger[j + 1] = temp;
            }
        }
    }
    int top = (pCount < 3) ? pCount : 3;
    cout << "Три самых длинных пассажирских поезда" << endl;
    for (int i = 0; i < top; i++) {
        cout << i + 1 << ". " << passenger[i].name
             << " — " << passenger[i].numCars << " вагонов" << endl;
    }
    cout << endl;
}

//изменение данных конкретного маршрута
void updateRoute(Route arr[], int size) {
    char name[50];
    cout << "Введите название маршрута для изменения: ";
    cin >> name;

    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            cout << "Введите новое количество вагонов: ";
            cin >> arr[i].numCars;

            int typeInput;
            cout << "Введите тип поезда (0-Грузовой, 1-Пассажирский, 2-Ремонтный, 3-Экспресс): ";
            cin >> typeInput;
            arr[i].type = (TrainType)typeInput;

            cout << "Маршрут '" << name << "' обновлён." << endl;
            printRoute(arr[i]);
            return;
        }
    }
    cout << "Маршрут '" << name << "' не найден." << endl;
}


int filterCargoToSeaside(const Route src[], int srcSize, Route dst[], const char* cities[], int numCities) {
    int count = 0;
    for (int i = 0; i < srcSize; i++) {
        if (src[i].type == CARGO) {
            for (int j = 0; j < numCities; j++) {
                if (strcmp(src[i].arrival, cities[j]) == 0) {
                    dst[count++] = src[i];
                    break;
                }
            }
        }
    }
    return count;
}

void sortByNumCars(Route arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j].numCars > arr[j + 1].numCars) {
                Route temp  = arr[j];
                arr[j]      = arr[j + 1];
                arr[j + 1]  = temp;
            }
        }
    }
}

//оберточная функция
void displayFilteredData(const Route src[], int srcSize) {
    const char* seasideCities[] = {
        "Сочи", "Владивосток", "Севастополь", "Ялта", "Геленджик"
    };
    int numCities = sizeof(seasideCities) / sizeof(seasideCities[0]);

    Route filtered[20];
    int filteredSize = filterCargoToSeaside(src, srcSize, filtered, seasideCities, numCities);
    if (filteredSize == 0) {
        cout << "Нет грузовых маршрутов в приморские города." << endl;
        return;
    }
    sortByNumCars(filtered, filteredSize);

    cout << "=== Грузовые маршруты в приморские города "
            "(отсортированы по числу вагонов) ===" << endl;
    for (int i = 0; i < filteredSize; i++)
        printRoute(filtered[i]);
}


//чтение файла
void updateNumCarsFromFile(const char* filename, Route arr[], int size) {
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
        cout << "Не удалось открыть файл " << filename << endl;
        return;
    }

    char name[50];
    int  newNumCars;
    int  updated = 0;

    while (fin >> name >> newNumCars) {
        for (int i = 0; i < size; i++) {
            if (strcmp(arr[i].name, name) == 0) {
                arr[i].numCars = newNumCars;
                updated++;
                break;
            }
        }
    }
    fin.close();
    cout << "Обновлено " << updated << " маршрутов из файла "
         << filename << endl;
}

//запись и чтение бинарного файла
void syncBinary(const char* filename, Route arr[], int size, int& count) {
    ifstream check(filename);
    if (check.good()) {
        check.close();
        fstream fin(filename, ios::binary | ios::in);
        fin.read((char*)arr, size * sizeof(Route));
        count = (int)(fin.gcount() / sizeof(Route));
        fin.close();
        cout << "Загружено " << count << " маршрутов из " << filename << endl;
    } else {
        check.close();
        ofstream fout(filename, ios::binary | ios::out);
        fout.write((char*)arr, size * sizeof(Route));
        fout.close();
        count = size;
        cout << "Файл создан, записано " << count << " маршрутов в " << filename << endl;
    }
}

int main() {
    setlocale(LC_ALL, "");

    Route schedule[20] = {
        {"Грузовой1", "Москва", "Сочи", {8,0}, {18,0}, 15, CARGO},
        {"Грузовой2", "Санкт-Петербург", "Владивосток", {6,30}, {22,45}, 20, CARGO},
        {"Грузовой3", "Новосибирск", "Севастополь", {10,15}, {21,0}, 12, CARGO},
        {"Грузовой4", "Екатеринбург", "Ялта", {7,45}, {19,30}, 18, CARGO},
        {"Грузовой5", "Казань", "Геленджик", {9,0}, {17,15}, 10, CARGO},
        {"Грузовой6", "Ростов", "Воронеж", {5,0}, {12,0}, 8, CARGO},
        {"Грузовой7", "Краснодар", "Ставрополь", {14,30}, {20,0}, 25, CARGO},
        {"Пасс1", "Москва", "Санкт-Петербург", {7,0}, {11,30}, 10, PASSENGER},
        {"Пасс2", "Москва", "Владивосток", {0,0}, {7,0}, 18, PASSENGER},
        {"Пасс3", "Сочи", "Москва", {20,0}, {6,0}, 12, PASSENGER},
        {"Пасс4", "Новосибирск", "Москва", {13,0}, {5,0}, 22, PASSENGER},
        {"Пасс5", "Екатеринбург", "Севастополь", {11,0}, {23,0}, 9, PASSENGER},
        {"Пасс6", "Казань", "Ялта", {8,0}, {18,30}, 14, PASSENGER},
        {"Рем1", "Москва", "Тула", {9,0}, {11,0}, 4, REPAIR},
        {"Рем2", "Санкт-Петербург", "Псков", {10,0}, {13,0}, 5, REPAIR},
        {"Эксп1", "Москва", "Нижний Новгород", {6,0}, {8,30}, 7, EXPRESS},
        {"Эксп2", "Москва", "Казань", {7,0}, {10,0}, 8, EXPRESS},
        {"Эксп3", "Санкт-Петербург", "Москва", {18,0}, {21,30}, 6, EXPRESS},
        {"Грузовой8", "Челябинск", "Омск", {4,0}, {10,0}, 11, CARGO},
        {"Грузовой9", "Уфа", "Самара", {12,0}, {16,0}, 9, CARGO}
    };
    int size = 20;

    cout << "Исходный список маршрутов" << endl;
    for (int i = 0; i < size; i++)
        printRoute(schedule[i]);

    displayFilteredData(schedule, size);


    cout << "Данные по заданному маршруту" << endl;
    printTrainByName(schedule, size);

    //три самых длинных пассажирских
    printTop3LongestPassenger(schedule, size);

    cout << "Изменение данных маршрута" << endl;
    updateRoute(schedule, size);

    //для примера посмотрим Грузовой3
    cout << "До обновления 'Грузовой3':" << endl;
    printRoute(schedule[2]);
    cout << "Обновление из текстового файла: " << endl;
    updateNumCarsFromFile("train_numcars.txt", schedule, size);
    cout << "После обновления 'Грузовой3':" << endl;
    printRoute(schedule[2]);

    int loadedSize = 0;
    syncBinary("routes.bin", schedule, size, loadedSize);

    cout << "Данные из бинарного файла" << endl;
    for (int i = 0; i < loadedSize; i++)
        printRoute(schedule[i]);


    cin.ignore(100);
    cin.get();
    return 0;
}
