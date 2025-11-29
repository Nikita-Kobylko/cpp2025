#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Используем union-like подход
class SpecialParam {
 private:
  enum Type { INT, DOUBLE, STRING } type;
  union {
    int int_val;
    double double_val;
  };
  std::string str_val;

 public:
  SpecialParam(int val) : type(INT), int_val(val) {}
  SpecialParam(double val) : type(DOUBLE), double_val(val) {}
  SpecialParam(const std::string& val) : type(STRING), str_val(val) {}

  int getInt() const { return int_val; }
  double getDouble() const { return double_val; }
  std::string getString() const { return str_val; }
  Type getType() const { return type; }
};

// Абстрактный базовый класс
class MagicItem {
 protected:
  std::string name;
  int price;
  double weight;

 public:
  MagicItem(const std::string& n, int p, double w)
      : name(n), price(p), weight(w) {}
  virtual ~MagicItem() = default;

  // Чисто виртуальный метод для получения специфического параметра
  virtual SpecialParam getSpecParam() const = 0;

  // Геттеры
  std::string getName() const { return name; }
  int getPrice() const { return price; }
  double getWeight() const { return weight; }

  // Виртуальный метод для получения типа предмета
  virtual std::string getType() const = 0;
};

// Класс Оружие
class Weapon : public MagicItem {
 private:
  int damage;

 public:
  Weapon(const std::string& n, int p, double w, int d)
      : MagicItem(n, p, w), damage(d) {}

  SpecialParam getSpecParam() const override { return SpecialParam(damage); }

  std::string getType() const override { return "Weapon"; }

  int getDamage() const { return damage; }
};

// Класс Броня
class Armor : public MagicItem {
 private:
  int defense;

 public:
  Armor(const std::string& n, int p, double w, int d)
      : MagicItem(n, p, w), defense(d) {}

  SpecialParam getSpecParam() const override { return SpecialParam(defense); }

  std::string getType() const override { return "Armor"; }

  int getDefense() const { return defense; }
};

// Класс Зелье
class Potion : public MagicItem {
 private:
  double duration;

 public:
  Potion(const std::string& n, int p, double w, double d)
      : MagicItem(n, p, w), duration(d) {}

  SpecialParam getSpecParam() const override { return SpecialParam(duration); }

  std::string getType() const override { return "Potion"; }

  double getDuration() const { return duration; }
};

// Класс Свиток
class Scroll : public MagicItem {
 private:
  std::string effect;

 public:
  Scroll(const std::string& n, int p, double w, const std::string& e)
      : MagicItem(n, p, w), effect(e) {}

  SpecialParam getSpecParam() const override { return SpecialParam(effect); }

  std::string getType() const override { return "Scroll"; }

  std::string getEffect() const { return effect; }
};

// Класс для представления магазина
class Shop {
 private:
  std::string name;
  std::vector<std::unique_ptr<MagicItem>> items;

 public:
  Shop(const std::string& n) : name(n) {}

  void addItem(std::unique_ptr<MagicItem> item) {
    items.push_back(std::move(item));
  }

  // Методы для вычисления статистики
  double getAveragePrice() const {
    if (items.empty()) return 0.0;
    double total = 0;
    for (const auto& item : items) {
      total += item->getPrice();
    }
    return total / items.size();
  }

  double getAverageWeight() const {
    if (items.empty()) return 0.0;
    double total = 0;
    for (const auto& item : items) {
      total += item->getWeight();
    }
    return total / items.size();
  }

  // Метод для получения самого частого эффекта у свитков
  std::string getMostCommonScrollEffect() const {
    std::map<std::string, int> effectCount;
    for (const auto& item : items) {
      if (item->getType() == "Scroll") {
        Scroll* scroll = dynamic_cast<Scroll*>(item.get());
        if (scroll) {
          effectCount[scroll->getEffect()]++;
        }
      }
    }

    if (effectCount.empty()) return "";

    auto maxEffect = effectCount.begin();
    for (auto it = effectCount.begin(); it != effectCount.end(); ++it) {
      if (it->second > maxEffect->second) {
        maxEffect = it;
      }
    }

    return maxEffect->first;
  }

  // Методы для получения средних значений специфических параметров
  double getAverageWeaponDamage() const {
    int count = 0;
    double total = 0;
    for (const auto& item : items) {
      if (item->getType() == "Weapon") {
        Weapon* weapon = dynamic_cast<Weapon*>(item.get());
        if (weapon) {
          total += weapon->getDamage();
          count++;
        }
      }
    }
    return count > 0 ? total / count : 0;
  }

