# include <iostream>
# include <conio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <cstdlib> 
# include <fstream>
# include <string>
# include <ctime>
# include <time.h>       /* time */
# include <chrono>

# define CLOCKS_PER_SEC  1000
# define Number_range 10 
# define TOT_FRAMES 500
# define FRAMES_SEND 10

using namespace std;
class selective {
private:
	int bits_num;
	int window_num;
	int flag_temp;
	int flag;
	int lost_bits;
	int deliver_bits;
	int all_number_sent_bits;
	float* Time_Array;
	int * window_lost;
	string* main_array;
	string* recive_array;

	fstream input_array;
	fstream send_list;
	fstream recive_list1;
	fstream recive_list2;

public:

	selective(int, int);
	void send_function();
	bool recive_check();
	void send_recive_engine();
	void read_from_file();
	void get_result();
	void get_array();
	void print(char, char,char);
};




int main()
{

	selective first_test(20, 5);
	first_test.send_function();
	first_test.get_result();

	_getch();
	int a;
	cin >> a;
}

selective::selective(int bits, int window)
{
	flag = 0;
	lost_bits = 0;
	bits_num = bits;
	deliver_bits = 0;
	all_number_sent_bits = 0;
	window_num = window;
	window_lost = new int[window];
	main_array = new string[bits_num];
	recive_array = new string[bits_num];

	Time_Array = new float[window_num];

	input_array.open("main_input.txt");
	read_from_file();

	input_array.close();
}

void selective::send_function()
{
	bool check1;
	bool check2;
	bool check3;
	send_list.open("send_list.txt");
	recive_list1.open("recive_list1.txt");
	recive_list2.open("recive_list2.txt");
	if (bits_num - flag > window_num) {
		for (int i = 0; i < window_num; i++) {

			float time;
			check1 = recive_check();
			auto current_time = chrono::system_clock::now();
			time = chrono::duration<float>(chrono::system_clock::now() - current_time).count();
			Time_Array[i] = time;
			if (check1)
				send_list << 1 << endl;
			else {
				send_list << 0 << endl;
				lost_bits++;
			}
			check2 = recive_check();
			if (check2) {
				recive_list1 << 1 << endl;
				all_number_sent_bits++;
			}
			if (!check2) {
				
				recive_list1 << 0 << endl;
				lost_bits++;
			}
			check3 = recive_check();
			if (check3) {
				recive_list2 << 1 << endl;
				all_number_sent_bits++;
			}
			if (!check3) {
				
				recive_list2 << 0 << endl;
				lost_bits++;
			}
		}
	}

	if (bits_num - flag <= window_num)
	{

		for (int i = 0; i < bits_num - flag; i++) {
			float time;
			check1 = recive_check();
			auto current_time = chrono::system_clock::now();
			time = chrono::duration<float>(chrono::system_clock::now() - current_time).count();
			Time_Array[i] = time;
			if (check1)
				send_list << 1 << endl;
			else {
				send_list << 0 << endl;
				lost_bits++;
			}
			bool check2 = recive_check();
			if (check2) {
				recive_list1 << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;

			}
			if (!check2) {
				recive_list1 << 0 << endl;
				lost_bits++;
			}
			check3 = recive_check();
			if (check3) {
				recive_list2 << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;
				all_number_sent_bits++;
			}
			if (!check3) {
				recive_list2 << 0 << endl;
				lost_bits++;
			}
		}
	}


	send_list.close();
	recive_list1.close();
	recive_list2.close();
	send_recive_engine();


}

bool selective::recive_check()
{
	if (rand() % 5 == 0)
		return false;
	return true;
}

void selective::send_recive_engine()
{
	send_list.open("send_list.txt");
	recive_list1.open("recive_list1.txt");
	recive_list2.open("recive_list2.txt");
	char send_item;
	char recive_item1;
	char recive_item2;

	for (int i = 0; i < window_num; i++) {
		if (flag == bits_num)
			return;

		send_list >> send_item;
		recive_list1 >> recive_item1;
		recive_list2 >> recive_item2;
		
		print(send_item, recive_item1, recive_item2);
		if (recive_item1 == '1' && recive_item2 == '1' && send_item == '1') {
			recive_array[flag] = main_array[flag];
			flag++;
			deliver_bits++;
			
		}
		if (recive_item1 != '1' || recive_item2 != '1' || send_item != '1') {
			bool pass = true;
			float time;
			char r1, r2;
			char s;
			bool check1;
			bool check2;
			bool check3;
			while (pass) {
				check1 = recive_check();
				if (check1) {
					s = '1';
				}
				else {
					s = '0';
					lost_bits++;
				}
				check2 = recive_check();
				if (check2) {
					r1 = '1';
					all_number_sent_bits++;
				}
				if (!check2) {
					r1 = '0';
					lost_bits++;
				}
				check3 = recive_check();
				if (check3) {
					r2 = '1';
					all_number_sent_bits++;
				}
				if (!check3) {
					r2 = '0';
					lost_bits++;
				}
				if (s == '1' && r1 == '1' && r2 == '1') {
					pass = false;
					recive_array[flag] = main_array[flag];
					print(s, r1, r2);
					recive_array[flag] = main_array[flag];
					flag++;
					deliver_bits++;
				}
				else {
					print(s, r1, r2);
					lost_bits++;
				}
			}		
		}
	}

	send_list.close();
	recive_list1.close();
	recive_list2.close();
	send_function();
}


void selective::read_from_file()
{
	string temp;
	input_array.open("input_array.txt");
	for (int i = 0; i < bits_num; i++) {
		input_array >> temp;
		main_array[i] = temp;
		temp = ' ';
	}

}

void selective::get_result()
{
	cout << "the number of lost bits is: " << lost_bits << endl;
	cout << "the number of deliverd bits is: " << deliver_bits << endl;
}

void selective::get_array()
{
	for (int i = 0; i < bits_num; i++)
		cout << main_array[i] << endl;
}
void selective::print(char send_item, char recive_item1, char recive_item2)
{
	cout << "\ncurrent data: " << flag << "\n";
	if (send_item == '1')
		cout << " deliverd frame " << "\n";
	else if (send_item != '1')
		cout << " doesnt deliverd frame!!! " << "\n";
	if (recive_item1 == '1')
		cout << " Arrival Ack1  " << "\n";
	else if (recive_item1 != '1')
		cout << " doesnt Arrival Ack1 !!! " << "\n";
	if (recive_item2 == '1')
		cout << " Arrival Ack2  " << "\n";
	else if (recive_item2 != '1')
		cout << " doesnt Arrival Ack2 !!! " << "\n";
}