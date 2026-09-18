# LeetCode 1520: Maximum Number of Non-Overlapping Substrings

[![Difficulty: Hard](https://img.shields.io/badge/Difficulty-Hard-red.svg)](#)
[![Topic: Greedy](https://img.shields.io/badge/Topic-Greedy-blue.svg)](#)
[![Topic: Strings](https://img.shields.io/badge/Topic-Strings-green.svg)](#)
[![Topic: Two%20Pointers](https://img.shields.io/badge/Topic-Two%20Pointers-orange.svg)](#)

---

## 📖 Problem Statement

Given a string `s` of lowercase English letters, you need to find the **maximum number of non-overlapping substrings** of `s` that meet the following conditions:

1. **Non-overlapping:** The substrings cannot overlap (no shared indices).
2. **Character Completeness:** If a substring contains a character `c`, then `s` must contain **all** occurrences of `c` inside this substring.
3. **Length Minimization (Tie-breaker):** If there are multiple answers with the maximum number of substrings, return the one with the **minimum total length**.

Return the substrings in **any order**.

---

### Example 1

```text
Input: s = "adefaddabbcesxy"
Output: ["e","f","ccc"]
Explanation:
- "e" contains all 'e's (indices [2..2]).
- "f" contains all 'f's (indices [3..3]).
- "ccc" contains all 'c's (indices [10..12]).
The substrings do not overlap and their total length is 1 + 1 + 3 = 5.
Picking ["e","f","ccc"] gives 3 substrings, which is the maximum possible.
```

### Example 2

```text
Input: s = "abbaccd"
Output: ["d","bb","cc"]
Explanation:
While ("abba") is a valid substring containing all 'a's and 'b's, choosing ["d", "bb", "cc"]
gives 3 substrings instead of 2.
```

### Constraints

- $1 \le \text{s.length} \le 10^5$
- `s` contains only lowercase English letters (`'a'` through `'z'`).

---

## 💡 Key Insights & Intuition

### 1. The Character Enclosure Rule
If any chosen substring includes a character `ch`, it **must** include every appearance of `ch` in the entire string.
Therefore, a valid substring starting at or containing `ch` must span at least from the first index of `ch` (`first[ch]`) to the last index of `ch` (`last[ch]`).

### 2. Cascading Expansion
When we examine the range `[first[ch], last[ch]]`, other characters may appear inside this range.
- If a character `x` appears inside `[first[ch], last[ch]]`:
  - The substring's right boundary must extend to at least `last[x]`.
  - If `first[x] < first[ch]`, it is **impossible** to form a valid substring whose leftmost index is `first[ch]` because `x` would pull the start boundary further to the left. In this case, we discard `first[ch]` as a candidate starting point!
- We continue expanding the right boundary until all characters enclosed in `[start, end]` have all their occurrences within `[start, end]`.

### 3. Interval Scheduling (Greedy Choice)
Once we collect all valid candidate intervals `[start, end]`:
- This problem reduces to the classic **Interval Scheduling Problem (Activity Selection)**:
  - Select the maximum number of non-overlapping intervals.
- **Greedy Strategy:** Sort candidate intervals by their **end position** in ascending order.
- Iterate through the sorted intervals:
  - If an interval starts after the end of the last chosen interval (`start > last_chosen_end`), we pick it!
  - Because sorting by end index naturally prefers shorter and earlier-finishing valid substrings, this maximizes the count of non-overlapping intervals and simultaneously minimizes the total length.

---

## 📐 Detailed Algorithm Steps

```text
Step 1: Track First & Last Occurrences
   first[c] = first index of character c
   last[c]  = last index of character c

Step 2: Generate Candidate Valid Intervals
   For each unique character c in s:
       start = first[c]
       end = last[c]
       valid = True
       For i from start to end:
           if first[s[i]] < start:
               # Found a character that starts before our start boundary
               valid = False
               break
           end = max(end, last[s[i]])
       If valid:
           Add [start, end] to candidates

Step 3: Greedy Interval Selection
   Sort candidates by end index ascending.
   prev_end = -1
   For [start, end] in candidates:
       if start > prev_end:
           Select s[start : end + 1]
           prev_end = end
```

---

## 💻 Implementations

### Python 3

```python
class Solution:
    def maxNumOfSubstrings(self, s: str) -> list[str]:
        # Step 1: Record first and last index for each character
        first = {}
        last = {}
        for i, ch in enumerate(s):
            if ch not in first:
                first[ch] = i
            last[ch] = i

        # Step 2: Find all valid minimal intervals
        intervals = []
        for ch in first:
            start = first[ch]
            end = last[ch]
            is_valid = True

            i = start
            while i <= end:
                c = s[i]
                if first[c] < start:
                    # Cannot start a valid substring at `start`
                    is_valid = False
                    break
                end = max(end, last[c])
                i += 1

            if is_valid:
                intervals.append((start, end))

        # Step 3: Sort by end index (greedy interval scheduling)
        intervals.sort(key=lambda x: x[1])

        # Step 4: Pick non-overlapping intervals
        res = []
        prev_end = -1
        for start, end in intervals:
            if start > prev_end:
                res.append(s[start : end + 1])
                prev_end = end

        return res
```

### C++

```cpp
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<string> maxNumOfSubstrings(string s) {
        int n = s.size();
        vector<int> first(26, -1), last(26, -1);

        for (int i = 0; i < n; ++i) {
            int c = s[i] - 'a';
            if (first[c] == -1) first[c] = i;
            last[c] = i;
        }

        vector<pair<int, int>> intervals;

        for (int c = 0; c < 26; ++c) {
            if (first[c] == -1) continue;

            int start = first[c];
            int end = last[c];
            bool valid = true;

            for (int i = start; i <= end; ++i) {
                int cur = s[i] - 'a';
                if (first[cur] < start) {
                    valid = false;
                    break;
                }
                end = max(end, last[cur]);
            }

            if (valid) {
                intervals.push_back({start, end});
            }
        }

        // Sort intervals by their ending position
        sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        vector<string> result;
        int prev_end = -1;
        for (const auto& [start, end] : intervals) {
            if (start > prev_end) {
                result.push_back(s.substr(start, end - start + 1));
                prev_end = end;
            }
        }

        return result;
    }
};
```

### Java

```java
import java.util.*;

class Solution {
    public List<String> maxNumOfSubstrings(String s) {
        int n = s.length();
        int[] first = new int[26];
        int[] last = new int[26];
        Arrays.fill(first, -1);
        Arrays.fill(last, -1);

        for (int i = 0; i < n; i++) {
            int idx = s.charAt(i) - 'a';
            if (first[idx] == -1) {
                first[idx] = i;
            }
            last[idx] = i;
        }

        List<int[]> intervals = new ArrayList<>();

        for (int i = 0; i < 26; i++) {
            if (first[i] == -1) continue;

            int start = first[i];
            int end = last[i];
            boolean valid = true;

            for (int j = start; j <= end; j++) {
                int charIdx = s.charAt(j) - 'a';
                if (first[charIdx] < start) {
                    valid = false;
                    break;
                }
                end = Math.max(end, last[charIdx]);
            }

            if (valid) {
                intervals.add(new int[]{start, end});
            }
        }

        // Sort intervals by end index ascending
        intervals.sort(Comparator.comparingInt(a -> a[1]));

        List<String> res = new ArrayList<>();
        int prevEnd = -1;
        for (int[] interval : intervals) {
            int start = interval[0];
            int end = interval[1];
            if (start > prevEnd) {
                res.add(s.substring(start, end + 1));
                prevEnd = end;
            }
        }

        return res;
    }
}
```

### JavaScript (ES6+)

```javascript
/**
 * @param {string} s
 * @return {string[]}
 */
var maxNumOfSubstrings = function(s) {
    const first = {};
    const last = {};

    for (let i = 0; i < s.length; i++) {
        const ch = s[i];
        if (first[ch] === undefined) first[ch] = i;
        last[ch] = i;
    }

    const intervals = [];

    for (const ch of Object.keys(first)) {
        let start = first[ch];
        let end = last[ch];
        let valid = true;

        for (let i = start; i <= end; i++) {
            const curr = s[i];
            if (first[curr] < start) {
                valid = false;
                break;
            }
            end = Math.max(end, last[curr]);
        }

        if (valid) {
            intervals.push([start, end]);
        }
    }

    // Sort by end position
    intervals.sort((a, b) => a[1] - b[1]);

    const res = [];
    let prevEnd = -1;

    for (const [start, end] of intervals) {
        if (start > prevEnd) {
            res.push(s.slice(start, end + 1));
            prevEnd = end;
        }
    }

    return res;
};
```

---

## ⏱ Complexity Analysis

| Metric | Complexity | Explanation |
| :--- | :--- | :--- |
| **Time Complexity** | $\mathcal{O}(|\Sigma| \cdot N) = \mathcal{O}(N)$ | $|\Sigma| = 26$ lowercase English letters. For each distinct character, we scan at most $N$ characters to extend boundaries. Sorting $\le 26$ intervals takes $\mathcal{O}(26 \log 26) = \mathcal{O}(1)$. Extracting substrings takes $\mathcal{O}(N)$. Overall time is strictly linear $\mathcal{O}(N)$. |
| **Space Complexity** | $\mathcal{O}(|\Sigma|) = \mathcal{O}(1)$ | We store first/last index arrays of size 26 and at most 26 intervals. (Excluding memory required to store the output strings). |

---

## 🧪 Edge Cases Considered

1. **Entire string has all identical characters:** `s = "aaaaa"` $\rightarrow$ `["aaaaa"]`.
2. **All characters are unique:** `s = "abcdef"` $\rightarrow$ `["a", "b", "c", "d", "e", "f"]`.
3. **Interleaving characters that force a single large substring:** `s = "ababa"` $\rightarrow$ `'a'` and `'b'` mutually enclose each other $\rightarrow$ `["ababa"]`.
4. **Nested independent substrings:** `s = "abbaccd"` $\rightarrow$ `["bb", "cc", "d"]` instead of `["abba", "cc", "d"]` because `"bb"` minimizes length and allows max count.
