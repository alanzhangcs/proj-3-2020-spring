#include <iostream>

using namespace std;



//  a * b 
int * mat_mul(int *a, int *b, int a_row, int a_col, int b_row, int b_col) {
    if (a_col != b_col) {
        return NULL;
    }

    int* answer = new int[a_row * b_col]; 
    for (int m=0; m < a_row; m++) {
        for (int s = 0; s < b_col; s++) {
            answer[m * a_row + s] = 0;
            for (int n = 0; n < a_col; n++) {
                answer[m * a_row + s] += a[m * a_row + n] * b[n * a_col + s];

            }
        }
    }
// 输出矩阵结果，
    for (int m = 0; m < a_row; m++){
        for (int s = 0; s < b_col; s++){
            cout << answer[m * a_row + s] << "\t";
        }
        cout << endl;

    }
    return answer;

    

}

int main() {
    int a[4] = {2,2,2,2};
    int b[4] = {1,1,1,1};

    int *answer = mat_mul(a, b, 2, 2, 2, 2);


}


