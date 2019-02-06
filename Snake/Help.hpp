#ifndef HELP_H
#define HELP_H


#include<queue>
using namespace std;
const int inf = 9999;
int G[51][51], prev_x[51][51], prev_y[51][51], n = 50;
int step_x[] = {0, -1, 0, +1};
int step_y[] = {+1, 0, -1, 0};
queue<int> qx, qy;


void bfs(int x, int y){
    int d[52][52];
    for (int i = 0; i < n+2; i++)
        for (int j = 0; j < n+2; j++)
            d[i][j] = inf;
    d[x][y] = 0;
    qx.push(x);
    qy.push(y);
    while (!qx.empty()){
        int x_now = qx.front(), y_now = qy.front();
        qx.pop(); qy.pop();
        for (int k = 0; k < 4; k++){
            int x_neighbor = x_now + step_x[k], y_neighbor = y_now + step_y[k];
            if (G[x_neighbor][y_neighbor] == 0 && d[x_neighbor][y_neighbor] > d[x_now][y_now] + 1){
                d[x_neighbor][y_neighbor] = d[x_now][y_now] + 1;
               // d::cout << "neigbors: " << x_neighbor << " " << y_neighbor << "\n";
                prev_x[x_neighbor][y_neighbor] = x_now;
                prev_y[x_neighbor][y_neighbor] = y_now;
                qx.push(x_neighbor); qy.push(y_neighbor);
            }
        }
    }

}

int way_engine(float *BodyX, float *BodyY, int snake_size, float x_f, float y_f)
{

int x0, y0, x, y;

//std::cout << "0\n";
//    cin >> n >> x0 >> y0 >> x >> y;
    for (int i = 0 ; i< n+2; i++)
        for (int j = 0; j < n+2; j++)
            G[i][j] = inf;

    for (int i = 1; i < n+1; i++)
        for (int j = 1; j < n+1; j++)
            G[i][j] = 0;
  //  std::cout << "1\n";
   // std::cout << (int) (BodyX[0]/10+1) << " " << (int) (BodyY[0]/10+1) << " " << snake_size<<"\n";
    x0 = BodyX[0]/10+1;
   y0 = BodyY[0]/10+1;
   x = x_f/10+1;
   y = y_f/10 + 1;


    prev_x[x0][y0] = -1; prev_y[x0][y0] = -1;
    for (int i = 0; i < snake_size; i++)
    {
        ///std::cout << "y ";
        G[(int) (BodyX[i]/10+1)][(int) (BodyY[i]/10+1)] = 1;
    }
    G[x][y] = 0;
      //  std::cout << "\n";

    //std::cout << "2\n";
/*
    for (int i = 0; i < n+2; i++)
    {
        for (int j = 0; j < n+2; j++)
            if (G[i][j] != inf) std::cout << G[i][j] << " ";
            else std::cout << ". ";
        std::cout << "\n";
    }

    Sleep(10000);
*/
    bfs(x0, y0);

    //std::cout << "3\n";
for (int i = 1; i < n+1; i++)
    for (int j = 1; j < n+1; j++)
        G[i][j] = 1;
    int x_cur = x, y_cur = y;
    //Sleep(10000);
// std::cout << "4\n";
    while (true){
        G[x_cur][y_cur] = 0;
        int a = x_cur, b = y_cur;
      //  std::cout << x_cur << " " << y_cur << "\n";
        x_cur = prev_x[a][b];
        y_cur = prev_y[a][b];
        if (x_cur == x0 && y_cur == y0) break;
    }
 //std::cout << "5\n";
    G[x0][y0] = 0;

    if (G[x0+1][y0] == 0) return 1;
    if (G[x0-1][y0] == 0) return 3;
    if (G[x0][y0-1] == 0) return 0;
    if (G[x0][y0+1] == 0) return 2;
    return 0;
    }

#endif // HELP_H
