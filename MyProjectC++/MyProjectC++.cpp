#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <Windows.h>

// Класс для лошадей
class Horse {
private:
    std::string name;
    std::string breed;
    int stamina;
    char gender;

public:
    Horse(std::string n, std::string b, int s, char g) : name(n), breed(b), stamina(s), gender(g) {}

    std::string getName() const { return name; }
    std::string getBreed() const { return breed; }
    int getStamina() const { return stamina; }
    char getGender() const { return gender; }

    void decreaseStamina(int amount) { stamina -= amount; }
    void increaseStamina(int amount) { stamina += amount; }
};

// Основной класс для корма
class Feed {
public:
    virtual int getStaminaBoost() const = 0;
    virtual int getCost() const = 0;
    virtual std::string getName() const = 0;
};
//Классы корма
class Apple : public Feed {
public:
    int getStaminaBoost() const override { return 15; }
    int getCost() const override { return 10; }
    std::string getName() const override { return "Яблоко"; }
};

class Oats : public Feed {
public:
    int getStaminaBoost() const override { return 10; }
    int getCost() const override { return 3; }
    std::string getName() const override { return "Овес"; }
};

class Carrots : public Feed {
public:
    int getStaminaBoost() const override { return 12; }
    int getCost() const override { return 7; }
    std::string getName() const override { return "Морковь"; }
};

// Класс для ученика
class Student {
private:
    std::string name;
    int experience;
    int coins;

public:
    Student(std::string n) : name(n), experience(0), coins(100) {}

    std::string getName() const { return name; }
    int getExperience() const { return experience; }
    int getCoins() const { return coins; }

    void increaseExperience(int amount) { experience += amount; }
    void decreaseCoins(int amount) { coins -= amount; }
    void increaseCoins(int amount) { coins += amount; }
};

// Класс для конной фермы
class HorseFarm {
private:  
    std::vector<Student> students;
    std::vector<Feed*> feeds;
public:
    std::vector<Horse> horses;
    HorseFarm() {
        // Инициализация корма
        feeds.push_back(new Apple());
        feeds.push_back(new Oats());
        feeds.push_back(new Carrots());

        // Инициализация лошадей
        horses.push_back(Horse("Камелия", "Алтайская", 50, 'ж'));
        horses.push_back(Horse("Вихрь", "Забайкальская", 60, 'м'));
    }

    int selectHorseForBreeding(char gender, int stamina) const {

        std::vector<int> availableHorses;
       
        for (int i = 0; i < horses.size(); ++i) {
            const Horse& horse = horses[i];
            if (horse.getGender() == gender || horse.getStamina() < stamina) {
                continue; 
            }
            availableHorses.push_back(i);
        }

        // Если нет подходящих лошадей для скрещивания
        if (availableHorses.empty()) {
            std::cout << "Нет подходящих лошадей для скрещивания.\n";
            return -1; // Возвращаем -1, чтобы показать отсутствие выбора
        }

        // Вывод списка доступных лошадей для скрещивания
        std::cout << "Список доступных лошадей для скрещивания:\n";
        for (int i = 0; i < availableHorses.size(); ++i) {
            const Horse& horse = horses[availableHorses[i]];
            std::cout << i + 1 << ". Имя: " << horse.getName() << ". Порода: " 
                << horse.getBreed() << ". Выносливость: " << horse.getStamina() << ".\n";
        }

        // Выбор лошади для скрещивания
        int selection;
        do {
            std::cout << "Выберите лошадь для скрещивания (номер): ";
            std::cin >> selection;
        } while (selection < 1 || selection > availableHorses.size());

        // Возвращаем индекс выбранной лошади
        return availableHorses[selection - 1];
    }

