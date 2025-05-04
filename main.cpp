#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

void readFullMatrixManual(vector<vector<int>>& matrix, int& rows, int& cols) {
    cout << "Enter number of rows and columns: ";
    cin >> rows >> cols;
    matrix.assign(rows, vector<int>(cols));
    cout << "Enter matrix elements:\n";
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> matrix[i][j];
}

void generateRandomMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    matrix.assign(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = rand() % 10;
}

void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row)
            cout << val << " ";
        cout << "\n";
    }
}

void convertToSparse(const vector<vector<int>>& matrix, vector<int>& vval, vector<int>& vpos, vector<int>& vptr) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vval.clear(); vpos.clear(); vptr.clear();
    vptr.push_back(0);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] != 0) {
                vval.push_back(matrix[i][j]);
                vpos.push_back(j);
            }
        }
        vptr.push_back(vval.size());
    }
}

void sparseToFullMatrix(const vector<int>& vval, const vector<int>& vpos, const vector<int>& vptr, int rows, int cols, vector<vector<int>>& matrix) {
    matrix.assign(rows, vector<int>(cols, 0));
    for (int i = 0; i < rows; i++) {
        for (int k = vptr[i]; k < vptr[i + 1]; k++) {
            int col = vpos[k];
            matrix[i][col] = vval[k];
        }
    }
}

void printSparse(const vector<int>& vval, const vector<int>& vpos, const vector<int>& vptr) {
    cout << "vval: ";
    for (int x : vval) cout << x << " ";
    cout << "\nvpos: ";
    for (int x : vpos) cout << x << " ";
    cout << "\nvptr: ";
    for (int x : vptr) cout << x << " ";
    cout << "\n";
}

void subtractFromDiagonalSparse(vector<int>& vval, const vector<int>& vpos, const vector<int>& vptr, int rows, int cols, int num) {
    for (int i = 0; i < rows; i++) {
        for (int k = vptr[i]; k < vptr[i + 1]; k++) {
            if (vpos[k] == i) {
                vval[k] -= num;
                break;
            }
        }
    }
}

void readMatrixFromFile(const string& filename, vector<vector<int>>& matrix, int& rows, int& cols) {
    ifstream file(filename);
    if (!file) {
        cout << "Cannot open file.\n";
        return;
    }
    file >> rows >> cols;
    matrix.assign(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            file >> matrix[i][j];
    file.close();
}

void readSparseManualDirect(vector<int>& vval, vector<int>& vpos, vector<int>& vptr, int& rows, int& cols) {
    cout << "Enter number of rows and columns: ";
    cin >> rows >> cols;
    int n;
    cout << "Enter number of non-zero elements: ";
    cin >> n;
    vval.resize(n);
    vpos.resize(n);
    vptr.resize(rows + 1);
    cout << "Enter vval: ";
    for (int i = 0; i < n; i++) cin >> vval[i];
    cout << "Enter vpos: ";
    for (int i = 0; i < n; i++) cin >> vpos[i];
    cout << "Enter vptr: ";
    for (int i = 0; i <= rows; i++) cin >> vptr[i];
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<vector<int>> matrix;
    vector<int> vval, vpos, vptr;
    int rows = 0, cols = 0, choice;

    while (true) {
        cout << "menu:\n";
        cout << "1 - Enter sparse matrix manually (vval, vpos, vptr)\n";
        cout << "2 - Read matrix from file\n";
        cout << "3 - Enter full matrix manually\n";
        cout << "4 - Generate random matrix\n";
        cout << "5 - Show full matrix\n";
        cout << "6 - Show sparse representation\n";
        cout << "7 - Subtract number from main diagonal\n";
        cout << "0 - Exit\n";
        cout << "Your choice: ";
        cin >> choice;
        if (choice == 0) break;
        switch (choice) {
        case 1:
            readSparseManualDirect(vval, vpos, vptr, rows, cols);
            sparseToFullMatrix(vval, vpos, vptr, rows, cols, matrix);
            break;
        case 2:
            cout << "Reading matrix from file...\n";
            readMatrixFromFile("matrix.txt", matrix, rows, cols);
            if (matrix.empty()) {
                cout << "Matrix is empty after reading from file.\n";
            }
            else {
                cout << "Matrix successfully loaded.\n";
                printMatrix(matrix);
            }
            convertToSparse(matrix, vval, vpos, vptr);
            break;
        case 3:
            readFullMatrixManual(matrix, rows, cols);
            convertToSparse(matrix, vval, vpos, vptr);
            break;
        case 4:
            cout << "Enter number of rows and columns: ";
            cin >> rows >> cols;
            generateRandomMatrix(matrix, rows, cols);
            convertToSparse(matrix, vval, vpos, vptr);
            break;
        case 5:
            cout << "Matrix:\n";
            printMatrix(matrix);
            break;
        case 6:
            cout << "Sparse representation:\n";
            printSparse(vval, vpos, vptr);
            break;
        case 7: {
            int num;
            cout << "Enter number to subtract from diagonal: ";
            cin >> num;
            subtractFromDiagonalSparse(vval, vpos, vptr, rows, cols, num);
            sparseToFullMatrix(vval, vpos, vptr, rows, cols, matrix);
            cout << "Diagonal updated.\n";
            break;
        }
        default:
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}