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
	fstream recive_list;


public:
	go_backn(int, int);
	void send_function();
	bool recive_check();
	void send_recive_engine();
	void remove_line(int);
	void read_from_file();
	void get_result();
	void get_array();
	void print(char, char);
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
	recive_list.open("recive_list.txt");

	if (bits_num - flag > window_num) {
		for (int i = 0; i < window_num; i++) {
			float time;
			bool check = recive_check();
			auto current_time = chrono::system_clock::now();
			time = chrono::duration<float>(chrono::system_clock::now() - current_time).count();
			Time_Array[i] = time;
			if (check)
				send_list << 1 << endl;
			else
				send_list << 0 << endl;
			check = recive_check();
			if (check) {
				recive_list << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;
				all_number_sent_bits++;
			}
			else if (!check) {
				recive_list << 0 << endl;
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
			send_list << 1 << endl;
			bool check = recive_check();
			if (check) {
				recive_list << 1 << endl;
				Time_Array[i] = (chrono::duration<float>(chrono::system_clock::now() - current_time).count()) * 1000;

			}
			else if (!check) {
				recive_list << 0 << endl;
				lost_bits++;
			}
		}
	}


	send_list.close();
	recive_list.close();
	send_recive_engine();


}

bool go_backn::recive_check()
{
	if (rand() % 5 == 0)
		return false;
	return true;
}


void go_backn::send_recive_engine()
{

	send_list.open("send_list.txt");
	recive_list.open("recive_list.txt");

	for (int i = 0; i < window_num; i++) {
		char send_item;
		char recive_item;
		if (flag == bits_num)
			return;
		send_list >> send_item;
		recive_list >> recive_item;

		if (recive_item == '1' && send_item == '1') {
			print(send_item, recive_item);
			recive_array[flag] = main_array[flag];
			flag++;
			deliver_bits++;
		}
		else
		{
			print(send_item, recive_item);
			send_list.close();
			recive_list.close();
			send_function();

		}

	}
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

void go_backn::print(char send_item, char recive_item)
{
	cout << "current data: " << flag << "\n";
	if (send_item == '1')
		cout << " deliverd frame " << "\n";
	else if (send_item != '1')
		cout << " doesnt deliverd frame!!! " << "\n";
	if (recive_item == '1')
		cout << " Arrival Ack  " << "\n\n\n";
	else if (recive_item != '1')
		cout << " doesnt Arrival Ack !!! " << "\n\n\n";
}

int main()
{

	go_backn first_test(100, 5);
	first_test.send_function();
	first_test.get_result();


	

	_getch();

	int a;
	cin >> a;
}
