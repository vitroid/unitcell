/*   与えられた直方単位格子を並べて、傾いた格子に再編する。*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

extern double rint();

int stackx[1000],stacky[1000],stackz[1000];
double stackt[1000],stackr[1000];
int nstack=0;

void push(int jx,int jy,int jz,double t,double rj)
{
  /*既存のベクトルと素かどうかをチェックしろ*/
  int i,nj;
  nj = jx*jx+jy*jy+jz*jz;
  for(i=0;i<nstack;i++){
    int n,ni;
    int ix,iy,iz;
    ix = stackx[i];
    iy = stacky[i];
    iz = stackz[i];
    ni = ix*ix+iy*iy+iz*iz;
    n = stackx[i]*jx + stacky[i]*jy + stackz[i]*jz;
    if(n*n==ni*nj){
      if(nj<ni){
	/*printf("reduce (%d)%d %d %d -> %d %d %d\n",i,ix,iy,iz,jx,jy,jz);*/
	stackx[i]=jx;
	stacky[i]=jy;
	stackz[i]=jz;
	stackt[i]=t;
	stackr[i]=rj;
      }
      return;
    }
  }
  stackx[nstack]=jx;
  stacky[nstack]=jy;
  stackz[nstack]=jz;
  stackt[nstack]=t;
  stackr[nstack]=rj;
  nstack++;
}


int main(int argc,char *argv[])
{
  double bx,by,bz;
  int ix,iy,iz;
  int jx,jy,jz;
  double rix,riy,riz;
  double rjx,rjy,rjz;
  double rkx,rky,rkz;
  int range=10;
  double tolerance=0.02;
  double ri;
  int j,k;
  double tj,tk,rj,rk;
  extern char *optarg;
  extern int optind,optopt;
  int errflg=0;
  char c;
  
  while((c = getopt(argc,argv,":t:"))!=-1)
    switch(c)
      {
      case 't':
	tolerance = atof(optarg);
	break;
      case '?':
	fprintf(stderr,"Unrecognized option: - %c\n",optopt);
	errflg++;
	break;
      case ':':
	fprintf(stderr,"Missing argument: - %c\n",optopt);
	errflg++;
	break;
      }
  if((argc-optind)!=6){
    errflg++;
  }else{
    bx = atof(argv[optind++]);
    by = atof(argv[optind++]);
    bz = atof(argv[optind++]);
    ix = atoi(argv[optind++]);
    iy = atoi(argv[optind++]);
    iz = atoi(argv[optind++]);
  }
  if(errflg){
    fprintf(stderr,"usage : %s [-t tolerance] xlen ylen zlen ix iy iz\n",argv[0]);
    fprintf(stderr,"tolerance: Maximum of inner product between two axes.(default=0.02)\n");
    fprintf(stderr,"xlen,ylen,zlen: Unit cell size.\n");
    fprintf(stderr,"ix,iy,iz: Direction of i-vector (j- and k-vectors will be calculated).\n");
    
    exit(1);
  }
    
  rix = ix*bx;
  riy = iy*by;
  riz = iz*bz;
  
  ri = sqrt(rix*rix+riy*riy+riz*riz);
  /*まず与えられたベクトルに直交するベクトルを列挙する。*/
  for(jx=-range;jx<=range;jx++){
    rjx=jx*bx;
    for(jy=-range;jy<=range;jy++){
      rjy=jy*by;
      for(jz=-range;jz<=range;jz++){
	rjz=jz*bz;
	rj = sqrt(rjx*rjx+rjy*rjy+rjz*rjz);
	if(rj!=0){
	  tj = fabs(rix*rjx+riy*rjy+riz*rjz)/(ri*rj);
	  if(tj < tolerance){
	    /*printf("%f %d %d %d %d\n",tj,nstack,jx,jy,jz);*/
	    push(jx,jy,jz,tj,rj);
	  }
	}
	
      }
    }
  }
  /*  {
    int i;
    for(i=0;i<nstack;i++){
      printf("%d %d %d %d\n",i,stackx[i],stacky[i],stackz[i]);
    }
  }*/
  
  
  /*  printf("%d\n",nstack);*/
  /*互いに直交する組合せを選ぶ。*/
  for(j=0;j<nstack;j++){
    rjx = stackx[j]*bx;
    rjy = stacky[j]*by;
    rjz = stackz[j]*bz;
    tj  = stackt[j];
    rj  = stackr[j];
    for(k=j+1;k<nstack;k++){
      double tjk;
      rkx = stackx[k]*bx;
      rky = stacky[k]*by;
      rkz = stackz[k]*bz;
      tk  = stackt[k];
      rk  = stackr[k];
      tjk = fabs(rkx*rjx+rky*rjy+rkz*rjz)/(rk*rj);
      /*printf("%f\n",tjk);*/
      if(tjk < tolerance){
	double x,y,z,v;
	x = rky*rjz - rkz*rjy;
	y = rkz*rjx - rkx*rjz;
	z = rkx*rjy - rky*rjx;
	v = fabs(x*rix+y*riy+z*riz);
	printf("%d %f %d %d %d %d %d %d %d %d %d\n"
	       ,(int)rint(v/(bx*by*bz))
	       ,v/(ri*rj*rk)
	       ,ix,iy,iz,
	       stackx[j],
	       stacky[j],
	       stackz[j],
	       stackx[k],
	       stacky[k],
	       stackz[k]);
      }
    }
  }
  exit(0);
}
