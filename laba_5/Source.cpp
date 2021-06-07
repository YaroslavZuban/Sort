#include <iostream>
#include <stdio.h>
#include <Windows.h>

using namespace std;

const UINT buf_size = 20, N = 20, code_size = 7;

struct T1_elem // ��������� �������� ������� 1
{
   UINT product_code = 0, product_count = 0; char product_name[buf_size] = "";
};

struct table // ��������� ������� 1
{
   T1_elem elem[N]; UINT n = 0, count = 0;

public:

   int i = 0, j = 0; // ������� ���������� ������� ��� ������� ����������

   table(FILE* fp) // ����������� ������
   {

      char buf[buf_size] = ""; // ����� ��� �������� ����
      UINT size_buf = 0; // ����� ��� �������� �����

      if (fp != nullptr) // ���� �������
      {
         fseek(fp, 0, SEEK_END); // ��������� ������ ��������� � ����� �����
         long pos = ftell(fp); // ������� ����� �����

         if (pos > 0)
         {
            rewind(fp);

            fscanf_s(fp, "%d", &size_buf); // ��������� ���� ������� � �����

            while (!feof(fp) && n < N) // ���� �� ����� ����� ��� �� ����� �� ����� �������
            {
               UINT i = 0;

               for (i; i < n && elem[i].product_code != size_buf; i++) {} // �������� ���� �� ���� ������� � �������

               if (i == n) // ���� ����� ��� �� ����
               {
                  elem[n].product_code = size_buf; // ��������� ���� ������� � �������
                  fscanf_s(fp, "%s", buf, buf_size); // ��������� �������� ������� � �����
                  strcpy_s(elem[n].product_name, buf); // ��������� �������� ������� � �������
                  fscanf_s(fp, "%d", &elem[n].product_count); // ��������� ���-�� ������� � �������
                  n++;
                  fscanf_s(fp, "%d", &size_buf); // ��������� ���� ������� � �����
               }
               else
               {
                  fscanf_s(fp, "%s", buf, buf_size); // ���������� �������� �������
                  fscanf_s(fp, "%d", &size_buf); // ��������� ���-�� ������� � �����
                  elem[i].product_count += size_buf;
                  fscanf_s(fp, "%d", &size_buf); // ��������� ���� ������� � �����
               }
            }
         }
         else cout << "���� ����, ���������� ������� ����������.\n";
      }
      else cout << "���� �� ��������.\n";
   }

   void Quicksort(int L, int R)
   {

      if (n != 0)
      {
         if (count == 0) {
            printf("%s", "\n������� ����� ������� ����������:\n");
            count++;
         }

         part(L, R, &i, &j);
         if (L < j) Quicksort(L, j);
         if (i < R) Quicksort(i, R);
      }
      else cout << "������� ������, ������� ���������� ����������.\n";
   }

   void Shell_sort()
   {
      if (n != 0)
      {
         printf("%s", "\n������� ����� ���������� �����:\n");

         UINT h[N], s = 0; h[s] = 1; //��������� ��������� ��� = 1

         while (h[s] <= n / 2) // ���� ��������� ��� ������ ���� ����� ����������� �������� �������
             h[++s] = 2 * h[s-1] + 1; // ��������� ��������� ��� 

         for (int m = s; m >= 0; m--) // ������� � ����������� ���� � ���� �� �������� ��� ���� 
            for (UINT k = 0; k < h[m]; k++) // ���������� ��� ����� ��������� � ����� h[m]
            {
               UINT i = h[m] + k; // ������ ������� �������� � ��������������� �����
               
               while (i <= n - 1) 
               {
                  T1_elem x = elem[i]; j = i - h[m]; // x - ������� ��������������� �����, j - ������ �������� � ������������� �����

                  while (j >= 0 && x.product_code < elem[j].product_code) // ���� ����������� ��������� �������
                  {
                     elem[j + h[m]] = elem[j]; j -= h[m]; // ����������� ������� ��������������� ����� � �������������
                  }

                  elem[j + h[m]] = x; i += h[m]; // ��������� ������� �� ������ � �������
               }
            }
      }
      else cout << "�������� ������, ���������� ����� ����������.\n";
   }

