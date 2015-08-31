/*
Copyright (C) 2003 by the Board of Trustees of Massachusetts Institute of
Technology, hereafter designated as the Copyright Owners.
 
License to use, copy, modify, sell and/or distribute this software and
its documentation for any purpose is hereby granted without royalty,
subject to the following terms and conditions:
 
1.  The above copyright notice and this permission notice must
appear in all copies of the software and related documentation.
 
2.  The names of the Copyright Owners may not be used in advertising or
publicity pertaining to distribution of the software without the specific,
prior written permission of the Copyright Owners.
 
3.  THE SOFTWARE IS PROVIDED "AS-IS" AND THE COPYRIGHT OWNERS MAKE NO
REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, BY WAY OF EXAMPLE, BUT NOT
LIMITATION.  THE COPYRIGHT OWNERS MAKE NO REPRESENTATIONS OR WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS TRADEMARKS OR OTHER
RIGHTS. THE COPYRIGHT OWNERS SHALL NOT BE LIABLE FOR ANY LIABILITY OR DAMAGES
WITH RESPECT TO ANY CLAIM BY LICENSEE OR ANY THIRD PARTY ON ACCOUNT OF, OR
ARISING FROM THE LICENSE, OR ANY SUBLICENSE OR USE OF THE SOFTWARE OR ANY
SERVICE OR SUPPORT.
 
LICENSEE shall indemnify, hold harmless and defend the Copyright Owners and
their trustees, officers, employees, students and agents against any and all
claims arising out of the exercise of any rights under this Agreement,
including, without limiting the generality of the foregoing, against any
damages, losses or liabilities whatsoever with respect to death or injury to
person or damage to property arising from or out of the possession, use, or
operation of Software or Licensed Program(s) by LICENSEE or its customers.

*/

#include "../FCWindow.h" // Enrico
#include "mulGlobal.h"
#include "zbufGlobal.h"

#include <string.h>

// funciton prototypes
void dischg(charge *pq);
void dismat(double **mat, int rows, int cols);
void disfchg(charge *pq);

disExtrasimpcube(pc)
cube *pc;
{
  viewprintf(stdout, "cubes[%d][%d][%d][%d]\n", pc->level, pc->j, pc->k, pc->l);
}

disExParsimpcube(pc)
cube *pc;
{
  cube *pa = pc->parent;
  viewprintf(stdout, "cubes[%d][%d][%d][%d], ", pc->level, pc->j, pc->k, pc->l);
  viewprintf(stdout, "parent = cubes[%d][%d][%d][%d]\n", pa->level, pa->j, pa->k, pa->l);
}

dissimpcube(pc)
cube *pc;
{
  viewprintf(stdout, "cube center: x=%g y=%g z=%g\n", pc->x, pc->y, pc->z);
  viewprintf(stdout, "index=%d dindex=%d level=%d loc_exact=%d mul_exact=%d numkids=%d\n",
	 pc->index, pc->dindex, pc->level,
	 pc->loc_exact, pc->mul_exact, pc->numkids);
  viewprintf(stdout, "numnbrs=%d upnumvects=%d directnumvects=%d downnumvects=%d\n",
	 pc->numnbrs, pc->upnumvects, pc->directnumvects, pc->downnumvects);
}

discube(pc)
cube *pc;
{
int i;
  viewprintf(stdout, "cube center: x=%g y=%g z=%g\n", pc->x, pc->y, pc->z);
  viewprintf(stdout, "index=%d dindex=%d level=%d loc_exact=%d mul_exact=%d numkids=%d\n",
	 pc->index, pc->dindex, pc->level,
	 pc->loc_exact, pc->mul_exact, pc->numkids);
  viewprintf(stdout, "numnbrs=%d upnumvects=%d directnumvects=%d downnumvects=%d\n",
	 pc->numnbrs, pc->upnumvects, pc->directnumvects, pc->downnumvects);
  if(pc->directnumvects > 0) {
    viewprintf(stdout, "num of elements in ");
    for(i=0; i < pc->directnumvects; i++) {
      viewprintf(stdout, "v%d = %d ", i, pc->directnumeles[i]);
    }
    viewprintf(stdout, "\nchgs\n");
    for(i=0; i < pc->directnumeles[0]; i++) {
      dischg(pc->chgs[i]);
    }
  }
  if(pc->downnumvects > 0) {
    viewprintf(stdout, "num of down elements in ");
    for(i=0; i < pc->downnumvects; i++) {
      viewprintf(stdout, "v%d = %d ", i, pc->downnumeles[i]);
    }
  }
}

disupcube(pc)
cube *pc;
{


}

disdirectcube(pc)
cube *pc;
{
int i;
  for(i=0; i < pc->directnumvects; i++) {
    viewprintf(stdout, "matrix %d\n", i);
    dismat(pc->directmats[i], pc->directnumeles[0], pc->directnumeles[i]);
    if(i==0) {
      viewprintf(stdout, "lu factored matrix\n");
      dismat(pc->directlu, pc->directnumeles[0], pc->directnumeles[i]);
    }
  }
}


dissys(sys)
ssystem *sys;
{
int i, j, k, l, side;
  viewprintf(stdout, "side=%d depth=%d order=%d\n",
	 sys->side, sys->depth, sys->order);
  viewprintf(stdout, "Cube corner is x=%g y=%g z=%g\n", sys->minx, sys->miny, sys->minz);
  viewprintf(stdout, "Cube side length= %g\n", sys->length);
  viewprintf(stdout, "Printing all the cubes\n");
  for(i = 0, side = 1; i <= sys->depth; i++, side *= 2) {
    for(j=0; j < side; j++) {
      for(k=0; k < side; k++) {
	for(l=0; l < side; l++) {
	  viewprintf(stdout, "\ncubes[%d][%d][%d][%d]\n", i, j, k, l);
	  dissimpcube(&(sys->cubes[i][j][k][l]));
/*	  disdirectcube(&(sys->cubes[i][j][k][l])); */
	}
      }
    }
  }
}



void dismat(mat, rows, cols)
double **mat;
int rows, cols;
{
int i,j;
  if(cols != 0) {
    for(i=0; i < rows; i++) {
      viewprintf(stdout, "\n i=%d\n", i);
      for(j=0; j < cols; j++) {
        viewprintf(stdout, "%d %g  ", j, mat[i][j]);
        if(((j+1) % 5) == 0) viewprintf(stdout, "\n");
      }
    }
    viewprintf(stdout, "\n");
  }
}


disvect(v, size)
double *v;
int size;
{
int i;
  for(i=0; i < size; i++) {
    viewprintf(stdout, "i=%d %g ", i, v[i]);
    if(((i+1) % 5) == 0) viewprintf(stdout, "\n");
  }
  viewprintf(stdout, "\n");
}

void dischg(pq)
charge *pq;
{
  viewprintf(stdout, "cond=%d index=%d\n", pq->cond, pq->index);
}

disallchg(pq) 
charge *pq;
{
charge *nq;
  for(nq = pq; nq != NULL; nq = nq->next) disfchg(pq);
}

void disfchg(pq) 
charge *pq;
{
/*
  printf("Cond=%d Corners\n", pq->cond);
  printf("x0=%g y0=%g z0=%g\n", pq->x0, pq->y0, pq->z0);
  printf("x1=%g y1=%g z1=%g\n", pq->x1, pq->y1, pq->z1);
  printf("x2=%g y2=%g z2=%g\n", pq->x2, pq->y2, pq->z2);
  printf("x3=%g y3=%g z3=%g\n", pq->x3, pq->y3, pq->z3);
  printf("Center\n");
  printf("x=%g y=%g z=%g\n", pq->x, pq->y, pq->z);
*/
}

/*
  dumps a rows x cols matrix of doubles; assumes indices from zero 
*/
void dumpMat(mat, rows, cols)
int rows, cols;
double **mat;
{
  int i, j;
  for(i = 0; i < rows; i++) {
    viewprintf(stdout, "    row%d ", i);
    for(j = 0; j < cols; j++) {
      if(mat[i][j] < 0.0) viewprintf(stdout, "%.5e ", mat[i][j]);
      else viewprintf(stdout, " %.5e ", mat[i][j]);
    }
    viewprintf(stdout, "\n");
  }
}

