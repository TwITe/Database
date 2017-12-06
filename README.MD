# База данных

Это простейшая имплементация базы данных, реализованная в виде библиотеки.
К проекту прилагаются уже готовые тесты, использующие *framework* ***Catch***, который также включен в проект.
Особенностью является то, что база данных полностью кроссплатформенна.

## Установка

Клонируйте данный репозиторий с GitHub:

```
mkdir ~/database
cd ~/database
git clone https://github.com/TwITe/Database.git
```

## Быстрый старт

  ```c++
  #include "database.h"
  using namespace std;

  int main() {
      initialize_db("/home/database/data_files/", 25);

      store_helper(2, "hello world!");

      cout << load_helper<string>(2) << endl;
  }
  ```

  ***Вывод:***
  ```
   hello world!
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

**API базы данных:**

- bool store(int id, void* data, size_t data_size) - сохранение данных типа void*
- void* load(int id) - загрузка данных типа void* по указанному id
- void delete_id(int id) - удаление данных по указанному id из базы данных

---

**Список хелперов для хранения данных**
- ```c++store_helper(int id, <T> data)``` - хранение данных типов int, double, string, vector \<int>\, vector \<double>\, vector \<string>\

---

**Список хелперов для загрузки данных**
- ```c++load_helper\<int>\(id)``` - загрузка данных типа int
- ```c++load_helper\<double>\(id)``` -  - загрузка данных типа double
- ```c++load_helper\<string>\(id)``` - загрузка данных типа string
- ```c++load_int_vector_helper(id)``` - загрузка вектора типа int
- ```c++load_double_vector_helper(id)``` - загрузка вектора типа double
- ```c++load_string_vector_helper(id)``` - загрузка вектора типа string
