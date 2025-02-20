# Rod Cutting Problem — Single Piece File Test Cases

All test cases below share the same **`piecefile.txt`**:

This means:

- Length **1** -> Price **2**  
- Length **2** -> Price **5**  
- Length **3** -> Price **7**  
- Length **4** -> Price **9**  
- Length **5** -> Price **10**  
- Length **6** -> Price **13**  
- Length **7** -> Price **17**  
- Length **8** -> Price **19**  
- Length **10** -> Price **24**

We run **one** of these commands (depending on cache policy):

```bash
./rodcut_lru piecefile.txt
```
OR

```bash
./rodcut_mru piecefile.txt
```

# Test Case 1: Single Small Rod

Rod length: 3

Output:

1 @ 1 = 2
1 @ 2 = 5
Remainder: 0
Value: 7

# Test Case 2: Medium Rod

Rod length: 8

Output:

4 @ 2 = 20
Remainder: 0
Value: 20

# Test Case 3: Large Rod

Rod length: 10

Output:

1 @ 10 = 25
Remainder: 0
Value: 25

# Test Case 4: Repeated Queries (Cache Demo)

Rod length = 8

Output:

4 @ 2 = 20
Remainder: 0
Value: 20

Rod Length = 5

Output:

1 @ 1 = 2
2 @ 2 = 10
Remainder: 0
Value: 12

Rod Length: 8 again:

Output:

4 @ 2 = 20
Remainder: 0
Value: 20

(Should match the first time, now retrieved from cache.)

Rod Length: 5 again**:

Output:

1 @ 1 = 2
2 @ 2 = 10
Remainder: 0
Value: 12

The cache influences performance