/*
  dumps a rows x cols matrix of doubles; assumes indices from zero 
*/
void dumpCorners(fp, mat, rows, cols)
int rows, cols;
double **mat;
FILE *fp;
{
  int i, j;
  for(i = 0; i < rows; i++) {
    fprintf(fp, "  corner%d ", i);
    for(j = 0; j < cols; j++) {
      if(mat[i][j] < 0.0) fprintf(fp, "%.5e ", mat[i][j]);
      else fprintf(fp, " %.5e ", mat[i][j]);
    }
    fprintf(fp, "\n");
  }
}

/*
  dumps a vector of itegers along side a vector of doubles, index from zero
*/
void dumpVecs(dblvec, intvec, size)
double *dblvec;
int *intvec, size;
{
  int i;

  for(i = 0; i < size; i++) {
    viewprintf(stdout, "%d %d %g\n", i, intvec[i], dblvec[i]);
  }
}

/*
  dumps the relative coordinates of an array of charges or evaluation pnts
*/
void dumpChgs(chgs, numchgs, x, y, z)
int numchgs;
double x, y, z;
charge **chgs;
{
  int i;
  double rho, cosA, beta;
  for(i = 0; i < numchgs; i++) {
    xyz2sphere(chgs[i]->x, chgs[i]->y, chgs[i]->z,
	       x, y, z, &rho, &cosA, &beta);
    viewprintf(stdout, "    %d %d ", chgs[i]->index, chgs[i]->cond);
    if(rho < 0) viewprintf(stdout, "(%.5e ", rho);
    else viewprintf(stdout, "( %.5e ", rho);
    if(cosA < 0) viewprintf(stdout, "%.5e ", cosA);
    else viewprintf(stdout, " %.5e ", cosA);
    if(beta < 0) viewprintf(stdout, "%.5e) ", beta);
    else viewprintf(stdout, " %.5e) ", beta);
    if(x < 0) viewprintf(stdout, "(%.5e ", chgs[i]->x);
    else viewprintf(stdout, "( %.5e ", chgs[i]->x);
    if(y < 0) viewprintf(stdout, "%.5e ", chgs[i]->y);
    else viewprintf(stdout, " %.5e ", chgs[i]->y);
    if(z < 0) viewprintf(stdout, "%.5e)\n", chgs[i]->z);
    else viewprintf(stdout, " %.5e)\n", chgs[i]->z);
  }
}

/*
  dumps the relative coordinates of an array of charges or evaluation pnts
  - also dumps dummy bit
*/
void dumpChgsWDummy(chgs, numchgs, is_dummy, x, y, z)
int numchgs, *is_dummy;
double x, y, z;
charge **chgs;
{
  int i;
  double rho, cosA, beta;
  for(i = 0; i < numchgs; i++) {
    xyz2sphere(chgs[i]->x, chgs[i]->y, chgs[i]->z,
	       x, y, z, &rho, &cosA, &beta);
    viewprintf(stdout, "    %d %d(%d) %d ", chgs[i]->index, is_dummy[i], 
	    chgs[i]->dummy, chgs[i]->cond);
    if(rho < 0) viewprintf(stdout, "(%.5e ", rho);
    else viewprintf(stdout, "( %.5e ", rho);
    if(cosA < 0) viewprintf(stdout, "%.5e ", cosA);
    else viewprintf(stdout, " %.5e ", cosA);
    if(beta < 0) viewprintf(stdout, "%.5e) ", beta);
    else viewprintf(stdout, " %.5e) ", beta);
    if(x < 0) viewprintf(stdout, "(%.5e ", chgs[i]->x);
    else viewprintf(stdout, "( %.5e ", chgs[i]->x);
    if(y < 0) viewprintf(stdout, "%.5e ", chgs[i]->y);
    else viewprintf(stdout, " %.5e ", chgs[i]->y);
    if(z < 0) viewprintf(stdout, "%.5e)\n", chgs[i]->z);
    else viewprintf(stdout, " %.5e)\n", chgs[i]->z);
  }
}

/*
  display the matrix built for a given charge to multipole transformation
*/
void dispQ2M(mat, chgs, numchgs, x, y, z, order)
int numchgs, order;
double **mat, x, y, z;
charge **chgs;
{
  viewprintf(stdout, "\nQ2M MATRIX: cube at (%.5e %.5e %.5e)\n", x, y, z);
  dumpMat(mat, multerms(order), numchgs);
  viewprintf(stdout, 
	  "    CHARGES IN CUBE # cond (rho_i cos(alpha_i) beta_i) (x y z):\n");
  dumpChgs(chgs, numchgs, x, y, z);
}

/*
  display the matrix built for a given multipole to local transformation
*/
void dispM2L(mat, x, y, z, xp, yp, zp, order)
int order;
double **mat, x, y, z, xp, yp, zp;
{
  viewprintf(stdout, 
   "\nM2L MATRIX: multi at (%.5e %.5e %.5e) -> local at (%.5e %.5e %.5e)\n",
	  x, y, z, xp, yp, zp);
  dumpMat(mat, multerms(order), multerms(order));
}

/*
  display the matrix built for a given charge to local transformation
*/
void dispQ2L(mat, chgs, numchgs, x, y, z, order)
int numchgs, order;
double **mat, x, y, z;
charge **chgs;
{
  viewprintf(stdout, "\nQ2L MATRIX: cube at (%.5e %.5e %.5e)\n", x, y, z);
  dumpMat(mat, multerms(order), numchgs);
  viewprintf(stdout, 
	  "    CHARGES IN CUBE # cond (rho_i cos(alpha_i) beta_i) (x y z):\n");
  dumpChgs(chgs, numchgs, x, y, z);
}

/*
  display the matrix built for a given charge to potential transformation
*/
void dispQ2P(mat, chgs, numchgs, is_dummy, pchgs, numpchgs)
int numchgs, numpchgs, *is_dummy;
double **mat;
charge **chgs, **pchgs;
{
  viewprintf(stdout, "\nQ2P MATRIX:\n");
  dumpMat(mat, numpchgs, numchgs);
  viewprintf(stdout, 
	  "    PANELS IN CUBE # dummy(real) cond (rho_i cos(alpha_i) beta_i) (x y z):\n");
  dumpChgsWDummy(chgs, numchgs, is_dummy, 0.0, 0.0, 0.0);
  viewprintf(stdout, 
	  "    EVALS IN CUBE # cond (rho_i cos(alpha_i) beta_i) (x y z):\n");
  dumpChgs(pchgs, numpchgs, 0.0, 0.0, 0.0);
}

/*
  display the matrix built for a given charge to potential transformation
*/
void dispQ2PDiag(mat, chgs, numchgs, is_dummy)
int numchgs, *is_dummy;
double **mat;
charge **chgs;
{
  viewprintf(stdout, "\nQ2PDiag MATRIX:\n");
  dumpMat(mat, numchgs, numchgs);
  viewprintf(stdout, 
	  "    PANELS IN CUBE # dummy(real) cond (rho_i cos(alpha_i) beta_i) (x y z):\n");
  dumpChgsWDummy(chgs, numchgs, is_dummy, 0.0, 0.0, 0.0);
}

/*
  display the matrix built for a given multipole to multipole transformation
*/
void dispM2M(mat, x, y, z, xp, yp, zp, order)
int order;
double **mat, x, y, z, xp, yp, zp;
{
  viewprintf(stdout, 
      "\nM2M MATRIX: cube at (%.5e %.5e %.5e) shifted to (%.5e %.5e %.5e)\n", 
	  x, y, z, xp, yp, zp);
  dumpMat(mat, multerms(order), multerms(order));
}

/*
  display the matrix built for a given local to local transformation
*/
void dispL2L(mat, x, y, z, xp, yp, zp, order)
int order;
double **mat, x, y, z, xp, yp, zp;
{
  viewprintf(stdout, 
      "\nL2L MATRIX: cube at (%.5e %.5e %.5e) shifted to (%.5e %.5e %.5e)\n", 
	  x, y, z, xp, yp, zp);
  dumpMat(mat, multerms(order), multerms(order));
}

