import gmpy2 


p = 10957
g = 2
r = 10022
a = 1307 
m= 101


a = gmpy2.powmod(g, r, p)
b = gmpy2.powmod(g, 102, p)
print(a)
print(b)

def baby_step_giant_step():
    e = 1
    lookup_table = {}
    for j in range(m):
        lookup_table[e] = j
        e = (e * g) % p

    gamma = gmpy2.powmod(g, m, p)  
    gamma_inv = gmpy2.invert(gamma, p) 
    val = a
    for k in range(m):
        if val in lookup_table:
            r = k * m + lookup_table[val]
            return r
        val = (val * gamma_inv) % p
        
    
    print("No solution found")
    return None

final = baby_step_giant_step()
print(final)

