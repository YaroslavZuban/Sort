#include <iostream>
#include <stdio.h>
#include <Windows.h>

using namespace std;

const UINT buf_size = 20, N = 20, code_size = 7;

struct T1_elem // структура элемента таблицы 1
{
   UINT product_code = 0, product_count = 0; char product_name[buf_size] = "";
};

struct table // структура таблицы 1
{
   T1_elem elem[N]; UINT n = 0, count = 0;

public:

   int i = 0, j = 0; // индексы разделения таблицы для быстрой сортировки

   table(FILE* fp) // конструктор класса
   {

      char buf[buf_size] = ""; // буфер для хранения слов
      UINT size_buf = 0; // буфер для хранения шифра

      if (fp != nullptr) // ввод таблицы
      {
         fseek(fp, 0, SEEK_END); // указатель потока смещается в конец файла
         long pos = ftell(fp); // позиция конца файла

         if (pos > 0)
         {
            rewind(fp);

            fscanf_s(fp, "%d", &size_buf); // поместили шифр изделия в буфер

            while (!feof(fp) && n < N) // пока не конец файла или не дошли до конца таблицы
            {
               UINT i = 0;

               for (i; i < n && elem[i].product_code != size_buf; i++) {} // проверка есть ли шифр изделия в таблице

               if (i == n) // если шифра ещё не было
               {
                  elem[n].product_code = size_buf; // поместили шифр изделия в таблицу
                  fscanf_s(fp, "%s", buf, buf_size); // поместили название изделия в буфер
                  strcpy_s(elem[n].product_name, buf); // поместили название изделия в таблицу
                  fscanf_s(fp, "%d", &elem[n].product_count); // поместили кол-во изделий в таблицу
                  n++;
                  fscanf_s(fp, "%d", &size_buf); // поместили шифр изделия в буфер
               }
               else
               {
                  fscanf_s(fp, "%s", buf, buf_size); // пропустили названия изделия
                  fscanf_s(fp, "%d", &size_buf); // поместили кол-во изделий в буфер
                  elem[i].product_count += size_buf;
                  fscanf_s(fp, "%d", &size_buf); // поместили шифр изделия в буфер
               }
            }
         }
         else cout << "Файл пуст, заполнение таблицы невозможно.\n";
      }
      else cout << "Файл не открылся.\n";
   }

   void Quicksort(int L, int R)
   {

      if (n != 0)
      {
         if (count == 0) {
            printf("%s", "\nТаблица после быстрой сортировки:\n");
            count++;
         }

         part(L, R, &i, &j);
         if (L < j) Quicksort(L, j);
         if (i < R) Quicksort(i, R);
      }
      else cout << "Таблица пустая, быстрая сортировка невозможна.\n";
   }

   void Shell_sort()
   {
      if (n != 0)
      {
         printf("%s", "\nТаблица после сортировки Шелла:\n");

         UINT h[N], s = 0; h[s] = 1; //объявляем начальный шаг = 1

         while (h[s] <= n / 2) // пока очередной шаг меньше либо равен размерности половины таблицы
             h[++s] = 2 * h[s-1] + 1; // вычисляем следующий шаг 

         for (int m = s; m >= 0; m--) // начиная с наибольшего шага и пока не переберём все шаги 
            for (UINT k = 0; k < h[m]; k++) // перебираем все серии элементов с шагом h[m]
            {
               UINT i = h[m] + k; // индекс первого элемента в неупорядоченной части
               
               while (i <= n - 1) 
               {
                  T1_elem x = elem[i]; j = i - h[m]; // x - элемент неупорядоченной части, j - индекс элемента в упорядоченной части

                  while (j >= 0 && x.product_code < elem[j].product_code) // пока выполняется отношение порядка
                  {
                     elem[j + h[m]] = elem[j]; j -= h[m]; // переместили элемент неупорядоченной части в упорядоченную
                  }

                  elem[j + h[m]] = x; i += h[m]; // поставили элемент из буфера в таблицу
               }
            }
      }
      else cout << "Таблицая пустая, сортировка Шелла невозможна.\n";
   }

   void output()
   {
      if (n == 0) cout << "В таблице отсутствует информация.\n";
      for (UINT i = 0; i < n; i++)
      {
         printf("|%d |%-10s|%d\n",
            elem[i].product_code,
            elem[i].product_name,
            elem[i].product_count);
      }
   }

   void sortPyramid()
   {
      if (n != 0)
      {
         printf("%s", "\nТаблица после пирамидальной сортировки:\n");

         for (int k = (n - 1) / 2; k >= 0; k--)
            pyramidSort(k, n - 1);

         for (int k = n - 1; k >= 1; k--)
         {
            T1_elem x = elem[0];
            elem[0] = elem[k];
            elem[k] = x;
            pyramidSort(0, k - 1);
         }
      }
      else cout << "Таблица пустая, пирамидальная сортировка невозможна.\n";
   }

private:

   void part(int L, int R, int* ri, int* rj)
   {

      T1_elem x = elem[(L + R) / 2];
      int i = L, j = R;

      while (i <= j)
      {
         while (elem[i].product_code < x.product_code) i++;

         while (elem[j].product_code > x.product_code) j--;

         if (i <= j)
         {
            T1_elem c = elem[i]; elem[i] = elem[j]; elem[j] = c; i++; j--;
         }
      }
      *ri = i; *rj = j;
   }

   void pyramidSort(UINT start, UINT end)
   {
      while (start * 2 < end) {
         UINT index = 2 * start + 1;

         if (start * 2 + 2 <= end && elem[2 * start + 1].product_code < elem[2 * start + 2].product_code) {
            index++;
         }

         if (elem[start].product_code < elem[index].product_code) {
            T1_elem tepm = elem[start];
            elem[start] = elem[index];
            elem[index] = tepm;
            start = index;
         }
         else {
            return;
         }
      }
   }
};