/*
  display the matrix built for a given multipole to potential transformation
*/
void dispM2P(mat, x, y, z, chgs, numchgs, order)
int numchgs, order;
double **mat, x, y, z;
charge **chgs;
{
  viewprintf(stdout, "\nM2P MATRIX: cube at (%.5e %.5e %.5e)\n", x, y, z);
  dumpMat(mat, numchgs, multerms(order));
  viewprintf(stdout, 
	  "    EVAL PNTS IN CUBE # cond (rho_i, cos(alpha_i), beta_i):\n");
  dumpChgs(chgs, numchgs, x, y, z);
}

/*
  display the matrix built for a given local to potential transformation
*/
void dispL2P(mat, x, y, z, chgs, numchgs, order)
int numchgs, order;
double **mat, x, y, z;
charge **chgs;
{
  viewprintf(stdout, "\nL2P MATRIX: cube at (%.5e %.5e %.5e)\n", x, y, z);
  dumpMat(mat, numchgs, multerms(order));
  viewprintf(stdout, 
	  "    EVAL PNTS IN CUBE # cond (rho_i, cos(alpha_i), beta_i):\n");
  dumpChgs(chgs, numchgs, x, y, z);
}

/*
  displays upward pass and moment vectors associated with a cube - debug only
*/
void dumpUpVecs(pc)
cube *pc;
{
  int i, j;
  viewprintf(stdout, 
    "\nUPWARD PASS/MOMENT VECTORS, LEVEL %d CUBE AT (%.5e %.5e %.5e):\n",
	  pc->level, pc->x, pc->y, pc->z);
  for(i = 0; i < pc->upnumvects; i++) {
    viewprintf(stdout, "%d", i);
    for(j = 0; j < pc->upnumeles[i]; j++) {
      if(pc->upvects[i][j] < 0.0) 
	  viewprintf(stdout, " %.5e", pc->upvects[i][j]);
      else viewprintf(stdout, "  %.5e", pc->upvects[i][j]);
    }
    viewprintf(stdout, "\n");
  }
  viewprintf(stdout, "M");
  for(j = 0; j < pc->multisize; j++) {
    if(pc->multi[j] < 0.0) viewprintf(stdout, " %.5e", pc->multi[j]);
    else viewprintf(stdout, "  %.5e", pc->multi[j]);
  }
  viewprintf(stdout, "\n");
}

/*
  displays the upward pass vectors for the eight level 1 cubes - debug only
*/
void dumpLevOneUpVecs(sys)
ssystem *sys;
{
  int i, j, k;
  cube *****cubes = sys->cubes;
  for(i = 0; i < 2; i++) {
    for(j = 0; j < 2; j++) {
      for(k = 0; k < 2; k++) {
	if(cubes[1][i][j][k] != NULL) dumpUpVecs(cubes[1][i][j][k]);
      }
    }
  }
}

/*
  checks a cube (direct, local or eval) list for bad cube structs - debug only
  -- doesn't quite do this - always uses direct list for one thing
*/
void chkList(sys, listtype)
ssystem *sys;
int listtype;			/* DIRECT, LOCAL or EVAL */
{
  int cnt[BUFSIZ];		/* # of cubes processed by level */
  int depth = sys->depth;
  int lev, nn;
  int i, j;
  cube *nc;
  for(i = 0; i <= depth; i++) cnt[i] = 0;
  nc = sys->directlist;
  while(nc != NULL) {
    /* check number and level of neighbors */
    lev = nc->level;
    nn = nc->numnbrs;
    for(i = 0; i < nn; i++) {
      if(lev != ((nc->nbrs)[i])->level) {
	viewprintf(stderr, "chkList: level %d cube has a level %d nbr\n", lev,
		((nc->nbrs)[i])->level);
	viewprintf(stderr, " ok cubes ");
	for(j = 0; j <= depth; j++) viewprintf(stderr, "lev%d: %d ", j, cnt[j]);
	viewprintf(stderr, "\n");
	FCExit(FC_GENERIC_ERROR);
      }
    }
    /* check number of kids */
    if(lev == depth && nc->numkids != 0) {
      viewprintf(stderr, "chkList: level %d cube has children\n", lev);
      viewprintf(stderr, " ok cubes ");
      for(j = 0; j <= depth; j++) viewprintf(stderr, "lev%d: %d ", j, cnt[j]);
      viewprintf(stderr, "\n");
      FCExit(FC_GENERIC_ERROR);
    }
    /* if lowest level, check status of eval and direct vects */
    if(lev == depth) {
      if(nc->dindex == 0 || nc->directnumeles == NULL) {
	viewprintf(stderr, "chkList: level %d cube has bad direct info\n", lev);
	viewprintf(stderr, " ok cubes ");
	for(j = 0; j <= depth; j++) viewprintf(stderr, "lev%d: %d ", j, cnt[j]);
	viewprintf(stderr, "\n");
	FCExit(FC_GENERIC_ERROR);
      }
      if(nc->evalnumvects == 0 && listtype == EVAL) {
	viewprintf(stderr, "chkList: level %d cube has no eval info\n", lev);
	viewprintf(stderr, " ok cubes ");
	for(j = 0; j <= depth; j++) viewprintf(stderr, "lev%d: %d ", j, cnt[j]);
	viewprintf(stderr, "\n");
	FCExit(FC_GENERIC_ERROR);
      }
    }
    cnt[lev]++;
    if(listtype == DIRECT) nc = nc->dnext;
    else if(listtype == LOCAL) nc = nc->lnext;
    else if(listtype == EVAL) nc = nc->enext;
    else {
      viewprintf(stderr, "chkList: bad flag\n");
      FCExit(FC_GENERIC_ERROR);
    }
  }
  if(listtype == DIRECT) viewprintf(stdout, "\nDirect ");
  else if(listtype == LOCAL) viewprintf(stdout, "\nLocal ");
  else viewprintf(stdout, "\nEval ");
  viewprintf(stdout, "list ok: ");
  for(j = 0; j <= depth; j++) viewprintf(stdout, "lev%d: %d ", j, cnt[j]);
  viewprintf(stdout, "\n\n");
}

/*
  chks a cube for bad cube struct (direct, local or eval) entries - debug only
*/
void chkCube(sys, nc, listtype)
ssystem *sys;
cube *nc;
int listtype;			/* DIRECT, LOCAL or EVAL */
{
  int depth = sys->depth;
  int lev, nn;
  int i;
  if(nc != NULL) {
    /* check number and level of neighbors */
    lev = nc->level;
    nn = nc->numnbrs;
    for(i = 0; i < nn; i++) {
      if(lev != ((nc->nbrs)[i])->level) {
	viewprintf(stdout, "chkCube: level %d cube has a level %d nbr\n", lev,
		((nc->nbrs)[i])->level);
/*	FCExit(FC_GENERIC_ERROR);*/
      }
    }
    /* check number of kids */
    if(lev == depth && nc->numkids != 0) {
      viewprintf(stdout, "chkCube: level %d cube has children\n", lev);
/*      FCExit(FC_GENERIC_ERROR);*/
    }
    /* if lowest level, check status of eval and direct vects */
    if(lev == depth) {
      if(nc->dindex == 0) {
	viewprintf(stdout, "chkCube: level %d cube has zero direct index\n", lev);
/*	FCExit(FC_GENERIC_ERROR);*/
      }
      if(nc->directnumeles == NULL) {
	viewprintf(stdout, 
		"chkCube: level %d cube has NULL directnumeles\n", lev);
/*	FCExit(FC_GENERIC_ERROR);*/
      }
      if(nc->evalnumvects == 0 && listtype == EVAL) {
	viewprintf(stdout, "chkCube: level %d cube has no eval info\n", lev);
/*	FCExit(FC_GENERIC_ERROR);*/
      }
      if(nc->eval == NULL && listtype == EVAL) {
	viewprintf(stdout, "chkCube: level %d cube has no eval pntr\n", lev);
      }
    }
  }
}

