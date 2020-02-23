#include <cstdio>
#include <deque>
#include <algorithm>
#include <map>
#include <assert.h>
using namespace std;

struct Gene {
    char marker;
    int type;
    int index;
    Gene(char m, int t) : marker(m), type(t) {}
    Gene() : marker(0), type(0) {}
};

Gene gene[1000001];
// index = gene type, value = (marker, how many ended here)
deque< Gene > stack[1000001]; 
int cnt[1000001] = {0};
int state[1000001] = {0};
int cur[1000001] = {0};

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf(" %c%d", &gene[i].marker, &gene[i].type);
        gene[i].index = i;
    }
    
    // used for telling if all genes of type are used
    for (int i = 0; i < n; i++) {
        cnt[gene[i].type]++;
    }

    int best = 0;
    int index = 1;
    int cur_size = 0;
    
    for (int i = 0; i < 2 * n; i++) {
        int j = i % n;
        int type = gene[j].type;
        char marker = gene[j].marker;
        if (cur[type] * 2 == cnt[type] && state[j] == 2) cur_size -= 1;
        if (i >= n && state[j] == 1) {
            stack[type].pop_front();
            state[j] = 0;
        } else if (i >= n && state[j] == 2) {
            cur[type] -= 1;
            assert(cur[type] >= 0);
        }
        if (marker == 's') {
            stack[type].push_back(gene[j]);
            state[j] = 1; // waiting state
        } else if (marker == 'e' && stack[type].size() > 0) {
            Gene back = stack[type].back();
            stack[type].pop_back();
            cur[type] += 1; // new pair
            if (cur[type] * 2 == cnt[type]) {
                cur_size += 1; // gene type is fully nested        
            }
            // complete state
            state[back.index] = 2;
        }
        if (best <= cur_size && i >= n - 1) {
            int cand = gene[j].index + 2 <= n ? gene[j].index + 2 : 1;
            if (best < cur_size || index > cand) {
                index = cand;
            }
            best = cur_size;
        }
    }
    printf("%d %d\n", index, best);
}