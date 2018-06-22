#include <iostream>
#include <fstream>

using namespace std;

struct CELL{
	int value = 0;
	CELL* predecessor = NULL;
	int x;
	int y;
};

int getSmalllest(int v1, int v2, int v3){
	int smallest = v1;
	int index = 1;
	if (v2 < smallest){
		smallest = v2;
		index = 2;
	}
	if (v3 < smallest){
		smallest = v3;
		index = 3;
	}

	return index;
}

void dynamicProgramming(char* x, char* y, int m, int n){
	CELL** table = new CELL*[m + 1];
	for (int i = 0; i < m + 1; i++){
		table[i] = new CELL[n + 1];
		//初始化每一行
		for (int j = 0; j < n; j++){
			table[i][j].value = 0;
			table[i][j].x = i;
			table[i][j].y = j;
		}
		table[i][n].value = 2 * (m - i);
		table[i][n].x = i;
		table[i][n].y = n;
		//初始化最后一行
		if (i == m){
			for (int j = 0; j < n; j++) {
				table[m][j].value = 2 * (n - j);
				table[m][j].x = m;
				table[m][j].y = j;
			}
		}
	}

	//计算结果
	for (int i = m - 1; i >= 0; i--){
		for (int j = n - 1; j >= 0; j--){
			int value1 = table[i][j + 1].value + 2;
			int value2 = table[i + 1][j].value + 2;
			int penalty = 1;
			if (x[i] == y[j]){
				penalty = 0;
			}
			int value3 = table[i + 1][j + 1].value + penalty;
			//给对应的cell赋值
			int index = getSmalllest(value1, value2, value3);
			if (index == 1){
				table[i][j].predecessor = &(table[i][j + 1]);
				//table[i][j + 1].predecessor = &(table[i][j]);
				table[i][j].value = value1;
			}
			else if (index == 2){
				table[i][j].predecessor = &(table[i + 1][j]);
				//table[i + 1][j].predecessor = &(table[i][j]);
				table[i][j].value = value2;
			}
			else if (index == 3){
				table[i][j].predecessor = &(table[i + 1][j + 1]);
				//table[i + 1][j + 1].predecessor = &(table[i][j]);
				table[i][j].value = value3;
			}
		}
	}

	cout << "表格结果" << endl;
	//打印表格结果
	cout << "x/y" << "\t";
	for (int i = 0; i < n; i++){
		cout << y[i] << "\t";
	}
	cout << "-" << endl;
	for (int i = 0; i < m + 1; i++){
		if (i == m){
			cout << "-" << "\t";
		}
		else{
			cout << x[i] << "\t";
		}
		for (int j = 0; j < n + 1; j++){
			cout << table[i][j].value << "\t";
		}
		cout << endl;
	}

	//获取对齐序，从后到前
	CELL* cell = &(table[0][0]);
	int max = m > n ? m : n;
	char* sequence_x = new char[max];
	char* sequence_y = new char[max];
	int index = 0;
	cout << "Path:";
	do{
		cout << "(" << cell->x << "," << cell->y << ")->";
		CELL* cell_next = cell->predecessor;
		if (cell_next == NULL){
			break;
		}
		int x_difference = cell_next->x - cell->x;
		int y_difference = cell_next->y - cell->y;
		//将序列记录下来
		if (x_difference == 1 && y_difference == 1){
			sequence_x[index] = x[cell->x];
			sequence_y[index] = y[cell->y];
		}
		else if (x_difference == 1 && y_difference == 0){
			sequence_x[index] = x[cell->x];
			sequence_y[index] = '-';
		}
		else if (x_difference == 0 && y_difference == 1){
			sequence_x[index] = '-';
			sequence_y[index] = y[cell->y];
		}
		index++;
		cell = cell_next;
	} while (cell != NULL);
	cout << endl << "DNA序列对齐结果" << endl;

	sequence_x[max - 1] = '\0';
	sequence_y[max - 1] = '\0';
	cout << sequence_x << endl;
	cout << sequence_y << endl;
}

int divideAndConquer(char* x, char* y, int i, int j, int m, int n){
	if (i == m && j == n){
		return 0;
	}
	else if (i == m && j != n){
		return 2 * (n - j);
	}
	else if (i != m && j == n){
		return 2 * (m - i);
	}

	int value1 = divideAndConquer(x, y, i + 1, j, m, n) + 2;
	int value2 = divideAndConquer(x, y, i, j + 1, m, n) + 2;
	int penalty = 1;
	if (x[i] == y[j]){
		penalty = 0;
	}
	int value3 = divideAndConquer(x, y, i + 1, j + 1, m, n) + penalty;
	
	int index = getSmalllest(value1, value2, value3);
	if (index == 1){
		return value1;
	}
	else if (index == 2){
		return value2;
	}
	else if (index == 3){
		return value3;
	}
}

int main() {
	//char x[20] = "TAAGGTCA";
	//char y[20] = "AACAGTTACC";
	char x[20] = "";
	char y[20] = "";

	cout << "X sequence: ";
	cin.getline(x, 20);
	cout << "Y sequence: ";
	cin.getline(y, 20);

	cout << x << ": " << strlen(x) << endl;
	cout << y << ": " << strlen(y) << endl;

	size_t m = strlen(x);
	size_t n = strlen(y);
	
	//dynamicProgramming(x, y, m, n);
	cout << divideAndConquer(x, y, 0, 0, m, n);

	system("pause");
	return 0;
}