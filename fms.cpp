#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

class account
{
	int acno;
	char name[50];
	int deposit;
	char type;
public:
	void create_account();	
	void show_account() const;	
	void modify();	
	void dep(int);	
	void draw(int);	
	void report() const;	
	int retacno() const;	
	int retdeposit() const;	
	char rettype() const;
};

void account::create_account()
{
	cout << "\nНомер счёта:";
	cin >> acno;
	cout << "\n\nНазвание счёта: ";
	cin.ignore();
	cin.getline(name, 50);
	cout << "\nТип счёта (C/S) : ";
	cin >> type;
	type = toupper(type);
	cout << "\nВведите начальную сумму: ";
	cin >> deposit;
	cout << "\n\n\n/ Счёт успешно создан /";
}

void account::show_account() const
{
	cout << "\nСчёт: " << acno;
	cout << "\nНазвание: ";
	cout << name;
	cout << "\nТип: " << type;
	cout << "\nСостояние счёта: " << deposit;
}

void account::modify()
{
	cout << "\nСчёт: " << acno;
	cout << "\nНазвание: ";
	cin.ignore();
	cin.getline(name, 50);
	cout << "\nТип: ";
	cin >> type;
	type = toupper(type);
	cout << "\nСостояние счёта: ";
	cin >> deposit;
}

void account::dep(int x)
{
	deposit += x;
}

void account::draw(int x)
{
	deposit -= x;
}

void account::report() const
{
	cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}

int account::retacno() const
{
	return acno;
}

int account::retdeposit() const
{
	return deposit;
}

char account::rettype() const
{
	return type;
}

void write_account();
void display_sp(int);
void modify_account(int);
void delete_account(int);
void display_all();		
void deposit_withdraw(int, int); 
void hello();

int main()
{
	setlocale(0, "");
	char ch;
	int num;
	hello();
	do
	{
		system("cls");
		cout << "\n\n\n\t* Вы находитесь в главном меню. *";
		cout << "\n\n\t1. Создать новый счёт";
		cout << "\n\n\t2. Пополнение";
		cout << "\n\n\t3. Вывод";
		cout << "\n\n\t4. Текущее состояние счёта";
		cout << "\n\n\t5. Счёта";
		cout << "\n\n\t6. Удаление счёта";
		cout << "\n\n\t7. Настройки счёта";
		cout << "\n\n\t8. Выйти";
		cout << "\n\n\t/ Выберите что-нибудь. (1-8) /";
		cout << "\n\n\t";
		cout << "\n\n\t* Система управления финансами *";
		cin >> ch;
		system("cls");
		switch (ch)
		{
		case '1':
			write_account();
			break;
		case '2':
			cout << "\n\n\tВведите номер счёта: "; cin >> num;
			deposit_withdraw(num, 1);
			break;
		case '3':
			cout << "\n\n\tВведите номер счёта: "; cin >> num;
			deposit_withdraw(num, 2);
			break;
		case '4':
			cout << "\n\n\tВведите номер счёта: "; cin >> num;
			display_sp(num);
			break;
		case '5':
			display_all();
			break;
		case '6':
			cout << "\n\n\tВведите номер счёта: "; cin >> num;
			delete_account(num);
			break;
		case '7':
			cout << "\n\n\tВведите номер счёта: "; cin >> num;
			modify_account(num);
			break;
		case '8':
			cout << "\n\n\t* Вы вышли с системы управления финансами. *";
			break;
		default:cout << "\a";
		}
		cin.ignore();
		cin.get();
	} while (ch != '8');
	return 0;
}

void write_account()
{
	account ac;
	ofstream outFile;
	outFile.open("account.dat", ios::binary | ios::app);
	ac.create_account();
	outFile.write(reinterpret_cast<char*> (&ac), sizeof(account));
	outFile.close();
}