  double getAverageArmorDefense() const {
    int count = 0;
    double total = 0;
    for (const auto& item : items) {
      if (item->getType() == "Armor") {
        Armor* armor = dynamic_cast<Armor*>(item.get());
        if (armor) {
          total += armor->getDefense();
          count++;
        }
      }
    }
    return count > 0 ? total / count : 0;
  }

  double getAveragePotionDuration() const {
    int count = 0;
    double total = 0;
    for (const auto& item : items) {
      if (item->getType() == "Potion") {
        Potion* potion = dynamic_cast<Potion*>(item.get());
        if (potion) {
          total += potion->getDuration();
          count++;
        }
      }
    }
    return count > 0 ? total / count : 0;
  }

  // Методы для проверки наличия типов предметов
  bool hasWeapons() const {
    for (const auto& item : items) {
      if (item->getType() == "Weapon") return true;
    }
    return false;
  }

  bool hasArmor() const {
    for (const auto& item : items) {
      if (item->getType() == "Armor") return true;
    }
    return false;
  }

  bool hasPotions() const {
    for (const auto& item : items) {
      if (item->getType() == "Potion") return true;
    }
    return false;
  }

  bool hasScrolls() const {
    for (const auto& item : items) {
      if (item->getType() == "Scroll") return true;
    }
    return false;
  }

  // Геттеры
  std::string getName() const { return name; }
  size_t getItemCount() const { return items.size(); }

  // Метод для вывода статистики
  void printStats() const {
    std::cout << "=== Магазин: " << name << " ===\n";
    std::cout << "Всего предметов: " << items.size() << "\n\n";

    std::cout << "Средняя цена: " << getAveragePrice() << " золота\n";
    std::cout << "Средний вес: " << getAverageWeight() << " кг\n\n";

    std::cout << "Статистика по предметам:\n";

    if (hasWeapons()) {
      std::cout << "- Оружие: средний урон = " << getAverageWeaponDamage()
                << "\n";
    } else {
      std::cout << "- Оружие: нет в наличии\n";
    }

    if (hasArmor()) {
      std::cout << "- Броня: средняя защита = " << getAverageArmorDefense()
                << "\n";
    } else {
      std::cout << "- Броня: нет в наличии\n";
    }

    if (hasPotions()) {
      std::cout << "- Зелья: средняя длительность = "
                << getAveragePotionDuration() << "\n";
    } else {
      std::cout << "- Зелья: нет в наличии\n";
    }

    if (hasScrolls()) {
      std::cout << "- Свитки: самый частый эффект "
                << getMostCommonScrollEffect() << "\n";
    } else {
      std::cout << "- Свитки: нет в наличии\n";
    }

    std::cout << "\n";
  }
};

// Функция для чтения данных из файла
std::vector<Shop> readShopsFromFile(const std::string& filename) {
  std::vector<Shop> shops;
  std::ifstream file(filename);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
    return shops;
  }

  Shop* currentShop = nullptr;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;

    if (line.find("Магазин:") != std::string::npos) {
      // Находим название магазина после "Магазин:"
      size_t pos = line.find("Магазин:") + 8;
      std::string shopName = line.substr(pos);
      // Убираем лишние пробелы
      shopName.erase(0, shopName.find_first_not_of(" \t"));
      shopName.erase(shopName.find_last_not_of(" \t") + 1);

      shops.push_back(Shop(shopName));
      currentShop = &shops.back();
    } else if (line.find("Предметы:") != std::string::npos) {
      // Пропускаем строку с количеством предметов
      continue;
    } else if (!line.empty() && currentShop) {
      // Читаем данные предмета
      std::istringstream itemStream(line);
      std::string type, name;
      int price;
      double weight;
      std::string specParamStr;

      itemStream >> type >> name >> price >> weight >> specParamStr;

      if (type == "Оружие") {
        int damage = std::stoi(specParamStr);
        currentShop->addItem(std::unique_ptr<MagicItem>(
            new Weapon(name, price, weight, damage)));
      } else if (type == "Броня") {
        int defense = std::stoi(specParamStr);
        currentShop->addItem(std::unique_ptr<MagicItem>(
            new Armor(name, price, weight, defense)));
      } else if (type == "Зелье") {
        double duration = std::stod(specParamStr);
        currentShop->addItem(std::unique_ptr<MagicItem>(
            new Potion(name, price, weight, duration)));
      } else if (type == "Свиток") {
        currentShop->addItem(std::unique_ptr<MagicItem>(
            new Scroll(name, price, weight, specParamStr)));
      }
    }
  }

  file.close();
  return shops;
}

int main() {
  // Читаем данные из файла
  std::vector<Shop> shops = readShopsFromFile("magic_shops.txt");

  // Выводим статистику для каждого магазина
  for (const auto& shop : shops) {
    shop.printStats();
  }

  return 0;
}