   void output()
   {
      if (n == 0) cout << "� ������� ����������� ����������.\n";
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
         printf("%s", "\n������� ����� ������������� ����������:\n");

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
      else cout << "������� ������, ������������� ���������� ����������.\n";
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

struct T2_elem // ��������� �������� ������� 2
{
   UINT product_code = 0, product_count = 0, product_price = 0; char product_name[buf_size] = "";
};

struct table2 // ��������� ������� 2
{
   T2_elem elem[N]; UINT n = 0;

public:
   void output()
   {
      if (n == 0) cout << "� ������� ���������� ����������.\n";
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

struct T3_elem // ��������� �������� ������� 3
{
   UINT product_code = 0, product_cost = 0;
};

struct table3 // ��������� ������� 3
{
   T3_elem elem[N]; UINT n = 0;

public:
   table3(FILE* fp)
   {
      UINT size_buf = 0; // ����� ��� �������� �����, ����
      if (fp != nullptr)
      {
         fseek(fp, 0, SEEK_END); // ��������� ������ ��������� � ����� �����
         long pos = ftell(fp); // ������� ����� �����
         if (pos > 0)
         {
            rewind(fp);
            fscanf_s(fp, "%d", &size_buf); // ��������� ���� ������� � �����
            while (!feof(fp) && n < N) // ���� �� ����� ����� ��� �� ����� �� ����� �������
            {
               elem[n].product_code = size_buf; // ��������� ���� ������� � �������
               fscanf_s(fp, "%d", &elem[n].product_cost); // ��������� ��������� ������� � �������
               n++;
               fscanf_s(fp, "%d", &size_buf); // ��������� ���� ������� � �����
            }
         }
         else cout << endl << "���� ����, ���������� ������� ����������.\n";
      }
      else cout << "���� �� ��������\n";
   }

   void output()
   {
      if (n == 0) cout << "� ������� ���������� ����������\n";
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
   else cout << "� ����� �� ������ ����������� ������, ���������� ��������� \'����\' ����������.\n ";
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
   else cout << "� ����� �� ������ ����������� ������, ���������� ��������� \'����\' ����������. \n";
}

int main()
{
   setlocale(LC_ALL, "Russian");
   ///  ������� ���������� 

   FILE* fp = nullptr;
   fopen_s(&fp, "WORK.txt", "r");
   printf("%s", "������� ����� �����:\n");

   table table_1(fp);
   table_1.output();
   table_1.i = 0;
   table_1.j = table_1.n - 1;
   table_1.Quicksort(table_1.i, table_1.j);
   table_1.output();

   ////////////////////////////////////////////////////////

   /// ���������� ������������� ���������� 

   if (fp != nullptr) rewind(fp); // ����������� ��������� � ������ �����
   table table_P(fp);
   table_P.sortPyramid();
   table_P.output();

   ////////////////////////////////////////////////////////

   /// ���������� ����� ���������� 

   if (fp != nullptr) rewind(fp); // ����������� ��������� � ������ �����
   table table_W(fp);
   table_W.Shell_sort();
   table_W.output();

   ////////////////////////////////////////////////////////

   //������� �����������
   fopen_s(&fp, "PRICE.txt", "r");
   printf("%s", "\n������� �����������:\n");
   table3 table_3(fp);
   table_3.output();

   //������� ���������
   table2 table_2;
   for (UINT i = 0; i < table_1.n; i++)
   {
      table_2.elem[i].product_code = table_1.elem[i].product_code;
      table_2.elem[i].product_count = table_1.elem[i].product_count;
      strcpy_s(table_2.elem[i].product_name, table_1.elem[i].product_name);
      table_2.n++;
   }

   // ���������������� �����
   sequential_search(table_2, table_3);

   cout << "\n ���������������� �����\n";
   table_2.output();

   table2 table_5;
   for (UINT i = 0; i < table_1.n; i++)
   {
      table_5.elem[i].product_code = table_1.elem[i].product_code;
      table_5.elem[i].product_count = table_1.elem[i].product_count;
      strcpy_s(table_5.elem[i].product_name, table_1.elem[i].product_name);
      table_5.n++;
   }

   // �������� �����
   cout << "\n\n �������� �����\n";
   binary_search(table_5, table_3);
   table_5.output();

   return 0;
}