/*
  checks the lowest level cubes for trouble using chkCube - debug only
*/
void chkLowLev(sys, listtype)
ssystem *sys;
int listtype;			/* DIRECT, LOCAL or EVAL */
{
  int i, j, k, l, side, depth = sys->depth, cnt = 0;
  cube *nc, *****cubes = sys->cubes;
  for(i = 1, side = 1; i <= depth; i++, side *= 2);
  for(j=0; j < side; j++) {	/* loop through all cubes at level depth */
    for(k=0; k < side; k++) {
      for(l=0; l < side; l++) {
	nc = cubes[depth][j][k][l];
	if(nc != NULL) {
	  chkCube(sys, nc, listtype);
	  cnt++;
	}
      }
    }
  }
  viewprintf(stdout,"Total lowest level (level %d) cubes checked = %d\n", 
	  depth, cnt);
}

/*
  dump the contents of a face struct
*/
void dump_face(fp, fac)
face *fac;
FILE *fp;
{
  int i;
  face **behind = fac->behind;

  fprintf(fp, "Face %d, %d sides, depth %d, mark %d, greylev %d\n", 
	  fac->index, fac->numsides, fac->depth, fac->mark, fac->greylev);
  fprintf(fp, "  plane: n = (%g %g %g) rhs = %g\n",
	  fac->normal[0], fac->normal[1], fac->normal[2], fac->rhs);
  fprintf(fp, "  behind [depth(index)]:");
  for(i = 0; i < fac->numbehind; i++) {
    fprintf(fp, " %d(%d)", behind[i]->depth, behind[i]->index);
    if(i % 10 == 0 && i != 0) fprintf(fp, "\n");
  }
  i--;
  if(!(i % 10 && i != 0)) fprintf(fp, "\n");
  fprintf(fp, " Corners\n");
  dumpCorners(fp, fac->c, fac->numsides, 3);
}  

/*
  core display routine used below
*/
void dumpSynCore1(str, depth, fcnt, excnt, emcnt, tcnt)
int depth, *fcnt, *excnt, *emcnt, *tcnt;
char *str;
{
  int i;
  viewprintf(stdout, "%-13s", str);
  for(i = 0; i <= depth; i++) {
    sprintf(str, "%d/%d/%d/%d ", fcnt[i], excnt[i], emcnt[i], tcnt[i]);
    if(i < 2) viewprintf(stdout, "%8s", str);
    else if(i == 2) viewprintf(stdout, "%12s", str);
    else if(i == 3) viewprintf(stdout, "%16s", str);
    else if(i == 4) viewprintf(stdout, "%20s", str);
    else if(i == 5) viewprintf(stdout, "%24s", str);
    else viewprintf(stdout, "%28s", str);
  }
  viewprintf(stdout, "\n");
}
/*
  core display rtn used below
*/
dumpSynCore2(str, depth, cnt)
int depth, *cnt;
char *str;
{
  int i;

  viewprintf(stdout, "%-13s", str);
  for(i = 0; i <= depth; i++) {
    sprintf(str, "%d    ", cnt[i]);
    if(i < 2) viewprintf(stdout, "%8s", str);
    else if(i == 2) viewprintf(stdout, "%12s", str);
    else if(i == 3) viewprintf(stdout, "%16s", str);
    else if(i == 4) viewprintf(stdout, "%20s", str);
    else if(i == 5) viewprintf(stdout, "%24s", str);
    else viewprintf(stdout, "%28s", str);
  }
  viewprintf(stdout, "\n");
}

/*
  displays number of exact, full, empty and total cubes per level in
  all cubes, and eval, direct, multi and local lists
*/
void dumpSynop(sys)
ssystem *sys;
{
  int i, j, k, l, side, depth = sys->depth, lev;
  int excnt[BUFSIZ], fcnt[BUFSIZ], emcnt[BUFSIZ], tcnt[BUFSIZ];
  extern int *multicnt, *localcnt;
  char str[BUFSIZ];
  cube *****cubes = sys->cubes, *nc;

  for(i = 0; i <= depth; i++) excnt[i] = fcnt[i] = emcnt[i] = tcnt[i] = 0;

  viewprintf(stdout, 
	  "\nCUBE AND EXPANSION SYNOPSIS (full/mul_exact/empty/ttl):\n");
  viewprintf(stdout, "             ");
  for(i = 0; i <= depth; i++) {
    sprintf(str, "level%d ", i);
    if(i < 2) viewprintf(stdout, "%8s", str);
    else if(i == 2) viewprintf(stdout, "%12s", str);
    else if(i == 3) viewprintf(stdout, "%16s", str);
    else if(i == 4) viewprintf(stdout, "%20s", str);
    else if(i == 5) viewprintf(stdout, "%24s", str);
    else viewprintf(stdout, "%28s", str);
  }
  viewprintf(stdout, "\n");
  /* dump cube usage by level */
  for(i = 0, side = 1; i <= depth; i++, side *= 2) {
    for(j=0; j < side; j++) {	/* loop through all cubes at levels >= 0 */
      for(k=0; k < side; k++) {
	for(l=0; l < side; l++) {
	  nc = cubes[i][j][k][l];
	  tcnt[i]++;
	  if(nc != NULL) {
	    lev = nc->level;
	    fcnt[i]++;
	    if(nc->mul_exact == TRUE) excnt[i]++;
	  }
	  else emcnt[i]++;
	}
      }
    }
  }
  sprintf(str, "All cubes");
  dumpSynCore1(str, depth, fcnt, excnt, emcnt, tcnt);
  
  for(i = 0; i <= depth; i++) excnt[i] = fcnt[i] = emcnt[i] = tcnt[i] = 0;
  /* dump cube direct list by level */
  for(nc = sys->directlist; nc != NULL; nc = nc->dnext) {
    lev = nc->level;
    tcnt[lev]++;
    if(nc->upnumvects > 0) fcnt[lev]++;
    else emcnt[lev]++;
    if(nc->mul_exact == TRUE) excnt[lev]++;
  }
  sprintf(str, "Direct list");
  dumpSynCore1(str, depth, fcnt, excnt, emcnt, tcnt);

  for(i = 0; i <= depth; i++) excnt[i] = fcnt[i] = emcnt[i] = tcnt[i] = 0;
  /* dump cube local list by level */
  for(i = 0; i <= depth; i++) {
    for(nc = sys->locallist[i]; nc != NULL; nc = nc->lnext) {
      lev = nc->level;
      tcnt[lev]++;
      if(nc->upnumvects > 0) fcnt[lev]++;
      else emcnt[lev]++;
      if(nc->mul_exact == TRUE) excnt[lev]++;
    }
  }
  sprintf(str, "Local list");
  dumpSynCore1(str, depth, fcnt, excnt, emcnt, tcnt);
    
  for(i = 0; i <= depth; i++) excnt[i] = fcnt[i] = emcnt[i] = tcnt[i] = 0;
  /* dump cube multipole list by level */
  for(i = 0; i <= depth; i++) {
    for(nc = sys->multilist[i]; nc != NULL; nc = nc->mnext) {
      lev = nc->level;
      tcnt[lev]++;
      if(nc->upnumvects > 0) fcnt[lev]++;
      else emcnt[lev]++;
      if(nc->mul_exact == TRUE) excnt[lev]++;
    }
  }
  sprintf(str, "Multi list");
  dumpSynCore1(str, depth, fcnt, excnt, emcnt, tcnt);


  sprintf(str, "Multis built");
  dumpSynCore2(str, depth, multicnt);

  sprintf(str, "Locals built");
  dumpSynCore2(str, depth, localcnt);

}

/*
  dumps the Gaussian unit (statcoulombs/meter^2) charge densities on panels
*/
void dumpChgDen(fp, q, chglist)
double *q;
charge *chglist;
FILE *fp;
{
  charge *panel;

  for(panel = chglist; panel != NULL; panel = panel->next) {
    if(panel->dummy) continue;
    fprintf(fp, "%d\tq/A = %.5e %g", panel->index,
	    q[panel->index]/panel->area, panel->area);
    if(panel->surf->type == CONDTR) fprintf(fp, " CONDTR");
    if(panel->surf->type == DIELEC) fprintf(fp, " DIELEC");
    if(panel->surf->type == BOTH) fprintf(fp, " BOTH");
    fprintf(fp, " (%.3g %.3g %.3g)", panel->x, panel->y, panel->z);
    fprintf(fp, " cond = %d\n", panel->cond);
  }
  fflush(fp);
}

