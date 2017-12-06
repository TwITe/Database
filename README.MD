# База данных

Это простейшая имплементация базы данных, реализованная в виде библиотеки.
К проекту прилагаются уже готовые тесты, использующие *framework* ***Catch***, который также включен в проект.
Особенностью является то, что база данных полностью кроссплатформенна.

## Установка

Клонируйте данный репозиторий с GitHub:

    ```
    \mkdir ~/database\
    
    \cd ~/database\
    
    \git clone https://github.com/TwITe/Database.git\
    ```
    
## Быстрый старт

  ```c++
  int main() {
      initialize_db("/home/database/data_files/", 25);
      string b = "database test";
      store_helper(2, b);
      cout << load_helper<string>(2) << endl;
  }
  ```

  ***Вывод:***
  ```
   database test
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

- store(data) - сохранение данных типа void*
- load(id) - загрузка данных типа void* по указанному id

---

**Список хелперов для хранения данных**
- store_helper(id, data)

---

**Список хелперов для загрузки данных**
- load_helper\<int>\(id)
- load_helper\<double>\(id)
- load_helper\<string>\(id)
- load_int_vector_helper(id)
- load_double_vector_helper(id)
- load_string_vector_helper(id)
