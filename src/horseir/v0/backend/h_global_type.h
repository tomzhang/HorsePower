
#define H_B 0
#define H_H 1
#define H_I 2
#define H_L 3
#define H_F 4
#define H_E 5
#define H_X 6
#define H_C 7  //char
#define H_Q 8  //sym
#define H_S 9  //str
#define H_M 10
#define H_D 11
#define H_Z 12
#define H_U 13
#define H_W 14
#define H_T 15
#define H_G 16 //list
#define H_N 17 //dict
#define H_Y 18 //enum
#define H_A 90 //table
#define H_K 91 //ktable
#define H_V 93 //value item

#define caseB case H_B:
#define caseH case H_H:
#define caseI case H_I:
#define caseL case H_L:
#define caseF case H_F:
#define caseE case H_E:
#define caseC case H_C:
#define caseQ case H_Q:
#define caseS case H_S:
#define caseM case H_M:
#define caseD case H_D:
#define caseZ case H_Z:
#define caseU case H_U:
#define caseW case H_W:
#define caseT case H_T:
#define caseX case H_X:
#define caseG case H_G:
#define caseN case H_N:
#define caseY case H_Y:
#define caseA case H_A:
#define caseK case H_K:
#define caseV case H_V:

#define vp(x)  x->typ
#define vn(x)  x->len

#define vb(x)  x->b
#define vh(x)  x->h
#define vi(x)  x->i32
#define vl(x)  x->i64
#define vf(x)  x->f32
#define ve(x)  x->f64
#define vc(x)  x->c
#define vq(x)  x->q
#define vs(x)  x->s
#define vm(x)  x->m
#define vd(x)  x->d
#define vz(x)  x->z
#define vu(x)  x->u
#define vw(x)  x->w
#define vt(x)  x->t
#define vx(y)  y->x
#define vy(x)  x->y
#define va(x)  x->a
#define vg(x)  x->g

#define sB(x) ((B*)(vg(x)))
#define sH(x) ((H*)(vg(x)))
#define sI(x) ((I*)(vg(x)))
#define sL(x) ((L*)(vg(x)))
#define sE(x) ((E*)(vg(x)))
#define sF(x) ((F*)(vg(x)))
#define sC(x) ((C*)(vg(x)))
#define sQ(x) ((Q*)(vg(x)))
#define sS(x) ((S*)(vg(x)))
#define sX(x) ((X*)(vg(x)))
#define sY(x) ((L*)(vg(x)))
#define sM(x) ((M*)(vg(x)))
#define sD(x) ((D*)(vg(x)))
#define sZ(x) ((Z*)(vg(x)))
#define sU(x) ((U*)(vg(x)))
#define sW(x) ((W*)(vg(x)))
#define sT(x) ((T*)(vg(x)))
#define sG(x) ((G*)(vg(x)))
#define sV(x) ((V )(vg(x)))

#define vB(x,i) sB(x)[i]
#define vH(x,i) sH(x)[i]
#define vI(x,i) sI(x)[i]
#define vL(x,i) sL(x)[i]
#define vE(x,i) sE(x)[i]
#define vF(x,i) sF(x)[i]
#define vC(x,i) sC(x)[i]
#define vQ(x,i) sQ(x)[i]
#define vS(x,i) sS(x)[i]
#define vX(x,i) sX(x)[i]
#define vY(x,i) sY(x)[i]
#define vM(x,i) sM(x)[i]
#define vD(x,i) sD(x)[i]
#define vZ(x,i) sZ(x)[i]
#define vU(x,i) sU(x)[i]
#define vW(x,i) sW(x)[i]
#define vT(x,i) sT(x)[i]
#define vV(x,k) (sV(x)+(k))

/* special x */

#define xp  vp(x)
#define xn  vn(x)

#define xb  vb(x)
#define xh  vh(x)
#define xi  vi(x)
#define xl  vl(x)
#define xf  vf(x)
#define xe  ve(x)
#define xc  vc(x)
#define xq  vq(x)
#define xs  vs(x)
#define xm  vm(x)
#define xd  vd(x)
#define xz  vz(x)
#define xu  vu(x)
#define xw  vw(x)
#define xt  vt(x)
#define xx  vx(x)
#define xg  vg(x)

#define xB(k) vB(x,k)
#define xH(k) vH(x,k)
#define xI(k) vI(x,k)
#define xL(k) vL(x,k)
#define xF(k) vF(x,k)
#define xE(k) vE(x,k)
#define xQ(k) vQ(x,k)
#define xC(k) vC(x,k)
#define xS(k) vS(x,k)
#define xM(k) vM(x,k)
#define xD(k) vD(x,k)
#define xZ(k) vZ(x,k)
#define xU(k) vU(x,k)
#define xW(k) vW(x,k)
#define xT(k) vT(x,k)
#define xX(k) vX(x,k)
#define xY(k) vY(x,k)
#define xG(k) ((V)(xg)+k)
#define xV(k) ((V)(xg)+k)
#define xN(k) xV(k)

