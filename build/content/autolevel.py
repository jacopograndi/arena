import json

def vectorize (obj, forbidlist):
    for c in obj:
        for key in c:
            if not(key in forbidlist):
                val = c[key]
                try: val = int(val);
                except: pass
                try: val = float(val);
                except: pass
                c[key] = [val for i in range(3)]
    return obj
                

def comp(filename, forbidlist):
    with open(filename+".txt", "r") as f: obj = json.loads(f.read())
    obj = vectorize(obj, forbidlist)
    with open(filename+".txt", "w") as f:
        f.write(json.dumps(obj, indent=4))
    
if __name__ == "__main__":
    #comp("weapons", ["name", "damage_type"])
    #comp("augments", ["name"])
    #comp("batteries", ["name"])
    #comp("armor", ["name"])
    #comp("chassis", ["name"])
    comp("brains", ["name"])
