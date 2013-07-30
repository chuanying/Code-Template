//n��ɫ�ӣ�ÿ��ɫ��m�棬ÿһ���ֵ�ֱ���1-m���㽫n��ɫ��ͬʱ�ף���غ����г���������ּ���������Ϊsum��
//����һ������x�����sum>x������Ӯ������n��m��x������Ӯ�ĸ��ʡ�1<=n<=100��1<=m<=10��m<=x<n*m����ע΢����
//�š����ֹ��С�����һʱ���˽���������⼰�����

#define nMax 105
#define mMax 15
#define mnMax nMax*mMax+1
float getProbability(int n, int m, int x){

	int counts[mnMax];
	int counts_tmp[mnMax];
	memset(counts, 0, mnMax*sizeof(int));

	for(int i=1; i<=m; i++) counts[i] = 1;
	for(int i=2; i<=n; i++){
		memset(counts_tmp, 0, mnMax*sizeof(int));

		for(int j=1; j<=m; j++){//for each element
			for(int k = m*(i-1); k >= i-1; k--)
				counts_tmp[k+j] += counts[k];
		}
		for(int j = 0; j < m*n+1; j++) counts[j] = counts_tmp[j];
	}

	int count = 0;
	for(int i = m*n; i > x; i--)
		count += counts[i];

	int total_count = m;
	for(int i = 1; i < n; i++)
		total_count *= m;

	return float(count)/float(total_count);
}