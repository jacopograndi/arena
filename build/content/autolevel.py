import json

def vectorize (obj, forbidlist, _):
    for c in obj:
        for key in c:
            if not(key in forbidlist):
                val = c[key]
                try: val = int(val);
                except: pass
                try: val = float(val);
                except: pass
                c[key] = [val for i in range(3)]
    
def query (obj, allowlist, op):
    for c in obj:
        for key in c:
            if key in allowlist:
                c[key] = op(c[key])
                

def comp(filename, l, fun, op=None):
    with open(filename+".txt", "r") as f: obj = json.loads(f.read())
    fun(obj, l, op)
    with open(filename+"-new"+".txt", "w") as f:
        f.write(json.dumps(obj, indent=4))
    
if __name__ == "__main__":
    #comp("weapons", ["name", "damage_type"])
    #comp("augments", ["name"])
    #comp("batteries", ["name"])
    #comp("armor", ["name"])
    #comp("chassis", ["name"])
    #comp("brains", ["name"])
    #comp("brains", ["name"])
    
    amt = .5
    perc_inc = lambda x: [x[i]*(1.0+(i/2.0)*amt) for i in range(3)]
    comp("weapons", ["damage"], query, perc_inc)
