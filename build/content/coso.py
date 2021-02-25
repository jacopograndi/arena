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
    return obj
    
def query (obj, allowlist, op):
    for c in obj:
        for key in c:
            if key in allowlist:
                c[key] = op(c[key])
    return obj
                
def update (obj, forbidlist, op):
    return op(obj)

def select (obj, allowlist, op):
    l = []
    for c in obj:
        for key in c:
            if key in allowlist:
                l += [c]
    return l
                

def comp(filename, l, fun, op=None, rewrite=None):
    with open(filename+".txt", "r") as f: obj = json.loads(f.read())
    nobj = fun(obj, l, op)
    if rewrite is None:
        with open(filename+".txt", "w") as f:
            f.write(json.dumps(nobj, indent=4))
    return nobj
    
def change_values():
    amt = .5
    perc_inc = lambda x: [x[0]*(1.0+(i/2.0)*amt) for i in range(3)]
    perc_dec = lambda x: [x[0]*(1.0-(i/2.0)*amt) for i in range(3)]
    
    comp("weapons", ["damage"], query, perc_inc)
    comp("weapons", ["cooldown"], query, perc_dec)
    comp("weapons", ["weight"], query, perc_dec)
    comp("weapons", ["range"], query, perc_inc)
    
    comp("armor", ["weight"], query, perc_dec)
    comp("armor", ["pierce"], query, perc_inc)
    comp("armor", ["spread"], query, perc_inc)
    comp("armor", ["fusion"], query, perc_inc)
    comp("armor", ["explosive"], query, perc_inc)
    comp("armor", ["laser"], query, perc_inc)
    comp("armor", ["emp"], query, perc_inc)
    comp("armor", ["impact"], query, perc_inc)
    
    comp("batteries", ["weight"], query, perc_dec)
    comp("batteries", ["capacity"], query, perc_inc)
    
    comp("chassis", ["weight"], query, perc_dec)
    comp("chassis", ["weight_max"], query, perc_inc)
    comp("chassis", ["hp"], query, perc_inc)
    comp("chassis", ["speed"], query, perc_inc)
    
    comp("brains", ["upkeep"], query, perc_dec)
    
    comp("augments", ["weight"], query, perc_inc)
    comp("augments", ["damage_pierce"], query, perc_inc)
    comp("augments", ["damage_spread"], query, perc_inc)
    comp("augments", ["damage_fusion"], query, perc_inc)
    comp("augments", ["damage_explosive"], query, perc_inc)
    comp("augments", ["damage_laser"], query, perc_inc)
    comp("augments", ["damage_emp"], query, perc_inc)
    comp("augments", ["damage_impact"], query, perc_inc)
    comp("augments", ["armor_pierce"], query, perc_inc)
    comp("augments", ["armor_spread"], query, perc_inc)
    comp("augments", ["armor_fusion"], query, perc_inc)
    comp("augments", ["armor_explosive"], query, perc_inc)
    comp("augments", ["armor_laser"], query, perc_inc)
    comp("augments", ["armor_emp"], query, perc_inc)
    comp("augments", ["armor_impact"], query, perc_inc)
    comp("augments", ["add_range"], query, perc_inc)
    comp("augments", ["add_cooldown"], query, perc_inc)
    comp("augments", ["recharge"], query, perc_inc)
    comp("augments", ["add_hp"], query, perc_inc)

def get_data ():
    # static data analysis, compute image
    get_first = lambda x: x
    xsc = comp("weapons", ["damage"], select, get_first, False)
    if len(xsc) < 2: pygame.quit(); return
    wstep = 200
    w = len(xsc)*wstep
    hmax = 512
    xs = [x["damage"][0]/x["cooldown"][0]*x["weight"][0]*x["range"][0] for x in xsc]
    maxx = max(xs); minx = min(xs)
    
    import pygame
    pygame.init()
    pygame.display.set_mode((20, 20))
    size = (w, hmax)
    scr = pygame.Surface((size[0], size[1]+15)).convert_alpha()
    scr.fill((0,0,0))
    font = pygame.font.SysFont("Courier New", 10)
    
    for i in range(len(xs)):
        x = xs[i]
        h = (x-minx)/(maxx-minx) *size[1]
        rect = wstep*i, 15+size[1]-h, wstep, h
        pygame.draw.rect(scr, (255,255,255), rect)
        txt = font.render(xsc[i]["name"], True, (255,255,255))
        scr.blit(txt, (rect[0], rect[1]-12))
    pygame.image.save(scr, "__results.png")
    pygame.quit()

if __name__ == "__main__":
    try:
        #comp("weapons", ["name", "damage_type"])
        #comp("augments", ["name"])
        #comp("batteries", ["name"])
        #comp("armor", ["name"])
        #comp("chassis", ["name"])
        #comp("brains", ["name"])
        
        #change_values()
        
        #get_data();
        
        
        forbidlist = ["name"]
        def rename (obj):
            nobj = []
            for d in obj:
                nd = {}
                for (key, value) in d.items():
                    if not(key in forbidlist): nd["base_"+key] = value
                    else: nd[key] = value
                nobj += [nd]
            return nobj
        #comp("chassis", ["name"], update, rename)
        comp("weapons", ["name"], update, rename)
        comp("augments", ["name"], update, rename)
        comp("batteries", ["name"], update, rename)
        comp("brains", ["name"], update, rename)
        comp("armor", ["name"], update, rename)
        
    except:
        import traceback
        traceback.print_exc()
        input()
