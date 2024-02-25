#ifndef MATRIXMATH_h
#define MATRIXMATH_h

#include <Arduino.h>
class MatrixMath {
    private:
        int *matrix;
        int rows;
        int cols;
    public:
        MatrixMath(int rows, int cols){
            matrix = new int[rows * cols];
            this->rows = rows;
            this->cols = cols;
        }
        ~MatrixMath(){
            delete[] matrix;
        }
        void set(int row, int col, int value){
            matrix[row * cols + col] = value;
        }
        int get(int row, int col){
            return matrix[row * cols + col];
        }
        void add(MatrixMath m){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) + m.get(i, j));
                }
            }
        }
        void subtract(MatrixMath m){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) - m.get(i, j));
                }
            }
        }
        void multiply(MatrixMath m){
            int *result = new int[rows * m.cols];
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < m.cols; j++){
                    int sum = 0;
                    for(int k = 0; k < cols; k++){
                        sum += get(i, k) * m.get(k, j);
                    }
                    result[i * m.cols + j] = sum;
                }
            }
            delete[] matrix;
            matrix = result;
            cols = m.cols;
        }
        void print(){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    Serial.print(get(i, j));
                    Serial.print(" ");
                }
                Serial.println();
            }
        }

        void scale(int scalar){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) * scalar);
                }
            }
        }

        void transpose(){
            int *result = new int[cols * rows];
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    result[j * rows + i] = get(i, j);
                }
            }
            delete[] matrix;
            matrix = result;
            int temp = rows;
            rows = cols;
            cols = temp;
        }

        void copy(MatrixMath m){
            delete[] matrix;
            matrix = new int[m.rows * m.cols];
            rows = m.rows;
            cols = m.cols;
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, m.get(i, j));
                }
            }
        }

        void identity(int size){
            delete[] matrix;
            matrix = new int[size * size];
            rows = size;
            cols = size;
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    if(i == j){
                        set(i, j, 1);
                    } else {
                        set(i, j, 0);
                    }
                }
            }
        }

        void zero(int rows, int cols){
            delete[] matrix;
            matrix = new int[rows * cols];
            this->rows = rows;
            this->cols = cols;
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, 0);
                }
            }
        }

        void copy(int *m, int rows, int cols){
            delete[] matrix;
            matrix = new int[rows * cols];
            this->rows = rows;
            this->cols = cols;
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, m[i * cols + j]);
                }
            }
        }

        void add(int scalar){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) + scalar);
                }
            }
        }

        void subtract(int scalar){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) - scalar);
                }
            }
        }

        void multiply(int scalar){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) * scalar);
                }
            }
        }

        void divide(int scalar){
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    set(i, j, get(i, j) / scalar);
                }
            }
        }

        void dotProduct(int *vector, int size){
            int *result = new int[rows];
            for(int i = 0; i < rows; i++){
                int sum = 0;
                for(int j = 0; j < size; j++){
                    sum += get(i, j) * vector[j];
                }
                result[i] = sum;
            }
            delete[] matrix;
            matrix = result;
            cols = 1;
        }

        void crossProduct(int *vector){
            int *result = new int[3];
            result[0] = get(1, 0) * vector[2] - get(2, 0) * vector[1];
            result[1] = get(2, 0) * vector[0] - get(0, 0) * vector[2];
            result[2] = get(0, 0) * vector[1] - get(1, 0) * vector[0];
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 1;
        }

        void crossProduct(MatrixMath m){
            int *result = new int[3];
            result[0] = get(1, 0) * m.get(2, 0) - get(2, 0) * m.get(1, 0);
            result[1] = get(2, 0) * m.get(0, 0) - get(0, 0) * m.get(2, 0);
            result[2] = get(0, 0) * m.get(1, 0) - get(1, 0) * m.get(0, 0);
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 1;
        }

        void normalize(){
            int magnitude = 0;
            for(int i = 0; i < rows; i++){
                magnitude += get(i, 0) * get(i, 0);
            }
            magnitude = sqrt(magnitude);
            for(int i = 0; i < rows; i++){
                set(i, 0, get(i, 0) / magnitude);
            }
        }

        int magnitude(){
            int result = 0;
            for(int i = 0; i < rows; i++){
                result += get(i, 0) * get(i, 0);
            }
            return sqrt(result);
        }

        void rotateX(float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            result[0] = 1;
            result[1] = 0;
            result[2] = 0;
            result[3] = 0;
            result[4] = c;
            result[5] = -s;
            result[6] = 0;
            result[7] = s;
            result[8] = c;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        void rotateY(float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            result[0] = c;
            result[1] = 0;
            result[2] = s;
            result[3] = 0;
            result[4] = 1;
            result[5] = 0;
            result[6] = -s;
            result[7] = 0;
            result[8] = c;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        void rotateZ(float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            result[0] = c;
            result[1] = -s;
            result[2] = 0;
            result[3] = s;
            result[4] = c;
            result[5] = 0;
            result[6] = 0;
            result[7] = 0;
            result[8] = 1;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        void rotate(float x, float y, float z, float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            float t = 1 - c;
            result[0] = t * x * x + c;
            result[1] = t * x * y - z * s;
            result[2] = t * x * z + y * s;
            result[3] = t * x * y + z * s;
            result[4] = t * y * y + c;
            result[5] = t * y * z - x * s;
            result[6] = t * x * z - y * s;
            result[7] = t * y * z + x * s;
            result[8] = t * z * z + c;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        void rotate(MatrixMath m, float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            float t = 1 - c;
            result[0] = t * m.get(0, 0) * m.get(0, 0) + c;
            result[1] = t * m.get(0, 0) * m.get(1, 0) - m.get(2, 0) * s;
            result[2] = t * m.get(0, 0) * m.get(2, 0) + m.get(1, 0) * s;
            result[3] = t * m.get(0, 0) * m.get(1, 0) + m.get(2, 0) * s;
            result[4] = t * m.get(1, 0) * m.get(1, 0) + c;
            result[5] = t * m.get(1, 0) * m.get(2, 0) - m.get(0, 0) * s;
            result[6] = t * m.get(0, 0) * m.get(2, 0) - m.get(1, 0) * s;
            result[7] = t * m.get(1, 0) * m.get(2, 0) + m.get(0, 0) * s;
            result[8] = t * m.get(2, 0) * m.get(2, 0) + c;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        void rotate(int x, int y, int z, float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            float t = 1 - c;
            result[0] = t * x * x + c;
            result[1] = t * x * y - z * s;
            result[2] = t * x * z + y * s;
            result[3] = t * x * y + z * s;
            result[4] = t * y * y + c;
            result[5] = t * y * z - x * s;
            result[6] = t * x * z - y * s;
            result[7] = t * y * z + x * s;
            result[8] = t * z * z + c;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        void rotate(int *m, float angle){
            int *result = new int[9];
            float c = cos(angle);
            float s = sin(angle);
            float t = 1 - c;
            result[0] = t * m[0] * m[0] + c;
            result[1] = t * m[0] * m[1] - m[2] * s;
            result[2] = t * m[0] * m[2] + m[1] * s;
            result[3] = t * m[0] * m[1] + m[2] * s;
            result[4] = t * m[1] * m[1] + c;
            result[5] = t * m[1] * m[2] - m[0] * s;
            result[6] = t * m[0] * m[2] - m[1] * s;
            result[7] = t * m[1] * m[2] + m[0] * s;
            result[8] = t * m[2] * m[2] + c;
            delete[] matrix;
            matrix = result;
            rows = 3;
            cols = 3;
        }

        
};



#endif // MATRIXMATH_h