/*
  like dumpMat but different formating and row labels (for dumpMatBldCnts)
*/
void dumpMatCnts(mat, depth, type)
int **mat, depth;
char *type;
{
  int i, j;
  char str[BUFSIZ];

  viewprintf(stdout,
	  "\n%s MATRIX BUILD TOTALS (row = from cube, col = to cube):\n", 
	  type);

  for(i = 0; i <= depth; i++) {
    sprintf(str, " to %d ", i);
    if(i == 0) viewprintf(stdout, "%13s", str);
    else if(i < 10) viewprintf(stdout, "%6s", str);
    else viewprintf(stdout, "%7s", str);
  }
  viewprintf(stdout, "\n");

  for(i = 0; i <= depth; i++) {
    sprintf(str, "from %d ", i);
    viewprintf(stdout, "%-7s", str); /* print row label */
    for(j = 0; j <= depth; j++) {
      sprintf(str, "%d ", mat[i][j]);
      if(j < 10) viewprintf(stdout, "%6s", str);
      else viewprintf(stdout, "%7s", str);
    }
    viewprintf(stdout, "\n");
  }

}

/*
  display matrix build count totals
*/
void dumpMatBldCnts(sys)
ssystem *sys;
{
  char type[BUFSIZ];
  extern int **Q2Mcnt, **Q2Lcnt, **Q2Pcnt, **L2Lcnt;
  extern int **M2Mcnt, **M2Lcnt, **M2Pcnt, **L2Pcnt, **Q2PDcnt;

  sprintf(type, "Q2M");
  dumpMatCnts(Q2Mcnt, sys->depth, type);

  sprintf(type, "Q2L");
  dumpMatCnts(Q2Lcnt, sys->depth, type);

  sprintf(type, "Q2P");
  dumpMatCnts(Q2Pcnt, sys->depth, type);

  sprintf(type, "M2M");
  dumpMatCnts(M2Mcnt, sys->depth, type);

  sprintf(type, "M2L");
  dumpMatCnts(M2Lcnt, sys->depth, type);

  sprintf(type, "M2P");
  dumpMatCnts(M2Pcnt, sys->depth, type);

  sprintf(type, "L2L");
  dumpMatCnts(L2Lcnt, sys->depth, type);

  sprintf(type, "L2P");
  dumpMatCnts(L2Pcnt, sys->depth, type);

  sprintf(type, "Q2PDiag");
  dumpMatCnts(Q2PDcnt, sys->depth, type);

}

/* 
  dumps state of important compile flags
*/
void dumpConfig(fp, name)
char *name;
FILE *fp;
{
  int size = -1;		/* for '#define MAXITER size' case */

  fprintf(fp, "\n%s CONFIGURATION FLAGS:\n", name);

  fprintf(fp, " DISCRETIZATION CONFIGURATION\n");

  fprintf(fp, "   WRMETH");
  if(WRMETH == COLLOC)
      fprintf(fp, " == COLLOC (point collocation)\n");
  else if(WRMETH == SUBDOM)
      fprintf(fp, " == SUBDOM (not implemented - do collocation)\n");
  else if(WRMETH == GALKIN)
      fprintf(fp, " == GALKIN (not implemented - do collocation)\n");
  fprintf(fp, "   ELTYPE");
  if(ELTYPE == CONST)
      fprintf(fp, " == CONST (constant panel densities)\n");
  else if(ELTYPE == AFFINE)
      fprintf(fp, " == AFFINE (not implemented - use constant)\n");
  else if(ELTYPE == QUADRA)
      fprintf(fp, " == QUADRA (not implemented - use constant)\n");

  fprintf(fp, " MULTIPOLE CONFIGURATION\n");

  fprintf(fp, "   DNTYPE");
  if(DNTYPE == NOLOCL) 
      fprintf(fp, " == NOLOCL (no locals in dwnwd pass)\n");
  else if(DNTYPE == NOSHFT) 
      fprintf(fp, " == NOSHFT (no local2local shift dwnwd pass)\n");
  else if(DNTYPE == GRENGD) 
      fprintf(fp, " == GRENGD (full Greengard dwnwd pass)\n");
  fprintf(fp, "   MULTI");
  if(MULTI == ON) fprintf(fp, " == ON (include multipole part of P*q)\n");
  else fprintf(fp, " == OFF (don't use multipole part of P*q)\n");
  fprintf(fp, "   RADINTER");
  if(RADINTER == ON) 
      fprintf(fp," == ON (allow parent level interaction list entries)\n");
  else 
   fprintf(fp," == OFF (use only cube level interaction list entries)\n");
  fprintf(fp, "   NNBRS == %d (max distance to a nrst neighbor)\n", NNBRS);
  fprintf(fp, "   ADAPT");
  if(ADAPT == ON) 
      fprintf(fp, " == ON (adaptive - no expansions in exact cubes)\n");
  else fprintf(fp, " == OFF (not adaptive - expansions in all cubes)\n");
  fprintf(fp, "   OPCNT");
  if(OPCNT == ON) 
      fprintf(fp, " == ON (count P*q ops - exit after mat build)\n");
  else fprintf(fp, " == OFF (no P*q op count - iterate to convergence)\n");

  fprintf(fp, "   MAXDEP");
  fprintf(fp, 
	  " == %d (assume no more than %d partitioning levels are needed)\n",
	  MAXDEP, MAXDEP);

  fprintf(fp, "   NUMDPT");
  fprintf(fp, 
	  " == %d (do %d potential evaluations for each dielectric panel)\n",
	  NUMDPT, NUMDPT);

  fprintf(fp, " LINEAR SYSTEM SOLUTION CONFIGURATION\n");

  fprintf(fp, "   ITRTYP");
  if(ITRTYP == GCR)
      fprintf(fp, " == GCR (generalized conjugate residuals)\n");
  else if(ITRTYP == GMRES)
      fprintf(fp, " == GMRES (generalized minimum residuals)\n");
  else fprintf(fp, " == %d (not implemented - use GCR)\n", ITRTYP);

  fprintf(fp, "   PRECOND");
  if(PRECOND == BD) {
    fprintf(fp, 
	    " == BD (use block diagonal preconditioner)\n");
  }
  else if(PRECOND == OL) {
    fprintf(fp, 
	    " == OL (use overlap preconditioner)\n");
  }
  else if(PRECOND == NONE) {
    fprintf(fp, 
	    " == NONE (no preconditioner)\n");
  }
  else fprintf(fp, " == %d (not implemented - use BD, OL or NONE)\n", PRECOND);

  fprintf(fp, "   DIRSOL");
  if(DIRSOL == ON) 
      fprintf(fp, " == ON (do the whole calculation directly)\n");
  else fprintf(fp, " == OFF (do the calculation iteratively)\n");

  fprintf(fp, "   EXPGCR");
  if(EXPGCR == ON) 
      fprintf(fp, " == ON (do all P*q's explicitly w/full matrix)\n");
  else fprintf(fp, " == OFF (do P*q's with multipole)\n");

  fprintf(fp, "   MAXITER");
  if(MAXITER < 0) {
    fprintf(fp, " == size (for n panel system, do at most n iterations)\n");
  }
  else fprintf(fp, " == %d (stop after %d iterations if not converged)\n", 
	  MAXITER, MAXITER);

  fprintf(fp, "   EXRTSH");
  fprintf(fp, 
	  " == %g (exact/ttl cubes > %g on lowest level => stop refinement)\n",
	  EXRTSH, EXRTSH);
}