    void crossbreed(Student& student, char gender, int stamina, int selectedHorseIndex) {
        Horse& selectedHorse = horses[selectedHorseIndex];

        // Проверка пола и выносливости выбранной лошади
        if (selectedHorse.getGender() != gender || selectedHorse.getStamina() < stamina) {
            std::cout << "Выбранная лошадь не подходит для скрещивания.\n";
            return;
        }

        // Генерация сообщения о новой лошади
        std::string genderString = (gender == 'м') ? "мужского" : "женского";
        std::string breedMessage = (selectedHorse.getBreed() == "Алтайская") ? "Алтайская" : "Забайкальская";
        std::string randomMessage = (rand() % 2 == 0) ? "на свет появилсь маленькая лошадка" : "на свет появилсь большая лошадка";
        std::cout << "Поздравляем, " << randomMessage << " " << genderString << " пола породы " << breedMessage << ".\n";

        // Запрос имени новой лошади
        std::string newName;
        std::cout << "Предложите имя для новой лошади: ";
        std::cin >> newName;

        // Добавление новой лошади в список
        horses.push_back(Horse(newName, selectedHorse.getBreed(), 1, gender));

        int initialHorseIndex = selectedHorseIndex;
        Horse& initialHorse = horses[initialHorseIndex];
        initialHorse.decreaseStamina(20);

        // Уменьшение выносливости изначально выбранной лошади
        if (initialHorseIndex > 0) {
            Horse& initialHorses = horses[initialHorseIndex - 1];
            initialHorses.decreaseStamina(20);
        }
  
        // Увеличение монет и опыта у ученика
        student.increaseCoins(50);
        student.increaseExperience(20);
    }


    void ride(Student& student, int horseIndex, int speed) {
        Horse& horse = horses[horseIndex];

        // Уменьшаем выносливость лошади
        int staminaDecrease = speed * 5;
        horse.decreaseStamina(staminaDecrease);

        // Увеличиваем опыт ученика
        int experienceIncrease = speed * 5;
        student.increaseExperience(experienceIncrease);

        // Если выносливость лошади меньше или равна 0, то она умирает
        if (horse.getStamina() <= 0) {
            std::cout << "Лошадь " << horse.getName() << " умерла от усталости!\n";
            horses.erase(horses.begin() + horseIndex);
        }
    }

    void displayHorses() const {
        int number = 1;
        std::cout << "Список лошадей:\n";
        for (const Horse& horse : horses) {
            std::cout << number++ << ". Имя: " << horse.getName() << ". Порода - " << horse.getBreed() <<
                ", выносливость - " << horse.getStamina() << ", гендер - " << horse.getGender() << ".\n";
        }
    }

    void displayFeed() const {
        int number = 1;
        std::cout << "Список кормов:\n";
        for (const Feed* feed : feeds) {
            std::cout << number++ << ". " << feed->getName() << " - " << feed->getCost() << 
                " монет, +" << feed->getStaminaBoost() << " выносливости\n";
        }
    }

    void interact(Student& student, int horseIndex, int feedIndex) {
        Horse& horse = horses[horseIndex];
        Feed* feed = feeds[feedIndex];

        // Уменьшаем выносливость остальных лошадей
        for (Horse& otherHorse : horses) {
            if (&otherHorse != &horse) {
                otherHorse.decreaseStamina(5);
            }
        }

        // Проверка выносливости всех лошадей
        for (int i = 0; i < horses.size(); ++i) {
            if (horses[i].getStamina() <= 0) {
                std::cout << "Лошадь " << horses[i].getName() << " умерла от недостатка еды!\n";
                horses.erase(horses.begin() + i);
            }
        }

        // Кормление лошади
        horse.increaseStamina(feed->getStaminaBoost());
        student.decreaseCoins(feed->getCost()); // Уменьшаем количество монет у ученика    
    }
};

