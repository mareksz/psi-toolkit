# Toy grammar for Polish

s = np vp*
    np.Number == vp.Number

ap = adj*

ap = adv ap*

np = subst*

np = ap np*
    ap.Case == np.Case,
    ap.Gender == np.Gender,
    ap.Number == ap.Number

vp = verb*

vp = adv vp*

vp = vp* np
    np.Case == acc

