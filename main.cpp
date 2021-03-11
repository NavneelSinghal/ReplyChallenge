#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,avx,avx2")

#include <bits/stdc++.h>

using namespace std;

using pii = pair<long long int, long long int>;
using ll = long long;

#define F first
#define S second

void pre() {}

struct Building {
    ll bx, by, bl, bc;
    Building() {}
    Building(int a) { cin >> bx >> by >> bl >> bc; }
};

struct Antenna {
    ll index;
    ll ar, ac;
    Antenna() {}
    Antenna(int i) {
        cin >> ar >> ac;
        index = i;
    }
};

void solve() {
    ll w, h, n, m, r;
    ll times = 500;

    cin >> w >> h;
    cin >> n >> m >> r;

    // chunk size
    ll unguarded_size_x = sqrt(w);
    ll unguarded_size_y = sqrt(h);

    ll x_size = clamp(unguarded_size_x, 1LL, w);
    ll y_size = clamp(unguarded_size_y, 1LL, h);
    // ll x_size = min(max(1LL, unguarded_size_x), w);
    // ll y_size = min(max(1LL, unguarded_size_y), h);

    ll x_chunks = (w + x_size - 1) / x_size;
    ll y_chunks = (h + y_size - 1) / y_size;

    vector<Building> buildings(n);
    vector<Antenna> ant(m);

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < n; i++) buildings[i] = Building(0);
    for (int i = 0; i < m; i++) ant[i] = Antenna(i);

    vector<vector<vector<Building>>> blocks = vector<vector<vector<Building>>>(
        x_chunks, vector<vector<Building>>(y_chunks));
    vector<vector<ll>> sum = vector<vector<ll>>(x_chunks, vector<ll>(y_chunks));

    for (auto& building : buildings) {
        blocks[building.bx / x_size][building.by / y_size].push_back(building);
        sum[building.bx / x_size][building.by / y_size] += building.bc;
    }

    priority_queue<pair<ll, pii>> pq;
    for (int i = 0; i < x_chunks; i++)
        for (int j = 0; j < y_chunks; j++)
            pq.emplace(sum[i][j], make_pair(1LL * i, 1LL * j));

    sort(ant.begin(), ant.end(),
         [&](const Antenna& a1, const Antenna& a2) { return a1.ac > a2.ac; });

    vector<pair<ll, pii>> ans;
    set<pii> dup;
    dup.emplace(0, 0);

    for (int k = 0; k < m; k++) {
        auto [sum_val, index] = pq.top();
        pq.pop();
        if (sum_val == 0) break;
        ll score = 0;
        pii answer = {0, 0};

        auto check = [&](int i, int j, int antx, int anty, int antw,
                         int range) {
            ll answer = 0;
            for (auto k : blocks[i][j]) {
                ll dist = abs(k.bx - antx) + abs(k.by - anty);
                if (dist <= range) answer += antw * k.bc - k.bl * dist;
            }
            return answer;
        };

        for (auto j = 0; j < times; j++) {
            pii curr;
            bool found = false;
            int iter = 0;
            while (true) {
                ll chunk_size_x = x_size;
                ll chunk_size_y = y_size;
                if (index.F == x_chunks - 1)
                    chunk_size_x = w - index.F * x_size;
                if (index.S == y_chunks - 1)
                    chunk_size_y = w - index.S * y_size;
                assert(chunk_size_x != 0);
                assert(chunk_size_y != 0);
                curr = {index.F * x_size + rng() % chunk_size_x,
                        index.S * y_size + rng() % chunk_size_y};
                iter++;
                if (iter == 100) break;
                if (dup.find(curr) == dup.end() && curr.F < w && curr.S < h) {
                    found = true;
                    break;
                }
            }

            if (!found) continue;
            ll now =
                check(index.F, index.S, curr.F, curr.S, ant[k].ac, ant[k].ar);
            if (now > score) {
                score = now;
                answer = curr;
            }
        }

        auto update = [&](int i, int j, int antx, int anty, int range) {
            auto it = remove_if(
                blocks[i][j].begin(), blocks[i][j].end(),
                [&](const Building& k) {
                    return abs(k.bx - antx) + abs(k.by - anty) <= range;
                });
            blocks[i][j].erase(it, blocks[i][j].end());
            sum[i][j] = 0;
            for (auto k : blocks[i][j]) sum[i][j] += k.bc;
        };
        if (score > 0) {
            update(index.F, index.S, answer.F, answer.S, ant[k].ar);
            pq.emplace(sum[index.F][index.S], index);
            ans.emplace_back(ant[k].index, answer);
            dup.insert(answer);
        }
    }

    cout << ans.size() << "\n";
    for (auto [antindex, index] : ans)
        cout << antindex << " " << index.F << " " << index.S << "\n";
    cout.flush();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    pre();
    long long int num = 1;
    string files[] = {"a", "b", "c", "d", "e", "f"};
    for (int TT = 0; TT < 6; TT++) {
        string inp = "test/" + files[TT] + ".in";
        string out = "output/" + files[TT] + ".out";
        freopen(inp.c_str(), "r", stdin);
        freopen(out.c_str(), "w", stdout);
        // cerr<<"ran"<<"\n";
        solve();
    }
}

