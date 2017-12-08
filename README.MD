# База данных

Это простейшая имплементация базы данных, реализованная в виде библиотеки.
К проекту прилагаются уже готовые тесты, использующие framework *Catch*, который также включен в проект.
Особенностью является то, что база данных полностью кроссплатформенна.

## Установка

Клонируйте данный репозиторий с **GitHub**:

```
mkdir ~/database
cd ~/database
git clone https://github.com/TwITe/Database.git
```

## Быстрый старт

Сохранение строки
  ```c++
  #include "database.h"
  using namespace std;

  int main() {
      initialize_db("/home/database/data_files/", 25);

      store_helper(2, "hello world!");

      cout << load_helper<string>(2); //hello world!
  }
  ```

Сохранение вектора типа **int**
  ```c++
  #include "database.h"
  using namespace std;

  int main() {
      initialize_db("/home/database/data_files/", 25);

      vector <int> test;
      for (int i = 100; i > 0; i-= 10) {
          test.push_back(i / 2);
      }

      store_helper(3, c);

      vector <int> loaded_vector = load_int_vector_helper(3);
      for (auto i: loaded_vector) {
          cout << i << " "; //50 45 40 35 30 25 20 15 10 5
      }
  }
  ```

Удаление существующего id и сохранение новых данных

```c++
#include "database.h"
using namespace std;

int main() {
    initialize_db("/home/twite/CLionProjects/Database/data_files/", 25);

    string s = "database test";

    store_helper(2, s);
    cout << load_helper<string>(2) << endl; //database test

    delete_id(2);

    s = "string was saved with the same id as earlier";

    store_helper(2, s);
    cout << load_helper<string>(2); //string was saved with the same id as earlier
}
```

## Использование

Инициализируйте базу данных, установив путь сохранения файлов и размер файлов с данными

 **Linux**:
 ```c++
 initialize_db("/home/user_name/CLionProjects/Database/data_files/", 20)
 ```
 **Windows**:
  ```c++
  initialize_db("c:\\users\\user_name\\documents\\visual studio 2017\\projects\\database\\data_files\\", 20)
  ```

---

**API базы данных:**

*Базовые функции:*

Сохранение данных типа ***void\****
  ```c++
  bool store(int id, void* data, size_t data_size)
  ```

Загрузка данных типа ***void\**** по указанному id
   ```c++
 void* load(int id)
```

Удаление данных по указанному id из базы данных
  ```c++
  void delete_id(int id)
  ```

---

*Список хелперов для сохранения данных:*

Сохранение данных типов **_int_, _double_, _string_, vector \<_int_>\, vector \<_double_>\, vector \<_string_>**
```c++
store_helper(int id, <T> data)
```

---

*Список хелперов для загрузки данных:*

*Загрузка данных типа **int***
 ```c++
load_helper<int>(int id)
```

*Загрузка данных типа **double***
 ```c++
load_helper<double>(int id)
```

*Загрузка данных типа **string***
 ```c++
load_helper<string>(int id)
```

*Загрузка вектора типа **int***
 ```c++
load_int_vector_helper(int id)
```

*Загрузка вектора типа **double***
 ```c++
load_double_vector_helper(int id)
```

*Загрузка вектора типа **string***
 ```c++
load_string_vector_helper(int id)
```