#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>
using namespace std;

// lol ill figure out inheritance later
struct PriceHeightIndex {
    int price, height, index;
    PriceHeightIndex() : price(0), height(0), index(0) {}
    PriceHeightIndex(int p, int h, int i) : price(p), height(h), index(i) {}
    bool operator<(const PriceHeightIndex &y) const {
        return make_pair(price, height) < make_pair(y.price, y.height);
    }
};

struct Height {
    int price, height, index;
    Height() :price(0), height(0), index(0) {}
    Height(int p, int h, int i) : price(p), height(h), index(i) {}
    bool operator<(const Height &y) const {
        return height < y.height;
    }
};

// ancient casters
Height toHeight(PriceHeightIndex x) {
    return Height(x.price, x.height, x.index);
}

PriceHeightIndex toPriceHeightIndex(Height x) {
    return PriceHeightIndex(x.price, x.height, x.index);
}

struct Range {
    int start, end, row;
    Range() : start(0), end(0), row(0) {}
    Range(int s, int e, int r) : start(s), end(e), row(r) {}
    bool operator<(const Range &y) const {
        return make_pair(start, - end) < make_pair(y.start, - y.end);
    }
};

PriceHeightIndex p[2][500000];
vector< Range > range[2];
multiset<Height> num; // (height, index)

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &p[i][j].price); // price
            p[i][j].index = j;
        }
        for (int j = 0; j < n; j++) {
            scanf("%d", &p[i][j].height); // height
        }
    }

    sort(p[0], p[0] + n);
    sort(p[1], p[1] + n);

    // identify ranges
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < n - 1; i++) {
            if (p[k][i].price == p[k][i + 1].price) { // equal price
                int start = i;
                int end = i + 2;
                while (p[k][end].price == p[k][i].price && i < n) end++; // end
                range[0].emplace_back(start, end - 1, k); //[start, end)
                i = end - 1;
                // printf("- %d (%d, %d)\n", k, start, end);
            }
        }
    }

    // earliest start occurs first
    // farthest end is prioritized when start is tiebreaked
    sort(range[0].begin(), range[0].end());

    // remove same start, because they are contained
    if (range[0].size() > 0) {
        range[1].push_back(range[0][0]);
        for (int i = 1; i < range[0].size(); i++) {
            // same start (contained)
            if (range[0][i - 1].start == range[0][i].start) continue;
            // end inside previous range (contained)
            if (range[0][i].start <= range[0][i - 1].end && 
                range[0][i].end <= range[0][i - 1].end) continue;
            
            range[1].push_back(range[0][i]);
        }
    }

    for (int i = 0; i < range[1].size(); i++) {
        int &row = range[1][i].row;
        int &start = range[1][i].start;
        int &end = range[1][i].end;
        num.clear();
        for (int j = start; j <= end; j++) {
            num.insert(toHeight(p[row][j]));
        }
        if (row == 1) {
            for (int j = start; j <= end; j++) {
                auto k = num.lower_bound(toHeight(p[row ^ 1][j]));

                if (k != num.begin()) k--;
                p[row][j] = toPriceHeightIndex(*k);
                num.erase(k);
            }
        } else {
            for (int j = start; j <= end; j++) {
                auto k = num.upper_bound(toHeight(p[row ^ 1][j]));
                if (k == num.end()) k--;
                p[row][j] = toPriceHeightIndex(*k);
                num.erase(k);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (p[0][i].height <= p[1][i].height) {
            printf("impossible\n");
            return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", p[0][i].index + 1);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", p[1][i].index + 1);
    }
    printf("\n");
}