struct T2_elem // структура элемента таблицы 2
{
   UINT product_code = 0, product_count = 0, product_price = 0; char product_name[buf_size] = "";
};

struct table2 // структура таблицы 2
{
   T2_elem elem[N]; UINT n = 0;

public:
   void output()
   {
      if (n == 0) cout << "В таблице отсутсвует информация.\n";
      for (UINT i = 0; i < n; i++)
      {
         printf("|%d |%-10s|%-2d|%-2d\n",
            elem[i].product_code,
            elem[i].product_name,
            elem[i].product_count,
            elem[i].product_price);
      }
   }
};

struct T3_elem // структура элемента таблицы 3
{
   UINT product_code = 0, product_cost = 0;
};

struct table3 // структура таблицы 3
{
   T3_elem elem[N]; UINT n = 0;

public:
   table3(FILE* fp)
   {
      UINT size_buf = 0; // буфер для хранения шифра, цены
      if (fp != nullptr)
      {
         fseek(fp, 0, SEEK_END); // указатель потока смещается в конец файла
         long pos = ftell(fp); // позиция конца файла
         if (pos > 0)
         {
            rewind(fp);
            fscanf_s(fp, "%d", &size_buf); // поместили шифр изделия в буфер
            while (!feof(fp) && n < N) // пока не конец файла или не дошли до конца таблицы
            {
               elem[n].product_code = size_buf; // поместили шифр изделия в таблицу
               fscanf_s(fp, "%d", &elem[n].product_cost); // поместили стоимость изделия в таблицу
               n++;
               fscanf_s(fp, "%d", &size_buf); // поместили шифр изделия в буфер
            }
         }
         else cout << endl << "Файл пуст, заполнение таблицы невозможно.\n";
      }
      else cout << "Файл не открылся\n";
   }

   void output()
   {
      if (n == 0) cout << "В таблице отсутсвует информация\n";
      for (UINT i = 0; i < n; i++)
      {
         printf("|%-2d|%-2d\n",
            elem[i].product_code,
            elem[i].product_cost);
      }
   }
};

void sequential_search(table2& table_2, table3& table_3)
{
   if (table_2.n != 0 && table_3.n != 0)
   {
      for (UINT i = 0; i < table_2.n; i++)
         for (UINT j = i; j < table_3.n; j++)
            if (table_2.elem[i].product_code == table_3.elem[j].product_code) table_2.elem[i].product_price = table_3.elem[j].product_cost;
   }
   else cout << "В одной из таблиц отсуствутют данные, заполнение значением \'цена\' невозможно.\n ";
}

void binary_search(table2& table_2, table3& table_3)
{
   if (table_2.n != 0 && table_3.n != 0)
   {
      for (UINT i = 0; i < table_3.n; i++)
      {
         UINT r = table_3.n, l = 0; bool flag = true;
         while (flag)
         {
            UINT mid = (l + r) / 2;
            if (table_3.elem[i].product_code < table_2.elem[mid].product_code) r = mid;
            else if (table_3.elem[i].product_code > table_2.elem[mid].product_code) l = mid;
            else
            {
               table_2.elem[i].product_price = table_3.elem[i].product_cost;
               flag = false;
            }
         }
      }
   }
   else cout << "В одной из таблиц отсуствутют данные, заполнение значением \'цена\' невозможно. \n";
}

int main()
{
   setlocale(LC_ALL, "Russian");
   ///  быстрая сотрировка 

   FILE* fp = nullptr;
   fopen_s(&fp, "WORK.txt", "r");
   printf("%s", "Таблица после ввода:\n");

   table table_1(fp);
   table_1.output();
   table_1.i = 0;
   table_1.j = table_1.n - 1;
   table_1.Quicksort(table_1.i, table_1.j);
   table_1.output();

   ////////////////////////////////////////////////////////

   /// Сортировка пирамидальная сортировка 

   if (fp != nullptr) rewind(fp); // переместили указатель в начало файла
   table table_P(fp);
   table_P.sortPyramid();
   table_P.output();

   ////////////////////////////////////////////////////////

   /// Сортировка Шелла сортировка 

   if (fp != nullptr) rewind(fp); // переместили указатель в начало файла
   table table_W(fp);
   table_W.Shell_sort();
   table_W.output();

   ////////////////////////////////////////////////////////

   //таблица прейскурант
   fopen_s(&fp, "PRICE.txt", "r");
   printf("%s", "\nТаблица прейскурант:\n");
   table3 table_3(fp);
   table_3.output();

   //таблица продукция
   table2 table_2;
   for (UINT i = 0; i < table_1.n; i++)
   {
      table_2.elem[i].product_code = table_1.elem[i].product_code;
      table_2.elem[i].product_count = table_1.elem[i].product_count;
      strcpy_s(table_2.elem[i].product_name, table_1.elem[i].product_name);
      table_2.n++;
   }

   // последовательный поиск
   sequential_search(table_2, table_3);

   cout << "\n последовательный поиск\n";
   table_2.output();

   table2 table_5;
   for (UINT i = 0; i < table_1.n; i++)
   {
      table_5.elem[i].product_code = table_1.elem[i].product_code;
      table_5.elem[i].product_count = table_1.elem[i].product_count;
      strcpy_s(table_5.elem[i].product_name, table_1.elem[i].product_name);
      table_5.n++;
   }

   // бинарный поиск
   cout << "\n\n бинарный поиск\n";
   binary_search(table_5, table_3);
   table_5.output();

   return 0;
}