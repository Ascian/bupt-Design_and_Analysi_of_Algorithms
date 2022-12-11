#include <fstream>
#include <vector>

using namespace std;

//下三角矩阵
template<class Type>
class LowerTriangularMatrix
{
public:
    LowerTriangularMatrix(const int& n, const Type& default_value)
        :num_(n), matrix_(new Type* [n])
    {
        for (int i = 0; i < n; i++)
        {
            matrix_[i] = new Type[i];
            for (int j = 0; j < i; j++)
                matrix_[i][j] = default_value;
        }
    }
    ~LowerTriangularMatrix()
    {
        for (int i = 0; i < num_; i++)
        {
            delete[] matrix_[i];
        }
        delete[] matrix_;
    }

    const Type& at(const int& x, const int& y) const
    {
        if (x < y)
            return matrix_[y][x];
        return matrix_[x][y];
    }

    void set(const int& x, const int& y, const Type& value)
    {
        if (x < y)
            matrix_[y][x] = value;
        else
            matrix_[x][y] = value;
    }

    const int& size() const
    {
        return num_;
    }

private:
    int num_;
    Type** matrix_;
};

template<class Type>
void Dijkstra(Type* dist, int* prev, const int& v, const LowerTriangularMatrix<Type>& c)
{
    
}

int main()
{
    ifstream fin("dijkstra.in", ios::in);
    int n, e;
    fin >> n >> e;
    LowerTriangularMatrix<int> c(n, INT_MAX);
    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        fin >> u >> v >> w;
        c.set(u - 1, v - 1, w);
    }
    fin.close();

    ofstream fout("dijkstra.out", ios::out);
    int* dist = new int[n];
    int* prev = new int[n];
    Dijkstra(dist, prev, 0, c);
    fout << dist[n - 1];
    fout.close();
    delete[] dist;
    delete[] prev;
    return 0;
}