/*
  pads a string on the right up to a given length, truncates if too long
*/
char *padName(tostr, frstr, len)
char *tostr, *frstr;
int len;
{
  int i;

  for(i = 0; frstr[i] != '\0'; i++) tostr[i] = frstr[i];
  if(i > len) tostr[len] = '\0';		/* truncate */
  else {			/* pad */
    for(; i < len; i++) tostr[i] = ' ';
    tostr[len] = '\0';
  }
  return(tostr);
}

/*
  returns a string of spaces (doesn't stdio have this somewhere?)
*/
char *spaces(str, num)
char *str;
int num;
{
  int i;

  for(i = 0; i < num; i++) str[i] = ' ';
  str[num] = '\0';
  return(str);
}
    
/*
  prints the capacitance matrix with symmetrized (averaged) off-diagonals
  - mks units (Farads) are used
  - some attempt to scale (eg pF, nF, uF etc) is made
  - also checks for M-matrix sign pattern, diag dominance
*/
void mksCapDump(capmat, numconds, relperm, name_list)
double **capmat, relperm;
int numconds;
Name **name_list;
{
  int i, j, toobig, toosmall, maxlen, sigfig, colwidth, i_killed, j_killed;
  int first_offd;
  double maxdiag = 0.0, minoffd, rowttl, scale = 1.0, **sym_mat;
  double mat_entry;
  char unit[BUFSIZ], name[BUFSIZ], *padName(), *spaces(), cond_name[BUFSIZ];
  char *getConductorName();
  extern NAME *start_name;	/* NAME structs giving conductor names */
  extern ITER *kill_num_list, *kinp_num_list;
  extern double iter_tol;
  // Enrico
  int icp, jcp;

  first_offd = TRUE;
  minoffd = capmat[1][1];	/* this entry is always present */
				/* - in the 1 cond case, assign is still ok */

  /* set up symetrized matrix storage */
  CALLOC(sym_mat, numconds+1, double*, ON, AMSC);
  for(i=1; i <= numconds; i++)  {
    CALLOC(sym_mat[i], numconds+1, double, ON, AMSC);
  }

  /* get the smallest and largest (absolute value) symmetrized elements */
  /* check for non-M-matrix symmetrized capacitance matrix */
  for(i = 1; i <= numconds; i++) {

    /* skip conductors removed from input */
    if(want_this_iter(kinp_num_list, i)) continue;

    i_killed = want_this_iter(kill_num_list, i);

    if(capmat[i][i] <= 0.0 && !i_killed) {
      viewprintf(stderr, "\nmksCapDump: Warning - capacitance matrix has non-positive diagonal\n  row %d\n", i+1);
    }
    maxdiag = MAX(maxdiag, fabs(capmat[i][i]));
    rowttl = 0.0;
    for(j = 1; j <= numconds; j++) {

      /* skip conductors removed from input */
      if(want_this_iter(kinp_num_list, j)) continue;

      if(j == i) {
	    sym_mat[i][i] = capmat[i][i];
	    continue;
      }

      /* if this column was not calculated and neither was the column
         with the same number as the current row, then symetrized mat has
	     no entry at [i][j], [j][i] */
      j_killed = want_this_iter(kill_num_list, j);
      if(i_killed && j_killed) continue;

      /* if this column was calculated but column with the same number
         as the current row wasnt, then symmetrized mat has unaveraged entry 
	     at [i][j], [j][i] */
      else if(i_killed && !j_killed) mat_entry = capmat[i][j];

      /* if this column was not calculated but column with the same number
         as the current row was, then symmetrized mat has unaveraged entry 
	 at [i][j], [j][i] */
      else if(!i_killed && j_killed) mat_entry = capmat[j][i];

      /* if this column was calculated and column with the same number
         as the current row was also, then symmetrized mat has averaged entry 
	     at [i][j], [j][i] */
      else mat_entry = (capmat[i][j] + capmat[j][i])/2.0;

      rowttl += mat_entry;

      if(mat_entry >= 0.0) {
	    viewprintf(stderr, "\nmksCapDump: Warning - capacitance matrix has non-negative off-diagonals\n  row %d col %d\n", i, j);
      }
      if(fabs(mat_entry) != 0.0) {
	    if(first_offd) {
	      minoffd = fabs(mat_entry);
	      first_offd = FALSE;
        }
	    else minoffd = MIN(minoffd, fabs(mat_entry));
      }

      sym_mat[i][j] = mat_entry;
    }
    if(rowttl + capmat[i][i] <= 0.0 && !i_killed) {
      viewprintf(stderr, "\nmksCapDump: Warning - capacitance matrix is not strictly diagonally dominant\n  due to row %d\n", i);
    }
  }

  /* figure the units to use for the matrix entries 
     - set up so smallest element is between 0.1 and 10 */
  if(minoffd*FPIEPS*relperm*scale > 10.0) toobig = TRUE;
  else toobig = FALSE;
  if(minoffd*FPIEPS*relperm*scale < 0.1) toosmall = TRUE;
  else toosmall = FALSE;
  while(toobig == TRUE || toosmall == TRUE) {
    if(toobig == TRUE) {
      scale *= 1e-3;
      if(minoffd*FPIEPS*relperm*scale <= 10.0) break;
    }
    if(toosmall == TRUE) {
      scale *= 1e+3;
      if(minoffd*FPIEPS*relperm*scale >= 0.1) break;
    }
  }

  /* get the appropriate unit string */
  if(scale == 1e-18) strcpy(unit, "exa");
  else if(scale == 1e-15) strcpy(unit, "peta");
  else if(scale == 1e-12) strcpy(unit, "tera");
  else if(scale == 1e-9) strcpy(unit, "giga");
  else if(scale == 1e-6) strcpy(unit, "mega");
  else if(scale == 1e-3) strcpy(unit, "kilo");
  else if(scale == 1.0) strcpy(unit, "");
  else if(scale == 1e+3) strcpy(unit, "milli");
  else if(scale == 1e+6) strcpy(unit, "micro");
  else if(scale == 1e+9) strcpy(unit, "nano");
  else if(scale == 1e+12) strcpy(unit, "pico");
  else if(scale == 1e+15) strcpy(unit, "femto");
  else if(scale == 1e+18) strcpy(unit, "atto");
  else sprintf(unit, "every unit is %g ", 1/scale);

  /* get the length of the longest name */
  maxlen = 0;
  for(i = 1; i <= numconds; i++) {
    maxlen = (int) MAX(strlen(getConductorName(i, name_list)), maxlen);
  }

  /* print the matrix */

  sigfig = (int) (2+log10(1.0/iter_tol));	/* get no. significant figs to prnt */
  colwidth = sigfig+6;		/* field width for cap mat columns */

  if(ITRDAT == OFF) 
    viewprintf(stdout, "\n");

  if(kill_num_list != NULL) 
    viewprintf(stdout, "\nPARTIAL CAPACITANCE MATRIX, %sfarads\n", unit);
  else 
    viewprintf(stdout, "\nCAPACITANCE MATRIX, %sfarads\n", unit);
 
 if(numconds < 10) 
    viewprintf(stdout, "%s", spaces(unit, maxlen+2));
  else if(numconds < 100) 
    viewprintf(stdout, "%s", spaces(unit, maxlen+3));
  else 
    viewprintf(stdout, "%s", spaces(unit, maxlen+4));

  for(j = 1; j <= numconds; j++) { /* column headings */
    if(want_this_iter(kinp_num_list, j)) continue;
    sprintf(name, "%d ", j);
    sprintf(unit, "%%%ds", colwidth+1);
    viewprintf(stdout, unit, name);
  }
  viewprintf(stdout, "\n");

  for(i = 1; i <= numconds; i++) { /* rows */

    /* skip conductors removed from input */
    if(want_this_iter(kinp_num_list, i)) continue;

    // Enrico, count number of actual items in the matrix
    strctCapMatrix.m_lDim++;
 
    sprintf(unit, "%d", i);

    strcpy(cond_name, getConductorName(i, name_list));

    if(numconds < 10)
	  viewprintf(stdout, "%s %1s", padName(name, cond_name, maxlen), unit);
    else if(numconds < 100)
	  viewprintf(stdout, "%s %2s", padName(name, cond_name, maxlen), unit);
    else
	  viewprintf(stdout, "%s %3s", padName(name, cond_name, maxlen), unit);

    for(j = 1; j <= numconds; j++) {

      /* skip conductors removed from input */
      if(want_this_iter(kinp_num_list, j)) continue;

      if(want_this_iter(kill_num_list, i) 
	     && want_this_iter(kill_num_list, j)) {
	    /* print a blank if capacitance was not calculated */
	    viewprintf(stdout, "%s", spaces(unit, colwidth+1));
      }
      else {
	    sprintf(unit, " %%%d.%dg", colwidth, sigfig);
	    viewprintf(stdout, unit, scale*FPIEPS*relperm*sym_mat[j][i]);
      }
    }
    viewprintf(stdout, "\n");
  }

  //
  // Enrico, copy data into structure to pass results back to main process
  // 
  // allocate capacitance matrix
  strctCapMatrix.m_daCapMatrix = malloc(sizeof(double*) * strctCapMatrix.m_lDim);
  for(i=0; i < strctCapMatrix.m_lDim; i++)  {
      strctCapMatrix.m_daCapMatrix[i] = malloc(sizeof(double) * strctCapMatrix.m_lDim);
  }
  // allocate list of names
  strctCapMatrix.m_sCondNames = malloc(sizeof(char*) * strctCapMatrix.m_lDim);
  for(i=0; i < strctCapMatrix.m_lDim; i++)  {
      strctCapMatrix.m_sCondNames[i] = malloc(sizeof(char) * FC_MAX_NAME_LEN);
  }
  // fill structure with data
  for(i=1, icp=0; i<=numconds; i++) { /* rows */

    /* skip conductors removed from input */
    if(want_this_iter(kinp_num_list, i)) continue;

    // store conductor name
    strncpy(strctCapMatrix.m_sCondNames[icp], getConductorName(i, name_list), FC_MAX_NAME_LEN);
	strctCapMatrix.m_sCondNames[icp][FC_MAX_NAME_LEN-1] = '\0';

    for(j=1, jcp=0; j<=numconds; j++) {

      /* skip conductors removed from input */
      if(want_this_iter(kinp_num_list, j)) continue;

      if(want_this_iter(kill_num_list, i) 
	     && want_this_iter(kill_num_list, j)) {
	    /* store a zero if capacitance was not calculated */
        // (it's not really correct; should warn the user someway)
	    strctCapMatrix.m_daCapMatrix[jcp][icp] = 0.0;
      }
      else {
	    strctCapMatrix.m_daCapMatrix[jcp][icp] = FPIEPS*relperm*sym_mat[j][i];
      }
      jcp++;
    }
    icp++;
  }

}

