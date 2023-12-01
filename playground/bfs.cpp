
#include <list>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <vector>

int nlimit = 20;
int mlimit = 100;

int n, m;
int u, v;

int main() {
  scanf("%d %d", &n, &m);
  if (n >= nlimit || m >= mlimit) {
    return 0;
  }

  if (n <= 0) {
    return 0;
  }

  std::vector<std::list<int>> graph(n);

  for (int i = 0; i < m; i++) {
    int u, v;
    scanf("%d %d", &u, &v);
    --u;
    --v;
    if (u < 0 || v < 0 || u >= n || v >= n) {
      return 0;
    }

    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  std::queue<int> q;
  q.push(0);
  std::vector<bool> used(n, false);
  std::vector<int> d(n, -1);
  d[0] = 0;

  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    for (auto nxt : graph[cur]) {
      if (used[nxt]) {
        continue;
      }
      used[nxt] = true;
      q.push(nxt);
      d[nxt] = d[cur] + 1;
    }
  }

  if (n > 7 && d[1] == 1 && d[5] == 2 && d[7] == 3) {
    __builtin_trap();
  }
  return 0;
}
