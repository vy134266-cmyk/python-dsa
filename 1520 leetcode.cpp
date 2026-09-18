class Solution:
    def maxNumOfSubstrings(self, s: str) -> list[str]:
        n = len(s)

        # First and last occurrence of every character
        first = [n] * 26
        last = [-1] * 26

        for i, ch in enumerate(s):
            idx = ord(ch) - ord('a')
            first[idx] = min(first[idx], i)
            last[idx] = i

        # Find the smallest valid interval starting from index l
        def get_interval(l):
            r = last[ord(s[l]) - ord('a')]
            i = l

            while i <= r:
                idx = ord(s[i]) - ord('a')

                # This character started before l,
                # so this interval cannot be valid.
                if first[idx] < l:
                    return -1

                r = max(r, last[idx])
                i += 1

            return r

        intervals = []

        # Try every character's first occurrence
        for i in range(n):
            if i != first[ord(s[i]) - ord('a')]:
                continue

            r = get_interval(i)

            if r != -1:
                intervals.append((r, i))

        # Greedy: choose interval with earliest ending position
        intervals.sort()

        ans = []
        prev_end = -1

        for r, l in intervals:
            if l > prev_end:
                ans.append(s[l:r + 1])
                prev_end = r

        return ans