int main() {
    SetConsoleCP(1251);
    setlocale(0, "");

    std::string playerName;
    std::cout << "Введите ваше имя: ";
    std::cin >> playerName;

    Student player(playerName);
    HorseFarm farm;
    int number;

    std::cout << "\nДобро пожаловать на ферму, " << player.getName() << "!\n";
    std::cout << "У вас " << player.getCoins() << " монет и " << player.getExperience() << " опыта.\n";

    farm.displayHorses();

    int horseIndex, feedIndex;

    do {
        std::cout << "Выберите лошадь для взаимодействия (номер): ";
        std::cin >> horseIndex;

        if (horseIndex < 1 || horseIndex > farm.horses.size()) {
            std::cout << "Неверный номер лошади. Пожалуйста, выберите существующую лошадь.\n";
        }
        else {
            std::cout << "Вы успешно выбрали лошадь " << farm.horses[horseIndex - 1].getName() << "\n";
            // Продолжение взаимодействия с выбранной лошадью
            break; // Выход из цикла после успешного выбора лошади
        }
    } while (true);

    // Основной игровой цикл
    while (true) {

        if (farm.horses.empty()) {
            std::cout << "Все лошади на ферме умерли. Вы проиграли!\n";
            break; // Выход из цикла while
        }

        std::cout << "Список действий: \n 1. Кормить лошадь. \n 2. Кататься на лошади.";
        std::cout << " \n 3. Скрестить лошадь. \n 4. Выбрать другую лошадь. \n 5. Уйти с фермы\n";
        std::cout << "Выберите действие (номер): ";
        std::cin >> number;

        if (number == 1) {
            farm.displayFeed();
            std::cout << "Выберите корм (номер): ";
            std::cin >> feedIndex;
            switch (feedIndex)
            {
            case 1:
                farm.interact(player, horseIndex - 1, feedIndex - 1);
                // Увеличение опыта ученика
                player.increaseExperience(5);
                break;

            case 2:
                farm.interact(player, horseIndex - 1, feedIndex - 1);
                player.increaseExperience(1);
                break;

            case 3:
                farm.interact(player, horseIndex - 1, feedIndex - 1);
                player.increaseExperience(3);
                break;
            default:
                break;
            }
        }
        else if (number == 2) {
            int speed;
            std::cout << "Выберите скорость езды: \n 1 - шаг \n 2 - рысь \n 3 - галоп\n";
            std::cout << "Введите выбранную скорость (номер):\n ";
            std::cin >> speed;

            switch (speed) {
            case 1:
                farm.ride(player, horseIndex - 1, speed);
                std::cout << "Вы медленным шагом прогулялись по ферме.. ";
                break;

            case 2:
                farm.ride(player, horseIndex - 1, speed);
                std::cout << "Вы рысью прокатились по территории фермы... ";
                break;

            case 3:
                farm.ride(player, horseIndex - 1, speed);
                std::cout << "Вы галопом пронеслись по дороге с препятствиями! ";
                break;

            default:
                std::cout << "Неверная скорость.\n";
                break;
            }
        }
        else if (number == 3) {
            char gender = farm.horses[horseIndex - 1].getGender(); 
            // Пол лошади берется из изначального выбора
            int stamina = 20;
            int selectedHorseIndex = farm.selectHorseForBreeding(gender, stamina);
            if (selectedHorseIndex != -1) {
                farm.crossbreed(player, farm.horses[selectedHorseIndex].getGender(),
                    farm.horses[selectedHorseIndex].getStamina(), selectedHorseIndex);
            }
        }
        else if (number == 4) {
            farm.displayHorses();
            do {
                std::cout << "Выберите лошадь для взаимодействия (номер): ";
                std::cin >> horseIndex;

                if (horseIndex < 1 || horseIndex > farm.horses.size()) {
                    std::cout << "Неверный номер лошади. Пожалуйста, выберите существующую лошадь.\n";
                }
                else {
                    std::cout << "Вы успешно выбрали лошадь " << farm.horses[horseIndex - 1].getName() << "\n";
                    // Продолжение взаимодействия с выбранной лошадью
                    break; // Выход из цикла после успешного выбора лошади
                }
            } while (true);
        }
        else if (number == 5) {
            std::cout << "Вы покинули ферму. До новых встреч!\n";
            break; // Выход из цикла while
        }

        else {
            std::cout << "Неверный выбор.\n";
            continue;
        }

        // Проверка на умерших лошадей
        for (int i = 0; i < farm.horses.size(); ++i) {
            if (farm.horses[i].getStamina() <= 0) {
                std::cout << "Лошадь " << farm.horses[i].getName() << " скончалась!\n";
                farm.horses.erase(farm.horses.begin() + i);
            }
        }

        // Обновление данных в режиме онлайн
        std::cout << "\nОбновленные данные:\n";
        std::cout << "У вас " << player.getCoins() << " монет и " << player.getExperience() << " опыта.\n";
        farm.displayHorses();
    }
    return 0;
}
