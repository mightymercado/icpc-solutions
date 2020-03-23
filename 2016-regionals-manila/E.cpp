// Tested using https://www.acmicpc.net/problem/15385
// because icpclivearchive does not work...

// The main idea is that regardless of X, you can
// build an expression that results to Y in 28 steps.
// The editorial summarizes that you can do it recursively,
// with addition, multiplication and substraction.
// The base case is to build Y using (X + X + ... (y times)) / X

// The key step to the AC solution is cancelling out
// common divisions. Without this, I was getting
// 30+ operations. 

// Hint: https://math.stackexchange.com/questions/3591002/shortest-way-to-write-787-in-an-arithmetic-expression

#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

const int ADD = 0;
const int MUL = 1;
const int SUB = 2; 
const int DEF = 3;
const int N = 1212;
int sagot[N] = {0};
int nxt[N] = {0};
int opt[N] = {0};
string dp[1212] = {"A"};

string build(int y) {

  // printf("%d\n", y);
  if (dp[y] != string("A")) return dp[y];
  
  if (opt[y] == DEF) {
    string r = "";
    for (int i = 0; i < y; i++) r += "x";
    for (int i = 0; i < y - 1; i++) r += "+";
    return r + "x/";
  } else if (opt[y] == ADD) {
    string s1 = build(nxt[y]);
    string s2 = build(y - nxt[y]);
    s1 = s1.substr(0, s1.size() - 2); // remove division
    s2 = s2.substr(0, s2.size() - 2); // remove division
    return dp[y] = s1 + s2 + "+x/";
  } else if (opt[y] == MUL) {
    string s2 = build(nxt[y]);
    s2 = s2.substr(0, s2.size() - 2);
    return dp[y] = build(y / nxt[y]) + s2 + "*x/";
  } else /*if (opt[y] == SUB)*/ {
    string s1 = build(y + nxt[y]);
    string s2 = build(nxt[y]);
    s1 = s1.substr(0, s1.size() - 2); // remove division
    s2 = s2.substr(0, s2.size() - 2); // remove division
    return dp[y] = s1 + s2 + "-x/";
  }
}

int eval(string s) {
  typedef unsigned long long int u64;
  vector<u64> v;
  for(int i = 0; i < s.size(); i++) {
    if (s[i] == 'x') {
      v.push_back(1212);
    } else {
      u64 y = v.back(); v.pop_back();
      u64 x = v.back(); v.pop_back();
      u64 z;
      if (s[i] == '+') z = x + y;
      else if (s[i] == '*') z = x * y;
      else if (s[i] == '/') z = x / y;
      else if (s[i] == '-') z = x - y;
      v.push_back(z);
    }
  }
  if (v.size() != 1) return -1;
  return v[0];
}

int main() {
  for (int i = 0; i < 1212; i++) dp[i] = "A";
  sagot[0] = 1;

  for (int i = 1; i < N; i++) {
      sagot[i] = i;
      opt[i] = DEF;
  }

  for (int i = 2; i < N; i++) {
    for (int j = 1; j < i; j++) {
      if (sagot[i] > sagot[j] + sagot[i - j]) {
        nxt[i] = j;
        opt[i] = ADD;
        sagot[i] = sagot[j] + sagot[i - j];
      }
    }

    for (int j = 2; j * j <= i; j++) {
      if (i % j != 0) continue;
      if (sagot[i] > sagot[i / j] + sagot[j] + 1) {
        sagot[i] = sagot[i / j] + sagot[j] + 1;
        nxt[i] = j;
        opt[i] = MUL;
      }
    }
  }

  for (int i = N; i >= 2; i--) {
    for (int j = 1; j < i && i + j < N; j++) {
      if (sagot[i] > sagot[i + j] + sagot[j]) {
        sagot[i] = sagot[i + j] + sagot[j];
        nxt[i] = j;
        opt[i] = SUB;
      }
    }
  }

  int t;
  scanf("%d", &t);
  while (t--) {
    int x, y;
    scanf("%d %d", &x, &y);
    if (x == y) {
      printf("x\n");
    } else if (x == 0) {
      printf("IMPOSSIBLE\n");
    } else if (y == 0) {
      printf("xx-\n");
    } else {
      string s = build(y);
      printf("%s\n", s.c_str());
    }
  }  
}