void display_sp(int n)
{
	account ac;
	bool flag = false;
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "/ Файл не может быть открыт! Нажмите любую клавишу... /";
		return;
	}
	while (inFile.read(reinterpret_cast<char*> (&ac), sizeof(account)))
	{
		if (ac.retacno() == n)
		{
			ac.show_account();
			flag = true;
		}
	}
	inFile.close();
	if (flag == false)
		cout << "\n\n/ Такой номер счёта не существует. /";
}

void modify_account(int n)
{
	bool found = false;
	account ac;
	fstream File;
	File.open("account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "/ Файл не может быть открыт! Нажмите любую клавишу... /";
		return;
	}
	while (!File.eof() && found == false)
	{
		File.read(reinterpret_cast<char*> (&ac), sizeof(account));
		if (ac.retacno() == n)
		{
			ac.show_account();
			cout << "\n\nВведите новые данные учетной записи" << endl;
			ac.modify();
			int pos = (-1) * static_cast<int>(sizeof(account));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char*> (&ac), sizeof(account));
			cout << "\n\n\t(!) Учётная запись обновлена.";
			found = true;
		}
	}
	File.close();
	if (found == false)
		cout << "\n\n/ Учётная запись не найдена. /";
}

void delete_account(int n)
{
	account ac;
	ifstream inFile;
	ofstream outFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "/ Файл не может быть открыт! Нажмите любую клавишу... /";
		return;
	}
	outFile.open("Temp.dat", ios::binary);
	inFile.seekg(0, ios::beg);
	while (inFile.read(reinterpret_cast<char*> (&ac), sizeof(account)))
	{
		if (ac.retacno() != n)
		{
			outFile.write(reinterpret_cast<char*> (&ac), sizeof(account));
		}
	}
	inFile.close();
	outFile.close();
	remove("account.dat");
	rename("Temp.dat", "account.dat");
	cout << "\n\n\t/ Учётная запись удалена. /";
}

void display_all()
{
	account ac;
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "/ Файл не может быть открыт! Нажмите любую клавишу... /";
		return;
	}
	cout << "/////////////////////////////////////////////////////\n";
	cout << "/ Счёт /     / Название /         / Тип /  / Баланс /\n";
	cout << "/////////////////////////////////////////////////////\n";
	while (inFile.read(reinterpret_cast<char*> (&ac), sizeof(account)))
	{
		ac.report();
	}
	inFile.close();
}

void deposit_withdraw(int n, int option)
{
	int amt;
	bool found = false;
	account ac;
	fstream File;
	File.open("account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "/ Файл не может быть открыт! Нажмите любую клавишу... /";
		return;
	}
	while (!File.eof() && found == false)
	{
		File.read(reinterpret_cast<char*> (&ac), sizeof(account));
		if (ac.retacno() == n)
		{
			ac.show_account();
			if (option == 1)
			{
				cout << "\n\n\t/// Пополнение ///";
				cout << "\n\nВведите сумму пополнения: ";
				cin >> amt;
				ac.dep(amt);
			}
			if (option == 2)
			{
				cout << "\n\n\t/// Вывод ///";
				cout << "\n\nВведите сумму вывода: ";
				cin >> amt;
				int bal = ac.retdeposit() - amt;
				if ((bal < 500 && ac.rettype() == 'S') || (bal < 1000 && ac.rettype() == 'C'))
					cout << "/ Пополните баланс. /";
				else
					ac.draw(amt);
			}
			int pos = (-1) * static_cast<int>(sizeof(ac));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char*> (&ac), sizeof(account));
			cout << "\n\n\t (!) Учётная запись обновлена.";
			found = true;
		}
	}
	File.close();
	if (found == false)
		cout << "\n\n/ Учётная запись не найдена. /";
}

void hello()
{
	cout << "\n\n\n\t  * Добро пожаловать";
	cout << "\n\n\t  в систему управления финансами! *";
	cout << "\n\n\n\n";
	cout << "\n\nНажмите клавишу * enter *, чтобы продолжить.";
	cin.get();
}