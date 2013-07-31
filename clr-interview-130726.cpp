//һ��С������ʾΪһ��N��N�ķ��񣬴�(0,0)��(N-1, N-1)��һ����վ��λ��(x,y)���������������ߣ�һ��һ���ӣ�ѡ���������ҵĿ�������һ���ġ�
//�����߳�С��������ζ��������������Ҫ��n�������������ĸ����ж����д�����롣

#define NMAX 20

int diff[][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
float getDeathProb(int x, int y, int w, int h, int n){
	float p[10][NMAX][NMAX];
	
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			if(i == 0 || i == w-1){
				if(j == 0 || j == w-1)  p[0][i][j]=0.5;
				else p[0][i][j]=0.25;
			}
			else p[0][i][j] = 0.0;
		}
	}
	
	for(int step = 1; step < n; step++){
		for(int i = 0; i < h; i++){
			for(int j = 0; j < w; j++){
				p[step][i][j] = 0.0;
				for(int k = 0; k < 4; k++){
					int co_x = i+diff[k][0];
					int co_y = j+diff[k][1];
					p[step][i][j] += (co_x<0 || co_y<0 || co_x>=h || co_y>w)? 0.0f : p[step-1][co_x][co_y];
				}
				p[step][i][j] = p[step][i][j]*0.25;
			}
		}
	}
	return p[n-1][x][y];
}