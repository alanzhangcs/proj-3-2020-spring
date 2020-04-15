# proj-3-2020-springt
para_dot_noalgo为未改算法版本。
para_dot修改了算法（计算顺序），但不得不提前赋值。希望能继续改进。

naive.cpp:一个未优化的串行实现版本。
## naive.cpp 函数说明
Matrix load_data(const char* filename): 从文件中读取矩阵（快速版）
void write_data(const char* filename, Matrix m): 将一个矩阵写到文件里
Matrix mat_mul(Matrix a, Matrix b): 返回一个新矩阵，原矩阵不动
void normalize(Matrix res): 矩阵按行线性归一化。
void softmax(Matrix res): 直接在当前矩阵上操作，将矩阵按行softmax
double cross_entropy_loss(Matrix logits, Matrix ground_truth): 对输出值（必须全为正数，默认已经softmax）和label做交叉熵
Matrix logits2prediction(Matrix logits): 将softmax后的输出值转化为0-1的预测值（生成新的矩阵）。
double accuracy(Matrix prediction, Matrix ground_truth) : 对预测值和labels计算准确率.
