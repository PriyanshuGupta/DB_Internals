#include <bits/stdc++.h>
using namespace std;

#define PKBBUFFERSIZE 100000001

int pointer;

bool compare(pair<int, int> p, pair<int, int> q){
	if(q.second > p.second) return true;
	return false;
}

//sort the given file
void mysort(){
	// cout<<"came here"<<endl;
	ifstream file;
	file.open("read.txt");
	vector<pair<int, int> > arr;
	int read, value;
	while(file>>read>>value){
		arr.push_back(make_pair(read, value));
	}
	// cout<<"arr size "<<arr.size()<<endl;
	sort(arr.begin(), arr.end(), compare);
	ofstream out;
	out.open("sort.txt", std::ios_base::app);
	for(int i=0;i<arr.size();i++){
		out<<arr[i].first<<" "<<arr[i].second<<endl;
		// cout<<arr[i]<<endl;
	}
}

vector<pair<int, int> > readFile(int n, int m){// read elements from n to m both inclusive and returns a vector
	ifstream f;
	f.open("sort.txt");
	int read, value;
	// cout<<"start"<<endl;
	// cout<<"n "<<n<<endl;
	// cout<<"m "<<m<<endl;
	vector<pair<int, int> > ret;
	for(int i=1;i<n;i++){
		f>>read>>value;
		// cout<<read<<endl;
	}
	bool c=true;
	for(int i=n;i<=m;i++){
		if(f>>read>>value){
			ret.push_back(make_pair(read, value));
		}
		else{
			c=false;
			pointer=i;
			break;
		}
	}
	if(c){
		pointer=m+1;
	}
	// cout<<"end"<<endl;
	return ret;
}

void externalsort(){
	ifstream file;
	file.open("input.txt");
	bool check=true;
	int read, value;
	// cout<<"came in external sort"<<endl;
	int num_chunks=0;
	int total=0;
	while(check){
		// cout<<"came in while"<<endl;
		ofstream out;
		out.open("read.txt");
		int c=0;
		for(int i=0;i<PKBBUFFERSIZE;i++){
			c++;
			if(file>>read>>value){
				total++;
				out<<read<<" "<<value<<endl;
				// cout<<"read "<<read<<endl;
			}
			else{
				check=false;
				if(i!=0) num_chunks++;
				break;
			}
		}
		if(c==PKBBUFFERSIZE){
			num_chunks++;
		}
		mysort();
	}
	int ext=num_chunks+1;
	if(ext==2){
		cout<<"life is chill"<<endl;
		rename("sort.txt", "final.txt");
	}
	else if(ext>=3){
		vector<vector<pair<int, int> > > s;
		int size=PKBBUFFERSIZE/ext;
		// cout<<"size "<<size<<endl; 
		// cout<<"PKBBUFFERSIZE "<<PKBBUFFERSIZE<<endl;
		int ptrs_b[num_chunks];
		for(int i=0;i<num_chunks;i++){
			vector<pair<int, int> > temp=readFile(PKBBUFFERSIZE*(i)+1, PKBBUFFERSIZE*(i)+size);
			// cout<<"temp "<<temp[0]<<endl;
			s.push_back(temp);
			ptrs_b[i]=pointer;
		}
		int ptrs[num_chunks];//pointers to different chunks loaded in buffer
		for (int i = 0; i < num_chunks; i++){
			ptrs[i]=0;
		}
		bool exists[num_chunks];
		for (int i = 0; i < num_chunks; i++){
			exists[i]=true;
		}
		ofstream out;
		out.open("final.txt");
		for(int j=0;j<total;j++){
			int min, recID;
			// cout<<"min "<<min<<endl;
			int ptr;
			for(int i=0;i<num_chunks;i++){
				if(exists[i]){
					min=s[i][ptrs[i]].second;
					recID=s[i][ptrs[i]].first;
					ptr=i;
					break;
				}
			}
			// cout<<"min "<<min<<endl;
			for(int i=0;i<num_chunks;i++){//got the pointer to minimum
				if(exists[i]){
					if(s[i][ptrs[i]].second<min){
						min=s[i][ptrs[i]].second;
						recID=s[i][ptrs[i]].first;
						ptr=i;
					}
				}
			}
			out<<recID<<" "<<min<<endl;
			ptrs[ptr]++;
			// for(int i=0;i<num_chunks;i++){
			// 	cout<<"ptrs[i] "<<ptrs[i]<<" s[ptr].size() "<<s[ptr].size()<<" ";
			// }
			// cout<<endl;
			// cout<<"min "<<min<<endl;
			// cout<<"s1 "<<s[0][0].second<<"s2 "<<s[1][0].second<<endl;
			if(ptrs[ptr]==s[ptr].size()){//is empty
				if(ptrs_b[ptr]>PKBBUFFERSIZE*(ptr+1) || ptrs_b[ptr]> total){
					cout<<ptr<<" chunk is completed"<<endl;
					exists[ptr]=false;
				}
				else{
					vector<pair<int, int> > temp=readFile(ptrs_b[ptr], ptrs_b[ptr]+size-1);
					ptrs_b[ptr]=pointer;
					s[ptr]=temp;
					ptrs[ptr]=0;
				}
			}
		}
	}
}
int main(){
	remove("sort.txt");
	externalsort();
	remove("read.txt");
}
