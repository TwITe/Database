# База данных

Это моя собственная простейшая имплементация базы данных, реализованная в виде библиотеки.
Также, к проекту прилагаются уже готовые тесты, использующие *framework* ***Catch***, который также включен в проект.
Особенностью является то, что база данных кроссплатформенна.

## Установка

Загрузите данный репозиторий с GitHub:
 [База данных](https://github.com/TwITe/Database/archive/master.zip)

## Начало работы

Инициализируйте базу данных, установив путь сохранения файлов и размер файлов с данными

 **Linux**:
 ```c++
 initialize_db("/home/user_name/CLionProjects/Database/data_files/", 20)
 ```
 **Windows**:
  ```c++
  initialize_db("c:\\users\\user_name\\documents\\visual studio 2017\\projects\\database\\data_files\\", 20)
  ```

###### **_Значение, выставленное для размера файлов с данными, по умолчанию составляет 8388608 байт (8 МБ)_**

## Использование

В библиотеке реализованы различные хелперы, помогающие хранить и загружать данные.

---

**Список хелперов для хранения данных**
- store_helper(id, data)

---

**Список хелперов для загрузки данных**
- load_helper<int>(id)
- load_helper<double>(id)
- load_helper<string>(id)
- load_int_vector_helper(id)
- load_double_vector_helper(id)
- load_string_vector_helper(id)

**Важно!**
Если вы хотите хранить новые данные с id, который уже содержится в базе данных, перед этим требуется удалить все
записи о ранее записанном id с данными. Проделать это можно, вызвав функцию удаления

  ```c++
  delete_id(id)
  ```

Пример программы, которая инициализирует базу данных, сохраняет данные разных типов, удаляет некоторые из них и загружает (все действия выполняются на Linux):

  ```c++
  int main() {
      initialize_db("/home/database/data_files/", 25);
      int a = 10;
      store_helper(1, a);
      cout << load_helper<int>(1);
      string b = "database test";
      store_helper(2, b);
      cout << load_helper<string>(2) << endl;
      delete_id(2);
      b = "string was saved with the same id";
      store_helper(2, b);
      cout << load_helper<string>(2) << endl;
      vector <int> c;
      for (int i = 100; i > 0; i-= 10) {
          c.push_back(i / 2);
      }
      store_helper(3, c);
      vector <int> loaded_vector = load_int_vector_helper(3);
      for (auto i: c) {
          cout << i << " ";
      }
  }
  ```

  ***Вывод:***
  ```
   10
   database test
   string was saved with the same id as earlier
   50 45 40 35 30 25 20 15 10 5
  ```
