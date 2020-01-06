using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Numerics;

public class Day22b
{
    const Int64 factory = 119315717514047;
    const Int64 shuffles = 101741582076661;
    const long target = 2020;

    static BigInteger a = 1; // deck state (increment)
    static BigInteger b = 0; // deck state (offset)

    public static void Main(string[] args)
    {
        // Read input and reverse it
        var content = "";
        using (var sr = new StreamReader(@"C:\Users\Stephen\Documents\GitHub\AdventOfCode\aoc2019\Day22b\input.txt"))
            content = sr.ReadToEnd();
        String[] lines = content.Split('\n').Where(s => s.Trim() != "").Reverse().ToArray();

        // Undo each step in one pass of the input file
        foreach (var line in lines)
        {
            if (line.StartsWith("deal into new stack"))
                UndoDealIntoNewStack();
            else if (line.StartsWith("cut "))
                UndoCut(Int64.Parse(line.Substring(4)));
            else if (line.StartsWith("deal with increment "))
                UndoDealWithIncrement(Int64.Parse(line.Substring(20)));
            else
                Debug.Assert(false);
        }

        // Determine the net effect of one pass of the input file
        var a1 = (long)(a + factory) % factory;
        var b1 = (long)(b + factory) % factory;

        // Determine the net effect of multiple passes of the above
        var part2 =
            (modpow(a1, shuffles, factory) * target + b1 * (modpow(a1, shuffles, factory) + factory - 1) *
             modpow(a1 - 1, factory - 2, factory) + factory) % factory;

        Console.WriteLine("Part 2: {0}", part2);
        Debug.Assert(part2 == 71345377301237);
    }

    static BigInteger modpow(long n, long power, long mod)
    {
        return (long)BigInteger.ModPow(new BigInteger(n), new BigInteger(power), new BigInteger(mod));
    }

    static void UndoDealIntoNewStack()
    {
        a = -a % factory;
        b = -(b + 1) % factory;
    }

    static void UndoCut(Int64 n)
    {
        b = (b + n) % factory;
    }

    static void UndoDealWithIncrement(Int64 n)
    {
        var p = (modpow(n, factory - 2, factory) + factory) % factory;
        a = (a * p) % factory;
        b = (b * p) % factory;
    }
}
