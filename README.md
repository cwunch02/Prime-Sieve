# Concurrent Prime Sieve

In my third project for Operating Systems, I created a concurrent version of the prime sieve (https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes) using pipes.
This sieve simply sees seeks out if a number can be divided by N and then passes it through a pipe to another process to continue if it can't be divided.
It will output all primes between 2 and M that is specified by the user.
