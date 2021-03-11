#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,avx,avx2")
#pragma GCC optimization("O3")
#pragma GCC optimization("unroll-loops")

#include <bits/stdc++.h>

#define rep(i, n) for (int i = 0; i < n; ++i)
#define REP(i, a, b) for (int i = a; i <= b; ++i)
#define filei freopen("f.in", "r", stdin);
#define fileo freopen("output.txt", "w", stdout);
#define gooi cout << "Case #" << i + 1 << " :";
#define s(n) scanf("%d", &n)
#define rev(i, n) for (int i = n; i >= 0; --i)
#define REV(i, a, b) for (int i = a; i >= b; --i)
#define miN(a, b) (((a) < (b)) ? (a) : (b))
#define sc(n) scanf("%c", &n)
#define tr(c, i) for (typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define INF 1000000000
#define pii pair<long long int, long long int>
#define pb(a) push_back(a)
#define F first
#define S second

typedef long long ll;
using namespace std;

void pre() {}

struct Build {
    ll bx, by, bl, bc;
    Build() {}
    Build(int a) { cin >> bx >> by >> bl >> bc; }
};

struct Antan {
    ll index;
    ll ar, ac;
    Antan() {}
    Antan(int i) {
        cin >> ar >> ac;
        index = i;
    }
};

void solve() {
    ll w, h, n, m, r;
    ll times = 200;

    cin >> w >> h;
    cin >> n >> m >> r;

    // chunk size

    ll something_x = sqrt(w);
    ll something_y = sqrt(h);

    ll x_size = min(max(1LL, something_x), w);
    ll y_size = min(max(1LL, something_y), h);

    ll x_chunks = (w + x_size - 1) / x_size;
    ll y_chunks = (h + y_size - 1) / y_size;

    vector<Build> buildings(n);
    vector<Antan> ant(m);

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < n; i++) {
        buildings[i] = Build(0);
    }

    for (int i = 0; i < m; i++) {
        ant[i] = Antan(i);
    }

    vector<vector<vector<Build>>> blocks = vector<vector<vector<Build>>>(
        x_chunks, vector<vector<Build>>(y_chunks));
    vector<vector<ll>> sum = vector<vector<ll>>(x_chunks, vector<ll>(y_chunks));

    for (auto& building : buildings) {
        blocks[building.bx / x_size][building.by / y_size].push_back(building);
        sum[building.bx / x_size][building.by / y_size] += building.bc;
    }

    priority_queue<pair<ll, pii>> pq;

    // sqrt issues?
    for (int i = 0; i < x_chunks; i++) {
        for (int j = 0; j < y_chunks; j++) {
            pq.emplace(sum[i][j], make_pair(1LL * i, 1LL * j));
        }
    }

    sort(ant.begin(), ant.end(),
         [&](const Antan& a1, const Antan& a2) { return a1.ac > a2.ac; });

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
                if (dist <= range) {
                    answer += antw * k.bc - k.bl * dist;
                }
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
                if (index.F == x_chunks - 1) {
                    chunk_size_x = w - index.F * x_size;
                }
                if (index.S == y_chunks - 1) {
                    chunk_size_y = w - index.S * y_size;
                }

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
                blocks[i][j].begin(), blocks[i][j].end(), [&](const Build& k) {
                    return abs(k.bx - antx) + abs(k.by - anty) <= range;
                });
            blocks[i][j].erase(it, blocks[i][j].end());
            sum[i][j] = 0;
            for (auto k : blocks[i][j]) {
                sum[i][j] += k.bc;
            }
        };

        if (score > 0) {
            update(index.F, index.S, answer.F, answer.S, ant[k].ar);
            pq.emplace(sum[index.F][index.S], index);
            ans.emplace_back(ant[k].index, answer);
            dup.insert(answer);
        }
    }

    cout << ans.size() << "\n";

    for (auto [antindex, index] : ans) {
        cout << antindex << " " << index.F << " " << index.S << "\n";
    }

    cout.flush();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    pre();
    long long int num = 1;
    filei;
    fileo;
    string files[] = {"a", "b", "c", "d", "e", "f"};
    for (int TT = 1; TT < 6; TT++) {
        string inp = files[TT] + ".in";
        string out = files[TT] + ".out";
        freopen(inp.c_str(), "r", stdin);
        freopen(out.c_str(), "w", stdout);
        // cerr<<"ran"<<"\n";
        solve();
    }
}

