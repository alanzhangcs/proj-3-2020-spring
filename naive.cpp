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
    free(buf);

    int max_len = m.row * m.col * 7;
    buf = (char *)malloc(max_len);
    int cursor = 0;
    m.data = (double *)malloc(sizeof(double) * m.row * m.col);
    if(m.data == NULL){
        printf("Out of memory error!\n");
        exit(1);
    }
    fread(buf, 1, max_len, f);
    // printf("%c%c%c", buf[0], buf[1], buf[2]);

    int char_cursor = 0;
    double temp_num;
    char ch;
    double float_count;
    for(int i = 0; i < m.row; i++){
        // if(i % 1000 == 0)
        //     printf("loading data: %% %f\n", (double)i / m.row);
        for(int j = 0; j < m.col; j++){
            temp_num = 0;
            float_count = 10;
            ch = buf[char_cursor++];

            bool is_neg = ch == '-'?1:0;
            if (ch == '-')       
                ch = buf[char_cursor++];

            while(ch != ' ' && ch != '\n'){
                if(ch=='.'){
                    ch = buf[char_cursor++];
                    break;
                }
                temp_num *= 10;
                temp_num += ch - 48;
                ch = buf[char_cursor++];
            }
            while(ch != ' ' && ch != '\n'){
                temp_num += (ch - 48) / float_count;
                float_count *= 10;
                ch = buf[char_cursor++];
            }
            m.data[cursor++] = is_neg ? -temp_num:temp_num;
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
            for (int n = 0; n < a.col; n++) {
                res.data[m * res.col + s] += a.data[m * a.col + n] * b.data[n * b.col + s];
            }
            // if( res.data[m * res.col + s] == 0)
            //     printf("%d %d %d\n",m ,s);
        }
    }
    return res;
}


void normalize(Matrix res) {
    double sum;
    for (int m=0; m < res.row; m++) {
        sum = 0;
        for (int s = 0; s < res.col; s++){
            sum += res.data[m * res.col + s];
        }
        for (int s = 0; s < res.col; s++) 
            res.data[m * res.col + s] = res.data[m * res.col + s] / sum;
    }
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


double cross_entropy_loss(Matrix logits, Matrix ground_truth) {
    // Suppose the logits have already softmaxed
    if (logits.row != ground_truth.row || logits.col != ground_truth.col) {
        printf("Error! matrix dim not match.\n");
        printf("Calc %d,%d * %d,%d \n", logits.row, logits.col, ground_truth.row, ground_truth.col);
        return -1;
    }
    double sum = 0;
    double clip;
    // printf("%f", log(0.001));
    for (int m=0; m < logits.row; m++) {
        for (int s = 0; s < logits.col; s++){
            if(ground_truth.data[m * logits.col + s] != 0){
                clip = 1 - logits.data[m * logits.col + s];
                clip = clip < 0.001 ? 0.001: clip;
                // if(isnan(log(clip)))
                //     printf("%f %f", clip, logits.data[m * logits.col + s]);
                sum -= ground_truth.data[m * logits.col + s] * log(clip);
                // if(isnan(sum))
                //     printf("%f", clip);
            }
        }
    }
    return sum / logits.row;
}


Matrix logits2prediction(Matrix logits) {
    // Suppose the logits have already softmaxed
    Matrix res;
    res.row = logits.row;
    res.col = logits.col;
    res.data = (double*)malloc(sizeof(double) * res.row * res.col);
    int arg_max;
    double row_max;
    for (int m=0; m < logits.row; m++) {
        row_max = -1;
        arg_max = 0;
        for (int s = 0; s < logits.col; s++){
            res.data[m * res.col + s] = 0;
            if (logits.data[m * res.col + s] > row_max){
                // printf("%f %f\n", row_max, logits.data[m * res.col + s]);
                row_max = logits.data[m * res.col + s];
                arg_max = s;
            }
        }
        res.data[m * res.col + arg_max] = 1;
        // printf("%d ", arg_max);
    }
    return res;
}


double accuracy(Matrix prediction, Matrix ground_truth) {
    if (prediction.row != ground_truth.row || prediction.col != ground_truth.col) {
        printf("Error! matrix dim not match.\n");
        printf("Calc %d,%d * %d,%d \n", prediction.row, prediction.col, ground_truth.row, ground_truth.col);
        return -1;
    }
    int row = prediction.row, col = prediction.col;
    double acc = 0;
    int total_num = ground_truth.row;
    double temp_sum;
    bool correct;
    for (int m=0; m < row; m++) {
        temp_sum = 0;
        correct = true;
        for (int s = 0; s < col; s++){
            correct = correct? prediction.data[m * col + s] == ground_truth.data[m * col + s]: correct;
            temp_sum += ground_truth.data[m * col + s];
        }
        acc += correct && temp_sum != 0 ? 1 : 0;
        total_num -= temp_sum == 0 ? 1 : 0;
    }
    // printf("%f %d\n", acc, total_num);
    return acc / total_num;
}


int main() {
    clock_t t;
    t = clock();
    Matrix features = load_data("data/features.txt");
    Matrix labels = load_data("data/labels.txt");
    Matrix w = load_data("data/w.txt");
    normalize(features);
    printf("Load data: %fs\n", (double)(clock() - t) / CLOCKS_PER_SEC);

    t = clock();
    Matrix output = mat_mul(features, w);
    softmax(output);
    // write_data("test2.txt", output);
    double loss = cross_entropy_loss(output, labels);
    Matrix prediction = logits2prediction(output);
    double acc = accuracy(prediction, labels);
    printf("Calculate time: %fs\n", (double)(clock() - t) / CLOCKS_PER_SEC);
    
    printf("Result: loss = %f, acc = %f", loss, acc);
    write_data("prediction.txt", prediction);
    // Matrix features = load_data("data/features.txt");
    return 0;
}



