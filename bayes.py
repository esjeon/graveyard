#!/usr/bin/env python
# bayes.py - Bayesian network
# 
# Copyright © 2013 Eon S. Jeon <esjeon@live.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
# 

class factor:
    "factor class which represents factors in Bayesian Network"
    """vars vals"""
    
    #
    # restrict
    #
    #
    def restrict (self, var, bval):
        "restricts the value of the given variable to the specifed value"
        col = self.vars.index (var)
        
        nuvals = map (lambda row: self.vals[row],
                      filter(lambda row: self.getCell (row, col) == bval,
                             range (0, len(self.vals))))
        nuvars = self.vars[:]
        del nuvars[col]
        
        return factor (nuvars, nuvals)
    
    def restrict_mult (self, vars, bvals):
        "restricts multiple variables"
        
        ft = self
        for i in range(len(vars)):
            ft = ft.restrict (vars[i], bvals[i])
        return ft
    
    #
    # multiply
    #
    #
    @staticmethod
    def multiply (a, b):
        "returns the product between 2 factors"
        
        alen, blen = (len(a.vars), len(b.vars))
        aset, bset = (set(a.vars), set(b.vars))
        inter = aset & bset
        aset, bset = (aset - inter, bset - inter)
        
        nuvars = list(aset) + list(inter) + list(bset)
        
        nuvals = []
        for r in range(1 << len(nuvars)):
            row = factor.Int2Row (len(nuvars), r)
            fa = a.restrict_mult (nuvars[:alen], row[:alen])
            fb = b.restrict_mult (nuvars[-blen:], row[-blen:])
            nuvals.append (fa.vals[0] * fb.vals[0])
        
        return factor (nuvars, nuvals)
    
    def __mul__ (self, other):
        "mutiplication operator(*)"
        return factor.multiply (self, other)
    
    
    #
    # sumout
    #
    #
    def sumout (self, var):
        "sums out a variable in this factor"

        col = self.vars.index (var)
        
        tbl = {True:[], False:[]}
        for row in range (len(self.vals)):
            c = self.getCell (row, col)
            tbl[c].append (self.vals[row])
        
        nuvals = (lambda T,F:
                    map (lambda i: T[i]+F[i],
                         range(len(T)))
        ) (tbl[True], tbl[False])
        
        nuvars = self.vars[:]
        del nuvars[col]
        
        return factor (nuvars, nuvals)

    def sumout_mult (self, vars):
        f = self
        for var in vars:
            f = f.sumout (var)
        return f
    
    #
    # normalize
    #
    #
    def normalize (self):
        "normalizes factor"
        s = sum (self.vals)
        nuvars = self.vars[:]
        nuvals = map(lambda v: v/s, self.vals)
        return factor (nuvars, nuvals)
        
    
    #
    # inference
    #
    #
    @staticmethod
    def inference (factors, queries, hvars, evidences):
        F = factor([],[1])
        for q in queries:
            for f in factors:
                if f.vars[-1] == q:
                    F = F * f
                    factors.remove (f)
                    break
        
        print F
        
        for hv in hvars:
            if hv in queries: continue
            print hv
            
            rl = []
            for f in factors:
                if hv in f.vars:
                    F = F * f
                    rl.append (f)
            
            for f in rl:
                factors.remove (f)
            
            if hv in evidences:
                F = F.restrict (hv, evidences[hv])
            else:
                F = F.sumout (hv)
            print F
                
        return F
    
    #
    # Constructor
    #
    #
    def __init__ (self, vars, vals):
        assert (type(vars)==list)
        assert (type(vals)==list)
        assert (len(vals)==1<<len(vars))
        self.vars = vars
        self.vals = vals
        
    
    #
    # Utilities
    #
    #
    
    def __repr__ (self):
        return "f%s" % str(tuple(self.vars))
    def __str__ (self):
        s = ""
        for v in self.vars:
            s=s+("%7s" % str(v))
        s=s+('\n')
            
        for row in range(0, len(self.vals)):
            for col in range (0, len(self.vars)):
                s=s+("%7s" % str(self.getCell(row,col)))
            s=s+(" %4f\n" % self.vals[row])
        return s
    
    def getCell (self, row, col):
        return ((row >> (len(self.vars)-col-1))&1) == 0
    
    @staticmethod
    def Int2Row (n, row):
        if n == 0: return []
        return  factor.Int2Row(n-1, row>>1)+ [(row&1 == 0)]


def f (vars, vals):
    if type(vars) != list:
        vars = [vars]
        
    if len(vals) == 1<<len(vars):
        return factor (vars, vals)
    elif len(vals) == 1<<(len(vars)-1):
        # last variable should be permuted
        nuvals = []
        for v in vals:
            nuvals.append (v)
            nuvals.append (1-v)
        return factor (vars, nuvals)
    return None


if __name__ == '__main__':
    ftrav = f(['Trav'], [0.05])
    foc = f(['OC'], [0.60])
    ffraud = f(['Trav', 'Fraud'], [0.01, 0.004])
    fcrp = f(['OC', 'CRP'], [0.1, 0.001])
    ffp = f(['Trav', 'Fraud', 'FP'], [0.9, 0.9, 0.1, 0.01])
    fip = f(['OC', 'Fraud', 'IP'], [0.02, 0.01, 0.011, 0.001])
    
    #result = factor.inference([ftrav,foc,ffraud,fcrp,ffp,fip], ['Fraud'], ['Trav', 'FP', 'Fraud', 'IP', 'OC', 'CRP'], {})
    #result2 = factor.inference([ftrav,foc,ffraud,fcrp,ffp,fip], ['Fraud'], ['Trav', 'FP', 'Fraud', 'IP', 'OC', 'CRP'], {'Trav':True})
    #result3 = factor.inference([ftrav,foc,ffraud,fcrp,ffp,fip], ['Fraud'], ['Trav', 'FP', 'Fraud', 'IP', 'OC', 'CRP'], {'FP':True, 'IP':True, 'CRP':False, 'Trav':False, 'OC':False})
