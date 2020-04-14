#include <iostream>
#include <omp.h>
using namespace std;

//  a * b 
int * mat_mul(int *a, int *b, int a_row, int a_col, int b_row, int b_col) {
	if (a_col != b_row) {
		return NULL;
	}

	int* answer = new int[a_row * b_col];
	int i, j, k;
	#pragma omp parallel for
	for (i = 0; i < a_row; i++) {
		#pragma omp parallel for
		for (j = 0; j < a_col; j++)
			answer[i * a_row + j] = 0;
		#pragma omp parallel for
		for (j = 0; j < a_col; j++) {
			#pragma omp parallel for
			for (k = 0; k < b_col; k++) {
				answer[i * a_row + k] += a[i * a_col + j] * b[j * b_col + i];
			}
		}

	}
	/*
	for (int m = 0; m < a_row; m++) {
		for (int s = 0; s < b_col; s++) {
			answer[m * a_row + s] = 0;
			for (int n = 0; n < a_col; n++) {
				answer[m * a_row + s] += a[m * a_row + n] * b[n * a_col + s];

			}
		}
	}*/
	// Êä³ö¾ØÕó½á¹û£¬
	for (int m = 0; m < a_row; m++) {
		for (int s = 0; s < b_col; s++) {
			cout << answer[m * a_row + s] << "\t";
		}
		cout << endl;

	}
	return answer;



}

int main() {
	int a[9] = { 2,2,2, 2,2,2, 2,2,2 };
	int b[9] = { 1,1,1, 1,1,1, 1,1,1 };

	int *answer = mat_mul(a, b, 3, 3, 3, 3);


}