/*
  dumps brief information about multipole set up
*/
void dumpMulSet(sy, numLev, order)
ssystem *sy;
int numLev, order;
{
  int numcubes, numsides, i;

  for(numcubes = 1, i = 0; i < numLev; numcubes *= 8, i++);
  for(numsides = 1, i = 0; i < numLev; numsides *= 2, i++);

  viewprintf(stdout, "\nMULTIPOLE SETUP SUMMARY\n");
  viewprintf(stdout, "Level 0 cube extremal coordinates\n");
  viewprintf(stdout, "  x: %g to %g\n", 
	  sy->minx, sy->minx + numsides * (sy->length));
  viewprintf(stdout, "  y: %g to %g\n", 
	  sy->miny, sy->miny + numsides * (sy->length));
  viewprintf(stdout, "  z: %g to %g\n", 
	  sy->minz, sy->minz + numsides * (sy->length));
  viewprintf(stdout, "Level %d (lowest level) cubes\n  %d total\n", 
	  numLev, numcubes);
  viewprintf(stdout, 
	  "  side length = %g\n  maximum number of panels in each = %d\n",
	  sy->length, sy->mul_maxlq);
  viewprintf(stdout, "  maximum number of evaluation points in each = %d\n",
	  sy->loc_maxlq);
  viewprintf(stdout, 
	  "Maximum number of panels treated with a multipole expansion = %d\n",
	  sy->max_panel);
  viewprintf(stdout, 
  "Maximum number of evaluation points treated with a local expansion = %d\n",
	  sy->max_eval_pnt);
  viewprintf(stdout, 
	  "Maximum number of panels treated exactly = %d (limit = %d)\n",
	  sy->mul_maxq, multerms(order));
  viewprintf(stdout, 
   "Maximum number of evaluation points treated exactly = %d (limit = %d)\n",
	  sy->loc_maxq, multerms(order));
}

/*
  dump all the preconditioner matrices in the direct list cubes as one
  big matrix for matlab read in "Ctil"
  - figures preconditioner by multiplying it by columns of the inverse
    and dumping results one column at a time
  - figures the unpreconditioned matrix (using calcp) and dumps it to "P"
  - type determines which of the matrices to dump
*/
void dump_preconditioner(sys, chglist, type)
charge *chglist;
ssystem *sys;
int type;			/* 1=>dump P and C; 2=>P only; 3=>C only */
{
  int num_panels, i, j;
  charge *pp, *pi;
  double calcp();
  FILE *fp;

  /* find the number of panels */
  for(num_panels = 0, pp = chglist; pp != NULL; pp = pp->next, num_panels++);

  /* open the output file */
  if((fp = fopen("prec.mat","w")) == NULL) {
    viewprintf(stderr, "dump_preconditioner: can't open `prec.mat'\n");
    FCExit(FC_GENERIC_ERROR);
  }
  
  if(type == 1 || type == 3) {
    viewprintf(stdout, "\nDumping preconditioner to `prec.mat' as `Ctil'\n");
    /* dump the preconditioner one column at a time */
    /* - savemat arg "type" can be used to make rowwise dumps
         type = x0xx  =>  columnwise dumps 
         type = x1xx  =>  rowwise dumps (see matlab manual) */
    for(j = 1; j < num_panels+1; j++) {
      /* load I col into q and zero p */
      for(i = 0; i < num_panels+1; i++) {
	if(i == j) sys->q[i] = 1.0;
	else sys->q[i] = 0.0;
      }
      /* figure the column of C in p (xfered to q after calculation) */
      mulPrecond(sys, PRECOND);
      /* dump the preconditioner column */
      if(j == 1) savemat_mod(fp, 1000, "Ctil", num_panels, num_panels, 0, 
			     &(sys->q[1]), (double *)NULL, 0, num_panels);
      else savemat_mod(fp, 1000, "Ctil", num_panels, num_panels, 0, 
		       &(sys->q[1]), (double *)NULL, 1, num_panels);

    }
  }

  if(type == 1 || type == 2) {
    viewprintf(stdout, "\nDumping pot. coeff. mat. to `prec.mat' as `P'\n");
    /* dump the P matrix - DANGER: does n^2 calcp() calls,
       but storage is only n */
    /* are q indices from 1?? */
    for(j = 1; j < num_panels+1; j++) {
      /* find the panel with the current index (select column) */
      for(pp = chglist; pp != NULL; pp = pp->next) {
	if(pp->index == j) break;
      }
      if(pp == NULL) {
	viewprintf(stderr, "dump_preconditioner: no charge with index %d\n", j);
	FCExit(FC_GENERIC_ERROR);
      }
      for(i = 0; i < num_panels+1; i++) sys->p[i] = 0.0;
      /* find the column---influence of q_j on potentials at each q_i */
      for(i = 1, pi = chglist; pi != NULL; i++, pi = pi->next) {
	sys->p[pi->index] = calcp(pp, pi->x, pi->y, pi->z, NULL);
      }
      /* dump the column */
      if(j == 1) savemat_mod(fp, 1000, "P", num_panels, num_panels, 0, 
			     &(sys->p[1]), (double *)NULL, 0, num_panels);
      else savemat_mod(fp, 1000, "P", num_panels, num_panels, 0, 
		       &(sys->p[1]), (double *)NULL, 1, num_panels);

    }
  }

  fclose(fp);
}

