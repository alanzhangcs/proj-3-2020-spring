#include <iostream>
#include <bits/stdc++.h>
#include <time.h>

using namespace std;

struct Matrix{
    double * data;
    int col;
    int row;
};

Matrix load_data(const char* filename){
    Matrix m;

    FILE* f = fopen(filename, "r");
    char* buf = (char*)malloc(100);
    fgets(buf, 100, f);
    sscanf(buf, "%d %d", &(m.row), &(m.col));

    int row_max_len = m.col*7 > 200 ? m.col * 7:200;
    buf = (char *)malloc(row_max_len);
    int cursor = 0;
    m.data = (double *)malloc(sizeof(double) * m.row * m.col);
    if(m.data == NULL){
        printf("Out of memory error!\n");
        exit(1);
    }
    int line_cursor;
    for(int i = 0; i < m.row; i++){
        fgets(buf, row_max_len, f);
        line_cursor = 0;
        for(int j = 0; j < m.col; j++){
            m.data[cursor++] = atof(buf + line_cursor);
            while(buf[line_cursor] != ' ' && buf[line_cursor] != '\n')
                line_cursor++;
        }
    }

    free(buf);
    fclose(f);
    return m;
}

void write_data(const char* filename, Matrix m){
    FILE* f = fopen(filename, "w");
    fprintf(f, "%d ", m.row);
    fprintf(f, "%d\n", m.col);

    for(int i = 0; i < m.row; i++){
        for(int j = 0; j < m.col; j++){
            fprintf(f, "%.3f", m.data[i * m.col + j]);
            if (j != m.col - 1)
                fprintf(f, " ");
            else
                fprintf(f, "\n");
        }
    }
    fclose(f);

}

Matrix mat_mul(Matrix a, Matrix b) {
    Matrix res;
    if (a.col != b.row) {
        printf("Error! matrix dim not match.\n");
        printf("Calc %d,%d * %d,%d \n", a.row, a.col, b.row, b.col);
        return res;
    }

    res.row = a.row;
    res.col = b.col;
    res.data = (double *)malloc(sizeof(double) * a.row * b.col); 

    for (int m=0; m < res.row; m++) {
        for (int s = 0; s < res.col; s++) {
            res.data[m * res.col + s] = 0;
            for (int n = 0; n < res.col; n++) {
                res.data[m * res.col + s] += a.data[m * a.col + n] * b.data[n * b.row + s];
            }
        }
    }
    return res;
}


Matrix mat_add_vec(Matrix a, Matrix b, int axis) {
    Matrix res;
    if (axis == 0 ? a.row : a.col != b.row) {
        printf("Error! matrix dim not match.\n");
        printf("Calc %d,%d + %d,%d \n", a.row, a.col, b.row, b.col);
        return res;
    }

    res.row = a.row;
    res.col = a.col;
    res.data = (double *)malloc(sizeof(double) * res.row * res.col); 
    for (int m=0; m < res.row; m++) {
        for (int s = 0; s < res.col; s++) {
            if(axis == 0)
                res.data[m * res.col + s] = a.data[m * res.col + s] + b.data[m];
            else
                res.data[m * res.col + s] = a.data[m * res.col + s] + b.data[s];
        }
    }
    return res;
}


void softmax(Matrix res) {
    double sum;
    for (int m=0; m < res.row; m++) {
        sum = 0;
        for (int s = 0; s < res.col; s++){
            res.data[m * res.col + s] = exp(res.data[m * res.col + s]);
            sum += res.data[m * res.col + s];
        }
        for (int s = 0; s < res.col; s++) 
            res.data[m * res.col + s] = res.data[m * res.col + s] / sum;
    }
}


int main() {
    clock_t t;
    t = clock();
    Matrix bias = load_data("data/bias.txt");
    Matrix features = load_data("data/features.txt");
    Matrix labels = load_data("data/labels.txt");
    Matrix w = load_data("data/w.txt");
    printf("Load data: %fs\n", (double)(clock() - t) / CLOCKS_PER_SEC);

    t = clock();
    Matrix output1 = mat_mul(features, w);
    Matrix output2 = mat_add_vec(output1, bias, 1);
    // write_data("test1.txt", output2);
    // free(output1.data);
    softmax(output2);
    printf("Calculate time: %fs\n", (double)(clock() - t) / CLOCKS_PER_SEC);

    write_data("test.txt", output2);
    // Matrix features = load_data("data/features.txt");

}



