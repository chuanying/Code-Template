// ����һ�����飬������ֻ����0��1�����ҵ�һ����������У�����0��1����������ͬ�ġ� 
// ��1��10101010 ��������䱾�� ��2��1101000 �����110100 ����չ���Լ���˼·��

//http://mp.weixin.qq.com/mp/appmsg/show?__biz=MjM5ODIzNDQ3Mw==&appmsgid=10000095&itemidx=1&sign=44976d2b68723146821c3a9fa252b8be

vector<int> getLength(vector<int> &A){
	vector<int> ret;
	if(A.size() == 0) return ret;
	
	vector<int> stat(A.size());
	if(A[0] == 0) stat[0] = -1;
	else stat[0] = 1;
	for(int i = 1; i < A.size(); i++){
		if(A[i] == 0) stat[i] = stat[i-1] - 1;
		else stat[i] = stat[i-1] + 1;
	}
	
	if(stat[A.size()-1] == 0)
		return vector<int>(A.begin(), A.end());
	
	map<int, int> mymap;
	int maxlen = 0;
	int b = 0;
	int e = 0;
	for(int i = 0; i < A.size(); i++){
		if(stat[i] == 0){
			if(maxlen < i+1){
				maxlen = i + 1;
				b = 0; 
				e = i+1;
			}
		}
		else{
			if(mymap.count(stat[i]) > 0){
				int curlen = i - mymap[stat[i]] + 1;
				if(curlen > maxlen){
					maxlen = curlen;
					b = mymap[stat[i]];
					e = i+1;
				}
			}
			else
				mymap[stat[i]] = i;
		}
	}
	return vector<int>(A.begin()+b, A.begin()+e);
}
int main()
{
	int arr[] = {1, 1, 0, 1, 0, 0, 0};
	vector<int> A(arr, arr+sizeof(arr)/sizeof(int));
	vector<int> ret = getLength(A);
	return 0;
}