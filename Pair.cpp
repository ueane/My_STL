#include<string>
#include<iostream>

using namespace std;

template <class type1, class type2>
struct my_pair
{
	typedef type1 first_type;
	typedef type2 second_type;

	type1 first;
	type2 second;

	my_pair() :first(type1()),second(type2()) {}

	my_pair(const type1&a,const type2&b) :first(a),second(b) {}

	template<class U,class V>
	my_pair(const my_pair<U,V>&p)
		:first(p.first),second(p.second) {cout<<"copy"<<endl;}
};


template<class type1,class type2>
bool operator==(const my_pair<type1,type2>&s1, const my_pair<type1,type2>&s2)
{
	return (s1.first == s2.first) && (s1.second == s2.second);
}

template<class type1,class type2>
bool operator>(const my_pair<type1,type2>&s1, const my_pair<type1,type2>&s2)
{
	return (s1.first > s2.first) || (!(s1.first < s2.first) && s1.second > s2.second);
}

template<class type1,class type2>
bool operator<(const my_pair<type1,type2>&s1,const my_pair<type1,type2>&s2)
{
	return (s1.first < s2.first) || (!(s1.first > s2.first) && s1.second < s2.second);
}

int main()
{
	
	my_pair<int,string> a2;
	cout<<a2.first<<" "<<a2.second<<endl;

	my_pair<int,string> a1(123,"aaaa");
	cout<<a1.first<<" "<<a1.second<<endl;


	my_pair<int,string> s1(a1),s2(a1);

    
	my_pair<int,string> s3;	
	s3=a1;
	cout<<s3.first<<" "<<s3.second<<endl;
	return 0;
}
