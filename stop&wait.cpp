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

class Stop_wait {
private:
	int bits_num;

	int flag;
	int lost_bits;
	int deliver_bits;
	float Time_check;
	string* main_array;
	fstream input_array;

public:
	Stop_wait(int);
	void send_function();

	bool recive_check();
	void remove_line(int);
	void read_from_file();
	void get_result();
	void get_array();
	void sleeping(int);
	bool send_check();
};

Stop_wait::Stop_wait(int bits)
{
	flag = 0;
	lost_bits = 0;
	bits_num = bits;
	deliver_bits = 0;
	main_array = new string[bits_num];
	input_array.open("main_input.txt");
	read_from_file();
	input_array.close();
}

void Stop_wait::send_function()
{
	for (int i = 0; i < bits_num; i++) {


		string current_data = main_array[i];

		//auto current_time = chrono::system_clock::now();
		//time = chrono::duration<float>(chrono::system_clock::now() - current_time).count();

		bool pass = true;
		while (pass) {

			bool check_send = send_check();
			if (check_send)
				cout << current_data << "  sent successfuly" << endl;
			else {
				cout << current_data << "  send failed " << endl;
				lost_bits++;
			}

			if (check_send) {

				float time;
				float main_time = 0;
				auto current_time = chrono::system_clock::now();

				sleeping(1);

				bool check = recive_check();

				if (check) {
					time = (chrono::duration<float>(chrono::system_clock::now() - current_time).count());
					deliver_bits++;
					flag++;
					cout << current_data << "  recived successfuly" << endl;
					pass = false;

				}
				else if (!check) {
					lost_bits++;
					cout << current_data << "  recived failed !!! " << endl;
					pass = true;
					sleeping(0.1);
					time = (chrono::duration<float>(chrono::system_clock::now() - current_time).count());
				}
				time++;
				if (main_time > 1)
					pass = true;
				cout << endl;
			}

		}



	}
}

bool Stop_wait::recive_check()
{
	if (rand() % 10 == 0)
		return false;
	return true;
}

void Stop_wait::remove_line(int input)
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

void Stop_wait::read_from_file()
{
	string temp;
	input_array.open("input_array.txt");
	for (int i = 0; i < bits_num; i++) {
		input_array >> temp;
		main_array[i] = temp;
		temp = ' ';
	}


}

void Stop_wait::get_result()
{
	cout << "the number of lost bits is: " << lost_bits << endl;
	cout << "the number of deliverd bits is: " << deliver_bits << endl;

}

void Stop_wait::get_array()
{
	for (int i = 0; i < bits_num; i++)
		cout << main_array[i] << endl;
}

void Stop_wait::sleeping(int seconds)
{
	clock_t endwait;
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

bool Stop_wait::send_check()
{
	if (rand() % 5 == 0)
		return false;
	return true;
}




int main()
{

	Stop_wait first_test(40);
	//first_test.get_array();
	first_test.send_function();
	first_test.get_result();


	_getch();

}






