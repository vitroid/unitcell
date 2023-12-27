#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#define MAXCELL 31
extern double rint();

int map[MAXCELL][MAXCELL][MAXCELL];
double bx,by,bz;
double rix,riy,riz;
double rjx,rjy,rjz;
double rkx,rky,rkz;
double rri,rrj,rrk;
double ri,rj,rk;
double* x;
double* y;
double* z;
char** residual;

int n;
int ix,iy,iz,jx,jy,jz,kx,ky,kz;
int yap=0;
int cells=0;
int count=0;

void occupycell(int nx,int ny,int nz)
{
  int di,dj,dk;
  if(map[nx][ny][nz]==0){
    map[nx][ny][nz]=1;
    for(di=-1;di<=1;di++){
      int vix,viy,viz;
      vix=ix*di;
      viy=iy*di;
      viz=iz*di;
      for(dj=-1;dj<=1;dj++){
	int vjx,vjy,vjz;
	vjx=jx*dj;
	vjy=jy*dj;
	vjz=jz*dj;
	for(dk=-1;dk<=1;dk++){
	  int vkx,vky,vkz;
	  vkx=kx*dk+vix+vjx+nx;
	  vky=ky*dk+viy+vjy+ny;
	  vkz=kz*dk+viz+vjz+nz;
	  if((vkx>=0)&&(vkx<MAXCELL)&&
	     (vky>=0)&&(vky<MAXCELL)&&
	     (vkz>=0)&&(vkz<MAXCELL)){
	    occupycell(vkx,vky,vkz);
	  }
	}
      }
    }
  }
}

void FindEmptyCell(int nx,int ny,int nz)
{
  int xi,yi,zi;
  int i;
  if(map[nx][ny][nz]==0){
    cells++;
    fprintf(stderr,"%d %d %d\n",nx,ny,nz);
    for(i=0;i<n;i++){
      double xx,yy,zz;
      double p0i,p0j,p0k;
      double pi,pj,pk;
      xx = x[i]+nx*bx;
      yy = y[i]+ny*by;
      zz = z[i]+nz*bz;
      p0i = xx*rix+yy*riy+zz*riz;
      p0j = xx*rjx+yy*rjy+zz*rjz;
      p0k = xx*rkx+yy*rky+zz*rkz;
      pi = p0i/rri;
      pj = p0j/rrj;
      pk = p0k/rrk;
      pi -= rint(pi);
      pj -= rint(pj);
      pk -= rint(pk);
      if(yap)
	printf("t %f %f %f %d\n",pi*ri,pj*rj,pk*rk,count++);
      else
	printf("%f %f %f %s",pi*ri,pj*rj,pk*rk,residual[i]);
    }
    occupycell(nx,ny,nz);
    for(xi=nx-1;xi<=nx+1;xi++){
      if(xi>=0&&xi<MAXCELL){
	for(yi=ny-1;yi<=ny+1;yi++){
	  if(yi>=0&&yi<MAXCELL){
	    for(zi=nz-1;zi<=nz+1;zi++){
	      if(zi>=0&&zi<MAXCELL){
		FindEmptyCell(xi,yi,zi);
	      }
	    }
	  }
	}
      }
    }
  }
}

