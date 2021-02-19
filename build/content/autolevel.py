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
    
    #comp("armor", ["weight"], query, lambda x: [x[i]/5 for i in range(3)])