/*
  do an n^2/2 check to see if any panels have the same center points
  (debug only)
*/
int has_duplicate_panels(fp, chglst)
charge *chglst;
FILE *fp;
{
  int no_duplicates;
  charge *cp, *cpinner;

  no_duplicates = TRUE;
  for(cp = chglst; cp != NULL; cp = cp->next) {
    for(cpinner = cp->next; cpinner != NULL; cpinner = cpinner->next) {
      if(cp->x == cpinner->x && cp->y == cpinner->y && cp->z == cpinner->z) {
	no_duplicates = FALSE;

	if(cp->surf->type == CONDTR) fprintf(fp, "Panels %d(CONDTR)",
						cp->index);
	if(cp->surf->type == DIELEC) fprintf(fp, "Panels %d(DIELEC)",
						cp->index);
	if(cp->surf->type == BOTH) fprintf(fp, "Panels %d(BOTH)",
					      cp->index);

	if(cpinner->surf->type == CONDTR) fprintf(fp, " and %d(CONDTR)",
						cpinner->index);
	if(cpinner->surf->type == DIELEC) fprintf(fp, " and %d(DIELEC)",
						cpinner->index);
	if(cpinner->surf->type == BOTH) fprintf(fp, " and %d(BOTH)",
					      cpinner->index);

	fprintf(fp, " both have center (%.3g %.3g %.3g)\n",
		cp->x, cp->y, cp->z);
      }
    }
  }

  if(no_duplicates) return(FALSE);
  else return(TRUE);
}

#if DSQ2PD == ON
/*
  dump the condensed matrix for matlab use
*/
void dumpQ2PDiag(nextc)
cube *nextc;
{
  int i, j, ind, pos_d, neg_d;
  double temp[BUFSIZ], temp_mat[100][100], **rmat;
  double pos_fact, neg_fact, panel_fact;
  FILE *fp, *fopen();

  /* dump the diag matrix to a matlab file along with its dummy vector */
  /*   must complie on sobolev with /usr/local/matlab/loadsave/savemat.o */
  if((fp = fopen("Q2PDiag.mat", "w")) == NULL) {
    viewprintf(stderr, "dumpQ2PDiag: can't open `Q2PDiag.mat' to write\n");
    FCExit(FC_GENERIC_ERROR);
  }
  if(sizeof(temp) < nextc->upnumeles[0]*nextc->upnumeles[0]) {
    viewprintf(stderr, "dumpQ2PDiag: temporary arrays not big enough\n");
    FCExit(FC_GENERIC_ERROR);
  }

  /* incorporate the electric field evaluation into the matrix */
  /* if a row corresponds to a flux density difference, alter it */
  rmat = nextc->directmats[0];
  for(i = 0; i < nextc->upnumeles[0]; i++) {
    if(nextc->chgs[i]->dummy) {
      for(j = 0; j < nextc->upnumeles[0]; j++) temp_mat[i][j] = 0.0;
      continue;
    }

    if(nextc->chgs[i]->surf->type == CONDTR) {
      for(j = 0; j < nextc->upnumeles[0]; j++) {
	temp_mat[i][j] = rmat[i][j];
      }
    }
    else {

      pos_fact 
	  = nextc->chgs[i]->surf->outer_perm/nextc->chgs[i]->pos_dummy->area;
      pos_d = nextc->chgs[i]->pos_dummy->index - 1;
      neg_fact 
	  = nextc->chgs[i]->surf->inner_perm/nextc->chgs[i]->neg_dummy->area;
      neg_d = nextc->chgs[i]->neg_dummy->index - 1;
      panel_fact = pos_fact + neg_fact;

      for(j = 0; j < nextc->upnumeles[0]; j++) {
	temp_mat[i][j] = pos_fact*rmat[pos_d][j] - panel_fact*rmat[i][j]
	    + neg_fact*rmat[neg_d][j];
      }
    }
  }
  
  /* flatten the Q2PDiag matrix */
  for(j = ind = 0; j < nextc->upnumeles[0]; j++) {
    for(i = 0; i < nextc->upnumeles[0]; i++) {
      temp[ind++] = temp_mat[i][j];
    }
  }
  savemat(fp, 1000, "A", nextc->upnumeles[0], nextc->upnumeles[0],
	  0, temp, (double *)NULL);

  /* make the is_dummy vector a vector of doubles */
  for(i = 0; i < nextc->upnumeles[0]; i++) 
      temp[i] = (double)(nextc->nbr_is_dummy[0][i]);
  savemat(fp, 1000, "is_dummy", nextc->upnumeles[0], 1,
	  0, temp, (double *)NULL);

  /* make a vector with 0 => CONDTR 1 => DIELEC 2 => BOTH -1 => dummy */
  for(i = 0; i < nextc->upnumeles[0]; i++) {
    if(nextc->chgs[i]->dummy) temp[i] = -1.0;
    else temp[i] = (double)(nextc->chgs[i]->surf->type);
  }
  savemat(fp, 1000, "surf_type", nextc->upnumeles[0], 1,
	  0, temp, (double *)NULL);

  fclose(fp);
  viewprintf(stdout, "Dumped Q2PDiag matrix to `Q2PDiag.mat'\n");
}

#endif

#if 1 == 0
/*
  for debug only on SPARC2 -- NaN trap error handler (see man matherr)
*/
int matherr(exc)
struct exception *exc;
{
  viewprintf(stderr, "matherr: ");

  if(exc->type == DOMAIN) viewprintf(stderr, "DOMAIN error in");
  else if(exc->type == SING) viewprintf(stderr, "SING error in");
  else if(exc->type == OVERFLOW) viewprintf(stderr, "OVERFLOW error in");
  else if(exc->type == UNDERFLOW) viewprintf(stderr, "UNDERFLOW error in");

  viewprintf(stderr, " %s\n", exc->name);

  viewprintf(stderr, " args: %g %g returning: %g\n", exc->arg1, exc->arg2,
	  exc->retval);
  FCExit(FC_GENERIC_ERROR);
}
#endif

/*
  debug only - check a vector to make sure it has zeros in dummy entries
*/
void chkDummy(vector, is_dummy, size)
double *vector;
int *is_dummy;
int size;
{
  int i, first = TRUE;

  for(i = 0; i < size; i++) {
    if(is_dummy[i] && vector[i] != 0.0) {
      if(first) {
	first = FALSE;
	viewprintf(stderr, "\nchkDummy: entries should be 0.0: %d", i);
      }
      else viewprintf(stderr, " %d", i);
    }
  }
  if(!first) viewprintf(stderr, "\n");
}

/*
  debug only - print message if dummy list doesn't match panel list
*/
void chkDummyList(panels, is_dummy, n_chgs)
charge **panels;
int *is_dummy;
int n_chgs;
{
  int i;
  int first = TRUE;
  
  for(i = 0; i < n_chgs; i++) {
    if(is_dummy[i] && !panels[i]->dummy || !is_dummy[i] && panels[i]->dummy) {
      if(first) {
	first = FALSE;
	viewprintf(stderr, "chkDummyList: inconsistent dummy list entries:\n");
      }
      viewprintf(stderr, " %d is_dummy = %d, panel->dummy = %d\n", i,
	      is_dummy[i], panels[i]->dummy);
    }
  }

}

/*
  print the conductor names to a file
*/
void dumpCondNames(fp, name_list)
FILE *fp;
Name *name_list;
{ 
  int i;
  char *last_alias();
  Name *cur_name;

  fprintf(fp, "CONDUCTOR NAMES\n");
  for(cur_name = name_list, i = 0; cur_name != NULL; 
      cur_name = cur_name->next, i++) {
    fprintf(fp, "  %d `%s'\n", i+1, last_alias(cur_name));
  }
}

/*
  debug only: dump state of internal conductor name list
*/
int dumpNameList(name_list)
Name *name_list;
{
  Name *cur_name, *cur_alias;

  for(cur_name = name_list; cur_name != NULL; cur_name = cur_name->next) {
    viewprintf(stdout, "`%s'\n", cur_name->name);
    for(cur_alias = cur_name->alias_list; cur_alias != NULL; 
	cur_alias = cur_alias->next) {
      viewprintf(stdout, "  `%s'\n", cur_alias->name);
    }
  }
  return(TRUE);
}