int main(int argc,char *argv[])
{
  char buf[1000];
  char tag[1000];
  int i;
  double rih,rjh,rkh;
  int nx,ny;
  int ii,jj,kk;
  //int count;
  int vol;
  int vx,vy,vz;

  extern char *optarg;
  extern int optind,optopt;
  int errflg=0;
  char c;
  
  while((c = getopt(argc,argv,":y"))!=-1)
    switch(c)
      {
      case 'y':
	yap++;
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
  if((argc-optind)!=9){
    errflg++;
  }else{
    ix=atoi(argv[optind++]);
    iy=atoi(argv[optind++]);
    iz=atoi(argv[optind++]);
    jx=atoi(argv[optind++]);
    jy=atoi(argv[optind++]);
    jz=atoi(argv[optind++]);
    kx=atoi(argv[optind++]);
    ky=atoi(argv[optind++]);
    kz=atoi(argv[optind++]);
  }
  if(errflg){
    fprintf(stderr,"usage : %s [-y] [--] ix iy iz jx jy jz kx ky kz\n",argv[0]);
    fprintf(stderr,"-y: Yaplot style output\n");
    fprintf(stderr,"--: Necessary if ix is minus.\n");
    fprintf(stderr,"ix,iy,iz...: Direction of i-, j-, and k-vectors calculated by unitcell command.\n");
    
    exit(1);
  }
  
  while(NULL!=fgets(buf,sizeof(buf),stdin)){
    if(0==strncmp(buf,"@BOX3",5)){
      fgets(buf,sizeof(buf),stdin);
      sscanf(buf,"%lf %lf %lf\n",&bx,&by,&bz);
    }
    if((0==strncmp(buf,"@NX3A",5))||(0==strncmp(buf,"@NX4A",5))||(0==strncmp(buf,"@AR3A",5))){
      strcpy(tag,buf);
      fgets(buf,sizeof(buf),stdin);
      sscanf(buf,"%d",&n);
      x = malloc( sizeof(double) * n );
      y = malloc( sizeof(double) * n );
      z = malloc( sizeof(double) * n );
      residual = malloc( sizeof( char * ) * n );
      for(i=0;i<n;i++){
	char *b,*c=NULL;
	fgets(buf,sizeof(buf),stdin);
	b=buf;
	c=strsep(&b," \t");
	while((*c=='\0')&&(b!=NULL))
	  c=strsep(&b," \t");
	x[i]=atof(c);
	c=strsep(&b," \t");
	while((*c=='\0')&&(b!=NULL))
	  c=strsep(&b," \t");
	y[i]=atof(c);
	c=strsep(&b," \t");
	while((*c=='\0')&&(b!=NULL))
	  c=strsep(&b," \t");
	z[i]=atof(c);
	if(b!=NULL){
            residual[i] = malloc( strlen( b ) + 1 );
            strcpy(residual[i],b);
        }
	else {
            residual[i] = malloc( 2 );
            strcpy(residual[i],"\n");
        }
      }
    }
  }
  /*与えられた3つのベクトルの張る空間に、単位格子の座標を射影すればいい。*/
  /*実ベクトルで計算するのではなく、tripletで計算する。*/
  ii = ix*ix+iy*iy+iz*iz;
  jj = jx*jx+jy*jy+jz*jz;
  kk = kx*kx+ky*ky+kz*kz;
  
  rix = ix*bx;
  riy = iy*by;
  riz = iz*bz;
  rri = (rix*rix+riy*riy+riz*riz);
  ri = sqrt(rri);
  rih=ri*0.5;
  
  rjx = jx*bx;
  rjy = jy*by;
  rjz = jz*bz;
  rrj = (rjx*rjx+rjy*rjy+rjz*rjz);
  rj = sqrt(rrj);
  rjh=rj*0.5;

  rkx = kx*bx;
  rky = ky*by;
  rkz = kz*bz;
  rrk = (rkx*rkx+rky*rky+rkz*rkz);
  rk = sqrt(rrk);
  rkh=rk*0.5;

  vx=iy*jz-iz*jy;
  vy=iz*jx-ix*jz;
  vz=ix*jy-iy*jx;
  vol=abs(vx*kx+vy*ky+vz*kz);
  if(yap){
    printf("@ 2\nr 0.1\n");
    for(nx=-1;nx<2;nx+=2){
      for(ny=-1;ny<2;ny+=2){
	printf("l %f %f %f %f %f %f\n",nx*rih,ny*rjh,-rkh,nx*rih,ny*rjh,rkh);
	printf("l %f %f %f %f %f %f\n",ny*rih,-rjh,nx*rkh,ny*rih,rjh,nx*rkh);
	printf("l %f %f %f %f %f %f\n",-rih,nx*rjh,ny*rkh,rih,nx*rjh,ny*rkh);
      }
    }
    printf("l 0 0 0  %f %f %f\n",bx*rix/ri,bx*rjx/rj,bx*rkx/rk);
    printf("l 0 0 0  %f %f %f\n",by*riy/ri,by*rjy/rj,by*rky/rk);
    printf("l 0 0 0  %f %f %f\n",bz*riz/ri,bz*rjz/rj,bz*rkz/rk);
    printf("t %f %f %f x\n",bx*rix/ri,bx*rjx/rj,bx*rkx/rk);
    printf("t %f %f %f y\n",by*riy/ri,by*rjy/rj,by*rky/rk);
    printf("t %f %f %f z\n",bz*riz/ri,bz*rjz/rj,bz*rkz/rk);
  }else{
    printf("@BOX3\n%f %f %f\n",ri,rj,rk);
    printf("%s%d\n",tag,n*vol);
  }
  
  FindEmptyCell(0,0,0);
  if(vol!=cells){
    fprintf(stderr,"Internal error.\n");
    exit(1);
  }
  /*  gauss(g);*/
  /*3次元のエラトステネスの篩を作る。*/

  for( i=0; i<n; i++ ){
      free(residual[i] );
  }
  free(residual);
  free( x );
  free( y );
  free( z );
  
  exit(0);
}
