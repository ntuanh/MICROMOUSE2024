#include<iostream>
#include<stack>



std::stack<int> st;

int main(){
    st.push(1);
    st.push(2);
    std :: cout << st.top();
    st.pop();
    std :: cout << st.empty() << std::endl;
    
    std :: cout << st.top();
    return 0 ;
}