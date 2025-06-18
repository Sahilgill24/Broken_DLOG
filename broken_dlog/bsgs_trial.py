import gmpy2 


p = 10957
g = 2
r = 10022
a = 1307 
m= 101
# so it works atleast for small values
# and will work for larger values also. 

a = gmpy2.powmod(g, r, p)
b = gmpy2.powmod(g, 2718, p)
print(a)
print(b)

def baby_step_giant_step():
    
    # 6 seconds for this on 350000 steps, actually we need 2^25 steps
    # 18 seconds for 2^20 steps, and is able to store it in memory 
    # Baby step
    # solution not in 1/4th values. 
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

def solve_linear_congruence(a, b, m):
 
    d = gmpy2.gcd(a, m)
    
    if b % d != 0:
        return []  
    
    a_reduced = a // d
    b_reduced = b // d
    m_reduced = m // d

    a_inv = gmpy2.invert(a_reduced, m_reduced)
    x0 = (a_inv * b_reduced) % m_reduced
    
    solutions = []
    for i in range(d):
        solutions.append((x0 + i * m_reduced) % m)
    
    return solutions


def solve_discrete_log_equation(t, r, c, p):
    modulus = p - 1
    rhs = (t - r) % modulus
    
    print(f"Solving: {c}x ≡ {rhs} (mod {modulus})")
    print(f"gcd(c, p-1) = {gmpy2.gcd(c, modulus)}")
    
    solutions = solve_linear_congruence(c, rhs, modulus)
    
    if not solutions:
        print("No solutions exist!")
        return None
    
    print(f"Found {len(solutions)} solution(s):")
    for i, sol in enumerate(solutions):
        print(f"x_{i} = {sol}")
        verification = (c * sol) % modulus
        expected = rhs
        print(f"Verification: {c} * {sol} ≡ {verification} (mod {modulus})")
        print(f"Expected: {expected}")
        print(f"Correct: {verification == expected}")
        print()
    
    return solutions
