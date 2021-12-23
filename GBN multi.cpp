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

class go_backn {
private:
	int bits_num;
	int window_num;

	int flag;
	int lost_bits;
	int deliver_bits;
	int all_number_sent_bits;
	float* Time_Array;

	string* main_array;
	string* recive_array;

	fstream input_array;
	fstream send_list;
	fstream recive_list1;
	fstream recive_list2;


public:
	go_backn(int, int);
	void send_function();
	bool recive_check();
	void send_recive_engine();
	void remove_line(int);
	void read_from_file();
	void get_result();
	void get_array();
	void print(char, char, char);
};

go_backn::go_backn(int bits, int window)
{
	flag = 0;
	lost_bits = 0;
	bits_num = bits;
	deliver_bits = 0;
	all_number_sent_bits = 0;
	window_num = window;
	main_array = new string[bits_num];
	recive_array = new string[bits_num];

	Time_Array = new float[window_num];

	input_array.open("main_input.txt");
	read_from_file();

	input_array.close();
}

void go_backn::send_function()
{

	send_list.open("send_list.txt");
	recive_list1.open("recive_list1.txt");
	recive_list2.open("recive_list2.txt");
	bool check;
	bool check1;
	bool check2;
	if (bits_num - flag > window_num) {
		for (int i = 0; i < window_num; i++) {
			float time;

			auto current_time = chrono::system_clock::now();
			time = chrono::duration<float>(chrono::system_clock::now() - current_time).count();
			Time_Array[i] = time;
			check = recive_check();
			if (check)
				send_list << 1 << endl;
			else {
				send_list << 0 << endl;
				lost_bits++;
				
			}
			check1 = recive_check();
			check2 = recive_check();

			if (check1) {
				recive_list1 << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;
				all_number_sent_bits++;
			}
			if (!check1) {
				recive_list1 << 0 << endl;
				
				lost_bits++;
			}
			if (check2) {
				recive_list2 << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;
				all_number_sent_bits++;
			}
			if (!check2) {
				recive_list2 << 0 << endl;
				
				lost_bits++;
			}


		}
	}

	if (bits_num - flag <= window_num)
	{

		for (int i = 0; i < bits_num - flag; i++) {
			float time;

			auto current_time = chrono::system_clock::now();
			time = chrono::duration<float>(chrono::system_clock::now() - current_time).count();
			Time_Array[i] = time;
			check = recive_check();
			if (check)
				send_list << 1 << endl;
			else {
				send_list << 0 << endl;
				
				lost_bits++;
			}
			check1 = recive_check();
			check2 = recive_check();

			if (check1) {
				recive_list1 << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;

			}
			if (!check1) {
				recive_list1 << 0 << endl;
				
				lost_bits++;
			}
			if (check2) {
				recive_list2 << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;

			}
			if (!check2) {
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

bool go_backn::recive_check()
{
	if (rand() % 10 == 0)
		return false;
	return true;
}


void go_backn::send_recive_engine()
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
		if (recive_item1 != '1' || recive_item2 != '1' || send_item != '1')
		{
			send_list.close();
			recive_list1.close();
			recive_list2.close();

			send_function();

		}

	}
	send_list.close();
	recive_list1.close();
	recive_list2.close();
	send_function();
}
void go_backn::remove_line(int input)
{
	fstream main_file;
	main_file.open("input_array.txt");

	ofstream temp_file;
	temp_file.open("temp.txt");

	string temp;

	for (int i = 0; i < 11; i++) {
		main_file >> temp;
		if (i != input) {
			temp_file << temp << endl;
		}

	}

	remove("input_array.txt");
	rename("temp.txt", "input_array.txt"); //this line dosnt work carefully!
}

void go_backn::read_from_file()
{
	string temp;
	input_array.open("input_array.txt");
	for (int i = 0; i < bits_num; i++) {
		input_array >> temp;
		main_array[i] = temp;
		temp = ' ';
	}


}

void go_backn::get_result()
{
	cout << "the number of lost bits is: " << lost_bits << endl;
	cout << "the number of deliverd bits is: " << deliver_bits << endl;

}

void go_backn::get_array()
{
	for (int i = 0; i < bits_num; i++)
		cout << main_array[i] << endl;
}

void go_backn::print(char send_item, char recive_item1, char recive_item2)
{
	cout << "current data: " << flag << "\n";
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
		cout << " doesnt Arrival Ack2 !!! " << "\n\n\n";
}

int main()
{

	go_backn first_test(20, 5);
	first_test.send_function();
	first_test.get_result();

	_getch();

	int a;
	cin >> a;
}